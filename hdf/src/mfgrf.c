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
 *  mgscomp:    Call GRsetcompress to compress an image in the file [Later]
 *  mgisattr:    Call GRsetattr to write an attribute for an object
 *  mgiscatt:   Call GRsetattr to write a char attribute for an obj.
 *  mgatinf:    Call GRattrinfo get information about an attribute
 *  mggcatt:    Call GRgetattr to read a char attribute from the file
 *  mggnatt:    Call GRgetattr to read a numeric attribute from the file
 *  mggattr:    Call GRgetattr to read an attribute from the file
 *  mgfndat:    Call GRfindattr to get the index of an attribute for a name
 * Remarks: 
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "mfgr.h"

/* Local pre-processor macros */
#define XDIM    0
#define YDIM    1

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

/*-----------------------------------------------------------------------------
 * Name:    mgicreat
 * Purpose: Call GRcreate to create a raster image.
 * Inputs:
 *      grid: GR ID of interface to create image in
 *      name: name of raster image
 *      ncomp: number of components per pixel
 *      nt: number-type of each component
 *      il: interlace scheme to use
 *      dimsizes[2]: dimensions of the image to create
 *      nlen: length of the name string
 * Returns: RI ID on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRcreate
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgicreat(intf * grid, _fcd name, intf *ncomp, intf *nt, intf *il, intf dimsizes[2], intf *nlen)
{
    char       *fn;
    int32       cdims[2];
    intf        ret;

    /* Convert the FORTRAN string into a C string */
    fn = HDf2cstring(name, (intn)*nlen);
    if (fn == NULL)
        return FAIL;

    /* Copy the array of dimensions into an array of the proper type */
    cdims[XDIM]=(int32)dimsizes[XDIM];
    cdims[YDIM]=(int32)dimsizes[YDIM];
    ret = (intf)GRcreate((int32)*grid, fn, (int32)*ncomp,(int32)*nt,(int32)*il,cdims);
    HDfree(fn);

    return(ret);
}   /* end mgicreat() */

/*-----------------------------------------------------------------------------
 * Name:    mgselct
 * Purpose: Call GRselect to choose an existing raster image.
 * Inputs:
 *      grid: GR ID of interface
 *      index: the index of the image to select
 * Returns: RI ID on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRselect
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgselct(intf * grid, intf *index)
{
    return((intf)GRselect((int32)*grid,(int32)*index));
}   /* end mgselct() */

/*-----------------------------------------------------------------------------
 * Name:    mgin2ndx
 * Purpose: Call GRnametoindex to map a raster image name to an index
 * Inputs:
 *      grid: GR ID of interface
 *      name: the name of the raster image to find
 *      nlen: the length of the name
 * Returns: image index on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRnametoindex
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgin2ndx(intf * grid, _fcd name, intf *nlen)
{
    char       *fn;
    intf        ret;

    /* Convert the FORTRAN string into a C string */
    fn = HDf2cstring(name, (intn)*nlen);
    if (fn == NULL)
        return FAIL;

    /* Copy the array of dimensions into an array of the proper type */
    ret = (intf)GRnametoindex((int32)*grid, fn);
    HDfree(fn);

    return(ret);
}   /* end mgin2ndx() */

/*-----------------------------------------------------------------------------
 * Name:    mggiinf
 * Purpose: Call GRgetiminfo to get information about an image
 * Inputs:
 *      riid: RI ID of the image
 *      name: the name of the raster image to find
 *      ncomp: the number of components each pixel has
 *      nt: the number type of each component
 *      il: the interlace of the image components
 *      dimsizes: the dimensions of the image
 *      nattr: the number of attributes the image has
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetiminfo
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmggiinf(intf * riid, _fcd name, intf *ncomp, intf *nt, intf *il, intf *dimsizes, intf *nattr)
{
    int32   t_ncomp,
        t_nt,
        t_il,
        t_dimsizes[2],
        t_nattr;
    intf    ret;

    /* Copy the array of dimensions into an array of the proper type */
    ret = (intf)GRgetiminfo((int32)*riid, (char *)_fcdtocp(name), &t_ncomp, &t_nt, &t_il, t_dimsizes, &t_nattr);
    *ncomp=(intf)t_ncomp;
    *nt=(intf)t_nt;
    *il=(intf)t_il;
    dimsizes[XDIM]=t_dimsizes[XDIM];
    dimsizes[YDIM]=t_dimsizes[YDIM];
    *nattr=(intf)t_nattr;

    return(ret);
}   /* end mggiinf() */

