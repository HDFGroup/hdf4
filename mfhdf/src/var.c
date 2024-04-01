/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1993, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
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

#include "nc_priv.h"

NC_var *
NC_new_var(const char *name, nc_type type, int ndims, const int *dims)
{
    NC_var *ret;

    ret = calloc(1, sizeof(NC_var));
    if (ret == NULL)
        goto alloc_err;

    ret->name = NC_new_string((unsigned)strlen(name), name);
    if (ret->name == NULL)
        goto alloc_err;

    ret->assoc = NC_new_iarray((unsigned)ndims, dims);
    if (ret->assoc == NULL)
        goto alloc_err;

    ret->shape  = NULL;
    ret->dsizes = NULL;
    ret->attrs  = NULL;
    ret->type   = type;
    ret->len    = 0;
    ret->szof   = NC_typelen(type);
    ret->begin  = 0;

    ret->vgid        = 0;
    ret->data_ref    = 0;
    ret->data_tag    = DATA_TAG; /* Assume normal data unless set   */
    ret->data_offset = 0;        /* Assume data starts at beginning */
    ret->block_size  = -1;       /* start off with no block size set */
    ret->numrecs     = 0;        /* Only used in unlimited dimension case */
    ret->aid         = FAIL;
    ret->ndg_ref     = 0;
    ret->var_type    = UNKNOWN; /* Unknown whether this var is an SDS or a coord var */
    ret->HDFtype     = hdf_map_type(type);
    ret->HDFsize     = DFKNTsize(ret->HDFtype);
    ret->is_ragged   = FALSE;
    ret->created     = FALSE; /* This is set in SDcreate() if it's a new SDS */
    ret->set_length  = FALSE; /* This is set in SDwritedata() if the data needs its length set */

    return ret;
alloc_err:
    nc_serror("NC_new_var");
    return NULL;
}

/*
 * Free var
 *
 * NOTE: Changed return value to return 'int'
 *       If successful returns SUCCEED else FAIL -GV 9/19/97
 */
int
NC_free_var(NC_var *var)
{
    int ret_value = SUCCEED;

    if (var != NULL) {
        if (NC_free_string(var->name) == FAIL) {
            ret_value = FAIL;
            goto done;
        }
        if (NC_free_iarray(var->assoc) == FAIL) {
            ret_value = FAIL;
            goto done;
        }
        free(var->shape);
        free(var->dsizes);

        if (NC_free_array(var->attrs) == FAIL) {
            ret_value = FAIL;
            goto done;
        }
        free(var);
    }

done:
    return ret_value;
}

/*
 * 'compile' the shape and len of a variable
 *  return -1 on error
 */
