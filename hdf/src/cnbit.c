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

/*
 FILE
       cnbit.c
       HDF N-bit encoding I/O routines

 REMARKS

 DESIGN

 EXPORTED ROUTINES
    None of these routines are designed to be called by other users except
    for the modeling layer of the compression routines.

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
    10/10/93     Starting writing specs & coding prototype
*/

/* General HDF includes */
#include "hdf.h"
#include "hfile.h"
#include "herr.h"

#define CNBIT_MASTER
#define CODER_CLIENT
/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */

/* Internal Defines */

/* Local Variables */
static const uint8 mask_arr8[9]={    /* array of values with [n] bits set */
    0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF
};

static const uint32 mask_arr32[33]={    /* array of values with [n] bits set */
    0x00000000,
    0x00000001,0x00000003,0x00000007,0x0000000F,
    0x0000001F,0x0000003F,0x0000007F,0x000000FF,
    0x000001FF,0x000003FF,0x000007FF,0x00000FFF,
    0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,
    0x0001FFFF,0x0003FFFF,0x0007FFFF,0x000FFFFF,
    0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,
    0x01FFFFFF,0x03FFFFFF,0x07FFFFFF,0x0FFFFFFF,
    0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF
};

/* declaration of the functions provided in this module */
PRIVATE int32 HCIcnbit_staccess
    (accrec_t *access_rec, int16 access);

PRIVATE int32 HCIcnbit_init
    (accrec_t *access_rec);

#ifdef QAK
PRIVATE int32 HCIcnbit_decode
    (compinfo_t *info,int32 length,uint8 *buf);
#endif

PRIVATE int32 HCIcnbit_encode
    (compinfo_t *info,int32 length,uint8 *buf);

#ifdef QAK
PRIVATE int32 HCIcnbit_term
    (compinfo_t *info);
#endif

/*--------------------------------------------------------------------------
 NAME
    HCIcnbit_init -- Initialize a N-bit compressed data element.

 USAGE
    int32 HCIcnbit_init(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcnbit_staccess and HCIcnbit_seek

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32 HCIcnbit_init(accrec_t *access_rec)
{
    char *FUNC="HCIcnbit_init";         /* for HERROR */
    compinfo_t *info;                   /* special element information */
    comp_coder_nbit_info_t *nbit_info;  /* ptr to N-bit info */
    uint8 mask;                         /* mask to apply */
    intn bits;                          /* number of bits in number type */
    intn top_bit,bot_bit;               /* bits around a range of bytes */
    intn mask_top,mask_bot;             /* top and bottom bits in mask */
    intn i;                             /* local counting variable */

    info=(compinfo_t *)access_rec->special_info;
    if(Hseek(info->aid,0,DF_START)==FAIL)   /* seek to beginning of element */
        HRETURN_ERROR(DFE_SEEKERROR,FAIL);

    nbit_info=&(info->cinfo.coder_info.nbit_info);

    /* Initialize N-bit state information */
    nbit_info->buf_pos=0;           /* start at the beginning of the buffer */
    nbit_info->nt_pos=0;            /* start at beginning of the NT info */
    nbit_info->offset=0;            /* offset into the file */
    HDmemset(nbit_info->mask_buf,nbit_info->nt_size,
            (nbit_info->fill_one==TRUE ? 0xff : 0));

    bits=nbit_info->nt_size*8;  /* compute # of bits */
    mask_top=nbit_info->mask_off;   /* compute top and bottom ends of mask */
    mask_bot=nbit_info->mask_off-(nbit_info->mask_len-1);

    top_bit=bits-1;     /* set the initial top and bottom bits */
    bot_bit=bits-8;
    HDmemset(nbit_info->mask_info,0,sizeof(nbit_info->mask_info));  /* set to 0 */
    for(i=0; i<nbit_info->nt_size; i++) {   /* initialize the bitmask info */
        if(mask_top>=top_bit) {  /* mask offset is above current top bit */
            if(mask_bot<=bot_bit) { /* entire byte is in mask */
                nbit_info->mask_info[i].offset=7;
                nbit_info->mask_info[i].length=8;
                nbit_info->mask_info[i].mask=mask_arr8[8];
              } /* end if */
            else {  /* only top part of byte is in mask */
                nbit_info->mask_info[i].offset=7;
                nbit_info->mask_info[i].length=(top_bit-mask_bot)+1;
                nbit_info->mask_info[i].mask=mask_arr8[(top_bit-mask_bot)+1]<<(8-((top_bit-mask_bot)+1));
                break;  /* we've found the bottom of the mask, we're done */
              } /* end else */
          } /* end if */
        else {  /* mask top is below current top bit */
            if(mask_top>=bot_bit) { /* mask top is inside current byte */
                if(mask_bot>=bot_bit) { /* mask top to bottom of byte is in mask */
                    nbit_info->mask_info[i].offset=mask_top-bot_bit;
                    nbit_info->mask_info[i].length=(mask_top-bot_bit)+1;
                    nbit_info->mask_info[i].mask=mask_arr8[(mask_top-bot_bit)+1];
                  } /* end if */
                else {  /* entire bit-field is inside of this byte */
                    nbit_info->mask_info[i].offset=mask_top-bot_bit;
                    nbit_info->mask_info[i].length=(mask_top-mask_bot)+1;
                    nbit_info->mask_info[i].mask=mask_arr8[(mask_top-mask_bot)+1]<<(mask_bot-bot_bit);
                    break;  /* we've found the bottom of the mask, we're done */
                  } /* end else */
              } /* end if */
            else    /* mask top is below bottom bit, just continue */
                {}
          } /* end else */
        top_bit-=8;     /* decrement location in fill mask */
        bot_bit-=8;
      } /* end for */

    /* mask to 0 the bits where the bit-field will go */
    if(nbit_info->fill_one==TRUE) {
        for(i=0; i<nbit_info->nt_size; i++)
            nbit_info->mask_buf[i]&=~(nbit_info->mask_info[i].mask);
      } /* end if */

    return(SUCCEED);
}   /* end HCIcnbit_init() */

