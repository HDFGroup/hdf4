/*
 * This file contains the HDF/netCDF based multi-file interface for SDSs
 */

/*

Reading interface:
------------------

fid    = SDstart(file name, access);

        --- get the number of data-sets in the file ---
num    = SDnumber(fid);
        
sdsid  = SDselect(fid, i, ...);    --- 0 <= i < num ---

        --- return the name, rank, dimsizes, #of attr, datatype ---
status = SDgetinfo(sdsid, ...); 

status = SDreaddata(sdsid, ...);

status = SDgetrange(sdsid, ...);

ref    = SDgetrefnumber(sdsid);

status = SDend(fid);

Questions, should the index parameter to SDselect() be zero based or one
based?  Right now, it is zero based.

*/

#include "mfsd.h"
#include "hfile.h"

extern NC *NC_check_id() ;

/*
 * We will assume that the way hfile.c handles fids is fixed and so
 *   we can hack a structure table based on it
 */

static intn SDTbl[MAX_FILE];

#define FIDtoSDTbl(i) ((VALIDFID(i) ? &(SDTbl[(uint32)(i) & 0xffff]) : NULL))


/* ---------------------------- SDIhandle_from_id ---------------------------- */
/*

  Map an ID of type typ to a handle.  Return NULL on error

*/
PRIVATE
#ifdef PROTOTYPE
NC * SDIhandle_from_id(int32 id, intn typ)
#else
NC * SDIhandle_from_id(id, typ)
int32 id;
intn typ;
#endif
{

    NC * handle;
    int32 tmp;

    /* check that it is the proper type of id */
    tmp = (id >> 16) & 0xff;
    if(tmp != typ)
        return(NULL);
    
    /* get the file from top 8 bits*/
    tmp = (id >> 24) & 0xff;
    handle = NC_check_id(tmp);

    return(handle);

} /* SDIhandle_from_id */

/* ------------------------------ SDIget_var ------------------------------ *//*

  Given a valid handle and an sdsid find the NC_var record for this variable

*/
PRIVATE
#ifdef PROTOTYPE
NC_var * SDIget_var(NC *handle, int32 sdsid)
#else
NC_var * SDIget_var(handle, sdsid)
NC * handle;
int32 sdsid;
#endif
{

    NC_array **ap ;
    int32 varid;

    /* varid is low 16bits of sdsid */
    varid = sdsid & 0xffff;

    if(handle->vars != NULL && varid >= 0 && varid < handle->vars->count) {
        ap = (NC_array **)handle->vars->values;
        ap += varid;
    } else {
        return(NULL);
    }
    
    return((NC_var *)*ap);
    
} /* SDIget_var */


/* ------------------------------ SDIget_dim ------------------------------ *//*

  Given a valid handle and an dimid find the NC_dim record for this dimension

*/
PRIVATE
#ifdef PROTOTYPE
NC_dim * SDIget_dim(NC *handle, int32 id)
#else
NC_dim * SDIget_dim(handle, id)
NC * handle;
int32 id;
#endif
{

    NC_array **ap ;
    int32 dimid;

    /* dimid is low 16bits of id */
    dimid = id & 0xffff;

    if(handle->dims != NULL && dimid >= 0 && dimid < handle->dims->count) {
        ap = (NC_array **)handle->dims->values;
        ap += dimid;
    } else {
        return(NULL);
    }
    
    return((NC_dim *)*ap);
    
} /* SDIget_dim */


/* ------------------------------- SDstart -------------------------------- */
/*

  Do what is required to initialize the SDS interface

  Return FAIL on failure file ID otherwise.

*/
#ifdef PROTOTYPE
int32 SDstart(char *name, int32 HDFmode)
#else
int32 SDstart(name, HDFmode)
     char *name;
     int32 HDFmode;
#endif 
{

    intn   cdfid, fid;
    intn   NCmode;
    NC   * handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDstart: I've been called\n");
#endif

    if(HDFmode & DFACC_WRITE)
        NCmode = NC_RDWR;
    else
        NCmode = NC_NOWRITE;

    if(HDFmode & DFACC_CREATE) {
        cdfid = nccreate(name, NC_CLOBBER);
    } else {
        cdfid = ncopen(name, NCmode);
    }

    if(cdfid == -1) return FAIL;

    handle = NC_check_id(cdfid);
    if(handle == NULL) 
        return FAIL;
    handle->flags &= ~(NC_INDEF);

    fid = (((int32) cdfid) << 24) + (((int32) CDFTYPE) << 16) + cdfid;

    return fid;

} /* SDstart */


/* -------------------------------- SDend --------------------------------- */
/*
  Close the file
  
  Return FAIL on failure, SUCCEED otherwise

*/
#ifdef PROTOTYPE
int32 SDend(int32 id)
#else
int32 SDend(id)
int32 id;
#endif
{

    intn cdfid;
    NC   * handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDend: I've been called\n");
#endif

    cdfid = id & 0xffff;

#ifndef SYNC_ON_EACC

    /* get the handle */
    handle = SDIhandle_from_id(id, CDFTYPE);
    if(handle == NULL) 
        return FAIL;

    /* make sure we can write to the file */
    if(handle->flags & NC_RDWR) {

        handle->xdrs->x_op = XDR_ENCODE;

        /* see if the meta-data needs to be updated */
        if(handle->flags & NC_HDIRTY) {
            if(!xdr_cdf(handle->xdrs, &handle) )
                return FAIL;
            handle->flags &= ~(NC_NDIRTY | NC_HDIRTY);
        } else {

            /* see if the numrecs info needs updating */
            if(handle->flags & NC_NDIRTY) {
                if(!xdr_numrecs(handle->xdrs, handle) )
                    return FAIL;
                handle->flags &= ~(NC_NDIRTY);
            }
	}
    }

#endif /* SYNC_ON_EACC */

    return(ncclose(cdfid));

} /* SDend */


/* ------------------------------- SDnumber ------------------------------- */
/*
  Return the number of SDS-type objects and global attributes in the file

  Return FAIL on error 

*/
#ifdef PROTOTYPE
int32 SDfileinfo(int32 fid, int32 *datasets, int32 *attrs)
#else
int32 SDfileinfo(fid, datasets, attrs)
int32   fid;
int32 * datasets;
int32 * attrs;
#endif
{

    NC * handle;
    intn cdfid;

#ifdef SDDEBUG
    fprintf(stderr, "SDnumber: I've been called\n");
#endif

    /* check that fid is valid */
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL)
        return(FAIL);

