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

/* General HDF includes */
#include "hdf.h"

#define CSZIP_MASTER
#define CODER_CLIENT
/* HDF compression includes */
#include "hcompi.h"     /* Internal definitions for compression */

/* internal defines */
#define TMP_BUF_SIZE    8192    /* size of throw-away buffer */

/* #define TESTING */

/* declaration of the functions provided in this module */
PRIVATE int32 HCIcszip_staccess
            (accrec_t * access_rec, int16 acc_mode);

PRIVATE int32 HCIcszip_init
            (accrec_t * access_rec);

PRIVATE int32 HCIcszip_decode
            (compinfo_t * info, int32 length, uint8 *buf);

PRIVATE int32 HCIcszip_encode
            (compinfo_t * info, int32 length, const uint8 *buf);

PRIVATE int32 HCIcszip_term
            (compinfo_t * info);

/*--------------------------------------------------------------------------
 NAME
    HCIcszip_init -- Initialize a SZIP compressed data element.

 USAGE
    int32 HCIcszip_init(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcszip_staccess and HCIcszip_seek

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcszip_init(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCIcszip_init");
    compinfo_t *info;           /* special element information */
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */


    info = (compinfo_t *) access_rec->special_info;
    if (Hseek(info->aid, 0, DF_START) == FAIL)  /* seek to beginning of element */
        HRETURN_ERROR(DFE_SEEKERROR, FAIL);

    szip_info = &(info->cinfo.coder_info.szip_info);

    /* Initialize SZIP state information */
    szip_info->szip_state = SZIP_INIT;     /* start in initial state */
    szip_info->offset = 0;   /* offset into the file */

    return (SUCCEED);
}   /* end HCIcszip_init() */

/*--------------------------------------------------------------------------
 NAME
    HCIcszip_decode -- Decode SZIP compressed data into a buffer.

 USAGE
    int32 HCIcszip_decode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to read into the buffer
    uint8 *buf;         OUT: buffer to store the bytes read

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to decode SZIP data from the file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcszip_decode(compinfo_t * info, int32 length, uint8 *buf)
{
    CONSTR(FUNC, "HCIcszip_decode");
	accrec_t *access_rec;
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */
	uint8 *in_buffer;
	uint8 *out_buffer;
	int32 in_length;
	int32 out_length;
	int bytes_per_pixel;
	int32 bytes;

#ifdef H4_HAVE_LIBSZ

    szip_info = &(info->cinfo.coder_info.szip_info);
	if (szip_info->szip_state == SZIP_INIT)
		{
		if ((access_rec = HAatom_object(info->aid)) == NULL)    /* get the access_rec pointer */
		    HRETURN_ERROR(DFE_ARGS, FAIL);

		if(HTPinquire(access_rec->ddid,NULL,NULL,NULL,&in_length)==FAIL)
			HRETURN_ERROR(DFE_INTERNAL, FAIL);

	    if ((in_buffer = (uint8 *) HDmalloc(in_length)) == NULL)
    	    HRETURN_ERROR(DFE_NOSPACE, FAIL);

		bytes_per_pixel = (szip_info->bits_per_pixel + 7) >> 3;
		if (bytes_per_pixel == 3)
			bytes_per_pixel++;

		out_length = szip_info->pixels * bytes_per_pixel;
	    if ((out_buffer = (uint8 *) HDmalloc(out_length)) == NULL)
    	    HRETURN_ERROR(DFE_NOSPACE, FAIL);

		if (Hread(info->aid, in_length, in_buffer) == FAIL)
			HRETURN_ERROR(DFE_READERROR, FAIL);

#if 0
		bytes = szip_uncompress_memory(szip_info->options_mask, szip_info->bits_per_pixel, szip_info->pixels_per_block, szip_info->pixels_per_scanline, in_buffer, in_length, out_buffer, szip_info->pixels); 
#endif
		bytes = szip_uncompress_memory(szip_info->options_mask, szip_info->bits_per_pixel, szip_info->pixels_per_block, szip_info->pixels_per_scanline, in_buffer, in_length, out_buffer, out_length); 

/*		printf("Uncompressed bytes = %d\n", bytes); */

		if (bytes != out_length)
			HRETURN_ERROR(DFE_INTERNAL, FAIL);

		HDfree(in_buffer);
		szip_info->szip_state = SZIP_RUN;
		szip_info->buffer = out_buffer;
		szip_info->buffer_pos = 0;
		szip_info->buffer_size = out_length;
		szip_info->offset = 0;
		}
