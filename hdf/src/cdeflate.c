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
static char RcsId[] = "$Revision$";
#endif

/* $Id$ */

/*
   FILE
   cdeflate.c
   HDF gzip 'deflate' encoding I/O routines

   REMARKS

   DESIGN

   EXPORTED ROUTINES
   None of these routines are designed to be called by other users except
   for the modeling layer of the compression routines.

   AUTHOR
   Quincey Koziol

   MODIFICATION HISTORY
   10/24/95     Starting coding
 */

/* General HDF includes */
#include "hdf.h"

#define CDEFLATE_MASTER
#define CODER_CLIENT
/* HDF compression includes */
#include "hcompi.h"     /* Internal definitions for compression */

/* Internal Defines */
/* #define TESTING */

/* declaration of the functions provided in this module */
PRIVATE int32 HCIcdeflate_init(accrec_t * access_rec);

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_init -- Initialize a gzip 'deflate' compressed data element.

 USAGE
    int32 HCIcdeflate_init(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcdeflate_staccess and HCIcdeflate_seek

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_init(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCIcdeflate_init");
    compinfo_t *info;       /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to deflate info */

    info = (compinfo_t *) access_rec->special_info;

    if (Hseek(info->aid, 0, 0) == FAIL)  /* seek to beginning of element */
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Initialize deflation state information */
    deflate_info->offset = 0;   /* start at the beginning of the data */
    deflate_info->acc_init = 0; /* second stage of initializing not performed */
    deflate_info->acc_mode = 0; /* init access mode to illegal value */

    /* initialize compression context */
    deflate_info->deflate_context.zalloc=(alloc_func)Z_NULL;
    deflate_info->deflate_context.zfree=(free_func)Z_NULL;
    deflate_info->deflate_context.opaque=NULL;
    deflate_info->deflate_context.data_type=Z_BINARY;

    /* Allocate compression I/O buffer */
    if ((deflate_info->io_buf= (VOIDP) HDmalloc(DEFLATE_BUF_SIZE)) == NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);
    
    return (SUCCEED);
}   /* end HCIcdeflate_init() */

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_decode -- Decode skipping Huffman compressed data into a buffer.

 USAGE
    int32 HCIcdeflate_decode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to read into the buffer
    uint8 *buf;         OUT: buffer to store the bytes read

 RETURNS
    Returns # of bytes decompressed or FAIL

 DESCRIPTION
    Common code called to decode gzip 'deflated' data from the file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_decode(compinfo_t * info, int32 length, uint8 *buf)
{
    CONSTR(FUNC, "HCIcdeflate_decode");
    comp_coder_deflate_info_t *deflate_info;    /* ptr to deflate info */
    int32 bytes_read;

    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Set up the deflation buffers to point to the user's buffer to fill */
    deflate_info->deflate_context.next_out=buf;
    deflate_info->deflate_context.avail_out=(uInt)length;
    while(deflate_info->deflate_context.avail_out>0)
      {
#ifdef QAK
int32 t1, t2;

if(t1!=deflate_info->deflate_context.avail_out ||
    t2!=deflate_info->deflate_context.avail_in)
  {
    printf("%s: avail_out=%d, avail_in=%d\n",FUNC,(int)deflate_info->deflate_context.avail_out,(int) deflate_info->deflate_context.avail_in);
    t1=deflate_info->deflate_context.avail_out;
    t2=deflate_info->deflate_context.avail_in;
  }
#endif
          /* Get more bytes from the file, if we've run out */
          if(deflate_info->deflate_context.avail_in==0)
            {
                int32 file_bytes;

                deflate_info->deflate_context.next_in=deflate_info->io_buf;
                if((file_bytes=Hread(info->aid,DEFLATE_BUF_SIZE,deflate_info->deflate_context.next_in))==FAIL)
                    HRETURN_ERROR(DFE_READERROR,FAIL);
                deflate_info->deflate_context.avail_in=(uInt)file_bytes;
            } /* end if */

          /* break out if we've reached the end of the compressed data somehow */
          if(inflate(&(deflate_info->deflate_context),Z_NO_FLUSH)==Z_STREAM_END)
              break;
      } /* end while */
    bytes_read=(int32)length-(int32)deflate_info->deflate_context.avail_out;
    deflate_info->offset+=bytes_read;

    return(bytes_read);
}   /* end HCIcdeflate_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_encode -- Encode data from a buffer into gzip 'deflated'
                            compressed data

 USAGE
    int32 HCIcdeflate_encode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to store from the buffer
    uint8 *buf;         OUT: buffer to get the bytes from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to encode gzip 'deflated' data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_encode(compinfo_t * info, int32 length, const VOIDP buf)
{
    CONSTR(FUNC, "HCIcdeflate_encode");
    comp_coder_deflate_info_t *deflate_info;    /* ptr to skipping Huffman info */

    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Set up the deflation buffers to point to the user's buffer to empty */
    deflate_info->deflate_context.next_in=buf;
    deflate_info->deflate_context.avail_in=(uInt)length;
    while(deflate_info->deflate_context.avail_in>0)
      {
          /* Write more bytes from the file, if we've filled our buffer */
          if(deflate_info->deflate_context.avail_out==0)
            {
                if(deflate_info->deflate_context.next_out!=NULL)
                    if(Hwrite(info->aid,DEFLATE_BUF_SIZE,deflate_info->io_buf)==FAIL)
                        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                deflate_info->deflate_context.next_out=deflate_info->io_buf;
                deflate_info->deflate_context.avail_out=DEFLATE_BUF_SIZE;
            } /* end if */

          /* break out if we've reached the end of the compressed data somehow */
          if(deflate(&(deflate_info->deflate_context),Z_NO_FLUSH)!=Z_OK)
              HRETURN_ERROR(DFE_CENCODE,FAIL);
      } /* end while */
    deflate_info->offset += length;    /* incr. abs. offset into the file */

    return (length);
}   /* end HCIcdeflate_encode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_term -- Close down internal buffering for gzip 'deflate' encoding

 USAGE
    int32 HCIcdeflate_term(info,acc_mode)
    compinfo_t *info;   IN: the info about the compressed element
    uint32 acc_mode;    IN: the access mode the data element was opened with

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to flush gzip 'deflated' data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_term(compinfo_t * info,uint32 acc_mode)
{
    CONSTR(FUNC, "HCIcdeflate_term");
    comp_coder_deflate_info_t *deflate_info;    /* ptr to deflation info */

    deflate_info = &(info->cinfo.coder_info.deflate_info);

    if(acc_mode&DFACC_WRITE)
      { /* flush the "deflated" data to the file */
          intn status;

          do
            {
              /* Write more bytes from the file, if we've filled our buffer */
              if(deflate_info->deflate_context.avail_out==0)
                {
                    if(Hwrite(info->aid,DEFLATE_BUF_SIZE,deflate_info->io_buf)==FAIL)
                        HRETURN_ERROR(DFE_WRITEERROR,FAIL);
                    deflate_info->deflate_context.next_out=deflate_info->io_buf;
                    deflate_info->deflate_context.avail_out=DEFLATE_BUF_SIZE;
                } /* end if */

                status=deflate(&(deflate_info->deflate_context),Z_FINISH);
            } while(status==Z_OK);
          if(status!=Z_STREAM_END)
              HRETURN_ERROR(DFE_CENCODE,FAIL);
          if(deflate_info->deflate_context.avail_out<DEFLATE_BUF_SIZE)
              if(Hwrite(info->aid,(int32)(DEFLATE_BUF_SIZE-deflate_info->deflate_context.avail_out),deflate_info->io_buf)==FAIL)
                  HRETURN_ERROR(DFE_WRITEERROR,FAIL);

          /* Close down the deflation buffer */
          if(deflateEnd(&(deflate_info->deflate_context))!=Z_OK)
              HRETURN_ERROR(DFE_CTERM,FAIL);
      } /* end if */
    else
      { /* finish up any inflated data */
          /* Close down the inflation buffer */
          if(inflateEnd(&(deflate_info->deflate_context))!=Z_OK)
              HRETURN_ERROR(DFE_CTERM,FAIL);
      } /* end else */

    /* Get rid of the I/O buffer */
    HDfree(deflate_info->io_buf);

    return (SUCCEED);
}   /* end HCIcdeflate_term() */

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_staccess -- Start accessing a gzip 'deflate' compressed data element.

 USAGE
    int32 HCIcdeflate_staccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcdeflate_stread and HCIcdeflate_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_staccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HCIcdeflate_staccess");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to deflate info */

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* need to check for not writing, as opposed to read access */
    /* because of the way the access works */
    if (!(acc_mode&DFACC_WRITE))
        info->aid = Hstartread(access_rec->file_id, DFTAG_COMPRESSED,
                                  info->comp_ref);
    else
        info->aid = Hstartwrite(access_rec->file_id, DFTAG_COMPRESSED,
                                   info->comp_ref, info->length);
    if (info->aid == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);

    /* Make certain we can append to the data when writing */
    if ((acc_mode&DFACC_WRITE) && Happendable(info->aid) == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);

    /* initialize the common deflate coding info */
    if(HCIcdeflate_init(access_rec)==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);

    return (SUCCEED);
}   /* end HCIcdeflate_staccess() */