#ifdef SDDEBUG
    fprintf(stderr, "SDnumber: looked up handle as %d\n", handle);
#endif
    
    *datasets = ((handle->vars != NULL) ? handle->vars->count : 0);
    *attrs    = ((handle->attrs != NULL) ? handle->attrs->count : 0);

    return SUCCEED;

} /* SDfileinfo */

/* ------------------------------- SDselect ------------------------------- */
/*
  
  Return an id for the index-th data set in the file

  How are we going to construct SDS ids?  We've got 32bits, the current
     plan is

     sdsID:

     32       24       16               0
     ------------------------------------
     |  fid   | id-type| position index |
     ------------------------------------

     Since an fid is 32bits and we need to shove it down to 8bits we
  need to hack it.  In hfile.h it is defined that there can be at most
  MAX_FILE objects open at a time.  Rip that part of the fid out and
  store it in the sdsID when we need an fid we'll have to rebuild it
  from the info we have stored.  UGLY.

  The other option would be to have a table of pointers to vars and then
  the sdsID would just be an index into that table (and we'd have to
  make sure all of the necessary info was available from the table
  entry).

  Maybe it would be better to store the netCDF file id rather than the 
  HDF one -- it would then be easier to map to the handle where all
  of the action is anyway.

*/
#ifdef PROTOTYPE
int32 SDselect(int32 fid, intn index)
#else
int32 SDselect(fid, index)
int32 fid;
intn  index;
#endif
{
    NC    *handle;
    int32 sdsid;         /* the id we're gonna build */

#ifdef SDDEBUG
    fprintf(stderr, "SDselect: I've been called (index: %d) \n", index);
#endif

    /* check that fid is valid */
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL)
        return(FAIL);

    /* check that a data set with this index exists */
    if(handle->vars == NULL)
        return(FAIL);

    if(handle->vars->count < index)
        return(FAIL);

    sdsid  = (((int32) fid & 0xffff) << 24) + (((int32) SDSTYPE) << 16) + index;

    return(sdsid);

} /* SDselect */

/* ------------------------------ SDgetinfo ------------------------------- */
/*

  Return basic information about a dataset (name, rank, dimsizes, number
    of attributes, number type, etc...)

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetinfo(int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
                int32 *nt, int32 *nattr)
#else
int32 SDgetinfo(sdsid, name, rank, dimsizes, nt, nattr)
int32 sdsid;
char  *name;
int32 *rank, *nt, *nattr, *dimsizes;
#endif
{

    NC     * handle;
    NC_var * var;
    intn     i;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetinfo: I've been called\n");
#endif

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    if(name != NULL) {
        HDstrncpy(name, var->name->values, var->name->count);
        name[var->name->count] = '\0';
    }

    *rank  = var->assoc->count;
    *nt    = hdf_map_type(var->type);

    *nattr = (var->attrs ? var->attrs->count : 0);

    for(i = 0; i < *rank; i++)
        dimsizes[i] = (int32) var->shape[i];

    if(dimsizes[0] == NC_UNLIMITED) {
        if(handle->is_hdf)
            dimsizes[0] = var->numrecs;
        else
            dimsizes[0] = handle->numrecs;
    }

#ifdef SDDEBUG
    fprintf(stderr, "SDgetinfo: looked up handle as %d\n", handle);
#endif

    return(SUCCEED);

} /* SDgetinfo */

/* ------------------------------ SDreaddata ------------------------------- */
/*

  Read some data out of the given sdsID

  Return SUCCEED else FAIL

*/
#ifdef PROTOTYPE
int32 SDreaddata(int32 sdsid, int32 *start, int32 *stride, int32 *end, Void *data)
#else
int32 SDreaddata(sdsid, start, stride, end, data)
int32 sdsid;
int32 *start, *stride, *end;
Void  *data;
#endif
{

    NC     * handle;
    int32    varid, status;
#ifdef BIG_LONGS
    long     Start[MAX_NC_DIMS], End[MAX_NC_DIMS], Stride[MAX_NC_DIMS];
#else 
    int32    *Start, *End, *Stride;
#endif

#ifdef SDDEBUG
    fprintf(stderr, "SDreaddata: I've been called\n");
#endif
    
    if((start == NULL) || (end == NULL) || (data == NULL))
        return FAIL;

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    /* get ready to read */
    handle->xdrs->x_op = XDR_DECODE ;
    
    /* oops, how do we know this ? */
    varid = sdsid & 0xffff;

    /*
     * In general, (long) == int32 
     * In cases where it doesn't we need to convert
     */
#ifdef BIG_LONGS

    {
        int i;
        NC_var * var = SDIget_var(handle, sdsid);
        if(var == NULL)
            return FAIL;
        
        for(i = 0; i < var->assoc->count; i++) {
            Start[i]  = (long) start[i];
            End[i]    = (long) end[i];
            if(stride) Stride[i] = (long) stride[i];
        }
    }

#else

    Start  = start;
    End    = end;
    Stride = stride;

#endif

    /* call the readg routines if a stride is given */
    if(stride == NULL)
        status = NCvario(handle, varid, Start, End, (Void *)data);
    else
        status = NCgenio(handle, varid, Start, End, Stride, NULL, (VOID *)data);

    if(status == -1)
        return FAIL;
    else
        return SUCCEED;
            
} /* SDreaddata */

/* ---------------------------- SDgetrefnumber ---------------------------- */
/*

  Given an sdsID return its reference number.  Return 0 on error.

*/
uint16
#ifdef PROTOTYPE
SDgerefnumber(int32 sdsid)
#else
SDgetrefnumber(sdsid)
int32 sdsid;
#endif
{

    NC     * handle;
    NC_var * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetrefnumber: I've been called\n");
#endif

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return 0;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return 0;

    return ((uint16) var->data_ref);

} /* SDgetrefnumber */

/* ---------------------------- SDnametoindex ----------------------------- */
/*

  Given a data set name return the index (not ID) of the first data set
  whose name matches.

  As of this writing, there could be multiple data sets with the same name

  Return FAIL on error

*/
int32
#ifdef PROTOTYPE
SDnametoindex(int32 fid, char *name)
#else
SDnametoindex(fid, name)
int32 fid;
char  *name;
#endif
{

    intn     ii, len;
    NC     * handle;
    NC_var ** dp;

#ifdef SDDEBUG
    fprintf(stderr, "SDnametoindex: I've been called\n");
#endif

    /* check that fid is valid */
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    len = HDstrlen(name) ;
    dp = (NC_var**)handle->vars->values ;
    for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
        if( len == (*dp)->name->count &&
           HDstrncmp(name, (*dp)->name->values, (*dp)->name->count) == 0) {
            return(ii) ;
        }
    }

    return(FAIL) ;
    
} /* SDnametoindex */



