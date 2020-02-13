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

#if defined(i386) | defined(__i386)
# define SWAP_DOUBLES
#endif
#ifdef MIPSEL
# define SWAP_DOUBLES
#endif
#ifdef MSDOS
# define SWAP_DOUBLES
#endif
#ifdef __FreeBSD__
# define SWAP_DOUBLES
#endif
#if defined _WIN32 | __MINGW32__ | defined WINNT
# define SWAP_DOUBLES
#endif

/* from netCDF */
#define XDRUNIT 4

/* from netCDF */
/* signature: void swapinline32(unsigned int* ip) */
#define swapinline32(ip) \
{ \
    char dst[4]; \
    char* src = (char*)(ip); \
    dst[0] = src[3]; \
    dst[1] = src[2]; \
    dst[2] = src[1]; \
    dst[3] = src[0]; \
    *(ip) = *((unsigned int*)dst); \
}

/*
 * NB: Not portable.
 * Only two cases handled here:
 *    IEEE floating point and Vaxen
 */

#ifdef vax

/* What IEEE single precision floating point looks like on a Vax */
struct    ieee_single {
    unsigned int    mantissa: 23;
    unsigned int    exp     : 8;
    unsigned int    sign    : 1;
};

/* Vax single precision floating point */
struct    vax_single {
    unsigned int    mantissa1 : 7;
    unsigned int    exp       : 8;
    unsigned int    sign      : 1;
    unsigned int    mantissa2 : 16;
};

#define VAX_SNG_BIAS     0x81
#define IEEE_SNG_BIAS    0x7f

static struct sgl_limits {
    struct vax_single s;
    struct ieee_single ieee;
} max = {
    { 0x7f, 0xff, 0x0, 0xffff },    /* Max Vax */
    { 0x0, 0xff, 0x0 }              /* Max IEEE */
};
static struct sgl_limits min = {
    { 0x0, 0x0, 0x0, 0x0 },         /* Min Vax */
    { 0x0, 0x0, 0x0 }               /* Min IEEE */
};
#endif /* vax */

bool_t
xdr_float(xdrs, fp)
    register XDR *xdrs;
    register float *fp;
{
#ifdef vax
    struct ieee_single is;
    struct vax_single vs, *vsp;
    struct sgl_limits *lim;
    int i;
#endif
    switch (xdrs->x_op) {

    case XDR_ENCODE:
#ifndef vax
        return (XDR_PUTLONG(xdrs, (long *)fp));
#else
        vs = *((struct vax_single *)fp);

        switch(vs.exp){
        case 0 :
            /* all vax float with zero exponent map to zero */
            is = min.ieee ;
            break ;
        case 2 :
        case 1 :
            /* These will map to subnormals */
            is.exp = 0 ;
            is.mantissa = (vs.mantissa1 << 16) | vs.mantissa2;
            /* lose some precision */
            is.mantissa >>= 3 - vs.exp ;
            is.mantissa += (1 << (20 + vs.exp)) ;
            break ;
        case 0xff : /* max.s.exp */
            if( vs.mantissa2 == max.s.mantissa2
                && vs.mantissa1 == max.s.mantissa1)
            {
                /* map largest vax float to ieee infinity */
                is = max.ieee ;
                break ;
            } /* else, fall thru */
        default :
            is.exp = vs.exp - VAX_SNG_BIAS + IEEE_SNG_BIAS;
            is.mantissa = (vs.mantissa1 << 16) | vs.mantissa2;
        }

        is.sign = vs.sign;
        return (XDR_PUTLONG(xdrs, (long *)&is));
#endif

    case XDR_DECODE:
#ifndef vax
        return (XDR_GETLONG(xdrs, (long *)fp));
#else
        vsp = (struct vax_single *)fp;
        if (!XDR_GETLONG(xdrs, (long *)&is))
            return (FALSE);

        switch(is.exp) {
        case 0 :
            if(is.mantissa == min.ieee.mantissa) {
                *vsp = min.s ;
            }
            else {
                unsigned tmp = is.mantissa >> 20 ;
                if(tmp >= 4) {
                    vsp->exp = 2 ;
                }
                else if (tmp >= 2) {
                    vsp->exp = 1 ;
                }
                else {
                    *vsp = min.s ;
                    break ;
                } /* else */
                tmp = is.mantissa - (1 << (20 + vsp->exp )) ;
                tmp <<= 3 - vsp->exp ;
                vsp->mantissa2 = tmp ;
                vsp->mantissa1 = (tmp >> 16);
            }
            break ;
        case 0xfe :
        case 0xff :
            *vsp = max.s ;
            break ;
        default :
            vsp->exp = is.exp - IEEE_SNG_BIAS + VAX_SNG_BIAS;
            vsp->mantissa2 = is.mantissa;
            vsp->mantissa1 = (is.mantissa >> 16);
        }

        vsp->sign = is.sign;
        return (TRUE);
#endif

    case XDR_FREE:
        return (TRUE);
    }
    return (FALSE);
}