int
NC_var_shape(NC_var *var, NC_array *dims)
{
    unsigned long *shape, *dsizes;
    int            ii;
    unsigned long *shp, *dsp, *op;
    int           *ip;
    NC_dim       **dp;
    size_t         xszof;

    xszof = var->HDFsize;

    /* Fixed memory leaks reported in bug# 418. BMR - Apr 8, 01 */

    /*
     * Allocate the shape array
     */
    ii = var->assoc->count;
    if (ii == 0) {
        /* scalar var, len == szof */
        var->len = xszof;
        goto out;
    }
    shape = malloc((size_t)ii * sizeof(unsigned long));
    if (shape == NULL) {
        nc_serror("NC_var_shape");
        return -1;
    }

    /*
     * use the user supplied dimension indices
     * to determine the shape
     */
    for (ip = var->assoc->values, op = shape; ii > 0; ii--) {
        if (*ip < 0 || *ip >= ((dims != NULL) ? dims->count : 1)) {
            NCadvise(NC_EBADDIM, "Bad dimension id %d", *ip);
            free(shape);
            return -1;
        }
        dp  = (NC_dim **)dims->values + *ip;
        *op = (*dp)->size;
        if (*op == NC_UNLIMITED && ii != var->assoc->count) {
            NCadvise(NC_EUNLIMPOS, "NC_UNLIMITED size applied to index other than 0 %d",
                     var->assoc->count - ii);
            free(shape);
            return -1;
        }
        op++;
        ip++;
    }

    /* Free memory if this var already has shape previously allocated */
    free(var->shape);
    var->shape = shape;

    /*
     * Allocate the dsizes array
     */
    ii     = var->assoc->count;
    dsizes = malloc((size_t)ii * sizeof(unsigned long));
    if (dsizes == NULL) {
        free(shape);
        var->shape = NULL;
        nc_serror("NC_var_shape");
        return -1;
    }

    /* Free memory if this var already has dsizes previously allocated */
    free(var->dsizes);
    var->dsizes = dsizes;

    /*
     * Compute var->len and the dsizes
     */
    shp      = shape + var->assoc->count - 1; /* count is > 0 here */
    dsp      = dsizes + var->assoc->count - 1;
    var->len = (*shp) ? (*shp) : 1; /* boundary condition for rec */
    var->len = var->len * xszof;
    if (dsp != NULL)
        *dsp = xszof;

    for (shp--, dsp--; shp >= shape; shp--, dsp--) {
        *dsp = var->len;
        if (shp != shape || *shp) /* include last mult for non-rec vars */
            var->len *= *shp;
    }

out:
    /* don't round-up for HDF-encoded files */
    if (var->cdf->file_type != HDF_FILE)

        switch (var->type) {
            case NC_BYTE:
            case NC_CHAR:
            case NC_SHORT:
                if (var->len % 4 != 0) {
                    var->len += 4 - var->len % 4; /* round up */
                                                  /*        *dsp += 4 - *dsp%4 ; */
                }
            default:
                break;
        }

    return var->assoc->count;
}

int
ncvardef(int cdfid, const char *name, nc_type type, int ndims, const int dims[])
{
    NC      *handle;
    NC_var  *var[1];
    NC_var **dp;
    int      len;

    cdf_routine_name = "ncvardef";

    if (!NC_indefine(cdfid, TRUE))
        return -1;

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;

    if (!NCcktype(type))
        return -1;

    if (ndims < 0) /* 0 => scalar */
    {
        NCadvise(NC_EINVAL, "Number of dimensions %d < 0", ndims);
        return -1;
    }

    if (ndims > 0) {
        if (handle->dims == NULL || ndims > handle->dims->count) {
            NCadvise(NC_EINVAL, "Invalid number of dimensions %d > %d", ndims,
                     (handle->dims != NULL) ? handle->dims->count : 0);
            return -1;
        }
    }

    if (handle->vars == NULL) /* first time */
    {
        var[0] = NC_new_var(name, type, ndims, dims);
        if (var[0] == NULL)
            return -1;
        handle->vars = NC_new_array(NC_VARIABLE, (unsigned)1, (uint8_t *)var);
        if (handle->vars == NULL)
            return -1;
    }
    else if (handle->vars->count >= H4_MAX_NC_VARS) {
        NCadvise(NC_EMAXVARS, "maximum number of variables %d exceeded", handle->vars->count);
        return -1;
    }
    else {
        /* check for name in use */
        len = (int)strlen(name);
        dp  = (NC_var **)handle->vars->values;
        for (int ii = 0; ii < handle->vars->count; ii++, dp++) {
            if (len == (*dp)->name->len && strncmp(name, (*dp)->name->values, len) == 0) {
                NCadvise(NC_ENAMEINUSE, "variable \"%s\" in use with index %d", (*dp)->name->values, ii);
                return -1;
            }
        }
        var[0] = NC_new_var(name, type, ndims, dims);
        if (var[0] == NULL)
            return -1;
        if (NC_incr_array(handle->vars, (uint8_t *)var) == NULL)
            return -1;
    }
    (*var)->cdf = handle; /* for NC_var_shape */
    if (NC_var_shape(*var, handle->dims) != -1) {
        (*var)->ndg_ref = Hnewref(handle->hdf_file);
        return handle->vars->count - 1;
    }
    /* unwind */
    handle->vars->count--;
    NC_free_var(var[0]);
    return -1;
}