/*--------------------------------------------------------------------------
 NAME
    HCIcdeflate_staccess2 -- 2nd half of start accessing a gzip 'deflate'
                            compressed data element.

 USAGE
    int32 HCIcdeflate_staccess2(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcdeflate_stread and HCIcdeflate_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcdeflate_staccess2(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HCIcdeflate_staccess2");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to deflate info */

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Initialize the gzip library */
    if(acc_mode&DFACC_WRITE)
      {
        if(deflateInit(&(deflate_info->deflate_context),deflate_info->deflate_level)!=Z_OK)
            HRETURN_ERROR(DFE_CINIT, FAIL);

        /* set access mode */
        deflate_info->acc_mode=DFACC_WRITE;

        /* force I/O with the file at first */
        deflate_info->deflate_context.next_out=NULL;
        deflate_info->deflate_context.avail_out=0;
      } /* end if */
    else
      {
        if(inflateInit(&(deflate_info->deflate_context))!=Z_OK)
            HRETURN_ERROR(DFE_CINIT, FAIL);

        /* set access mode */
        deflate_info->acc_mode=DFACC_READ;
        
        /* force I/O with the file at first */
        deflate_info->deflate_context.avail_in=0;
      } /* end else */

    /* set flag to indicate second stage of initialization is finished */
    deflate_info->acc_init=1;

    return (SUCCEED);
}   /* end HCIcdeflate_staccess2() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_stread -- start read access for compressed file

 USAGE
    int32 HCPcdeflate_stread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start read access on a compressed data element using the deflate scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcdeflate_stread(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcdeflate_stread");

    if (HCIcdeflate_staccess(access_rec, DFACC_READ) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);

    return (SUCCEED);
}   /* HCPcdeflate_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_stwrite -- start write access for compressed file

 USAGE
    int32 HCPcdeflate_stwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start write access on a compressed data element using the deflate scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcdeflate_stwrite(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcdeflate_stwrite");

    if (HCIcdeflate_staccess(access_rec, DFACC_WRITE) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);

    return (SUCCEED);
}   /* HCPcdeflate_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_seek -- Seek to offset within the data element

 USAGE
    int32 HCPcdeflate_seek(access_rec,offset,origin)
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
int32
HCPcdeflate_seek(accrec_t * access_rec, int32 offset, int origin)
{
    CONSTR(FUNC, "HCPcdeflate_seek");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to gzip 'deflate' info */
    uint8      *tmp_buf;        /* pointer to throw-away buffer */

    /* shut compiler up */
    origin = origin;

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Check if second stage of initialization has been performed */
    if(deflate_info->acc_init==0)
      {
        if (HCIcdeflate_staccess2(access_rec, DFACC_READ) == FAIL)
            HRETURN_ERROR(DFE_CINIT, FAIL);
      } /* end if */