#if 0
	/*** This code is duplicated below ***/
	else if (szip_info->szip_state == SZIP_RUN)
		{
		HDmemcpy(buf, szip_info->buffer + szip_info->buffer_pos, length);
		szip_info->buffer_pos += length;
		szip_info->buffer_size -= length;
		}
#endif

	if (length > szip_info->buffer_size)
		{	
		HDfree(szip_info->buffer);
		return (FAIL);
		}

	HDmemcpy(buf, szip_info->buffer + szip_info->buffer_pos, length);
	szip_info->buffer_pos += length;
	szip_info->buffer_size -= length;
	szip_info->offset = szip_info->buffer_pos;

	/*printf("HCIcszip_decode: new offset = %d\n", szip_info->offset);*/

	if (szip_info->buffer_size == 0)
		HDfree(szip_info->buffer);

    return (SUCCEED);

#else /* ifdef H4_HAVE_LIBSZ */

    HRETURN_ERROR(DFE_CANTCOMP, FAIL);

#endif /* H4_HAVE_LIBSZ */

}   /* end HCIcszip_decode() */

/*--------------------------------------------------------------------------
 NAME
    HCIcszip_encode -- Encode data from a buffer into SZIP compressed data

 USAGE
    int32 HCIcszip_encode(info,length,buf)
    compinfo_t *info;   IN: the info about the compressed element
    int32 length;       IN: number of bytes to store from the buffer
    const uint8 *buf;         OUT: buffer to get the bytes from

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to encode SZIP data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcszip_encode(compinfo_t * info, int32 length, const uint8 *buf)
{
    CONSTR(FUNC, "HCIcszip_encode");
	uint8 *out_buffer;
	int32 out_size; 
	int32 pixels;
	int bytes_per_pixel;
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */
	int32 buffer_size;

#ifdef H4_HAVE_LIBSZ

    szip_info = &(info->cinfo.coder_info.szip_info);
	if (szip_info->szip_state == SZIP_INIT)
		{
		bytes_per_pixel = (szip_info->bits_per_pixel + 7) >> 3;
		if (bytes_per_pixel == 3)
			bytes_per_pixel = 4;

		buffer_size = szip_info->pixels * bytes_per_pixel;
		if ((szip_info->buffer = HDmalloc(buffer_size)) == NULL)
			HRETURN_ERROR(DFE_NOSPACE, FAIL);
	
		szip_info->buffer_size = buffer_size;
		szip_info->buffer_pos = 0;
		szip_info->szip_state = SZIP_RUN;
		}

	HDmemcpy(szip_info->buffer + szip_info->buffer_pos, buf, length);
	szip_info->buffer_pos += length;
	szip_info->buffer_size -= length;

    return (SUCCEED);

#else /* ifdef H4_HAVE_LIBSZ */

    HRETURN_ERROR(DFE_CANTDECOMP, FAIL);

#endif /* H4_HAVE_LIBSZ */

}   /* end HCIcszip_encode() */


/*--------------------------------------------------------------------------
 NAME
    HCIcszip_term -- Flush encoded data from internal buffer to SZIP compressed data

 USAGE
    int32 HCIcszip_term(info)
    compinfo_t *info;   IN: the info about the compressed element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called to flush SZIP data into a file.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcszip_term(compinfo_t * info)
{
    CONSTR(FUNC, "HCIcszip_term");
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */
	uint8 *out_buffer;
	int32 out_bytes;
	int32 out_buffer_size;
	int bytes_per_pixel;
	int32 pixels;

