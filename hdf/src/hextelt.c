/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*LINTLIBRARY */
/* ------------------------------ hextelt.c -------------------------------

   Routines for external elements, i.e., data elements that reside on
   some other file.  These elements have no limits on their length.
   While users are prevented from reading beyond what is written, a
   user can write an unlimited amount of data.

   17-Mar-93
   Adding offset and "starting length" to elements so that a user can
   take an existing file with some data in it and create an HDF file
   which has a pointer to that data.

   File Organization
  ******************
   DD for External Element pointing to External Element Description Record
   =======================================================================
   <-  2 bytes -> <- 2 bytes -> <- 4 bytes -> <- 4bytes ->
   --------------------------------------------------------
   |extended tag | reference # |  Offset     |  Length    |
   --------------------------------------------------------
                                    \______________/
   __________________________________________|
   V
   EXTERNAL ELEMENT DESCRIPTION RECORD(EEDR - 12 + file_name_length bytes)
   ========================================================================
   <-  4 bytes  -> <- 4 bytes -> <- 4 bytes -> <- variable ->
   ---------------------------------------------------------
   | ext_tag_desc |   offset    |  length     | filename   |
   ---------------------------------------------------------

   ext_tag_desc  - EXT_EXTERN(16 bit constant). Identifies this as an
                   external element description record.
   offset        - Location of the element(data) within the external 
                   file(32 bit field)
   length        - Length in bytes of the element(data) in the external
                   file(32 bit field)
   filename      - Non-null terminated ASCII string naming the external
                   file(variable length)

 LOCAL ROUTINES
   None
 EXPORTED ROUTINES
   HXcreate         -- create an external element
   HXPsetaccesstype -- set the access type of the external file
   HXIstaccess      -- set up AID to access an ext elem
   HXPstread        -- open an access record for reading
   HXPstwrite       -- open an access record for reading
   HXPseek          -- set the seek position
   HXPread          -- read some data out of an external file
   HXPwrite         -- write some data out to an external file
   HXPinquire       -- retreive information about an external element
   HXPendacess      -- close file, free AID
   HXPcloseAID      -- close file but keep AID active
   HXPinfo          -- return info about an external element
   HXPreset         -- replace the current external info with new info
------------------------------------------------------------------------- */

#include "hdf.h"
#include "hfile.h"

/* extinfo_t -- external elt information structure */

typedef struct
  {
      int         attached;     /* number of access records attached
                                   to this information structure */
      int32       extern_offset;
      int32       length;       /* length of this element */
      int32       length_file_name;     /* length of the external file name */
      int32       para_extfile_id;  /* parallel ID of the external file */
      hdf_file_t  file_external;    /* external file descriptor */
      char       *extern_file_name;     /* name of the external file */
      intn        file_open;    /* has the file been opened yet ? */
  }
extinfo_t;

/* forward declaration of the functions provided in this module */
PRIVATE int32 HXIstaccess
            (accrec_t * access_rec, int16 access);
PRIVATE char *HXPbuildfilename
	(const char *ext_fname, const intn acc_mode);

/* ext_funcs -- table of the accessing functions of the external
   data element function modules.  The position of each function in
   the table is standard */
funclist_t  ext_funcs =
{
    HXPstread,
    HXPstwrite,
    HXPseek,
    HXPinquire,
    HXPread,
    HXPwrite,
    HXPendaccess,
    HXPinfo,
    HXPreset,
};

