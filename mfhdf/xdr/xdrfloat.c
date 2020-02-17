/* @(#)xdr_float.c    1.1 87/11/04 3.9 RPCSRC */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_float.c 1.12 87/08/11 Copyr 1984 Sun Micro";
#endif

#include "H4api_adpt.h"

/*
 * xdr_float.c, Generic XDR routines impelmentation.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * These are the "floating point" xdr routines used to (de)serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include <stdio.h>

#include "types.h"
#include "xdr.h"


bool_t
xdr_float(xdrs, fp)
    register XDR *xdrs;
    register float *fp;
{
    switch (xdrs->x_op) {

    case XDR_ENCODE:
        return (XDR_PUTLONG(xdrs, (long *)fp));

    case XDR_DECODE:
        return (XDR_GETLONG(xdrs, (long *)fp));

    case XDR_FREE:
        return (TRUE);
    }
    return (FALSE);
}

bool_t
xdr_double(xdrs, dp)
    register XDR *xdrs;
    double *dp;
{
    register long *lp;

    switch (xdrs->x_op) {
      case XDR_ENCODE:
        lp = (long *)dp;
        return (XDR_PUTLONG(xdrs, lp++) && XDR_PUTLONG(xdrs, lp));
        break;

      case XDR_DECODE:
        /* Pull two units */
        lp = (long *)dp;
        return (XDR_GETLONG(xdrs, lp++) && XDR_GETLONG(xdrs, lp));
        break;

    case XDR_FREE:
        return (TRUE);
    } /* switch xdrs->x_op */

    return(FALSE);
}