/*
 * This routine works on Suns (Sky / 68000's) and Vaxen.
 */

#ifdef vax
/* What IEEE double precision floating point looks like on a Vax */
struct    ieee_double {
    unsigned int    mantissa1 : 20;
    unsigned int    exp       : 11;
    unsigned int    sign      : 1;
    unsigned int    mantissa2 : 32;
};

/* Vax double precision floating point */
struct  vax_double {
    unsigned int    mantissa1 : 7;
    unsigned int    exp       : 8;
    unsigned int    sign      : 1;
    unsigned int    mantissa2 : 16;
    unsigned int    mantissa3 : 16;
    unsigned int    mantissa4 : 16;
};

#define VAX_DBL_BIAS    0x81
#define IEEE_DBL_BIAS    0x3ff
#define MASK(nbits)    ((1 << nbits) - 1)

static struct dbl_limits {
    struct    vax_double d;
    struct    ieee_double ieee;
} dbl_limits[2] = {
    {{ 0x7f, 0xff, 0x0, 0xffff, 0xffff, 0xffff },    /* Max Vax */
    { 0x0, 0x7ff, 0x0, 0x0 }},            /* Max IEEE */
    {{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},        /* Min Vax */
    { 0x0, 0x0, 0x0, 0x0 }}                /* Min IEEE */
};

#endif /* vax */

/* from netCDF */
/* Double needs special handling */
void
xdrntohdouble(char* c8, double* dp)
{
    unsigned int ii[2];
    memcpy(ii,c8,(size_t)2*XDRUNIT);
    unsigned int tmp;

    swapinline32(&ii[0]);
    swapinline32(&ii[1]);

    /* interchange ii[0] and ii[1] */
    tmp = ii[0];
    ii[0] = ii[1];
    ii[1] = tmp;
    if(dp) *dp = *(double*)ii;
}

bool_t
xdr_double(xdrs, dp)
    register XDR *xdrs;
    double *dp;
{
    int status = TRUE;
    double dbl_val = 100.0;

    if (!dp)
        return(FALSE);

    switch (xdrs->x_op) {
      case XDR_ENCODE:
#ifdef SWAP
        xdrntohdouble((char*)&dbl_val, dp);
#else
        dbl_val = *dp;
#endif
        status = xdr_opaque(xdrs, (caddr_t)&dbl_val, (off_t)2*XDRUNIT);
        break;

      case XDR_DECODE:
        /* Pull two units */
        status = xdr_opaque(xdrs, (caddr_t)&dbl_val, (off_t)2*XDRUNIT);
#ifdef SWAP
        xdrntohdouble((char*)&dbl_val, dp);
#else
        *dp = dbl_val;
#endif
        break;

    case XDR_FREE:
        return (TRUE);
    } /* switch xdrs->x_op */

    /* Return status from xdr_opaque */
    return status;
}
