/* @(#)xdr_stdio.c  1.1 87/11/04 3.9 RPCSRC */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_stdio.c 1.16 87/08/11 Copyr 1984 Sun Micro";
#endif

#include "H4api_adpt.h"

/*
 * xdr_stdio.c, XDR implementation on standard i/o file.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * This set of routines implements a XDR on a stdio stream.
 * XDR_ENCODE serializes onto the stream, XDR_DECODE de-serializes
 * from the stream.
 */

#include <stdio.h>

#ifdef H4_HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef H4_HAVE_NETINET_H
#  include <netinet/in.h>     /* for htonl() */
#else
#  ifdef H4_HAVE_WINSOCK2_H
#    include <winsock2.h>
#  endif
#endif

#include "types.h"
#include "xdr.h"

static bool_t   xdrstdio_getlong(XDR *, long *);
static bool_t   xdrstdio_putlong(XDR *, long *);
static bool_t   xdrstdio_getbytes(XDR *, caddr_t, u_int );
static bool_t   xdrstdio_putbytes(XDR *, caddr_t, u_int );
static u_long   xdrstdio_getpos(XDR *);
static bool_t   xdrstdio_setpos(XDR *, u_long);
static long *   xdrstdio_inline(XDR *, u_int);
static void xdrstdio_destroy(XDR *);

/*
 * Ops vector for stdio type XDR
 */
static struct xdr_ops   xdrstdio_ops = {
    xdrstdio_getlong,   /* deseraialize a long int */
    xdrstdio_putlong,   /* seraialize a long int */
    xdrstdio_getbytes,  /* deserialize counted bytes */
    xdrstdio_putbytes,  /* serialize counted bytes */
    xdrstdio_getpos,    /* get offset in the stream */
    xdrstdio_setpos,    /* set offset in the stream */
    xdrstdio_inline,    /* prime stream for inline macros */
    xdrstdio_destroy    /* destroy stream */
};

/*
 * Initialize a stdio xdr stream.
 * Sets the xdr stream handle xdrs for use on the stream file.
 * Operation flag is set to op.
 */
void
xdrstdio_create(xdrs, file, op)
    register XDR *xdrs;
    FILE *file;
    enum xdr_op op;
{
    xdrs->x_op = op;
    xdrs->x_ops = &xdrstdio_ops;
    xdrs->x_private = (caddr_t)file;
    xdrs->x_handy = 0;
    xdrs->x_base = 0;
}

/*
 * Destroy a stdio xdr stream.
 * Cleans up the xdr stream handle xdrs previously set up by xdrstdio_create.
 */
static void
xdrstdio_destroy(xdrs)
    register XDR *xdrs;
{
    (void)fflush((FILE *)xdrs->x_private);
    /* xx should we close the file ?? */
}

static bool_t
xdrstdio_getlong(xdrs, lp)
    XDR *xdrs;
    register long *lp;
{
    if (fread((caddr_t)lp, sizeof(long), 1, (FILE *)xdrs->x_private) != 1)
        return (FALSE);

#ifndef mc68000
    *lp = ntohl(*lp);
#endif
    return (TRUE);
}

static bool_t
xdrstdio_putlong(xdrs, lp)
    XDR *xdrs;
    long *lp;
{

#ifndef mc68000
    long mycopy = htonl(*lp);
    lp = &mycopy;
#endif

    if (fwrite((caddr_t)lp, sizeof(long), 1, (FILE *)xdrs->x_private) != 1)
        return (FALSE);

    return (TRUE);
}

static bool_t
xdrstdio_getbytes(xdrs, addr, len)
    XDR *xdrs;
    caddr_t addr;
    u_int len;
{

    if ((len != 0) && (fread(addr, (int)len, 1, (FILE *)xdrs->x_private) != 1))
        return (FALSE);
    return (TRUE);
}

static bool_t
xdrstdio_putbytes(xdrs, addr, len)
    XDR *xdrs;
    caddr_t addr;
    u_int len;
{

    if ((len != 0) && (fwrite(addr, (int)len, 1, (FILE *)xdrs->x_private) != 1))
        return (FALSE);
    return (TRUE);
}

static u_long
xdrstdio_getpos(xdrs)
    XDR *xdrs;
{

    return ((u_long)ftell((FILE *)xdrs->x_private));
}

static bool_t
xdrstdio_setpos(xdrs, pos)
    XDR *xdrs;
    u_long pos;
{

    return ((fseek((FILE *)xdrs->x_private,(long)pos, 0) < 0) ?
        FALSE : TRUE);
}

/*ARGSUSED*/
static long *
xdrstdio_inline(xdrs, len)
    XDR *xdrs;
    u_int len;
{

    /*
     * Must do some work to implement this: must insure
     * enough data in the underlying stdio buffer,
     * that the buffer is aligned so that we can indirect through a
     * long *, and stuff this pointer in xdrs->x_buf.  Doing
     * a fread or fwrite to a scratch buffer would defeat
     * most of the gains to be had here and require storage
     * management on this buffer, so we don't do this.
     */
    return (NULL);
}
