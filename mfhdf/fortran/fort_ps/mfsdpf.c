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
  mfsdpf.c -- based on mfsdf.c,v 1.26
  This file contains the HDF-style C stubs for the multi-file SDS
  interface for Fortran PowerStation.

  The basic routines called by fortran will be of the form sfxxxx
  in mfsdpff.f. 
  All c stubs in this file are named nscxxxx having the FNAME()
  function applied to it.
*/

#ifdef HDF
#include "mfhdf.h"

#ifdef PROTOTYPE
FRETVAL(intf) nscstart(_fcd name, intf *access, intf *namelen);
FRETVAL(intf) nscend(intf *file_id);
FRETVAL(intf) nscendacc(intf *id);
FRETVAL(intf) nscfinfo(intf *file_id, intf *datasets, intf *gattr);
FRETVAL(intf) nscselct(intf *file_id, intf *index);
FRETVAL(intf) nscdimid(intf *id, intf *index);
FRETVAL(intf) nscginfo(intf *id, _fcd name, intf *rank, 
                intf *dimsizes, intf *nt, intf *nattr, intf *len);
FRETVAL(intf) nscgcal(intf *id, float64 *cal, float64 *cale, 
                float64 *ioff, float64 *ioffe, intf *nt);
FRETVAL(intf) nscscal(intf *id, float64 *cal, float64 *cale,
                float64 *ioff, float64 *ioffe, intf *nt);
FRETVAL(intf) nscsdscale(intf *id, intf *count, intf *nt, VOIDP values);
FRETVAL(intf) nscgdscale(intf *id, VOIDP values);
FRETVAL(intf) nscscfill(intf *id, _fcd val);
FRETVAL(intf) nscgcfill(intf *id, _fcd val);
FRETVAL(intf) nscsfill(intf *id, VOIDP val);
FRETVAL(intf) nscgfill(intf *id, VOIDP val);
FRETVAL(intf) nscgrange(intf *id, VOIDP max, VOIDP min);
FRETVAL(intf) nscsrange(intf *id, VOIDP max, VOIDP min);
FRETVAL(intf) nscn2index(intf *id, _fcd name, intf *namelen);
FRETVAL(intf) nsccreate(intf *id, _fcd name, intf *nt, intf *rank, 
                        intf *dims, intf *namelen);
FRETVAL(intf) nscsdimstr(intf *id, _fcd l, _fcd u, _fcd f, intf *ll, 
                        intf *ul, intf *fl);
FRETVAL(intf) nscsdimname(intf *id, _fcd name, intf *len);
FRETVAL(intf) nscsdatstr(intf *id, _fcd l, _fcd u, _fcd f, _fcd c, 
                        intf *ll, intf *ul, intf *fl, intf *cl);
FRETVAL(intf) nscrcatt(intf *id, intf *index, _fcd buf);
FRETVAL(intf) nscrnatt(intf *id, intf *index, VOIDP buf);
FRETVAL(intf) nscrattr(intf *id, intf *index, VOIDP buf);
FRETVAL(intf) nscrdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
FRETVAL(intf) nscwdata(intf *id, intf *start, intf *stride, 
                       intf *end, VOIDP values);
FRETVAL(intf) nscgdimstrs(intf *dim, _fcd label, _fcd unit, 
                      _fcd format,intf *llabel,intf *lunit, 
                      intf *lformat, intf *mlen);
FRETVAL(intf) nscrcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
FRETVAL(intf) nscwcdata(intf *id, intf *start, intf *stride, 
                       intf *end, _fcd values);
FRETVAL(intf) nscgdatstrs(intf *id, _fcd label, _fcd unit,
                 _fcd format, _fcd coord, intf *llabel, intf *lunit,
                   intf *lformat, intf *lcoord, intf *len);
FRETVAL(intf) nscgainfo(intf *id, intf *number, _fcd name, intf *nt, 
                   intf *count, intf *len);
FRETVAL(intf) nscgdinfo(intf *id, _fcd name, intf *sz, intf *nt, 
                   intf *nattr, intf *len);