/*-----------------------------------------------------------------------------
 * Name:    mgwcimg
 * Purpose: Call mgwrimg to write char type image data to the file
 * Inputs:
 *      riid: RI ID of the image
 *      start: the starting location of the image data to write
 *      stride: the stride of image data to write
 *      count: the number of pixels in each dimension to write
 *      data: the image data (pixels) to write out
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRwriteimage
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgwcimg(intf * riid, intf *start, intf *stride, intf *count, _fcd data)
{
    return(nmgwrimg(riid, start, stride, count, _fcdtocp(data)));
}   /* end mgwcimg() */

/*-----------------------------------------------------------------------------
 * Name:    mgwrimg
 * Purpose: Call GRwriteimage to write image data to the file
 * Inputs:
 *      riid: RI ID of the image
 *      start: the starting location of the image data to write
 *      stride: the stride of image data to write
 *      count: the number of pixels in each dimension to write
 *      data: the image data (pixels) to write out
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRwriteimage
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgwrimg(intf * riid, intf *start, intf *stride, intf *count, VOIDP data)
{
    int32 t_start[2],
        t_stride[2],
        t_count[2];

    /* Copy the array of dimensions into an array of the proper type */
    t_start[XDIM]=(int32)start[XDIM]; t_start[YDIM]=(int32)start[YDIM];
    t_stride[XDIM]=(int32)stride[XDIM]; t_stride[YDIM]=(int32)stride[YDIM];
    t_count[XDIM]=(int32)count[XDIM]; t_count[YDIM]=(int32)count[YDIM];

    return((intf)GRwriteimage((int32)*riid, t_start, t_stride, t_count, data));
}   /* end mgwrimg() */

/*-----------------------------------------------------------------------------
 * Name:    mgrcimg
 * Purpose: Call mgrdimg to read char type image data from a file
 * Inputs:
 *      riid: RI ID of the image
 *      start: the starting location of the image data to read
 *      stride: the stride of image data to read
 *      count: the number of pixels in each dimension to read
 *      data: the image data (pixels) to read out
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreadimage
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgrcimg(intf * riid, intf *start, intf *stride, intf *count, _fcd data)
{
    return(nmgrdimg(riid, start, stride, count, _fcdtocp(data)));
}   /* end mgrcimg() */

/*-----------------------------------------------------------------------------
 * Name:    mgrdimg
 * Purpose: Call GRreadimage to read image data from a file
 * Inputs:
 *      riid: RI ID of the image
 *      start: the starting location of the image data to read
 *      stride: the stride of image data to read
 *      count: the number of pixels in each dimension to read
 *      data: the image data (pixels) to read out
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreadimage
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgrdimg(intf * riid, intf *start, intf *stride, intf *count, VOIDP data)
{
    int32 t_start[2],
        t_stride[2],
        t_count[2];

    /* Copy the array of dimensions into an array of the proper type */
    t_start[XDIM]=(int32)start[XDIM]; t_start[YDIM]=(int32)start[YDIM];
    t_stride[XDIM]=(int32)stride[XDIM]; t_stride[YDIM]=(int32)stride[YDIM];
    t_count[XDIM]=(int32)count[XDIM]; t_count[YDIM]=(int32)count[YDIM];

    return((intf)GRreadimage((int32)*riid, t_start, t_stride, t_count, data));
}   /* end mgrdimg() */

/*-----------------------------------------------------------------------------
 * Name:    mgendac
 * Purpose: Call GRendaccess to terminate access to a raster image
 * Inputs:
 *      riid: RI ID of the image
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRendaccess
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgendac(intf * riid)
{
    return((intf)GRendaccess((int32)*riid));
}   /* end mgendac() */

/*-----------------------------------------------------------------------------
 * Name:    mgid2rf
 * Purpose: Call GRidtoref to map a RI index into a reference #
 * Inputs:
 *      riid: RI ID of the image
 * Returns: ref. # on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRidtoref
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgid2rf(intf * riid)
{
    return((intf)GRidtoref((int32)*riid));
}   /* end mgid2rf() */

/*-----------------------------------------------------------------------------
 * Name:    mgr2idx
 * Purpose: Call GRreftoindex to map a reference # into a ri index
 * Inputs:
 *      grid: GR ID of the file
 *      ref: ref. # of the image
 * Returns: a valid index # on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreftoindex
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgr2idx(intf * grid, intf *ref)
{
    return((intf)GRreftoindex((int32)*grid,(uint16)*ref));
}   /* end mgr2idx() */

