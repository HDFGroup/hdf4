/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-----------------------------------------------------------------------------
 * File:    dfsdf.c
 * Purpose: C stubs for Fortran SDS routines
 * Invokes: dfsd.c dfkit.c
 * Contents:
 *  dsgdast:       Call DFSDgetdatastrs to get attributes of data
 *  dsigdis:       Call DFSDgetdimstrs to get attributes of a dimension
 *  dsgdisc:       Call DFSDgetdimscale to get scale for a dimension
 *  dsgrang:       Call DFSDgetmaxmin to get max and min data values
 *  dssdims:       Call DFSDsetdims to set dimensions for subsequent SDGs
 *  dssdisc:       Call DFSDsetdimscale to set scale for subsequent SDGs
 *  dssrang:       Call DFSDsetmaxmin to set max/min values for subsequent SDGs
 *  dsclear:       Call DFSDclear to erase values set for subsequent SDGs
 *  dsslens:       Call DFSDsetlengths to set maximum lengths of string
 *  dsgdiln:       Call DFSDgetdimlen to get lengths of strings for a dimension
 *  dsgdaln:       Call DFSDgetdatalen to get lengths of data strings
 *  dsfirst:       Call DFSDrestart to get SDGs again from beginning of file
 *  dspslc:        Call DFSDIputslice to write slice to file
 *  dseslc:        Call DFSDendslice to end slice writes, write SDG to file
 *  dssnt:         Call DFSDsetNT to set number type
 *  dsgnt:         Call DFSDgetNT to get number type for reading
 *  dsigdim:       Call DFSDgetdims to get dimensions of next SDG
 *  dsigdat:       Call DFSDgetdata to get data values
 *  dsipdat:       Call DFSDIputdata to write SDG to new file
 *  dsiadat:       Call DFSDIputdata to append SDG to existing file
 *  dsigslc:       Call DFSDIgetslice to get slice from file
 *  dsisslc:       Call DFSDstartslice to set up to write slice
 *  dslref:        Call DFSDlastref to get ref of last SDS accessed
 *  dsinum:        Call DFSDndatasets to get number of SDG in the file
 *  dsip32s:       Call DFSDpre32sdg to test if the sdg was written by HDF prior to
 *                      version 3.2
 *  dfsdgetdatastrs_:Call DFSDgetdatastrs to get attributes of data
 *  dfsdgetdimscale_:Call DFSDgetdimscale to get scale for a dimension
 *  dfsdgetrange_:  Call DFSDgetmaxmin to get max and min data values
 *  dfsdsetdims_:   Call DFSDsetdims to set dimensions for subsequent SDGs
 *  dfsdsetdimscale_:Call DFSDsetdimscale to set scale for subsequent SDGs
 *  dfsdsetrange_:  Call DFSDsetmaxmin to set max/min values for subsequent SDGs
 *  dfsdclear_:     Call DFSDclear to erase values set for subsequent SDGs
 *  dfsdsetlengths_:Call DFSDsetlengths to set maximum lengths of string
 *  dfsdgetdimlen_: Call DFSDgetdimlen to get lengths of strings for a dimension
 *  dfsdgetdatalen_:Call DFSDgetdatalen to get lengths of data strings
 *  dfsdrestart_:   Call DFSDrestart to get SDGs again from beginning of file
 *  dfsdputslice_:  Call DFSDIputslice to write slice to file
 *  dfsdendslice_:  Call DFSDendslice to end slice writes, write SDG to file
 *  dfsdsetnt_:     Call DFSDsetNT to set number type
 *  dfsdgetnt_:     Call DFSDgetNT to get number type
 *  dfsdlastref_:   Call DFSDlastref to get ref of last SDS accessed
 *  dsiwref:        Call DFSDwriteref to set up next ref to write
 *  dssfill:        Call DFSDsetfillvalue to set fill value for SDS
 *  dsgfill:        Call DFSDgetfillvalue to get fill value from SDS
 *  dsisslab:       Call DFSDstartslab to set up write to SDS
 *  dswslab:        Call DFSDwriteslab to write slab to file
 *  dseslab:        Call DFSDendslab to end slab writes, write NDG to file
 *  dsirslab:       Call DFSDreadslab to get slab from SDS
 * Remarks: no C stubs needed for the put string routines, only Fortran stubs
 *---------------------------------------------------------------------------*/

#include "hdf_priv.h"
#include "dfsd_priv.h"
#include "hproto_fortran.h"

/*-----------------------------------------------------------------------------
 * Name:    dsgdisc
 * Purpose: Call DFSDgetdimscale to get scale for a dimension
 * Inputs:  dim: dimension to get attributes for
 *          maxsize: size of scale array
 *          scale: array to return scale in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimscale
 *---------------------------------------------------------------------------*/

