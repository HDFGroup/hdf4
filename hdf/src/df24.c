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
 * File:    df24.c
 * Purpose: read and write 24-bit raster images
 * Invokes: dfgr.c
 * Contents:
 *  DF24getdims: get dimensions of image
 *  DF24reqil: use this interlace when returning image
 *  DF24getimage: read in image
 *  DF24setdims: set dimensions of image
 *  DF24addimage: write out image
 *
 * Remarks: A RIG specifies attributes associated with an image- lookup table,
 *          dimension, compression, color compensation etc.
 *---------------------------------------------------------------------------*/


#include "dfgr.h"
#include "herr.h"

static int Newdata = 0;                /* does Readrig contain fresh data? */
static int dimsset = 0;                /* have dimensions been set? */

#define LUT     0
#define IMAGE   1


/*-----------------------------------------------------------------------------
 * Name:    DF24getdims
 * Purpose: get dimensions of next image RIG
 * Inputs:  filename: name of HDF file
 *          pxdim, pydim: pointer to locations for returning x,y dimensions
 *          pil: location for returning interlace of image in file
 * Returns: 0 on success, -1 on failure with DFerror set
 *          *pxdim, *pydim, *pil set on success
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIgetdims
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24getdims(char *filename, int32 *pxdim, int32 *pydim, int *pil)
#else
int DF24getdims(filename, pxdim, pydim, pil)
    char *filename;
    int32 *pxdim, *pydim;
    int *pil;
#endif
{
    int ncomps;

    do {
        if (DFGRIgetdims(filename, pxdim, pydim, &ncomps, pil, IMAGE)<0)
            return FAIL;
    } while (ncomps!=3);

    Newdata = 1;
    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DF24reqil
 * Purpose: get next image with specified interlace
 * Inputs:  il: interlace to get next image with
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIreqil
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24reqil(int il)
#else
int DF24reqil(il)
    int il;
#endif
{
    return(DFGRIreqil(il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    DF24getimage
 * Purpose: get image from next RIG
 * Inputs:  filename: name of HDF file
 *          image: pointer to space to return image
 *          xdim, ydim: dimensions of space to return lut
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIgetimlut
 * Remarks: space is assumed to be xdim * ydim * 3 bytes
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24getimage(char *filename, VOIDP image, int32 xdim, int32 ydim)
#else
int DF24getimage(filename, image, xdim, ydim)
    char *filename;
    VOIDP image;
    int32 xdim, ydim;
#endif
{
    int ret, il;
    int32 tx, ty;

    if (!Newdata && DF24getdims(filename, &tx, &ty, &il) == FAIL)
       return FAIL;
    ret = DFGRIgetimlut(filename, image, xdim, ydim, IMAGE, 0);
    Newdata = 0;
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    DF24setdims
 * Purpose: set dimensions of image to write next
 * Inputs:  xdim, ydim: dimensions of image
 *          il: interlace of image
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetdims
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24setdims(int32 xdim, int32 ydim)
#else
int DF24setdims(xdim, ydim)
    int32 xdim, ydim;
#endif
{
    dimsset = 1;
    return(DFGRIsetdims(xdim, ydim, 3, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    DF24setil
 * Purpose: set interlace of image to write next
 * Inputs:  il: interlace of image
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIsetil
 * Remarks: none
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24setil(int il)
#else
int DF24setil(il)
    int il;
#endif
{
    return(DFGRIsetil(il, IMAGE));
}

/*-----------------------------------------------------------------------------
 * Name:    DF24restart
 * Purpose: restart file
 * Inputs:
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIrestart
 * Remarks: none
 *---------------------------------------------------------------------------*/
#ifdef PROTOTYPE
int DF24restart(void)
#else
int DF24restart()
#endif
{
    return DFGRIrestart();
}

/*-----------------------------------------------------------------------------
 * Name:    DF24addimage
 * Purpose: Write out image
 * Inputs:  filename: name of HDF file
 *          image: image to write
 *          xdim, ydim: dimensions of array image
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF HLL (high-level library) users, utilities, other routines
 * Invokes: DFGRIaddimlut
 * Remarks: array image is assumed to be xdim * ydim * 3 bytes
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24addimage(char *filename, VOIDP image, int32 xdim, int32 ydim)
#else
int DF24addimage(filename, image, xdim, ydim)
    char *filename;
    VOIDP image;
    int32 xdim, ydim;
#endif
{
    /* 0 == C */
    if (!dimsset && DFGRIsetdims(xdim, ydim, 3, IMAGE) == FAIL)
       return FAIL;
    dimsset = 0; /* reset to new rig */

    return(DFGRIaddimlut(filename, image, xdim, ydim, IMAGE, 0, 0));
}

#ifdef PROTOTYPE
int DF24putimage(char *filename, VOIDP image, int32 xdim, int32 ydim)
#else
int DF24putimage(filename, image, xdim, ydim)
    char *filename;
    VOIDP image;
    int32 xdim, ydim;
#endif
{
    /* 0 == C */
    if (!dimsset && DFGRIsetdims(xdim, ydim, 3, IMAGE) == FAIL)
       return FAIL;
    dimsset = 0; /* reset to new rig */

    return(DFGRIaddimlut(filename, image, xdim, ydim, IMAGE, 0, 1));
}


/*-----------------------------------------------------------------------------
 * Name:    DF24readref
 * Purpose: Set ref of 24-rig to get next
 * Inputs:  filename: file to which this applies
 *          ref: reference number of next get
 * Returns: 0 on success, -1 on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFGRreadref
 * Remarks: checks if 24-rig with this ref exists
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DF24readref(char *filename, uint16 ref)
#else
int DF24readref(filename, ref)
    char *filename;
    uint16 ref;
#endif
{
    return (DFGRreadref(filename, ref));
}