FRETVAL(intf) nscscatt(intf *id, _fcd name, intf *nt, intf *count,
                    _fcd data, intf *len);
FRETVAL(intf) nscsnatt(intf *id, _fcd name, intf *nt, intf *count, VOIDP data, intf *len);
FRETVAL(intf) nscsattr(intf *id, _fcd name, intf *nt, intf *count, 
                       VOIDP data, intf *len);
FRETVAL(intf) nscid2ref(intf *id);
FRETVAL(intf) nscr2idx(intf *id, intf *ref);
FRETVAL(intf) nsciscvar(intf *id);
FRETVAL(intf) nscsextf(intf *id, _fcd name, intf *offset,
                       intf *namelen);
FRETVAL(intf) nscsnbit(intf *id, intf *start_bit, intf *bit_len,
                       intf *sign_ext, intf *fill_one);
FRETVAL(intf) nscsacct(intf *id, intf *type);
FRETVAL(intf) nscsdmvc(intf *id, intf *compmode);
FRETVAL(intf) nscisdmvc(intf *id);
FRETVAL(intf) nscsflmd(intf *id, intf *fillmode);
#else
FRETVAL(intf) nscstart();
FRETVAL(intf) nscend();
FRETVAL(intf) nscendacc();
FRETVAL(intf) nscfinfo();
FRETVAL(intf) nscselct();
FRETVAL(intf) nscdimid();
FRETVAL(intf) nscginfo();
FRETVAL(intf) nscgcal();
FRETVAL(intf) nscscal();
FRETVAL(intf) nscsdscale();
FRETVAL(intf) nscgdscale();
FRETVAL(intf) nscscfill();
FRETVAL(intf) nscgcfill();
FRETVAL(intf) nscsfill();
FRETVAL(intf) nscgfill();
FRETVAL(intf) nscgrange();
FRETVAL(intf) nscsrange();
FRETVAL(intf) nscn2index();
FRETVAL(intf) nsccreate();
FRETVAL(intf) nscsdimstr();
FRETVAL(intf) nscsdimname();
FRETVAL(intf) nscsdatstr();
FRETVAL(intf) nscrcatt();
FRETVAL(intf) nscrnatt();
FRETVAL(intf) nscrattr();
FRETVAL(intf) nscrdata();
FRETVAL(intf) nscwdata();
FRETVAL(intf) nscgdimstrs();
FRETVAL(intf) nscrcdata();
FRETVAL(intf) nscwcdata();
FRETVAL(intf) nscgdatstrs();
FRETVAL(intf) nscgainfo();
FRETVAL(intf) nscgdinfo();
FRETVAL(intf) nscscatt();
FRETVAL(intf) nscsnatt();
FRETVAL(intf) nscsattr();
FRETVAL(intf) nscid2ref();
FRETVAL(intf) nscr2idx();
FRETVAL(intf) nsciscvar();
FRETVAL(intf) nscsextf();
FRETVAL(intf) nscsnbit();
FRETVAL(intf) nscsacct();
FRETVAL(intf) nscsdmvc();
FRETVAL(intf) nscisdmvc();
FRETVAL(intf) nscsflmd();
#endif /* PROTOTYPE */

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
    HDfree((VOIDP)fn);

    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scend
 * Purpose: Call SDend close to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscend(intf *file_id)
#else
nscend(file_id)
    intf *file_id;
#endif /* PROTOTYPE */
{
    return(SDend(*file_id));
}

