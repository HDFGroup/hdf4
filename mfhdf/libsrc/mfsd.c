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

/* -------------------------------- mfsd.c -------------------------------- */
/*

  This file contains the HDF/netCDF based multi-file interface for SDSs

  All of the routines that make up this interface have names beginning
  with SD.  Routines beginning with SDI are internal routines and
  should not be used outside of this module.

  Defining SDDEBUG will print status messages to stderr

Reading interface:
------------------

fid    = SDstart(file name, access);

        --- get the number of data-sets in the file ---
num    = SDnumber(fid);
        
sdsid  = SDselect(fid, i, ...);   0 <= i < num

        --- return the name, rank, dimsizes, #of attr, datatype ---
status = SDgetinfo(sdsid, ...); 

status = SDreaddata(sdsid, ...);

status = SDgetrange(sdsid, ...);

status = SDend(fid);

*/

#include "mfhdf.h"
#include "local_nc.h"
#include "hfile.h"

PRIVATE NC_dim * SDIget_dim
    PROTO((NC *handle, int32 id));

/* Local function prototypes */
PRIVATE NC * SDIhandle_from_id 
    PROTO((int32 id, intn typ));
PRIVATE NC_var *SDIget_var
    PROTO((NC *handle, int32 sdsid));
PRIVATE intn SDIputattr 
    PROTO((NC_array **ap, char *name, int32 nt, intn count, VOIDP data));
PRIVATE int32 SDIgetcoordvar 
    PROTO((NC *handle, NC_dim *dim, int32 id, int32 nt));
PRIVATE int32 SDIfreevarAID 
    PROTO((NC * handle, int32 index));
PRIVATE intn SDIapfromid
    PROTO((int32 id, NC ** handlep, NC_array *** app));


/* ---------------------------- SDIhandle_from_id ---------------------------- */
/*

 NAME
	SDIhandle_from_id -- get the handle from this object
 USAGE
	NC * SDIhandle_from_id(id, type)
        in32 id;                IN: an object (file, dim, dataset) ID
        intn type;              IN: the type of ID this is
 RETURNS
        NULL or the handle
 DESCRIPTION
        Map an ID to the handle for this file

--------------------------------------------------------------------------- */
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
    handle = NC_check_id((int)tmp);

    return(handle);

} /* SDIhandle_from_id */


/* ------------------------------ SDIget_var ------------------------------ */
/*

 NAME
	SDIget_var -- get the variable record
 USAGE
	NC_var * SDIget_var(handle, id)
        NC   * handle;            IN: the handle for this file
        in32   id;                IN: a dataset ID
 RETURNS
        NULL or the variable object
 DESCRIPTION
        Map an ID and a handle to the NC_var object for this dataset

--------------------------------------------------------------------------- */
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
        return (NULL);
    }
    
    return ((NC_var *)*ap);
    
} /* SDIget_var */


/* ------------------------------ SDIget_dim ------------------------------ */
/*

 NAME
	SDIget_dim -- get the dimension record
 USAGE
	NC_var * SDIget_dim(handle, id)
        NC   * handle;            IN: the handle for this file
        in32   id;                IN: a dimension ID
 RETURNS
        NULL or the variable object
 DESCRIPTION
        Map an ID and a handle to the NC_dim object for this dimension

--------------------------------------------------------------------------- */
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

 NAME
	SDstart -- open a file
 USAGE
	int32 SDstart(path, HDFmode)
        char * path;           IN: file name to open
        int32  HDFmode;        IN: access mode to open file with
 RETURNS
        A file ID or FAIL
 DESCRIPTION
        Open a file by calling ncopen() or nccreate() and return a
        file ID to the file.

--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
int32 SDstart(const char *name, int32 HDFmode)
#else
int32 SDstart(name, HDFmode)
     const char *name;
     int32 HDFmode;
#endif 
{

    intn   cdfid;
    int32  fid;
    intn   NCmode;
    NC   * handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDstart: I've been called\n");
#endif

    /* turn off annoying crash on error stuff */
    ncopts = 0;

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

 NAME
	SDend -- close a file
 USAGE
	int32 SDend(id)
        int32 id;               IN: file ID of file to close
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Close the file

--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDend(int32 id)
#else
intn SDend(id)
int32 id;
#endif
{

    intn cdfid;
    NC   * handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDend: I've been called\n");
#endif

    cdfid = (intn)id & 0xffff;

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
                if (handle->file_type != HDF_FILE)
                handle->flags &= ~(NC_NDIRTY);
            }
	}
    }

#endif /* SYNC_ON_EACC */

    /* Get rid of the temporary buffer allocated for I/O */
    SDPfreebuf();

    return(ncclose(cdfid));

} /* SDend */


/* ------------------------------ SDfileinfo ------------------------------ */
/*

 NAME
	SDfileinfo -- get info about an open file
 USAGE
	int32 SDfileinfo(fid, datasets, attrs)
        int32   id;               IN:  file ID
        int32 * datasets;         OUT: number of datasets in the file
        int32 * attrs;            OUT: number of global attributes
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Return the number of datasets and global attributes in the file.
        NOTE:  the number of datasets includes coordinate variable
        datasets.  The routine SDiscoordvar() should be used if the
        distinction is important.

--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDfileinfo(int32 fid, int32 *datasets, int32 *attrs)
#else
intn SDfileinfo(fid, datasets, attrs)
int32   fid;
int32 * datasets;
int32 * attrs;
#endif
{

    NC * handle;

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
    
    *(int32 *)datasets = ((handle->vars != NULL) ? handle->vars->count : 0);
    *(int32 *)attrs    = ((handle->attrs != NULL) ? handle->attrs->count : 0);

    return SUCCEED;

} /* SDfileinfo */


/* ------------------------------- SDselect ------------------------------- */
/*

 NAME
	SDselect -- get a dataset ID
 USAGE
	int32 SDend(fid, index)
        int32 id;               IN: file ID
        int32 index;            IN: index of dataset to get ID for
 RETURNS
        An ID to a dataset else FAIL
 DESCRIPTION
        Return an id for the index-th data set in the file

        We've got 32bits, the current ID looks like:

        sdsID:
        
        32       24       16               0
        ------------------------------------
        |  fid   | id-type| position index |
        ------------------------------------
        
        fid is the netCDF based file ID (i.e. from ncopen).  ID type
        is SDSTYPE defined in mfhdf.h and position index is the 
        position in the file of this dataset.

        The position index is zero based
        
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
int32 SDselect(int32 fid, int32 index)
#else
int32 SDselect(fid, index)
int32 fid;
int32 index;
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

 NAME
	SDgetinfo -- get info about a dataset
 USAGE
	int32 SDgetinfo(sdsid, name, rank, dimsizes, nt, atttr)
        int32   sdsid;               IN:  dataset ID
        char  * name;                OUT: name of the dataset
        int32 * rank;                OUT: rank of the dataset
        int32 * dimsize;             OUT: array of dimension siszes
        int32 * nt;                  OUT: number type of data
        int32 * attr;                OUT: the number of local attributes
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        The user is repsonsible for allocating space to hold
        the dataset name.  It can be at most MAX_NC_NAME 
        characters in length.  NULL can be passed for the name
        if it is not required.

        dimsizes should be an array to hold the dimension sizes
        a dataset can have at most MAX_VAR_DIMS dimensions.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDgetinfo(int32 sdsid, char *name, int32 *rank, int32 *dimsizes, 
                int32 *nt, int32 *nattr)
#else
intn SDgetinfo(sdsid, name, rank, dimsizes, nt, nattr)
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
        HDstrncpy(name, var->name->values, var->name->len);
        name[var->name->len] = '\0';
    }

    *rank  = var->assoc->count;
    if(!var->HDFtype)
        *nt    = hdf_map_type(var->type);
    else
        *nt    = var->HDFtype;

    *nattr = (var->attrs ? var->attrs->count : 0);

    for(i = 0; i < *rank; i++)
        dimsizes[i] = (int32) var->shape[i];

    if(dimsizes[0] == NC_UNLIMITED) {
        if(handle->file_type == HDF_FILE)
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

 NAME
	SDreaddata -- read a hyperslab of data
 USAGE
	int32 SDreaddata(sdsid, start, stride, edge, data)
        int32   sdsid;               IN:  dataset ID
        int32 * start;               IN:  coords of starting point
        int32 * stride;              IN:  stride along each dimension
        int32 * edge;                IN:  number of values to read per dimension
        VOIDP   data;                OUT: data buffer
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Read a hyperslab of data from the given variable.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDreaddata(int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data)
#else
intn SDreaddata(sdsid, start, stride, end, data)
int32 sdsid;
int32 *start, *stride, *end;
VOIDP data;
#endif
{

    NC     * handle;
    intn     varid;
    int32    status;
    NC_dim * dim = NULL;
#ifdef BIG_LONGS
    long     Start[MAX_VAR_DIMS], End[MAX_VAR_DIMS], Stride[MAX_VAR_DIMS];
#else
    long    *Start, *End, *Stride;
#endif

#ifdef SDDEBUG
    fprintf(stderr, "SDreaddata: I've been called\n");
#endif
    
    if((start == NULL) || (end == NULL) || (data == NULL))
        return FAIL;

    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) {
        handle = SDIhandle_from_id(sdsid, DIMTYPE);
        if(handle == NULL) 
            return FAIL;
        dim = SDIget_dim(handle, sdsid);
    }

    if(handle->vars == NULL)
        return FAIL;

    /* get ready to read */
    handle->xdrs->x_op = XDR_DECODE ;
   
    /* 
     * figure out the index of the variable to read from
     * the user might have passed us a dimension, in which
     * case we want to reade from its coordinate variable
     */
    if(dim) {

        varid = SDIgetcoordvar(handle, dim, (int32)(sdsid & 0xffff), (int32) 0);

    } else {
        
        /* oops, how do we know this ? */
        varid = (intn)sdsid & 0xffff;

    }

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

    Start  = (long *)start;
    End    = (long *)end;
    Stride = (long *)stride;