#ifdef H4_HAVE_LIBSZ

    szip_info = &(info->cinfo.coder_info.szip_info);
	if (szip_info->szip_state != SZIP_RUN)
    	return (SUCCEED);

	szip_info->szip_state = SZIP_TERM;

	bytes_per_pixel = (szip_info->bits_per_pixel + 7) >> 3;
	if (bytes_per_pixel == 3)
		bytes_per_pixel = 4;

	out_buffer_size = szip_info->pixels * 1.25 * bytes_per_pixel;
	if ((out_buffer = HDmalloc(out_buffer_size)) == NULL)
		HRETURN_ERROR(DFE_NOSPACE, FAIL);

	if (szip_info->pixels % szip_info->pixels_per_scanline)
		{
		fprintf(stderr, "Pixels (%d) must integer multiple of pixels per scanline (%d)\n", szip_info->pixels, szip_info->pixels_per_scanline);
		return (FAIL);
		}

	if (szip_info->pixels_per_scanline % szip_info->pixels_per_block)
		{
		fprintf(stderr, "Pixels per scanline (%d) must be an integer multiple of pixels per block (%d)\n", szip_info->pixels_per_scanline, szip_info->pixels_per_block);
		return (FAIL);
		}

	out_bytes = szip_compress_memory(szip_info->options_mask, szip_info->bits_per_pixel, szip_info->pixels_per_block, szip_info->pixels_per_scanline, szip_info->buffer, szip_info->pixels, out_buffer);

/*	printf("Compressed bytes = %d\n", out_bytes);*/

	Hwrite(info->aid, out_bytes, out_buffer);

	HDfree(out_buffer);

    return (SUCCEED);

#else /* H4_HAVE_LIBSZ */

    HRETURN_ERROR(DFE_CANTCOMP, FAIL);

#endif /* H4_HAVE_LIBSZ */

}   /* end HCIcszip_term() */

/*--------------------------------------------------------------------------
 NAME
    HCIcszip_staccess -- Start accessing a SZIP compressed data element.

 USAGE
    int32 HCIcszip_staccess(access_rec, access)
    accrec_t *access_rec;   IN: the access record of the data element
    int16 access;           IN: the type of access wanted

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Common code called by HCIcszip_stread and HCIcszip_stwrite

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
PRIVATE int32
HCIcszip_staccess(accrec_t * access_rec, int16 acc_mode)
{
    CONSTR(FUNC, "HCIcszip_staccess");
    compinfo_t *info;           /* special element information */

#ifdef H4_HAVE_LIBSZ

    info = (compinfo_t *) access_rec->special_info;

#ifdef OLD_WAY
    if (acc_mode == DFACC_READ)
        info->aid = Hstartread(access_rec->file_id, DFTAG_COMPRESSED,
                               info->comp_ref);
    else
        info->aid = Hstartwrite(access_rec->file_id, DFTAG_COMPRESSED,
                                info->comp_ref, info->length);

    if (info->aid == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);
    if ((acc_mode&DFACC_WRITE) && Happendable(info->aid) == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);
#else /* OLD_WAY */
    if (acc_mode == DFACC_READ)
        info->aid = Hstartread(access_rec->file_id, DFTAG_COMPRESSED,
                               info->comp_ref);
    else
        info->aid = Hstartaccess(access_rec->file_id, DFTAG_COMPRESSED,
                                info->comp_ref, DFACC_RDWR|DFACC_APPENDABLE);

    if (info->aid == FAIL)
        HRETURN_ERROR(DFE_DENIED, FAIL);
#endif /* OLD_WAY */
    return (HCIcszip_init(access_rec));  /* initialize the SZIP info */

#else /* H4_HAVE_LIBSZ */

    HRETURN_ERROR(DFE_DENIED, FAIL);

