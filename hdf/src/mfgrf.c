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

/*-----------------------------------------------------------------------------
 * File:    mfsdf.c
 * Purpose: C stubs for Fortran GR routines
 * Invokes: mfgr.c
 * Contents:
 *  mgstart:    Call GRstart to initialize the GR interface for a file
 *  mgfinfo:    Call GRfileinfo for information about the file
 *  mgend:      Call GRend to close down the GR interface to a file
 *  mgcreat:    Call GRcreate to create a raster image
 *  mgselct:    Call GRselect to select an existing raster image for I/O
 *  mgn2ndx:    Call GRnametoindex to map an image's name into a index in a file
 *  mggiinf:    Call GRgetiminfo to get information about an image 
 *  mgwrimg:    Call GRwriteimage to write image data to the file
 *  mgrdimg:    Call GRreadimage to read image data from the file
 *  mgendac:    Call GRendaccess to end access to a raster image
 *  mggdid:     Call GRgetdimid to get a dimension ID for an image [Later]
 *  mgsdnam:    Call GRsetdimname to set a dimension's name [Later]
 *  mggdinf:    Call GRgetdiminfo to get information about a dimension [Later]
 *  mgid2rf:    Call GRidtoref to map an RI ID into a ref. # for annotating
 *  mgr2idx:    Call GRreftoindex to map a ref. # into an index for the image
 *  mgrltil:    Call GRreqlutil to request the interlace of the next LUT read
 *  mgrimil:    Call GRreqimageil to request the interlace of the next image read
 *  mggltid:    Call GRgetlutid to get a palette ID for an image
 *  mgglinf:    Call GRgetlutinfo to get information about a palette
 *  mgwrlut:    Call GRwritelut to write a palette to the file
 *  mgrdlut:    Call GRreadlut to read a palette from the file
 *  mgsxfil:    Call GRsetexternalfile to move an image into an external file
 *  mgsactp:    Call GRsetaccesstype to set the access type for an image
 *  mgscomp:    Call GRsetcompress to compress an image in the file
 *  mgsattr:    Call GRsetattr to write an attribute for an object
 *  mgatinf:    Call GRattrinfo get information about an attribute
 *  mggattr:    Call GRgetattr to read an attribute from the file
 *  mgfndat:    Call GRfindattr to get the index of an attribute for a name
 * Remarks: 
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "mfgr.h"

/* Prototypes */
extern FRETVAL(intf) ndsiwref(_fcd filename, intf * fnlen, intf * ref);
extern FRETVAL(intf) ndsisslab(_fcd filename, intf * fnlen);
extern FRETVAL(intf) ndsirslab(_fcd filename, intf * fnlen, intf start[], intf slab_size[],
                               intf stride[], VOIDP buffer, intf buffer_size[]);

/*-----------------------------------------------------------------------------
 * Name:    mgstart
 * Purpose: Call GRstart to initialize the GR interface for a file
 * Inputs:  fid: HDF file ID of file to initialize
 * Returns: GR ID on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRstart
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgstart(intf * fid)
{
    return((intf)GRstart((int32)*fid));
}   /* end mgstart() */

/*-----------------------------------------------------------------------------
 * Name:    mgfinfo
 * Purpose: Call GRfileinfo to get information about the images in a file
 * Inputs:  grid: GR ID of file for information
 *          n_datasets: # of raster images in the file
 *          n_attrs: # of "global" GR attributes
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRfileinfo
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgfinfo(intf * grid,intf *n_datasets,intf *n_attrs)
{
    int32 n_data, n_attr;
    intf ret;

    ret=(intf)GRfileinfo((int32)*grid,&n_data,&n_attr);
    *n_datasets=(intf)n_data;
    *n_attrs=(intf)n_attr;
    return(ret);
}   /* end mgfinfo() */

/*-----------------------------------------------------------------------------
 * Name:    mgend
 * Purpose: Call GRend to close the GR interface for a file
 * Inputs:  grid: GR ID of interface to close
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRend
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgend(intf * grid)
{
    return((intf)GRend((int32)*grid));
}   /* end mgend() */