#endif

    /* call the readg routines if a stride is given */
    if(stride == NULL)
        status = NCvario(handle, varid, Start, End, (Void *)data);
    else
        status = NCgenio(handle, varid, Start, End, Stride, NULL, (Void *)data);

    if(status == -1)
        return FAIL;
    else
        return SUCCEED;

} /* SDreaddata */


/* ---------------------------- SDnametoindex ----------------------------- */
/*

 NAME
	SDnametoindex -- map a dataset name to an index
 USAGE
	int32 SDnametoindex(fid, name)
        int32   fid;               IN: file ID
        char  * name;              IN: name of dataset to search for
 RETURNS
        Index of a dataset or FAIL
 DESCRIPTION
        Given a data set name return the index (not ID) of the 
        first data set whose name matches.

        There can be multiple data sets with the same name.  In 
        such a case we only ever return the index of the first
        such dataset.
        Wildcards are not supported
--------------------------------------------------------------------------- */
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
        if( len == (*dp)->name->len &&
           HDstrncmp(name, (*dp)->name->values, len) == 0) {
            return(ii) ;
        }
    }

    return(FAIL) ;
    
} /* SDnametoindex */



/* ------------------------------ SDgetrange ------------------------------ */
/*

 NAME
	SDgetrange -- simulate a call to DFSDgetrange
 USAGE
	int32 SDgetrange(id, max, min)
        int32 id;               IN:  dataset ID
        VOIDP max;              OUT: valid max
        VOIDP min;              OUT: valid min
 RETURNS
        On error or missing attributes return FAIL else SUCCEED.
 DESCRIPTION
        If a "valid_range" attribute is provided return its 
        values in pmax and pmin.  Else if both a "valid max" 
        AND a "vaild min" exist return their values in pmax and pmin.

        Arrgghh, in HDF it was assumed that the max and min values 
        were of the same data type as the rest of the data.  So 
        the user would know the amount of storage to send to get 
        back the max and min values.  This is not the case with 
        netCDF.  This routine will only work if they are already 
        the same number types.
--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDgetrange(int32 sdsid, VOIDP pmax, VOIDP pmin)
#else
SDgetrange(sdsid, pmax, pmin)
int32 sdsid;
VOIDP pmax, pmin;
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

        if(((*attr1)->HDFtype != var->HDFtype) || 
           ((*attr2)->HDFtype != var->HDFtype)) {
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

 NAME
	SDcreate -- create a new dataset
 USAGE
	int32 SDcreate(fid, name, nt, rank, dimsizes)
        int32   fid;              IN: file ID
        char  * name;             IN: dataset name
        int32   nt;               IN: dataset number type
        int32   rank;             IN: rank of dataset
        int32 * dimsizes;         IN: array of dimension sizes
 RETURNS
        An ID to the new dataset else FAIL
 DESCRIPTION
        Simulate a call to ncvardef without having to be in 
        define mode.  name can be at most MAX_NC_NAME
        characters.  Rank can be at most MAX_VAR_DIMS

        It looks like for the call to NC_new_var() we need to 
        have dimension IDs already.  So I guess we should just 
        create the fake dimensions now and when optional 
        information comes in (i.e.  name, tying to other 
        dimensions) we'll go in and modify the structure in place.
        This is gonna be heinous.  Please do not attempt this at home
        See SDselect() for a discussion on how SDS IDs are set
        up.
--------------------------------------------------------------------------- */
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
    int32     sdsid;
    nc_type   nctype;
    char      dimname[MAX_NC_NAME];
    intn      i, num;
    intn    * dims;
    intn      is_ragged;

#ifdef SDDEBUG
    fprintf(stderr, "SDcreate: I've been called\n");
#endif

    /* check that fid is valid */
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL)
        return(FAIL);

    /* fudge the name since its optional */
    if((name == NULL) || (name[0] == ' ') || (name[0] == '\0'))
        name = "DataSet";

    /* check if its a ragged array */
    if((rank > 1) && dimsizes[rank - 1] == SD_RAGGED) {
        printf("YOW!  We have a ragged array kids: %s\n", name);
        rank--;
        is_ragged = TRUE;
    } else {
        is_ragged = FALSE;        
    }

    /* make fake dimensions which may or may not be over-ridden later */
    dims = (intn *) HDmalloc(rank * sizeof(intn));
    if(dims == NULL)
        return FAIL;

    if(rank > MAX_NC_DIMS)
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
    nctype = hdf_unmap_type((int)nt);
    var = (NC_var *) NC_new_var(name, nctype, (int)rank, dims);
    if(var == NULL)
        return FAIL;

    /* NC_new_var strips off "nativeness" add it back in if appropriate */
    var->HDFtype = nt;
    var->HDFsize = DFKNTsize(nt);
    var->cdf = handle; /* set cdf before calling NC_var_shape */
    /* get a new NDG ref for this sucker */
    var->ndg_ref = Hnewref(handle->hdf_file);

    /* set ragged status */
    var->is_ragged = is_ragged;
    
    /* no ragged array info stored yet */
    if(var->is_ragged) {
        var->rag_list = NULL;
        var->rag_fill = 0;
    }

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

    /* free dims */
    HDfree((VOIDP)dims);

    return sdsid;

} /* SDcreate */


/* ------------------------------ SDgetdimid ------------------------------ */
/*

 NAME
	SDgetdimid -- get a dimension ID
 USAGE
	int32 SDgetdimid(sdsid, index)
        int32 sdsid;             IN: dataset ID
        int32 index;             IN: index of dimension
 RETURNS
        An ID to the dimension else FAIL
 DESCRIPTION
        Given an sdsid and a dimension number return a 
        dimid.  Index is a ZERO based quantity

        The dimID looks similar to the sdsID except DIMTYPE 
        is substituted for SDSTYPE as the id-type:

        dimID:
        
        32       24       16               0
        ------------------------------------
        |  fid   | id-type| position index |
        ------------------------------------
        
--------------------------------------------------------------------------- */
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

 NAME
	SDsetdimname -- give a name to a dimension
 USAGE
	int32 SDsetdmname(dimid, name)
        int32   sdsid;           IN: dataset ID
        char  * name;            IN: dimension name
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Set the name of a dimension -- at most MAX_NC_NAME characters.  
        If this name is already in use we should point to the 
        existing dimension with that name.  If the sizes are 
        different return an error.  If this dimension already has
        a name throw it out and use the new one.