/*------------------------------------------------------------------------ 
NAME
   HXcreate -- create an external element
USAGE
   int32 HXcreate(file_id, tag, ref, ext_name, offset, len)
   int32  file_id;      IN: file ID for HDF file
   int16  tag;          IN: tag number for external elem
   int16  ref;          IN: ref number for external elem
   char * ext_name;     IN: external file name
   int32  offset;       IN: offset where elem should start in ext file
   int32  len;          IN: current len of element if already in
                            ext file (see desc below)
RETURNS
   returns AID to external element or FAIL
DESCRIPTION
   Create a data element in an external file.  If that 
   file already exists, we will simply *modify* that file, 
   not delete it and start over.  Offset and start_len 
   are for encapsulating data that already exists in a 
   seperate file so that it can be referenced from the HDF file.

   If the objext we are writing out already exists in an
   HDF file and is "promoted" then the start_len is ignored
   since we already know its current length.  However, offset
   is still respected

   Return an AID to the newly created external element, FAIL
   on error.

--------------------------------------------------------------------------*/
int32
HXcreate(int32 file_id, uint16 tag, uint16 ref, const char *extern_file_name, int32 offset, int32 start_len)
{
    CONSTR(FUNC, "HXcreate");   /* for HERROR */
    filerec_t  *file_rec;       /* file record */
    accrec_t   *access_rec;     /* access element record */
    int         slot;
    dd_t       *dd;
    ddblock_t  *data_block;     /* dd block ptr to exist data element */
    int32       data_idx;       /* dd list index to existing data element */
    hdf_file_t  file_external;  /* external file descriptor */
    extinfo_t  *info;           /* special element information */
    dd_t       *data_dd;        /* dd of existing regular element */
    uint16      special_tag;    /* special version of tag */
    uint8       local_ptbuf[14 + MAX_PATH_LEN];     /* temp working buffer */
    int32       file_off;       /* offset in the file we are at currently */
    char	*fname;		/* filename built from external filename */

    /* clear error stack and validate args */
    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec) || !extern_file_name || (offset < 0) || SPECIALTAG(tag)
        || (special_tag = MKSPECIALTAG(tag)) == DFTAG_NULL)
        HRETURN_ERROR(DFE_ARGS, FAIL);

    if (!(file_rec->access & DFACC_WRITE))
        HRETURN_ERROR(DFE_DENIED, FAIL);

    /* get a slot in the access records table */
    slot = HIget_access_slot();
    if (slot == FAIL)
        HRETURN_ERROR(DFE_TOOMANY, FAIL);
    access_rec = &access_records[slot];

    /* look for existing data element of the same tag/ref */
    if (FAIL != HIlookup_dd(file_rec, tag, ref, &data_block, &data_idx))
      {
	  data_dd = &(data_block->ddlist[data_idx]);
	  if (SPECIALTAG(data_dd->tag))
	    {

		/* abort since we cannot convert the data element to an external
		   data element */
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_CANTMOD, FAIL);
	    }
      }
    else
        data_dd = (dd_t *) NULL;

    /* look for empty dd to use */
    if (FAIL == HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD,
                            &file_rec->null_block, &file_rec->null_idx))
      {
          if (FAIL == HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC))
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_NOFREEDD, FAIL);
            }
          else
            {
                access_rec->block = file_rec->ddlast;
                access_rec->idx = 0;
            }
      }
    else
      {
          access_rec->block = file_rec->null_block;
          access_rec->idx = file_rec->null_idx;
      }
    dd = &access_rec->block->ddlist[access_rec->idx];

    /* build the customized external file name. */
    if (!(fname = HXPbuildfilename(extern_file_name, DFACC_CREATE)))
        HRETURN_ERROR(DFE_BADOPEN, FAIL);

    /* create the external file */
    file_external = HI_OPEN(fname, DFACC_WRITE);
    if (OPENERR(file_external))
    {
        file_external = HI_CREATE(fname);
        if (OPENERR(file_external))
	{
	    access_rec->used = FALSE;
	    HDfree(fname);
	    HRETURN_ERROR(DFE_BADOPEN, FAIL);
	}
    }
    HDfree(fname);

    /* set up the special element information and write it to file */
    access_rec->special_info = (VOIDP) HDmalloc((uint32) sizeof(extinfo_t));
    info = (extinfo_t *) access_rec->special_info;
    if (!info)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    if (data_dd)
      {
          VOIDP       buf;      /* temporary buffer */

#ifdef OLD_WAY
          buf = (VOIDP) HDmalloc((uint32) data_dd->length);
          if (!buf)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            }
          if (HI_SEEK(file_rec->file, data_dd->offset) == FAIL)
            {
                HDfree((VOIDP) info);
                HDfree((VOIDP) buf);
                HRETURN_ERROR(DFE_SEEKERROR, FAIL);
            }
          if (HI_READ(file_rec->file, buf, data_dd->length) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HDfree((VOIDP) buf);
                HRETURN_ERROR(DFE_READERROR, FAIL);
            }
#else  /* OLD_WAY */
          if ((buf = (VOIDP) HDmalloc((uint32) data_dd->length)) == NULL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            }
          if (Hgetelement(file_id, data_dd->tag, data_dd->ref, (VOIDP)buf) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HDfree((VOIDP) buf);
                HRETURN_ERROR(DFE_READERROR, FAIL);
            }
#endif /* OLD_WAY */
          if (HI_SEEK(file_external, offset) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HDfree((VOIDP) buf);
                HRETURN_ERROR(DFE_SEEKERROR, FAIL);
            }
          if (HI_WRITE(file_external, buf, data_dd->length) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HDfree((VOIDP) buf);
                HRETURN_ERROR(DFE_WRITEERROR, FAIL);
            }
          HDfree((VOIDP) buf);
          info->length = data_dd->length;

      }
    else
      {
          info->length = start_len;
      }

    info->attached         = 1;
    info->file_open        = TRUE;
    info->file_external    = file_external;
    info->extern_offset    = offset;
    info->extern_file_name = (char *) HDstrdup(extern_file_name);
    if (!info->extern_file_name)
      {
          access_rec->used = FALSE;
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    info->length_file_name = HDstrlen(extern_file_name);
    {
        uint8      *p = local_ptbuf;
        INT16ENCODE(p, SPECIAL_EXT);
        INT32ENCODE(p, info->length);
        INT32ENCODE(p, info->extern_offset);
        INT32ENCODE(p, info->length_file_name);
        HDstrcpy((char *) p, extern_file_name);
    }
    dd->ref         = ref;
    dd->tag         = special_tag;
    dd->length      = 14 + info->length_file_name;
    if ((dd->offset = HPgetdiskblock(file_rec, dd->length, TRUE)) == FAIL)
      {
          access_rec->used = FALSE;
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_INTERNAL, FAIL);
      }     /* end if */
    if (HI_WRITE(file_rec->file, local_ptbuf, dd->length) == FAIL)
      {
          access_rec->used = FALSE;
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_WRITEERROR, FAIL);
      }

    if (FAIL == HIupdate_dd(file_rec, access_rec->block, access_rec->idx, FUNC))
      {
          access_rec->used = FALSE;
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_CANTUPDATE, FAIL);
      }

    /* add new DD to hash table */
    if (FAIL == HIadd_hash_dd(file_rec, dd->tag, dd->ref, access_rec->block,
                              access_rec->idx))
      {
          access_rec->used = FALSE;
          HDfree((VOIDP) info);
          HRETURN_ERROR(DFE_CANTHASH, FAIL);
      }

    if (data_dd)
      {
          if (Hdeldd(file_id, data_dd->tag, data_dd->ref) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HRETURN_ERROR(DFE_CANTDELDD, FAIL);
            }   /* end if */
          if (HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref) == FAIL)
            {
                access_rec->used = FALSE;
                HDfree((VOIDP) info);
                HRETURN_ERROR(DFE_CANTDELHASH, FAIL);
            }   /* end if */
      }

    /* update access record and file record */
    access_rec->special_func = &ext_funcs;
    access_rec->special      = SPECIAL_EXT;
    access_rec->posn         = 0;
    access_rec->access       = DFACC_RDWR;
    access_rec->file_id      = file_id;
    access_rec->appendable   = FALSE;     /* start data as non-appendable */
    access_rec->flush        = FALSE;  /* start data as not needing flushing */

    file_rec->attach++;
    if (ref > file_rec->maxref)
        file_rec->maxref = ref;

    /* update end of file pointer? */
    file_off = HI_TELL(file_rec->file);