/*-----------------------------------------------------------------------------
 * Name:    scendacc
 * Purpose: Call SDendaccess close to end access on a dataset 
 * Inputs:  id: id of the data set
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscendacc(intf *id)
#else
nscendacc(id)
    intf *id;
#endif /* PROTOTYPE */
{
    return(SDendaccess(*id));
}
/*-----------------------------------------------------------------------------
 * Name:    scfinfo
 * Purpose: Call SDfileinfo to get number of datasets and global attrs in the file
 * Inputs:  file_id: handle to HDF file
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscfinfo(intf *file_id, intf *datasets, intf *gattr)
#else
nscfinfo(file_id, datasets, gattr)
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
 * Name:    scselct
 * Purpose: Call SDselect to return a handle to a data set in the given file
 * Inputs:  file_id: handle to HDF file
 *          index:   number of data set to chose
 * Returns: sdsid on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscselct(intf *file_id, intf *index)
#else
nscselct(file_id, index)
     intf *file_id;
     intf *index;
#endif /* PROTOTYPE */
{
    return(SDselect(*file_id, *index));
}

/*-----------------------------------------------------------------------------
 * Name:    scdimid
 * Purpose: Get an id for a dimension of a given data set
 * Inputs:  id: handle to a data set
 *          index: number of dimension to chose
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscdimid(intf *id, intf *index)
#else
nscdimid(id, index)
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
 * Name:    scginfo
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
nscginfo(intf *id, _fcd name, intf *rank, intf *dimsizes, intf *nt, intf *nattr, intf *len)
#else
nscginfo(id, name, rank, dimsizes, nt, nattr, len)
     intf *id;
     _fcd name;
     intf *rank, *dimsizes, *nt, *nattr, *len;
#endif /* PROTOTYPE */
{
    char *iname;
    int32 status;
    int32 cdims[100], i;
    int32 rank32, nt32, nattr32;

    iname = NULL;
    if(*len)   iname  = (char *) HDmalloc((uint32)*len + 1);

    status = SDgetinfo((int32) *id, iname, &rank32, cdims, &nt32, &nattr32);

    for(i = 0; i < rank32; i++)
        dimsizes[i] = cdims[rank32 - i - 1];

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfree((VOIDP)iname);
  
    *rank  = (intf) rank32;
    *nt    = (intf) nt32;
    *nattr = (intf) nattr32;

    return(status);

}

/*-----------------------------------------------------------------------------
 * Name:    scgcal
 * Purpose: Call SDgetcal to get the calibration information
 * Inputs:  id: handle to a data set
 * Outputs: cal, cale, ioff, ioffe : calibration factors and error
 *          nt: number type of data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgcal(intf *id, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, intf *nt)
#else
nscgcal(id, cal, cale, ioff, ioffe, nt)
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
 * Name:    scscal
 * Purpose: Call SDsetcal to set the calibration information
 * Inputs:  id: handle to a data set
 *          cal, cale, ioff, ioffe : calibration factors and error
 *          nt: number type of data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscscal(intf *id, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, intf *nt)
#else
nscscal(id, cal, cale, ioff, ioffe, nt)
     intf *id;
     float64 *cal, *cale, *ioff, *ioffe;
     intf *nt;
#endif /* PROTOTYPE */
{
    return(SDsetcal(*id, *cal, *cale, *ioff, *ioffe, *nt));
}

/*-----------------------------------------------------------------------------
 * Name:    scsdscale
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
nscsdscale(intf *id, intf *count, intf *nt, VOIDP values)
#else
nscsdscale(id, count, nt, values)
     intf *id;
     intf *count, *nt;
     VOIDP values;
#endif /* PROTOTYPE */
{
    return(SDsetdimscale(*id, *count, *nt, values));
}

/*-----------------------------------------------------------------------------
 * Name:    scgdscale
 * Purpose: Call SDgetdimscale to get the values for a dimension
 * Inputs:  id: handle to a dimension
 * Output   values: data
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgdscale(intf *id, VOIDP values)
#else
nscgdscale(id, values)
     intf *id;
     VOIDP values;
#endif /* PROTOTYPE */
{
    return(SDgetdimscale(*id, values));
}

/*----------------------------------------------------------
 * Name:    scscfill
 * Purpose: Call nsfsnfill to set the char fill value
 * Inputs:  id: handle to a dimension
 *          val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *----------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscscfill(intf *id, _fcd val)
#else
nscscfill(id, val)
     intf *id;
     _fcd val;
#endif /* PROTOTYPE */
{
    return(nscsfill(id, (VOIDP) _fcdtocp(val)));
}

