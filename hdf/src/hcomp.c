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
    9/21/93     Starting writing specs & coding prototype
*/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.6.2.1  1993/10/10 22:10:12  koziol
Moved Tag descriptions into a header file.  Updated compression routines.

 * Revision 1.6  1993/10/06  20:27:30  koziol
 * More compression fixed, and folded Doug's suggested change into VSappendable.
 *
 * Revision 1.5  1993/10/04  20:02:43  koziol
 * Updated error reporting in H-Layer routines, and added more error codes and
 * compression stuff.
 *
 * Revision 1.4  1993/10/01  20:00:54  koziol
 * Put "extern C" block around function prototypes for C++ compatibility.
 *
 * Revision 1.3  1993/09/30  19:05:00  koziol
 * Added basic compressing functionality for special tags.
 *
 * Revision 1.2  1993/09/28  18:44:12  koziol
 * Fixed various things the Sun's pre-processor didn't like.
 *
 * Revision 1.1  1993/09/28  18:04:21  koziol
 * Removed OLD_WAY & QAK ifdef's.  Removed oldspecial ifdef's for special
 * tag handling.  Added new compression special tag type.
 *
 *
 */

/* General HDF includes */
#define COMPRESS_MASTER
#include "hdf.h"
#include "hfile.h"
#include "herr.h"

/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */

/* Local defines */
#define COMP_HEADER_VERSION 0
#define COMP_HEADER_LENGTH  14
#define COMP_START_BLOCK    1

/* declaration of the functions provided in this module */
PRIVATE int32 HCIstaccess
    PROTO((accrec_t *access_rec, int16 access));

PRIVATE int32 HCIinit_coder
    PROTO((comp_coder_info_t *cinfo,comp_coder_t coder_type));

PRIVATE int32 HCIinit_model
    PROTO((comp_model_info_t *minfo,comp_model_t model_type));

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
    HCPendaccess
};

/*--------------------------------------------------------------------------
 NAME
    HCIinit_coder -- Set the coder function pointers

 USAGE
    int32 HCIinit_coder(cinfo,coder_type)
    comp_coder_info_t *cinfo;   IN/OUT: pointer to coder information to modify
    comp_coder_t coder_type;    IN: the type of encoding to use

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Sets the encoder function pointers and the encoder type for a given
    coder type.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIinit_coder(comp_coder_info_t *cinfo,comp_coder_t coder_type)
#else
PRIVATE int32 HCIinit_coder(cinfo, coder_type)
    comp_coder_info_t *cinfo;   /* encoding information to modify */
    comp_coder_t coder_type;     /* type of encoding to use */
#endif
{
    char *FUNC="HCIsetcoder_funcs";  /* for HERROR */

    switch(coder_type) {    /* determin the type of encoding */
        case COMP_CODE_RLE:           /* Run-length encoding */
#ifdef TESTING
printf("HCIinit_coder(): RLE coder type\n");
#endif
            cinfo->coder_type=COMP_CODE_RLE;    /* set coding type */
            cinfo->coder_funcs=crle_funcs;      /* set the RLE func. ptrs */
#ifdef QAK
            HCPinit_crle(&(cinfo->coder_info.rle_info));  /* initialize the coder */
#endif
            break;

        default:
           HRETURN_ERROR(DFE_BADCODER,FAIL);
      } /* end switch */
    return(SUCCEED);
}   /* end HCIinit_coder() */

/*--------------------------------------------------------------------------
 NAME
    HCIinit_model -- Set the model function pointers

 USAGE
    int32 HCIinit_model(minfo,model_type)
    comp_model_info_t *minfo;   IN/OUT: pointer to model information to modify
    comp_model_t model_type;    IN: the type of encoding to use

 RETURNS
    Return SUCCEED or FAIL

 DESCRIPTION
    Sets the modeling function pointers and the model type for a given
    model type.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIinit_model(comp_model_info_t *minfo,comp_model_t model_type)
#else
PRIVATE int32 HCIinit_model(minfo, model_type)
    comp_model_info_t *minfo;   /* modeling information to modify */
    comp_model_t model_type;    /* type of modeling to use */
#endif
{
    char *FUNC="HCIinit_model";  /* for HERROR */

    switch(model_type) {    /* determine the type of modeling */
        case COMP_MODEL_STDIO:        /* standard C stdio modeling */
#ifdef TESTING
printf("HCIinit_model(): stdio model type\n");
#endif
            minfo->model_type=COMP_MODEL_STDIO;    /* set model type */
            minfo->model_funcs=mstdio_funcs;    /* set the stdio func. ptrs */
#ifdef QAK
            HCPinit_mstdio(&(minfo->model_info.stdio_info),0);  /* initialize the model */
#endif
            break;

        default:
           HRETURN_ERROR(DFE_BADMODEL,FAIL);
      } /* end switch */

    return(SUCCEED);
}   /* end HCIinit_model() */