--------------------------------------------------------------------------- */
intn
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
        if( len == (*dp)->name->len &&
           HDstrncmp(name, (*dp)->name->values, (size_t)len) == 0) {
            if(dim != (*dp)) {
                /* a dimension with this name already exists */
                /* so change to point to it */
                if(dim->size != (*dp)->size) return FAIL;
                ap = (NC_array **) handle->dims->values;
                ap += id & 0xffff;
                NC_free_dim(dim);
                (*dp)->count += 1;
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

 NAME
	SDendaccess -- close a sds ID
 USAGE
	int32 SDendaccess(sdsid)
        int32   sdsid;           IN: dataset ID
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Close down this access ID to a data object

        Usually, this will do nothing.  However, if the meta-data 
        has changed and SYNC_ON_EACC is defiend flush it all out 
        to disk.
          
--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDendaccess(int32 id)
#else
SDendaccess(id)
int32   id;
#endif
{

    NC        * handle;
    int32       status;
	
#ifdef SDDEBUG
    fprintf(stderr, "SDendaccess: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

#ifdef SYNC_ON_EACC

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

#else 

    /* free the AID */
    status = SDIfreevarAID(handle, id & 0xffff);
    if(status == FAIL)
        return FAIL;
     
#endif /* SYNC_ON_EACC */
  
    return SUCCEED;
    
} /* SDendaccess */


/* ------------------------------ SDIputattr ------------------------------ */
/*

 NAME
	SDIputattr -- put an attribute in an attribute list
 USAGE
	int32 SDIputattr(ap, name, nt, count, data)
        NC_array ** ap;          IN/OUT: attribute list
        char     *  name;        IN:     attribute name
        int32       nt;          IN:     attribute number type
        int32       count;       IN:     number of attribute values
        VOIDP       data;        IN:     attribute values
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Common code for adding an attribute to an attribute list.
        The list gets created if it had previously been empty

--------------------------------------------------------------------------- */
PRIVATE intn
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
    nc_type  type;   /* unmap -- HDF type to NC type */

#ifdef SDDEBUG
    fprintf(stderr, "SDIputattr: I've been called\n");
#endif
    
    type = hdf_unmap_type(nt);
    if(*ap == NULL ) { /* first time */
        attr = (NC_attr *) NC_new_attr(name,type,(unsigned)count,data) ;
        if(attr == NULL)
            return FAIL;
        *ap = NC_new_array(NC_ATTRIBUTE,(unsigned)1, (Void*)&attr) ;
        if(*ap == NULL)
            return FAIL;
    }
    else   {
        if((atp = NC_findattr(ap, name)) != NULL) { /* name in use */
            old = *atp ;
            *atp = (NC_attr *) NC_new_attr(name,type,(unsigned)count,data);
            if(*atp == NULL) {
                *atp = old;
                return FAIL;
            }
            NC_free_attr(old);
        }
        else   {
            if((*ap)->count >= MAX_NC_ATTRS) {  /* Too many */
            return FAIL;
            }
            /* just add it */
            attr = (NC_attr *) NC_new_attr(name,type,(unsigned)count,data);
            if(attr == NULL)
                return FAIL;
            if(NC_incr_array((*ap), (Void *)&attr) == NULL)
                return FAIL;
        }
    }
    /* succeeded. Add HDFtype  */
    if (attr != NULL)   
        attr->HDFtype = nt;
    return SUCCEED;
    
} /* SDIputattr */


/* ------------------------------ SDsetrange ------------------------------ */
/*

 NAME
	SDsetrange -- simulate a call to DFSDsetrange
 USAGE
	int32 SDsetrange(id, max, min)
        int32 id;               IN: dataset ID
        VOIDP max;              IN: valid max
        VOIDP min;              IN: valid min
 RETURNS
        On error FAIL else SUCCEED.
 DESCRIPTION
        Store range info for this variable in the valid_range 
        attribute.  If that attribute already exists overwrite 
        the current values.  It is assumed that the values are 
        the same type as the data set.

        It is up to the user to decide what is meant by the
        "valid" max and min.

--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDsetrange(int32 sdsid, VOIDP pmax, VOIDP pmin)
#else
SDsetrange(sdsid, pmax, pmin)
int32 sdsid;
VOIDP pmax, pmin;
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
    sz = DFKNTsize(var->HDFtype | DFNT_NATIVE);
    HDmemcpy(data, pmin, sz);
    HDmemcpy(data + sz, pmax, sz);

    /* call common code */
    if(SDIputattr(&var->attrs, "valid_range", var->HDFtype, (intn) 2, 
                  (VOIDP) data) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetrange */


/* ----------------------------- SDIapfromid ------------------------------ */
/*

 NAME
	SDIapfromid -- get the attribute list
 USAGE
	int32 SDapfromid(id, handle, app)
        int32        id;               IN:  object ID
        NC       **  handlep;          IN:  handle for this file
        NC_array *** app;  / yuk! /    OUT: attribute list
 RETURNS
        On error FAIL else SUCCEED.
 DESCRIPTION
        Given a ID figure out what the handle and attribute 
        list pointer are for that object.

--------------------------------------------------------------------------- */
PRIVATE
intn
#ifdef PROTOTYPE
SDIapfromid(int32 id, NC ** handlep, NC_array *** app)
#else
SDIapfromid(id, handlep, app)
int32        id;
NC       **  handlep;
NC_array *** app;
#endif
{
    NC     * handle;
    NC_var * var;
    NC_dim * dim;
    int32    varid;

    handle = NULL;

    /* see if its a variable ID */
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle != NULL) { 
        
        /* find the variable */
        var = SDIget_var(handle, id);
        if(var == NULL)
            return FAIL;

        (*app) = &(var->attrs);
        (*handlep) = handle;
        return SUCCEED;
    } 

    /* see if its a file ID */
    handle = SDIhandle_from_id(id, CDFTYPE);
    if(handle != NULL) {
        (*app) = &(handle->attrs);
        (*handlep) = handle;
        return SUCCEED;
    }

    /* see if its a dimension ID */
    handle = SDIhandle_from_id(id, DIMTYPE);
    if(handle != NULL) {

        /* find the dimension */
        dim = SDIget_dim(handle, id);
        if(dim == NULL)
            return FAIL;

        /* get index of coordinate variable */
        varid = SDIgetcoordvar(handle, dim, (int32)(id & 0xffff), (int32) 0);

        /* get the variable object */
        var = NC_hlookupvar(handle, varid);
        if(var == NULL)
            return FAIL;

        (*app) = &(var->attrs);
        (*handlep) = handle;
        return SUCCEED;

    }

    return FAIL;

} /* SDIapfromid */


/* ------------------------------ SDsetattr ------------------------------- */
/*

 NAME
	SDsetattr -- user level function to create and set an attribute
 USAGE
	int32 SDsetattr(id, name, nt, count, data)
        int32   id;          IN: object ID
        char  * name;        IN: attribute name
        int32   nt;          IN: attribute number type
        int32   count;       IN: number of attribute values
        VOIDP   data;        IN: attribute values
 RETURNS
        On error FAIL else SUCCEED.
 DESCRIPTION
        Given an ID and an attribute defintion attach the atrribute 
        to the thing represented by the ID.  For starters, the valid 
        IDs could be variable, file or dimesnion IDs
--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDsetattr(int32 id, char *name, int32 nt, int32 count, VOIDP data)
#else
SDsetattr(id, name, nt, count, data)
int32 id;
char  *name;
int32 nt;
int32 count;
VOIDP data;
#endif
{

    NC_array **ap;
    NC       * handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetattr: I've been called\n");
#endif

    /* sanity check args */
    if(name == NULL) 
        return FAIL;
    /* This release doesn't support native number types for attr  */
    if (nt & DFNT_NATIVE) 
        return FAIL;

    /* determine what type of ID we've been given */
    if(SDIapfromid(id, &handle, &ap) == FAIL)
        return FAIL;

    /* still no handle ? */
    if(handle == NULL)
        return FAIL;
    
    /* hand over to SDIputattr */
         
    if(SDIputattr(ap, name, nt, count, data) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;
    
} /* SDsetattr */


/* ------------------------------ SDattrinfo ------------------------------- */
/*

 NAME
	SDattrinfo -- get info about an attribute
 USAGE
	int32 SDattrinfo(id, index, name, nt, count)
        int32   id;          IN:  object ID
        int32   index;       IN:  attribute index
        char  * name;        OUT: attribute name
        int32 * nt;          OUT: attribute number type
        int32 * count;       OUT: number of attribute values
 RETURNS
        On error FAIL else SUCCEED.
 DESCRIPTION
        Inquire about an attribute.  Attribute indexes are zero based.

        Given the ID of the attribute's parent and the attribute's 
        index return the number type, name and count of the attribute 
        so the user knows how much space to provide to read it

--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDattrinfo(int32 id, int32 index, char *name, int32 *nt, int32 *count)
#else
SDattrinfo(id, index, name, nt, count)
int32 id, index;
char  *name;
int32 *nt;
int32  *count;
#endif
{

    NC_array *  ap;
    NC_array ** app;
    NC_attr  ** atp;
    NC       *  handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDattrinfo: I've been called\n");
#endif

    /* sanity check args */
    if((name == NULL) || (nt == NULL) || (count == NULL))
        return FAIL;

    /* determine what type of ID we've been given */
    if(SDIapfromid(id, &handle, &app) == FAIL)
        return FAIL;

    ap = (*app);

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
        HDstrncpy(name, (*atp)->name->values, (*atp)->name->len);
        name[(*atp)->name->len] = '\0';
    }

    *count = (*atp)->data->count;
    if (handle->file_type != HDF_FILE)
        *nt    = hdf_map_type((*atp)->data->type);
    else
        *nt = (*atp)->HDFtype;

    return SUCCEED;

} /* SDattrinfo */