/*------------------------------------------------------------
 * Name:    scgcfill
 * Purpose: Call sfgfill to get the char fill value
 * Inputs:  id: handle to a dimension
 * Output:  val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *-----------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgcfill(intf *id, _fcd val)
#else
nscgcfill(id, val)
     intf *id;
     _fcd val;
#endif /* PROTOTYPE */
{
    return(nscgfill(id, (VOIDP) _fcdtocp(val)));
}

/*---------------------------------------------------------
 * Name:    scsfill
 * Purpose: Call SDsetfillvalue to set the numeric fill value
 * Inputs:  id: handle to a dimension
 *          val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscsfill(intf *id, VOIDP val)
#else
nscsfill(id, val)
     intf *id;
     VOIDP val;
#endif /* PROTOTYPE */
{
    return(SDsetfillvalue(*id, val));
}

/*----------------------------------------------------------
 * Name:    scgfill
 * Purpose: Call SDgetfillvalue to get the fill value. 
 * Inputs:  id: handle to a dimension
 * Output:  val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgfill(intf *id, VOIDP val)
#else
nscgfill(id, val)
     intf *id;
     VOIDP val;
#endif /* PROTOTYPE */
{
    return(SDgetfillvalue(*id, val));
}

/*------------------------------------------------------------
 * Name:    scgrange
 * Purpose: Call SDgetrange to get the valid range info
 * Inputs:  id: handle to a dataset
 * Output:  min: the min
 *          max: the max
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscgrange(intf *id, VOIDP max, VOIDP min)
#else
nscgrange(id, max, min)
     intf *id;
     VOIDP max;
     VOIDP min;
#endif /* PROTOTYPE */
{
    return(SDgetrange(*id, max, min));
}

/*-----------------------------------------------------------------------------
 * Name:    scsrange
 * Purpose: Call SDsetrange to set the valid range info
 * Inputs:  id: handle to a dataset
 *          min: the min
 *          max: the max
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscsrange(intf *id, VOIDP max, VOIDP min)
#else
nscsrange(id, max, min)
     intf *id;
     VOIDP max;
     VOIDP min;
#endif /* PROTOTYPE */
{
    return(SDsetrange(*id, max, min));
}

/*-----------------------------------------------------------------------------
 * Name:    scn2index
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
    HDfree((VOIDP)fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    sccreate
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
    cdims = (int32 *) HDmalloc(sizeof(int32) * (*rank));
    if(!cdims) return FAIL;

    for(i = 0; i < *rank; i++)
        cdims[i] = dims[*rank - i - 1];

    ret = (intf) SDcreate(*id, fn, *nt, *rank, cdims);

    HDfree((VOIDP)fn);
    HDfree((VOIDP)cdims);
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
    if(ll) HDfree((VOIDP)lstr);
    if(ul) HDfree((VOIDP)ustr);
    if(fl) HDfree((VOIDP)fstr);
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
    if(len) HDfree((VOIDP)nstr);
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
    if(ll) HDfree((VOIDP)lstr);
    if(ul) HDfree((VOIDP)ustr);
    if(fl) HDfree((VOIDP)fstr);
    if(cl) HDfree((VOIDP)cstr);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scrcatt
 * Purpose: Call sfrnatt to get the contents of a char attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscrcatt(intf *id, intf *index, _fcd buf)
#else
nscrcatt(id, index, buf)
     intf *id;
     intf *index;
     _fcd buf;
#endif /* PROTOTYPE */
{
    return(nscrnatt(id, index, (VOIDP) _fcdtocp(buf)));
}

/*-----------------------------------------------------------------------------
 * Name:    scrnatt
 * Purpose: Call SDreadattr to get the contents of a numeric attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscrnatt(intf *id, intf *index, VOIDP buf)
#else
nscrnatt(id, index, buf)
     intf *id;
     intf *index;
     VOIDP buf;
#endif /* PROTOTYPE */
{
    return(SDreadattr(*id, *index, buf));
}