#ifdef TESTING
    printf("HXcreate: file_rec->f_end_off=%d\n",file_rec->f_end_off);
    printf("HXcreate: file_off=%d\n",file_off);
#endif
    if (file_off > file_rec->f_end_off)
      file_rec->f_end_off = file_off;

    return ASLOT2ID(slot);  /* return access id */
} /* HXcreate */

/*------------------------------------------------------------------------ 
NAME
   HXPsetaccesstype -- set the access type of the external file
USAGE
   intn HXPsetaccesstype(access_rec)
   accrec_t *access_rec   IN/OUT: access record of the external element
RETURNS
   SUCCEED if no error, else FAIL
DESCRIPTION
   Open the external file according to the access type specified.

--------------------------------------------------------------------------*/
intn
HXPsetaccesstype(accrec_t * access_rec)
{
    char       *FUNC = "HXPsetaccesstype";  /* for HERROR */
    hdf_file_t  file_external;  /* external file descriptor */
#ifdef CM5
    int32       para_extfile_id;    /* parallel external file id */
#endif /* CM5 */
    extinfo_t  *info;           /* special element information */
    char	*fname;

    /* clear error stack and validate args */
    HEclear();

    /* sanity check */
    if (!access_rec)
        HRETURN_ERROR(DFE_ARGS, FAIL);

    info = (extinfo_t *) access_rec->special_info;
    if (!info)
      {
          access_rec->used = FALSE;
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }

    /* build the customized external file name. */
    if (!(fname = HXPbuildfilename(info->extern_file_name, DFACC_OLD)))
        HRETURN_ERROR(DFE_BADOPEN, FAIL);

    /* Open the external file for the correct access type */
    switch (access_rec->access_type)
      {
          case DFACC_SERIAL:
              file_external = HI_OPEN(fname, DFACC_WRITE);
              if (OPENERR(file_external))
                {
                    file_external = HI_CREATE(fname);
                    if (OPENERR(file_external))
                      {
                          access_rec->used = FALSE;
			  HDfree(fname);
                          HRETURN_ERROR(DFE_BADOPEN, FAIL);
                      }
                }
	      HDfree(fname);
              info->file_external = file_external;
              break;
#ifdef CM5
          case DFACC_PARALLEL:
              para_extfile_id = CM_OPEN(fname, DFACC_WRITE);
              if (para_extfile_id == FAIL)
                {
                    para_extfile_id = CM_CREATE(fname);
                    if (para_extfile_id == FAIL)
                      {
                          access_rec->used = FALSE;
			  HDfree(fname);
                          HRETURN_ERROR(DFE_BADOPEN, FAIL);
                      }
                }
	      HDfree(fname);
              info->para_extfile_id = para_extfile_id;
              break;

#endif /* CM5 */
          default:
	      HDfree(fname);
              HRETURN_ERROR(DFE_BADOPEN, FAIL);
      }

    return SUCCEED;
}

/* ----------------------------- HXIstaccess ------------------------------ */
/*
NAME
   HXIstaccess -- set up AID to access an ext elem
USAGE
   int32 HXIstaccess(access_rec, acc_mode)
   access_t * access_rec;   IN: access record to fill in
   int16      acc_mode;     IN: access mode
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HXIstread and HXIstwrite resolve to this function.
   Given an active AID fill in all of the special information.
   If this information has already been read in for a different
   element use that else we must go out to the HDF file and
   pull in the information ourselves

---------------------------------------------------------------------------*/
PRIVATE int32
HXIstaccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HXIstaccess");    /* for HERROR */
    dd_t       *info_dd;        /* dd of the special information element */
    extinfo_t  *info;           /* special element information */
    filerec_t  *file_rec;       /* file record */
    uint8       local_ptbuf[12];    /* working buffer */

    /* get file record and validate */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec) || !(file_rec->access & acc_mode))
        HRETURN_ERROR(DFE_ARGS, FAIL);

    /* intialize the access record */
    access_rec->special = SPECIAL_EXT;
    access_rec->posn = 0;
    access_rec->access = acc_mode|DFACC_READ;

    /* get the dd for information */
    info_dd = &access_rec->block->ddlist[access_rec->idx];

    /* get the special info record */
    access_rec->special_info = HIgetspinfo(access_rec,
                                           info_dd->tag, info_dd->ref);
    if (access_rec->special_info)
      {   /* found it from other access records */
	  info = (extinfo_t *) access_rec->special_info;
	  info->attached++;
      }
    else
      {   /* look for information in the file */
	  if (HI_SEEK(file_rec->file, info_dd->offset + 2) == FAIL)
	    {
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_SEEKERROR, FAIL);
	    }
	  if (HI_READ(file_rec->file, local_ptbuf, 12) == FAIL)
	    {
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_READERROR, FAIL);
	    }
	  access_rec->special_info = (VOIDP) HDmalloc((uint32) sizeof(extinfo_t));
	  info = (extinfo_t *) access_rec->special_info;
	  if (!info)
	    {
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_NOSPACE, FAIL);
	    }
	  {
	      uint8      *p = local_ptbuf;
	      INT32DECODE(p, info->length);
	      INT32DECODE(p, info->extern_offset);
	      INT32DECODE(p, info->length_file_name);
	  }
	  info->extern_file_name = (char *) HDmalloc((uint32)
						info->length_file_name + 1);
	  if (!info->extern_file_name)
	    {
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_NOSPACE, FAIL);
	    }
	  if (HI_READ(file_rec->file, info->extern_file_name,
		      info->length_file_name) == FAIL)
	    {
		access_rec->used = FALSE;
		HRETURN_ERROR(DFE_READERROR, FAIL);
	    }
	  info->extern_file_name[info->length_file_name] = '\0';

          /* delay file opening until needed */
          info->file_open = FALSE;