/* ------------------------------ SDgetrange ------------------------------ */

/*

  Simulate a call to DFSDgetrange()

  If a "valid_range" attribute is provided return its values in pmax and 
  pmin.  Else if both a "valid max" AND a "vaild min" exist return their 
  values in pmax and pmin.

  Arrgghh, in HDF it was assumed that the max and min values were of the 
  same data type as the rest of the data.  So the user would know the
  amount of storage to send to get back the max and min values.  This is 
  not the case with netCDF.  I think for the "range" calls we will cast the
  values over to the type of the data set.  If people want the full 
  resolution, they should use the getattr() interface.

  Actually, that gets really gross.  For now, things will only work if
  they are already the same number types.

  On error or missing attributes return FAIL else SUCCEED.

*/
intn
#ifdef PROTOTYPE
SDgetrange(int32 sdsid, Void *pmax, Void *pmin)
#else
SDgetrange(sdsid, pmax, pmin)
int32 sdsid;
Void *pmax, *pmin;
#endif
{
    NC      * handle;
    NC_var  * var;
    NC_attr ** attr, ** attr1, ** attr2;
    NC_array * array;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetrange: I've been called\n");
#endif

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    attr = (NC_attr **) NC_findattr(&(var->attrs), "valid_range");
    if((attr != NULL) && ((*attr)->data->type == var->type)) {

        /* BUG: this may be a pointer to a pointer */
        array = (NC_array *) (*attr)->data;
        HDmemcpy(pmin, array->values, array->szof);
        HDmemcpy(pmax, array->values + array->szof, array->szof);

    } else {

        attr1 = (NC_attr **) NC_findattr(&(var->attrs), "valid_max");
        attr2 = (NC_attr **) NC_findattr(&(var->attrs), "valid_min");
        
        if((attr1 == NULL) || (attr2 == NULL)) {
#ifdef SDDEBUG
            fprintf(stderr, "No dice on range info (missing at least one)\n");
#endif   
            return FAIL;
        }

        if(((*attr1)->data->type != var->type) || 
           ((*attr2)->data->type != var->type)) {
#ifdef SDDEBUG
            fprintf(stderr, "No dice on range info (wrong types)\n");
#endif   
            return FAIL;
        }

        NC_copy_arrayvals((char *)pmax, (*attr1)->data) ;
        NC_copy_arrayvals((char *)pmin, (*attr2)->data) ;

    }
    
    return SUCCEED;

} /* SDgetrange */


/* -------------------------- CREATION INTERFACE -------------------------- */
/*

        --- create a new data set ---
sdsid   = SDcreate(fid, name, numbertype, rank, dimsizes);  

        --- associate a name with a dimension.  If a prev sdsid is ---
        --- provided then it is assumed that the current dimension is the ---
        --- same as the dimension with the same name of the previous sds ---
status  = SDsetdim(sdsid, dimnumber, dimname, [prev sdsid] );

        --- note that it will be possible to store values for a ---
        --- dimension without having to name it ---
status  = SDsetdimvalues(sdsid, dimnumber, numbertype, count, data);
        
        --- set the units and format strings ---
status  = SDsetdimstrs(sdsid, dimnumber, unitstr, formatstr);

        --- we will need an SDendaccess() so that we know when it ---
        --- is safe to write the information out in this case ---
status  = SDendaccess(sdsid);

*/

/* ------------------------------- SDcreate ------------------------------- */
/*

  Simulate a call to ncvardef without having to be in define mode
  
  It looks like for the call to NC_new_var() we need to have dimension
  IDs already.  So I guess we should just create the fake dimensions
  now and when optional information comes in (i.e.  name, tying
  to other dimensions) we'll go in and modify the structure in place.

  This is gonna be heinous.  Please do not attempt this at home

  Return FAIL if things don't work

*/
int32
#ifdef PROTOTYPE
SDcreate(int32 fid, char *name, int32 nt, int32 rank, int32 *dimsizes)
#else
SDcreate(fid, name, nt, rank, dimsizes)
int32 fid;
char  *name;
int32 nt, rank, *dimsizes;
#endif
{
    NC      * handle;
    NC_var  * var;
    NC_dim  * newdim;
    int32     sdsid, nctype;
    char      dimname[80];
    intn      i, num;
    intn    * dims;

#ifdef SDDEBUG
    fprintf(stderr, "SDcreate: I've been called\n");
#endif

    /* check that fid is valid */
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL)
        return(FAIL);

    /* fudge the name since its optional */
    if(name == NULL)
        name = "DataSet";

    /* make fake dimensions which may or may not be over-ridden later */
    dims = (intn *) HDgetspace(rank * sizeof(intn));
    if(dims == NULL)
        return FAIL;

    for(i = 0; i < rank; i++) {

        num = (handle->dims ? handle->dims->count : 0);
        sprintf(dimname, "fakeDim%d", num);

        newdim = (NC_dim *) NC_new_dim(dimname, dimsizes[i]); 
        if(newdim == NULL) return FAIL;
        
        if(handle->dims == NULL) { /* first time */
            handle->dims = NC_new_array(NC_DIMENSION,(unsigned)1, (Void *)&newdim);
            if(handle->dims == NULL)
                return FAIL;
	} else if(handle->dims->count >= MAX_NC_DIMS) {
            return FAIL;
	} else {
            if( NC_incr_array(handle->dims, (Void *)&newdim) == NULL)
                return FAIL;
	}

        dims[i] = (intn) handle->dims->count -1;

    }
    
    /* create the actual variable */
    nctype = hdf_unmap_type(nt);
    var = (NC_var *) NC_new_var(name, nctype, rank, dims);
    if(var == NULL)
        return FAIL;
    
    /* add it to the handle */
    if(handle->vars == NULL) { /* first time */
        handle->vars = NC_new_array(NC_VARIABLE,(unsigned)1, (Void *)&var);
        if(handle->vars == NULL)
            return FAIL;
    } else {
        if(handle->vars->count >= MAX_NC_VARS) {
            return FAIL;
        } else {
            if( NC_incr_array(handle->vars, (Void *)&var) == NULL)
                return FAIL;
	}
    }

    /* compute all of the shape information */
    if(NC_var_shape(var, handle->dims) == -1)
        return FAIL;
    
    /* create a handle we can give back to the user */
    sdsid  = (((int32) fid) << 24) + (((int32) SDSTYPE) << 16);
    sdsid += handle->vars->count -1;

    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return sdsid;

} /* SDcreate */