/* ------------------------------ SDreadattr ------------------------------ */
/*

 NAME
	SDreadattr -- read an attribute's values
 USAGE
	int32 SDattrinfo(id, index, data)
        int32 id;          IN:  object ID
        int32 index;       IN:  attribute index
        VOIDP data;        OUT: data buffer 
 RETURNS
        On error FAIL else SUCCEED.
 DESCRIPTION
        Read the actual contents of the given attribute

        Assume that the user has called SDinqattr() and so 
        has allocated sufficient space

--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDreadattr(int32 id, int32 index, VOIDP buf)
#else
SDreadattr(id, index, buf)
int32 id, index;
VOIDP buf;
#endif
{

    NC_array *  ap;
    NC_array ** app;
    NC_attr  ** atp;
    NC       *  handle;

#ifdef SDDEBUG
    fprintf(stderr, "SDreadattr: I've been called\n");
#endif

    /* sanity check args */
    if(buf == NULL)
        return FAIL;
    
    /* determine what type of ID we've been given */
    if(SDIapfromid(id, &handle, &app) == FAIL)
        return FAIL;

    ap = (*app);

    if((ap == NULL) || (index >= ap->count))
        return FAIL;

    /* 
     * ap is the proper attribute list now look up something with this
     *  index
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

 NAME
	SDwritedata -- write a hyperslab of data
 USAGE
	int32 SDwritedata(sdsid, start, stride, edge, data)
        int32   sdsid;               IN: dataset ID
        int32 * start;               IN: coords of starting point
        int32 * stride;              IN: stride along each dimension
        int32 * edge;                IN: number of values to write per dimension
        VOIDP   data;                IN: data buffer
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Write out a chunk o data.  Except for the line setting the 
        XDR op-code this is exactly the same as SDreaddata().  The 
        two routines should really be combined at some point
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDwritedata(int32 sdsid, int32 *start, int32 *stride, int32 *end, VOIDP data)
#else
intn SDwritedata(sdsid, start, stride, end, data)
int32 sdsid;
int32 *start, *stride, *end;
VOIDP data;
#endif
{

    NC     * handle;
    intn     varid;
    int32    status;
    NC_dim * dim = NULL;
#ifdef BIG_LONGS
    long     Start[MAX_VAR_DIMS], End[MAX_VAR_DIMS], Stride[MAX_VAR_DIMS];
#else
    long    *Start, *End, *Stride;
#endif

#ifdef SDDEBUG
    fprintf(stderr, "SDwritedata: I've been called\n");
#endif

    if((start == NULL) || (end == NULL) || (data == NULL))
        return FAIL;


    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) {
        handle = SDIhandle_from_id(sdsid, DIMTYPE);
        if(handle == NULL) 
            return FAIL;
        dim = SDIget_dim(handle, sdsid);
    }

    if(handle->vars == NULL)
        return FAIL;

    /* get ready to write */
    handle->xdrs->x_op = XDR_ENCODE;
    
    /* 
     * figure out the index of the variable to write to
     * the user might have passed us a dimension, in which
     * case we want to write to its coordinate variable
     */
    if(dim) {

        varid = SDIgetcoordvar(handle, dim, (int32)(sdsid & 0xffff), (int32) 0);

    } else {
        
        /* oops, how do we know this ? */
        varid = (intn)sdsid & 0xffff;

    }

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

    Start  = (long *)start;
    End    = (long *)end;
    Stride = (long *)stride;

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

 NAME
	SDsetdatastrs -- set "data strings"
 USAGE
	int32 SDsetdatastrs(sdsid, l, u, f, c)
        int32   sdsid;          IN: dataset ID
        int32 * l;              IN: label string ("long_name")
        int32 * u;              IN: units string ("units")
        int32 * f;              IN: format string ("format")
        int32 * c;              IN: coordsys string ("coordsys")
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Store information about the 'label', 'units', 'format' and 
        'cordsys' attributes of a dataset.  All of the values 
        are optional.  If no string is desired NULL should be passed 
        in its place.  
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDsetdatastrs(int32 sdsid, char *l, char *u, char *f, char *c)
#else
intn SDsetdatastrs(sdsid, l, u, f, c)
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
    
    if(l && l[0] != '\0') 
        if(SDIputattr(&var->attrs, "long_name", DFNT_CHAR, 
                      (intn) HDstrlen(l), l) == FAIL)
            return FAIL;

    if(u && u[0] != '\0') 
        if(SDIputattr(&var->attrs, "units", DFNT_CHAR, 
                      (intn) HDstrlen(u), u) == FAIL)
            return FAIL;

    if(f && f[0] != '\0') 
        if(SDIputattr(&var->attrs, "format", DFNT_CHAR, 
                      (intn) HDstrlen(f), f) == FAIL)
            return FAIL;
    
    if(c && c[0] !='\0') 
        if(SDIputattr(&var->attrs, "cordsys", DFNT_CHAR, 
                      (intn) HDstrlen(c), c) == FAIL)
            return FAIL;
    
    /* make sure it gets reflected in the file */
    if(l || u || f || c)
        handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdatastrs */


/* ------------------------------- SDsetcal ------------------------------- */
/*

 NAME
	SDsetcal -- set calibration information
 USAGE
	int32 SDsetcal(sdsid, cal, cale, ioff, ioffe, nt)
        int32   sdsid;          IN: dataset ID
        float64 cal;            IN: multiplicative factor
        float64 cale;           IN: multiplicative factor error
        float64 ioff;           IN: integer offset 
        float64 ioffe;          IN: integer offset error
        int32   nt;             IN: number type of uncalibrated data
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Store calibration information.  What is the formula?

--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDsetcal(int32 sdsid, float64 cal, float64 cale, float64 ioff, float64 ioffe, int32 nt)
#else
intn SDsetcal(sdsid, cal, cale, ioff, ioffe, nt)
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
    
    if(SDIputattr(&var->attrs, "scale_factor", DFNT_FLOAT64, 
                  (intn) 1, (VOIDP) &cal) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "scale_factor_err", DFNT_FLOAT64, 
                  (intn) 1, (VOIDP) &cale) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "add_offset", DFNT_FLOAT64, 
                  (intn) 1, (VOIDP) &ioff) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "add_offset_err", DFNT_FLOAT64, 
                  (intn) 1, (VOIDP) &ioffe) == FAIL)
        return FAIL;
    if(SDIputattr(&var->attrs, "calibrated_nt", DFNT_INT32, 
                  (intn) 1, (VOIDP) &nt) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetcal */


/* --------------------------- SDsetfillvalue ----------------------------- */
/*

 NAME
	SDsetfillvalue -- set the fill value
 USAGE
	int32 SDsetfillvalue(id, val)
        int32 id;               IN: dataset ID
        VOIDP val;              IN: fillvalue
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Set the fill value for this data set.  The fill value 
        is assumed to have the same number type as the dataset
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDsetfillvalue(int32 sdsid, VOIDP val)
#else
intn SDsetfillvalue(sdsid, val)
int32 sdsid;
VOIDP val;
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
    
    if(SDIputattr(&var->attrs, _FillValue, var->HDFtype, 
                  (intn) 1, val) == FAIL)
        return FAIL;
    
    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetfillvalue */


/* --------------------------- SDgetfillvalue ----------------------------- */
/*

 NAME
	SDgetfillvalue -- get the fill value
 USAGE
	int32 SDgetfillvalue(id, val)
        int32 id;               IN:  dataset ID
        VOIDP val;              OUT: fillvalue
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Retreive the fill value for this data set if one has been
        stored.  The fill value has the same number type as the
        dataset
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDgetfillvalue(int32 sdsid, VOIDP val)
#else
intn SDgetfillvalue(sdsid, val)
int32 sdsid;
VOIDP val;
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

 NAME
	SDgetdatastrs -- get "data strings"
 USAGE
	int32 SDgetdatasts(sdsid, l, u, f, c, len)
        int32   sdsid;          IN:  dataset ID
        char  * l;              OUT: label string ("long_name")
        char  * u;              OUT: units string ("units")
        char  * f;              OUT: format string ("format")
        char  * c;              OUT: coordsys string ("coordsys")
        int32   len;            IN:  buffer length
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Read information about the 'label', 'units', 'format' and 
        'cordsys' attributes of a dataset.  All of the values 
        are optional.  If no string is desired NULL should be passed 
        in its place.  Assume all buffers are len bytes long.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDgetdatastrs(int32 sdsid, char *l, char *u, char *f, char *c, intn len)
#else
intn SDgetdatastrs(sdsid, l, u, f, c, len)
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

 NAME
	SDgetcal -- get calibration information
 USAGE
	int32 SDgetcal(sdsid, cal, cale, ioff, ioffe, nt)
        int32     sdsid;          IN:  dataset ID
        float64 * cal;            OUT: multiplicative factor
        float64 * cale;           OUT: multiplicative factor error
        float64 * ioff;           OUT: integer offset 
        float64 * ioffe;          OUT: integer offset error
        int32   * nt;             OUT: number type of uncalibrated data
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Retreive calibration information.  What is the formula?

--------------------------------------------------------------------------- */

#ifdef PROTOTYPE
intn SDgetcal(int32 sdsid, float64 *cal, float64 *cale, float64 *ioff, float64 *ioffe, int32 *nt)
#else
intn SDgetcal(sdsid, cal, cale, ioff, ioffe, nt)
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

 NAME
	SDgetcoordvar -- get index of coordinate variable
 USAGE
	int32 SDgetcoordvar(handle, dim, id, nt)
        NC     * handle;         IN: file handle
        NC_dim * dim;            IN: dimension to find coord var of
        int32    id;             IN: dimension ID
        int32    nt;             IN: number type to use if new variable
 RETURNS
        A variable index or FAIL on error
 DESCRIPTION
        Given a dimension return the index of its coordinate variable
        creating one if needed.  If we need to create a variable and an nt
        is not supplied (i.e. is equal to zero) use 32bit floats.
        
        If the variable already exists and the existing nt is different
        from the supplied one (and the supplied one is not zero) replace
        the nt by the new one.  ID is needed so that we can set the 
        dimension of the variable correctly if we need to.  Yuck.
        
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
PRIVATE int32 SDIgetcoordvar(NC *handle, NC_dim *dim, int32 id, int32 nt)
#else
PRIVATE int32 SDIgetcoordvar(handle, dim, id, nt)
NC     * handle;
NC_dim * dim;
int32    id, nt;
#endif
{ 

    NC_string * name;
    int32       ii, len;
    nc_type     nctype;
    NC_var   ** dp, *var;
    intn        dimindex;

    /* look for a variable with the same name */
    name = dim->name;
    len = dim->name->len;
    dp = (NC_var**)handle->vars->values ;
    for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
        if( len == (*dp)->name->len &&
           HDstrncmp(name->values, (*dp)->name->values, (size_t)len) == 0) {

            /* see if we need to change the number type */
            
            if((nt != 0) && (nt != (*dp)->type)) {
#ifdef SDDEBUG
                fprintf(stderr, "SDIgetcoordvar redefining type\n");
#endif
                (*dp)->type = hdf_unmap_type((intn)nt);
                (*dp)->HDFtype = nt;
                   /* don't forget to reset the sizes  */
                (*dp)->szof = NC_typelen((*dp)->type);
                (*dp)->HDFsize = DFKNTsize(nt);
                
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

    nctype = hdf_unmap_type((intn)nt);
    dimindex = (intn)id;
    var = (NC_var *) NC_new_var(name->values, nctype, (unsigned)1, &dimindex);
    if(var == NULL)
        return FAIL;
    
    /* get a new NDG ref for this sucker */
    var->ndg_ref = Hnewref(handle->hdf_file);

    /* add it to the handle */
    if(handle->vars->count >= MAX_NC_VARS)
        return FAIL;
    var->cdf = handle; /* set cdf before calling NC_var_shape */
    /* compute all of the shape information */
    if(NC_var_shape(var, handle->dims) == -1)
        return FAIL;
    
    if(NC_incr_array(handle->vars, (Void *)&var) == NULL)
        return FAIL;
    
    return(handle->vars->count -1);

} /* SDIgetcoordvar */


/* ----------------------------- SDsetdimstrs ----------------------------- */
/*

 NAME
	SDsetdimstrs -- set "dimension strings"
 USAGE
	int32 SDsetdatastrs(sdsid, l, u, f, c)
        int32   sdsid;          IN: dimension ID
        int32 * l;              IN: label string ("long_name")
        int32 * u;              IN: units string ("units")
        int32 * f;              IN: format string ("format")
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Store information about the 'label', 'units' and 'format' 
        attributes of a dimension.  All three of the values are optional.  
        If no string is desired NULL should be passed in its place.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDsetdimstrs(int32 id, char *l, char *u, char *f)
#else
intn SDsetdimstrs(id, l, u, f)
int32 id;
char  *l, *u, *f;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    intn        varid;
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
    varid = (intn)SDIgetcoordvar(handle, dim, (int32)(id & 0xffff), (int32)0);
    if(varid == FAIL)
        return FAIL;

    /* get the variable object */
    var = NC_hlookupvar(handle, varid);
    if(var == NULL)
        return FAIL;

    /* set the attributes */
    if(l && l[0] != '\0')
        if(SDIputattr(&var->attrs, "long_name", DFNT_CHAR,
                      (intn) HDstrlen(l), l) == FAIL)
            return FAIL;

    if(u && u[0] != '\0')
        if(SDIputattr(&var->attrs, "units", DFNT_CHAR,
                      (intn) HDstrlen(u), u) == FAIL)
            return FAIL;

    if(f && f[0] != '\0')
        if(SDIputattr(&var->attrs, "format", DFNT_CHAR,
                      (intn) HDstrlen(f), f) == FAIL)
            return FAIL;

    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdimstrs */


/* ---------------------------- SDIfreevarAID ----------------------------- */
/*

 NAME
	SDIfreevarAID -- free a variables AID
 USAGE
	int32 SDgetdimscale(handle, index)
        NC    * handle;           IN: file handle
        int32   index;            IN: variable index
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Free the AID of the variable with the given index
 
--------------------------------------------------------------------------- */
PRIVATE
#ifdef PROTOTYPE
int32 SDIfreevarAID(NC * handle, int32 index)
#else
int32 SDIfreevarAID(handle, index)
NC * handle;
int32 index;
#endif
{

    NC_array **ap ;
    NC_var * var;

    if(handle == NULL || !handle->vars)
        return FAIL;

    if(index < 0 || index > handle->vars->count)
        return FAIL;

    ap = (NC_array **)handle->vars->values;
    ap += index;

    var = (NC_var *) *ap;

    if(var->aid != 0 && var->aid != FAIL)
        Hendaccess(var->aid);

    var->aid = FAIL;

    return SUCCEED;

} /* SDIfreevarAID */
 

/* ---------------------------- SDsetdimscale ----------------------------- */
/*

 NAME
	SDsetdimscale -- store scale information for the dimension
 USAGE
	int32 SDsetdimsacle(id, count, nt, data)
        int32 id;               IN: dimension ID
        int32 count;            IN: number of values
        int32 nt;               IN: number type of data
        VOIDP data;             IN: scale values
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Store information about the 'scales' of a dimension.  Dimensions
        do not have to have the same number type as the dataset.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDsetdimscale(int32 id, int32 count, int32 nt, VOIDP data)
#else
intn SDsetdimscale(id, count, nt, data)
int32 id;
int32 count, nt;
VOIDP data;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    int32       status;
    intn        varid;
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

    /* sanity check, if not SD_UNLIMITED */
    if( dim->size != 0 && count != dim->size)
        return FAIL;

    /* look for a variable with the same name */
    varid = (intn)SDIgetcoordvar(handle, dim, id & 0xffff, nt);
    if(varid == -1)
        return FAIL;

    /* store the data */
    handle->xdrs->x_op = XDR_ENCODE;
    start[0] = 0;
    end[0]   = count;
    status = NCvario(handle, varid, start, end, (Void *)data);
    if(status == FAIL)
        return FAIL;

    /* free the AID */
    status = SDIfreevarAID(handle, varid);
    if(status == FAIL)
        return FAIL;

    /* make sure it gets reflected in the file */
    handle->flags |= NC_HDIRTY;

    return SUCCEED;

} /* SDsetdimsacle */


/* ---------------------------- SDgetdimscale ---------------------------- */
/*

 NAME
	SDgetdimscale -- get scale information for the dimension
 USAGE
	int32 SDgetdimscale(id, data)
        int32 id;               IN:  dimension ID
        VOIDP data;             OUT: scale values
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Retreive the scale information stored with a dimension.  It is 
        assumed that the user has called SDdiminfo() and that the data 
        array is long enough to hold the values.
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDgetdimscale(int32 id, VOIDP data)
#else
intn SDgetdimscale(id, data)
int32 id;
VOIDP data;
#endif
{

    NC        * handle;
    NC_dim    * dim;
    NC_var    * vp;
    int32       status;
    intn        varid;
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
    varid = (intn)SDIgetcoordvar(handle, dim, (int32)(id & 0xffff), (int32)0);
    if(varid == -1)
        return FAIL;

    /* store the data */
    handle->xdrs->x_op = XDR_DECODE;
    start[0] = 0;
    if (dim->size != 0)
        end[0] = dim->size;
    else   {
        if (handle->file_type != HDF_FILE)
            end[0] = handle->numrecs;
        else   {
            vp = SDIget_var(handle, varid);
            if (vp == NULL)
                return FAIL;
            end[0] = vp->numrecs;
        }
    }
  
    status   = NCvario(handle, varid, start, end, (Void *)data);
    if(status == FAIL)
        return FAIL;

    /* free the AID */
    status = SDIfreevarAID(handle, varid);
    if(status == FAIL)
        return FAIL;

    return SUCCEED;

} /* SDsetdimsacle */