#if 0
          info->file_external = HI_OPEN(info->extern_file_name, acc_mode);
          if (OPENERR(info->file_external))
            {
                access_rec->used = FALSE;
                HRETURN_ERROR(DFE_BADOPEN, FAIL);
            }
#endif /* 0 */

          info->attached = 1;
      }

    file_rec->attach++;
    return ASLOT2ID(access_rec - access_records);
}	/* HXIstaccess */

/* ------------------------------ HXPstread ------------------------------- */
/*
NAME
   HXPstread -- open an access record for reading
USAGE
   int32 HXPstread(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HXIstaccess to fill in the access rec for 
   reading

---------------------------------------------------------------------------*/
int32
HXPstread(accrec_t * rec)
{
    return HXIstaccess(rec, DFACC_READ);
}   /* HXPstread */

/* ------------------------------ HXPstwrite ------------------------------- */
/*
NAME
   HXPstwrite -- open an access record for reading
USAGE
   int32 HXPstwrite(access_rec)
   access_t * access_rec;   IN: access record to fill in
RETURNS
   The AID of the access record on success FAIL on error.
DESCRIPTION
   Calls to HXIstaccess to fill in the access rec for writing

---------------------------------------------------------------------------*/
int32
HXPstwrite(accrec_t * rec)
{
    return HXIstaccess(rec, DFACC_WRITE);
}   /* HXPstwrite */

