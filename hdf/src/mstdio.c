/*
 FILE
       mstdio.c
       HDF stdio-like modeling I/O routines

 REMARKS

 DESIGN

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
    9/28/93     Starting writing specs & coding prototype
*/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/09/30 19:05:21  koziol
Added basic compressing functionality for special tags.

 *
 */

/* General HDF includes */
#include "hdf.h"
#include "hfile.h"
#include "herr.h"

#define MSTDIO_MASTER
/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */
#include "mstdio.h"         /* stdio modeling header */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_stread --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_stread(accrec_t *rec)
#else
int32 HCPmstdio_stread(rec)
    accrec_t *rec;
#endif
{
}   /* HCPmstdio_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_stwrite --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_stwrite(accrec_t *rec)
#else
int32 HCPmstdio_stwrite(rec)
    accrec_t *rec;
#endif
{
}   /* HCPmstdio_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_seek --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_seek(accrec_t *access_rec, int32 offset, int origin)
#else
int32 HCPmstdio_seek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    int origin;
#endif
{
    char *FUNC="HCPmstdio_seek";      /* for HERROR */

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
}   /* HCPmstdio_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_read --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_read(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPmstdio_read(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to read in */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPmstdio_read";      /* for HERROR */
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
}   /* HCPmstdio_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_write --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_write(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPmstdio_write(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to write */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPmstdio_write";     /* for HERROR */
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
}   /* HCPmstdio_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_inquire --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
                        uint16 *pref, int32 *plength, int32 *poffset,
                        int32 *pposn, int16 *paccess, int16 *pspecial)
#else
int32 HCPmstdio_inquire(access_rec, pfile_id, ptag, pref, plength, poffset,
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
}   /* HCPmstdio_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_endaccess --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_endaccess(accrec_t *access_rec)
#else
int32 HCPmstdio_endaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPmstdio_endaccess"; /* for HERROR */
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
}   /* HCPmstdio_endaccess() */
