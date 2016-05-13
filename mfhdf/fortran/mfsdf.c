/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  Copyright by The HDF Group.                                               *
*  Copyright by the Board of Trustees of the University of Illinois.         *
*  All rights reserved.                                                      *
*                                                                            *
*  This file is part of HDF.  The full HDF copyright notice, including       *
*  terms governing use, modification, and redistribution, is contained in    *
*  the files COPYING and Copyright.html.  COPYING can be found at the root   *
*  of the source code distribution tree; Copyright.html can be found at      *
*  http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
*  access to either file, you may request a copy from help@hdfgroup.org.     *
* * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
#include "mfsdf.h"

#ifdef HDF

#ifdef PROTOTYPE
FRETVAL(intf) nsfscfill(intf *id, _fcd val);
FRETVAL(intf) nsfsfill(intf *id, VOIDP val);
FRETVAL(intf) nsfgfill(intf *id, VOIDP val);
FRETVAL(intf) nsfrnatt(intf *id, intf *index, VOIDP buf);
FRETVAL(intf) nscsnatt(intf *id, _fcd name, intf *nt, intf *count, VOIDP data, intf *len);
FRETVAL(intf) nsfsflmd(intf *id, intf *fillmode);
#else
FRETVAL(intf) nsfscfill();
FRETVAL(intf) nsfsfill();
FRETVAL(intf) nsfgfill();
FRETVAL(intf) nsfrnatt();
FRETVAL(intf) nscsnatt();
FRETVAL(intf) nsfsflmd();
#endif /* PROTOTYPE */

#if defined _WIN32 && !defined CMAKE_INTDIR

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
/*-----------------------------------------------------------------------------
 * Name:    scisrcrd
 * Purpose: call SDisrecord to see if a dataset is a record variable
 * Inputs:  id: sds id
 * Returns: TRUE/FALSE (1/0))
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfisrcrd(intf *id)
#else
nscisrcrd(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDisrecord(*id));
}
/*-----------------------------------------------------------------------------
 * Name:    scsblsz
 * Purpose: call SDsetblocksize
 * Inputs:  id:          sd id
            block_size:  block size  in bytes
 * Returns: SUCCEED/FAIL (0/-1)
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscsblsz(intf *id, intf *block_size)
#else
nscsblsz(id, block_size)
     intf *id;
     intf *block_size;
#endif /* PROTOTYPE */
{
    return((intf) SDsetblocksize(*id, *block_size));
}

#endif /* _WIN32 && !CMAKE_INTDIR */
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

/*----------------------------------------------------------
 * Name:    sdfscfill
 * Purpose: Call nsfsnfill to set the char fill value
 * Inputs:  id: handle to a dimension
 *          val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *----------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfscfill(intf *id, _fcd val)
#else
nsfscfill(id, val)
     intf *id;
     _fcd val;
#endif /* PROTOTYPE */
{
    return(nsfsfill(id, (VOIDP) _fcdtocp(val)));
}

/*------------------------------------------------------------
 * Name:    sdfgcfill
 * Purpose: Call sfgfill to get the char fill value
 * Inputs:  id: handle to a dimension
 * Output:  val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *-----------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfgcfill(intf *id, _fcd val)
#else
nsfgcfill(id, val)
     intf *id;
     _fcd val;
#endif /* PROTOTYPE */
{
    return(nsfgfill(id, (VOIDP) _fcdtocp(val)));
}

/*---------------------------------------------------------
 * Name:    sdfsfill
 * Purpose: Call SDsetfillvalue to set the numeric fill value
 * Inputs:  id: handle to a dimension
 *          val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *------------------------------------------------------------*/

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

/*----------------------------------------------------------
 * Name:    sdfgfill
 * Purpose: Call SDgetfillvalue to get the fill value. 
 * Inputs:  id: handle to a dimension
 * Output:  val: the fill value
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *------------------------------------------------------*/

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

/*------------------------------------------------------------
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
    HDfree((VOIDP)fn);
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
 * Name:    sfrcatt
 * Purpose: Call sfrnatt to get the contents of a char attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfrcatt(intf *id, intf *index, _fcd buf)
#else
nsfrcatt(id, index, buf)
     intf *id;
     intf *index;
     _fcd buf;
#endif /* PROTOTYPE */
{
    return(nsfrnatt(id, index, (VOIDP) _fcdtocp(buf)));
}