/* ------------------------------ HXPseek ------------------------------- */
/*
NAME
   HXPseek -- set the seek posn
USAGE
   int32 HXPseek(access_rec, offset, origin)
   access_t * access_rec;      IN: access record to mess with
   int32      offset;          IN: seek offset
   int32      origin;          IN: where we should calc the offset from
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Set the seek posn in the given external element

---------------------------------------------------------------------------*/
int32
HXPseek(accrec_t * access_rec, int32 offset, int origin)
{
    CONSTR(FUNC, "HXPseek");    /* for HERROR */

    /* Adjust offset according to origin.
       there is no upper bound to posn */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += ((extinfo_t *) (access_rec->special_info))->length;
    if (offset < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* set the offset */
    access_rec->posn = offset;
    return SUCCEED;

}   /* HXPseek */

/* ------------------------------ HXPread ------------------------------- */
/*
NAME
   HXPread -- read some data out of an external file
USAGE
   int32 HXPseek(access_rec, length, data)
   access_t * access_rec;      IN: access record to mess with
   int32      length;          IN: number of bytes to read
   VOIDP      data;            IN: buffer for data
RETURNS
   The number of bytes read or FAIL on error
DESCRIPTION
   Read in some data from an external file.  If length is zero
   read until the end of the element.  It is assumed that the
   data buffer is big enough to store the data.

   BUG:  Need to investigate what happens if length would take
   us off the end of what has been written -- should only read
   until the end.

---------------------------------------------------------------------------*/
int32
HXPread(accrec_t * access_rec, int32 length, VOIDP data)
{
    CONSTR(FUNC, "HXPread");    /* for HERROR */
    extinfo_t  *info =          /* information on the special element */
    (extinfo_t *) access_rec->special_info;

    /* validate length */
    if (length < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* adjust length if it falls off the end of the element */
    if ((length == 0) || (access_rec->posn + length > info->length))
        length = info->length - access_rec->posn;
    else if (length < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* see if the file is open, if not open it */
    if (!info->file_open)
    {
	char	*fname;

	/* build the customized external file name. */
	if (!(fname = HXPbuildfilename(info->extern_file_name, DFACC_OLD)))
	    HRETURN_ERROR(DFE_BADOPEN, FAIL);

	info->file_external = HI_OPEN(fname, access_rec->access);
	HDfree(fname);
	if (OPENERR(info->file_external))
	{
	    access_rec->used = FALSE;
	    HERROR(DFE_BADOPEN);
	    HEreport("Could not find external file %s\n", info->extern_file_name);
	    return (FAIL);
	}
	info->file_open = TRUE;
    }

    /* read it in from the file */
#ifdef CM5
    if (access_rec->access_type == DFACC_PARALLEL)
      {
          /* parallel access handling */
          if (CM_SEEK(info->para_extfile_id, access_rec->posn + info->extern_offset) == FAIL)
              HRETURN_ERROR(DFE_SEEKERROR, FAIL);
          if (CM_READ(info->para_extfile_id, data, length) == FAIL)
            {
                HRETURN_ERROR(DFE_READERROR, FAIL);
            }
      }
    else
#endif
      {
          if (HI_SEEK(info->file_external, access_rec->posn + info->extern_offset) == FAIL)
              HRETURN_ERROR(DFE_SEEKERROR, FAIL);
          if (HI_READ(info->file_external, data, length) == FAIL)
              HRETURN_ERROR(DFE_READERROR, FAIL);
      }

    /* adjust access position */
    access_rec->posn += length;

    return length;
}	/* HXPread */

/* ------------------------------ HXPwrite ------------------------------- */
/*
NAME
   HXPwrite -- write some data out to an external file
USAGE
   int32 HXPwrite(access_rec, length, data)
   access_t * access_rec;      IN: access record to mess with
   int32      length;          IN: number of bytes to read
   VOIDP      data;            IN: buffer of data
RETURNS
   The number of bytes written or FAIL on error
DESCRIPTION
   Write out some data to an external file.  

   It looks like this will allow us to write to a file even if we only
   have a read AID for it.   Is that really the behavior that we want?

---------------------------------------------------------------------------*/
int32
HXPwrite(accrec_t * access_rec, int32 length, const VOIDP data)
{
    uint8       local_ptbuf[4]; /* temp buffer */
    CONSTR(FUNC, "HXPwrite");   /* for HERROR */
    extinfo_t  *info =          /* information on the special element */
                    (extinfo_t *) (access_rec->special_info);
    uint8      *p =            /* temp buffer ptr */
                   local_ptbuf;
    dd_t       *info_dd =      /* dd of infromation element */
                         &access_rec->block->ddlist[access_rec->idx];
    filerec_t  *file_rec =     /* file record */
                          FID2REC(access_rec->file_id);
    int32       file_off;      /* offset in the file we are at currently */

    /* validate length */
    if (length < 0)
        HRETURN_ERROR(DFE_RANGE, FAIL);

    /* see if the file is open, if not open it */
    if (!info->file_open)
    {
	char *fname;

	/* build the customized external file name. */
	if (!(fname = HXPbuildfilename(info->extern_file_name, DFACC_OLD)))
	    HRETURN_ERROR(DFE_BADOPEN, FAIL);

	info->file_external = HI_OPEN(fname, access_rec->access);
	HDfree(fname);
        if (OPENERR(info->file_external))
            {
                access_rec->used = FALSE;
                HERROR(DFE_BADOPEN);
                HEreport("Could not find external file %s\n", info->extern_file_name);
                return (FAIL);
            }
        info->file_open = TRUE;
    }

    /* write the data onto file */
#ifdef CM5
    if (access_rec->access_type == DFACC_PARALLEL)
      {
          /* parallel access handling */
          if (CM_SEEK(info->para_extfile_id,
                      access_rec->posn + info->extern_offset) == FAIL)
              HRETURN_ERROR(DFE_SEEKERROR, FAIL);
          if (CM_WRITE(info->para_extfile_id, data, length) == FAIL)
            {
                HRETURN_ERROR(DFE_DENIED, FAIL);
            }
      }
    else
#endif /* CM5 */
      {
	  if (HI_SEEK(info->file_external,
		      access_rec->posn + info->extern_offset) == FAIL)
	      HRETURN_ERROR(DFE_SEEKERROR, FAIL);
	  if (HI_WRITE(info->file_external, data, length) == FAIL)
	    {
		/* this external file might not be opened with write permission,
		   reopen the file and try again */
		hdf_file_t  f = HI_OPEN(info->extern_file_name, DFACC_WRITE);
		if (OPENERR(f) ||
		HI_SEEK(f, access_rec->posn + info->extern_offset) == FAIL ||
		    HI_WRITE(f, data, length) == FAIL)
		  {
		      HI_CLOSE(f);
		      HRETURN_ERROR(DFE_DENIED, FAIL);
		  }
		HI_CLOSE(info->file_external);

		/* if okay, substitute the file descriptor */
		info->file_external = f;
	    }
      }

    /* update access record, and information about special elelemt */
    access_rec->posn += length;
    if (access_rec->posn > info->length)
      {
          info->length = access_rec->posn;
          INT32ENCODE(p, info->length);
          if (HI_SEEK(file_rec->file, info_dd->offset + 2) == FAIL)
              HRETURN_ERROR(DFE_SEEKERROR, FAIL);
          if (HI_WRITE(file_rec->file, local_ptbuf, 4) == FAIL)
              HRETURN_ERROR(DFE_WRITEERROR, FAIL);
      }

    /* update end of file pointer? */
    file_off = HI_TELL(file_rec->file);
#ifdef TESTING
    printf("HXwrite: file_rec->f_end_off=%d\n",file_rec->f_end_off);
    printf("HXwrite: file_off=%d\n",file_off);
#endif
    if (file_off > file_rec->f_end_off)
      file_rec->f_end_off = file_off;

    return length;    /* return length of bytes written */
}	/* HXPwrite */

/* ------------------------------ HXPinquire ------------------------------ */
/*
NAME
   HXPinquire -- retreive information about an external element
USAGE
   int32 HXPinquire(access_rec, file, tag, ref, len, off, pos, acc, sp)
   access_t * access_rec;      IN:  access record to return info about
   uint16   * file;            OUT: file ID;
   uint16   * tag;             OUT: tag of info record;
   uint16   * ref;             OUT: ref of info record;
   int32    * len;             OUT: length of element;
   int32    * off;             OUT: offset of element (NOT correct);
   int32    * pos;             OUT: current position in element;
   int16    * acc;             OUT: access mode;
   int16    * sp;              OUT: special code;
RETURNS
   SUCCEED
DESCRIPTION
   Return interesting information about an external element.
   NULL can be passed for any of the OUT parameters if their
   value is not needed.
   BUG: The offset returned is not correct.

---------------------------------------------------------------------------*/
int32
HXPinquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
           uint16 *pref, int32 *plength, int32 *poffset,
           int32 *pposn, int16 *paccess, int16 *pspecial)
{
    dd_t       *info_dd =       /* dd of special information */
    &(access_rec->block->ddlist[access_rec->idx]);
    extinfo_t  *info =          /* special information record */
    (extinfo_t *) access_rec->special_info;

    /* fill in the variables if they are present */
    if (pfile_id)
        *pfile_id = access_rec->file_id;
    if (ptag)
        *ptag = info_dd->tag;
    if (pref)
        *pref = info_dd->ref;
    if (plength)
        *plength = info->length;
    if (poffset)
        *poffset = 0;   /* meaningless -- actually not anymore */
    if (pposn)
        *pposn = access_rec->posn;
    if (paccess)
        *paccess = access_rec->access;
    if (pspecial)
        *pspecial = access_rec->special;

    return SUCCEED;
}	/* HXPinquire */

/* ----------------------------- HXPendaccess ----------------------------- */
/*
NAME
   HXPendacess -- close file, free AID
USAGE
   intn HXPendaccess(access_rec)
   access_t * access_rec;      IN:  access record to close
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Close the file pointed to by the current AID and free the AID

---------------------------------------------------------------------------*/
intn
HXPendaccess(accrec_t * access_rec)
{
    CONSTR(FUNC, "HXPendaccess");   /* for HERROR */
    filerec_t  *file_rec =      /* file record */
    FID2REC(access_rec->file_id);

    /* close the file pointed to by this access rec */
    HXPcloseAID(access_rec);

    /* validate file record */
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used = FALSE;

    return SUCCEED;
}	/* HXPendaccess */

/* ----------------------------- HXPcloseAID ------------------------------ */
/*
NAME
   HXPcloseAID -- close file but keep AID active
USAGE
   int32 HXPcloseAID(access_rec)
   access_t * access_rec;      IN:  access record of file to close
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   close the file currently being pointed to by this AID but 
   do *NOT* free the AID.

   This is called by Hnextread() which reuses an AID to point to
   the 'next' object as requested.  If the current object was an
   external object, the external file needs to be closed before all
   reference to it is lost.

---------------------------------------------------------------------------*/
int32
HXPcloseAID(accrec_t * access_rec)
{
#ifdef LATER
    CONSTR(FUNC, "HXPcloseAID");    /* for HERROR */
#endif
    extinfo_t  *info =          /* special information record */
    (extinfo_t *) access_rec->special_info;

    /* detach the special information record.
       If no more references to that, free the record */

    if (--(info->attached) == 0)
      {
          if (info->file_open)
              HI_CLOSE(info->file_external);
          HDfree((VOIDP) info->extern_file_name);
          HDfree((VOIDP) info);
      }

    return SUCCEED;
}   /* HXPcloseAID */

/* ------------------------------- HXPinfo -------------------------------- */
/*
NAME
   HXPinfo -- return info about an external element
USAGE
   int32 HXPinfo(access_rec, info_block)
   accrec_t        * access_rec; IN: access record of element
   sp_info_block_t * info_block; OUT: information about the special element 
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Return information about the given external element.  Info_block is
   assumed to be non-NULL.  Do not make a copy of the path, just have
   the info_block copy point to our local copy.

   --------------------------------------------------------------------------- */
int32
HXPinfo(accrec_t * access_rec, sp_info_block_t * info_block)
{
    char       *FUNC = "HXPinfo";   /* for HERROR */
    extinfo_t  *info =          /* special information record */
    (extinfo_t *) access_rec->special_info;

    /* validate access record */
    if (access_rec->special != SPECIAL_EXT)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* fill in the info_block */
    info_block->key = SPECIAL_EXT;

    info_block->offset = info->extern_offset;
    info_block->path = info->extern_file_name;

    return SUCCEED;

}   /* HXPinfo */

/* ------------------------------- HXPreset ------------------------------- */
/*
NAME
   HXPreset -- replace the current external info with new info
USAGE
   int32 HXPreset(access_rec, info_block)
   accrec_t        * access_rec;   IN: access record of element
   sp_info_block_t * info_block;   IN: information about the special element 
RETURNS
   SUCCEED / FAIL
DESCRIPTION
   Reset information about the given external element.  Info_block is
   assumed to be non-NULL.

   Basically, what this routine does is throw out the old file
   information for a special element and replaces it with a new
   file name.  This is useful for when a file has changed places.
   The offset and length are assumed to be the same.

---------------------------------------------------------------------------*/
int32
HXPreset(accrec_t * access_rec, sp_info_block_t * info_block)
{
    char       *FUNC = "HXPreset";  /* for HERROR */
    dd_t       *dd;             /* dd of existing external element record */
    filerec_t  *file_rec;       /* file record */
    uint8       local_ptbuf[14 + MAX_PATH_LEN];     /* temp buffer */
    extinfo_t  *info =          /* special information record */
    (extinfo_t *) access_rec->special_info;

    /* validate access record -- make sure is already external element */
    if (access_rec->special != SPECIAL_EXT)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* just replace with other external element info for now  */
    /* (i.e., this can not change the type of special element */
    if (info_block->key != SPECIAL_EXT)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* check validity of file record */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec))
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* get the DD of the existing special element record */
    dd = &access_rec->block->ddlist[access_rec->idx];

    /* update our internal pointers */
    info->extern_offset = info_block->offset;
    info->extern_file_name = (char *) HDstrdup(info_block->path);
    if (!info->extern_file_name)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);
    info->length_file_name = HDstrlen(info->extern_file_name);

    /*
     * delete the existing tag / ref object
     * accomplish this by changing the offset and length of the existing
     *  special element DD and writing it in a new place
     */
    dd->length = 14 + info->length_file_name;
    if ((dd->offset = HPgetdiskblock(file_rec, dd->length, TRUE)) == FAIL)
        HRETURN_ERROR(DFE_INTERNAL, FAIL);

    /* write the new external file record */
    {
        uint8      *p = local_ptbuf;
        INT16ENCODE(p, SPECIAL_EXT);
        INT32ENCODE(p, info->length);
        INT32ENCODE(p, info->extern_offset);
        INT32ENCODE(p, info->length_file_name);
        HDstrcpy((char *) p, (char *) info->extern_file_name);
    }

    /* write out the new external file record */
    if (HI_WRITE(file_rec->file, local_ptbuf, dd->length) == FAIL)
      {
          HERROR(DFE_WRITEERROR);
          access_rec->used = FALSE;
          return FAIL;
      }

    /* update the DD block in the file */
    if (HIupdate_dd(file_rec, access_rec->block, access_rec->idx, FUNC) == FAIL)
        return FAIL;

    return SUCCEED;
}	/* HXPreset */