/*
 * Recompute the shapes of all variables
 * Sets handle->begin_rec to start of first record variable
 * returns -1 on error
 */
int
NC_computeshapes(NC *handle)
{
    NC_var **vbase, **vpp;
    NC_var  *first = NULL;

    handle->begin_rec = 0;
    handle->recsize   = 0;

    if (handle->vars == NULL)
        return 0;
    vbase = (NC_var **)handle->vars->values;
    for (vpp = vbase; vpp < &vbase[handle->vars->count]; vpp++) {
        (*vpp)->cdf = handle;

        if (NC_var_shape(*vpp, handle->dims) == -1)
            return -1;
        if (IS_RECVAR(*vpp)) {
            if (first == NULL)
                first = *vpp;
            handle->recsize += (*vpp)->len;
        }
    }
    if (first != NULL) {
        handle->begin_rec = first->begin;
        /*
         * for special case of exactly one record variable, pack values
         */
        if (handle->recsize == first->len)
            handle->recsize = *first->dsizes;
    }
    return handle->vars->count;
}

int
ncvarid(int cdfid, const char *name)
{
    NC      *handle;
    NC_var **dp;
    int      len;

    cdf_routine_name = "ncvarid";

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;
    if (handle->vars == NULL)
        return -1;
    len = (int)strlen(name);
    dp  = (NC_var **)handle->vars->values;
    for (int ii = 0; ii < handle->vars->count; ii++, dp++) {
        if (len == (*dp)->name->len && strncmp(name, (*dp)->name->values, len) == 0) {
            return ii;
        }
    }
    NCadvise(NC_ENOTVAR, "variable \"%s\" not found", name);
    return -1;
}

/*
 * Given valid handle and varid, return var
 *  else NULL on error
 */
NC_var *
NC_hlookupvar(NC *handle, int varid)
{
    NC_array **ap;

    if (varid == NC_GLOBAL) /* Global is error in this context */
    {
        return NULL;
    }
    else if (handle->vars != NULL && varid >= 0 && varid < handle->vars->count) {
        ap = (NC_array **)handle->vars->values;
        ap += varid;
    }
    else {
        NCadvise(NC_ENOTVAR, "%d is not a valid variable id", varid);
        return NULL;
    }
    return (NC_var *)*ap;
}

/*
 * Given cdfid and varid, return var
 *  else NULL on error
 */
static NC_var *
NC_lookupvar(int cdfid, int varid)
{
    NC *handle;

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return NULL;

    return NC_hlookupvar(handle, varid);
}

int
ncvarinq(int cdfid, int varid, char *name, nc_type *typep, int *ndimsp, int dims[], int *nattrsp)
{
    NC_var *vp;

    cdf_routine_name = "ncvarinq";

    vp = NC_lookupvar(cdfid, varid);
    if (vp == NULL)
        return -1;

    if (name != NULL) {
        (void)memcpy(name, vp->name->values, vp->name->len);
        name[vp->name->len] = 0;
    }

    if (typep != NULL)
        *typep = vp->type;
    if (ndimsp != NULL) {
        *ndimsp = vp->assoc->count;
    }
    if (dims != NULL) {
        for (int ii = 0; ii < vp->assoc->count; ii++) {
            dims[ii] = vp->assoc->values[ii];
        }
    }
    if (nattrsp != NULL) {
        if (vp->attrs != NULL) {
            *nattrsp = vp->attrs->count;
        }
        else {
            *nattrsp = 0;
        }
    }

    return varid;
}