/*--------------------------------------------------------------------------
 NAME
    HCcreate -- Create a compressed data element

 USAGE
    int32 HCcreate(id,tag,ref,model_type,coder_type)
    int32 id;            IN: the file id to create the data in
    uint16 tag,ref;      IN: the tag/ref pair which is to be compressed
    intn model_type;     IN: the type of modeling to use
    intn coder_type;     IN: the type of encoding to use

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
int32 HCcreate(int32 file_id, uint16 tag, uint16 ref, comp_model_t model_type,
        comp_coder_t coder_type)
#else
int32 HCcreate(file_id, tag, ref, model_type, coder_type)
    int32 file_id;      /* file record id */
    uint16 tag, ref;    /* tag/ref of the special data element to create */
    comp_model_t model_type;    /* type of modeling to use */
    comp_coder_t coder_type;    /* type of encoding to use */
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
    uint8 *p;               /* pointer into the temp. buffer */
    int32 ret;
    uint16 ctype,mtype;     /* temp. storage for coder and model types */

    /* clear error stack and validate args */
    HEclear();
    file_rec=FID2REC(file_id);
    if(file_rec==NULL || file_rec->refcount==0 || SPECIALTAG(tag)
            || (special_tag=MKSPECIALTAG(tag))==DFTAG_NULL)
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
#else
        HRETURN_ERROR(DFE_UNSUPPORTED,FAIL);
#endif
      } /* end if */
    else {  /* start new compressed data element */
        info->length=COMP_START_BLOCK;
      } /* end else */

    /* set up compressed special info structure */
    info->attached=1;
    info->comp_ref=Hnewref(file_id);         /* get the new reference # */
    HCIinit_model(&(info->minfo),model_type);
    HCIinit_coder(&(info->cinfo),coder_type);

    /* write special element info to the file */
    p=tbuf;
    INT16ENCODE(p, SPECIAL_COMP);           /* specify special tag type */
    UINT16ENCODE(p, COMP_HEADER_VERSION);   /* specify header version */
    INT32ENCODE(p, info->length);           /* write length of un-comp. data */
    UINT16ENCODE(p, (uint16)info->comp_ref);/* specify ref # of comp. data */
    mtype=(uint16)model_type;
    UINT16ENCODE(p, (uint16)mtype);    /* specify model type stored */
    ctype=(uint16)coder_type;
    UINT16ENCODE(p, (uint16)ctype);    /* specify coder type stored */
    if(HI_SEEKEND(file_rec->file)==FAIL) {
       access_rec->used=FALSE;
       HRETURN_ERROR(DFE_SEEKERROR,FAIL);
      } /* end if */

    /* write compressed special element data to the file */
    dd->offset=HI_TELL(file_rec->file);
    dd->length=COMP_HEADER_LENGTH;
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
    access_rec->appendable=FALSE;   /* start data as non-appendable */
    access_rec->flush=FALSE;        /* start data as not needing flushing */

    file_rec->attach++;
    if(ref>file_rec->maxref)
        file_rec->maxref=ref;

    /* propagate the initialization down to the modeling layer */
    if((ret=(*(info->minfo.model_funcs.stwrite))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    return(ASLOT2ID(slot));
}   /* end HCcreate() */

/*--------------------------------------------------------------------------
 NAME
    HCIstaccess -- Start accessing a compressed data element.

 USAGE
    int32 HCIstaccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Common code called by HCIstread and HCIstwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIstaccess(accrec_t *access_rec, int16 access)
#else
PRIVATE int32 HCIstaccess(access_rec, access)
    accrec_t *access_rec;   /* access record */
    int16 access;           /* access mode */
#endif
{
    char *FUNC="HCIstaccess";   /* for HERROR */
    dd_t *info_dd;              /* dd of the special information element */
    compinfo_t *info;           /* special element information */
    filerec_t *file_rec;        /* file record */
    uint16 header_version;      /* version of the compression header */
    uint16 mtype,ctype;         /* temporary variables for model and coder type */
    comp_model_t model_type;    /* type of modeling to use */
    comp_coder_t coder_type;    /* type of encoding to use */
    int32 ret;

    /* get file record and validate */
    file_rec=FID2REC(access_rec->file_id);
    if(!file_rec || file_rec->refcount==0 || !(file_rec->access & access))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* intialize the access record */
    access_rec->special=SPECIAL_COMP;
    access_rec->posn=0;
    access_rec->access=access;

    /* get the dd for information */
    info_dd=&access_rec->block->ddlist[access_rec->idx];

    /* get the special info record */
#ifdef OLD_WAY
/* this won't work because the compression state will get screwed up */
    access_rec->special_info=HIgetspinfo(access_rec,info_dd->tag,info_dd->ref);
    if(access_rec->special_info) {  /* found it from other access records */
        info=(compinfo_t *)access_rec->special_info;
        info->attached++;
      } /* end if */
    else {  /* look for information in the file */
#endif
        if(HI_SEEK(file_rec->file, info_dd->offset+2)==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        }
        if((ret=HI_READ(file_rec->file, tbuf, (COMP_HEADER_LENGTH-2)))==FAIL) {
            access_rec->used=FALSE;
            HRETURN_ERROR(DFE_READERROR,FAIL);
        }
        access_rec->special_info=(VOIDP)HDgetspace(sizeof(compinfo_t));
        info=(compinfo_t *)access_rec->special_info;
        if(info==NULL) {
           access_rec->used=FALSE;
           HRETURN_ERROR(DFE_NOSPACE,FAIL);
        }
        {
            uint8 *p=tbuf;
            UINT16DECODE(p, header_version);    /* get header length */
            INT32DECODE(p, info->length);   /* get _uncompressed_ data length */
            UINT16DECODE(p, info->comp_ref);    /* get ref # of comp. data */
            UINT16DECODE(p, mtype);             /* get model type */
	    model_type=(comp_model_t)mtype;
            UINT16DECODE(p, ctype);            /* get encoding type */
	    coder_type=(comp_coder_t)ctype;
        }
        info->attached=1;
        if(HCIinit_model(&(info->minfo),model_type)==FAIL)
	    HRETURN_ERROR(DFE_MINIT,FAIL);
        if(HCIinit_coder(&(info->cinfo),coder_type)==FAIL)
	    HRETURN_ERROR(DFE_CINIT,FAIL);
#ifdef OLD_WAY
      } /* end else */
#endif

    file_rec->attach++;

    return(ASLOT2ID(access_rec-access_records));
}   /* end HCIstaccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPstread -- Start read access on a compressed data element.

 USAGE
    int32 HCPstread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Start read access on a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPstread(accrec_t *access_rec)
#else
int32 HCPstread(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPstread";     /* for HERROR */
    compinfo_t *info;           /* information on the special element */
    int32 aid;                  /* AID to return */
    int32 ret;

    if((aid=HCIstaccess(access_rec, DFACC_READ))==FAIL)
        HRETURN_ERROR(DFE_DENIED,FAIL);
    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.stread))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);
    return(aid);
}   /* end HCPstread() */