/* ------------------------------ SDdiminfo ------------------------------- */
/*

 NAME
	SDdiminfo -- get info about a dimension
 USAGE
	int32 SDdiminfo(sdsid, name, size, nt, atttr)
        int32   dimid;               IN:  dimension ID
        char  * name;                OUT: name of the dimension
        int32 * size;                OUT: size of the dimension
        int32 * nt;                  OUT: number type of scales
        int32 * attr;                OUT: the number of local attributes
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Return basic information about a dimension (name, sizes, number
        of attributes, number type, etc...) The user is repsonsible for 
        allocating space to hold the dataset name.  It can be at most 
        MAX_NC_NAME characters in length.  NULL can be passed for the 
        name if it is not required.

--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDdiminfo(int32 id, char *name, int32 *size, int32 *nt, int32 *nattr)
#else
intn SDdiminfo(id, name, size, nt, nattr)
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

    if(name != NULL) {
        HDstrncpy(name, dim->name->values, dim->name->len);
        name[dim->name->len] = '\0';
    } else {
        name = dim->name->values;
    }

    *size  = dim->size;

    if(handle->vars) {
        len = dim->name->len;
        dp = (NC_var**)handle->vars->values;
        for(ii = 0 ; ii < handle->vars->count ; ii++, dp++) {
            if( len == (*dp)->name->len &&
               HDstrncmp(name, (*dp)->name->values, (*dp)->name->len) == 0)
                {
                    if (handle->file_type == HDF_FILE)
                         *nt = ((*dp)->numrecs ? (*dp)->HDFtype : 0);
                    else *nt = (*dp)->HDFtype;

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

 NAME
	SDgetdimstrs -- get "data strings"
 USAGE
	int32 SDgetddimsts(sdsid, l, u, f, len)
        int32   sdsid;          IN:  dataset ID
        char  * l;              OUT: label string ("long_name")
        char  * u;              OUT: units string ("units")
        char  * f;              OUT: format string ("format")
        int32   len;            IN:  buffer length
 RETURNS
        SUCCEED / FAIL
 DESCRIPTION
        Read as many of the dimension strings as possible.  Assume that 
        if a pointer is not NULL that we have len bytes that we can use 
        to return the values
 
--------------------------------------------------------------------------- */
#ifdef PROTOTYPE
intn SDgetdimstrs(int32 id, char *l, char *u, char *f, intn len)
#else
intn SDgetdimstrs(id, l, u, f, len)
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
    fprintf(stderr, "SDgetdimstrs: I've been called\n");
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
            if( namelen == (*dp)->name->len &&
               HDstrncmp(name, (*dp)->name->values, (size_t)namelen) == 0)
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

 NAME
	SDsetexternalfile -- store info in a separate file
 USAGE
	int32 SDsetexternalfile(id, filename, offset)
        int32   id;                  IN: dataset ID
        char  * filename;            IN: name of external file
        int32   offset;              IN: offset in external file
 RETURNS
        Return SUCCEED or FAIL

 DESCRIPTION
        Specify that the actual data for this dataset be stored in a 
        separate file (and "external file" in HDF terms).

        Only the data (as in SDwritedata()) will be stored externally.  
        Attributes and such will still be in the main file

        IMPORTANT:  It is the user's responsibility to see that the 
        separate files are transported when the main file is moved.

        IMPORTANT:  This can only be called *once* for a given dataset.  
        The HDF utility 'hdfpack' may be able to undo it.

        IMPORTANT:  This will only work on datasets stored in HDF files.

--------------------------------------------------------------------------- */

intn SDsetexternalfile(int32 id, char *filename, int32 offset)
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
    if(handle == NULL || handle->file_type != HDF_FILE)
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FAIL;

    /* already exists */
    if(var->data_ref) {

        /* no need to give a length since the element already exists */
        status = (intn)HXcreate(handle->hdf_file, (uint16)DATA_TAG, (uint16) var->data_ref,
                          filename, offset, (int32)0);

    } else {
        int32   length;

        /* look up the length */
        length = var->len;

        /* element doesn't exist so we need a reference number */
        var->data_ref = Hnewref(handle->hdf_file);
        if(var->data_ref == 0)
            return FAIL;

        /* need to give a length since the element does not exist yet */
        status = (intn)HXcreate(handle->hdf_file, (uint16)DATA_TAG, (uint16) var->data_ref,
                          filename, offset, length);

    }

    if(status != FAIL) {
        if((var->aid != 0) && (var->aid != FAIL))
            Hendaccess(var->aid);
        var->aid = status;
    }

    return(status);

} /* SDsetexternalfile */


/* -------------------------- SDsetnbitdataset ---------------------------- */
/*

 NAME
	SDsetnbitdataset -- Create/convert a dataset to n-bit representation
 USAGE
	intn SDsetnbitdataset(id, start_bit, bit_len, sign_ext, fill_one)
        int32   id;                  IN: dataset ID
        intn start_bit;              IN: starting bit offset (lowest=0)
        intn bit_len;                IN: # of bits to write
        intn sign_ext;               IN: Whether to sign extend
        intn fill_one;               IN: Whether to fill background w/1's
 RETURNS
        Return SUCCEED or FAIL

 DESCRIPTION
        Specify that the actual data for this dataset be represented as a
        n-bit dataset internally in the HDF file.

        The start_bit parameter determines the lowest bit to write out,
        the bit_len parameter determines how many bits to write out.  The
        bits in the data include the lowest bit (start_bit) and count up
        bit_len-1 bits to write out.  For example, starting at bit 2
        and writing 4 bits from the following bit data, "01111011", would
        write out the bit data, "1110", to the dataset on disk.

        The sign_ext parameter determines whether the top bit (highest bit #)
        is used to sign extend the bits whether data is read back from the
        disk.  The fill_one parameter is used to determine whether to
        fill the "background bits" (the bits not in the data written to the
        file) with 1's or 0's when the data is read back from the file.

        Only the data (as in SDwritedata()) will be stored in n-bit
        representation.  Attributes and such will still be stored normally.

        IMPORTANT:  This will only work on datasets stored in HDF files.

        NOTE: n-bit "compression" is described more fully in the cnbit.c file.

--------------------------------------------------------------------------- */

