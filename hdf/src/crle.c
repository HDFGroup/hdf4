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
Revision 1.2.2.1  1993/10/10 22:09:59  koziol
Moved Tag descriptions into a header file.  Updated compression routines.

 * Revision 1.2  1993/10/06  20:27:20  koziol
 * More compression fixed, and folded Doug's suggested change into VSappendable.
 *
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
#define RUN_MASK        0x80    /* bit mask for run-length control bytes */
#define COUNT_MASK      0x7f    /* bit mask for count of run or mix */

/* declaration of the functions provided in this module */
PRIVATE int32 HCIcrle_staccess
    PROTO((accrec_t *access_rec, int16 access));

PRIVATE int32 HCIcrle_init
    PROTO((accrec_t *access_rec));

PRIVATE int32 HCIcrle_decode
    PROTO((compinfo_t *info,int32 length,uint8 *buf));

PRIVATE int32 HCIcrle_encode
    PROTO((compinfo_t *info,int32 length,uint8 *buf));

PRIVATE int32 HCIcrle_term
    PROTO((compinfo_t *info));

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
    if(Hseek(info->aid,0,DF_START)==FAIL)   /* seek to beginning of element */
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);

    rle_info=&(info->cinfo.coder_info.rle_info);

    /* Initialize RLE state information */
    rle_info->rle_state=INIT;       /* start in initial state */
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
    int32 length;       IN: number of bytes to read into the buffer
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
PRIVATE int32 HCIcrle_decode(compinfo_t *info,int32 length,uint8 *buf)
#else
PRIVATE int32 HCIcrle_decode(info,length,buf)
    compinfo_t *info;       /* compression info */
    int32 length;           /* number of bytes to read in */
    uint8 *buf;             /* buffer to read data into */
