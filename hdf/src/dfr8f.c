#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfr8F.c
 * Purpose: C stubs for Fortran RIS routines
 * Invokes: dfr8.c dfkit.c
 * Contents: 
 *  d8spal:     Set palette to write out with subsequent images
 *  d8first:    Call DFR8restart to reset sequencing to first image
 *  d8igdim:    Call DFR8getdims to get dimensions of next image
 *  d8igimg:    Call DFR8getimage to get next image
 *  d8ipimg:    Call DFR8putimage to write image to new file
 *  d8iaimg:    Call DFR8putimage to add image to existing file
 *  d8irref:    Call DFR8readref to set ref to get next
 *  d8iwref:    Call DFR8writeref to set ref to put next
 *  d8inims:    Call DFR8nimages to get the number of images in the file
 *  d8lref:     Call DFR8lastref to get ref of last image read/written
 *  dfr8lastref:    Call DFR8lastref to get ref of last image read/written
 *  dfr8setpalette: Set palette to write out with subsequent images
 *  dfr8restart:    Call DFR8restart to reset sequencing to first image
 * Remarks: none
 *---------------------------------------------------------------------------*/

#include "hdf.h"

#ifndef DFR8_FNAMES
#   define DFR8_FNAMES
#ifdef DF_CAPFNAMES
#   define nd8spal   FNAME(D8SPAL)
#   define nd8first  FNAME(D8FIRST)
#   define nd8igdim  FNAME(D8IGDIM)
#   define nd8igimg  FNAME(D8IGIMG)
#   define nd8ipimg  FNAME(D8IPIMG)
#   define nd8iaimg  FNAME(D8IAIMG)
#   define nd8irref  FNAME(D8IRREF)
#   define nd8iwref  FNAME(D8IWREF)
#   define nd8inims  FNAME(D8INIMS)
#   define nd8lref   FNAME(D8LREF)
#   define ndfr8lastref      FNAME(DFR8LASTREF)
#   define ndfr8setpalette   FNAME(DFR8SETPALETTE)
#   define ndfr8restart  FNAME(DFR8RESTART)
#else   /* !DF_CAPFNAMES */
#   define nd8spal   FNAME(d8spal)
#   define nd8first  FNAME(d8first)
#   define nd8igdim  FNAME(d8igdim)
#   define nd8igimg  FNAME(d8igimg)
#   define nd8ipimg  FNAME(d8ipimg)
#   define nd8iaimg  FNAME(d8iaimg)
#   define nd8irref  FNAME(d8irref)
#   define nd8iwref  FNAME(d8iwref)
#   define nd8inims  FNAME(d8inims)
#   define nd8lref   FNAME(d8lref)
#   define ndfr8lastref      FNAME(dfr8lastref)
#   define ndfr8setpalette   FNAME(dfr8setpalette)
#   define ndfr8restart  FNAME(dfr8restart)
#endif /* DF_CAPFNAMES */
#endif /* DFR8_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    d8spal
 * Purpose: Set palette to be written out with subsequent images
 * Inputs:  pal: palette to associate with subsequent images
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8setpalette
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8spal(_fcd pal)
#else
nd8spal(pal)
    _fcd pal;
#endif /* PROTOTYPE */
{
    return (DFR8setpalette((uint8 *)_fcdtocp(pal)));
}


/*-----------------------------------------------------------------------------
 * Name:    d8first
 * Purpose: Reset sequencing back to first image
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8restart
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8first(void)
#else
nd8first()
#endif /* PROTOTYPE */
{
    return(DFR8restart());
}

