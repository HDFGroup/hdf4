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
 * File:    df24F.c
 * Purpose: read and write 24-bit raster images
 * Invokes: dfgr.c df24.c
 * Contents: 
 *  d2reqil_: use this interlace when returning image
 *  df24reqil_: use this interlace when returning image
 *  d2sdims_: set dimensions of image
 *  df24setdims_: set dimensions of image
 *  d2setil_: set interlace for image
 *  df24setil_: set interlace for image
 *  d2first_: restart 24 bit raster
 *  df24restart_: restart 24 bit raster
 *  d2igdim_: get dimensions of image
 *  d2igimg_: read in image
 *  d2iaimg_: write out image
 *
 * Remarks:A RIG specifies attributes associated with an image - lookup table, 
 *          dimension, compression, color compensation etc.
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "dfgr.h"

#ifndef DF24_FNAMES
#   define DF24_FNAMES
#ifdef DF_CAPFNAMES
#   define nd2reqil  FNAME(D2REQIL)
#   define ndf24reqil    FNAME(DF24REQIL)
#   define nd2sdims  FNAME(D2SDIMS)
#   define ndf24setdims  FNAME(DF24SETDIMS)
#   define nd2setil  FNAME(D2SETIL)
#   define ndf24setil    FNAME(DF24SETIL)
#   define nd2first  FNAME(D2FIRST)
#   define ndf24restart  FNAME(DF24RESTART)
#   define nd2igdim  FNAME(D2IGDIM)
#   define nd2igimg  FNAME(D2IGIMG)
#   define nd2iaimg  FNAME(D2IAIMG)
#   define nd2irref  FNAME(D2IRREF)
#else
#   define nd2reqil  FNAME(d2reqil)
#   define ndf24reqil    FNAME(df24reqil)
#   define nd2sdims  FNAME(d2sdims)
#   define ndf24setdims  FNAME(df24setdims)
#   define nd2setil  FNAME(d2setil)
#   define ndf24setil    FNAME(df24setil)
#   define nd2first  FNAME(d2first)
#   define ndf24restart  FNAME(df24restart)
#   define nd2igdim  FNAME(d2igdim)
#   define nd2igimg  FNAME(d2igimg)
#   define nd2iaimg  FNAME(d2iaimg)
#   define nd2irref  FNAME(d2irref)
#endif /* DF_CAPFNAMES */
#endif /* DF24_FNAMES */

#define LUT     0
#define IMAGE   1

static int dimsset = 0;


