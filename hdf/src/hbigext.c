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

/*LINTLIBRARY*/
/* ------------------------------ hextelt.c -------------------------------

 Routines for external elements, i.e., data elements that reside on
 some other file.  These elements have no limits on their length.
 While users are prevented from reading beyond what is written, a
 user can write an unlimited amount of data.

 17-Mar-93
 Adding offset and "starting length" to elements so that a user can
 take an existing file with some data in it and create an HDF file
 which has a pointer to that data.

-------------------------------------------------------------------------*/

#include "hdf.h"
#include "hfile.h"

/* extinfo_t -- external elt information structure */

typedef struct {
    int attached;              /* number of access records attached
                                  to this information structure */
    int32 extern_offset;
    int32 length;              /* length of this element */
    int32 length_file_name;    /* length of the external file name */
    hdf_file_t file_external;      /* external file descriptor */
    char *extern_file_name;    /* name of the external file */
} extinfo_t;

/* forward declaration of the functions provided in this module */
PRIVATE int32 HBIstaccess
    (accrec_t *access_rec, int16 acc_mode);

#ifdef OLD_WAY /* use global temp. buffer now */
/* Private buffer */
PRIVATE uint8 *ptbuf = NULL;
#endif 

/* bigext_funcs -- table of the accessing functions of the big external
   data element function modules.  The position of each function in
   the table is standard */

funclist_t bigext_funcs = {
    HBPstread,
    HBPstwrite,
    HBPseek,
    HBPinquire,
    HBPread,
    HBPwrite,
    HBPendaccess,
    HBPinfo,
};


/* ------------------------------------------------------------------------ 

 NAME
	HBcreate -- create an external element
 USAGE
	int32 HBcreate(file_id, tag, ref, ext_name, offset, len)
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
int32 HBcreate(int32 file_id, uint16 tag, uint16 ref, const char *extern_file_name, int32 offset, int32 start_len)
{
    CONSTR(FUNC,"HBcreate");     /* for HERROR */
    filerec_t *file_rec;       /* file record */
    accrec_t *access_rec;      /* access element record */
    int slot;
    dd_t *dd;
    ddblock_t *data_block;     /* dd block ptr to exist data element */
    int32 data_idx;            /* dd list index to existing data element */
    hdf_file_t file_external;      /* external file descriptor */
    extinfo_t *info;           /* special element information */
    dd_t *data_dd;             /* dd of existing regular element */
    uint16 special_tag;                /* special version of tag */
    uint8 local_ptbuf[14+MAX_PATH_LEN];

    /* clear error stack and validate args */
    HEclear();
    file_rec = FID2REC(file_id);
    if (BADFREC(file_rec) || !extern_file_name || (offset<0) || SPECIALTAG(tag)
            || (special_tag = MKSPECIALTAG(tag)) == DFTAG_NULL )
       HRETURN_ERROR(DFE_ARGS,FAIL);

    if (!(file_rec->access & DFACC_WRITE))
       HRETURN_ERROR(DFE_DENIED,FAIL);

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL)
      {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }
#endif

    /* get a slot in the access records table */
    slot = HIget_access_slot();
    if (slot == FAIL)
       HRETURN_ERROR(DFE_TOOMANY,FAIL);
    access_rec = &access_records[slot];

    /* look for existing data element of the same tag/ref */
    if (FAIL != HIlookup_dd(file_rec, tag, ref, &data_block, &data_idx)) {
       data_dd = &(data_block->ddlist[data_idx]);
       if (SPECIALTAG(data_dd->tag)) {

           /* abort since we cannot convert the data element to an external
              data element */

           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_CANTMOD,FAIL);
       }
    } else
       data_dd = (dd_t *) NULL;

    /* look for empty dd to use */
    if (FAIL == HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD,
                         &file_rec->null_block, &file_rec->null_idx)) {
       if (FAIL == HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC)) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_NOFREEDD,FAIL);
       } else {
           access_rec->block = file_rec->ddlast;
           access_rec->idx   = 0;
       }
    } else {
      access_rec->block = file_rec->null_block;
      access_rec->idx   = file_rec->null_idx;
    }
    dd = &access_rec->block->ddlist[access_rec->idx];

    /* create the external file */
    file_external = HI_OPEN(extern_file_name, DFACC_WRITE);
    if (OPENERR(file_external)) {
        file_external = HI_CREATE(extern_file_name);
        if(OPENERR(file_external)) {
            access_rec->used = FALSE;
            HRETURN_ERROR(DFE_BADOPEN,FAIL);
        }
    }

    /* set up the special element information and write it to file */
    access_rec->special_info = (VOIDP) HDgetspace((uint32)sizeof(extinfo_t));
    info = (extinfo_t *) access_rec->special_info;
    if (!info) {
       access_rec->used = FALSE;
       HRETURN_ERROR(DFE_NOSPACE,FAIL);
    }

    if (data_dd) {
       VOIDP buf;              /* temporary buffer */
       buf = (VOIDP)HDgetspace((uint32) data_dd->length);
       if (!buf) {
           access_rec->used = FALSE;
           HDfreespace((VOIDP)info);
           HRETURN_ERROR(DFE_NOSPACE,FAIL);
       }
       if (HI_SEEK(file_rec->file, data_dd->offset) == FAIL) {
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           HRETURN_ERROR(DFE_SEEKERROR,FAIL);
       }
       if (HI_READ(file_rec->file, buf, data_dd->length) == FAIL) {
           access_rec->used = FALSE;
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           HRETURN_ERROR(DFE_READERROR,FAIL);
       }
       if (HI_SEEK(file_external, offset) == FAIL) {
           access_rec->used = FALSE;
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           HRETURN_ERROR(DFE_SEEKERROR,FAIL);
       }
       if (HI_WRITE(file_external, buf, data_dd->length) == FAIL) {
           access_rec->used = FALSE;
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           HRETURN_ERROR(DFE_WRITEERROR,FAIL);
       }
       HDfreespace((VOIDP)buf);
       info->length = data_dd->length;
    } else {
       info->length = start_len;
    }

    info->attached = 1;
    info->file_external = file_external;
    info->extern_offset = offset;
    info->extern_file_name = (char *)HDstrdup((char *)extern_file_name);
    if (!info->extern_file_name) {
       access_rec->used = FALSE;
       HDfreespace((VOIDP)info);
       HRETURN_ERROR(DFE_NOSPACE,FAIL);
    }

    info->length_file_name = HDstrlen(extern_file_name);
    {
       uint8 *p = local_ptbuf;
       INT16ENCODE(p, SPECIAL_EXT);
       INT32ENCODE(p, info->length);
       INT32ENCODE(p, info->extern_offset);
       INT32ENCODE(p, info->length_file_name);
       HDstrcpy((char *) p, extern_file_name);
    }
    if (HI_SEEKEND(file_rec->file) == FAIL) {
       access_rec->used = FALSE;
       HDfreespace((VOIDP)info);
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    }
    dd->offset = HI_TELL(file_rec->file);
    dd->length = 14 + info->length_file_name;
    dd->tag = special_tag;
    dd->ref = ref;
    if (HI_WRITE(file_rec->file, local_ptbuf, dd->length) == FAIL) {
       HERROR(DFE_WRITEERROR);
       access_rec->used = FALSE;
       return FAIL;
    }

    if (FAIL == HIupdate_dd(file_rec, access_rec->block,
                           access_rec->idx, FUNC)) {
        access_rec->used = FALSE;
        HDfreespace((VOIDP)info);
        return FAIL;
    }

    /* add new DD to hash table */
    if (FAIL == HIadd_hash_dd(file_rec, dd->tag, dd->ref, access_rec->block,
                           access_rec->idx)) {
        access_rec->used = FALSE;
        HDfreespace((VOIDP)info);
        return FAIL;
    }

    if (data_dd) {
       Hdeldd(file_id, data_dd->tag, data_dd->ref);
       HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref);
    }

    /* update access record and file record */
    access_rec->special_func = &bigext_funcs;
    access_rec->special = SPECIAL_EXT;
    access_rec->posn = 0;
    access_rec->access = DFACC_WRITE;
    access_rec->file_id = file_id;
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */

    file_rec->attach++;
    if (ref > file_rec->maxref)
        file_rec->maxref = ref;

    return ASLOT2ID(slot);
}