/*--------------------------------------------------------------------------
 NAME
    HCIcnbit_decode -- Decode n-bit data into a buffer.

 USAGE
    int32 HCIcnbit_decode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to read into the buffer
    uint8 *buf;         OUT: buffer to store the bytes read

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to decode n-bit data from the file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32 HCIcnbit_decode(compinfo_t *info,int32 length,uint8 *buf)
{
    char *FUNC="HCIcnbit_decode";       /* for HERROR */
    comp_coder_nbit_info_t *nbit_info;  /* ptr to n-bit info */
    int32 orig_length;      /* original length to write */
    uint32 input_bits;      /* bits read from the file */
    nbit_mask_info_t *mask_info;    /* ptr to the mask info */
    intn copy_length;       /* number of bytes to copy */
    intn buf_items;         /* number of items which will fit into expansion buffer */
    bool top_bit,           /* the top bit in an n-bit item */
        top_bit_found;      /* set to TRUE if we've found the top bit */
    intn i,j;               /* local counting variable */

    /* get a local ptr to the nbit info for convenience */
    nbit_info=&(info->cinfo.coder_info.nbit_info);

    buf_items=NBIT_BUF_SIZE/nbit_info->nt_size; /* compute # of items in buffer */
    orig_length=length;     /* save this for later */
    for(; length>0; length--;buf++) {   /* decode until we have all the bytes */
#ifdef TESTING
printf("HCPcnbit_encode(): length=%d, buf=%p\n",length,buf);
#endif
        if(buf_pos>=NBIT_BUF_SIZE) {    /* re-fill buffer */
            /* get a ptr to the mask info for convenience also */
            mask_info=&(nbit_info->mask_info[0]);
            for(i=0; i<buf_items; i++) {
                HDmemcpy(buf,nbit_info->mask_buf,nbit_info->nt_size);
                if(nbit_info->sign_ext) {   /* special code for expanding sign extended data */
                  } /* end if */
                else {  /* no sign extension */
                    for(j=0; j<nbit_info->nt_size; j++) {
                        if(mask_info->length>0) {   /* check if we need to read bits */
                            Hbitread(info->aid,mask_info->length,input_bits);
                            *buf++=input_bits|(mask_info->mask <<
                                    ((mask_info->offset-mask_info->length)+1));
                          } /* end if */
                      } /* end for */
                  } /* end else */
              } /* end for */

            /* advance to the next mask position */
            mask_info++;
            /* advance buffer offset and check for wrap */
            if((++nbit_info->nt_pos)>=nbit_info->nt_size) {
                nbit_info->nt_pos=0;            /* reset to beginning of buffer */
                mask_info=nbit_info->mask_info; /* reset ptr to masks also */
              } /* end if */
          } /* end if */

        copy_length=(length>(NBIT_BUF_SIZE-(buf_pos+1))) ? (NBIT_BUF_SIZE-(buf_pos+1)) : length;

        HDmemcpy(buf,&(nbit_info->buffer[buf_pos]),copy_length);

      } /* end for */

    nbit_info->offset+=orig_length;  /* incr. abs. offset into the file */
    return(SUCCEED);
}   /* end HCIcnbit_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcnbit_encode -- Encode data from a buffer into n-bit data

 USAGE
    int32 HCIcnbit_encode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to store from the buffer
    uint8 *buf;         OUT: buffer to get the bytes from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to encode n-bit data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32 HCIcnbit_encode(compinfo_t *info,int32 length,uint8 *buf)
{
    char *FUNC="HCIcnbit_encode";       /* for HERROR */
    comp_coder_nbit_info_t *nbit_info;  /* ptr to n-bit info */
    int32 orig_length;      /* original length to write */
    uint32 output_bits;     /* bits to write to the file */
    nbit_mask_info_t *mask_info;    /* ptr to the mask info */

    /* get a local ptr to the nbit info for convenience */
    nbit_info=&(info->cinfo.coder_info.nbit_info);

    /* get a ptr to the mask info for convenience also */
    mask_info=&(nbit_info->mask_info[nbit_info->nt_pos]);

    orig_length=length;     /* save this for later */
    for(; length>0; length--;buf++) {  /* encode until we store all the bytes */
#ifdef TESTING
printf("HCPcnbit_encode(): length=%d, buf=%p\n",length,buf);
#endif
        if(mask_info->length>0) {   /* check if we need to output bits */
            output_bits=((*buf)&(mask_info->mask)) >>
                    ((mask_info->offset-mask_info->length)+1);
            Hbitwrite(info->aid,mask_info->length,output_bits);
          } /* end if */

        /* advance to the next mask position */
        mask_info++;
        /* advance buffer offset and check for wrap */
        if((++nbit_info->nt_pos)>=nbit_info->nt_size) {
            nbit_info->nt_pos=0;            /* reset to beginning of buffer */
            mask_info=nbit_info->mask_info; /* reset ptr to masks also */
          } /* end if */
      } /* end for */

    nbit_info->offset+=orig_length;  /* incr. abs. offset into the file */
    return(SUCCEED);
}   /* end HCIcnbit_encode() */

