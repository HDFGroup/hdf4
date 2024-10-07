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

NC_attr *
NC_new_attr(const char *name, nc_type type, unsigned count, const void *values)
{
    NC_attr *ret;

    ret = malloc(sizeof(NC_attr));
    if (ret == NULL)
        goto alloc_err;

    ret->name = NC_new_string((unsigned)strlen(name), name);
    if (ret->name == NULL)
        goto alloc_err;

    ret->data = NC_new_array(type, count, values);
    if (ret->data == NULL)
        goto alloc_err;
    ret->HDFtype = hdf_map_type(type);
    return ret;
alloc_err:
    nc_serror("NC_new_attr");
    return NULL;
}

/*
 * Free attr
 *
 * NOTE: Changed return value to return 'int'
 *       If successful returns SUCCEED else FAIL -GV 9/19/97
 */
int
NC_free_attr(NC_attr *attr)
{
    int ret_value = SUCCEED;

    if (attr != NULL) {
        if (NC_free_string(attr->name) == FAIL) {
            ret_value = FAIL;
            goto done;
        }

        if (NC_free_array(attr->data) == FAIL) {
            ret_value = FAIL;
            goto done;
        }

        free(attr);
    }

done:
    return ret_value;
}

/*
 *  Verify that this is a user nc_type
 */
bool_t
NCcktype(nc_type datatype)
{
    switch (datatype) {
        case NC_BYTE:
        case NC_CHAR:
        case NC_SHORT:
        case NC_LONG:
        case NC_FLOAT:
        case NC_DOUBLE:
            return TRUE;
        default:
            break;
    }
    NCadvise(NC_EBADTYPE, "Unknown type %d", datatype);
    return FALSE;
}

/*
 * Given cdfid and varid, return handle to array of attributes
 *  else NULL on error
 */
static NC_array **
NC_attrarray(int cdfid, int varid)
{
    NC        *handle;
    NC_array **ap;

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return NULL;

    if (varid == NC_GLOBAL) /* Global attribute, attach to cdf */
    {
        ap = &(handle->attrs);
    }
    else if (handle->vars != NULL && varid >= 0 && (unsigned)varid < handle->vars->count) {
        ap = (NC_array **)handle->vars->values;
        ap += varid;
        ap = &(((NC_var *)(*ap))->attrs); /* Whew! */
    }
    else {
        NCadvise(NC_EINVAL, "%d is not a valid variable id", varid);
        ap = NULL;
    }
    return ap;
}

/*
 * Step thru NC_ATTRIBUTE array, seeking match on name.
 *  return match or NULL if Not Found.
 */
NC_attr **
NC_findattr(NC_array **ap, const char *name)
{
    NC_attr **attr;
    unsigned  attrid;
    size_t    len;

    if (*ap == NULL)
        return NULL;

    attr = (NC_attr **)(*ap)->values;

    len = strlen(name);

    for (attrid = 0; attrid < (*ap)->count; attrid++, attr++) {
        if (len == (*attr)->name->len && strncmp(name, (*attr)->name->values, len) == 0) {
            return attr; /* Normal return */
        }
    }
    return NULL;
}

/*
 * Common code for attput and attcopy
 */
static int
NC_aput(int cdfid, NC_array **ap, const char *name, nc_type datatype, unsigned count, const void *values)
{
    NC       *handle;
    NC_attr  *attr[1];
    NC_attr **atp, *old;

    handle = NC_check_id(cdfid);
    if (handle == NULL)
        return -1;
    if (!(handle->flags & NC_RDWR))
        return -1;

    if (*ap == NULL) /* first time */
    {
        if (!NC_indefine(cdfid, TRUE))
            return -1;

        attr[0] = NC_new_attr(name, datatype, count, values);
        if (attr[0] == NULL)
            return -1;
        *ap = NC_new_array(NC_ATTRIBUTE, (unsigned)1, (uint8_t *)attr);
        if (*ap == NULL)
            return -1;
        return (int)(*ap)->count - 1;
    }

    if ((atp = NC_findattr(ap, name)) != NULL) /* name in use */
    {
        if (NC_indefine(cdfid, FALSE)) {
            old  = *atp;
            *atp = NC_new_attr(name, datatype, count, values);
            if (*atp == NULL) {
                *atp = old;
                return -1;
            }
            NC_free_attr(old);
            return (int)(*ap)->count - 1;
        }

        if (NC_re_array((*atp)->data, datatype, count, values) == NULL) {
            NCadvise(NC_ENOTINDEFINE, "Can't increase size unless in define mode");
            return -1;
        }

        (*atp)->HDFtype = hdf_map_type(datatype);
        if (handle->flags & NC_HSYNC) {
            handle->xdrs->x_op = XDR_ENCODE;
            if (!xdr_cdf(handle->xdrs, &handle))
                return -1;
            handle->flags &= ~(NC_NDIRTY | NC_HDIRTY);
        }
        else
            handle->flags |= NC_HDIRTY;
        return (int)(*ap)->count - 1;
    }

    if ((*ap)->count >= H4_MAX_NC_ATTRS) {
        NCadvise(NC_EMAXATTS, "maximum number of attributes %d exceeded", (*ap)->count);
        return -1;
    }

    if (NC_indefine(cdfid, TRUE)) {
        attr[0] = NC_new_attr(name, datatype, count, values);
        if (attr[0] == NULL)
            return -1;
        if (NC_incr_array((*ap), (uint8_t *)attr) == NULL)
            return -1;
        return (int)(*ap)->count - 1;
    }

    return -1;
}

int
ncattput(int cdfid, int varid, const char *name, nc_type datatype, int count, const void *values)
{
    NC_array **ap;

    cdf_routine_name = "ncattput";

    ap = NC_attrarray(cdfid, varid);
    if (ap == NULL)
        return -1;

    if (count < 0) {
        NCadvise(NC_EINVAL, "Invalid length %d", count);
        return -1;
    }

    if (!NCcktype(datatype))
        return -1;

    return NC_aput(cdfid, ap, name, datatype, (unsigned)count, values);
}

bool_t
xdr_NC_attr(XDR *xdrs, NC_attr **app)
{
    bool_t ret_value;

    if (xdrs->x_op == XDR_FREE) {
        NC_free_attr((*app));
        return TRUE;
    }

    if (xdrs->x_op == XDR_DECODE) {
        *app = malloc(sizeof(NC_attr));
        if (*app == NULL) {
            nc_serror("xdr_NC_attr");
            return FALSE;
        }
    }

    if (!xdr_NC_string(xdrs, &((*app)->name)))
        return FALSE;
    ret_value       = xdr_NC_array(xdrs, &((*app)->data));
    (*app)->HDFtype = hdf_map_type(((*app)->data)->type);
    return ret_value;
}

/*
 * How much space will the xdr'd attr take.
 */
int
NC_xlen_attr(NC_attr **app)
{
    int len;

    if (*app == NULL)
        return 4;

    len = NC_xlen_string((*app)->name);
    len += NC_xlen_array((*app)->data);

    return len;
}