/* ------------------------------ SDgetdimid ------------------------------ */
/*

  Given an sdsid and a dimension number return a dimid.  For consistancy
  dimension numbering begins at 1 and not 0

  The dimID looks similar to the sdsID except DIMTYPE is substituted for 
  SDSTYPE as the id-type:

    dimID:

     32       24       16               0
     ------------------------------------
     |  fid   | id-type| position index |
     ------------------------------------

  Return FAIL if there is a problem

*/
int32
#ifdef PROTOTYPE
SDgetdimid(int32 sdsid, intn number)
#else
SDgetdimid(sdsid, number)
int32 sdsid;
intn  number;
#endif
{
    NC      * handle;
    NC_var  * var;
    int32     id, dimindex;


#ifdef SDDEBUG
    fprintf(stderr, "SDgetdimid: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the variable */
    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    /* For the sake of fortran the user selects dims starting at 1 */
/*    number--; */

    /* check if enough / too many dims */
    if((var->assoc == NULL) || (var->assoc->count < number))
        return FAIL;

    /* get the dim number out of the assoc array */
    dimindex = var->assoc->values[number];

    /* build the dim id */
    id  = (sdsid & 0xff000000) + (((int32) DIMTYPE) << 16) + dimindex;

    return(id);

} /* SDgetdimid */

/* ----------------------------- SDsetdimname ----------------------------- */
/*

  Set the name and (later) connectivity of a dimension.  If this name is
  already in use we should point to the existing dimension with that
  name.  If the sizes are different return an error.

  IGNORE connections for now
  
  Return SUCCEED or FAIL

*/
int32
#ifdef PROTOTYPE
SDsetdimname(int32 id, char *name)
#else
SDsetdimname(id, name)
int32   id;
char  * name;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    NC_dim   ** dp;
    NC_string * old, * new ;
    NC_array ** ap;
    int32       len, ii;
	
#ifdef SDDEBUG
    fprintf(stderr, "SDsetdimname: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the dimension structure */
    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;

    /* check for name in use */
    len = HDstrlen(name) ;
    dp = (NC_dim**)handle->dims->values ;
    for(ii = 0 ; ii < handle->dims->count ; ii++, dp++) {
        if( len == (*dp)->name->count &&
           HDstrncmp(name, (*dp)->name->values, (*dp)->name->count) == 0) {
            if(dim != (*dp)) {
                /* a dimension with this name already exists */
                /* so change to point to it */
                if(dim->size != (*dp)->size) return FAIL;
                ap = (NC_array **) handle->dims->values;
                ap += id & 0xffff;
                (*ap) = (NC_array *) (*dp);
                return SUCCEED;
            }
        }
    }
    
    /* throw out the old name if it exists and create a new one */
    old = dim->name;
    new = NC_new_string((unsigned)HDstrlen(name),name);
    if(new == NULL)
        return FAIL;
    dim->name = new;
    NC_free_string(old);
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdimname */

/* ----------------------------- SDendaccess ------------------------------ */
/*

  Close down this access ID to a data object

  Usually, this will do nothing.  However, if the meta-data has changed 
  flush it all out to disk.

  Return FAIL on error else SUCCEED

*/
int32
#ifdef PROTOTYPE
SDendaccess(int32 id)
#else
SDendaccess(id)
int32   id;
#endif
{

    NC        * handle;
	
#ifdef SDDEBUG
    fprintf(stderr, "SDendaccess: I've been called\n");
#endif

#ifdef SYNC_ON_EACC

    /* get the handle */
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    /* make sure we can write to the file */
    if(handle->flags & NC_RDWR) {

        handle->xdrs->x_op = XDR_ENCODE;

        /* see if the meta-data needs to be updated */
        if(handle->flags & NC_HDIRTY) {
            if(!xdr_cdf(handle->xdrs, &handle) )
                return FAIL;
            handle->flags &= ~(NC_NDIRTY | NC_HDIRTY);
        } else {

            /* see if the numrecs info needs updating */
            if(handle->flags & NC_NDIRTY) {
                if(!xdr_numrecs(handle->xdrs, handle) )
                    return FAIL;
                handle->flags &= ~(NC_NDIRTY);
            }
	}
    }

#endif /* SYNC_ON_EACC */
        
    return SUCCEED;
    
} /* SDendaccess */

/* ------------------------------ SDIputattr ------------------------------ */
/*

  Common code for adding an attribute to an attribute list

  return FAIL on error else SUCCEED

*/
intn
#ifdef PROTOTYPE
SDIputattr(NC_array **ap, char *name, int32 nt, intn count, VOIDP data)
#else
SDIputattr(ap, name, nt, count, data)
NC_array **ap;
char     *name;
int32    nt;
intn     count;
VOIDP    data;
#endif
{
    
    NC_attr *attr;
    NC_attr **atp, *old;

#ifdef SDDEBUG
    fprintf(stderr, "SDIputattr: I've been called\n");
#endif

    if(*ap == NULL ) { /* first time */
        attr = (NC_attr *) NC_new_attr(name,nt,count,data) ;
        if(attr == NULL)
            return FAIL;
        *ap = NC_new_array(NC_ATTRIBUTE,(unsigned)1, (Void*)&attr) ;
        if(*ap == NULL)
            return FAIL;
        return SUCCEED;
    }

    if((atp = NC_findattr(ap, name)) != NULL) { /* name in use */
        old = *atp ;
        *atp = (NC_attr *) NC_new_attr(name,nt,count,data);
        if(*atp == NULL) {
            *atp = old;
            return FAIL;
        }
        NC_free_attr(old);
        return SUCCEED;
    }
 
    if((*ap)->count >= MAX_NC_ATTRS) {  /* Too many */
        return FAIL;
    }

    /* just add it */
    attr = (NC_attr *) NC_new_attr(name,nt,count,data);
    if(attr == NULL)
        return FAIL;
    if(NC_incr_array((*ap), (Void *)&attr) == NULL)
        return FAIL;
    return SUCCEED;
    
} /* SDIputattr */


/* ------------------------------ SDsetrange ------------------------------ */
/*

  Store range info for this variable in the valid_range attribute.  If that
  attribute already exists overwrite the current values.  It is assumed
  that the values are the same type as the data set.

  Return FAIL on error else SUCCEED

*/

intn
#ifdef PROTOTYPE
SDsetrange(int32 sdsid, Void *pmax, Void *pmin)
#else
SDsetrange(sdsid, pmax, pmin)
int32 sdsid;
Void *pmax, *pmin;
#endif
{
    NC      * handle;
    NC_var  * var;
    uint8     data[80];
    intn      sz;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetrange: I've been called\n");
#endif

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    if((pmax == NULL) || (pmin == NULL))
        return FAIL;

    /* move data values over */
    sz = DFKNTsize(var->HDFtype);
    HDmemcpy(data, pmin, sz);
    HDmemcpy(data + sz, pmax, sz);

    /* call common code */
    if(SDIputattr(&var->attrs, "valid_range", var->type, (intn) 2, 
                  (VOIDP) data) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;
    
} /* SDsetrange */

/* ------------------------------ SDsetattr ------------------------------- */
/*

  Given an ID and an attribute defintion attach the atrribute to the thing
  represented by the ID.  For starters, the valid IDs could either be 
  variable or file IDs

  Return FAIL on error else SUCCEED

*/
intn
#ifdef PROTOTYPE
SDsetattr(int32 id, char *name, int32 nt, intn count, Void *data)
#else
SDsetattr(id, name, nt, count, data)
int32 id;
char  *name;
int32 nt;
intn  count;
Void  *data;
#endif
{ 

    NC_array **ap;
    NC_var   * var;
    NC       * handle;
    int32      cdfid;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetattr: I've been called\n");
#endif

    /* sanity check args */
    if(name == NULL) 
        return FAIL;

    /* determine what type of ID we've been given */
    handle = NULL;
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle != NULL) { /* was a variable ID */
        var = SDIget_var(handle, id);
        if(var == NULL)
            return FAIL;
        ap = &(var->attrs);
    } else {
        /* see if its a fidle ID */
        handle = SDIhandle_from_id(id, CDFTYPE);
        if(handle == NULL)
            return FAIL;
        ap = &(handle->attrs);
    }

    /* still no handle ? */
    if(handle == NULL)
        return FAIL;
    
    /* hand over to SDIputattr */
    if(SDIputattr(ap, name, hdf_unmap_type(nt), count, data) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;
    
} /* SDsetattr */


/* ------------------------------ SDattrinfo ------------------------------- */
/*

  Inquire about an attribute.

  Given the ID of the attribute's parent and the attribute's index return
  the number type, name and count of the attribute so the user knows 
  how much space to provide to read it

  Returns FAIL on error else SUCCEED

*/
intn
#ifdef PROTOTYPE
SDattrinfo(int32 id, int32 index, char *name, int32 *nt, intn *count)
#else
SDattrinfo(id, index, name, nt, count)
int32 id, index;
char  *name;
int32 *nt;
intn  *count;
#endif
{

    NC_array *  ap;
    NC_attr  **atp;
    NC_var   * var;
    NC       * handle;
    int32      cdfid;

#ifdef SDDEBUG
    fprintf(stderr, "SDattrinfo: I've been called\n");
#endif

    /* sanity check args */
    if((name == NULL) || (nt == NULL) || (count == NULL))
        return FAIL;

    /* determine what type of ID we've been given */
    handle = NULL;
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle != NULL) { /* was a variable ID */
        var = SDIget_var(handle, id);
        if(var == NULL)
            return FAIL;
        ap = var->attrs;
    } else {
        /* see if its a file ID */
        handle = SDIhandle_from_id(id, CDFTYPE);
        if(handle == NULL)
            return FAIL;
        ap = handle->attrs;
    }

    if((ap == NULL) || (index >= ap->count))
        return FAIL;

    /* 
     * ap is the proper attribute list now look up something with this
     *  name
     */
    atp = (NC_attr **) ((char *)ap->values + index * ap->szof);

    if(*atp == NULL) 
        return FAIL;

    /* move the information over */
    if(name != NULL) {
        HDstrncpy(name, (*atp)->name->values, (*atp)->name->count);
        name[(*atp)->name->count] = '\0';
    }

    *count = (*atp)->data->count;
    *nt    = hdf_map_type((*atp)->data->type);

    return SUCCEED;

} /* SDattrinfo */


/* ------------------------------ SDreadattr ------------------------------ */
/*
  read the actual contents of the given attribute

  assume that the user has called SDinqattr() and so has allocated
  sufficient space

  return FAIL on error else SUCCEED
*/

intn
#ifdef PROTOTYPE
SDreadattr(int32 id, int32 index, Void *buf)
#else
SDreadattr(id, index, buf)
int32 id, index;
Void *buf;
#endif
{

    NC_array *  ap;
    NC_attr  **atp;
    NC_var   * var;
    NC       * handle;
    int32    * cdfid;

#ifdef SDDEBUG
    fprintf(stderr, "SDreadattr: I've been called\n");
#endif

    /* sanity check args */
    if(buf == NULL)
        return FAIL;
    
    /* determine what type of ID we've been given */
    handle = NULL;
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle != NULL) { /* was a variable ID */
        var = SDIget_var(handle, id);
        if(var == NULL)
            return FAIL;
        ap = var->attrs;
    } else {
        /* see if its a fidle ID */
        handle = SDIhandle_from_id(id, CDFTYPE);
        if(handle == NULL)
            return FAIL;
        ap = handle->attrs;
    }

    if((ap == NULL) || (index >= ap->count))
        return FAIL;

    /* 
     * ap is the proper attribute list now look up something with this
     *  name
     */
    atp = (NC_attr **) ((char *)ap->values + index * ap->szof);

    if(*atp == NULL) 
        return FAIL;

    /* move the information over */
    HDmemcpy(buf, (*atp)->data->values, 
             (*atp)->data->count * (*atp)->data->szof);

    return SUCCEED;

} /* SDreadattr */


/* ----------------------------- SDwritedata ------------------------------ */
/*

  Write out a chunk o data.  Except for the line setting the XDR op-code
  this is exactly the same as SDreaddata().  The two routines should
  really be combined at some point

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDwritedata(int32 sdsid, int32 *start, int32 *stride, int32 *end, Void *data)
#else
int32 SDwritedata(sdsid, start, stride, end, data)
int32 sdsid;
int32 *start, *stride, *end;
Void  *data;
#endif
{

    NC     * handle;
    int32    varid, status;
#ifdef BIG_LONGS
    long     Start[MAX_NC_DIMS], End[MAX_NC_DIMS], Stride[MAX_NC_DIMS];
#else 
    int32    *Start, *End, *Stride;
#endif

#ifdef SDDEBUG
    fprintf(stderr, "SDwritedata: I've been called\n");
#endif
    
    if((start == NULL) || (end == NULL) || (data == NULL))
        return FAIL;

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    /* get ready to read */
    handle->xdrs->x_op = XDR_ENCODE;
    
    /* oops, how do we know this ? */
    varid = sdsid & 0xffff;

    /*
     * In general, (long) == int32 
     * In cases where it doesn't we need to convert
     */
#ifdef BIG_LONGS

    {
        int i;
        NC_var * var = SDIget_var(handle, sdsid);
        if(var == NULL)
            return FAIL;
        
        for(i = 0; i < var->assoc->count; i++) {
            Start[i]  = (long) start[i];
            End[i]    = (long) end[i];
            if(stride) Stride[i] = (long) stride[i];
        }
    }

#else

    Start  = start;
    End    = end;
    Stride = Stride;

#endif

    /* call the writeg routines if a stride is given */
    if(stride == NULL)
        status = NCvario(handle, varid, Start, End, (Void *)data);
    else
        status = NCgenio(handle, varid, Start, End, Stride, NULL, (VOID *)data);

    if(status == -1)
        return FAIL;
    else
        return SUCCEED;
            
} /* SDwritedata */