/*-----------------------------------------------------------------------------
 * Name:    scrattr
 * Purpose: Call SDreadattr to get the contents of an attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscrattr(intf *id, intf *index, VOIDP buf)
#else
nscrattr(id, index, buf)
     intf *id;
     intf *index;
     VOIDP buf;
#endif /* PROTOTYPE */
{
    return(nscrnatt(id, index, buf));
}

/*------------------------------------------------------------
 * Name:    scrdata
 * Purpose: read a section of numeric data
 * Inputs:  id: dataset id
 *          start: start location
 *          stride: stride along each dimension
 *          end: number of values along each dim to read
 *          values: data 
 * Remarks: need to flip the dimensions to account for array ordering
 *          differences
 * Returns: 0 on success, -1 on failure with error set
 *----------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscrdata(intf *id, intf *start, intf *stride, intf *end, VOIDP values)
#else
nscrdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     VOIDP values;
#endif /* PROTOTYPE */
{
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
        if((cstride[i] = stride[rank - i - 1]) != 1) nostride = FALSE;
    }
    
    ret = (intf) SDreaddata(*id, cstart, (nostride? NULL : cstride), cend, values);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scwdata
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
nscwdata(intf *id, intf *start, intf *stride, intf *end, VOIDP values)
#else
nscwdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     VOIDP values;
#endif /* PROTOTYPE */
{
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
        if((cstride[i] = stride[rank - i - 1]) != 1) nostride = FALSE;
    }

    ret = (intf) SDwritedata(*id, cstart, (nostride? NULL : cstride), cend, values);
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
    intf ret;
    iunit = ilabel = iformat = NULL;

    if(*llabel)  ilabel  = (char *) HDmalloc((uint32)*llabel + 1);
    if(*lunit)   iunit   = (char *) HDmalloc((uint32)*lunit + 1);
    if(*lformat) iformat = (char *) HDmalloc((uint32)*lformat + 1);

    ret = (intf)SDgetdimstrs(*dim, ilabel, iunit, iformat, *mlen);

    HDpackFstring(ilabel,  _fcdtocp(label),  *llabel);
    HDpackFstring(iunit,   _fcdtocp(unit),   *lunit);
    HDpackFstring(iformat, _fcdtocp(format), *lformat);

    if(ilabel)  HDfree((VOIDP)ilabel);
    if(iunit)   HDfree((VOIDP)iunit);
    if(iformat) HDfree((VOIDP)iformat);

    return ret;
}

/*--------------------------------------------------------
 * Name:    scrcdata
 * Purpose: read a section of char data
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
nscrcdata(intf *id, intf *start, intf *stride, intf *end, _fcd values)
#else
nscrcdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     _fcd values;
#endif /* PROTOTYPE */
{
     return(nscrdata(id, start, stride, end, (VOIDP) _fcdtocp(values)));
}

/*--------------------------------------------------------
 * Name:    scwcdata
 * Purpose: write a section of char data
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
nscwcdata(intf *id, intf *start, intf *stride, intf *end, _fcd values)
#else
nscwcdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     _fcd values;
#endif /* PROTOTYPE */
{
     return(nscwdata(id, start, stride, end, (VOIDP) _fcdtocp(values)));
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
    intf ret;

    iunit = ilabel = iformat = NULL;

    if(*llabel)  ilabel  = (char *) HDmalloc((uint32)*llabel + 1);
    if(*lunit)   iunit   = (char *) HDmalloc((uint32)*lunit + 1);
    if(*lformat) iformat = (char *) HDmalloc((uint32)*lformat + 1);
    if(*lcoord)  icoord  = (char *) HDmalloc((uint32)*lcoord + 1);

    ret = (intf)SDgetdatastrs(*id, ilabel, iunit, iformat, icoord, *len);

    HDpackFstring(ilabel,  _fcdtocp(label),  *llabel);
    HDpackFstring(iunit,   _fcdtocp(unit),   *lunit);
    HDpackFstring(iformat, _fcdtocp(format), *lformat);
    HDpackFstring(icoord,  _fcdtocp(coord),  *lcoord);

    if(ilabel)  HDfree((VOIDP)ilabel);
    if(iunit)   HDfree((VOIDP)iunit);
    if(iformat) HDfree((VOIDP)iformat);
    if(icoord)  HDfree((VOIDP)icoord);

    return ret;
}