static	char*	extcreatedir = NULL;
static	char*	HDFEXTCREATEDIR = NULL;
static	char*	extdir = NULL;
static	char*	HDFEXTDIR = NULL;

/*------------------------------------------------------------------------ 
NAME
   HXsetcreatedir -- set the directory variable for creating external file
USAGE
   intn HXsetcreatedir(dir)
   char *dir		IN: directory for creating external file
RETURNS
   SUCCEED if no error, else FAIL
DESCRIPTION
   Set up the directory variable for creating external file.
   The directory content is copied into HXsetcreatedir area.
   If dir is NULL, the directory variable is unset.
   If error encountered during setup, previous value of createdir
   is not changed.

--------------------------------------------------------------------------*/
intn
HXsetcreatedir(char *dir)
{
    char       *FUNC = "HXsetcreatedir";  /* for HERROR */
    char	*pt;

    if (dir)
    {
	if (!(pt = HDstrdup(dir)))
	    HRETURN_ERROR(DFE_NOSPACE, FAIL);
    }
    else
	pt = dir;		/* will reset extcreatedir to NULL */

    if (extcreatedir)
	HDfree(extcreatedir);
    
    extcreatedir = pt;
    return(SUCCEED);
}	/* HXsetcreatedir */

/*------------------------------------------------------------------------ 
NAME
   HXsetdir -- set the directory variable for creating external file
USAGE
   intn HXsetdir(dir)
   char *dir		IN: directory for creating external file
RETURNS
   SUCCEED if no error, else FAIL
DESCRIPTION
   Set up the directory variable for creating external file.
   The directory content is copied into HXsetdir area.
   If dir is NULL, the directory variable is unset.
   If error encountered during setup, previous value of extdir
   is not changed.

--------------------------------------------------------------------------*/
intn
HXsetdir(char *dir)
{
    char       *FUNC = "HXsetdir";  /* for HERROR */
    char	*pt;

    if (dir)
    {
	if (!(pt = HDstrdup(dir)))
	    HRETURN_ERROR(DFE_NOSPACE, FAIL);
    }
    else
	pt = dir;		/* will reset extdir to NULL */

    if (extdir)
	HDfree(extdir);
    
    extdir = pt;
    return(SUCCEED);
}	/* HXsetdir */