/* ----------------------------- HBIstaccess ------------------------------ */
/*

 NAME
	HBIstaccess -- set up AID to access an ext elem
 USAGE
	int32 HBIstaccess(access_rec, access)
        access_t * access_rec;   IN: access record to fill in
        int16      access;       IN: access mode
 RETURNS
        The AID of the access record on success FAIL on error.
 DESCRIPTION
        Calls to HBIstread and HBIstwrite resolve to this function.
        Given an active AID fill in all of the special information.
        If this information has already been read in for a different
        element use that else we must go out to the HDF file and 
        pull in the information ourselves

--------------------------------------------------------------------------- */ 
PRIVATE int32 HBIstaccess(accrec_t *access_rec, int16 acc_mode)
{
    CONSTR(FUNC,"HBIstaccess");  /* for HERROR */
    dd_t *info_dd;             /* dd of the special information element */
    extinfo_t *info;           /* special element information */
    filerec_t *file_rec;       /* file record */
    uint8 local_ptbuf[12];

    /* get file record and validate */
    file_rec = FID2REC(access_rec->file_id);
    if (BADFREC(file_rec) || !(file_rec->access & acc_mode))
       HRETURN_ERROR(DFE_ARGS,FAIL);

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }
#endif
    
    /* intialize the access record */
    access_rec->special = SPECIAL_EXT;
    access_rec->posn = 0;
    access_rec->access = acc_mode;

    /* get the dd for information */
    info_dd = &access_rec->block->ddlist[access_rec->idx];

    /* get the special info record */
    access_rec->special_info = HIgetspinfo(access_rec,
                                          info_dd->tag, info_dd->ref);
    if (access_rec->special_info) {

       /* found it from other access records */

       info = (extinfo_t *)access_rec->special_info;
       info->attached++;

    } else {

       /* look for information in the file */

       if (HI_SEEK(file_rec->file, info_dd->offset+2) == FAIL) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_SEEKERROR,FAIL);
       }
       if (HI_READ(file_rec->file, local_ptbuf, 12) == FAIL) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_READERROR,FAIL);
       }
       access_rec->special_info = (VOIDP) HDgetspace((uint32)sizeof(extinfo_t));
       info = (extinfo_t *) access_rec->special_info;
       if (!info) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_NOSPACE,FAIL);
       }
       {
           uint8 *p = local_ptbuf;
           INT32DECODE(p, info->length);
           INT32DECODE(p, info->extern_offset);
           INT32DECODE(p, info->length_file_name);
       }
       info->extern_file_name = (char *)HDgetspace((uint32)
                                               info->length_file_name + 1);
       if (!info->extern_file_name) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_NOSPACE,FAIL);
       }
       if (HI_READ(file_rec->file, info->extern_file_name,
                  info->length_file_name) == FAIL) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_READERROR,FAIL);
       }
       info->extern_file_name[info->length_file_name] = '\0';
       info->file_external = HI_OPEN(info->extern_file_name, acc_mode);
       if (OPENERR(info->file_external)) {
           access_rec->used = FALSE;
           HRETURN_ERROR(DFE_BADOPEN,FAIL);
       }
       info->attached = 1;
    }

    file_rec->attach++;

    return ASLOT2ID(access_rec - access_records);

} /* HBIstaccess */


/* ------------------------------ HBPstread ------------------------------- */
/*

 NAME
	HBPstread -- open an access record for reading
 USAGE
	int32 HBPstread(access_rec)
        access_t * access_rec;   IN: access record to fill in
 RETURNS
        The AID of the access record on success FAIL on error.
 DESCRIPTION
        Calls to HBIstaccess to fill in the access rec for 
        reading

--------------------------------------------------------------------------- */ 
int32 HBPstread(accrec_t *rec)
{
    return HBIstaccess(rec, DFACC_READ);
} /* HBPstread */


/* ------------------------------ HBPstwrite ------------------------------- */
/*

 NAME
	HBPstwrite -- open an access record for reading
 USAGE
	int32 HBPstwrite(access_rec)
        access_t * access_rec;   IN: access record to fill in
 RETURNS
        The AID of the access record on success FAIL on error.
 DESCRIPTION
        Calls to HBIstaccess to fill in the access rec for writing

--------------------------------------------------------------------------- */ 
int32 HBPstwrite(accrec_t *rec)
{
    return HBIstaccess(rec, DFACC_WRITE);
} /* HBPstwrite */