#ifdef QAK
/*--------------------------------------------------------------------------
 NAME
    HCIcnbit_term -- Flush encoded data from internal buffer to RLE compressed data

 USAGE
    int32 HCIcnbit_term(info)
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
PRIVATE int32 HCIcnbit_term(compinfo_t *info)
#else
PRIVATE int32 HCIcnbit_term(info)
    compinfo_t *info;       /* compression info */
#endif
{
    char *FUNC="HCIcnbit_term";          /* for HERROR */
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
printf("HCPcnbit_term(): RUN - wrote run of %d bytes\n",rle_info->buf_length);
#endif
            break;

        case MIX:   /* mixed bunch of bytes */
            if(HDputc((uint8)((rle_info->buf_length-RLE_MIN_MIX)),info->aid)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
            if(Hwrite(info->aid,rle_info->buf_length,rle_info->buffer)==FAIL)
                HRETURN_ERROR(DFE_WRITEERROR,FAIL);
#ifdef TESTING
printf("HCPcnbit_term(): MIX - wrote mix of %d bytes\n",rle_info->buf_length);
#endif
            break;

        default:
            HRETURN_ERROR(DFE_INTERNAL,FAIL);
      } /* end switch */
    rle_info->rle_state=INIT;
    rle_info->second_byte=rle_info->last_byte=RLE_NIL;

    return(SUCCEED);
}   /* end HCIcnbit_term() */
#endif

/*--------------------------------------------------------------------------
 NAME
    HCIcnbit_staccess -- Start accessing a N-bit data element.

 USAGE
    int32 HCIcnbit_staccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcnbit_stread and HCIcnbit_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
PRIVATE int32 HCIcnbit_staccess(accrec_t *access_rec, int16 access)
#else
PRIVATE int32 HCIcnbit_staccess(access_rec, access)
    accrec_t *access_rec;   /* access record */
    int16 access;           /* access mode */
#endif
{
    char *FUNC="HCIcnbit_staccess";      /* for HERROR */
    compinfo_t *info;                   /* special element information */

    info=(compinfo_t *)access_rec->special_info;

    if(access==DFACC_READ)
        info->aid=Hstartbitread(access_rec->file_id,DFTAG_COMPRESSED,
                info->comp_ref);
    else
        info->aid=Hstartbitwrite(access_rec->file_id,DFTAG_COMPRESSED,
                info->comp_ref,info->length);

    if(info->aid==FAIL)
        HRETURN_ERROR(DFE_DENIED,FAIL);
    return(HCIcnbit_init(access_rec));   /* initialize the N-bit info */
}   /* end HCIcnbit_staccess() */

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
int32 HCPcnbit_stread(accrec_t *access_rec)
#else
int32 HCPcnbit_stread(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPcnbit_stread";     /* for HERROR */
    int32 ret;

    if((ret=HCIcnbit_staccess(access_rec, DFACC_READ))==FAIL)
        HRETURN_ERROR(DFE_CINIT,FAIL);
    return(ret);
}   /* HCPcnbit_stread() */

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
int32 HCPcnbit_stwrite(accrec_t *access_rec)
#else
int32 HCPcnbit_stwrite(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPcnbit_stwrite";     /* for HERROR */
    int32 ret;

    if((ret=HCIcnbit_staccess(access_rec, DFACC_WRITE))==FAIL)
        HRETURN_ERROR(DFE_CINIT,FAIL);
    return(ret);
}   /* HCPcnbit_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_seek -- Seek to offset within the data element

 USAGE
    int32 HCPcnbit_seek(access_rec,offset,origin)
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
int32 HCPcnbit_seek(accrec_t *access_rec, int32 offset, int origin)
#else
int32 HCPcnbit_seek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    int origin;
#endif
{
    char *FUNC="HCPcnbit_seek";      /* for HERROR */