#endif
{
    char *FUNC="HCIcrle_decode";        /* for HERROR */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */
    int32 orig_length;      /* original length to write */
    intn dec_len;           /* length to decode */
    intn c;                 /* character to hold a byte read in */

    rle_info=&(info->cinfo.coder_info.rle_info);

    orig_length=length;     /* save this for later */
    while(length>0) {   /* decode until we have all the bytes we need */
        if(rle_info->rle_state==INIT) { /* need to figure out RUN or MIX state */
            if((c=HDgetc(info->aid))==FAIL)
                HRETURN_ERROR(DFE_READERROR,FAIL);
            if(c&RUN_MASK) {   /* run byte */
                rle_info->rle_state=RUN;    /* set to run state */
                rle_info->buf_length=(c&COUNT_MASK)+RLE_MIN_RUN; /* run length */
                if((rle_info->last_byte=HDgetc(info->aid))==FAIL)
                    HRETURN_ERROR(DFE_READERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_decode(): INIT - found a run of %d bytes\n",rle_info->buf_length);
#endif
              } /* end if */
            else {      /* mix byte */
                rle_info->rle_state=MIX;    /* set to mix state */
                rle_info->buf_length=(c&COUNT_MASK)+RLE_MIN_MIX; /* mix length */
                if(Hread(info->aid,rle_info->buf_length,rle_info->buffer)==FAIL)
                    HRETURN_ERROR(DFE_READERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_decode(): INIT - found a mix of %d bytes\n",rle_info->buf_length);
#endif
                rle_info->buf_pos=0;
              } /* end else */
          } /* end if */
        else {  /* RUN or MIX states */
            if(length>rle_info->buf_length)  /* still need more data */
                dec_len=rle_info->buf_length;
            else        /* only grab "length" bytes */
                dec_len=length;

            if(rle_info->rle_state==RUN) {
#ifdef TESTING
printf("HCPcrle_decode(): RUN - decoding a run of %d bytes\n",dec_len);
#endif
                HDmemset(buf,rle_info->last_byte,dec_len);  /* copy the run */
              } /* end if */
            else {
#ifdef TESTING
printf("HCPcrle_decode(): MIX - decoding a mix of %d bytes\n",dec_len);
#endif
                HDmemcpy(buf,&(rle_info->buffer[rle_info->buf_pos]),dec_len);
                rle_info->buf_pos+=dec_len;
              } /* end else */

            rle_info->buf_length-=dec_len;
            if(rle_info->buf_length<=0) /* check for running out of bytes */
                rle_info->rle_state=INIT;   /* get the next status byte */
            length-=dec_len;    /* decrement the bytes to get */
            buf+=dec_len;       /* in case we need more bytes */
          } /* end else */
      } /* end while */

    rle_info->offset+=orig_length;  /* incr. abs. offset into the file */
    return(SUCCEED);
}   /* end HCIcrle_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcrle_encode -- Encode data from a buffer into RLE compressed data

 USAGE
    int32 HCIcrle_encode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to store from the buffer
    uint8 *buf;         OUT: buffer to get the bytes from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to encode RLE data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcrle_encode(compinfo_t *info,int32 length,uint8 *buf)
#else
PRIVATE int32 HCIcrle_encode(info,length,buf)
    compinfo_t *info;       /* compression info */
    int32 length;           /* number of bytes to write out */
    uint8 *buf;             /* buffer to read data from */
#endif
{
    char *FUNC="HCIcrle_encode";        /* for HERROR */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */
    int32 orig_length;      /* original length to write */
    intn dec_len;           /* length to decode */
    intn c;                 /* character to hold a byte read in */

    rle_info=&(info->cinfo.coder_info.rle_info);

    orig_length=length;     /* save this for later */
    while(length>0) {   /* encode until we stored all the bytes */
        switch(rle_info->rle_state) {
            case INIT:  /* initial encoding state */
#ifdef TESTING
printf("HCPcrle_encode(): INIT - shifting to MIX state, length=%d, buf=%p\n",length,buf);
#endif
                rle_info->rle_state=MIX;    /* shift to MIX state */
                rle_info->last_byte=rle_info->buffer[0]=*buf;
                rle_info->buf_length=1;
                rle_info->buf_pos=1;
                buf++;
                length--;
                break;

            case RUN:
                /* check for end of run */
                if(*buf!=rle_info->last_byte) {
                    rle_info->rle_state=MIX;
                    c=RUN_MASK|(rle_info->buf_length-RLE_MIN_RUN);
                    if(HDputc((uint8)c,info->aid)==FAIL)
                        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                    if(HDputc((uint8)rle_info->last_byte,info->aid)==FAIL)
                        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_encode(): RUN - shifting to MIX state, wrote run of %d bytes\n",rle_info->buf_length);
#endif
                    rle_info->last_byte=rle_info->buffer[0]=*buf;
                    rle_info->buf_length=1;
                    rle_info->buf_pos=1;
                  } /* end if */
                else {  /* run is continuing */
                    rle_info->buf_length++;
                    if(rle_info->buf_length>=RLE_MAX_RUN) {    /* check for too long */
                        c=RUN_MASK|(rle_info->buf_length-RLE_MIN_RUN);
                        if(HDputc((uint8)c,info->aid)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                        if(HDputc((uint8)rle_info->last_byte,info->aid)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_encode(): RUN - shifting to INIT state, wrote run of %d bytes\n",rle_info->buf_length);
#endif
                        rle_info->rle_state=INIT;
                        rle_info->second_byte=rle_info->last_byte=RLE_NIL;
                      } /* end if */
                  } /* end else */
                buf++;
                length--;
                break;

            case MIX:   /* mixed bunch of bytes */
                /* check for run */
#ifdef TESTING2
printf("HCPcrle_encode(): MIX - buf=%p, length=%d\n",buf,length);
#endif
                if(*buf==rle_info->last_byte && *buf==rle_info->second_byte) {
#ifdef TESTING2
printf("HCPcrle_encode(): MIX - shifting to RUN state, buf_length=%d buf_pos=%d\n",rle_info->buf_length,rle_info->buf_pos);
#endif
                    rle_info->rle_state=RUN;    /* shift to RUN state */
                    if(rle_info->buf_length>(RLE_MIN_RUN-1)) {  /* check for mixed data to write */
                        if(HDputc((uint8)((rle_info->buf_length-RLE_MIN_MIX)-(RLE_MIN_RUN-1)),info->aid)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                        if(Hwrite(info->aid,(rle_info->buf_length-(RLE_MIN_RUN-1)),rle_info->buffer)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_encode(): MIX - shifting to RUN state, wrote mix of %d bytes\n",rle_info->buf_length-(RLE_MIN_RUN-1));
#endif
                      } /* end if */
                    else {
#ifdef TESTING
printf("HCPcrle_encode(): MIX - shifting to RUN state, have run of %d bytes\n",rle_info->buf_length);
#endif
                      } /* end else */
                    rle_info->buf_length=RLE_MIN_RUN;
                  } /* end if */
                else {  /* continue MIX */
#ifdef TESTING2
printf("HCPcrle_encode(): MIX - continuing MIX state, buf_length=%d buf_pos=%d\n",rle_info->buf_length,rle_info->buf_pos);
#endif
                    rle_info->second_byte=rle_info->last_byte;
                    rle_info->last_byte=rle_info->buffer[rle_info->buf_pos]=*buf;
                    rle_info->buf_length++;
                    rle_info->buf_pos++;
                    if(rle_info->buf_length>=RLE_BUF_SIZE) {    /* check for too long */
                        if(HDputc((uint8)(rle_info->buf_length-RLE_MIN_MIX),info->aid)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                        if(Hwrite(info->aid,rle_info->buf_length,rle_info->buffer)==FAIL)
                            HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_encode(): MIX - shifting to INIT state, write mix of %d bytes\n",rle_info->buf_length);
#endif
                        rle_info->rle_state=INIT;
                        rle_info->second_byte=rle_info->last_byte=RLE_NIL;
                      } /* end if */
                  } /* end else */
                buf++;
                length--;
                break;

            default:
                HRETURN_ERROR(DFE_INTERNAL,FAIL);
          } /* end switch */
      } /* end while */

    rle_info->offset+=orig_length;  /* incr. abs. offset into the file */
    return(SUCCEED);
}   /* end HCIcrle_encode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcrle_term -- Flush encoded data from internal buffer to RLE compressed data

 USAGE
    int32 HCIcrle_term(info)
    compinfo_t *info;   IN: the info about the compressed element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to flush RLE data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcrle_term(compinfo_t *info)
#else
PRIVATE int32 HCIcrle_term(info)
    compinfo_t *info;       /* compression info */
#endif
{
    char *FUNC="HCIcrle_term";          /* for HERROR */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */
    intn dec_len;           /* length to decode */
    intn c;                 /* character to hold a byte read in */

    rle_info=&(info->cinfo.coder_info.rle_info);

    switch(rle_info->rle_state) {
        case RUN:
            c=RUN_MASK|(rle_info->buf_length-RLE_MIN_RUN);
            if(HDputc((uint8)c,info->aid)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
            if(HDputc((uint8)rle_info->last_byte,info->aid)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_term(): RUN - wrote run of %d bytes\n",rle_info->buf_length);
#endif
            break;

        case MIX:   /* mixed bunch of bytes */
            if(HDputc((uint8)((rle_info->buf_length-RLE_MIN_MIX)),info->aid)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
            if(Hwrite(info->aid,rle_info->buf_length,rle_info->buffer)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcrle_term(): MIX - wrote mix of %d bytes\n",rle_info->buf_length);
#endif
            break;

        default:
            HRETURN_ERROR(DFE_INTERNAL,FAIL);
      } /* end switch */
    rle_info->rle_state=INIT;
    rle_info->second_byte=rle_info->last_byte=RLE_NIL;

    return(SUCCEED);
}   /* end HCIcrle_term() */

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

    if(offset<rle_info->offset) {    /* need to seek from the beginning */
        if(access_rec->access==DFACC_WRITE && rle_info->rle_state!=INIT) 
            if(HCIcrle_term(info)==FAIL)
                HRETURN_ERROR(DFE_CTERM,FAIL);
        if(HCIcrle_init(access_rec)==FAIL)
            HRETURN_ERROR(DFE_CINIT,FAIL);
      } /* end if */

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
    HCPclre_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPcrle_read(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from a RLE compressed data element.

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
    VOIDP data;                /* data buffer */
#endif
{
    char *FUNC="HCPcrle_read";      /* for HERROR */
    compinfo_t *info;                   /* special element information */

    info=(compinfo_t *)access_rec->special_info;

    if(HCIcrle_decode(info,length,data)==FAIL)
        HRETURN_ERROR(DFE_CDECODE,FAIL);

    return length;
}   /* HCPcrle_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_write -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPwrite(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to write
    VOIDP data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to a RLE compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPcrle_write(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPcrle_write(access_rec, length, data)
    accrec_t *access_rec;       /* access record */
    int32 length;               /* length of data to write */
    VOIDP data;                 /* data buffer */
#endif
{
    char *FUNC="HCPcrle_write";     /* for HERROR */
    compinfo_t *info;               /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    /* Don't allow random write in a dataset unless: */
    /*  1 - append onto the end */
    /*  2 - start at the beginning and rewrite (at least) the whole dataset */
    if((info->length!=rle_info->offset)
            && (rle_info->offset!=0 || length<info->length))
        HRETURN_ERROR(DFE_UNSUPPORTED,FAIL);

    if(HCIcrle_encode(info,length,data)==FAIL)
        HRETURN_ERROR(DFE_CENCODE,FAIL);

    return(length);
}   /* HCPcrle_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPcrle_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
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
    [Currently a NOP].

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
    return(SUCCEED);
}   /* HCPcrle_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_endaccess -- Close the compressed data element

 USAGE
    int32 HCPendaccess(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Close the compressed data element and free modelling info.

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
    compinfo_t *info;               /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    /* flush out RLE buffer */
    if(access_rec->access==DFACC_WRITE && rle_info->rle_state!=INIT) 
        if(HCIcrle_term(info)==FAIL)
            HRETURN_ERROR(DFE_CTERM,FAIL);

    /* close the compressed data AID */
    if(Hendaccess(info->aid)==FAIL)
	HRETURN_ERROR(DFE_CANTCLOSE,FAIL);

    return(SUCCEED);
}   /* HCPcrle_endaccess() */

