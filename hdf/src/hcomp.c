#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/09/28 18:04:21  koziol
Removed OLD_WAY & QAK #ifdef's.  Removed oldspecial #ifdef's for special
tag handling.  Added new compression special tag type.

*
*/

/*
 FILE
       hcomp.c
       HDF compressed data I/O routines

 REMARKS
        These functions read and write compressed data to HDF data objects.
        The compressed data objects are implemented as "special tags"
        in the HDF file and the "H" layer I/O routines break out to the
        functions in this module to deal with them.

 DESIGN
            The compression I/O functions are designed as state machines.
        There are two seperate state machines implemented, as layers
        on top of one another.
            The top layer is the modeling layer, whose purpose is to
        send/receive uncompressed bytes between the higher layer
        (the "H" layer routines) and the lower layer, the coding layer.
        The modeling layer is constrained to look like Standard C
        I/O functions to the upper layer, while sending data in
        potentially unusual orders to the coding layer.  [An unusual
        order of bytes would be a Peano or Hilbert curve instead
        of the raster order more normally used].
            The lower layer is the coding layer, whose purpose is to
        send/receive bytes of data to the higher layer (the modeling
        layer) and to send/receive bits/bytes of data to the bit I/O
        functions after encoding them with some compression scheme.
            Both of these layers are designed as independent state
        machines whose state contains all the information to restart
        at a given point.  The purpose of this is to "cache" the
        state of each layer at certain [convenient] times in order
        to increase performance during random I/O.

 EXPORTED ROUTINES
        HCcreate - create or modify an existing data element to be compressed

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
    9/21/93     Starting coding specs
*/

#include "hdf.h"
#include "herr.h"
#include "hfile.h"

/* compinfo_t -- compressed element information structure */

typedef struct {
    int attached;              /* number of access records attached
                                  to this information structure */
#ifdef QAK
    int32 extern_offset;
    int32 length;              /* length of this element */
    int32 length_file_name;    /* length of the external file name */
    hdf_file_t file_external;      /* external file descriptor */
    char *extern_file_name;    /* name of the external file */
#endif
} compinfo_t;

/* declaration of the functions provided in this module */
PRIVATE int32 HCIstaccess
    PROTO((accrec_t *access_rec, int16 access));

/* comp_funcs -- struct of accessing functions for the compressed
   data element function modules.  The position of each function in
   the table is standard */

funclist_t comp_funcs={
    HCPstread,
    HCPstwrite,
    HCPseek,
    HCPinquire,
    HCPread,
    HCPwrite,
    HCPendaccess,
};

/*--------------------------------------------------------------------------
 NAME
    HCcreate -- Create a compressed data element

 USAGE
    int32 HCcreate(id,tag,ref)
    int32 id;            IN: the file id to create the data in
    uint16 tag,ref;      IN: the tag/ref pair which is to be compressed

 RETURNS
    Return an AID to the newly created compressed element, FAIL on error.

 DESCRIPTION
    Create a compressed data element.  If that data element already
    exists, we will compress that data element if it is currently un-compresed,
    or return FAIL if it is already compressed.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCcreate(int32 file_id, uint16 tag, uint16 ref)
#else
int32 HCcreate(file_id, tag, ref)
    int32 file_id;      /* file record id */
    uint16 tag, ref;    /* tag/ref of the special data element to create */
