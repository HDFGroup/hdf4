#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
 FILE
       mstdio.c
       HDF stdio-like modeling I/O routines

 REMARKS
    Basically a NOP interface, it just calls the encoding layer.

 DESIGN
    Just pass the data to the encoding layer, reporting errors.

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
    9/28/93     Starting writing specs & coding prototype
*/


/* General HDF includes */
#include "hdf.h"
#include "hfile.h"
#include "herr.h"

#define MSTDIO_MASTER
#define MODEL_CLIENT
/* HDF compression includes */
#include "hcompi.h"         /* Internal definitions for compression */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_stread -- start read access for compressed file

 USAGE
    int32 HCPmstdio_stread(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start read access on a compressed data element using the stdio modeling
    scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_stread(accrec_t *access_rec)
#else
int32 HCPmstdio_stread(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPmstdio_stread";  /* for HERROR */
    compinfo_t *info;               /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;

    /* set the offset */
    info->minfo.model_info.stdio_info.pos=0;

    if((ret=(*(info->cinfo.coder_funcs.stread))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(SUCCEED);
}   /* HCPmstdio_stread() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_stwrite -- start write access for compressed file

 USAGE
    int32 HCPmstdio_stwrite(access_rec)
    accrec_t *access_rec;   IN: the access record of the data element

 RETURNS
    Returns SUCCEED or FAIL

 DESCRIPTION
    Start write access on a compressed data element using the stdio modeling
    scheme.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int32 HCPmstdio_stwrite(accrec_t *access_rec)
#else
int32 HCPmstdio_stwrite(access_rec)
    accrec_t *access_rec;
#endif
{
    char *FUNC="HCPmstdio_stwrite";  /* for HERROR */
    compinfo_t *info;               /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;

    /* set the offset */
    info->minfo.model_info.stdio_info.pos=0;

    if((ret=(*(info->cinfo.coder_funcs.stwrite))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(SUCCEED);
}   /* HCPmstdio_stwrite() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_seek -- Seek to offset within the data element

 USAGE
    int32 HCPmstdio_seek(access_rec,offset,origin)
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
int32 HCPmstdio_seek(accrec_t *access_rec, int32 offset, int origin)
#else
int32 HCPmstdio_seek(access_rec, offset, origin)
    accrec_t *access_rec;
    int32 offset;
    int origin;
#endif
{
    char *FUNC="HCPmstdio_seek";    /* for HERROR */
    compinfo_t *info;               /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;

    /* set the offset */
    info->minfo.model_info.stdio_info.pos=offset;

    if((ret=(*(info->cinfo.coder_funcs.seek))(access_rec,offset,origin))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(ret);
}   /* HCPmstdio_seek() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_read -- Read in a portion of data from a compressed data element.

 USAGE
    int32 HCPmstdio_read(access_rec,length,data)
    accrec_t *access_rec;   IN: the access record of the data element
    int32 length;           IN: the number of bytes to read
    VOIDP data;             OUT: the buffer to place the bytes read

 RETURNS
    Returns the number of bytes read or FAIL

 DESCRIPTION
    Read in a number of bytes from a compressed data element, using
    stdio functionality.

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
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;

    /* adjust model position */
    info->minfo.model_info.stdio_info.pos+=length;

    if((ret=(*(info->cinfo.coder_funcs.read))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(ret);
}   /* HCPmstdio_read() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_write -- Write out a portion of data from a compressed data element.

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
int32 HCPmstdio_write(accrec_t *access_rec, int32 length, VOIDP data)
#else
int32 HCPmstdio_write(access_rec, length, data)
    accrec_t *access_rec;   /* access record */
    int32 length;           /* length of data to write */
    VOIDP data;             /* data buffer */
#endif
{
    char *FUNC="HCPmstdio_write";     /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;

    /* adjust model position */
    info->minfo.model_info.stdio_info.pos+=length;

    if((ret=(*(info->cinfo.coder_funcs.write))(access_rec,length,data))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(ret);
}   /* HCPmstdio_write() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_inquire -- Inquire information about the access record and data element.

 USAGE
    int32 HCPmstdio_inquire(access_rec,pfile_id,ptag,pref,plength,poffset,pposn,
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
int32 HCPmstdio_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag,
    uint16 *pref, int32 *plength, int32 *poffset,int32 *pposn, int16 *paccess,
    int16 *pspecial)
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
    char *FUNC="HCPmstdio_inquire";     /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->cinfo.coder_funcs.inquire))(access_rec,pfile_id,ptag,pref,
            plength, poffset,pposn, paccess, pspecial))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(ret);
}   /* HCPmstdio_inquire() */

/*--------------------------------------------------------------------------
 NAME
    HCPmstdio_endaccess -- Close the compressed data element

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
int32 HCPmstdio_endaccess(accrec_t *access_rec)
#else
int32 HCPmstdio_endaccess(access_rec)
    accrec_t *access_rec;      /* access record to dispose of */
#endif
{
    char *FUNC="HCPmstdio_endaccess"; /* for HERROR */
    compinfo_t *info;       /* information on the special element */
    int32 ret;

    info=(compinfo_t *)access_rec->special_info;
    if((ret=(*(info->cinfo.coder_funcs.endaccess))(access_rec))==FAIL)
        HRETURN_ERROR(DFE_CODER,FAIL);
    return(ret);
}   /* HCPmstdio_endaccess() */