/*-----------------------------------------------------------------------------
 * Name:    scgainfo
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
nscgainfo(intf *id, intf *number, _fcd name, intf *nt, intf *count, intf *len)
#else
nscgainfo(id, number, name, nt, count, len)
     intf *id, *number;
     _fcd name;
     intf *count, *len, *nt;
#endif /* PROTOTYPE */
{
    char  * iname;
    intn    status;
    int32   nt32;
    int32   cnt32;

    iname = NULL;
    if(*len)   iname  = (char *) HDmalloc((uint32)*len + 1);

    status = SDattrinfo(*id, *number, iname, &nt32, &cnt32);

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfree((VOIDP)iname);

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
nscgdinfo(intf *id, _fcd name, intf *sz, intf *nt, intf *nattr, intf *len)
#else
nscgdinfo(id, name, sz, nt, nattr, len)
     intf *id;
     _fcd name;
     intf *sz, *nattr, *len, *nt;
#endif /* PROTOTYPE */
{
    char *iname;
    int32 status;
    int32 sz32, nt32, nattr32;

    iname = NULL;
    if(*len)   iname  = (char *) HDmalloc((uint32)*len + 1);

    status = SDdiminfo(*id, iname, &sz32, &nt32, &nattr32);

    HDpackFstring(iname,  _fcdtocp(name),  *len);

    if(iname)  HDfree((VOIDP)iname);
  
    *nt    = (intf) nt32;
    *sz    = (intf) sz32;
    *nattr = (intf) nattr32;

    return(status);
}

/*-----------------------------------------------------------------------------
 * Name:    scscatt
 * Purpose: calls scsnatt to create a new char attribute (or modify an existing one)
 * Inputs:  id: id (file or data set)
 *          name: name of attribute
 *          nt: number type
 *          count: number of values
 *          data: where the values are
 *          len: length of name
 * Remarks: 
 * Returns: 0 on success, -1 on failure with error set
 *--------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscscatt(intf *id, _fcd name, intf *nt, intf *count, _fcd data, intf *len)
#else
nscscatt(id, name, nt, count, data, len)
     intf *id;
     _fcd name;
     intf *nt, *count;
     _fcd data;
     intf *len;
#endif /* PROTOTYPE */
{
    return(nscsnatt(id, name, nt, count, (VOIDP) _fcdtocp(data), len));
}

/*-------------------------------------------------------------
 * Name:    scsnatt
 * Purpose: create a new numeric attribute (or modify an existing one)
 * Inputs:  id: id (file or data set)
 *          name: name of attribute
 *          nt: number type
 *          count: number of values 
 *          data:  where the values are
 *          len: length of name
 * Remarks: This routine and scscattr are used to replace scsattr
 * Returns: 0 on success, -1 on failure with error set
 *--------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsnatt(intf *id, _fcd name, intf *nt, intf *count, VOIDP data, intf *len)
#else
nscsnatt(id, name, nt, count, data, len)
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

    HDfree((VOIDP)an);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    scsattr
 * Purpose: for backward compatibility. Calls SDsetattr to create
 *          or modify an existing attribute
 * Inputs:  id: id (file or data set)
 *          name: name of attribute
 *          nt: number type
 *          count: number of values
 *          data: where the values are
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 * Remarks: This routine and sfsattr should be phased out.
 *          sfsattr declairs data as char *, scscatt assumes
 *          data as VOIDP. This causes problems on VMS and T3D.
 *--------------------------------------------------------------------*/

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
    HDfree((VOIDP)an);
    return(ret);
}