#ifdef QAK
    /* Don't allow seeks when writing (currently) -QAK */
    if (access_rec->access&DFACC_WRITE)
        HRETURN_ERROR(DFE_UNSUPPORTED, FAIL);
#endif /* QAK */

    if (offset < deflate_info->offset)
      {     /* need to seek from the beginning */
          /* Reset the decompression buffer */
          if (deflateReset(&(deflate_info->deflate_context))!=Z_OK)
              HRETURN_ERROR(DFE_CINIT, FAIL);
          /* Go back to the beginning of the data-stream */
          if(Hseek(info->aid,0,0)==FAIL)
              HRETURN_ERROR(DFE_SEEKERROR, FAIL);
      }     /* end if */

    if ((tmp_buf = (uint8 *) HDmalloc(DEFLATE_TMP_BUF_SIZE)) == NULL)     /* get tmp buffer */
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    while (deflate_info->offset + DEFLATE_TMP_BUF_SIZE < offset)    /* grab chunks */
        if (HCIcdeflate_decode(info, DEFLATE_TMP_BUF_SIZE, tmp_buf) == FAIL)
          {
              HDfree(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL)
          }     /* end if */
    if (deflate_info->offset < offset)  /* grab the last chunk */
        if (HCIcdeflate_decode(info, offset - deflate_info->offset, tmp_buf) == FAIL)
          {
              HDfree(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL)
          }     /* end if */

    HDfree(tmp_buf);
    return (SUCCEED);
}   /* HCPcdeflate_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPcdeflate_read(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from the deflate compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcdeflate_read(accrec_t * access_rec, int32 length, VOIDP data)
{
    CONSTR(FUNC, "HCPcdeflate_read");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to gzip 'deflate' info */

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Check if second stage of initialization has been performed */
    if(deflate_info->acc_init==0)
      {
        if (HCIcdeflate_staccess2(access_rec, DFACC_READ) == FAIL)
            HRETURN_ERROR(DFE_CINIT, FAIL);
      } /* end if */

    if ((length=HCIcdeflate_decode(info, length, data)) == FAIL)
        HRETURN_ERROR(DFE_CDECODE, FAIL);

    return (length);
}   /* HCPcdeflate_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_write -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPcdeflate_write(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to write
    VOIDP data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to the deflate compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcdeflate_write(accrec_t * access_rec, int32 length, const VOIDP data)
{
    CONSTR(FUNC, "HCPcdeflate_write");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to skipping Huffman info */

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* Don't allow random write in a dataset unless: */
    /*  1 - append onto the end */
    /*  2 - start at the beginning and rewrite (at least) the whole dataset */
    if ((info->length != deflate_info->offset)
        && (deflate_info->offset != 0 || length < info->length))
        HRETURN_ERROR(DFE_UNSUPPORTED, FAIL);

    /* Check if second stage of initialization has been performed */
    if(deflate_info->acc_init==0)
      {
        if (HCIcdeflate_staccess2(access_rec, DFACC_WRITE) == FAIL)
            HRETURN_ERROR(DFE_CINIT, FAIL);
      } /* end if */

    if ((length=HCIcdeflate_encode(info, length, data)) == FAIL)
        HRETURN_ERROR(DFE_CENCODE, FAIL);

    return (length);
}   /* HCPcdeflate_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPcdeflate_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
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
int32
HCPcdeflate_inquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
                    uint16 *pref, int32 *plength, int32 *poffset,
                    int32 *pposn, int16 *paccess, int16 *pspecial)
{
    /* shut compiler up */
    access_rec = access_rec;
    pfile_id = pfile_id;
    ptag = ptag;
    pref = pref;
    plength = plength;
    poffset = poffset;
    pposn = pposn;
    paccess = paccess;
    pspecial = pspecial;

    return (SUCCEED);
}   /* HCPcdeflate_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPcdeflate_endaccess -- Close the compressed data element

 USAGE
    int32 HCPcdeflate_endaccess(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Close the compressed data element and free encoding info.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn
HCPcdeflate_endaccess(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcdeflate_endaccess");
    compinfo_t *info;           /* special element information */
    comp_coder_deflate_info_t *deflate_info;    /* ptr to gzip 'deflate' info */

    info = (compinfo_t *) access_rec->special_info;
    deflate_info = &(info->cinfo.coder_info.deflate_info);

    /* flush out buffer */
    if (HCIcdeflate_term(info,deflate_info->acc_mode) == FAIL)
        HRETURN_ERROR(DFE_CTERM, FAIL);

    /* close the compressed data AID */
    if (Hendaccess(info->aid) == FAIL)
        HRETURN_ERROR(DFE_CANTCLOSE, FAIL);

    return (SUCCEED);
}   /* HCPcdeflate_endaccess() */