/*-----------------------------------------------------------------------------
 * Name:    mgrltil
 * Purpose: Call GRreqlutil to request the interlacing scheme for the next LUT
 * Inputs:
 *      riid: RI ID of the image
 *      il: interlace scheme for next read
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreqlutil
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgrltil(intf * riid, intf *il)
{
    return((intf)GRreqlutil((int32)*riid,(intn)*il));
}   /* end mgrltil() */

/*-----------------------------------------------------------------------------
 * Name:    mgrimil
 * Purpose: Call GRreqimageil to request the interlacing scheme for the next image
 * Inputs:
 *      riid: RI ID of the image
 *      il: interlace scheme for next read
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreqimageil
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgrimil(intf * riid, intf *il)
{
    return((intf)GRreqimageil((int32)*riid,(intn)*il));
}   /* end mgrimil() */

/*-----------------------------------------------------------------------------
 * Name:    mggltid
 * Purpose: Call GRgetlutid to get the palette ID for an image
 * Inputs:
 *      riid: RI ID of the image
 *      lut_index: index of the palette to select
 * Returns: valud LUT ID on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetlutid
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmggltid(intf * riid, intf *lut_index)
{
    return((intf)GRgetlutid((int32)*riid,(intn)*lut_index));
}   /* end mggltid() */

/*-----------------------------------------------------------------------------
 * Name:    mgglinf
 * Purpose: Call GRgetlutinfo to get information about a palette
 * Inputs:
 *      lutid: LUT ID of the image
 *      ncomp: number of components per palette entry
 *      nt: number-type of components in palette entry
 *      il: interlace of components in palette entry
 *      nentries: number of palette entries
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetlutinfo
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgglinf(intf * lutid, intf *ncomp, intf *nt, intf *il, intf *nentries)
{
    int32 t_ncomp, t_nt, t_il, t_nentries;
    intf status;

    status=(intf)GRgetlutinfo((int32)*lutid,&t_ncomp,&t_nt,&t_il,&t_nentries);
    *ncomp=(intf)t_ncomp;
    *nt=(intf)t_nt;
    *il=(intf)t_il;
    *nentries=(intf)t_nentries;
    return(status);
}   /* end mgglinf() */

/*-----------------------------------------------------------------------------
 * Name:    mgwrlut
 * Purpose: Call GRwritelut to write data into a palette
 * Inputs:
 *      lutid: LUT ID of the image
 *      ncomp: number of components per palette entry
 *      nt: number-type of components in palette entry
 *      il: interlace of components in palette entry
 *      nentries: number of palette entries
 *      data: data to write
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRwritelut
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgwrlut(intf * lutid, intf *ncomp, intf *nt, intf *il, intf *nentries, VOIDP data)
{
    return((intf)GRwritelut((int32)*lutid,(int32)*ncomp,(int32)*nt,(int32)*il,(int32)*nentries,data));
}   /* end mgwrlut() */

/*-----------------------------------------------------------------------------
 * Name:    mgrdlut
 * Purpose: Call GRreadlut to read data from a palette
 * Inputs:
 *      lutid: LUT ID of the image
 *      data: data to read
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRreadlut
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgrdlut(intf * lutid, VOIDP data)
{
    return((intf)GRreadlut((int32)*lutid,data));
}   /* end mgrdlut() */

/*-----------------------------------------------------------------------------
 * Name:    mgisxfil
 * Purpose: Call GRsetexternalfile to convert an image into an external image
 * Inputs:
 *      riid: RI ID of the image
 *      filename: filename of the external file
 *      offset: offset in the external file to place image at
 *      nlen: the length of the name
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRsetexternalfile
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgisxfil(intf * riid, _fcd filename, intf *offset, intf *nlen)
{
    char       *fn;
    intf        ret;

    /* Convert the FORTRAN string into a C string */
    fn = HDf2cstring(filename, (intn)*nlen);
    if (fn == NULL)
        return FAIL;

    ret=(intf)GRsetexternalfile((int32)*riid,fn,(int32)*offset);
    HDfree(fn);

    return(ret);
}   /* end mgisxfil() */

/*-----------------------------------------------------------------------------
 * Name:    mgsactp
 * Purpose: Call GRsetaccesstype to determine access mode for an image
 * Inputs:
 *      riid: RI ID of the image
 *      accesstype: the type of access to image data
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRsetaccesstype
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgsactp(intf * riid, intf *accesstype)
{
    return((intf)GRsetaccesstype((int32)*riid,(int32)*accesstype));
}   /* end mgsactp() */