/*--------------------------------------------------------------------------
 NAME
    HCPstwrite -- Start write access on a compressed data element.

 USAGE
    int32 HCPstwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns an AID or FAIL

 DESCRIPTION
    Start write access on a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPstwrite(accrec_t *access_rec)
#else
int32 HCPstwrite(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPstwrite";    /* for HERROR */
    compinfo_t *info;           /* information on the special element */
    int32 aid;                  /* AID to return */
    int32 ret;

    if((aid=HCIstaccess(access_rec, DFACC_WRITE))==FAIL)
        HRETURN_ERROR(DFE_DENIED,FAIL);
    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.stwrite))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);
    return(aid);
}   /* end HCPstwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPseek -- Seek to offset within the data element

 USAGE
    int32 HCPseek(access_rec,offset,origin)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 offset;       IN: the offset in bytes from the origin specified
    intn origin;        IN: the origin to seek from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Seek to a position with a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPseek(accrec_t *access_rec, int32 offset, intn origin)
#else
int32 HCPseek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    intn origin;
#endif
{
    char *FUNC="HCPseek";   /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    /* Adjust offset according to origin.  There is no upper bound to posn */
    if(origin==DF_CURRENT)
        offset+=access_rec->posn;
    if(origin==DF_END)
        offset+=((compinfo_t *)(access_rec->special_info))->length;
    if(offset<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.seek))(access_rec,offset,origin))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* set the offset */
    access_rec->posn=offset;

    return(ret);
}   /* end HCPseek() */

/*--------------------------------------------------------------------------
 NAME
    HCPread -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPread(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPread(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPread(access_rec, length, data)
    accrec_t *access_rec;   /* access record */
    int32 length;           /* length of data to read in */
    VOIDP data;             /* data buffer */