int
ncvarrename(int cdfid, int varid, const char *newname)
{
    NC        *handle;
    NC_var   **vpp;
    int        len;
    NC_string *old, *new;

    cdf_routine_name = "ncvarrename";

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;
    if (!(handle->flags & NC_RDWR))
        return -1;

    /* check for name in use */
    len = (int)strlen(newname);
    vpp = (NC_var **)handle->vars->values;
    for (int ii = 0; ii < handle->vars->count; ii++, vpp++) {
        if (len == (*vpp)->name->len && strncmp(newname, (*vpp)->name->values, len) == 0) {
            NCadvise(NC_ENAMEINUSE, "variable name \"%s\" in use with index %d", (*vpp)->name->values, ii);
            return -1;
        }
    }

    if (varid == NC_GLOBAL) /* Global is error in this context */
    {
        NCadvise(NC_EGLOBAL, "action prohibited on NC_GLOBAL varid");
        return -1;
    }
    else if (handle->vars != NULL && varid >= 0 && varid < handle->vars->count) {
        vpp = (NC_var **)handle->vars->values;
        vpp += varid;
    }
    else {
        NCadvise(NC_ENOTVAR, "%d is not a valid variable id", varid);
        return -1;
    }

    old = (*vpp)->name;
    if (NC_indefine(cdfid, TRUE)) {
        new = NC_new_string((unsigned)strlen(newname), newname);
        if (new == NULL)
            return -1;
        (*vpp)->name = new;
        NC_free_string(old);
        return varid;
    }
    new = NC_re_string(old, (unsigned)strlen(newname), newname);
    if (new == NULL)
        return -1;
    if (handle->flags & NC_HSYNC) {
        handle->xdrs->x_op = XDR_ENCODE;
        if (!xdr_cdf(handle->xdrs, &handle))
            return -1;
        handle->flags &= ~(NC_NDIRTY | NC_HDIRTY);
    }
    else
        handle->flags |= NC_HDIRTY;
    return varid;
}

bool_t
xdr_NC_var(XDR *xdrs, NC_var **vpp)
{
    unsigned begin     = 0;
    int      temp_type = 0;
    unsigned temp_len  = 0;

    if (xdrs->x_op == XDR_FREE) {
        NC_free_var((*vpp));
        return TRUE;
    }

    if (xdrs->x_op == XDR_DECODE) {
        *vpp = calloc(1, sizeof(NC_var));
        if (*vpp == NULL) {
            nc_serror("xdr_NC_var");
            return FALSE;
        }
    }

    if (!xdr_NC_string(xdrs, &((*vpp)->name)))
        return FALSE;
    if (!xdr_NC_iarray(xdrs, &((*vpp)->assoc)))
        return FALSE;
    if (!xdr_NC_array(xdrs, &((*vpp)->attrs)))
        return FALSE;

    if (!h4_xdr_int(xdrs, &temp_type)) {
        return FALSE;
    }
    (*vpp)->type = (nc_type)temp_type;

    if (!h4_xdr_u_int(xdrs, &temp_len)) {
        return FALSE;
    }
    (*vpp)->len = (unsigned long)temp_len;

    if (xdrs->x_op == XDR_DECODE)
        (*vpp)->szof = NC_typelen((*vpp)->type);

    if (xdrs->x_op == XDR_ENCODE)
        begin = (*vpp)->begin;
    if (!h4_xdr_u_int(xdrs, &begin))
        return FALSE;
    if (xdrs->x_op == XDR_DECODE)
        (*vpp)->begin = begin;

    if (xdrs->x_op == XDR_DECODE) {

        (*vpp)->HDFtype   = hdf_map_type((*vpp)->type);
        (*vpp)->HDFsize   = DFKNTsize((*vpp)->HDFtype);
        (*vpp)->aid       = FAIL;
        (*vpp)->is_ragged = FALSE;
    }

    return TRUE;
}

/*
 * How much space will the xdr'd var take.
 *
 */
int
NC_xlen_var(NC_var **vpp)
{
    int len;

    if (*vpp == NULL)
        return 4;

    len = NC_xlen_string((*vpp)->name);
    len += NC_xlen_iarray((*vpp)->assoc);
    len += NC_xlen_array((*vpp)->attrs);
    len += 12;

    return len;
}