/* ---------------------------- SDsetdatastrs ----------------------------- */
/*

  Store information about the 'label', 'units', 'format' and 'cordsys' 
  attributes of a dataset.  All three of the values are optional.  If
  no string is desired NULL should be passed in its place.

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDsetdatastrs(int32 sdsid, char *l, char *u, char *f, char *c)
#else
int32 SDsetdatastrs(sdsid, l, u, f, c)
int32 sdsid;
char  *l, *u, *f, *c;
#endif
{

    NC     * handle;
    NC_var * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetdatastrs: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;
    
    if(l) 
        if(SDIputattr(&var->attrs, "long_name", NC_CHAR, 
                      (intn) HDstrlen(l), l) == FAIL)
            return FAIL;

    if(u) 
        if(SDIputattr(&var->attrs, "units", NC_CHAR, 
                      (intn) HDstrlen(u), u) == FAIL)
            return FAIL;

    if(f) 
        if(SDIputattr(&var->attrs, "format", NC_CHAR, 
                      (intn) HDstrlen(f), f) == FAIL)
            return FAIL;
    
    if(c) 
        if(SDIputattr(&var->attrs, "cordsys", NC_CHAR, 
                      (intn) HDstrlen(c), c) == FAIL)
            return FAIL;
    
    /* make sure it gets reflected in the file */
    if(l || u || f || c)
        handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdatastrs */

/* ------------------------------- SDsetcal ------------------------------- */
/*

  Store calibration information

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDsetcal(int32 sdsid, float64 cal, float64 cale, float64 ioff, float64 ioffe, int32 nt)
#else
int32 SDsetcal(sdsid, cal, cale, ioff, ioffe, nt)
int32 sdsid;
float64 cal, cale, ioff, ioffe;
int32 nt;
#endif
{

    NC     * handle;
    NC_var * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetcal: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;
    
    if(SDIputattr(&var->attrs, "scale_factor", NC_DOUBLE, 
                  (intn) 1, (VOIDP) &cal) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "scale_factor_err", NC_DOUBLE, 
                  (intn) 1, (VOIDP) &cale) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "add_offset", NC_DOUBLE, 
                  (intn) 1, (VOIDP) &ioff) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "add_offset_err", NC_DOUBLE, 
                  (intn) 1, (VOIDP) &ioffe) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "calibrated_nt", NC_LONG, 
                  (intn) 1, (VOIDP) &nt) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetcal */


/* --------------------------- SDsetfillvalue ----------------------------- */
/*

  Set the fill value information.  It is assumed that the fillvalue is
  of the same data type as the variable

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDsetfillvalue(int32 sdsid, Void *val)
#else
int32 SDsetfillvalue(sdsid, val)
int32 sdsid;
Void *val;
#endif
{

    NC     * handle;
    NC_var * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetfillvalue: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;
    
    if(SDIputattr(&var->attrs, _FillValue, var->type, 
                  (intn) 1, val) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetfillvalue */



/* --------------------------- SDgetfillvalue ----------------------------- */
/*

  Get the fill value information.  It is assumed that the fillvalue is
  of the same data type as the variable

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetfillvalue(int32 sdsid, Void *val)
#else
int32 SDgetfillvalue(sdsid, val)
int32 sdsid;
Void *val;
#endif
{

    NC       * handle;
    NC_var   * var;
    NC_attr ** attr;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetfillvalue: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    attr = (NC_attr **) NC_findattr(&(var->attrs), _FillValue);
    if(attr == NULL)
        return FAIL;

    NC_copy_arrayvals((char *)val, (*attr)->data) ;    
    
    return SUCCEED;

} /* SDgetfillvalue */

/* ---------------------------- SDgetdatastrs ----------------------------- */
/*

   Read as many of the data strings as possible.  Assume that if a pointer is
   not NULL that we have len bytes that we can use to return the values

   Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetdatastrs(int32 sdsid, char *l, char *u, char *f, char *c, intn len)
#else
int32 SDgetdatastrs(sdsid, l, u, f, c, len)
int32 sdsid;
char *l, *u, *f, *c;
intn len;
#endif
{

    NC       * handle;
    NC_var   * var;
    NC_attr ** attr;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetdatastrs: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    if(l) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "long_name");
        if(attr != NULL) {
            HDstrncpy((char *)l, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                l[(*attr)->data->count] = '\0';
        } else {
            l[0] = '\0';
        }
    }
    if(u) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "units");
        if(attr != NULL) {
            HDstrncpy((char *)u, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                u[(*attr)->data->count] = '\0';
        } else {
            u[0] = '\0';
        }
    }
    if(f) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "format");
        if(attr != NULL) {
            HDstrncpy((char *)f, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                f[(*attr)->data->count] = '\0';
        } else {
            f[0] = '\0';
        }
    }
    if(c) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "cordsys");
        if(attr != NULL) {
            HDstrncpy((char *)c, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                c[(*attr)->data->count] = '\0';
        } else {
            c[0] = '\0';
        }
    }
        
    return SUCCEED;

} /* SDgetdatastrs */

