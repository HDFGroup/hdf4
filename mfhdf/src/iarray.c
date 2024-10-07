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

/* values: VAX C doesn't like values[] */
NC_iarray *
NC_new_iarray(unsigned count, const int *values)
{
    NC_iarray *ret;
    int       *ip;
    size_t     memlen;

    ret = malloc(sizeof(NC_iarray));
    if (ret == NULL)
        goto alloc_err;
    ret->count = count;
    if (count != 0) /* allocate */
    {
        memlen      = count * sizeof(int);
        ret->values = malloc(memlen);
        if (ret->values == NULL)
            goto alloc_err;
        if (values != NULL) /* copy them in */
        {
            for (ip = ret->values; count > 0; count--)
                *ip++ = *values++;
        }
    }
    else {
        ret->values = NULL;
    }

    return ret;
alloc_err:
    nc_serror("NC_new_iarray");
    return NULL;
}

/*
 * Free iarray and its values
 */
int
NC_free_iarray(NC_iarray *iarray)
{
    if (iarray != NULL) {
        free(iarray->values);
        free(iarray);
    }

    return SUCCEED;
}

bool_t
xdr_NC_iarray(XDR *xdrs, NC_iarray **ipp)
{
    int     *ip;
    unsigned count = 0;
    bool_t   stat  = TRUE;

    switch (xdrs->x_op) {
        case XDR_FREE:
            NC_free_iarray((*ipp));
            return TRUE;
        case XDR_DECODE:
            /* need the length to pass to new */
            if (!hdf_xdr_u_int(xdrs, &count)) {
                return FALSE;
            }
            (*ipp) = NC_new_iarray((unsigned)count, (int *)NULL);
            if ((*ipp) == NULL)
                return FALSE;
            /* then deal with the array */
            for (ip = (*ipp)->values; (count > 0) && stat; count--)
                stat = hdf_xdr_int(xdrs, ip++);
            return stat;
        case XDR_ENCODE:
            /* first deal with the length */
            count = (*ipp)->count;
            if (!hdf_xdr_u_int(xdrs, &count)) {
                return FALSE;
            }
            /* then deal with the array */
            for (ip = (*ipp)->values; (count > 0) && stat; count--)
                stat = hdf_xdr_int(xdrs, ip++);
            return stat;
    }
    return FALSE;
}

/*
 * How much space will the xdr'd iarray take.
 */
int
NC_xlen_iarray(NC_iarray *iarray)
{
    int len = 4;
    if (iarray != NULL) {
        len += iarray->count * 4;
    }
    return len;
}