/*-----------------------------------------------------------------------------
 * Name:    mgiscatt
 * Purpose: Call mgisatt to store a char attribute about an image
 * Inputs:
 *      riid: RI ID of the image
 *      name: the name of the attribute
 *      nt: the number-type of the attribute
 *      count: the number of values in the attribute
 *      data: the data for the attribute
 *      nlen: the length of the name
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRsetattr
 *---------------------------------------------------------------------------*/
FRETVAL(intf)
nmgiscatt(intf * riid, _fcd name, intf *nt, intf *count, _fcd data, intf *nlen)
{
    return(nmgisattr(riid, name, nt, count, (VOIDP) _fcdtocp(data),
                    nlen));
}   /* end mgiscatt() */
/*-----------------------------------------------------------------------------
 * Name:    mgisattr
 * Purpose: Call GRsetattr to store an attribute about an image
 * Inputs:
 *      riid: RI ID of the image
 *      name: the name of the attribute
 *      nt: the number-type of the attribute
 *      count: the number of values in the attribute
 *      data: the data for the attribute
 *      nlen: the length of the name
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRsetattr
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgisattr(intf * riid, _fcd name, intf *nt, intf *count, VOIDP data, intf *nlen)
{
    char       *fn;
    intf        ret;

    /* Convert the FORTRAN string into a C string */
    fn = HDf2cstring(name, (intn)*nlen);
    if (fn == NULL)
        return FAIL;

    ret=(intf)GRsetattr((int32)*riid,fn,(int32)*nt,(int32)*count,data);
    HDfree(fn);

    return(ret);
}   /* end mgisattr() */

/*-----------------------------------------------------------------------------
 * Name:    mgatinf
 * Purpose: Call GRattrinfo to get information about an attribute
 * Inputs:
 *      riid: RI ID of the image
 *      index: the index of the attribute
 *      name: the name of the attribute
 *      nt: the number-type of the attribute
 *      count: the number of values in the attribute
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRattrinfo
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgatinf(intf * riid, intf *index, _fcd name, intf *nt, intf *count)
{
    int32 t_nt, t_count;
    intf        ret;

    ret=(intf)GRattrinfo((int32)*riid,(int32)*index,(char *)_fcdtocp(name),&t_nt,&t_count);
    *nt=(intf)t_nt;
    *count=(intf)t_count;

    return(ret);
}   /* end mgatinf() */

/*-----------------------------------------------------------------------------
 * Name:    mggcatt
 * Purpose: Call mggnatt to get a char attribute
 * Inputs:
 *      riid: RI ID of the image
 *      index: the index of the attribute
 *      data: the data for the attribute
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetattr
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmggcatt(intf * riid, intf *index, _fcd data)
{
    return(nmggnatt(riid, index, (VOIDP) _fcdtocp(data)));
}   /* end mggcatt() */

/*-----------------------------------------------------------------------------
 * Name:    mggnatt
 * Purpose: Call GRgetattr to get a numeric attribute
 * Inputs:
 *      riid: RI ID of the image
 *      index: the index of the attribute
 *      data: the data for the attribute
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetattr
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmggnatt(intf * riid, intf *index, VOIDP data)
{
    return((intf)GRgetattr((int32)*riid,(int32)*index,data));
}   /* end mggnatt() */

/*-----------------------------------------------------------------------------
 * Name:    mggattr
 * Purpose: Call GRgetattr to get an attribute
 * Inputs:
 *      riid: RI ID of the image
 *      index: the index of the attribute
 *      data: the data for the attribute
 * Returns: SUCCEED on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRgetattr
 * Remarks: This routine is replaced by mggcatt and mggmatt
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmggattr(intf * riid, intf *index, VOIDP data)
{
    return((intf)GRgetattr((int32)*riid,(int32)*index,data));
}   /* end mggattr() */

/*-----------------------------------------------------------------------------
 * Name:    mgifndat
 * Purpose: Call GRfindattr to locate an attribute
 * Inputs:
 *      riid: RI ID of the image
 *      name: the name for the attribute
 *      nlen: the length of the name for the attribute
 * Returns: attribute index on success, FAIL on failure
 * Users:   HDF Fortran programmers
 * Invokes: GRfindattr
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nmgifndat(intf * riid, _fcd name, intf *nlen)
{
    char       *fn;
    intf        ret;

    /* Convert the FORTRAN string into a C string */
    fn = HDf2cstring(name, (intn)*nlen);
    if (fn == NULL)
        return FAIL;

    ret=(intf)GRfindattr((int32)*riid,fn);
    HDfree(fn);

    return(ret);
}   /* end mgifndat() */