/*-----------------------------------------------------------------------------
 * Name:    sfrnatt
 * Purpose: Call SDreadattr to get the contents of a numeric attribute
 * Inputs:  id: handle to a dataset
 *          index: index of the attribute to read
 *          buf: space to hold info
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nsfrnatt(intf *id, intf *index, VOIDP buf)
#else
nsfrnatt(id, index, buf)
     intf *id;
     intf *index;
     VOIDP buf;
#endif /* PROTOTYPE */
{
    return(SDreadattr(*id, *index, buf));
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
    return(nsfrnatt(id, index, buf));
}

/*------------------------------------------------------------
 * Name:    sfrdata
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
nsfrdata(intf *id, intf *start, intf *stride, intf *end, VOIDP values)
#else
nsfrdata(id, start, stride, end, values)
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
     intf *mlen;
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
 * Name:    sfrcdata
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
nsfrcdata(intf *id, intf *start, intf *stride, intf *end, _fcd values)
#else
nsfrcdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     _fcd values;
#endif /* PROTOTYPE */
{
     return(nsfrdata(id, start, stride, end, (VOIDP) _fcdtocp(values)));
}

/*--------------------------------------------------------
 * Name:    sfwcdata
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
nsfwcdata(intf *id, intf *start, intf *stride, intf *end, _fcd values)
#else
nsfwcdata(id, start, stride, end, values)
     intf *id;
     intf *start, *stride, *end;
     _fcd values;
#endif /* PROTOTYPE */
{
     return(nsfwdata(id, start, stride, end, (VOIDP) _fcdtocp(values)));
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
    char *ilabel, *iunit, *iformat, *icoord=NULL;
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
 *          data as VOIDP.
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
nsfiscvar(id)
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
/*-----------------------------------------------------------------------------
 * Name:    sfsdmvc
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
nsfsdmvc(intf *id, intf *compmode)
#else
nsfsdmvc(id, compmode)
     intf *id;
     intf *compmode;
#endif /* PROTOTYPE */
{
    return((intf) SDsetdimval_comp(*id, *compmode));
}

/*-----------------------------------------------------------------------------
 * Name:    sfisdmvc
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
nsfisdmvc(intf *id)
#else
nsfisdmvc(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDisdimval_bwcomp(*id));
}

/*-----------------------------------------------------------------------------
 * Name:    sfsflmd
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
nsfsflmd(intf *id, intf *fillmode)
#else
nsfsflmd(id, fillmode)
    intf *id, *fillmode;
#endif  /* PROTOTYPE */
{
    return((intf) SDsetfillmode(*id, *fillmode));
}
#endif /* HDF */

/*-------------------------------------------------------------------------
 * Name:    scgichnk
 * Puporse: Call SDgetchunkinfo
 * Inputs:  id: SDS access id
 * Outputs: dim_length: chunk dimensions
 *          flags:            -1 - SDS is nonchunked
 *                             0 - SDS is chunked, no compression
 *                             1 - SDS is chunked and compressed 
 *                                 with RLE, SKPHUFF and GZIP 
 *                             2 - SDS is chunked and compressed with NBIT 
 * Actual parameters are not returned. Will be fixed after HDF41r.2
 *          comp_prm[0] = nbit_sign_ext, 
 *          comp_prm[1] = nbit_fill_one, 
 *          comp_prm[2] = nbit_start_bit, 
 *          comp_prm[3] = nbit_bit_len:     NBIT compression parametes 
 *
 *          comp_prm[0] = skphuff_skp_size: size of individual elements for 
 *                            Adaptive Huffman compression algorithm
 *          comp_prm[0] = deflate_level:    GZIP  compression parameter
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers          
 *-------------------------------------------------------------------------*/

    FRETVAL (intf)
#ifdef PROTOTYPE
      nscgichnk(intf *id, intf *dim_length, intf *flags)
#else
       nscgichnk( id, dim_length, flags)
       intf *id;       
       intf *dim_length;
       intf *flags;
#endif /* PROTOTYPE */
{

HDF_CHUNK_DEF chunk_def;  /* Chunk definition set */
int32 sdsid;              /* SDS id               */
int   i;
int32 rank, status, cflags;
intf ret;

int32 cdims[100], nt32, nattr32;
sdsid = *id;

/* Get  SDS rank */

       status = SDgetinfo(sdsid, NULL, &rank, cdims, &nt32, &nattr32 );
       if(status == FAIL) return FAIL;

/* Get SDS info */
  
       status = SDgetchunkinfo(sdsid, &chunk_def, &cflags);
       if(status == FAIL) return FAIL;


switch (cflags)  

  {

  case HDF_NONE:       /* Non-chunked SDS */

    *flags = -1;
     ret = 0;
     return(ret);

  case HDF_CHUNK:    /* Chunked, noncompressed SDS */

    *flags = 0;
     for (i=0; i < rank; i++)
          dim_length[rank-i-1] = chunk_def.chunk_lengths[i];
     ret = 0;
     return(ret);

  case (HDF_CHUNK | HDF_COMP):     /* Chunked and compressed SDS
                                      RLE, SKPHUFF or GZIP only */
 
     *flags = 1;
     for (i=0; i < rank; i++)
          dim_length[rank-i-1] =  chunk_def.comp.chunk_lengths[i];
     ret = 0;
     return(ret);

  case (HDF_CHUNK | HDF_NBIT):     /* Chunked and NBIT compressed SDS */
 
     *flags = 2;
     for (i=0; i < rank; i++)
          dim_length[rank-i-1] =  chunk_def.nbit.chunk_lengths[i];
     ret = 0;
     return(ret);

  default:

    return FAIL;
            
  }


}   
/*-----------------------------------------------------------------------------
 * Name:     scrchnk
 * Purpose:  read the specified chunk of NUMERIC data to the buffer
 * Inputs:   id        - data set ID
 *           start     - origin of chunk to read 
 * Outputs:  num_data  - buffer the data will be read into
 * Calls:    SDreadchunk
 * Remarks:  need to flip the dimensions to account for array ordering
 *           differencies (start --> cstart)
 *           If performance becomes an issue, use static cstart 
 * Returns:  0 on success, -1 on failure with error set
 *----------------------------------------------------------------------------*/   
    FRETVAL (intf)
#ifdef PROTOTYPE
       nscrchnk(intf *id, intf *start, VOIDP num_data)
#else
       nscrchnk(id, start, num_data)
                intf *id;
                intf *start;
                VOIDP num_data;
#endif /* PROTOTYPE */

{
       intf    ret;
       int32   rank, status, i;
       int32   *cstart;

       int32   cdims[100], nt32, nattr32;
/* Get rank of SDS */

       status = SDgetinfo(*id, NULL, &rank, cdims, &nt32, &nattr32);
       if(status == FAIL) return FAIL;

/* Allocate memory for cstart array; use static array, if performance
       becomes an issue */

       cstart = (int32 *) HDmalloc(sizeof(int32) * rank);
       if(!cstart) return FAIL;

/* Flip an array to account for array odering in Fortran and C */

       for ( i=0; i < rank; i++)
             cstart[i] = start[rank-i-1] - 1;

/* Call SDreadChunk function to read the data */

       ret = SDreadchunk(*id, cstart, num_data);

/* Free memory */

       HDfree((VOIDP)cstart);
       return(ret);

} 

/*-----------------------------------------------------------------------------
 * Name:     scrcchnk
 * Purpose:  read the specified chunk of CHARACTER data to the buffer
 * Inputs:   id        - data set ID
 *           start     - origin of chunk to read 
 * Outputs:  char_data  - buffer the data will be read into
 * Calls:    scrchnk 
 * Reamrks:  dimensions will be flipped in scrchnk function
 * Returns:  0 on success, -1 on failure with error set
 *----------------------------------------------------------------------------*/   
    FRETVAL (intf)
#ifdef PROTOTYPE 
       nscrcchnk(intf *id, intf *start, _fcd char_data)
#else
       nscrcchnk(id, start, char_data)
                intf *id;
                intf *start;
               _fcd  char_data;
#endif /* PROTOTYPE */

{
       intf  ret;

       ret = nscrchnk(id, start, (VOIDP) _fcdtocp(char_data));

       return(ret);

} 

/*-----------------------------------------------------------------------------
 * Name:     scscchnk
 * Purpose:  set the maximum number of chunks to cache 
 * Inputs:   id        - data set ID
 *           maxcache  - max number of chunks to cache
 *           flags     - flags= 0, HDF_CACHEALL
 *                       Currently only 0 can be passed
 * Calls:    SDsetchunkcache
 * Returns:  0 on success, -1 on failure with error set
 *----------------------------------------------------------------------------*/   
    FRETVAL (intf)
#ifdef PROTOTYPE
       nscscchnk(intf *id, intf *maxcache, intf *flags)
#else
       nscscchnk(id, maxcache, flags)
                intf *id;
                intf *maxcache;
                intf *flags;
#endif /* PROTOTYPE */

{
       intf  ret;

       ret = SDsetchunkcache(*id, *maxcache, *flags);

       return(ret);

} 

/*-------------------------------------------------------------------------
 * Name:    scschnk
 * Puporse: Call SDsetchunk
 * Inputs:  id: SDS access id
 *          dim_length: chunk dimensions
 *          comp_type:  type of compression
 *                              COMP_CODE_NONE    (0)
 *                              COMP_CODE_RLE     (1)
 *                              COMP_CODE_NBIT    (2)
 *                              COMP_CODE_SKPHUFF (3)
 *                              COMP_CODE_DEFLATE (4)
 *                              COMP_CODE_INVALID (5)
 *          comp_prm[0] = nbit_sign_ext, 
 *          comp_prm[1] = nbit_fill_one, 
 *          comp_prm[2] = nbit_start_bit, 
 *          comp_prm[3] = nbit_bit_len:     NBIT compression parametes 
 *
 *          comp_prm[0] = skphuff_skp_size: size of individual elements for 
 *                            Adaptive Huffman compression algorithm
 *          comp_prm[0] = deflate_level:    GZIP  compression parameter
 *          comp_prm[0] = option_mask:      SZIP option mask parameter
 *          comp_prm[1] = pixels_per_block  SZIP parameter
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers          
 *-------------------------------------------------------------------------*/

    FRETVAL (intf)
#ifdef PROTOTYPE
      nscschnk(intf *id, intf *dim_length, intf *comp_type,
                intf *comp_prm)
#else
       nscschnk( id, dim_length, comp_type,
                 comp_prm)
       intf *id;
       intf *dim_length;
       intf *comp_type;
       intf *comp_prm;
#endif /* PROTOTYPE */
{

    HDF_CHUNK_DEF chunk_def;  /* Chunk definition set */
    int32 sdsid;              /* SDS id               */
    int32 cflags;             /* chunk flags          */
    int   i, CASE;
    int32 rank, status;
    intf ret;

    int32 cdims[100], nt32, nattr32;

    /* Get rank of SDS */

       status = SDgetinfo((int32) *id, NULL , &rank, cdims, &nt32, &nattr32);
       if(status == FAIL) return FAIL;

    CASE = *comp_type;
    sdsid = *id;
    cflags = HDF_CHUNK | HDF_COMP;

    switch (CASE)  {

       case 0:       /* No compression */
         cflags = HDF_CHUNK;
         for (i=0; i < rank; i++)
               chunk_def.chunk_lengths[i] = dim_length[rank-i-1]; 
         break;
    
       case 1:       /* RLE compression */
         cflags = HDF_CHUNK | HDF_COMP;
         for (i=0; i < rank; i++)
                chunk_def.comp.chunk_lengths[i] = dim_length[rank-i-1];

         chunk_def.comp.comp_type = COMP_CODE_RLE;

         break;


       case 2:      /* N-bit encoding */
         cflags = HDF_CHUNK | HDF_NBIT;
         for (i=0; i < rank; i++)
                chunk_def.nbit.chunk_lengths[i] = dim_length[rank-i-1]; 

         chunk_def.nbit.sign_ext = comp_prm[0]; 
         chunk_def.nbit.fill_one = comp_prm[1]; 
         chunk_def.nbit.start_bit = comp_prm[2];
         chunk_def.nbit.bit_len = comp_prm[3]; 

         break; 
 
       case 3:      /* Skipping Huffman encoding */
         cflags = HDF_CHUNK | HDF_COMP;
          for (i=0; i < rank; i++)
                chunk_def.comp.chunk_lengths[i] = dim_length[rank-i-1];

          chunk_def.comp.comp_type = COMP_CODE_SKPHUFF;
          chunk_def.comp.cinfo.skphuff.skp_size = comp_prm[0]; 

          break;

       case 4:      /* GZIP compression */  
         cflags = HDF_CHUNK | HDF_COMP;
          for (i=0; i < rank; i++)
                 chunk_def.comp.chunk_lengths[i] = dim_length[rank-i-1];
           
          chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
          chunk_def.comp.cinfo.deflate.level = comp_prm[0];

          break;

       case 5:      /* SZIP compression */  
         cflags = HDF_CHUNK | HDF_COMP;
          for (i=0; i < rank; i++)
                 chunk_def.comp.chunk_lengths[i] = dim_length[rank-i-1];
           
          chunk_def.comp.comp_type = COMP_CODE_SZIP;
          chunk_def.comp.cinfo.szip.options_mask     = comp_prm[0];
          chunk_def.comp.cinfo.szip.pixels_per_block = comp_prm[1];
          break;

       default:

          return FAIL;
                    
     }

    ret = SDsetchunk(sdsid, chunk_def, cflags);
    return(ret);

}   
/*-----------------------------------------------------------------------------
 * Name:     scwchnk
 * Purpose:  write the specified chunk of NUMERIC data to the SDS
 * Inputs:   id        - data set ID
 *           start     - origin of chunk to write
 *           num_data  - buffer for data
 * Calls:    SDwritechunk
 * Remarks:  need to flip the dimensions to account for array ordering
 *           differencies (start --> cstart)
 *           If performance becomes an issue, use static cstart 
 * Returns:  0 on success, -1 on failure with error set
 *----------------------------------------------------------------------------*/   
    FRETVAL (intf)
#ifdef PROTOTYPE
       nscwchnk(intf *id, intf *start, VOIDP num_data)
#else
       nscwchnk(id, start, num_data)
                intf *id;
                intf *start;
                VOIDP num_data;
#endif /* PROTOTYPE */

{
       intf    ret;
       int32   rank, status, i;
       int32   *cstart;

       int32   cdims[100], nt32, nattr32;
/* Get rank of SDS */

       status = SDgetinfo(*id, NULL, &rank, cdims, &nt32, &nattr32);
       if(status == FAIL) return FAIL;

/* Allocate memory for cstart array; use static array, if performance
       becomes an issue */

       cstart = (int32 *) HDmalloc(sizeof(int32) * rank);
       if(!cstart) return FAIL;

/* Flip an array */

       for ( i=0; i < rank; i++)
             cstart[i] = start[rank-i-1] - 1;

/* Call SDwriteChunk function to write the data */

       ret = SDwritechunk(*id, cstart, num_data);

/* Free memory */ 

       HDfree((VOIDP)cstart);

       return(ret);

} 
/*-----------------------------------------------------------------------------
 * Name:     scwcchnk
 * Purpose:  write the specified chunk of CHARACTER data to the SDS 
 * Inputs:   id        - data set ID
 *           start     - origin of chunk to read 
 * Outputs:  char_data  - buffer the data will be read into
 * Calls:    scwchnk 
 * Reamrks:  dimensions will be flipped in scrchnk function
 * Returns:  0 on success, -1 on failure with error set
 *----------------------------------------------------------------------------*/   
    FRETVAL (intf)
#ifdef PROTOTYPE
       nscwcchnk(intf *id, intf *start, _fcd char_data)
#else
       nscwcchnk(id, start, char_data)
                intf *id;
                intf *start;
               _fcd  char_data;
#endif /* PROTOTYPE */

{
       intf  ret;

       ret = nscwchnk(id, start, (VOIDP) _fcdtocp(char_data));

       return(ret);

} 

/*-------------------------------------------------------------------------
 * Name:    scscompress
 * Puporse: Call SDsetcompress 
 * Inputs:  id: SDS access id
 *          comp_type:  type of compression
 *                      COMP_CODE_NONE = 0
 *                      COMP_CODE_RLE  = 1
 *                      COMP_CODE_SKPHUFF = 3
 *                      COMP_CODE_DEFLATE = 4
 *                      COMP_CODE_SZIP = 5
 *          comp_prm[0] = skphuff_skp_size: size of individual elements for 
 *                            Adaptive Huffman compression algorithm
 *          comp_prm[0] = deflate_level:    GZIP  compression parameter
 *          SZIP:
 *          comp_prm[0] = options_mask    
 *          comp_prm[1] = pixels_per_block 
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers          
 *-------------------------------------------------------------------------*/

    FRETVAL (intf)
#ifdef PROTOTYPE
       nscscompress(intf *id, intf *comp_type, intf *comp_prm)
#else
       nscscompress( id, comp_type, comp_prm)
       intf *id;
       intf *comp_type;
       intf *comp_prm;
#endif /* PROTOTYPE */
{

    int32 sdsid;              /* SDS id               */
    comp_info c_info;         /* compression info     */
    comp_coder_t c_type;              /* compression type definition */

    int   CASE;
    intf ret;



    CASE = *comp_type;
    sdsid = *id;
    switch (CASE)  {

       case COMP_CODE_NONE:       /* No compression */
         c_type = COMP_CODE_NONE;
         break;
    
       case COMP_CODE_RLE:             /* RLE compression */
         c_type = COMP_CODE_RLE;
         break;
 
       case COMP_CODE_SKPHUFF:      /* Skipping Huffman encoding */
          c_type = COMP_CODE_SKPHUFF;
          c_info.skphuff.skp_size = comp_prm[0]; 
          break;

       case COMP_CODE_DEFLATE:      /* GZIP compression */  
          c_type = COMP_CODE_DEFLATE;
          c_info.deflate.level = comp_prm[0]; 
          break;

       case COMP_CODE_SZIP:      /* SZIP compression */  
          c_type = COMP_CODE_SZIP;
          c_info.szip.options_mask = comp_prm[0]; 
          c_info.szip.pixels_per_block = comp_prm[1]; 
          break;
       default:

          return FAIL;
                    
                     }

    ret = SDsetcompress(sdsid, c_type, &c_info);
    return(ret);

}
/*-------------------------------------------------------------------------
 * Name:    scgcompress
 * Puporse: Call SDgetcompress 
 * Inputs:  id: SDS access id
 * Outputs: comp_type:  type of compression
 *                      COMP_CODE_NONE = 0
 *                      COMP_CODE_RLE  = 1
 *                      COMP_CODE_NBIT = 2
 *                      COMP_CODE_SKPHUFF = 3
 *                      COMP_CODE_DEFLATE = 4
 *                      COMP_CODE_SZIP = 5
 *          SKPHUFF:
 *          comp_prm[0] = skphuff_skp_size: size of individual elements for 
 *                            Adaptive Huffman compression algorithm
 *          GZIP:
 *          comp_prm[0] = deflate_level:    GZIP  compression parameter
 *          NBIT:
 *          comp_prm[0] = nbit_sign_ext
 *          comp_prm[1] = nbit_fill_one
 *          comp_prm[2] = nbit_start_bit
 *          comp_prm[3] = nbit_bit_len
 *          SZIP:
 *          comp_prm[0] = options_mask             IN
 *          comp_prm[1] = pixels_per_block         IN
 *          comp_prm[2] = pixels_per_scanline      OUT
 *          comp_prm[3] = bits_per_pixel           OUT
 *          comp_prm[4] = pixels                   OUT
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers          
 *-------------------------------------------------------------------------*/

    FRETVAL (intf)
#ifdef PROTOTYPE
       nscgcompress(intf *id, intf *comp_type, intf *comp_prm)
#else
       nscgcompress( id, comp_type, comp_prm)
       intf *id;
       intf *comp_type;
       intf *comp_prm;
#endif /* PROTOTYPE */
{

    comp_info c_info;         /* compression info     */
    comp_coder_t c_type;              /* compression type definition */

    int   CASE;
    intn c_ret;
    intf ret = -1;


    c_ret = SDgetcompinfo(*id, &c_type, &c_info);
    if (c_ret == 0) {
    CASE = (int)c_type;
    switch (CASE)  {

       case COMP_CODE_NONE:       /* No compression */
         *comp_type = 0;
	  ret = 0;
         break;
    
       case COMP_CODE_RLE:             /* RLE compression */
         *comp_type = 1;
	  ret = 0;
         break;

       case COMP_CODE_NBIT:      /* NBIT encoding */
          *comp_type = 2;
          comp_prm[0] = (intf)c_info.nbit.sign_ext; 
          comp_prm[1] = (intf)c_info.nbit.fill_one; 
          comp_prm[2] = (intf)c_info.nbit.start_bit; 
          comp_prm[3] = (intf)c_info.nbit.bit_len; 
	  ret = 0;
          break;

 
       case COMP_CODE_SKPHUFF:      /* Skipping Huffman encoding */
          *comp_type = 3;
          comp_prm[0] = (intf)c_info.skphuff.skp_size; 
	  ret = 0;
          break;

       case COMP_CODE_DEFLATE:      /* GZIP compression */  
          *comp_type = 4;
          comp_prm[0] = (intf)c_info.deflate.level; 
	  ret = 0;
          break;

       case COMP_CODE_SZIP:      /* SZIP encoding */
          *comp_type = 5;
          comp_prm[0] = (intf)c_info.szip.options_mask;
          comp_prm[1] = (intf)c_info.szip.pixels_per_block;
          comp_prm[2] = (intf)c_info.szip.pixels_per_scanline; 
          comp_prm[3] = (intf)c_info.szip.bits_per_pixel; 
          comp_prm[4] = (intf)c_info.szip.pixels; 
	  ret = 0;
          break;

       default:

          return FAIL;
                    
                     } /*end CASE */
    } /* end if */
    return(ret);

}
/*-----------------------------------------------------------------------------
 * Name:    sfisrcrd
 * Purpose: call SDisrecord to see if a dataset is a record variable
 * Inputs:  id: sds id
 * Returns: TRUE/FALSE (1/0))
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfisrcrd(intf *id)
#else
nsfisrcrd(id)
     intf *id;
#endif /* PROTOTYPE */
{
    return((intf) SDisrecord(*id));
}
/*-----------------------------------------------------------------------------
 * Name:    sfsblsz
 * Purpose: call SDsetblocksize
 * Inputs:  id:          sd id
            block_size:  block size  in bytes
 * Returns: SUCCEED/FAIL (0/-1)
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nsfsblsz(intf *id, intf *block_size)
#else
nsfsblsz(id, block_size)
     intf *id;
     intf *block_size;
#endif /* PROTOTYPE */
{
    return((intf) SDsetblocksize(*id, *block_size));
}
/*-----------------------------------------------------------------------------
 * Name:    scchempty
 * Purpose: call SDcheckempty
 * Inputs:  id:          sd id
            flag:        TRUE/FALSE flag 
 * Returns: SUCCEED/FAIL (0/-1)
 *---------------------------------------------------------------------------*/
 
   FRETVAL(intf)
#ifdef PROTOTYPE
nscchempty(intf *id, intf *flag)
#else
nscchempty(id, flag)
     intf *id;
     intf *flag;
#endif /* PROTOTYPE */
{
    intn flag_c;
    intf status;
    status = SDcheckempty(*id, &flag_c);
    *flag = flag_c;
    return(status);
}   
/*-----------------------------------------------------------------------------
 * Name:    scgetfname
 * Purpose: Retrieves the name of the file given file identifier
 * Inputs:  file_id: file identifier
 * Outputs: file_name: file name 
 *          namelen: length of file name
 * Returns: real length on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscgetfname(intf *file_id, _fcd name, intf *namelen)
#else
nscgetfname(file_id, name, namelen)
     intf *file_id;
     _fcd name;
     intf *namelen;
#endif /* PROTOTYPE */

{
    char *fn;
    intn ret;
    
    fn = NULL;
    if(*namelen) fn = (char *)HDmalloc((uint32)*namelen + 1);

    ret = (intn) SDgetfilename(*file_id, fn);
    HDpackFstring(fn,  _fcdtocp(name),  *namelen);    
    
    if(fn) HDfree((VOIDP)fn);
    return(ret);
}

/*-----------------------------------------------------------------------------
 * Name:    scgetnamelen
 * Purpose: Retrieves the length of the object name
 * Inputs:  obj_id: object identifier
 * Outputs: namelen: name length
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscgetnamelen(intf *obj_id, intf *namelen)
#else
nscgetnamelen(obj_id, namelen)
     intf *obj_id;
     intf *namelen;
#endif /* PROTOTYPE */

{
    intn    ret;
    uint16  c_namelen;
    
    ret = (intn) SDgetnamelen(*obj_id, &c_namelen);

    *namelen = (intf)c_namelen; 
    return(ret);
}
/*-----------------------------------------------------------------------------
 * Name:    scidtype
 * Purpose: Retrieves type pf the object give an identifier
 * Inputs:  obj_id: object identifier
 * Outputs: obj_type: object type  -1 for invalide
 *                                  0 for file
 *                                  1 for data set
 *                                  2 for dimension scale
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscidtype(intf *obj_id, intf *obj_type)
#else
nscidtype(obj_id, obj_type)
     intf *obj_id;
     intf *obj_type;
#endif /* PROTOTYPE */

{
    intn    ret = -1;
    hdf_idtype_t  c_obj_type;;
    
    c_obj_type = SDidtype(*obj_id);

    *obj_type = (intf)c_obj_type; 
    if (c_obj_type >= 0) ret = 0;
    return(ret);
}
/*-----------------------------------------------------------------------------
 * Name:    scrmaxopenf
 * Purpose: Resets the max numebr of files can be opened at the same time
 * Inputs:  req_max: requested max number of files
 * Returns: current max number of opened files on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscrmaxopenf(intf *req_max)
#else
nscrmaxopenf(req_max)
     intf *req_max;
#endif /* PROTOTYPE */

{
    intf    cur_max;
    
    cur_max = (intf) SDreset_maxopenfiles(*req_max);
    return(cur_max);
}
/*-----------------------------------------------------------------------------
 * Name:    scgmaxopenf
 * Purpose: Retrieves current and maximum number of open files. 
 * Outputs: cat_max: current max of opened files
 *          sys_limit: system limit on open files
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscgmaxopenf(intf *cur_max, intf *sys_limit)
#else
nscgmaxopenf(cur_max, sys_limit)
     intf *cur_max;
     intf *sys_limit;
#endif /* PROTOTYPE */

{
    intf ret = 0;
    intn c_cur_max, c_sys_limit;
    
    ret = (intf) SDget_maxopenfiles(&c_cur_max, &c_sys_limit);
    if (ret < 0) return(FAIL);
    *cur_max = (intf)c_cur_max;
    *sys_limit = (intf)c_sys_limit;
    return(ret);
}
/*-----------------------------------------------------------------------------
 * Name:    scgnumopenf
 * Purpose: Returns the number of files currently being opened
 * Outputs: cur_num: number of files currently being opened
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscgnumopenf(intf *cur_num)
#else
nscgnumopenf(cur_num)
     intf *cur_num;
#endif /* PROTOTYPE */

{
    intf    ret = 0;
    intn    c_cur_num;
    
    c_cur_num = SDget_numopenfiles();
    if (c_cur_num < 1) ret = -1;
    *cur_num = (intf)c_cur_num;
    return(ret);
}
/*-----------------------------------------------------------------------------
 * Name:    scn2indices
 * Purpose: Retrieves indices of all variables with the same name. 
 * Inputs:  sd_id: SD interface identifier
 *          sds_name: data set name
 *          namelen:  length of the name
 *          n_vars:   number of variables (sizes of var_list and type_list 
 *                    arrays)
 * Outputs: var_list: list of indices
 *          type_list: list of types         
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscn2indices(intf *sd_id, _fcd name, intf *namelen, intf *var_list, intf *type_list, intf *n_vars)
#else
nscn2indices(sd_id, name, namelen, var_list, type_list, n_vars)
     intf *sd_id;
     _fcd name;
     intf *namelen;
     intf *var_list;
     intf *type_list;
     intf *n_vars;
#endif /* PROTOTYPE */

{
    char   *fn;
    intn    ret;
    hdf_varlist_t *c_var_list; 
    int idx;
    
    fn = HDf2cstring(name, *namelen);
    if (!fn) return(FAIL);
    c_var_list = (hdf_varlist_t *)HDmalloc(*n_vars * sizeof(hdf_varlist_t));
    if (!c_var_list) return(FAIL);

    ret = (intn) SDnametoindices(*sd_id, fn, c_var_list);
    
    if (ret == 0) {
	    for (idx = 0; idx < *n_vars; idx++) {
         	var_list[idx] = (intf)c_var_list[idx].var_index;
         	type_list[idx] = (intf)c_var_list[idx].var_type;
    	    }
    } /*endif*/
    HDfree(c_var_list);
    HDfree((VOIDP)fn);
    return(ret);
}
/*-----------------------------------------------------------------------------
 * Name:    scgnvars_byname
 * Purpose: Gets the number of data sets having the same name
 * Inputs:  sd_id: SD interface identifier
 *          sds_name: data set name
 *          namelen:  length of the name
 * Outputs: n_vars: number of data sets
 * Returns: 0 on success, -1 on failure 
 *---------------------------------------------------------------------------*/

   FRETVAL(intf)
#ifdef PROTOTYPE
nscgnvars_byname(intf *sd_id, _fcd name, intf *namelen, intf *n_vars)
#else
nscgnvars_byname(sd_id, name, namelen, n_vars)
     intf *sd_id;
     _fcd name;
     intf *namelen;
     intf *n_vars;
#endif /* PROTOTYPE */

{
    char   *fn;
    int32   c_n_vars;
    intn    ret;
    
    fn = HDf2cstring(name, *namelen);
    if (!fn) return(FAIL);

    ret = (intn) SDgetnumvars_byname(*sd_id, fn, &c_n_vars);

    HDfree((VOIDP)fn);
    if(ret == FAIL) return FAIL;
    *n_vars = (intf)c_n_vars;
    return(ret);
}


