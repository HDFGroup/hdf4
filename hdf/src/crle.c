/*
 FILE
       crle.c
       HDF run-length encoding I/O routines

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
Revision 1.2  1993/10/06 20:27:20  koziol
More compression fixed, and folded Doug's suggested change into VSappendable.

 * Revision 1.1  1993/09/30  19:04:41  koziol
 * Added basic compressing functionality for special tags.
 *
 *
 */

/* General HDF includes */
#include "hdf.h"
#include "hfile.h"
#include "herr.h"

#define CRLE_MASTER
#define CODER_CLIENT
/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */

/* internal defines */
#define TMP_BUF_SIZE    8192    /* size of throw-away buffer */

/* declaration of the functions provided in this module */
PRIVATE int32 HCIcrle_staccess
    PROTO((accrec_t *access_rec, int16 access));

PRIVATE int32 HCIcrle_init
    PROTO((accrec_t *access_rec));

/*--------------------------------------------------------------------------
 NAME
    HCIcrle_init -- Initialize a RLE compressed data element.

 USAGE
    int32 HCIcrle_init(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcrle_staccess and HCIcrle_seek

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcrle_init(accrec_t *access_rec)
#else
PRIVATE int32 HCIcrle_init(access_rec)
    accrec_t *access_rec;   /* access record */
#endif
{
    char *FUNC="HCIcrle_init";          /* for HERROR */
    compinfo_t *info;                   /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    /* Initialize RLE state information */
    rle_info->rle_state=MIX;        /* start in mixed state */
    rle_info->buf_pos=0;            /* start at the beginning of the buffer */
    rle_info->last_byte=RLE_NIL;    /* start with no code in the last byte */
    rle_info->second_byte=RLE_NIL;  /* start with no code here too */
    rle_info->offset=0;             /* offset into the file */

    return(SUCCEED);
}   /* end HCIcrle_init() */

/*--------------------------------------------------------------------------
 NAME
    HCIcrle_decode -- Decode RLE compressed data into a buffer.

 USAGE
    int32 HCIcrle_decode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    uint32 length;      IN: number of bytes to read into the buffer
    uint8 *buf;         OUT: buffer to store the bytes read

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to decode RLE data from the file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcrle_decode(compinfo_t *info,uint32 length,uint8 *buf)
#else
PRIVATE int32 HCIcrle_decode(info,length,buf)
    compinfo_t *info;       /* compression info */
    uint32 length;          /* number of bytes to read in */
    uint8 *buf;             /* buffer to read data into */
#endif
{
    char *FUNC="HCIcrle_decode";        /* for HERROR */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */

    rle_info=&(info->cinfo.coder_info.rle_info);

    return(SUCCEED);
}   /* end HCIcrle_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcrle_staccess -- Start accessing a RLE compressed data element.

 USAGE
    int32 HCIcrle_staccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcrle_stread and HCIcrle_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcrle_staccess(accrec_t *access_rec, int16 access)
#else
PRIVATE int32 HCIcrle_staccess(access_rec, access)
    accrec_t *access_rec;   /* access record */
    int16 access;           /* access mode */
#endif
{
    char *FUNC="HCIcrle_staccess";      /* for HERROR */
    compinfo_t *info;                   /* special element information */

    info=(compinfo_t *)access_rec->special_info;

    if(access==DFACC_READ)
        info->aid=Hstartread(access_rec->file_id,DFTAG_COMPRESSED,
                info->comp_ref);
    else
        info->aid=Hstartwrite(access_rec->file_id,DFTAG_COMPRESSED,
                info->comp_ref,info->length);

    if(info->aid==FAIL)
        HRETURN_ERROR(DFE_DENIED,FAIL);
    return(HCIcrle_init(access_rec));   /* initialize the RLE info */
}   /* end HCIcrle_staccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_stread -- start read access for compressed file

 USAGE
    int32 HCPmstdio_stread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start read access on a compressed data element using a simple RLE scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_stread(accrec_t *access_rec)
#else
int32 HCPcrle_stread(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPcrle_stread";     /* for HERROR */
    int32 ret;

    if((ret=HCIcrle_staccess(access_rec, DFACC_READ))==FAIL)
        HRETURN_ERROR(DFE_CINIT,FAIL);
    return(ret);
}   /* HCPcrle_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_stwrite -- start write access for compressed file

 USAGE
    int32 HCPmstdio_stwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start write access on a compressed data element using a simple RLE scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_stwrite(accrec_t *access_rec)
#else
int32 HCPcrle_stwrite(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPcrle_stwrite";     /* for HERROR */
    int32 ret;

    if((ret=HCIcrle_staccess(access_rec, DFACC_WRITE))==FAIL)
        HRETURN_ERROR(DFE_CINIT,FAIL);
    return(ret);
}   /* HCPcrle_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_seek -- Seek to offset within the data element

 USAGE
    int32 HCPcrle_seek(access_rec,offset,origin)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 offset;       IN: the offset in bytes from the origin specified
    intn origin;        IN: the origin to seek from [UNUSED!]

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Seek to a position with a compressed data element.  The 'origin'
    calculations have been taken care of at a higher level, it is an
    un-used parameter.  The 'offset' is used as an absolute offset
    because of this.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_seek(accrec_t *access_rec, int32 offset, int origin)
#else
int32 HCPcrle_seek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    int origin;
#endif
{
    char *FUNC="HCPcrle_seek";      /* for HERROR */
    compinfo_t *info;                   /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */
    uint8 *tmp_buf;                 /* pointer to throw-away buffer */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    if(offset<rle_info->offset)    /* need to seek from the beginning */
        HCIcrle_init(access_rec);

    if((tmp_buf=(uint8 *)HDgetspace(TMP_BUF_SIZE))==NULL)   /* get tmp buffer */
        HRETURN_ERROR(DFE_NOSPACE,FAIL);

    while(rle_info->offset+TMP_BUF_SIZE<offset)     /* grab chunks */
        if(HCIcrle_decode(info,TMP_BUF_SIZE,tmp_buf)==FAIL) {
            HDfreespace(tmp_buf);
            HRETURN_ERROR(DFE_CDECODE,FAIL);
          } /* end if */
    if(rle_info->offset<offset)             /* grab the last chunk */
        if(HCIcrle_decode(info,offset-rle_info->offset,tmp_buf)==FAIL) {
            HDfreespace(tmp_buf);
            HRETURN_ERROR(DFE_CDECODE,FAIL);
          } /* end if */

    HDfreespace(tmp_buf);
    return SUCCEED;
}   /* HCPcrle_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_read --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_read(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPcrle_read(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to read in */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPcrle_read";      /* for HERROR */
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
}   /* HCPcrle_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_write --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_write(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPcrle_write(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to write */
    VOIDP data;                        /* data buffer */
#endif
{
    char *FUNC="HCPcrle_write";     /* for HERROR */
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
}   /* HCPcrle_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_inquire --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
                        uint16 *pref, int32 *plength, int32 *poffset,
                        int32 *pposn, int16 *paccess, int16 *pspecial)
#else
int32 HCPcrle_inquire(access_rec, pfile_id, ptag, pref, plength, poffset,
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
}   /* HCPcrle_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_endaccess --

 USAGE

 RETURNS

 DESCRIPTION

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_endaccess(accrec_t *access_rec)
#else
int32 HCPcrle_endaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPcrle_endaccess"; /* for HERROR */
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
}   /* HCPcrle_endaccess() */