intf
ndsgdisc(intf *dim, intf *maxsize, void *scale)
{
    int rank, cdim;
    int isndg;

    DFSDIisndg(&isndg);
    if (isndg) {
        DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    return DFSDgetdimscale(cdim, *maxsize, scale);
}

/*-----------------------------------------------------------------------------
 * Name:    dsgrang
 * Purpose: Call DFSDgetrange to get maximum and minimum data values
 * Inputs:  pmax: float to return maximum in
 *          pmin: float to return minimum in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetrange
 *---------------------------------------------------------------------------*/

intf
ndsgrang(void *pmax, void *pmin)
{
    return DFSDgetrange(pmax, pmin);
}

/*-----------------------------------------------------------------------------
 * Name:    dssdims
 * Purpose: Call DFSDsetdims to set dimensions for subsequent SDGs
 * Inputs:  rank: no of dimensions of SDG
 *          dimsizes: array containing dimensions of SDG
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetdims
 *---------------------------------------------------------------------------*/

intf
ndssdims(intf *rank, intf dimsizes[])
{
    int32 i, *cdims, *p;
    intf  ret;

    p = (int32 *)malloc((size_t)(*rank) * sizeof(int32));
    if (p == NULL)
        return FAIL;
    cdims = p;
    for (i = 1; i <= *rank; i++) {
        *p = dimsizes[*rank - i];
        p++;
    }

    ret = DFSDsetdims((int)*rank, cdims);
    free(cdims);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dssdisc
 * Purpose: Call DFSDsetdimscale to set scales for subsequent SDGs
 * Inputs:  dim: dimension to set scale for
 *          dimsize: size of array scale
 *          scale: array of scale values
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetdimscale
 *---------------------------------------------------------------------------*/

intf
ndssdisc(intf *dim, intf *dimsize, void *scale)
{
    int cdim;
    int rank;

    DFSDIgetwrank(&rank);
    if (rank < *dim)
        return FAIL;
    cdim = rank - (int)*dim + 1;

    return DFSDsetdimscale(cdim, *dimsize, scale);
}

/*-----------------------------------------------------------------------------
 * Name:    dssrang
 * Purpose: Call DFSDsetrange to set max and min values for this SDG
 * Inputs:  max, min: max and min data values
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetrange
 * Remarks: Max and Min are set only for next SDG, reset to NULL after
 *---------------------------------------------------------------------------*/

intf
ndssrang(void *max, void *min)
{
    return DFSDsetrange(max, min);
}

/*-----------------------------------------------------------------------------
 * Name:    dsclear
 * Purpose: Call DFSDclear to erase values set for subsequent SDGs
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDclear
 *---------------------------------------------------------------------------*/

intf
ndsclear(void)
{
    return DFSDclear();
}

/*-----------------------------------------------------------------------------
 * Name:    dsslens
 * Purpose: Call DFSDsetlengths to set max lengths of strings
 * Inputs:  maxlen_label, maxlen_unit, maxlen_format, maxlen_coordsys: max lens
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetlengths
 *---------------------------------------------------------------------------*/

intf
ndsslens(intf *maxlen_label, intf *maxlen_unit, intf *maxlen_format, intf *maxlen_coordsys)
{
    return (
        DFSDsetlengths((int)*maxlen_label, (int)*maxlen_unit, (int)*maxlen_format, (int)*maxlen_coordsys));
}

/*-----------------------------------------------------------------------------
 * Name:    dsgdiln
 * Purpose: Call DFSDgetdimlen to get actual lengths of strings
 * Inputs:  dim: dimension to get lengths for
 *          llabel, lunit, lformat: integers to return lengths of each string in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimlen
 *---------------------------------------------------------------------------*/

intf
ndsgdiln(intf *dim, intf *llabel, intf *lunit, intf *lformat)
{
    int  rank, cdim;
    intf ret;
    int  isndg;
    int  cllabel, clunit, clformat; /* convert between intf and int */

    ret = DFSDIisndg(&isndg);
    if (isndg) {
        ret = DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    ret = (intf)DFSDgetdimlen(cdim, &cllabel, &clunit, &clformat);
    if (ret != FAIL) { /* if ok, copy the values over */
        *llabel  = cllabel;
        *lunit   = clunit;
        *lformat = clformat;
    } /* end if */
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsgdaln
 * Purpose: Call DFSDgetdatalen to get actual lengths of strings
 * Inputs:  llabel, lunit, lformat, lcoordsys: integers to return lengths in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdatalen
 *---------------------------------------------------------------------------*/

intf
ndsgdaln(intf *llabel, intf *lunit, intf *lformat, intf *lcoordsys)
{
    intf ret;
    int  cllabel, clunit, clformat, clcoordsys;

    ret = (intf)DFSDgetdatalen(&cllabel, &clunit, &clformat, &clcoordsys);
    if (ret != FAIL) {
        *llabel    = cllabel;
        *lunit     = clunit;
        *lformat   = clformat;
        *lcoordsys = clcoordsys;
    } /* end if */
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsfirst * Purpose: Call DFSDrestart to get SDGs again from the beginning
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDrestart
 *---------------------------------------------------------------------------*/

intf
ndsfirst(void)
{

    return DFSDrestart();
}

/*-----------------------------------------------------------------------------
 * Name:    dspslc
 * Purpose: Call DFSDIputslice to write slice to file
 * Inputs:  windims: array of size rank, containing size of slice
 *          data: array containing slice
 *          dims: dimensions of array data
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIputslice
 *---------------------------------------------------------------------------*/

intf
ndspslc(intf windims[], void *data, intf dims[])
{
    int32 *cdims, *cwindims, *p, *wp;
    int    i, rank;
    intf   ret;

    ret = DFSDIgetwrank(&rank);
    wp  = (int32 *)malloc((size_t)rank * sizeof(int32));
    if (wp == NULL)
        return FAIL;
    cwindims = wp;
    p        = (int32 *)malloc((size_t)rank * sizeof(int32));
    if (p == NULL) {
        free(cwindims);
        return FAIL;
    }
    cdims = p;
    for (i = 1; i <= rank; i++) { /* reverse dims & windims */
        *p = dims[rank - i];
        p++;
        *wp = windims[rank - i];
        wp++;
    }

    ret = DFSDIputslice(cwindims, data, cdims, 1);
    free(cdims);
    free(cwindims);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dseslc
 * Purpose: Call DFSDIendslice to finish slice writes and write out SDG
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIendslice
 *---------------------------------------------------------------------------*/

intf
ndseslc(void)
{

    return DFSDIendslice(1);
}

/*-----------------------------------------------------------------------------
 * Name:    dssnt
 * Purpose: Call DFSDsetNT to set number type for subsequent calls to
            DFSDputdata, DFSDadddata, DFSDsetdimscales.
 * Inputs:  numbertype
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Method:  Invokes DFSDsetNT
 * Remarks: 0 specifies default value
 *---------------------------------------------------------------------------*/

intf
ndssnt(intf *numbertype)
{
    return DFSDsetNT(*numbertype);
}

/*----------------------------------------------------------------------------
 * Name:    dsgnt
 * Purpose: Call DFSDgetNT to get number type for subsequent calls
 * Inputs:  pnumbertype
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Method:  Invokes DFSDgetNT
 * Remarks: 0 specifies default value
 *---------------------------------------------------------------------------*/

intf
ndsgnt(intf *pnumbertype)
{
    return DFSDgetNT((int32 *)pnumbertype);
}

/*-----------------------------------------------------------------------------
 * Name:    dsigdim
 * Purpose: Call DFSDgetdims to get dimensions of next SDG
 * Inputs:  filename: name of HDF file
 *          prank: integer to return rank in
 *          sizes: array to return dimensions in
 *          maxrank: dimension of array sizes
 *          lenfn: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   Fortran stub routine
 * Invokes: DFSDgetdims
 *---------------------------------------------------------------------------*/

intf
ndsigdim(_fcd filename, intf *prank, intf sizes[], intf *maxrank, intf *lenfn)
{
    char *fn;
    int32 i, tmp;
    int   isndg;
    intf  ret;

    fn = HDf2cstring(filename, (int)*lenfn);
    if (!fn)
        return -1;
    ret = DFSDgetdims(fn, (int *)prank, (int32 *)sizes, (int)*maxrank);
    DFSDIisndg(&isndg);
    if (isndg) {
        for (i = 0; i < ((int32)*prank) / 2; i++) {
            tmp                          = sizes[i];
            sizes[i]                     = sizes[(int32)*prank - i - 1];
            sizes[(int32)*prank - i - 1] = tmp;
        }
    }
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsigdat
 * Purpose: Call DFSDgetdata to get data values
 * Inputs:  filename: name of HDF file
 *          rank: no of dimensions in array data
 *          maxsizes: array containing dimensions of the array data
 *          data: array to return the data in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIgetdata,DFSDIrefresh,DFSDIisndg
 *---------------------------------------------------------------------------*/

intf
ndsigdat(_fcd filename, intf *rank, intf maxsizes[], void *data, intf *fnlen)
{
    int32  i;
    int    isndg;
    intf   ret;
    char  *fn;
    int32 *p, *cmaxsizes;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;
    /* if DFSDgetdims has not be called call DFSDIsdginfo to */
    /* refresh Readsdg       */
    if (DFSDIrefresh(fn) < 0)
        return FAIL;
    ret = DFSDIisndg(&isndg);
    if (isndg) {
        p = (int32 *)malloc((size_t)(*rank) * sizeof(int32));
        if (p == NULL)
            return FAIL;
        cmaxsizes = p;

        for (i = 1; i <= *rank; i++) {
            *p = maxsizes[*rank - i];
            p++;
        }
        ret = DFSDIgetdata(fn, (int)*rank, cmaxsizes, data, 1);
        free(cmaxsizes);
    }
    else
        ret = DFSDIgetdata(fn, (int)*rank, (int32 *)maxsizes, data, 1); /* 1==FORTRAN */
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsipdat
 * Purpose: Call DFSDIputdata to write SDG to new file
 * Inputs:  filename: name of HDF file
 *          rank: no of dimensions of array data
 *          dimsizes: array containing size of each dimension of array data
 *          data: array containing data values
 *          fnlen: length of string filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIputdata
 *---------------------------------------------------------------------------*/

intf
ndsipdat(_fcd filename, intf *rank, intf dimsizes[], void *data, intf *fnlen)
{
    char *fn;
    int32 i, *cdims, *p;
    intf  ret;

    /* reverse the dimsizes first  */
    p = (int32 *)malloc((size_t)(*rank) * sizeof(int32));
    if (p == NULL)
        return FAIL;
    cdims = p;
    for (i = 1; i <= *rank; i++) {
        *p = dimsizes[*rank - i];
        p++;
    }
    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;

    /* 0, 1 specify create mode, called from FORTRAN program */
    /* In HDF3.2 .hdf files, data and dimsizes are in C order  */
    ret = DFSDIputdata(fn, (int)*rank, cdims, data, 0, 1);
    free(fn);
    free(cdims);

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsiadat
 * Purpose: Call DFSDIputdata to append SDG to existing file
 * Inputs:  filename: name of HDF file
 *          rank: no of dimensions of array data
 *          dimsizes: array containing size of each dimension of array data
 *          data: array containing data values
 *          fnlen: length of string filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIputdata
 *---------------------------------------------------------------------------*/

intf
ndsiadat(_fcd filename, intf *rank, intf dimsizes[], void *data, intf *fnlen)
{
    char *fn;
    int32 i, *cdims, *p;
    intf  ret;

    /* reverse the dimsizes first  */
    p = (int32 *)malloc((size_t)(*rank) * sizeof(int32));
    if (p == NULL)
        return FAIL;
    cdims = p;
    for (i = 1; i <= *rank; i++) {
        *p = dimsizes[*rank - i];
        p++;
    }
    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;

    /* 1, 1 specify create mode, called from FORTRAN program */
    /* In HDF3.2 .hdf files, data and dimsizes are in C order  */
    ret = DFSDIputdata(fn, (int)*rank, cdims, data, 1, 1);
    free(fn);
    free(cdims);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsigslc
 * Purpose: Call DFSDIgetslice to read slice from file
 * Inputs:  filename: name of HDF file
 *          winst: array of size = rank of data, containing start of slice
 *          windims: array of size rank, containing end of slice
 *          data: array for returning slice
 *          ndims: no of dims of array data
 *          dims: dimensions of array data
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIgetslice
 *---------------------------------------------------------------------------*/

intf
ndsigslc(_fcd filename, intf winst[], intf windims[], void *data, intf dims[], intf *fnlen)
{
    char  *fn;
    intf   ret;
    int    rank, i;
    int32 *cdims, *cwindims, *cwinst, *p, *wp, *wsp;
    int    isndg;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;

    /* if DFSDgetdims has not be called call DFSDIsdginfo to */
    /* refresh Readsdg       */
    if (DFSDIrefresh(fn) < 0)
        return FAIL;

    ret = DFSDIisndg(&isndg);
    if (isndg) {
        ret = DFSDIgetrrank(&rank);
        p   = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (p == NULL)
            return FAIL;
        cdims = p;
        wp    = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (wp == NULL)
            return FAIL;
        cwindims = wp;
        wsp      = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (wsp == NULL)
            return FAIL;
        cwinst = wsp;

        for (i = 1; i <= rank; i++) {
            *p = dims[rank - i];
            p++;
            *wp = windims[rank - i];
            wp++;
            *wsp = winst[rank - i];
            wsp++;
        }
        ret = DFSDIgetslice(fn, cwinst, cwindims, data, cdims, 1);
        free(cdims);
        free(cwindims);
        free(cwinst);
    }
    else
        ret = DFSDIgetslice(fn, (int32 *)winst, (int32 *)windims, data, (int32 *)dims, 1);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsisslc
 * Purpose: Call DFSDstartslice to set up to write slice
 * Inputs:  filename: name of HDF file
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDstartslice
 *---------------------------------------------------------------------------*/

intf
ndsisslc(_fcd filename, intf *fnlen)
{
    char *fn;
    intf  ret;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;
    ret = DFSDstartslice(fn);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dsirref
 * Purpose: Call DFSDreadref to set up next ref to read
 * Inputs:  filename: name of HDF file
 *      ref: next ref to read
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDstartslice
 *---------------------------------------------------------------------------*/

intf
ndsirref(_fcd filename, intf *ref, intf *fnlen)
{
    char *fn;
    intf  ret;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;
    ret = DFSDreadref(fn, (uint16)*ref);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dslref
 * Purpose: Return last ref written or read
 * Inputs:  none
 * Globals: Lastref
 * Returns: ref on success, -1 on error with DFerror set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFANlastref
 * Remarks: none
 *---------------------------------------------------------------------------*/

intf
ndslref(void)
{
    return (intf)DFSDlastref();
}

/*-----------------------------------------------------------------------------
 * Name:    dsinum
 * Purpose: Return number of SDGs in the file
 * Inputs:  filename: name of file
 *          len: length of Fortran string filename
 * Returns: number of SDGs on success, -1 on failure with DFerror set
 * Users:   dsnum, dfsdnumber
 * Invokes: DFSDndataset, HDf2cstring
 * Method:  convert string, call DFSDndatasets
 *---------------------------------------------------------------------------*/

intf
ndsinum(_fcd filename, intf *len)
{
    char *cname;
    intf  status;

    cname = HDf2cstring(filename, (int)*len);
    if (!cname)
        return -1;
    status = DFSDndatasets(cname);
    free(cname);

    return status;
}

/*------------------------------------------------------------------------------
 * Name:     dsip32s
 * Purpose:  tests if the SDG with the specified ref was written by HDF prior to
 *            version 3.2
 * Input:    filename: name of HDF file
 *           ref: the ref number of the SDG
 *           ispre32: set to TRUE if the SDG/ref was written by old library;
 *                        to FALSE otherwise.
 *           len:     length of filename
 * Returns:  0 on success, -1 on failure
 * Users:    HDF Fortran programmers
 *------------------------------------------------------------------------------*/

intf
ndsip32s(_fcd filename, intf *ref, intf *ispre32, intf *len)
{
    char *cname;
    intf  status;

    cname = HDf2cstring(filename, (int)*len);
    if (!cname)
        return -1;
    status = DFSDpre32sdg(cname, (uint16)*ref, (int *)ispre32);

    free(cname);
    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetdatastrs
 * Purpose: Call DFSDgetdatastrs to get the data attributes
 * Inputs:  label, unit, format, coordsys: strings to return attributes in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdatastrs
 *---------------------------------------------------------------------------*/

intf
ndfsdgetdatastrs(_fcd label, _fcd unit, _fcd format, _fcd coordsys)
{
    return (DFSDgetdatastrs((char *)_fcdtocp(label), (char *)_fcdtocp(unit), (char *)_fcdtocp(format),
                            (char *)_fcdtocp(coordsys)));
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetdimstrs
 * Purpose: Call DFSDgetdimstrs to get attributes of a dimension
 * Inputs:  label, unit, format: strings to return attributes in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimstrs
 *---------------------------------------------------------------------------*/

intf
ndfsdgetdimstrs(intf *dim, _fcd label, _fcd unit, _fcd format)
{
    int isndg;
    int rank, cdim;

    DFSDIisndg(&isndg);
    if (isndg) {
        DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    return DFSDgetdimstrs(cdim, (char *)_fcdtocp(label), (char *)_fcdtocp(unit), (char *)_fcdtocp(format));
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetdimscale
 * Purpose: Call DFSDgetdimscale to get scale for a dimension
 * Inputs:  dim: dimension to get attributes for
 *          maxsize: size of scale array
 *          scale: array to return scale in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimscale
 *---------------------------------------------------------------------------*/

intf
ndfsdgetdimscale(intf *dim, intf *maxsize, void *scale)
{

    int isndg;
    int rank, cdim;

    DFSDIisndg(&isndg);
    if (isndg) {
        DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    return DFSDgetdimscale(cdim, *maxsize, scale);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetrange
 * Purpose: Call DFSDgetrange to get maximum and minimum data values
 * Inputs:  pmax: float to return maximum in
 *          pmin: float to return minimum in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetrange
 *---------------------------------------------------------------------------*/

intf
ndfsdgetrange(void *pmax, void *pmin)
{
    return DFSDgetrange(pmax, pmin);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsetdims
 * Purpose: Call DFSDsetdims to set dimensions for subsequent SDGs
 * Inputs:  rank: no of dimensions of SDG
 *          dimsizes: array containing dimensions of SDG
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetdims
 *---------------------------------------------------------------------------*/

intf
ndfsdsetdims(intf *rank, intf dimsizes[])
{

    int32 i, *cdims, *p;
    intf  ret;

    p = (int32 *)malloc((size_t)(*rank) * sizeof(int32));
    if (p == NULL)
        return FAIL;
    cdims = p;
    for (i = 1; i <= *rank; i++) {
        *p = dimsizes[*rank - i];
        p++;
    }

    ret = DFSDsetdims((int)*rank, cdims);
    free(cdims);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsetdimscale
 * Purpose: Call DFSDsetdimscale to set scales for subsequent SDGs
 * Inputs:  dim: dimension to set scale for
 *          dimsize: size of array scale
 *          scale: array of scale values
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetdimscale
 *---------------------------------------------------------------------------*/

intf
ndfsdsetdimscale(intf *dim, intf *dimsize, void *scale)
{
    int rank, cdim;

    DFSDIgetwrank(&rank);
    if (rank < *dim)
        return FAIL;
    cdim = rank - (int)*dim + 1;

    return DFSDsetdimscale(cdim, *dimsize, scale);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsetrange
 * Purpose: Call DFSDsetrange to set max and min values for this SDG
 * Inputs:  max, min: max and min data values
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetrange
 * Remarks: Max and Min are set only for next SDG, reset to NULL after
 *---------------------------------------------------------------------------*/

intf
ndfsdsetrange(void *max, void *min)
{
    return DFSDsetrange(max, min);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdclear
 * Purpose: Call DFSDclear to erase values set for subsequent SDGs
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDclear
 *---------------------------------------------------------------------------*/

intf
ndfsdclear(void)
{
    return DFSDclear();
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsetlengths
 * Purpose: Call DFSDsetlengths to set max lengths of strings
 * Inputs:  maxlen_label, maxlen_unit, maxlen_format, maxlen_coordsys: max lens
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetlengths
 *---------------------------------------------------------------------------*/

intf
ndfsdsetlengths(intf *maxlen_label, intf *maxlen_unit, intf *maxlen_format, intf *maxlen_coordsys)
{
    return (
        DFSDsetlengths((int)*maxlen_label, (int)*maxlen_unit, (int)*maxlen_format, (int)*maxlen_coordsys));
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetdimlen
 * Purpose: Call DFSDgetdimlen to get actual lengths of strings
 * Inputs:  dim: dimension to get lengths for
 *         llabel, lunit, lformat: integers to return lengths of each string in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimlen
 *---------------------------------------------------------------------------*/

intf
ndfsdgetdimlen(intf *dim, intf *llabel, intf *lunit, intf *lformat)
{
    int  isndg;
    int  rank, cdim;
    intf ret;
    int  cllabel, clunit, clformat; /* convert between intf and int */

    ret = DFSDIisndg(&isndg);
    if (isndg) {
        ret = DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    ret = (intf)DFSDgetdimlen(cdim, &cllabel, &clunit, &clformat);
    if (ret != FAIL) { /* if ok, copy the values over */
        *llabel  = cllabel;
        *lunit   = clunit;
        *lformat = clformat;
    } /* end if */
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetdatalen
 * Purpose: Call DFSDgetdatalen to get actual lengths of strings
 * Inputs:  llabel, lunit, lformat, lcoordsys: integers to return lengths in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdatalen
 *---------------------------------------------------------------------------*/

intf
ndfsdgetdatalen(intf *llabel, intf *lunit, intf *lformat, intf *lcoordsys)
{
    intf ret;
    int  cllabel, clunit, clformat, clcoordsys;

    ret = (intf)DFSDgetdatalen(&cllabel, &clunit, &clformat, &clcoordsys);
    if (ret != FAIL) {
        *llabel    = cllabel;
        *lunit     = clunit;
        *lformat   = clformat;
        *lcoordsys = clcoordsys;
    } /* end if */
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdrestart
 * Purpose: Call DFSDrestart to get SDGs again from the beginning
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDrestart
 *---------------------------------------------------------------------------*/

intf
ndfsdrestart(void)
{
    return DFSDrestart();
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdputslice
 * Purpose: Call DFSDIputslice to write slice to file
 * Inputs:  winst: array of size = rank of data, containing start of slice
 *          windims: array of size rank, containing end of slice
 *          data: array containing slice
 *          dims: dimensions of array data
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIputslice
 *---------------------------------------------------------------------------*/

intf
ndfsdputslice(intf windims[], void *data, intf dims[])
{
    int    rank, i;
    intf   ret;
    int32 *cdims, *cwindims, *p, *wp;

    ret = DFSDIgetwrank(&rank);
    wp  = (int32 *)malloc((size_t)(rank) * sizeof(int32));
    if (wp == NULL)
        return FAIL;
    cwindims = wp;
    p        = (int32 *)malloc((size_t)(rank) * sizeof(int32));
    if (p == NULL)
        return FAIL;
    cdims = p;
    for (i = 1; i <= rank; i++) { /* reverse dims & windims */
        *p = dims[rank - i];
        p++;
        *wp = windims[rank - i];
        wp++;
    }

    ret = DFSDIputslice(cwindims, data, cdims, 1);
    free(cdims);
    free(cwindims);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdendslice
 * Purpose: Call DFSDendslice to finish slice writes and write out SDG
 * Inputs:  none
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIendslice
 *---------------------------------------------------------------------------*/

intf
ndfsdendslice(void)
{
    return DFSDIendslice(1);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsetnt
 * Purpose: Call DFSDsetNT to set number type for subsequent calls to
 *          DFSDputdata, DFSDadddata, DFSDsetdimscales.
 * Inputs:  numbertype
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Method:  Invokes DFSDsetNT
 * Remarks: 0 specifies default value
 *---------------------------------------------------------------------------*/

intf
ndfsdsetnt(intf *numbertype)
{
    return DFSDsetNT(*numbertype);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdgetnt
 * Purpose: Call DFSDgetNT to get number type for subsequent calls
 * Inputs:  pnumbertype
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Method:  Invokes DFSDgetNT
 * Remarks: 0 specifies default value
 *---------------------------------------------------------------------------*/

intf
ndfsdgetnt(intf *pnumbertype)
{
    return DFSDgetNT((int32 *)pnumbertype);
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdlastref
 * Purpose: Return last ref written or read
 * Inputs:  none
 * Globals: Lastref
 * Returns: ref on success, -1 on error with DFerror set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFANlastref
 * Remarks: none
 *---------------------------------------------------------------------------*/

intf
ndfsdlastref(void)
{
    return (intf)DFSDlastref();
}

/*-----------------------------------------------------------------------------
 * Name:    dsisdis
 * Purpose: For the given dimension, set label, unit, format
 *          This routine needs to be called once for each dimension whose
 *          values the user wants to set.
 * Inputs:  dim: the dimension that this info applies to
 *          label: label to be used to describe this dimension
 *          unit: units for dimension
 *          format: format to be used in displaying
 *          llabel, lunit, lformat: lengths of corresponding strings
 * Globals:
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF users, utilities, other routines
 * Invokes: DFSDIsetdimstr
 * Method:
 *---------------------------------------------------------------------------*/

intf
ndsisdis(intf *dim, _fcd flabel, _fcd funit, _fcd fformat, intf *llabel, intf *lunit, intf *lformat)
{
    char *label  = HDf2cstring(flabel, (int)*llabel);
    char *unit   = HDf2cstring(funit, (int)*lunit);
    char *format = HDf2cstring(fformat, (int)*lformat);
    intf  status;
    int   rank, cdim;

    if (!(label && unit && format)) {
        free(label);
        free(unit);
        free(format);
        return FAIL;
    }
    status = DFSDIgetwrank(&rank);

    if (rank < *dim)
        return FAIL;
    cdim = rank - (int)*dim + 1;

    status = DFSDIsetdimstrs(cdim, label, unit, format);

    free(label);
    free(unit);
    free(format);

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    dsigdis
 * Purpose: Call DFSDgetdimstrs to get attributes of a dimension
 * Inputs:  label, unit, format: strings to return attributes in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdimstrs
 *---------------------------------------------------------------------------*/

intf
ndsigdis(intf *dim, _fcd label, _fcd unit, _fcd format, intf *llabel, intf *lunit, intf *lformat)
{
    char *ilabel, *iunit, *iformat;
    int   rank, cdim;
    int   isndg, status;

    DFSDIisndg(&isndg);
    if (isndg) {
        DFSDIgetrrank(&rank);
        if (rank < *dim)
            return FAIL;
        cdim = rank - (int)*dim + 1;
    }
    else
        cdim = (int)*dim;

    iunit = ilabel = iformat = NULL;

    if (*llabel)
        ilabel = (char *)malloc((size_t)*llabel + 1);
    if (*lunit)
        iunit = (char *)malloc((size_t)*lunit + 1);
    if (*lformat)
        iformat = (char *)malloc((size_t)*lformat + 1);

    status = DFSDgetdimstrs(cdim, ilabel, iunit, iformat);

    HDpackFstring(ilabel, _fcdtocp(label), (int)*llabel);
    HDpackFstring(iunit, _fcdtocp(unit), (int)*lunit);
    HDpackFstring(iformat, _fcdtocp(format), (int)*lformat);

    free(ilabel);
    free(iunit);
    free(iformat);

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    dsisdas
 * Purpose: Set label, unit and format for displaying subsequent SDGs
 * Inputs:  label: label to be used to describe data
 *          unit: unit corresponding to data values
 *          format: format to be used in displaying data values
 *          coordsys: type of coordinate system
 * Globals: Writesdg, Ref
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran stubs
 * Invokes: DFSDIsetdatastrs
 * Method:  Stores values in global structure Writesdg
 * Remarks:
 *---------------------------------------------------------------------------*/
intf
ndsisdas(_fcd flabel, _fcd funit, _fcd fformat, _fcd fcoordsys, intf *isfortran, intf *llabel, intf *lunit,
         intf *lformat, intf *lcoordsys)
{
    char *label    = HDf2cstring(flabel, (int)*llabel);
    char *unit     = HDf2cstring(funit, (int)*lunit);
    char *format   = HDf2cstring(fformat, (int)*lformat);
    char *coordsys = HDf2cstring(fcoordsys, (int)*lcoordsys);
    intf  status;

    if (!(label && unit && format)) {
        free(label);
        free(unit);
        free(format);
        return FAIL;
    }

    (void)isfortran;

    status = DFSDIsetdatastrs(label, unit, format, coordsys);

    free(label);
    free(unit);
    free(format);
    free(coordsys);

    return status;
} /* ndsisdas */

/*-----------------------------------------------------------------------------
 * Name:    dsigdas
 * Purpose: Call DFSDgetdatastrs to get the data attributes
 * Inputs:  label, unit, format, coordsys: strings to return attributes in
 * Returns: 0 on success, -1 on failure with    DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdatastrs
 *---------------------------------------------------------------------------*/

intf
ndsigdas(_fcd label, _fcd unit, _fcd format, _fcd coordsys, intf *llabel, intf *lunit, intf *lformat,
         intf *lcoord)
{
    char *ilabel, *iunit, *iformat, *icoord;
    intf  status;

    iunit = ilabel = iformat = icoord = NULL;

    if (*llabel)
        ilabel = (char *)malloc((uint32)*llabel + 1);
    if (*lunit)
        iunit = (char *)malloc((uint32)*lunit + 1);
    if (*lformat)
        iformat = (char *)malloc((uint32)*lformat + 1);
    if (*lcoord)
        icoord = (char *)malloc((uint32)*lcoord + 1);

    status = DFSDgetdatastrs(ilabel, iunit, iformat, icoord);

    HDpackFstring(ilabel, _fcdtocp(label), (int)*llabel);
    HDpackFstring(iunit, _fcdtocp(unit), (int)*lunit);
    HDpackFstring(iformat, _fcdtocp(format), (int)*lformat);
    HDpackFstring(icoord, _fcdtocp(coordsys), (int)*lcoord);

    free(ilabel);
    free(iunit);
    free(iformat);
    free(icoord);

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    dsscal
 * Purpose: Call DFSDsetcal to set calibration data
 * Inputs:  cal, cal_err   : calibration and error
 *          ioff, ioff_err : offset and error
 *          cal_type       : after calibration NT
 * Returns: 0 on success, -1 on failure
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetdatastrs
 *---------------------------------------------------------------------------*/
intf
ndsscal(float64 *cal, float64 *cal_err, float64 *ioff, float64 *ioff_err, intf *cal_type)
{
    intf    ret;
    float64 dcal      = 0.0;
    float64 dcal_err  = 0.0;
    float64 dioff     = 0.0;
    float64 dioff_err = 0.0;

    memcpy(&dcal, cal, sizeof(float64));
    memcpy(&dcal_err, cal_err, sizeof(float64));
    memcpy(&dioff, ioff, sizeof(float64));
    memcpy(&dioff_err, ioff_err, sizeof(float64));

    ret = (intf)DFSDsetcal((float64)dcal, (float64)dcal_err, (float64)dioff, (float64)dioff_err,
                           (int32)*cal_type);
    return ret;
} /* ndsscal */

/*-----------------------------------------------------------------------------
 * Name:    dsgcal
 * Purpose: Call DFSDgetcal to get calibration data
 * Inputs:  cal, cal_err   : calibration and error
 *          ioff, ioff_err : offset and error
 *          cal_type       : after calibration NT
 * Returns: 0 on success, -1 on failure
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetcal
 *---------------------------------------------------------------------------*/

intf
ndsgcal(float64 *cal, float64 *cal_err, float64 *ioff, float64 *ioff_err, intf *cal_type)
{
    intf    ret;
    float64 dcal;
    float64 dcal_err;
    float64 dioff;
    float64 dioff_err;

    ret = (intf)DFSDgetcal(&dcal, &dcal_err, &dioff, &dioff_err, (int32 *)cal_type);

    memcpy(cal, &dcal, sizeof(float64));
    memcpy(cal_err, &dcal_err, sizeof(float64));
    memcpy(ioff, &dioff, sizeof(float64));
    memcpy(ioff_err, &dioff_err, sizeof(float64));

    return ret;
} /* ndsgcal */

/*-----------------------------------------------------------------------------
 * Name:    dsiwref
 * Purpose: Call DFSDwriteref to set up next ref to write
 * Inputs:  filename: name of HDF file
 *          fnlen: length of filename
 *          ref: next ref to read
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDwriteref
 *---------------------------------------------------------------------------*/

intf
ndsiwref(_fcd filename, intf *fnlen, intf *ref)
{
    char *fn;
    intf  ret;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (!fn)
        return -1;
    ret = DFSDwriteref(fn, (uint16)*ref);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dfsdsfill
 * Purpose: Call DFSDsetfillvalue to set fill value for SDS
 * Inputs:  fill_value: fill value for SDS
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDsetfillvalue
 *---------------------------------------------------------------------------*/

intf
ndssfill(void *fill_value)
{
    return DFSDsetfillvalue(fill_value);
}

/*-----------------------------------------------------------------------------
 * Name:    dsgfill
 * Purpose: Call DFSDgetfillvalue to get fill value for SDS
 * Inputs:  fill_value: fill value of SDS
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDgetfillvalue
 *---------------------------------------------------------------------------*/

intf
ndsgfill(void *fill_value)
{
    return DFSDgetfillvalue(fill_value);
}

/*-----------------------------------------------------------------------------
 * Name:    dsisslab
 * Purpose: Set up slab writes to SDS
 * Inputs:  filename: file to which this applies
 *          fnlen: file name length
 * Returns: 0 on success, FAIL on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFSDstartslab
 * Remarks:
 *---------------------------------------------------------------------------*/

intf
ndsisslab(_fcd filename, intf *fnlen)
{
    char *fn;
    intf  ret;

    fn = HDf2cstring(filename, (int)*fnlen);
    if (fn == NULL)
        return FAIL;
    ret = DFSDstartslab(fn);
    free(fn);
    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dswslab
 * Purpose: Call DFSDwriteslab to write slab to file
 * Inputs:  start: array of size = rank of data, containing start of slab
 *          stride: array for subsampling
 *          count: array of size rank, containing size of slab
 *          data: array of data to be written
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDIgetwrank, malloc, free, HDf2cstring, DFSDwriteslab
 *---------------------------------------------------------------------------*/

intf
ndswslab(intf start[], intf stride[], intf count[], void *data)
{
    int32 *lstart, *lstride, *lcount, *aptr, *bptr, *cptr;
    int    i, rank;
    intf   ret;

    /*
     ** Lets reverse the order for the arrays since we
     ** are going from fortran to C
     */
    ret = DFSDIgetwrank(&rank);
    if (ret == FAIL)
        return FAIL;

    aptr = (int32 *)malloc((size_t)(3 * rank) * sizeof(int32));
    if (aptr == NULL)
        return FAIL;

    lstart  = aptr;
    lstride = bptr = aptr + rank;
    lcount = cptr = bptr + rank;

    for (i = 1; i <= rank; i++) { /* reverse start, stride & count */
        *aptr = start[rank - i];
        aptr++;
        *bptr = stride[rank - i];
        bptr++;
        *cptr = count[rank - i];
        cptr++;
    }

    ret = DFSDwriteslab(lstart, lstride, lcount, data);
    free(lstart);

    return ret;
}

/*-----------------------------------------------------------------------------
 * Name:    dseslab
 * Purpose: End slab writes to SDS, Write out NDG
 * Inputs:  None
 * Returns: 0 on success, FAIL on failure
 * Users:   HDF programmers, other routines and utilities
 * Invokes: DFSDendslab
 * Remarks:
 *---------------------------------------------------------------------------*/

intf
ndseslab(void)
{
    return DFSDendslab();
}

/*-----------------------------------------------------------------------------
 * Name:    dsirslab
 * Purpose: Call DFSDreadslab to read slab from SDS
 * Inputs:  filename: name of HDF file
 *          start: array of size = rank of data, containing start of slab
 *          slab_size: array of size rank, containing end of slab
 *          stride: sub sampling stride.
 *          buffer: array for returning slab
 *          buffer_size: dimensions of array data
 *          fnlen: length of filename
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFSDreadslab
 *---------------------------------------------------------------------------*/

intf
ndsirslab(_fcd filename, intf *fnlen, intf start[], intf slab_size[], intf stride[], void *buffer,
          intf buffer_size[])
{
    char  *fn;
    intf   ret;
    int    rank, i;
    int32 *lbuffer_size, *lslab_size, *lstart, *lstride;
    int32 *p, *wp, *wsp, *sp;
    int    isndg;

    /* Convert "filename" to fortran string */
    fn = HDf2cstring(filename, (int)*fnlen);
    if (fn == NULL)
        return FAIL;

    /* If DFSDgetdims has not be called call DFSDIsdginfo to refresh Readsdg */
    if (DFSDIrefresh(fn) < 0)
        return FAIL;

    ret = DFSDIisndg(&isndg);
    if (isndg) {
        ret = DFSDIgetrrank(&rank);
        p   = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (p == NULL)
            return FAIL;
        lbuffer_size = p;
        wp           = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (wp == NULL)
            return FAIL;
        lslab_size = wp;
        wsp        = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (wsp == NULL)
            return FAIL;
        lstart = wsp;
        sp     = (int32 *)malloc((size_t)rank * sizeof(int32));
        if (sp == NULL)
            return FAIL;
        lstride = sp;

        for (i = 1; i <= rank; i++) {
            *p = buffer_size[rank - i];
            p++;
            *wp = slab_size[rank - i];
            wp++;
            *wsp = start[rank - i];
            wsp++;
            *sp = stride[rank - i];
            sp++;
        }
        ret = DFSDreadslab(fn, lstart, lslab_size, lstride, buffer, lbuffer_size);
        free(lstart);
        free(lslab_size);
        free(lbuffer_size);
        free(lstride);
    }
    else
        ret = DFSDreadslab(fn, (int32 *)start, (int32 *)slab_size, (int32 *)stride, buffer,
                           (int32 *)buffer_size);
    free(fn);
    return ret;
}