#endif
{
    char *FUNC="HCcreate";  /* for HERROR */
    filerec_t *file_rec;    /* file record */
    accrec_t *access_rec;   /* access element record */
    int slot;
    dd_t *dd;
    ddblock_t *data_block;  /* dd block ptr to exist data element */
    int32 data_idx;         /* dd list index to existing data element */
    compinfo_t *info;       /* special element information */
    dd_t *data_dd;          /* dd of existing regular element */
    uint16 special_tag;     /* special version of tag */

    /* clear error stack and validate args */
    HEclear();
    file_rec=FID2REC(file_id);
    if(!file_rec || file_rec->refcount==0 || SPECIALTAG(tag) ||
            (special_tag=MKSPECIALTAG(tag))==DFTAG_NULL)
       HRETURN_ERROR(DFE_ARGS,FAIL);

    /* chech for access permission */
    if(!(file_rec->access & DFACC_WRITE))
       HRETURN_ERROR(DFE_DENIED,FAIL);

    /* get a slot in the access records table */
    if(FAIL==(slot=HIget_access_slot()))
       HRETURN_ERROR(DFE_TOOMANY,FAIL);

    access_rec=&access_records[slot];

    /* look for existing data element of the same tag/ref */
    if(FAIL!=HIlookup_dd(file_rec, tag, ref, &data_block, &data_idx)) {
        data_dd=&(data_block->ddlist[data_idx]);
        if(SPECIALTAG(data_dd->tag)) {
           /* abort since we cannot convert the data element to a compressed
              data element */
           access_rec->used=FALSE;
           HRETURN_ERROR(DFE_CANTMOD,FAIL);
          } /* end if */
      } /* end if */
    else
       data_dd=NULL;

    /* look for empty dd to use */
    if(FAIL==HIlookup_dd(file_rec, DFTAG_NULL, DFREF_WILDCARD,
                         &file_rec->null_block, &file_rec->null_idx)) {
        if(FAIL==HInew_dd_block(file_rec, FILE_NDDS(file_rec), FUNC)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_NOFREEDD,FAIL);
          }  /* end if */
        else {
           access_rec->block=file_rec->ddlast;
           access_rec->idx  =0;
          } /* end else */
      } /* end if */
    else {
        access_rec->block=file_rec->null_block;
        access_rec->idx  =file_rec->null_idx;
      } /* end else */
    dd=&access_rec->block->ddlist[access_rec->idx];

    /* set up the special element information and write it to file */
    info=(compinfo_t *)HDgetspace(sizeof(compinfo_t));
    access_rec->special_info=(VOIDP)info;
    if(info==NULL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_NOSPACE,FAIL);
      } /* end if */

    if(data_dd!=NULL) {     /* compress existing data */
#ifdef QAK
       VOIDP buf;              /* temporary buffer */

       buf=HDgetspace(data_dd->length);
       if(!buf) {
           HERROR(DFE_NOSPACE);
           HDfreespace((VOIDP)info);
           return FAIL;
       }
       if(HI_SEEK(file_rec->file, data_dd->offset)==FAIL) {
           HERROR(DFE_SEEKERROR);
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           return FAIL;
       }
       if(HI_READ(file_rec->file, buf, data_dd->length)==FAIL) {
           HERROR(DFE_READERROR);
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           return FAIL;
       }
       if(HI_SEEK(file_external, f_offset)==FAIL) {
           HERROR(DFE_SEEKERROR);
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           return FAIL;
       }
       if(HI_WRITE(file_external, buf, data_dd->length)==FAIL) {
           HERROR(DFE_WRITEERROR);
           HDfreespace((VOIDP)info);
           HDfreespace((VOIDP)buf);
           return FAIL;
       }
       HDfreespace((VOIDP)buf);
       info->length=data_dd->length;
#endif
      } /* end if */
    else {  /* start new compressed data element */
      } /* end else */

    /* set up compressed special info structure */
    info->attached=1;
#ifdef QAK
    info->file_external=file_external;
    info->extern_offset=f_offset;
    info->extern_file_name=(char *)HDstrdup(extern_file_name);
    if(!info->extern_file_name) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_NOSPACE,FAIL);
      } /* end if */
