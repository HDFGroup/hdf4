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

NC_dim *
NC_new_dim(const char *name, long size)
{
    NC_dim *ret;

    ret = malloc(sizeof(NC_dim));
    if (ret == NULL)
        goto alloc_err;

    ret->name = NC_new_string((unsigned)strlen(name), name);
    if (ret->name == NULL)
        goto alloc_err;

    ret->size  = (int32)size;
    ret->vgid  = 0; /* no vgroup representing this dimension yet -BMR 2010/12/29 */
    ret->count = 1;
    /*        ret->dim00_compat = (size == NC_UNLIMITED)? 0 : 1;  */
    ret->dim00_compat = 0;
    return ret;
alloc_err:
    nc_serror("NC_new_dim");
    return NULL;
}

/*
 * Free dim
 *
 * If successful returns SUCCEED else FAIL
 */
int
NC_free_dim(NC_dim *dim)
{
    int ret_value = SUCCEED;

    if (dim != NULL) {
        if (dim->count > 1) {
            dim->count -= 1;
            ret_value = SUCCEED;
            goto done;
        }

        if (NC_free_string(dim->name) == FAIL) {
            ret_value = FAIL;
            goto done;
        }
        free(dim);
    }

done:
    return ret_value;
}

int
ncdimdef(int cdfid, const char *name, long size)
{
    NC      *handle;
    NC_dim  *dim[1];
    NC_dim **dp;
    size_t   len;

    cdf_routine_name = "ncdimdef";

    if (!NC_indefine(cdfid, TRUE))
        return -1;

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;

    if (size < 0) /* NC_UNLIMITED #define'd as 0 */
    {
        NCadvise(NC_EINVAL, "Invalid size %d", size);
        return -1;
    }

    if (handle->dims == NULL) /* first time */
    {
        dim[0] = NC_new_dim(name, size);
        if (dim[0] == NULL)
            return -1;
        handle->dims = NC_new_array(NC_DIMENSION, (unsigned)1, (uint8_t *)dim);
        if (handle->dims == NULL)
            return -1;
    }
    else if (handle->dims->count >= H4_MAX_NC_DIMS) {
        NCadvise(NC_EMAXDIMS, "maximum number of dimensions %d exceeded", handle->dims->count);
        return -1;
    }
    else {
        /* check for name in use */
        len = strlen(name);
        dp  = (NC_dim **)handle->dims->values;
        for (unsigned ii = 0; ii < handle->dims->count; ii++, dp++) {
            if (len == (*dp)->name->len && strncmp(name, (*dp)->name->values, len) == 0) {
                NCadvise(NC_ENAMEINUSE, "dimension \"%s\" in use with index %d", (*dp)->name->values, ii);
                return -1;
            }
            if ((*dp)->size == NC_UNLIMITED && size == NC_UNLIMITED) {
                NCadvise(NC_EUNLIMIT, "NC_UNLIMITED size already in use: dimension \"%s\" (index %d)",
                         (*dp)->name->values, ii);
                return -1;
            }
        }

        dim[0] = NC_new_dim(name, size);
        if (dim[0] == NULL)
            return -1;
        if (NC_incr_array(handle->dims, (uint8_t *)dim) == NULL)
            return -1;
    }
    return (int)handle->dims->count - 1;
}

int
NC_dimid(NC *handle, char *name)
{
    size_t   len;
    NC_dim **dp;

    len = strlen(name);
    dp  = (NC_dim **)handle->dims->values;
    for (unsigned ii = 0; ii < handle->dims->count; ii++, dp++) {
        if (len == (*dp)->name->len && strncmp(name, (*dp)->name->values, len) == 0)
            return (int)ii;
    }
    NCadvise(NC_EBADDIM, "dim \"%s\" not found", name);
    return -1;
}

int
ncdiminq(int cdfid, int dimid, char *name, long *sizep)
{
    NC      *handle;
    NC_dim **dp;

    cdf_routine_name = "ncdiminq";

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;
    if (handle->dims == NULL)
        return -1;
    if (dimid >= handle->dims->count)
        return -1;

    dp = (NC_dim **)handle->dims->values;
    dp += dimid;

    if (name != NULL) {
        (void)memcpy(name, (*dp)->name->values, (size_t)(*dp)->name->len);
        name[(*dp)->name->len] = 0;
    }
    if (sizep != 0) {
        if ((*dp)->size == NC_UNLIMITED)
            *sizep = handle->numrecs;
        else
            *sizep = (*dp)->size;
    }
    return dimid;
}

bool_t
xdr_NC_dim(XDR *xdrs, NC_dim **dpp)
{
    if (xdrs->x_op == XDR_FREE) {
        NC_free_dim((*dpp));
        return TRUE;
    }

    if (xdrs->x_op == XDR_DECODE) {
        *dpp = malloc(sizeof(NC_dim));
        if (*dpp == NULL) {
            nc_serror("xdr_NC_dim");
            return FALSE;
        }
    }

    /* hmm...what does this do? */
    if (xdrs->x_op == XDR_DECODE)
        (*dpp)->count = 0;

    if (!xdr_NC_string(xdrs, &((*dpp)->name)))
        return FALSE;
    return hdf_xdr_int(xdrs, &((*dpp)->size));
}

/*
 * How much space will the xdr'd dim take.
 */
int
NC_xlen_dim(NC_dim **dpp)
{
    int len = 4;
    if (*dpp != NULL) {
        len += NC_xlen_string((*dpp)->name);
    }
    return len;
}
