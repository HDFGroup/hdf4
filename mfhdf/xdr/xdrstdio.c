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

#ifdef H4_HAVE_NETINET_IN_H
#  include <netinet/in.h>     /* for htonl() */
#else
#  ifdef H4_HAVE_WINSOCK2_H
#    include <winsock2.h>
#  endif
#endif

#include "types.h"
#include "xdr.h"

static void xdrstdio_destroy(XDR *);
static bool_t   xdrstdio_getlong(XDR *, long *);
static bool_t   xdrstdio_putlong(XDR *, const long *);
static bool_t   xdrstdio_getbytes(XDR *, char *, u_int);
static bool_t   xdrstdio_putbytes(XDR *, const char *, u_int);
static u_int    xdrstdio_getpos(XDR *);
static bool_t   xdrstdio_setpos(XDR *, u_int);
static int32_t *xdrstdio_inline(XDR *, u_int);

/*
 * Ops vector for stdio type XDR
 */
static const struct xdr_ops   xdrstdio_ops = {
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
    XDR *xdrs;
    FILE *file;
    enum xdr_op op;
{
    xdrs->x_op = op;
    xdrs->x_ops = &xdrstdio_ops;
    xdrs->x_private = file;
    xdrs->x_handy = 0;
    xdrs->x_base = 0;
}

/*
 * Destroy a stdio xdr stream.
 * Cleans up the xdr stream handle xdrs previously set up by xdrstdio_create.
 */
static void
xdrstdio_destroy(xdrs)
    XDR *xdrs;
{
    (void)fflush((FILE *)xdrs->x_private);
    /* xx should we close the file ?? */
}

static bool_t
xdrstdio_getlong(xdrs, lp)
    XDR *xdrs;
    long *lp;
{
	int32_t mycopy;

	if (fread(&mycopy, sizeof(int32_t), 1, (FILE *)xdrs->x_private) != 1)
		return (FALSE);

	*lp = (long)ntohl(mycopy);
	return (TRUE);
}

static bool_t
xdrstdio_putlong(xdrs, lp)
    XDR *xdrs;
    const long *lp;
{
	int32_t mycopy;

#if defined(_LP64)
	if ((*lp > UINT32_MAX) || (*lp < INT32_MIN))
		return (FALSE);
#endif

	mycopy = (int32_t)htonl((int32_t)*lp);
	if (fwrite(&mycopy, sizeof(int32_t), 1, (FILE *)xdrs->x_private) != 1)
		return (FALSE);
	return (TRUE);
}

static bool_t
xdrstdio_getbytes(xdrs, addr, len)
    XDR *xdrs;
    char *addr;
    u_int len;
{

	if ((len != 0) && (fread(addr, (size_t)len, 1, (FILE *)xdrs->x_private) != 1))
		return (FALSE);
	return (TRUE);
}

static bool_t
xdrstdio_putbytes(xdrs, addr, len)
    XDR *xdrs;
    const char *addr;
    u_int len;
{

	if ((len != 0) && (fwrite(addr, (size_t)len, 1,
	    (FILE *)xdrs->x_private) != 1))
		return (FALSE);
	return (TRUE);
}

static u_int
xdrstdio_getpos(xdrs)
    XDR *xdrs;
{

    return ((u_int)ftell((FILE *)xdrs->x_private));
}

static bool_t
xdrstdio_setpos(xdrs, pos)
    XDR *xdrs;
    u_int pos;
{

    return ((fseek((FILE *)xdrs->x_private, (long)pos, 0) < 0) ?
        FALSE : TRUE);
}

/* ARGSUSED */
static int32_t *
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