#endif

    /* write special element info to the file */
    {
       uint8 *p=tbuf;

       INT16ENCODE(p, SPECIAL_COMP);
    }
    if(HI_SEEKEND(file_rec->file)==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */

    /* write compressed special element data to the file */
    dd->offset=HI_TELL(file_rec->file);
    dd->length=2;
    dd->tag=special_tag;
    dd->ref=ref;
    if(HI_WRITE(file_rec->file, tbuf, dd->length)==FAIL) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    /* update DD block in the file */
    if(FAIL==HIupdate_dd(file_rec,access_rec->block,access_rec->idx,FUNC)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTUPDATE,FAIL);
      } /* end if */

    /* add new DD to hash table */
    if(FAIL==HIadd_hash_dd(file_rec,dd->tag,dd->ref,access_rec->block,access_rec->idx)) {
        access_rec->used=FALSE;
        HRETURN_ERROR(DFE_CANTHASH,FAIL);
      } /* end if */

    /* get rid of the old DD, if there was one */
    if(data_dd!=NULL) {
        if(FAIL==Hdeldd(file_id, data_dd->tag, data_dd->ref)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_CANTDELDD,FAIL);
          } /* end if */
        if(FAIL==HIdel_hash_dd(file_rec, data_dd->tag, data_dd->ref)) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_CANTDELHASH,FAIL);
          } /* end if */
      } /* end if */

    /* update access record and file record */
    access_rec->special_func=&comp_funcs;
    access_rec->special=SPECIAL_COMP;
    access_rec->posn=0;
    access_rec->access=DFACC_WRITE;
    access_rec->file_id=file_id;

    file_rec->attach++;
    if(ref>file_rec->maxref)
        file_rec->maxref=ref;

    return(ASLOT2ID(slot));
}   /* end HCcreate */


/* ----------------------------- HCIstaccess ------------------------------ */
/*

  start accessing a data element
  called by HCIstread and HCIstwrite

  Return FAIL on error

-*/
#ifdef PROTOTYPE
PRIVATE int32 HCIstaccess(accrec_t *access_rec, int16 access)
#else
PRIVATE int32 HCIstaccess(access_rec, access)
    accrec_t *access_rec;   /* access record */
    int16 access;           /* access mode */
#endif
{
    char *FUNC="HCIstaccess";  /* for HERROR */
#ifdef QAK
    dd_t *info_dd;             /* dd of the special information element */
    extinfo_t *info;           /* special element information */
    filerec_t *file_rec;       /* file record */

    /* get file record and validate */

    file_rec=FID2REC(access_rec->file_id);
    if(!file_rec || file_rec->refcount==0 || !(file_rec->access & access)) {
       HERROR(DFE_ARGS);
       return FAIL;
    }

    /* intialize the access record */

    access_rec->special=SPECIAL_EXT;
    access_rec->posn=0;
    access_rec->access=access;

    /* get the dd for information */

    info_dd=&access_rec->block->ddlist[access_rec->idx];

    /* get the special info record */

    access_rec->special_info=HIgetspinfo(access_rec,
                                          info_dd->tag, info_dd->ref);
    if(access_rec->special_info) {

       /* found it from other access records */

       info=(extinfo_t *)access_rec->special_info;
       info->attached++;

    } else {

       /* look for information in the file */

       if(HI_SEEK(file_rec->file, info_dd->offset+2)==FAIL) {
           HERROR(DFE_SEEKERROR);
           access_rec->used=FALSE;
           return FAIL;
       }
       if(HI_READ(file_rec->file, tbuf, 12)==FAIL) {
           HERROR(DFE_READERROR);
           access_rec->used=FALSE;
           return FAIL;
       }
       access_rec->special_info=(VOIDP) HDgetspace((uint32)sizeof(extinfo_t));
       info=(extinfo_t *) access_rec->special_info;
       if(!info) {
           HERROR(DFE_NOSPACE);
           access_rec->used=FALSE;
           return FAIL;
       }
       {
           uint8 *p=tbuf;
           INT32DECODE(p, info->length);
           INT32DECODE(p, info->extern_offset);
           INT32DECODE(p, info->length_file_name);
       }
       info->extern_file_name=(char *)HDgetspace((uint32)
                                               info->length_file_name + 1);
       if(!info->extern_file_name) {
           HERROR(DFE_NOSPACE);
           access_rec->used=FALSE;
           return FAIL;
       }
       if(HI_READ(file_rec->file, info->extern_file_name,
                  info->length_file_name)==FAIL) {
           HERROR(DFE_READERROR);
           access_rec->used=FALSE;
           return FAIL;
       }
       info->extern_file_name[info->length_file_name]='\0';
       info->file_external=HI_OPEN(info->extern_file_name, access);
       if(OPENERR(info->file_external)) {
           HERROR(DFE_BADOPEN);
           access_rec->used=FALSE;
           return FAIL;
       }
       info->attached=1;
    }

    file_rec->attach++;

    return ASLOT2ID(access_rec-access_records);
#endif
}