#endif
{
    char *FUNC="HCPread";   /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    /* validate length */
    if(length<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    info=(compinfo_t *)access_rec->special_info;

    /* adjust length if it falls off the end of the element */
    if(length==0) {
        length=info->length-access_rec->posn;
#ifdef QAK
printf("length=%d, info->length=%d, access_rec->posn=%d\n",length,info->length,access_rec->posn);
#endif
      } /* end if */
    else
        if(length<0 || access_rec->posn+length>info->length)
           HRETURN_ERROR(DFE_RANGE,FAIL);

    if((ret=(*(info->minfo.model_funcs.read))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* adjust access position */
    access_rec->posn+=length;

    return(length);
}   /* end HCPread() */

/*--------------------------------------------------------------------------
 NAME
    HCPwrite -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPwrite(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to write
    VOIDP data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to a compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
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
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    /* validate length */
    if(length<0)
       HRETURN_ERROR(DFE_RANGE,FAIL);

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.write))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* update access record, and information about special element */
    access_rec->posn+=length;
    if(access_rec->posn>info->length) {
        uint8 *p=tbuf;           /* temp buffer ptr */
        dd_t *info_dd=           /* dd of infromation element */
            &access_rec->block->ddlist[access_rec->idx];
        filerec_t *file_rec=FID2REC(access_rec->file_id);    /* file record */

        info->length=access_rec->posn;
        INT32ENCODE(p, info->length);
        if(HI_SEEK(file_rec->file, info_dd->offset+4)==FAIL)
            HRETURN_ERROR(DFE_SEEKERROR,FAIL);
        if(HI_WRITE(file_rec->file, tbuf, 4)==FAIL) /* re-write un-comp. len */
            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
      } /* end if */

    return(length);
}   /* end HCPwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPinquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPinquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
            paccess,pspecial)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 *pfile_id;        OUT: ptr to file id
    uint16 *ptag;           OUT: ptr to tag of information
    uint16 *pref;           OUT: ptr to ref of information
    int32 *plength;         OUT: ptr to length of data element
    int32 *poffset;         OUT: ptr to offset of data element
    int32 *pposn;           OUT: ptr to position of access in element
    int16 *paccess;         OUT: ptr to access mode
    int16 *pspecial;        OUT: ptr to special code

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Inquire information about the access record and data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPinquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
    uint16 *pref, int32 *plength, int32 *poffset,int32 *pposn,int16 *paccess,
    int16 *pspecial)
#else
int32 HCPinquire(access_rec, pfile_id, ptag, pref, plength, poffset, pposn,
            paccess, pspecial)
    accrec_t *access_rec;   /* access record */
    int32 *pfile_id;        /* ptr to file id, OUT */
    uint16 *ptag;           /* ptr to tag of information, OUT */
    uint16 *pref;           /* ptr to ref of information, OUT */
    int32 *plength;         /* ptr to length of data element, OUT */
    int32 *poffset;         /* ptr to offset of data element, OUT */
    int32 *pposn;           /* ptr to position of access in element, OUT */
    int16 *paccess;         /* ptr to access mode, OUT */
    int16 *pspecial;        /* ptr to special code, OUT */
#endif
{
    dd_t *info_dd=           /* dd of special information */
       &(access_rec->block->ddlist[access_rec->idx]);
    compinfo_t *info=         /* special information record */
       (compinfo_t *)access_rec->special_info;

    /* fill in the variables if they are present */
    if(pfile_id!=NULL)
        *pfile_id=access_rec->file_id;
    if(ptag!=NULL)
        *ptag=info_dd->tag;
    if(pref!=NULL)
        *pref=info_dd->ref;
    if(plength!=NULL)
        *plength=info->length;
    if(poffset!=NULL)
        *poffset=info_dd->offset;
    if(pposn!=NULL)
        *pposn=access_rec->posn;
    if(paccess!=NULL)
        *paccess=access_rec->access;
    if(pspecial!=NULL)
        *pspecial=access_rec->special;

    return(SUCCEED);
}   /* end HCPinquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPendaccess -- Close the compressed data element and free the AID

 USAGE
    int32 HCPendaccess(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Close the compressed data element and free the AID.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPendaccess(accrec_t *access_rec)
#else
int32 HCPendaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPendaccess";      /* for HERROR */
    filerec_t *file_rec=FID2REC(access_rec->file_id);   /* file record */

    /* validate file record */
    if(file_rec==(filerec_t *) NULL || file_rec->refcount==0)
       HRETURN_ERROR(DFE_INTERNAL,FAIL);

    /* close the file pointed to by this access rec */
    HCPcloseAID(access_rec);

    /* detach from the file */
    file_rec->attach--;

    /* free the access record */
    access_rec->used=FALSE;

    return(SUCCEED);
}   /* end HCPendaccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPcloseAID -- Get rid of the compressed data element data structures

 USAGE
    int32 HCPcloseAID(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Get rid of the compressed data element internal data structures

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcloseAID(accrec_t *access_rec)
#else
int32 HCPcloseAID(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPcloseAID"; /* for HERROR */
    compinfo_t *info;         /* special information record */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->minfo.model_funcs.endaccess))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_MODEL,FAIL);

    /* Free the compression information */
    if(--(info->attached)==0) {
        HDfreespace(info);
      } /* end if */

    return(SUCCEED);
}   /* end HCPcloseAID() */