/*-----------------------------------------------------------------------------
 * Name:    d2reqil
 * Purpose: get next image with specified interlace
 * Inputs:  il: interlace to get next image with
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIreqil
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2reqil(int *il)
#else
nd2reqil(il)
    int *il;
#endif /* PROTOTYPE */
{
    return(DFGRIreqil(*il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    d2sdims
 * Purpose: set dimensions of image to write next
 * Inputs:  xdim, ydim: dimensions of image
 *          il: interlace of image
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetdims
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2sdims(int32 *xdim, int32 *ydim)
#else
nd2sdims(xdim, ydim)
    int32 *xdim, *ydim;
#endif /* PROTOTYPE */
{
    dimsset = 1;
    return(DFGRIsetdims(*xdim, *ydim, 3, IMAGE));
}


/*-----------------------------------------------------------------------------
 * Name:    d2igdim
 * Purpose: get dimensions of next image RIG
 * Inputs:  filename: name of HDF file
 *          pxdim, pydim: pointer to locations for returning x,y dimensions
 *          pil: location for returning interlace of image in file
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 *          *pxdim, *pydim, *pil set on success
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DF24getdims
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2igdim(_fcd filename, int32 *pxdim, int32 *pydim, int *pil, int *fnlen)
#else
nd2igdim(filename, pxdim, pydim, pil, fnlen)
    _fcd filename;
    int32 *pxdim, *pydim;
    int *pil, *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *fnlen);
    ret =  DF24getdims(fn, pxdim, pydim, pil);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d2igimg
 * Purpose: get image from next RIG
 * Inputs:  filename: name of HDF file
 *          image: pointer to space to return image
 *          xdim, ydim: dimensions of space to return image
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIgetimlut
 * Remarks: space is assumed to be xdim * ydim * 3 bytes
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2igimg(_fcd filename, _fcd image, int32 *xdim, int32 *ydim, int *fnlen)
#else
nd2igimg(filename, image, xdim, ydim, fnlen)
    _fcd filename;
    _fcd image;
    int32 *xdim, *ydim;
    int *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *fnlen);
    ret =  DF24getimage(fn, (VOIDP)_fcdtocp(image), *xdim, *ydim);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d2iaimg
 * Purpose: Write out image
 * Inputs:  filename: name of HDF file
 *          image: image to write
 *          xdim, ydim: dimensions of array image
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIaddimlut
 * Remarks: array image is assumed to be xdim * ydim * ncomps bytes
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2iaimg(_fcd filename, _fcd image, int32 *xdim, int32 *ydim,
	 int *fnlen, int *newfile)
#else
nd2iaimg(filename, image, xdim, ydim, fnlen, newfile)
    _fcd filename;
    _fcd image;
    int32 *xdim, *ydim;
    int *fnlen, *newfile;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    if (!dimsset)
        if (DFGRIsetdims(*xdim, *ydim, 3, IMAGE)<0) return(-1);

    fn = HDf2cstring(filename, *fnlen);
    ret = DFGRIaddimlut(fn, (VOIDP)_fcdtocp(image), *xdim, *ydim,
            IMAGE, 1, *newfile);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    d2setil
 * Purpose: set interlace store with following images
 * Inputs:  il: interlace to set
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetil
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2setil(int *il)
#else
nd2setil(il)
    int *il;
#endif /* PROTOTYPE */
{
    return (DFGRIsetil(*il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    df24first
 * Purpose: restart 24 bit raster file
 * Inputs:  
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIrestart
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2first(void)
#else
nd2first()
#endif /* PROTOTYPE */
{
    return (DFGRIrestart());
}


/*-----------------------------------------------------------------------------
 * Name:    df24reqil
 * Purpose: get next image with specified interlace
 * Inputs:  il: interlace to get next image with
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIreqil
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndf24reqil(int *il)
#else
ndf24reqil(il)
    int *il;
#endif /* PROTOTYPE */
{
    return(DFGRIreqil(*il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    df24setdims
 * Purpose: set dimensions of image to write next
 * Inputs:  xdim, ydim: dimensions of image
 *          il: interlace of image
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetdims
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndf24setdims(int32 *xdim, int32 *ydim)
#else
ndf24setdims(xdim, ydim)
    int32 *xdim, *ydim;
#endif /* PROTOTYPE */
{
    dimsset = 1;
    return(DFGRIsetdims(*xdim, *ydim, 3, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    df24setil
 * Purpose: set interlace store with following images
 * Inputs:  il: interlace to set
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetil
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndf24setil(int *il)
#else
ndf24setil(il)
    int *il;
#endif /* PROTOTYPE */
{
    return (DFGRIsetil(*il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    df24restart
 * Purpose: restart 24 bit raster file
 * Inputs:  
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIrestart
 * Remarks: none
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
ndf24restart(void)
#else
ndf24restart()
#endif /* PROTOTYPE */
{
    return (DFGRIrestart());
}
/*-----------------------------------------------------------------------------
 * Name:    d2irref
 * Purpose: Internal stub for setting ref of rig to read next
 * Inputs:  filename: name of HDF file
 *          ref: reference
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRreadref
 * Remarks: 
 *---------------------------------------------------------------------------*/

    FRETVAL(int)
#ifdef PROTOTYPE
nd2irref(_fcd filename, int *ref, int *fnlen)
#else
nd2irref(filename, ref, fnlen)
    _fcd filename;
    int *ref;
    int *fnlen;
#endif /* PROTOTYPE */
{
    char *fn;
    int ret;

    fn = HDf2cstring(filename, *fnlen);
    ret = DFGRreadref(fn, (uint16)*ref);
    HDfreespace(fn);
    return(ret);
}