/*- HCPstread
 start reading an compressed data element
-*/
#ifdef PROTOTYPE
int32 HCPstread(accrec_t *rec)
#else
int32 HCPstread(rec)
    accrec_t *rec;
#endif
{
    return HCIstaccess(rec, DFACC_READ);
}

/*- HCPstwrite
 start writing an compressed data element
-*/
#ifdef PROTOTYPE
int32 HCPstwrite(accrec_t *rec)
#else
int32 HCPstwrite(rec)
    accrec_t *rec;
#endif
{
    return HCIstaccess(rec, DFACC_WRITE);
}

/*- HCPseek
 seek to offset within the data element
-*/
#ifdef PROTOTYPE
int32 HCPseek(accrec_t *access_rec, int32 offset, int origin)
#else
int32 HCPseek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    int origin;
#endif
{
    char *FUNC="HCPseek";      /* for HERROR */

#ifdef QAK
    /* Adjust offset according to origin.
       there is no upper bound to posn */

    if(origin==DF_CURRENT) offset += access_rec->posn;
    if(origin==DF_END)
       offset += ((extinfo_t *)(access_rec->special_info))->length;
    if(offset < 0) {
       HERROR(DFE_RANGE);
       return FAIL;
    }

    /* set the offset */

    access_rec->posn=offset;
    return SUCCEED;
#endif
}