/* ------------------------------- SDgetcal ------------------------------- */
/*

  Read in the calibration information 

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetcal(int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, int32 *nt)
#else
int32 SDgetcal(sdsid, cal, cale, ioff, ioffe, nt)
int32    sdsid;
float64  *cal, *cale, *ioff, *ioffe;
int32    *nt;
#endif
{

    NC       * handle;
    NC_var   * var;
    NC_attr ** attr;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetcal: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    attr = (NC_attr **) NC_findattr(&(var->attrs), "scale_factor");
    if(attr == NULL)
        return FAIL;
    NC_copy_arrayvals((char *)cal, (*attr)->data) ;    

    attr = (NC_attr **) NC_findattr(&(var->attrs), "scale_factor_err");
    if(attr == NULL)
        return FAIL;
    NC_copy_arrayvals((char *)cale, (*attr)->data) ;    

    attr = (NC_attr **) NC_findattr(&(var->attrs), "add_offset");
    if(attr == NULL)
        return FAIL;
    NC_copy_arrayvals((char *)ioff, (*attr)->data) ;    

    attr = (NC_attr **) NC_findattr(&(var->attrs), "add_offset_err");
    if(attr == NULL)
        return FAIL;
    NC_copy_arrayvals((char *)ioffe, (*attr)->data) ;    

    attr = (NC_attr **) NC_findattr(&(var->attrs), "calibrated_nt");
    if(attr == NULL)
        return FAIL;
    NC_copy_arrayvals((char *)nt, (*attr)->data) ;    
        
    return SUCCEED;

} /* SDgetcal */

/* ---------------------------- SDIgetcoordvar ---------------------------- */
/*
  
  Given a dimension return the index to the coordinate variable for it ---
  creating one if needed.  If we need to create a variable and an nt
  is not supplied (i.e. is equal to zero) use 32bit floats.

  If the variable already exists and the existing nt is different
  from the supplied one (and the supplied one is not zero) replace
  the nt by the new one.  ID is needed so that we can set the 
  dimension of the variable correctly if we need to.  Yuck.

  Return FAIL on error

*/
#ifdef PROTOTYPE
int32 SDIgetcoordvar(NC *handle, NC_dim *dim, int32 id, int32 nt)
#else
int32 SDIgetcoordvar(handle, dim, id, nt)
NC     * handle;
NC_dim * dim;
int32    id, nt;
#endif
{ 

    NC_string * name;
    int32       ii, nctype, len;
    NC_var   ** dp, *var;
    intn        dimindex;

    /* look for a variable with the same name */
    name = dim->name;
    len = dim->name->count;
    dp = (NC_var**)handle->vars->values ;
    for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
        if( len == (*dp)->name->count &&
           HDstrncmp(name->values, (*dp)->name->values, 
                     (*dp)->name->count) == 0) {

            /* see if we need to change the number type */
            
            if((nt != 0) && (nt != (*dp)->type)) {
#ifdef SDDEBUG
                fprintf(stderr, "SDIgetcoordvar redefining type\n");
#endif
                (*dp)->type = hdf_unmap_type(nt);
                (*dp)->HDFtype = nt;
                
                /* recompute all of the shape information */
                /* BUG: this may be a memory leak ??? */
                if(NC_var_shape((*dp), handle->dims) == -1)
                    return FAIL;
            }
            return ii;
        }
    }

    /* create a new var with this dim as only coord */
    if(nt == 0) nt = DFNT_FLOAT32;

    nctype = hdf_unmap_type(nt);
    dimindex = id;
    var = (NC_var *) NC_new_var(name->values, nctype, (unsigned)1, &dimindex);
    if(var == NULL)
        return FAIL;
    
    /* add it to the handle */
    if(handle->vars->count >= MAX_NC_VARS)
        return FAIL;
        
    /* compute all of the shape information */
    if(NC_var_shape(var, handle->dims) == -1)
        return FAIL;
    
    if(NC_incr_array(handle->vars, (Void *)&var) == NULL)
        return FAIL;
    
    return(handle->vars->count -1);

} /* SDIgetcoordvar */

/* ----------------------------- SDsetdimstrs ----------------------------- */
/*

  Store information about the 'label', 'units' and 'format' attributes of a 
  dimension.  All three of the values are optional.  If no string is desired 
  NULL should be passed in its place.

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDsetdimstrs(int32 id, char *l, char *u, char *f)
#else
int32 SDsetdimstrs(id, l, u, f)
int32 id;
char  *l, *u, *f;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    int32       varid;
    NC_var    * var;
	
#ifdef SDDEBUG
    fprintf(stderr, "SDsetdimstrs: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the dimension structure */
    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;
    
    /* look for a variable with the same name */
    varid = SDIgetcoordvar(handle, dim, id & 0xffff, 0);
    if(varid == NULL)
        return FAIL;

    /* get the variable object */
    var = NC_hlookupvar(handle, varid);
    if(var == NULL)
        return FAIL;

    /* set the attributes */
    if(l) 
        if(SDIputattr(&var->attrs, "long_name", NC_CHAR, 
                      (intn) HDstrlen(l), l) == FAIL)
            return FAIL;

    if(u) 
        if(SDIputattr(&var->attrs, "units", NC_CHAR, 
                      (intn) HDstrlen(u), u) == FAIL)
            return FAIL;

    if(f) 
        if(SDIputattr(&var->attrs, "format", NC_CHAR, 
                      (intn) HDstrlen(f), f) == FAIL)
            return FAIL;

    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdimstrs */

/* ---------------------------- SDsetdimscale ----------------------------- */
/*

  Store information about the 'scales' of a dimension

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDsetdimscale(int32 id, int32 count, int32 nt, Void *data)
#else
int32 SDsetdimscale(id, count, nt, data)
int32 id;
int32 count, nt;
Void  *data;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    int32       varid, status;
    long        start[1], end[1];
	
#ifdef SDDEBUG
    fprintf(stderr, "SDsetdimscales: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the dimension structure */
    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;

    /* sanity check */
    if(count != dim->size)
        return FAIL;
    
    /* look for a variable with the same name */
    varid = SDIgetcoordvar(handle, dim, id & 0xffff, nt);
    if(varid == -1)
        return FAIL;

    /* store the data */
    handle->xdrs->x_op = XDR_ENCODE;
    start[0] = 0;
    end[0]   = count;
    status = NCvario(handle, varid, start, end, (Void *)data);
    if(status == FAIL)
        return FAIL;

    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdimsacle */


/* ---------------------------- SDgetdimscale ---------------------------- */
/*

  Retreive the scale information stored with a dimension.  It is assumed that
  the user has called SDdiminfo() and that the data array is long enough to
  hold the values.

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetdimscale(int32 id, Void *data)
#else
int32 SDgetdimscale(id, data)
int32 id;
Void  *data;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    int32       varid, status;
    long        start[1], end[1];
	
#ifdef SDDEBUG
    fprintf(stderr, "SDgetdimscale: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the dimension structure */
    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;

    /* look for a variable with the same name */
    /* assume type Float32 can be over-ridden later */
    varid = SDIgetcoordvar(handle, dim, id & 0xffff, 0);
    if(varid == -1)
        return FAIL;

    /* store the data */
    handle->xdrs->x_op = XDR_DECODE;
    start[0] = 0;
    end[0]   = dim->size;
    status   = NCvario(handle, varid, start, end, (Void *)data);
    if(status == FAIL)
        return FAIL;

    return SUCCEED;

} /* SDsetdimsacle */


/* ------------------------------ SDdiminfo ------------------------------- */
/*

  Return basic information about a dimension (name, sizes, number
    of attributes, number type, etc...)

  Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDdiminfo(int32 id, char *name, int32 *size, int32 *nt,
                int32 *nattr)
#else
int32 SDdiminfo(id, name, size, nt, nattr)
int32 id;
char  *name;
int32 *nt, *nattr, *size;
#endif
{

    NC     *  handle;
    NC_dim *  dim;
    NC_var ** dp;
    intn      ii, len;

#ifdef SDDEBUG
    fprintf(stderr, "SDdiminfo: I've been called\n");
#endif

    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->dims == NULL)
        return FAIL;

    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;

    HDstrncpy(name, dim->name->values, dim->name->count);
    name[dim->name->count] = '\0';

    *size  = dim->size;

    if(handle->vars) {
        len = HDstrlen(name);
        dp = (NC_var**)handle->vars->values;
        for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
            if( len == (*dp)->name->count &&
               HDstrncmp(name, (*dp)->name->values, (*dp)->name->count) == 0)
                {
                    *nt = (*dp)->HDFtype;
                    *nattr = ((*dp)->attrs ? (*dp)->attrs->count : 0);
                    return SUCCEED;
                }
        }
    }

    /* no var so return NULL values */
    *nt    = 0;
    *nattr = 0;

    return(SUCCEED);

} /* SDdiminfo */


/* ----------------------------- SDgetdimstrs ----------------------------- */
/*

   Read as many of the dimension strings as possible.  Assume that if a 
   pointer is not NULL that we have len bytes that we can use to return 
   the values

   Return FAIL on error else SUCCEED

*/
#ifdef PROTOTYPE
int32 SDgetdimstrs(int32 id, char *l, char *u, char *f, intn len)
#else
int32 SDgetdimstrs(id, l, u, f, len)
int32 id;
char  *l, *u, *f;
intn  len;
#endif
{

    NC       * handle;
    NC_var   * var, ** dp;
    NC_dim   * dim;
    int32      ii, namelen;
    NC_attr ** attr;
    char     * name;

#ifdef SDDEBUG
    fprintf(stderr, "SDgetdatastrs: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle == NULL) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    dim = SDIget_dim(handle, id);
    if(dim == NULL)
        return FAIL;

    /* need to get a pointer to the var now */
    var = NULL;
    if(handle->vars) {
        name = dim->name->values;
        namelen = HDstrlen(name);
        dp = (NC_var**)handle->vars->values;
        for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
            if( namelen == (*dp)->name->count &&
               HDstrncmp(name, (*dp)->name->values, (*dp)->name->count) == 0)
                {
                    var = (*dp);
                }
        }
    }

    if(!var)
        return FAIL;

    if(l) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "long_name");
        if(attr != NULL) {
            HDstrncpy((char *)l, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                l[(*attr)->data->count] = '\0';
        } else {
            l[0] = '\0';
        }
    }
    if(u) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "units");
        if(attr != NULL) {
            HDstrncpy((char *)u, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                u[(*attr)->data->count] = '\0';
        } else {
            u[0] = '\0';
        }
    }
    if(f) {
        attr = (NC_attr **) NC_findattr(&(var->attrs), "format");
        if(attr != NULL) {
            HDstrncpy((char *)f, (*attr)->data->values, len);
            if((*attr)->data->count < len)
                f[(*attr)->data->count] = '\0';
        } else {
            f[0] = '\0';
        }
    }

    return SUCCEED;

} /* SDgetdimstrs */


/* -------------------------- SDsetexternalfile --------------------------- */
/*

  Specify that the actual data for this dataset be stored in a separate
  file (and "external file" in HDF terms).

  Only the data (as in SDwritedata()) will be stored externally.  Attributes
  and such will still be in the main file

  IMPORTANT:  It is the user's responsibility to see that the separate
  files are transported when the main file is moved.

  IMPORTANT:  This can only be called *once* for a given dataset.  The
  HDF utility 'hdfpack' may be able to undo it.

  IMPORTANT:  This will only work on datasets stored in HDF files.

  Return SUCCEED or FAIL

*/
int32
#ifdef PROTOTYPE
SDsetexternalfile(int32 id, char *filename, int32 offset)
#else
SDsetexternalfile(id, filename, offset)
int32   id;
char  * filename;
int32   offset;
#endif
{

    NC       * handle;
    NC_var   * var;
    intn       status;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetexternalfile: I've been called\n");
#endif
    
    if(!filename || offset < 0)
        return FAIL;

    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL || !handle->is_hdf) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FAIL;

    /* already exists */
    if(var->data_ref) {
        
        /* no need to give a length since the element already exists */
        status = HXcreate(handle->hdf_file, DATA_TAG, (uint16) var->data_ref,
                          filename, offset, 0);

    } else {
        int32   length;

        /* look up the length */
        length = var->len;

        /* element doesn't exist so we need a reference number */
        var->data_ref = Hnewref(handle->hdf_file);
        if(var->data_ref == 0) 
            return FAIL;

        /* need to give a length since the element does not exist yet */
        status = HXcreate(handle->hdf_file, DATA_TAG, (uint16) var->data_ref,
                          filename, offset, length);

    }

    return(status);

} /* SDsetexternalfile */