/* ------------------------------ HBPseek ------------------------------- */
/*

 NAME
	HBPseek -- set the seek posn
 USAGE
	int32 HBPseek(access_rec, offset, origin)
        access_t * access_rec;      IN: access record to mess with
        int32      offset;          IN: seek offset
        int32      origin;          IN: where we should calc the offset from
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Set the seek posn in the given external element

--------------------------------------------------------------------------- */
int32 HBPseek(accrec_t *access_rec, int32 offset, int origin)
{
    CONSTR(FUNC,"HBPseek");      /* for HERROR */

    /* Adjust offset according to origin.
       there is no upper bound to posn */
    if (origin == DF_CURRENT)
        offset += access_rec->posn;
    if (origin == DF_END)
        offset += ((extinfo_t *)(access_rec->special_info))->length;
    if (offset < 0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    /* set the offset */
    access_rec->posn = offset;
    return SUCCEED;

} /* HBPseek */


/* ------------------------------ HBPread ------------------------------- */
/*

 NAME
	HBPread -- read some data out of an external file
 USAGE
	int32 HBPseek(access_rec, length, data)
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

--------------------------------------------------------------------------- */
int32 HBPread(accrec_t *access_rec, int32 length, VOIDP data)
{
    CONSTR(FUNC,"HBPread");      /* for HERROR */
    extinfo_t *info =          /* information on the special element */
       (extinfo_t *)access_rec->special_info;

    /* validate length */
    if (length < 0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    /* adjust length if it falls off the end of the element */
    if (length == 0)
        length = info->length - access_rec->posn;
    else
        if (length < 0 || access_rec->posn + length > info->length)
           HRETURN_ERROR(DFE_RANGE,FAIL);

    /* read it in from the file */
    if (HI_SEEK(info->file_external,access_rec->posn+info->extern_offset)==FAIL)
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if (HI_READ(info->file_external, data, length) == FAIL)
       HRETURN_ERROR(DFE_READERROR,FAIL);

    /* adjust access position */
    access_rec->posn += length;

    return length;

} /* HBPread */


/* ------------------------------ HBPwrite ------------------------------- */
/*

 NAME
	HBPwrite -- write some data out to an external file
 USAGE
	int32 HBPwrite(access_rec, length, data)
        access_t * access_rec;      IN: access record to mess with
        int32      length;          IN: number of bytes to read
        VOIDP      data;            IN: buffer of data
 RETURNS
        The number of bytes written or FAIL on error
 DESCRIPTION
        Write out some data to an external file

--------------------------------------------------------------------------- */
int32 HBPwrite(accrec_t *access_rec, int32 length, const VOIDP data)
{
    CONSTR(FUNC,"HBPwrite");     /* for HERROR */
    extinfo_t *info =          /* information on the special element */
       (extinfo_t*)(access_rec->special_info);
    uint8 local_ptbuf[4];

    /* validate length */
    if (length < 0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

#ifdef OLD_WAY
    /* Check if temproray buffer has been allocated */
    if (ptbuf == NULL) {
        ptbuf = (uint8 *)HDgetspace(TBUF_SZ * sizeof(uint8));
        if (ptbuf == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
      }
#endif

    /* write the data onto file */
    if (HI_SEEK(info->file_external,
		access_rec->posn + info->extern_offset) == FAIL)
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
    if (HI_WRITE(info->file_external, data, length) == FAIL) {

       /* this external file might not be opened with write permission,
          reopen the file and try again */
       hdf_file_t f = HI_OPEN(info->extern_file_name, DFACC_WRITE);
       if (OPENERR(f) ||
                HI_SEEK(f,access_rec->posn + info->extern_offset) == FAIL ||
                HI_WRITE(f, data, length) == FAIL) {
           HI_CLOSE(f);
           HRETURN_ERROR(DFE_DENIED,FAIL);
       }
       HI_CLOSE(info->file_external);

       /* if okay, substitute the file descriptor */

       info->file_external = f;
    }

    /* update access record, and information about special elelemt */
    access_rec->posn += length;
    if (access_rec->posn > info->length) {
       uint8 *p =      /* temp buffer ptr */
           local_ptbuf;
       dd_t *info_dd =         /* dd of infromation element */
           &access_rec->block->ddlist[access_rec->idx];
       filerec_t *file_rec =   /* file record */
           FID2REC(access_rec->file_id);

       info->length = access_rec->posn;
       INT32ENCODE(p, info->length);
       if (HI_SEEK(file_rec->file, info_dd->offset+2) == FAIL)
           HRETURN_ERROR(DFE_SEEKERROR,FAIL);
       if (HI_WRITE(file_rec->file, local_ptbuf, 4) == FAIL)
           HRETURN_ERROR(DFE_WRITEERROR,FAIL);
    }

    return length;
} /* HBPwrite */


/* ------------------------------ HBPinquire ------------------------------ */
/*

 NAME
	HBPinquire -- retreive information about an external element
 USAGE
	int32 HBPinquire(access_rec, file, tag, ref, len, off, pos, acc, sp)
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

--------------------------------------------------------------------------- */
int32 HBPinquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
                        uint16 *pref, int32 *plength, int32 *poffset,
                        int32 *pposn, int16 *paccess, int16 *pspecial)
{
    dd_t *info_dd =            /* dd of special information */
       &(access_rec->block->ddlist[access_rec->idx]);
    extinfo_t *info =          /* special information record */
       (extinfo_t *)access_rec->special_info;

    /* fill in the variables if they are present */
    if (pfile_id) *pfile_id = access_rec->file_id;
    if (ptag)     *ptag = info_dd->tag;
    if (pref)     *pref = info_dd->ref;
    if (plength)  *plength = info->length;
    if (poffset)  *poffset = 0; /* meaningless -- actually not anymore */
    if (pposn)    *pposn = access_rec->posn;
    if (paccess)  *paccess = access_rec->access;
    if (pspecial) *pspecial = access_rec->special;

    return SUCCEED;

} /* HBPinquire */


/* ----------------------------- HBPendaccess ----------------------------- */
/*

 NAME
	HBPendacess -- close file, free AID
 USAGE
    intn HBPendaccess(access_rec)
        access_t * access_rec;      IN:  access record to close
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Close the file pointed to by the current AID and free the AID

--------------------------------------------------------------------------- */
intn HBPendaccess(accrec_t *access_rec)
{
    CONSTR(FUNC,"HBPendaccess"); /* for HERROR */
    filerec_t *file_rec =      /* file record */
       FID2REC(access_rec->file_id);

    /* close the file pointed to by this access rec */
    HBPcloseAID(access_rec);

    /* validate file record */
    if (BADFREC(file_rec))
       HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used = FALSE;

    return SUCCEED;

} /* HBPendaccess */


/* ----------------------------- HBPcloseAID ------------------------------ */
/*

 NAME
	HBPcloseAID -- close file but keep AID active
 USAGE
	int32 HBPcloseAID(access_rec)
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

--------------------------------------------------------------------------- */
int32 HBPcloseAID(accrec_t *access_rec)
{
#ifdef LATER
    CONSTR(FUNC,"HBPcloseAID"); /* for HERROR */
#endif
    extinfo_t *info =          /* special information record */
       (extinfo_t *)access_rec->special_info;

    /* detach the special information record.
       If no more references to that, free the record */

    if (--(info->attached) == 0) {
       HI_CLOSE(info->file_external);
       HDfreespace((VOIDP) info->extern_file_name);
       HDfreespace((VOIDP) info);
    }

    return SUCCEED;
} /* HBPcloseAID */

/* ------------------------------- HBPinfo -------------------------------- */
/*

 NAME
	HBPinfo -- return info about a big external element
 USAGE
	int32 HXPinfo(access_rec, info_block)
        accrec_t        * access_rec;        
                                IN: access record of element
        sp_info_block_t * info_block; 
                                OUT: information about the special element 
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Return information about the given big external element.  
        Info_block is assumed to be non-NULL.  Do not make a copy 
        of the path, just have the info_block copy point to our 
        local copy.

--------------------------------------------------------------------------- */
int32 HBPinfo(accrec_t * access_rec, sp_info_block_t * info_block)
{
    char *FUNC="HXPinfo";      /* for HERROR */
    extinfo_t *info =         /* special information record */
       (extinfo_t *)access_rec->special_info;

    /* validate access record */
    if (access_rec->special != SPECIAL_BIGEXT)
        HRETURN_ERROR(DFE_INTERNAL,FAIL);
    
    /* fill in the info_block */
    info_block->key = SPECIAL_BIGEXT;

    info_block->offset    = info->extern_offset;
    info_block->path      = info->extern_file_name;

    return SUCCEED;

} /* HBPinfo */