/*- HCPread
 read in a portion of data from the compressed element
-*/
#ifdef PROTOTYPE
int32 HCPread(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPread(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to read in */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPread";      /* for HERROR */
#ifdef QAK
    extinfo_t *info=         /* information on the special element */
       (extinfo_t *)access_rec->special_info;

    /* validate length */
    if(length < 0) {
       HERROR(DFE_RANGE);
       return FAIL;
    }

    /* adjust length if it falls off the end of the element */


    if(length==0) length=info->length - access_rec->posn;
    else
        if(length < 0 || access_rec->posn + length > info->length) {
           HERROR(DFE_RANGE);
           return FAIL;
        }

    /* read it in from the file */

    if(HI_SEEK(info->file_external, 
	       access_rec->posn + info->extern_offset)==FAIL) {
       HERROR(DFE_SEEKERROR);
       return FAIL;
    }
    if(HI_READ(info->file_external, data, length)==FAIL) {
       HERROR(DFE_READERROR);
       return FAIL;
    }

    /* adjust access position */

    access_rec->posn += length;

    return length;
#endif
}

/*- HCPwrite
 write a length of data to the element
-*/
#ifdef PROTOTYPE
int32 HCPwrite(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPwrite(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to write */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPwrite";     /* for HERROR */
#ifdef QAK
    extinfo_t *info=         /* information on the special element */
       (extinfo_t*)(access_rec->special_info);

    /* validate length */

    if(length < 0) {
       HERROR(DFE_RANGE);
       return FAIL;
    }

    /* write the data onto file */

    if(HI_SEEK(info->file_external, 
		access_rec->posn + info->extern_offset)==FAIL) {
       HERROR(DFE_SEEKERROR);
       return FAIL;
    }
    if(HI_WRITE(info->file_external, data, length)==FAIL) {

       /* this external file might not be opened with write permission,
          reopen the file and try again */

       hdf_file_t f=HI_OPEN(info->extern_file_name, DFACC_WRITE);
       if(OPENERR(f) || HI_SEEK(f, 
		   access_rec->posn + info->extern_offset)==FAIL ||
           HI_WRITE(f, data, length)==FAIL) {
           HERROR(DFE_DENIED);
           HI_CLOSE(f);
           return FAIL;
       }
       HI_CLOSE(info->file_external);

       /* if okay, substitute the file descriptor */

       info->file_external=f;
    }

    /* update access record, and information about special elelemt */

    access_rec->posn += length;
    if(access_rec->posn > info->length) {
       uint8 *p=     /* temp buffer ptr */
           tbuf;
       dd_t *info_dd=        /* dd of infromation element */
           &access_rec->block->ddlist[access_rec->idx];
       filerec_t *file_rec=  /* file record */
           FID2REC(access_rec->file_id);

       info->length=access_rec->posn;
       INT32ENCODE(p, info->length);
       if(HI_SEEK(file_rec->file, info_dd->offset+2)==FAIL) {
           HERROR(DFE_SEEKERROR);
           return FAIL;
       }
       if(HI_WRITE(file_rec->file, tbuf, 4)==FAIL) {
           HERROR(DFE_WRITEERROR);
           return FAIL;
       }
    }

    return length;
#endif
}


/* ------------------------------ HCPinquire ------------------------------ */
/*

 inquire information about the access record and data element

*/
#ifdef PROTOTYPE
int32 HCPinquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
                        uint16 *pref, int32 *plength, int32 *poffset,
                        int32 *pposn, int16 *paccess, int16 *pspecial)
#else
int32 HCPinquire(access_rec, pfile_id, ptag, pref, plength, poffset,
                        pposn, paccess, pspecial)
     accrec_t *access_rec;     /* access record */
     int32 *pfile_id;          /* ptr to file id, OUT */
     uint16 *ptag;             /* ptr to tag of information, OUT */
     uint16 *pref;             /* ptr to ref of information, OUT */
     int32 *plength;           /* ptr to length of data element, OUT */
     int32 *poffset;           /* ptr to offset of data element, OUT */
     int32 *pposn;             /* ptr to position of access in element, OUT */
     int16 *paccess;           /* ptr to access mode, OUT */
     int16 *pspecial;          /* ptr to special code */
#endif
{
#ifdef QAK
    dd_t *info_dd=           /* dd of special information */
       &(access_rec->block->ddlist[access_rec->idx]);
    extinfo_t *info=         /* special information record */
       (extinfo_t *)access_rec->special_info;

    /* fill in the variables if they are present */

    if(pfile_id) *pfile_id=access_rec->file_id;
    if(ptag) *ptag=info_dd->tag;
    if(pref) *pref=info_dd->ref;
    if(plength) *plength=info->length;
    if(poffset) *poffset=0; /* meaningless */
    if(pposn) *pposn=access_rec->posn;
    if(paccess) *paccess=access_rec->access;
    if(pspecial) *pspecial=access_rec->special;

    return SUCCEED;
#endif
}

/* ----------------------------- HCPendaccess ----------------------------- */
/*

  Close the file pointed to by the current AID and free the AID

*/
#ifdef PROTOTYPE
int32 HCPendaccess(accrec_t *access_rec)
#else
int32 HCPendaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPendaccess"; /* for HERROR */
#ifdef QAK
    filerec_t *file_rec=     /* file record */
       FID2REC(access_rec->file_id);

    /* close the file pointed to by this access rec */
    HXIcloseAID(access_rec);

    /* validate file record */

    if(file_rec==(filerec_t *) NULL || file_rec->refcount==0) {
       HERROR(DFE_INTERNAL);
       return FAIL;
    }

    /* detach from the file */

    file_rec->attach--;

    /* free the access record */

    access_rec->used=FALSE;

    return SUCCEED;
#endif
}