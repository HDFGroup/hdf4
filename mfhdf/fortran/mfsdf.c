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

  This file contains the HDF-style C stubs for the multi-file SDS
  interface.

  The basic routines called by fortran will be of the form sfxxxx

  If only a C stub is needed it will be named nsfxxxx have the FNAME()
  function applied to it.

  If a Fortran stub is also required the fortran stub will be called
  sfxxxx and the one in here will be nscxxx and again be FNAME()ed

*/

#include "mfhdf.h"

/*-----------------------------------------------------------------------------
 * Name:    scstart
 * Purpose: call SDstart to open HDF file
 * Inputs:  name: name of file to open
 *          access: access mode - integer with value DFACC_READ etc. 
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscstart(_fcd name, intf *access, intf *namelen)
#else
nscstart(name, access, namelen)
    _fcd name;
    intf *access;
    intf *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    intf ret;
    
    fn = HDf2cstring(name, *namelen);
    
    ret = (intf) SDstart(fn, *access);
    HDfreespace((VOIDP)fn);

    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sdfend
 * Purpose: Call SDend close to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfend(intf *file_id)
#else
nsfend(file_id)
    intf *file_id;
#endif /* PROTOTYPE */
{
    return(SDend(*file_id));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfendacc
 * Purpose: Call SDendaccess close to end access on a dataset 
 * Inputs:  id: id of the data set
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfendacc(intf *id)
#else
nsfendacc(id)
    intf *id;
#endif /* PROTOTYPE */
{
    return(SDendaccess(*id));
}
/*-----------------------------------------------------------------------------
 * Name:    sdffinfo
 * Purpose: Call SDfileinfo to get number of datasets and global attrs in the file
 * Inputs:  file_id: handle to HDF file
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsffinfo(intf *file_id, intf *datasets, intf *gattr)
#else
nsffinfo(file_id, datasets, gattr)
     intf *file_id;
     intf *datasets;
     intf *gattr;
#endif /* PROTOTYPE */
{
  int32 dset, nattr, status;

  status = SDfileinfo((int32) *file_id, &dset, &nattr);

  *datasets = (intf) dset;
  *gattr    = (intf) nattr;

  return (status);
}

/*-----------------------------------------------------------------------------
 * Name:    sfselect
 * Purpose: Call SDselect to return a handle to a data set in the given file
 * Inputs:  file_id: handle to HDF file
 *          index:   number of data set to chose
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfselect(intf *file_id, intf *index)
#else
nsfselect(file_id, index)
     intf *file_id;
     intf *index;
#endif /* PROTOTYPE */
{
    return(SDselect(*file_id, *index));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfdimid
 * Purpose: Get an id for a dimension of a given data set
 * Inputs:  id: handle to a data set
 *          index: number of dimension to chose
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfdimid(intf *id, intf *index)
#else
nsfdimid(id, index)
     intf *id;
     intf *index;
#endif /* PROTOTYPE */
{
    int32 rank, nt, dims[100], status, cdim, nattrs;

    status = SDgetinfo(*id, NULL, &rank, dims, &nt, &nattrs);
    if(status == FAIL) return FAIL;
    cdim = rank - (*index) -1;

    return(SDgetdimid(*id, cdim));
}

/*-----------------------------------------------------------------------------
 * Name:    sdcginfo
 * Purpose: Call SDgetinfo to get the basic information about a data set
 * Inputs:  id: handle to a data set
 * Outputs: name: the name of the data set
 *          rank: the rank
 *          dimsizes: sizes of the dimensions
 *          nt: number type
 *          nattr: number of attributes for this data set
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscginfo(intf *id, char *name, intf *rank, intf *dimsizes, intf *nt, intf *nattr, intf *len)
#else
nscginfo(id, name, rank, dimsizes, nt, nattr, len)
     intf *id;
     char *name;
     intf *rank, *dimsizes, *nt, *nattr, *len;
#endif /* PROTOTYPE */
{
    char *iname;
    int32 status;
    int32 cdims[100], i;
    int32 rank32, nt32, nattr32;

    iname = NULL;
    if(*len)   iname  = (char *) HDgetspace((uint32)*len + 1);

    status = SDgetinfo((int32) *id, iname, &rank32, cdims, &nt32, &nattr32);

    for(i = 0; i < rank32; i++)
        dimsizes[i] = cdims[rank32 - i - 1];

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfreespace((VOIDP)iname);
  
    *rank  = (intf) rank32;
    *nt    = (intf) nt32;
    *nattr = (intf) nattr32;

    return(status);

}

/*-----------------------------------------------------------------------------
 * Name:    sdfgcal
 * Purpose: Call SDgetcal to get the calibration information
 * Inputs:  id: handle to a data set
 * Outputs: cal, cale, ioff, ioffe : calibration factors and error
 *          nt: number type of data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfgcal(intf *id, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, intf *nt)
#else
nsfgcal(id, cal, cale, ioff, ioffe, nt)
     intf *id;
     float64 *cal, *cale, *ioff, *ioffe;
     intf *nt;
#endif /* PROTOTYPE */
{
  int32 nt32, status;
  
  status = SDgetcal((int32) *id, cal, cale, ioff, ioffe, &nt32);
  
  *nt = (intf) nt32;

  return (status);
}

/*-----------------------------------------------------------------------------
 * Name:    sdfscal
 * Purpose: Call SDsetcal to set the calibration information
 * Inputs:  id: handle to a data set
 *          cal, cale, ioff, ioffe : calibration factors and error
 *          nt: number type of data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfscal(intf *id, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, intf *nt)
#else
nsfscal(id, cal, cale, ioff, ioffe, nt)
     intf *id;
     float64 *cal, *cale, *ioff, *ioffe;
     intf *nt;
#endif /* PROTOTYPE */
{
    return(SDsetcal(*id, *cal, *cale, *ioff, *ioffe, *nt));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfsdscale
 * Purpose: Call SDsetdimscale to set the values for a dimension
 * Inputs:  id: handle to a dimension
 *          count: size of the dimension
 *          nt: number type of data
 *          values: data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfsdscale(intf *id, intf *count, intf *nt, VOIDP values)
#else
nsfsdscale(id, count, nt, values)
     intf *id;
     intf *count, *nt;
     VOIDP values;
#endif /* PROTOTYPE */
{
    return(SDsetdimscale(*id, *count, *nt, values));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfgdscale
 * Purpose: Call SDgetdimscale to get the values for a dimension
 * Inputs:  id: handle to a dimension
 * Output   values: data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfgdscale(intf *id, VOIDP values)
#else
nsfgdscale(id, values)
     intf *id;
     VOIDP values;
#endif /* PROTOTYPE */
{
    return(SDgetdimscale(*id, values));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfsfill
 * Purpose: Call SDsetfillvalue to set the fill value
 * Inputs:  id: handle to a dimension
 *          val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfsfill(intf *id, VOIDP val)
#else
nsfsfill(id, val)
     intf *id;
     VOIDP val;
#endif /* PROTOTYPE */
{
    return(SDsetfillvalue(*id, val));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfgfill
 * Purpose: Call SDgetfillvalue to get the fill value
 * Inputs:  id: handle to a dimension
 * Output:  val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfgfill(intf *id, VOIDP val)
#else
nsfgfill(id, val)
     intf *id;
     VOIDP val;
#endif /* PROTOTYPE */
{
    return(SDgetfillvalue(*id, val));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfgrange
 * Purpose: Call SDgetrange to get the valid range info
 * Inputs:  id: handle to a dimension
 * Output:  min: the min
 *          max: the max
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfgrange(intf *id, VOIDP max, VOIDP min)
#else
nsfgrange(id, max, min)
     intf *id;
     VOIDP max;
     VOIDP min;
#endif /* PROTOTYPE */
{
    return(SDgetrange(*id, max, min));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfsrange
 * Purpose: Call SDsetrange to set the valid range info
 * Inputs:  id: handle to a dimension
 *          min: the min
 *          max: the max
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfsrange(intf *id, VOIDP max, VOIDP min)
#else
nsfsrange(id, max, min)
     intf *id;
     VOIDP max;
     VOIDP min;
#endif /* PROTOTYPE */
{
    return(SDsetrange(*id, max, min));
}

/*-----------------------------------------------------------------------------
 * Name:    sdcn2index
 * Purpose: return the index of a data set with the given name
 * Inputs:  id: file id
 *          name: name to look for
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscn2index(intf *id, _fcd name, intf *namelen)
#else
nscn2index(id, name, namelen)
     intf *id;
     _fcd name;
     intf *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    intf ret;
    
    fn = HDf2cstring(name, *namelen);
    ret = (intf) SDnametoindex(*id, fn);
    HDfreespace((VOIDP)fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sdccreate
 * Purpose: create a new data set and return its id
 * Inputs:  id: file id
 *          name: name of data set
 *          nt: number type
 *          rank: rank
 *          dims: dimension sizes
 *          namelen: length of name
 * Remarks: need to flip the dimensions to account for array ordering
 *          differences
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsccreate(intf *id, _fcd name, intf *nt, intf *rank, intf *dims, intf *namelen)
#else
nsccreate(id, name, nt, rank, dims, namelen)
     intf *id;
     _fcd name;
     intf *nt, *rank;
     intf *dims;
     intf *namelen;
#endif /* PROTOTYPE */
{
    char   *fn;
    intf    ret;
    int32 * cdims, i;
    
    fn = HDf2cstring(name, *namelen);
    cdims = (int32 *) HDgetspace(sizeof(int32) * (*rank));
    if(!cdims) return FAIL;

    for(i = 0; i < *rank; i++)
        cdims[i] = dims[*rank - i - 1];

    ret = (intf) SDcreate(*id, fn, *nt, *rank, cdims);

    HDfreespace((VOIDP)fn);
    HDfreespace((VOIDP)cdims);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scsdimstr
 * Purpose: set the strings for this dimension
 * Inputs:  id: dim id
 *          label, unit and format strings and their lengths
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsdimstr(intf *id, _fcd l, _fcd u, _fcd f, intf *ll, intf *ul, intf *fl)
#else
nscsdimstr(id, l, u, f, ll, ul, fl)
     intf *id;
     _fcd l;
     _fcd u;
     _fcd f;
     intf *ll, *ul, *fl;
#endif /* PROTOTYPE */
{
    char *lstr;
    char *ustr;
    char *fstr;
    intf ret;
    
    if(ll) 
        lstr = HDf2cstring(l, *ll);
    else
        lstr = NULL;

    if(ul) 
        ustr = HDf2cstring(u, *ul);
    else
        ustr = NULL;

    if(fl) 
        fstr = HDf2cstring(f, *fl);
    else
        fstr = NULL;

    ret = (intf) SDsetdimstrs(*id, lstr, ustr, fstr);
    if(ll) HDfreespace((VOIDP)lstr);
    if(ul) HDfreespace((VOIDP)ustr);
    if(fl) HDfreespace((VOIDP)fstr);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scsdimname
 * Purpose: set the name of this dimension
 * Inputs:  id: dim id
 *          name and its length
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsdimname(intf *id, _fcd name, intf *len)
#else
nscsdimname(id, name, len)
     intf *id;
     _fcd name;
     intf *len;
#endif /* PROTOTYPE */
{
    char *nstr;
    intf ret;
    
    if(len) 
        nstr = HDf2cstring(name, *len);
    else
        nstr = NULL;

    ret = (intf) SDsetdimname(*id, nstr);
    if(len) HDfreespace((VOIDP)nstr);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scsdatstr
 * Purpose: set the strings for this dimension
 * Inputs:  id: dim id
 *          label, unit and format strings and their lengths
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsdatstr(intf *id, _fcd l, _fcd u, _fcd f, _fcd c, intf *ll, intf *ul, intf *fl, intf *cl)
#else
nscsdatstr(id, l, u, f, c, ll, ul, fl, cl)
     intf *id;
     _fcd l;
     _fcd u;
     _fcd f;
     _fcd c;
     intf *ll, *ul, *fl, *cl;
#endif /* PROTOTYPE */
{
    char *lstr;
    char *ustr;
    char *fstr;
    char *cstr;
    intf ret;
    
    if(ll) 
        lstr = HDf2cstring(l, *ll);
    else
        lstr = NULL;

    if(ul) 
        ustr = HDf2cstring(u, *ul);
    else
        ustr = NULL;

    if(fl) 
        fstr = HDf2cstring(f, *fl);
    else
        fstr = NULL;

    if(cl) 
        cstr = HDf2cstring(c, *cl);
    else
        cstr = NULL;

    ret = (intf) SDsetdatastrs(*id, lstr, ustr, fstr, cstr);
    if(ll) HDfreespace((VOIDP)lstr);
    if(ul) HDfreespace((VOIDP)ustr);
    if(fl) HDfreespace((VOIDP)fstr);
    if(cl) HDfreespace((VOIDP)cstr);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sfrattr
 * Purpose: Call SDreadattr to get the contents of an attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfrattr(intf *id, intf *index, VOIDP buf)
#else
nsfrattr(id, index, buf)
     intf *id;
     intf *index;
     VOIDP buf;
#endif /* PROTOTYPE */
{
    return(SDreadattr(*id, *index, buf));
}


/*-----------------------------------------------------------------------------
 * Name:    sfrdata
 * Purpose: read a section of data
 * Inputs:  id: dataset id
 *          start: start location
 *          stride: stride along each dimension
 *          end: number of values along each dim to read
 *          values: data 
 * Remarks: need to flip the dimensions to account for array ordering
 *          differences
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfrdata(intf *id, intf *start, intf *stride, intf *end, VOIDP values)
#else
nsfrdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     VOIDP values;
#endif /* PROTOTYPE */
{
    char   *fn;
    intf    ret;
    int32   i, rank, dims[100], nt, nattrs, status;
    int32   cstart[100], cstride[100], cend[100];
    int     nostride;
    
    status = SDgetinfo(*id, NULL, &rank, dims, &nt, &nattrs);
    if(status == FAIL) return FAIL;

    nostride = TRUE;

    for(i = 0; i < rank; i++) {
        cstart[i] = start[rank - i - 1];
        cend[i]   = end[rank - i - 1];
        if((cstride[i] = stride[rank - i - 1]) != 0) nostride = FALSE;
    }
    
#ifdef CM5
    ret = (intf) CMreaddata(*id, cstart, (nostride? NULL : cstride), cend, values);
#else
    if(nostride)
        ret = (intf) SDreaddata(*id, cstart, NULL, cend, values);
    else
        ret = (intf) SDreaddata(*id, cstart, cstride, cend, values);
#endif
  
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sfwdata
 * Purpose: write a section of data
 * Inputs:  id: dataset id
 *          start: start location
 *          stride: stride along each dimension
 *          end: number of values along each dim to write
 *          values: data 
 * Remarks: need to flip the dimensions to account for array ordering
 *          differences
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfwdata(intf *id, intf *start, intf *stride, intf *end, VOIDP values)
#else
nsfwdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     VOIDP values;
#endif /* PROTOTYPE */
{
    char   *fn;
    intf    ret;
    int32   i, rank, dims[100], nt, nattrs, status;
    int32   cstart[100], cstride[100], cend[100];
    intn    nostride;
    
    status = SDgetinfo(*id, NULL, &rank, dims, &nt, &nattrs);
    if(status == FAIL) return FAIL;

    nostride = TRUE;

    for(i = 0; i < rank; i++) {
        cstart[i] = start[rank - i - 1];
        cend[i]   = end[rank - i - 1];
        if((cstride[i] = stride[rank - i - 1]) != 0) nostride = FALSE;
    }
    
#ifdef CM5
    ret = (intf) CMwritedata(*id, cstart, (nostride? NULL : cstride), cend, values);
#else
    if(nostride)
        ret = (intf) SDwritedata(*id, cstart, NULL, cend, values);
    else
        ret = (intf) SDwritedata(*id, cstart, cstride, cend, values);
#endif
        
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scgdmstrs
 * Purpose: Return the "dimension strings"
 * Inputs:  label, unit, format: strings to return attributes in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgdimstrs(intf *dim, _fcd label, _fcd unit, _fcd format, intf *llabel,
         intf *lunit, intf *lformat, intf *mlen)
#else
nscgdimstrs(dim, label, unit, format, llabel, lunit, lformat, mlen)
     intf *dim;
     _fcd label, unit, format;
     intf *llabel, *lunit, *lformat;
     int *mlen;
#endif /* PROTOTYPE */
{
    char *ilabel, *iunit, *iformat;
    intn rank, cdim;
    intf ret;
    intn isndg, status;

    iunit = ilabel = iformat = NULL;

    if(*llabel)  ilabel  = (char *) HDgetspace((uint32)*llabel + 1);
    if(*lunit)   iunit   = (char *) HDgetspace((uint32)*lunit + 1);
    if(*lformat) iformat = (char *) HDgetspace((uint32)*lformat + 1);

    status = SDgetdimstrs(*dim, ilabel, iunit, iformat, *mlen);

    HDpackFstring(ilabel,  _fcdtocp(label),  *llabel);
    HDpackFstring(iunit,   _fcdtocp(unit),   *lunit);
    HDpackFstring(iformat, _fcdtocp(format), *lformat);

    if(ilabel)  HDfreespace((VOIDP)ilabel);
    if(iunit)   HDfreespace((VOIDP)iunit);
    if(iformat) HDfreespace((VOIDP)iformat);

    return status;
}

/*-----------------------------------------------------------------------------
 * Name:    scgdtstrs
 * Purpose: Return the "data strings"
 * Inputs:  label, unit, format coord: strings to return attributes in
 * Returns: 0 on success, -1 on failure with DFerror set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgdatstrs(intf *id, _fcd label, _fcd unit, _fcd format, _fcd coord, 
           intf *llabel, intf *lunit, intf *lformat, intf *lcoord, intf *len)
#else
nscgdatstrs(id, label, unit, format, coord, llabel, lunit, lformat, lcoord, len)
     intf *id;
     _fcd label, unit, format, coord;
     intf *llabel, *lunit, *lformat, *lcoord;
     intf *len;
#endif /* PROTOTYPE */
{
    char *ilabel, *iunit, *iformat, *icoord;
    intn rank, cdim;
    intf ret;
    intn isndg, status;

    iunit = ilabel = iformat = NULL;

    if(*llabel)  ilabel  = (char *) HDgetspace((uint32)*llabel + 1);
    if(*lunit)   iunit   = (char *) HDgetspace((uint32)*lunit + 1);
    if(*lformat) iformat = (char *) HDgetspace((uint32)*lformat + 1);
    if(*lcoord)  icoord  = (char *) HDgetspace((uint32)*lcoord + 1);

    status = SDgetdatastrs(*id, ilabel, iunit, iformat, icoord, *len);

    HDpackFstring(ilabel,  _fcdtocp(label),  *llabel);
    HDpackFstring(iunit,   _fcdtocp(unit),   *lunit);
    HDpackFstring(iformat, _fcdtocp(format), *lformat);
    HDpackFstring(icoord,  _fcdtocp(coord),  *lcoord);

    if(ilabel)  HDfreespace((VOIDP)ilabel);
    if(iunit)   HDfreespace((VOIDP)iunit);
    if(iformat) HDfreespace((VOIDP)iformat);
    if(icoord)  HDfreespace((VOIDP)icoord);

    return status;
}


/*-----------------------------------------------------------------------------
 * Name:    sfgainfo
 * Purpose: Call SDattrinfo to get the basic information about an attribute
 * Inputs:  id: handle to a data set
 * Outputs: name: the name of the data set
 *          count: the size
 *          nt: number type
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgainfo(intf *id, intf *number, char *name, intf *nt, intf *count, intf *len)
#else
nscgainfo(id, number, name, nt, count, len)
     intf *id, *number;
     char *name;
     intf *count, *len, *nt;
#endif /* PROTOTYPE */
{
    char  * iname;
    intn    status;
    int32   nt32;
    int32   cnt32;

    iname = NULL;
    if(*len)   iname  = (char *) HDgetspace((uint32)*len + 1);

    status = SDattrinfo(*id, *number, iname, &nt32, &cnt32);

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfreespace((VOIDP)iname);

    *nt    = (intf) nt32;
    *count = (intf) cnt32;
  
    return((intf)status);

}

/*-----------------------------------------------------------------------------
 * Name:    scgdinfo
 * Purpose: Call SDdiminfo to get the basic information about a dimension
 * Inputs:  id: handle to a dimension
 * Outputs: name: the name of the dimension
 *          count: the size
 *          nt: number type
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgdinfo(intf *id, char *name, intf *sz, intf *nt, intf *nattr, intf *len)
#else
nscgdinfo(id, name, sz, nt, nattr, len)
     intf *id;
     char *name;
     intf *sz, *nattr, *len, *nt;
#endif /* PROTOTYPE */
{
    char *iname;
    int32 status;
    int32 sz32, nt32, nattr32;

    iname = NULL;
    if(*len)   iname  = (char *) HDgetspace((uint32)*len + 1);

    status = SDdiminfo(*id, iname, &sz32, &nt32, &nattr32);

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfreespace((VOIDP)iname);
  
    *nt    = (intf) nt32;
    *sz    = (intf) sz32;
    *nattr = (intf) nattr;

    return(status);

}

/*-----------------------------------------------------------------------------
 * Name:    scsattr
 * Purpose: create a new attribute (or modify an existing one)
 * Inputs:  id: id (file or data set)
 *          name: name of attribute
 *          nt: number type
 *          rank: rank
 *          dims: dimension sizes
 *          namelen: length of name
 * Remarks: need to flip the dimensions to account for array ordering
 *          differences
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsattr(intf *id, _fcd name, intf *nt, intf *count, VOIDP data, intf *len)
#else
nscsattr(id, name, nt, count, data, len)
     intf *id;
     _fcd name;
     intf *nt, *count;
     VOIDP data;
     intf *len;
#endif /* PROTOTYPE */
{
    char   *an;
    intf    ret;
    
    an = HDf2cstring(name, *len);

    ret = (intf) SDsetattr(*id, an, *nt, *count, data);

    HDfreespace((VOIDP)an);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scfattr
 * Purpose: call SDfindattr to find an attribute
 * Inputs:  id: object to look at
 *          name: name of attribute to find
 * Returns: attribute id on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscfattr(intf *id, _fcd name, intf *namelen)
#else
nscfattr(id, name, namelen)
    _fcd name;
    intf *id;
    intf *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    intf ret;
    
    fn = HDf2cstring(name, *namelen);
    
    ret = (intf) SDfindattr(*id, fn);
    HDfreespace((VOIDP)fn);

    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sfid2ref
 * Purpose: call SDidtoref to map an idfindattr to find an attribute
 * Inputs:  id: variable id
 * Returns: reference number of a NDG representing this dataset
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfid2ref(intf *id)
#else
nsfid2ref(id)
    intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDidtoref(*id));
}


/*-----------------------------------------------------------------------------
 * Name:    sfref2index
 * Purpose: call SDreftoindex to map a reference number to an index of a 
 *          dataset
 * Inputs:  id: file id
 *          ref: reference number to look up
 * Returns: reference number of a NDG representing this dataset
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfref2index(intf *id, intf *ref)
#else
nsfref2index(id, ref)
     intf *id;
     intf *ref;
#endif /* PROTOTYPE */
{
    return((intf) SDreftoindex(*id, (int32) *ref));
}


/*-----------------------------------------------------------------------------
 * Name:    sfiscvar
 * Purpose: call SDiscoordvar to see if a dataset is a coordinate variable
 *          dataset
 * Inputs:  id: sds id
 * Returns: TRUE/FALSE
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfiscvar(intf *id)
#else
nsfiscvarindex(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDiscoordvar(*id));
}

/*-----------------------------------------------------------------------------
 * Name:    sdcsextf
 * Purpose: store data in an external file
 * Inputs:  id: sds id
 *          name: name of external file
 *          offset: Number of bytes from the beginning of the
 *                    external file to where the data starts
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsextf(intf *id, _fcd name, intf *offset, intf *namelen)
#else
nscsextf(id, name, offset, namelen)
     intf *id;
     _fcd name;
     intf *offset;
     intf *namelen;
#endif /* PROTOTYPE */
{
    char   *fn;
    intf    ret;
    
    fn = HDf2cstring(name, *namelen);
    ret = (intf) SDsetexternalfile(*id, fn, *offset);
    HDfreespace((VOIDP)fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sfsnbit
 * Purpose: store data in n-bit data element
 * Inputs:  id: sds id
 *          start_bit: starting bit offset
 *          bit_len: # of bits to write
 *          sign_ext: whether to use the top bit as a sign extender
 *          fill_one: whether to fill the "background bits" with ones
 * Returns: 0 on success, -1 on failure with error set
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfsnbit(intf *id, intf *start_bit, intf *bit_len, intf *sign_ext, intf *fill_one)
#else
nsfsnbit(id, start_bit, bit_len, sign_ext, fill_one)
     intf *id;
     intf *start_bit;
     intf *bit_len;
     intf *sign_ext;
     intf *fill_one;
#endif /* PROTOTYPE */
{
    return((intf)SDsetnbitdataset((int32)*id,(intn)*start_bit,(intn)*bit_len,
	(intn)*sign_ext,(intn)*fill_one));
}

/*-----------------------------------------------------------------------------
 * Name:    sdfsacct
 * Purpose: Call SDsetaccesstype to set the access type
 * Inputs:  id: sds id
 *          type: the access type
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfsacct(intf *id, intf *type)
#else
nsfsacct(id, type)
     intf *id;
     intf *type;
#endif /* PROTOTYPE */
{
    return((intf) SDsetaccesstype(*id, *type));
}