#endif /* H4_HAVE_LIBSZ */

}   /* end HCIcszip_staccess() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_stread -- start read access for compressed file

 USAGE
    int32 HCPcszip_stread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start read access on a compressed data element using a simple SZIP scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcszip_stread(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcszip_stread");
    int32       ret;

    if ((ret = HCIcszip_staccess(access_rec, DFACC_READ)) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);
    return (ret);
}   /* HCPcszip_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_stwrite -- start write access for compressed file

 USAGE
    int32 HCPcszip_stwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start write access on a compressed data element using a simple SZIP scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcszip_stwrite(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcszip_stwrite");
    int32       ret;

    if ((ret = HCIcszip_staccess(access_rec, DFACC_WRITE)) == FAIL)
        HRETURN_ERROR(DFE_CINIT, FAIL);
    return (ret);
}   /* HCPcszip_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_seek -- Seek to offset within the data element

 USAGE
    int32 HCPcszip_seek(access_rec,offset,origin)
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
HCPcszip_seek(accrec_t * access_rec, int32 offset, int origin)
{
    CONSTR(FUNC, "HCPcszip_seek");
    compinfo_t *info;           /* special element information */
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */
    uint8      *tmp_buf;        /* pointer to throw-away buffer */

    /* shut compiler up */
    origin = origin;

    info = (compinfo_t *) access_rec->special_info;
    szip_info = &(info->cinfo.coder_info.szip_info);

    if (offset < szip_info->offset)
      {     /* need to seek from the beginning */
          if ((access_rec->access&DFACC_WRITE) && szip_info->szip_state != SZIP_INIT)
              if (HCIcszip_term(info) == FAIL)
                  HRETURN_ERROR(DFE_CTERM, FAIL);
          if (HCIcszip_init(access_rec) == FAIL)
              HRETURN_ERROR(DFE_CINIT, FAIL);
      }     /* end if */

    if ((tmp_buf = (uint8 *) HDmalloc(TMP_BUF_SIZE)) == NULL)     /* get tmp buffer */
        HRETURN_ERROR(DFE_NOSPACE, FAIL);

    while (szip_info->offset + TMP_BUF_SIZE < offset)    /* grab chunks */
        if (HCIcszip_decode(info, TMP_BUF_SIZE, tmp_buf) == FAIL)
          {
              HDfree(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL)
          }     /* end if */
    if (szip_info->offset < offset)  /* grab the last chunk */
        if (HCIcszip_decode(info, offset - szip_info->offset, tmp_buf) == FAIL)
          {
              HDfree(tmp_buf);
              HRETURN_ERROR(DFE_CDECODE, FAIL)
          }     /* end if */

    HDfree(tmp_buf);
    return (SUCCEED);
}   /* HCPcszip_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPcszip_read(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    void * data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from a SZIP compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcszip_read(accrec_t * access_rec, int32 length, void * data)
{
    CONSTR(FUNC, "HCPcszip_read");
    compinfo_t *info;           /* special element information */

    info = (compinfo_t *) access_rec->special_info;

    if (HCIcszip_decode(info, length, data) == FAIL)
        HRETURN_ERROR(DFE_CDECODE, FAIL);

#ifdef QAK
printf("%s: *data=%u\n",FUNC,*(uint8 *)data);
#endif /* QAK */
    return (length);
}   /* HCPcszip_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_write -- Write out a portion of data from a compressed data element.

 USAGE
    int32 HCPcszip_write(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to write
    void * data;             IN: the buffer to retrieve the bytes written

 RETURNS
    Returns the number of bytes written or FAIL

 DESCRIPTION
    Write out a number of bytes to a SZIP compressed data element.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
HCPcszip_write(accrec_t * access_rec, int32 length, const void * data)
{
    CONSTR(FUNC, "HCPcszip_write");
    compinfo_t *info;           /* special element information */
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */

    info = (compinfo_t *) access_rec->special_info;
    szip_info = &(info->cinfo.coder_info.szip_info);

    /* Don't allow random write in a dataset unless: */
    /*  1 - append onto the end */
    /*  2 - start at the beginning and rewrite (at least) the whole dataset */
    if ((info->length != szip_info->offset)
        && (szip_info->offset != 0 && length <= (info->length-szip_info->offset)))
        HRETURN_ERROR(DFE_UNSUPPORTED, FAIL);

    if (HCIcszip_encode(info, length, data) == FAIL)
        HRETURN_ERROR(DFE_CENCODE, FAIL);

    return (length);
}   /* HCPcszip_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPcszip_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
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
HCPcszip_inquire(accrec_t * access_rec, int32 *pfile_id, uint16 *ptag,
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
}   /* HCPcszip_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPcszip_endaccess -- Close the compressed data element

 USAGE
    int32 HCPcszip_endaccess(access_rec)
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
HCPcszip_endaccess(accrec_t * access_rec)
{
    CONSTR(FUNC, "HCPcszip_endaccess");
    compinfo_t *info;           /* special element information */
    comp_coder_szip_info_t *szip_info;    /* ptr to SZIP info */

    info = (compinfo_t *) access_rec->special_info;
    szip_info = &(info->cinfo.coder_info.szip_info);

    /* flush out SZIP buffer */
    if ((access_rec->access&DFACC_WRITE) && szip_info->szip_state != SZIP_INIT)
        if (HCIcszip_term(info) == FAIL)
            HRETURN_ERROR(DFE_CTERM, FAIL);

    /* close the compressed data AID */
    if (Hendaccess(info->aid) == FAIL)
        HRETURN_ERROR(DFE_CANTCLOSE, FAIL);

    return (SUCCEED);
}   /* HCPcszip_endaccess() */