#ifdef QAK
    compinfo_t *info;                   /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */
    uint8 *tmp_buf;                 /* pointer to throw-away buffer */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    if(offset<rle_info->offset) {    /* need to seek from the beginning */
        if(access_rec->access==DFACC_WRITE && rle_info->rle_state!=INIT)
            if(HCIcnbit_term(info)==FAIL)
                HRETURN_ERROR(DFE_CTERM,FAIL);
        if(HCIcnbit_init(access_rec)==FAIL)
            HRETURN_ERROR(DFE_CINIT,FAIL);
      } /* end if */

    if((tmp_buf=(uint8 *)HDgetspace(TMP_BUF_SIZE))==NULL)   /* get tmp buffer */
        HRETURN_ERROR(DFE_NOSPACE,FAIL);

    while(rle_info->offset+TMP_BUF_SIZE<offset)     /* grab chunks */
        if(HCIcnbit_decode(info,TMP_BUF_SIZE,tmp_buf)==FAIL) {
            HDfreespace(tmp_buf);
            HRETURN_ERROR(DFE_CDECODE,FAIL);
          } /* end if */
    if(rle_info->offset<offset)             /* grab the last chunk */
        if(HCIcnbit_decode(info,offset-rle_info->offset,tmp_buf)==FAIL) {
            HDfreespace(tmp_buf);
            HRETURN_ERROR(DFE_CDECODE,FAIL);
          } /* end if */

    HDfreespace(tmp_buf);
#endif
    return SUCCEED;
}   /* HCPcnbit_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPcnbit_read(access_rec,length,data)
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
int32 HCPcnbit_read(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPcnbit_read(access_rec, length, data)
    accrec_t *access_rec;      /* access record */
    int32 length;              /* length of data to read in */
    VOIDP data;                /* data buffer */
#endif
{
    char *FUNC="HCPcnbit_read";      /* for HERROR */
#ifdef QAK
    compinfo_t *info;                   /* special element information */

    info=(compinfo_t *)access_rec->special_info;

    if(HCIcnbit_decode(info,length,data)==FAIL)
        HRETURN_ERROR(DFE_CDECODE,FAIL);

#endif
    return length;
}   /* HCPcnbit_read() */

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
int32 HCPcnbit_write(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPcnbit_write(access_rec, length, data)
    accrec_t *access_rec;       /* access record */
    int32 length;               /* length of data to write */
    VOIDP data;                 /* data buffer */
#endif
{
    char *FUNC="HCPcnbit_write";     /* for HERROR */
    compinfo_t *info;               /* special element information */

    info=(compinfo_t *)access_rec->special_info;

    if(HCIcnbit_encode(info,length,data)==FAIL)
        HRETURN_ERROR(DFE_CENCODE,FAIL);

    return(length);
}   /* HCPcnbit_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPclre_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPcnbit_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
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
int32 HCPcnbit_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
                        uint16 *pref, int32 *plength, int32 *poffset,
                        int32 *pposn, int16 *paccess, int16 *pspecial)
#else
int32 HCPcnbit_inquire(access_rec, pfile_id, ptag, pref, plength, poffset,
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
}   /* HCPcnbit_inquire() */

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
int32 HCPcnbit_endaccess(accrec_t *access_rec)
#else
int32 HCPcnbit_endaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPcnbit_endaccess"; /* for HERROR */
#ifdef QAK
    compinfo_t *info;               /* special element information */
    comp_coder_rle_info_t *rle_info;    /* ptr to RLE info */

    info=(compinfo_t *)access_rec->special_info;
    rle_info=&(info->cinfo.coder_info.rle_info);

    /* flush out RLE buffer */
    if(access_rec->access==DFACC_WRITE && rle_info->rle_state!=INIT)
        if(HCIcnbit_term(info)==FAIL)
            HRETURN_ERROR(DFE_CTERM,FAIL);

    /* close the compressed data AID */
    if(Hendaccess(info->aid)==FAIL)
	HRETURN_ERROR(DFE_CANTCLOSE,FAIL);

#endif
    return(SUCCEED);
}   /* HCPcnbit_endaccess() */