/* ------------------------------- HXPbuildfilename ------------------------------- */
/*
NAME
    HXPbuildfilename -- Build the Filename for the External Element
USAGE
    char* HXPbuildfilename(char *ext_fname, const intn acc_mode)
    char            * ext_fname;	IN: external filename as stored
    intn 	      acc_mode;		IN: access mode
RETURNS
    finalpath / NULL
DESCRIPTION
    Compose the external object file name.
    [More detail later.]

---------------------------------------------------------------------------*/
/* the following can be sped up by doing my own copying instead of scanning */
/* for end-of-line two extra times, or even use memcpy since the string lengths */
/* are calculated already.  For now, it works. */
#define HDstrcpy3(s1, s2, s3, s4)	(HDstrcat(HDstrcat(HDstrcpy(s1, s2),s3),s4))
#include <sys/stat.h>

PRIVATE
char *
HXPbuildfilename(const char *ext_fname, const intn acc_mode)
{
    char       *FUNC = "HXPbuildfilename";  /* for HERROR */
    int	        fname_len;		/* string length of the ext_fname */
    int	        path_len;		/* string length of prepend pathname */
    static	firstinvoked = 1;	/* true if invoked the first time */

    char	*finalpath;	/* Final pathname to return */
    char	*fname;
    struct	stat filestat;	/* for checking pathname existence */

    /* initialize HDFEXTDIR and HDFCREATEDIR if invoked the first time */
    if (firstinvoked){
	firstinvoked = 0;
	HDFEXTCREATEDIR = HDgetenv("HDFEXTCREATEDIR");
	HDFEXTDIR = HDgetenv("HDFEXTDIR");
    }

    if (!ext_fname)
        HRETURN_ERROR(DFE_ARGS, NULL);
    fname = (char *) ext_fname;

    /* get the space for the final pathname */
    if (!(finalpath=HDmalloc(MAX_PATH_LEN)))
	HRETURN_ERROR(DFE_NOSPACE, NULL);

    fname_len = HDstrlen(fname);
    
    switch (acc_mode){
    case DFACC_CREATE: {			/* Creating a new external element */
	if ( *fname == '/' ) {	/* Absolute Pathname */
		return (HDstrcpy(finalpath, fname));
	}
	else {				/* Relative Pathname */

	    /* try function variable */
	    if (extcreatedir) {
		path_len = HDstrlen(extcreatedir);

		if (fname_len + 1 + path_len + 1 > MAX_PATH_LEN ){
		    HDfree(finalpath);
		    HRETURN_ERROR(DFE_NOSPACE, NULL);
		}
		return(HDstrcpy3(finalpath, extcreatedir, "/", fname));
	    }

	    /* try Envrironment Variable */
	    if (HDFEXTCREATEDIR) {
		path_len = HDstrlen(HDFEXTCREATEDIR);

		if (fname_len + 1 + path_len + 1 > MAX_PATH_LEN ){
		    HDfree(finalpath);
		    HRETURN_ERROR(DFE_NOSPACE, NULL);
		}
		return(HDstrcpy3(finalpath, HDFEXTCREATEDIR, "/", fname));
	    }

	    /* try Head File Directory */
	    /* Don't have Head File information now.  Continue */

	    /* Just return the ext_fname */
	    return(HDstrcpy(finalpath, fname));
	}
	break;
    }
    case DFACC_OLD:{			/* Locating an old external element */
	if ( *fname == '/' ) {	/* Absolute Pathname */
	    if (stat(fname, &filestat) == 0){
		return (HDstrcpy(finalpath, fname));
	    }
	    else if (!extdir && !HDFEXTDIR) {
		HDfree(finalpath);
		HRETURN_ERROR(DFE_FNF, NULL);
	    }
	    /* stripe the pathname component */
	    fname = HDstrrchr(fname, '/') + 1;
	    fname_len = HDstrlen(fname);

	    /* continue to Relative Pathname */
	}


	/* Relative Pathname */
	{
	    char   *dir_pt, *path_pt;	/* temporary pointers */

	    /* try function variable */
	    if (extdir) {
		dir_pt = extdir;
		while (*dir_pt){
		    /* extract one extdir component to finalpath */
		    path_len = 0;
		    path_pt = finalpath;
		    while (*dir_pt && *dir_pt != ':'){
			if (path_len >= MAX_PATH_LEN){
			    HDfree(finalpath);
			    HRETURN_ERROR(DFE_NOSPACE, NULL);
			}
			*path_pt++ = *dir_pt++;
			path_len++;
		    }
		    if (*dir_pt == ':') dir_pt++;
		    *path_pt++ = '/';
		    path_len++;

		    if (fname_len + path_len + 1 > MAX_PATH_LEN ){
			HDfree(finalpath);
			HRETURN_ERROR(DFE_NOSPACE, NULL);
		    }
		    HDstrcpy(path_pt, fname);
		    if (stat(finalpath, &filestat) == 0 ){
			return(finalpath);
		    }
		}
	    }

	    /* try Envrironment Variable */
	    if (HDFEXTDIR) {
		dir_pt = HDFEXTDIR;
		while (*dir_pt){
		    /* extract one HDFEXTDIR component to finalpath */
		    path_len = 0;
		    path_pt = finalpath;
		    while (*dir_pt && *dir_pt != ':'){
			if (path_len >= MAX_PATH_LEN){
			    HDfree(finalpath);
			    HRETURN_ERROR(DFE_NOSPACE, NULL);
			}
			*path_pt++ = *dir_pt++;
			path_len++;
		    }
		    if (*dir_pt == ':') dir_pt++;
		    *path_pt++ = '/';
		    path_len++;

		    if (fname_len + path_len + 1 > MAX_PATH_LEN ){
			HDfree(finalpath);
			HRETURN_ERROR(DFE_NOSPACE, NULL);
		    }
		    HDstrcpy(path_pt, fname);
		    if (stat(finalpath, &filestat) == 0 ){
			return(finalpath);
		    }
		}
	    }

	    /* try Head File Directory */
	    /* Don't have Head File information now.  Continue */

	    /* See if the file exists */
	    if (stat(fname, &filestat) == 0 )
		return(HDstrcpy(finalpath, fname));

	    /* All have failed */
	    HDfree(finalpath);
	    return(NULL);
	}
	break;
    }
    default:
	HDfree(finalpath);
	HRETURN_ERROR(DFE_ARGS, NULL);
    }
}	/* HXPbuildfilename */