/*---------------------------------------------------------------------
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
    HDfree((VOIDP)fn);

    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scid2ref
 * Purpose: call SDidtoref to map an id to ref
 * Inputs:  id: variable id
 * Returns: reference number of a NDG representing this dataset
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscid2ref(intf *id)
#else
nscid2ref(id)
    intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDidtoref(*id));
}


/*-----------------------------------------------------------------------------
 * Name:    scfr2idx
 * Purpose: call SDreftoindex to map a reference number to an index of a 
 *          dataset
 * Inputs:  id: file id
 *          ref: reference number to look up
 * Returns: index of a NDG representing this dataset
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscr2idx(intf *id, intf *ref)
#else
nscr2idx(id, ref)
     intf *id;
     intf *ref;
#endif /* PROTOTYPE */
{
    return((intf) SDreftoindex(*id, (int32) *ref));
}


/*-----------------------------------------------------------------------------
 * Name:    sciscvar
 * Purpose: call SDiscoordvar to see if a dataset is a coordinate variable
 *          dataset
 * Inputs:  id: sds id
 * Returns: TRUE/FALSE
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsciscvar(intf *id)
#else
nsciscvar(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDiscoordvar(*id));
}

/*-----------------------------------------------------------------------------
 * Name:    scsextf
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
    if (!fn) return(FAIL);
    ret = (intf) SDsetexternalfile(*id, fn, *offset);
    HDfree((VOIDP)fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scsnbit
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
nscsnbit(intf *id, intf *start_bit, intf *bit_len, intf *sign_ext, intf *fill_one)
#else
nscsnbit(id, start_bit, bit_len, sign_ext, fill_one)
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
 * Name:    scsacct
 * Purpose: Call SDsetaccesstype to set the access type
 * Inputs:  id: sds id
 *          type: the access type
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscsacct(intf *id, intf *type)
#else
nscsacct(id, type)
     intf *id;
     intf *type;
#endif /* PROTOTYPE */
{
    return((intf) SDsetaccesstype(*id, *type));
}
/*-----------------------------------------------------------------------------
 * Name:    scsdmvc
 * Purpose: Call SDsetdimval_comp to set the dim value backward 
 *            compatibility type
 * Inputs:  id: dim id
 *          compmode: backward compatibility:
 *                    SD_DIMVAL_BW_COMP -- compatible (in mfhdf.h)
 *                    SD_DIMVAL_BW_INCOMP -- incompatible.
 *
 * Returns: SUCCESS on success, FAIL on failure 
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscsdmvc(intf *id, intf *compmode)
#else
nscsdmvc(id, compmode)
     intf *id;
     intf *compmode;
#endif /* PROTOTYPE */
{
    return((intf) SDsetdimval_comp(*id, *compmode));
}

/*-----------------------------------------------------------------------------
 * Name:    scisdmvc
 * Purpose: Call SDisdimval_bwcomp to get the dim value backward 
 *            compatibility 
 * Inputs:  id: dim id
 *
 * Returns: SD_DIMVAL_BW_COMP (1) if dimval is backward compatible;
            SD_DIMVAL_BW_INCOMP (0) for not compatible; (in mfhdf.h)
            FAIL (-1) for error.
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscisdmvc(intf *id)
#else
nscisdmvc(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDisdimval_bwcomp(*id));
}

/*-----------------------------------------------------------------------------
 * Name:    scsflmd
 * Purpose: Call SDsetfillmode to set for the file fill mode 
 *            as fill or nofill
 * Inputs:  id: file id
 *          fillmode: Desired fill mode for the file,
 *                                 either SD_FILL or SD_NOFILL.
 *                                 SD_FILL is the default mode.
 *
 * Returns: The current (before the new mode is set) fill mode;
 *          FAIL (-1) for error. 
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nscsflmd(intf *id, intf *fillmode)
#else
nscsflmd(id, fillmode)
    intf *id, *fillmode;
#endif  /* PROTOTYPE */
{
    return((intf) SDsetfillmode(*id, *fillmode));
}
#endif /* HDF */