intn SDsetnbitdataset(int32 id, intn start_bit, intn bit_len, intn sign_ext,
    intn fill_one)
{

    NC       * handle;
    NC_var   * var;
    intn       status;
    model_info m_info;  /* modeling information for the HCcreate() call */
    comp_info c_info;   /* "compression" information for the HCcreate() call */

#ifdef SDDEBUG
    fprintf(stderr, "SDsetnbitdataset: I've been called\n");
#endif

    if(start_bit<0 || bit_len<=0)
        return FAIL;

    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE)
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var=SDIget_var(handle, id);
    if(var==NULL)
        return FAIL;

    /* set up n-bit parameters */
    c_info.nbit.nt=var->HDFtype;
    c_info.nbit.sign_ext=sign_ext;
    c_info.nbit.fill_one=fill_one;
    c_info.nbit.start_bit=start_bit;
    c_info.nbit.bit_len=bit_len;

#ifdef SDDEBUG
printf("SDsetnbitdata(): nt=%d, sign_ext=%d, fill_one=%d, start_bit=%d, bit_len=%d\n",(intn)c_info.nbit.nt,(intn)c_info.nbit.sign_ext,(intn)c_info.nbit.fill_one,(intn)c_info.nbit.start_bit,(intn)c_info.nbit.bit_len);
#endif
    if(!var->data_ref) {   /* doesn't exist */
#ifdef SDDEBUG
printf("SDsetnbitdata(): dataset doesn't exist\n");
#endif

        /* element doesn't exist so we need a reference number */
        var->data_ref=Hnewref(handle->hdf_file);
        if(var->data_ref == 0)
            return FAIL;
      } /* end if */
    status=(intn)HCcreate(handle->hdf_file,(uint16)DATA_TAG,
            (uint16) var->data_ref,COMP_MODEL_STDIO,&m_info,COMP_CODE_NBIT,
             &c_info);

#ifdef SDDEBUG
printf("SDsetnbitdata(): HCcreate() status=%d\n",(intn)status);
#endif
    if(status != FAIL) {
        if((var->aid != 0) && (var->aid != FAIL))
            Hendaccess(var->aid);
        var->aid = status;
      } /* end if */
    return(status);
} /* SDsetnbitdataset */

/* -------------------------- SDsetcompress ---------------------------- */
/*

 NAME
	SDsetcompress -- Create/convert a dataset to compressed representation
 USAGE
	intn SDsetcompress(id, type, cinfo)
        int32   id;                  IN: dataset ID
        int32 type;             IN: the type of compression to perform on the
                                    next image
        comp_info *cinfo;       IN: ptr to compression information structure
 RETURNS
        Return SUCCEED or FAIL

 DESCRIPTION
        Specify a compression scheme for an SD dataset.

        Valid compression types available for this interface are listed in
        hcomp.h as COMP_nnnn.

        IMPORTANT:  This will only work on datasets stored in HDF files.

--------------------------------------------------------------------------- */

intn SDsetcompress(int32 id, int32 type, comp_info *c_info)
{

    NC       * handle;
    NC_var   * var;
    intn       status;
    model_info m_info;  /* modeling information for the HCcreate() call */

#ifdef SDDEBUG
    fprintf(stderr, "SDsetnbitdataset: I've been called\n");
#endif

    if (type < 0 || type > COMP_MAX_COMP || compress_map[type] == 0)
        return(FAIL);

    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE)
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var=SDIget_var(handle, id);
    if(var==NULL)
        return FAIL;

#ifdef SDDEBUG
printf("SDsetnbitdata(): nt=%d, sign_ext=%d, fill_one=%d, start_bit=%d, bit_len=%d\n",(intn)c_info.nbit.nt,(intn)c_info.nbit.sign_ext,(intn)c_info.nbit.fill_one,(intn)c_info.nbit.start_bit,(intn)c_info.nbit.bit_len);
#endif
    if(!var->data_ref) {   /* doesn't exist */
#ifdef SDDEBUG
printf("SDsetnbitdata(): dataset doesn't exist\n");
#endif

        /* element doesn't exist so we need a reference number */
        var->data_ref=Hnewref(handle->hdf_file);
        if(var->data_ref == 0)
            return FAIL;
      } /* end if */
    status=(intn)HCcreate(handle->hdf_file,(uint16)DATA_TAG,
            (uint16) var->data_ref,COMP_MODEL_STDIO,&m_info,type,
             c_info);

#ifdef SDDEBUG
printf("SDsetnbitdata(): HCcreate() status=%d\n",(intn)status);
#endif
    if(status != FAIL) {
        if((var->aid != 0) && (var->aid != FAIL))
            Hendaccess(var->aid);
        var->aid = status;
      } /* end if */
    return(status);
} /* SDsetcompress */


/* ------------------------------ SDfindattr ------------------------------ */
/*

 NAME
	SDfindattr -- find an attribute's index by name
 USAGE
	int32 SDfindattr(id, name)
        int32   id;             IN: object ID
        char  * name;           IN: attribute name
 RETURNS
        An attribute index or FAIL
 DESCRIPTION
        Given an ID to an object and an attribute name return the index 
        of the attribute with that name.  This does not support any
        form of wildcards / regular expressions
 
--------------------------------------------------------------------------- */
int32
#ifdef PROTOTYPE
SDfindattr(int32 id, char *attrname)
#else
SDfindattr(id, attrname)
int32   id;
char  * attrname;
#endif
{

    NC_array *  ap;
    NC_array ** app;
    NC_attr  ** attr;
    NC_var   *  var;
    NC       *  handle;
    int32       attrid, len;

     /* determine what type of ID we've been given */
    if(SDIapfromid(id, &handle, &app) == FAIL)
        return FAIL;

    ap = (*app);

    if(ap == NULL)
        return FAIL;

    /* 
     * ap is the proper attribute list now look up something with this
     *  name
     */

    attr = (NC_attr **) ap->values;
    len = HDstrlen(attrname);
    
    for(attrid = 0 ; attrid < ap->count ; attrid++, attr++)
	{
            if( len == (*attr)->name->len &&
               HDstrncmp(attrname, (*attr)->name->values, (size_t)len) == 0)
		{
                    return(attrid) ; /* found it */
		}
	}

    return FAIL;
    
} /* SDfindattr */


/* ----------------------------- SDidtoref ----------------------------- */
/*

 NAME
	SDidtoref -- get a unique reference number for this dataset
 USAGE
	int32 SDidtoref(id)
        int32 id;             IN: dataset ID
 RETURNS
        A reference number or FAIL
 DESCRIPTION
        Given an index return the ref of the associated NDG for 
        inclusion in Vgroups and annotations

--------------------------------------------------------------------------- */
int32
#ifdef PROTOTYPE
SDidtoref(int32 id)
#else
SDidtoref(id)
int32   id;
#endif
{

    NC       * handle;
    NC_var   * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDidtoref: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FAIL;

    return((int32) var->ndg_ref);

} /* SDidtoref */