/*-----------------------------------------------------------------------------
 * Name:    d8igdim
 * Purpose: Get dimensions of next image using DFR8getdims
 * Inputs:  filename: name of HDF file
 *          xdim, ydim - integers to return dimensions in
 *          ispal - boolean to indicate whether the image includes a palette
 *          lenfn - length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8getdims
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8igdim(_fcd filename, int32 *xdim, int32 *ydim, int *ispal, int *lenfn)
#else
nd8igdim(filename, xdim, ydim, ispal, lenfn)
    _fcd filename;
    int32 *xdim, *ydim;
    int *ispal, *lenfn;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *lenfn);
    ret =  DFR8getdims(fn, xdim, ydim, ispal);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d8igimg
 * Purpose: Get next image using DFR8getimage
 * Inputs:  filename: name of HDF file
 *          image: space provided for returning image
 *          xdim, ydim: dimension of space provided for image
 *          pal: space of 768 bytes for palette
 *          lenfn: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8getimage
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8igimg(_fcd filename, _fcd image, int32 *xdim, int32 *ydim, _fcd pal, int *lenfn)
#else
nd8igimg(filename, image, xdim, ydim, pal, lenfn)
    _fcd filename, image, pal;
    int32 *xdim, *ydim;
    int *lenfn;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *lenfn);
    ret =  DFR8getimage(fn, (uint8 *)_fcdtocp(image), *xdim, *ydim,
            (uint8 *)_fcdtocp(pal));
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d8ipimg
 * Purpose: Write out image to new file
 * Inputs:  filename: name of HDF file
 *          image: image to write out
 *          xdim, ydim: dimensions of image to write out
 *          compress: compression scheme
 *          lenfn: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8putimage
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8ipimg(_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *compress,
	 int *lenfn)
#else
nd8ipimg(filename, image, xdim, ydim, compress, lenfn)
    _fcd filename, image;
    int32 *xdim, *ydim;
    int *compress, *lenfn;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *lenfn);
    ret = DFR8putimage(fn, (VOIDP)_fcdtocp(image), *xdim, *ydim, *compress);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d8iaimg
 * Purpose: Add image to existing file
 * Inputs:  filename: name of HDF file
 *          image: image to write out
 *          xdim, ydim: dimensions of image to write out
 *          compress: compression scheme
 *          lenfn: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8addimage
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8iaimg(_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *compress,
	 int *lenfn)
#else
nd8iaimg(filename, image, xdim, ydim, compress, lenfn)
    _fcd filename, image;
    int32 *xdim, *ydim;
    int *compress, *lenfn;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *lenfn);
    ret = DFR8addimage(fn, (VOIDP)_fcdtocp(image), *xdim, *ydim, *compress);
    HDfreespace(fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    D8irref
 * Purpose: Set ref of image to get next
 * Inputs:  filename: file to which this applies
 *          ref: reference number of next get
 * Returns: 0 on success, -1 on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFR8Iopen, DFIfind
 * Remarks: checks if image with this ref exists
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8irref(_fcd filename, int32 *ref, int *fnlen)
#else
nd8irref(filename, ref, fnlen)
    _fcd filename;
    int32 *ref;
    int   *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;
    uint16 Ref;
    
    Ref = (uint16) *ref;

    fn = HDf2cstring(filename, *fnlen);
    if (!fn) return -1;
    ret = DFR8readref(fn, Ref);
    HDfreespace(fn);
    return ret;
}


/*-----------------------------------------------------------------------------
 * Name:    d8iwref
 * Purpose: Set ref of image to put next
 * Inputs:  filename: file to which this applies
 *          fnlen: length of the filename
 * Returns: 0 on success, -1 on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFR8writeref
 * Remarks: 
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8iwref(_fcd filename, int32 *ref, int *fnlen)
#else
nd8iwref(filename, ref, fnlen)
    _fcd filename;
    int32 *ref;
    int *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;
    uint16 Ref;

    Ref = (uint16) *ref;

    fn = HDf2cstring(filename, *fnlen);
    ret = DFR8writeref(fn, Ref);
    HDfreespace(fn);
    return (ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d8inims
 * Purpose: How many images are present in this file?
 * Inputs:  filename: file to which this applies
 *          fnlen: length of HDF file name
 * Returns: number of images on success, -1 on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFR8nimages
 * Remarks: 
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd8inims(_fcd filename, int *fnlen)
#else
nd8inims(filename, fnlen)
    _fcd filename;
    int *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *fnlen);
    ret = DFR8nimages(fn);
    HDfreespace(fn);
    return (ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d8lref
 * Purpose: return reference number of last element read or written
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with error set
 * Users:   Fortran stub routine
 * Invokes: DFR8lastref
 *---------------------------------------------------------------------------*/

        FRETVAL(int)
#ifdef PROTOTYPE
nd8lref(void)
#else
nd8lref()
#endif  /* PROTOTYPE */
{
    return(DFR8lastref());
}

/*-----------------------------------------------------------------------------
 * Name:    dfr8lastref
 * Purpose: Return last ref written or read
 * Inputs:  none
 * Returns: ref on success, -1 on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFR8lastref
 * Remarks: 
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfr8lastref(void)
#else
ndfr8lastref()
#endif /* PROTOTYPE */
{
    return(DFR8lastref());
}


/*-----------------------------------------------------------------------------
 * Name:    dfr8setpalette
 * Purpose: Set palette to be written out with subsequent images
 * Inputs:  pal: palette to associate with subsequent images
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8setpalette
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfr8setpalette(_fcd pal)
#else
ndfr8setpalette(pal)
    _fcd pal;
#endif /* PROTOTYPE */
{

    return (DFR8setpalette((uint8 *)_fcdtocp(pal)));
}

/*-----------------------------------------------------------------------------
 * Name:    dfr8restart
 * Purpose: Reset sequencing back to first image
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFR8restart
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndfr8restart(void)
#else
ndfr8restart()
#endif /* PROTOTYPE */
{

    return(DFR8restart());
}

