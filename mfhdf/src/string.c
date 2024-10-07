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

static uint32
compute_hash(unsigned count, const char *str)
{
    uint32 ret = 0;
    uint32 temp;

    /* check if string is NULL */
    if (str == NULL)
        return ret;

    while (count > sizeof(uint32)) {
        memcpy(&temp, str, sizeof(uint32));
        ret += temp;
        str += sizeof(uint32);
        count -= sizeof(uint32);
    } /* end while */

    if (count > 0) {
        temp = 0;
        memcpy(&temp, str, count);
        ret += temp;
    } /* end if */
    return ret;
} /* end compute_hash() */

NC_string *
NC_new_string(unsigned count, const char *str)
{
    NC_string *ret;
    size_t     memlen;

    if (count > H4_MAX_NC_NAME) {
        NCadvise(NC_EMAXNAME, "string \"%c%c%c%c%c%c ...\"  length %d exceeds %d", str[0], str[1], str[2],
                 str[3], str[4], str[5], count, H4_MAX_NC_NAME);
        return NULL;
    }

    ret = malloc(sizeof(NC_string));
    if (ret == NULL)
        goto alloc_err;
    ret->count = count;
    ret->len   = count;
    ret->hash  = compute_hash(count, str);
    if (count != 0) /* allocate */
    {
        memlen      = count + 1;
        ret->values = malloc(memlen);
        if (ret->values == NULL)
            goto alloc_err;
        if (str != NULL) {
            memcpy(ret->values, str, (size_t)count);
            ret->values[count] = 0;
        }
    }
    else { /* use what what you were given */
        ret->values = NULL;
    }

    return ret;
alloc_err:
    nc_serror("NC_new_string");
    free(ret);
    return NULL;
}

/*
 * Free string and its values
 */
int
NC_free_string(NC_string *cdfstr)
{
    if (cdfstr != NULL) {
        free(cdfstr->values);
        free(cdfstr);
    }

    return SUCCEED;
}

NC_string *
NC_re_string(NC_string *old, unsigned count, const char *str)
{
    if (old->count < count) /* punt */
    {
        NCadvise(NC_ENOTINDEFINE, "Must be in define mode to increase name length %d", old->count);
        return NULL;
    }

    if (str == NULL)
        return NULL;

    memcpy(old->values, str, count);
    memset(old->values + count, 0, (size_t)(old->count - count + 1));

    /* make sure len is always == to the string length */
    old->len  = count;
    old->hash = compute_hash(count, str);

    return old;
}

bool_t
xdr_NC_string(XDR *xdrs, NC_string **spp)
{
    unsigned count = 0;
    int      status;

    switch (xdrs->x_op) {
        case XDR_FREE:
            NC_free_string((*spp));
            return TRUE;
        case XDR_DECODE:
            /* need the length to pass to new */
            if (!hdf_xdr_u_int(xdrs, &count)) {
                return FALSE;
            }
            if (count == 0) {
                *spp = NULL;
                return TRUE;
            } /* else */
            (*spp) = NC_new_string((unsigned)count, (const char *)NULL);
            if ((*spp) == NULL)
                return FALSE;
            (*spp)->values[count] = 0;
            /* then deal with the characters */
            status = hdf_xdr_opaque(xdrs, (*spp)->values, (*spp)->count);

            /* might be padded */
            (*spp)->len = strlen((*spp)->values);
            return status;
        case XDR_ENCODE:
            /* first deal with the length */
            if (*spp == NULL) {
                count = 0;
                return hdf_xdr_u_int(xdrs, &count);
            } /* else */
            count = (*spp)->count;
            if (!hdf_xdr_u_int(xdrs, &count)) {
                return FALSE;
            }
            /* then deal with the characters */
            return hdf_xdr_opaque(xdrs, (*spp)->values, (*spp)->count);
    }
    return FALSE;
}

/*
 * How much space will the xdr'd string take.
 *
 */
int
NC_xlen_string(NC_string *cdfstr)
{
    int len = 4;
    int rem;
    if (cdfstr != NULL) {
        len += cdfstr->count;
        if ((rem = len % 4) != 0)
            len += 4 - rem;
    }
    return len;
}