/* ----------------------------- SDreftoindex ----------------------------- */
/*

 NAME
	SDreftoindex -- map a reference number to a dataset index
 USAGE
	int32 SDreftoindex(fid, ref)
        int32 fid;             IN: file ID
        int32 ref;             IN: reference number
 RETURNS
        A dataset index or FAIL
 DESCRIPTION
        Given a ref number return the index of the cooresponding dataset

--------------------------------------------------------------------------- */
int32
#ifdef PROTOTYPE
SDreftoindex(int32 fid, int32 ref)
#else
SDreftoindex(fid, ref)
int32    fid;
int32   ref;
#endif
{

    NC       * handle;
    NC_var  ** dp;
    intn       ii;

#ifdef SDDEBUG
    fprintf(stderr, "SDreftoindex: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(fid, CDFTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    dp = (NC_var**) handle->vars->values;
    for(ii = 0 ; ii < handle->vars->count ; ii++, dp++)
        if((*dp)->ndg_ref == ref)
            return(ii) ;
    
    return FAIL;

} /* SDreftoindex */


/* ----------------------------- SDisrecord ----------------------------- */
/*

 NAME
	SDisrecord -- check is var is a record variable
 USAGE
	int32 SDisrecord(id)
        int32 id;             IN: dataset ID
 RETURNS
        TRUE/FALSE
 DESCRIPTION
        Return TRUE if the dataset in question is a record variable
        else FALSE

--------------------------------------------------------------------------- */
int32
#ifdef PROTOTYPE
SDisrecord(int32 id)
#else
SDisrecord(id)
int32   id;
#endif
{

    NC       * handle;
    NC_var   * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDisrecord: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL)
        return FALSE;

    if(handle->vars == NULL)
        return FALSE;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FALSE;

    if(var->shape[0] == SD_UNLIMITED)
        return TRUE;
    else
        return FALSE;

} /* SDisrecord */


/* ----------------------------- SDiscoordvar ----------------------------- */
/*

 NAME
	SDiscoordvar -- check is var is a coord var
 USAGE
	int32 SDiscoordvar(id)
        int32 id;             IN: dataset ID
 RETURNS
        TRUE/FALSE
 DESCRIPTION
        Return TRUE if the dataset in question is a coordinate variable

--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDiscoordvar(int32 id)
#else
SDiscoordvar(id)
int32   id;
#endif
{

    NC       * handle;
    NC_var   * var;
    NC_dim   * dim;
    int32      dimindex;

#ifdef SDDEBUG
    fprintf(stderr, "SDiscoordvar: I've been called\n");
#endif
    
    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL)
        return FALSE;

    if(handle->vars == NULL)
        return FALSE;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FALSE;

    if(var->assoc->count != 1)
        return FALSE;

    dimindex = var->assoc->values[0];

    dim = SDIget_dim(handle, dimindex);
    if(dim == NULL)
        return FALSE;

    if(var->name->len != dim->name->len)
        return FALSE;

    if(HDstrcmp(var->name->values, dim->name->values))
        return FALSE;

    return TRUE;

} /* SDiscoordvar */


#if 0
/* ---------------------------- RAGGED ARRAYS ----------------------------- */
/*

  Ragged arrays are a cross between datasets and index structures.  The
  basic idea is that all but one of the dimensions is constant.  The
  other dimension can vary over the course of the dataset.  This is
  useful for storing equalarea grids and for making alogrithms much
  more complex.  

  Ragged arrays can be multi-dimensional and, eventually, record variables
  too.  A 2-dimensional ragged array would look like:

  **********
  ********
  *********    <---------- This is a line
  *****
  ***
  *********
  *******

  The above ragged array has 7 "lines" the "line length" of the fifth line is 
  three.  It is not necessary to set all of the line lengths at the same time
  nor retreive them all at the same time.   However, to specify the line
  length for line X, the length must be specified for all Y < X (is this
  really necessary?)

  Internally, the above ragged array would be stored as a one-dimensional
  dataset.  In addition, there will be a rag_fill array that contains the
  line lengths.  This rag_fill array will get written to a separate
  structure in the file (tag DFTAG_SDRAG).

*/
/* ------------------------------------------------------------------------ */


/* ------------------------------- SDsetrag ------------------------------- */
/*

  Set the lengths of the lines of a ragged array.

  Currently, these lines must be specified in increasing order (i.e. can't
  use hyperslab type locations to set them).  This should probably be made
  nicer once everything else works.

*/
int32
#ifdef PROTOTYPE
SDsetrag(int32 sdsid, int32 low, int32 count, int32 *sizes)
#else
SDsetrag(sdsid, low, count, sizes)
int32 sdsid;
int32 low;
int32 count;
int32 *sizes;
#endif
{

    NC       * handle;
    NC_var   * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetrag: I've been called\n");
#endif
    
    /* get the variable */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE) 
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, sdsid);
    if((var == NULL) || (var->is_ragged == FALSE))
        return FAIL;

    /* verify writing to a valid area */
    if(var->rag_fill != low) {
        printf("var->rag_fill %d    low %d\n", var->rag_fill, low); 
        return FAIL;
    }

    /* allocate some space for the ragged dimension if needed */
    /* BUG: will need to be changed for ragged records */
    if(var->rag_list == NULL) {
        var->rag_list = (int32 *) HDmalloc(sizeof(int32) * var->dsizes[0]);
        if(var->rag_list == NULL) return FAIL;
    }

    /* copy over the new values */
    HDmemcpy(&(var->rag_list[low]), sizes, sizeof(int32) * count);
    
    /* update count */
    var->rag_fill += count;

    return SUCCEED;

} /* SDsetrag */


/* ------------------------------- SDgetrag ------------------------------- */
/* 

  Get the line size for a chunk of a ragged array

*/
#endif /* 0 */


/* -------------------------- SDsetaccesstype --------------------------- */
/*

 NAME
	SDsetaccesstype -- set the I/O access type of an SD
 USAGE
	intn SDsetaccesstype(id, accesstype)
        int32   id;              IN: dataset ID
        uintn   accesstype;         IN: access type
 RETURNS
        Return SUCCEED if the SD data can be accessed via accesstype.
	Otherwise return FAIL.

 DESCRIPTION
        Set the type of I/O (serial, parallel, ...) for accessing the
	data of the SD.  Access types can be DFACC_SERIAL, DFACC_PARALLEL,
	DFACC_DEFAULT.
	
--------------------------------------------------------------------------- */
#ifdef CM5
extern CM_DEBUG;
#endif

intn SDsetaccesstype(int32 id, uintn accesstype)
{

    NC       * handle;
    NC_var   * var;
    intn       status;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetaccesstype: I've been called\n");
#endif

#ifdef CM5
if (CM_DEBUG > 0)
    printf("SDsetaccesstype under construction\n");
#endif

    switch (accesstype){
        case DFACC_DEFAULT:
        case DFACC_SERIAL:
        case DFACC_PARALLEL:
            break;
        otherwise:
            return (FAIL);
    }

    handle = SDIhandle_from_id(id, SDSTYPE);
    if(handle == NULL || handle->file_type != HDF_FILE)
        return FAIL;

    if(handle->vars == NULL)
        return FAIL;

    var = SDIget_var(handle, id);
    if(var == NULL)
        return FAIL;

    /* if aid is not valid yet, there is no access_rec setup yet. */
    /* Go ahead and try set it up. */
    if(var->aid == FAIL && hdf_get_vp_aid(handle, var) == FALSE)
        return(FAIL);
    else 
        return((intn)Hsetaccesstype(var->aid, accesstype));

} /* SDsetaccesstype */


/* ----------------------------- SDsetblocksize ----------------------------- */
/*

 NAME
	SDsetblocksize -- set the size of the linked blocks created.
 USAGE
	intn SDsetblocksize(sdsid, block_size)
        int32 sdsid;             IN: dataset ID
        int32 block_size;        IN: size of the block in bytes
 RETURNS
        SUCCEED/FAIL
 DESCRIPTION
        Set the size of the blocks used for storing the data for unlimited
	dimension datasets.  This is used when creating new datasets only,
	it does not have any affect on existing datasets.  The block_size 
	should probably be set to a multiple of the "slice" size.

--------------------------------------------------------------------------- */
intn
#ifdef PROTOTYPE
SDsetblocksize(int32 sdsid, int32 block_size)
#else
SDsetblocksize(sdsid, block_size)
int32 sdsid;
int32  block_size;
#endif
{
    NC      * handle;
    NC_var  * var;

#ifdef SDDEBUG
    fprintf(stderr, "SDsetblocksize: I've been called\n");
#endif

    /* get the handle */
    handle = SDIhandle_from_id(sdsid, SDSTYPE);
    if(handle == NULL) 
        return FAIL;

    /* get the variable */
    var = SDIget_var(handle, sdsid);
    if(var == NULL)
        return FAIL;

    /* set the block size */
    var->block_size=block_size;

    return(SUCCEED);
} /* SDsetblocksize */

