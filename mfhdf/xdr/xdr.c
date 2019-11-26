/* @(#)xdr.c    2.1 88/07/29 4.0 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr.c 1.35 87/08/12";
#endif

#include "H4api_adpt.h"

/*
 * xdr.c, Generic XDR routines implementation.
 *
 * Copyright (C) 1986, Sun Microsystems, Inc.
 *
 * These are the "generic" xdr routines used to serialize and de-serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "xdr.h"

/*
 * constants specific to the xdr "protocol"
 */
#define XDR_FALSE       ((long) 0)
#define XDR_TRUE        ((long) 1)
#define LASTUNSIGNED    ((u_int) 0-1)

/*
 * for unit alignment
 */
static char xdr_zero[BYTES_PER_XDR_UNIT] = { 0, 0, 0, 0 };

/*
 * Free a data structure using XDR
 * Not a filter, but a convenient utility nonetheless
 */
void
xdr_free(proc, objp)
    xdrproc_t proc;
    char *objp;
{
    XDR x;

    x.x_op = XDR_FREE;
    (*proc)(&x, objp);
}

/*
 * XDR nothing
 */
bool_t
xdr_void(/* xdrs, addr */)
    /* XDR *xdrs; */
    /* caddr_t addr; */
{
    return TRUE;
}

/*
 * XDR booleans
 */
bool_t
xdr_bool (XDR *xdrs, bool_t *p)
{
  int32_t i;
  switch (xdrs->x_op) {
  case XDR_ENCODE:
    i = *p ? TRUE : FALSE;
    return xdr_putlong (xdrs, &i);
  case XDR_DECODE:
    if (!xdr_getlong (xdrs, &i)) return FALSE;
    *p = i ? TRUE : FALSE;
    return TRUE;
  case XDR_FREE:
    return TRUE;
  default:
    return FALSE;
  }
}

/*
 * XDR enumerations
 */
bool_t
xdr_enum(xdrs, ep)
    XDR *xdrs;
    enum_t *ep;
{
    return xdr_int32_t (xdrs, ep);
}

/*
 * XDR opaque data
 * Allows the specification of a fixed size sequence of opaque bytes.
 * cp points to the opaque object and cnt gives the byte length.
 */
bool_t
xdr_opaque(xdrs, cp, cnt)
    register XDR *xdrs;
    caddr_t cp;
    register u_int cnt;
{
    register u_int rndup;
    static char crud[BYTES_PER_XDR_UNIT];

    /*
    * if no data we are done
    */
    if (cnt == 0)
        return (TRUE);

    /*
    * round byte count to full xdr units
    */
    rndup = cnt % BYTES_PER_XDR_UNIT;
    if (rndup > 0)
        rndup = BYTES_PER_XDR_UNIT - rndup;

    if (xdrs->x_op == XDR_DECODE) {
        if (!XDR_GETBYTES(xdrs, cp, cnt)) {
            return (FALSE);
        }
        if (rndup == 0)
            return (TRUE);
        return (XDR_GETBYTES(xdrs, crud, rndup));
    }

    if (xdrs->x_op == XDR_ENCODE) {
        if (!XDR_PUTBYTES(xdrs, cp, cnt)) {
            return (FALSE);
        }
        if (rndup == 0)
            return (TRUE);
        return (XDR_PUTBYTES(xdrs, xdr_zero, rndup));
    }

    if (xdrs->x_op == XDR_FREE) {
        return (TRUE);
    }

    return (FALSE);
}

/*
 * XDR counted bytes
 * *cpp is a pointer to the bytes, *sizep is the count.
 * If *cpp is NULL maxsize bytes are allocated
 */
bool_t
xdr_bytes(xdrs, cpp, sizep, maxsize)
    register XDR *xdrs;
    char **cpp;
    register u_int *sizep;
    u_int maxsize;
{
    register char *sp = *cpp;  /* sp is the actual string pointer */
    register u_int nodesize;

    /*
    * first deal with the length since xdr bytes are counted
    */
    if (! xdr_u_int(xdrs, sizep)) {
        return (FALSE);
    }
    nodesize = *sizep;
    if ((nodesize > maxsize) && (xdrs->x_op != XDR_FREE)) {
        return (FALSE);
    }

    /*
    * now deal with the actual bytes
    */
    switch (xdrs->x_op) {

    case XDR_DECODE:
        if (nodesize == 0) {
            return (TRUE);
        }
        if (sp == NULL) {
            *cpp = sp = (char *)mem_alloc(nodesize);
        }
        if (sp == NULL) {
            (void) fprintf(stderr, "xdr_bytes: out of memory\n");
            return (FALSE);
        }
        /* fall into ... */

    case XDR_ENCODE:
        return (xdr_opaque(xdrs, sp, nodesize));

    case XDR_FREE:
        if (sp != NULL) {
            mem_free(sp, nodesize);
            *cpp = NULL;
        }
        return (TRUE);
    }
    return (FALSE);
}

/* XDR 64bit integers */
bool_t
xdr_int64_t (XDR *xdrs, int64_t *ip)
{
    int32_t t1, t2;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        t1 = (int32_t) ((*ip) >> 32);
        t2 = (int32_t) (*ip);
        return (XDR_PUTINT32(xdrs, &t1) && XDR_PUTINT32(xdrs, &t2));
    case XDR_DECODE:
        if (!XDR_GETINT32(xdrs, &t1) || !XDR_GETINT32(xdrs, &t2))
            return FALSE;
        *ip = ((int64_t) t1) << 32;
        *ip |= (uint32_t) t2;    /* Avoid sign extension.  */
        return TRUE;
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 64bit unsigned integers */
bool_t
xdr_uint64_t (XDR *xdrs, uint64_t *uip)
{
    uint32_t t1;
    uint32_t t2;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        t1 = (uint32_t) ((*uip) >> 32);
        t2 = (uint32_t) (*uip);
        return (XDR_PUTINT32 (xdrs, (int32_t *) &t1) &&
                XDR_PUTINT32(xdrs, (int32_t *) &t2));
    case XDR_DECODE:
        if (!XDR_GETINT32(xdrs, (int32_t *) &t1) ||
                !XDR_GETINT32(xdrs, (int32_t *) &t2))
            return FALSE;
        *uip = ((uint64_t) t1) << 32;
        *uip |= t2;
        return TRUE;
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 32bit integers */
bool_t
xdr_int32_t (XDR *xdrs, int32_t *lp)
{
    switch (xdrs->x_op) {
    case XDR_ENCODE:
        return XDR_PUTINT32 (xdrs, lp);
    case XDR_DECODE:
        return XDR_GETINT32 (xdrs, lp);
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 32bit unsigned integers */
bool_t
xdr_uint32_t (XDR *xdrs, uint32_t *ulp)
{
    switch (xdrs->x_op) {
    case XDR_ENCODE:
        return XDR_PUTINT32 (xdrs, (int32_t *) ulp);
    case XDR_DECODE:
        return XDR_GETINT32 (xdrs, (int32_t *) ulp);
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 16bit integers */
bool_t
xdr_int16_t (XDR *xdrs, int16_t *ip)
{
    int32_t t;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        t = (int32_t) *ip;
        return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
        if (!XDR_GETINT32 (xdrs, &t))
            return FALSE;
        *ip = (int16_t) t;
        return TRUE;
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 16bit unsigned integers */
bool_t
xdr_uint16_t (XDR *xdrs, uint16_t *uip)
{
    uint32_t ut;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        ut = (uint32_t) *uip;
        return XDR_PUTINT32 (xdrs, (int32_t *) &ut);
    case XDR_DECODE:
        if (!XDR_GETINT32 (xdrs, (int32_t *) &ut))
            return FALSE;
        *uip = (uint16_t) ut;
        return TRUE;
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 8bit integers */
bool_t
xdr_int8_t (XDR *xdrs, int8_t *ip)
{
    uint32_t ut;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        ut = (uint32_t) *ip;
        return XDR_PUTINT32 (xdrs, (int32_t *) &ut);
    case XDR_DECODE:
        if (!XDR_GETINT32 (xdrs, (int32_t *) &ut))
            return FALSE;
        *ip = (int8_t) ut;
        return TRUE;
    case XDR_FREE:
        return TRUE;
    default:
        return FALSE;
    }
}

/* XDR 8bit unsigned integers */
bool_t
xdr_uint8_t (XDR *xdrs, uint8_t *uip)
{
  long ut;

  switch (xdrs->x_op) {
    case XDR_ENCODE:
      ut = (uint32_t) *uip;
      return XDR_PUTINT32 (xdrs, &ut);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &ut))
    return FALSE;
      *uip = (uint8_t) ut;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/*
 * Implemented here due to commonality of the object.
 */
bool_t
xdr_netobj(xdrs, np)
    XDR *xdrs;
    struct netobj *np;
{

    return (xdr_bytes(xdrs, &np->n_bytes, &np->n_len, MAX_NETOBJ_SZ));
}

/*
 * XDR a descriminated union
 * Support routine for discriminated unions.
 * You create an array of xdrdiscrim structures, terminated with
 * an entry with a null procedure pointer.  The routine gets
 * the discriminant value and then searches the array of xdrdiscrims
 * looking for that value.  It calls the procedure given in the xdrdiscrim
 * to handle the discriminant.  If there is no specific routine a default
 * routine may be called.
 * If there is no specific or default routine an error is returned.
 */
bool_t
xdr_union(xdrs, dscmp, unp, choices, dfault)
    register XDR *xdrs;
    enum_t *dscmp;        /* enum to decide which arm to work on */
    char *unp;        /* the union itself */
    const struct xdr_discrim *choices;    /* [value, xdr proc] for each arm */
    xdrproc_t dfault;    /* default xdr routine */
{
    register enum_t dscm;

    /*
    * we deal with the discriminator;  it's an enum
    */
    if (! xdr_enum(xdrs, dscmp)) {
        return (FALSE);
    }
    dscm = *dscmp;

    /*
    * search choices for a value that matches the discriminator.
    * if we find one, execute the xdr routine for that value.
    */
    for (; choices->proc != NULL_xdrproc_t; choices++) {
        if (choices->value == dscm)
            return ((*(choices->proc))(xdrs, unp, LASTUNSIGNED));
    }

    /*
    * no match - execute the default xdr routine if there is one
    */
    return ((dfault == NULL_xdrproc_t) ? FALSE :
        (*dfault)(xdrs, unp, LASTUNSIGNED));
}


/*
 * Non-portable xdr primitives.
 * Care should be taken when moving these routines to new architectures.
 */


/*
 * XDR null terminated ASCII strings
 * xdr_string deals with "C strings" - arrays of bytes that are
 * terminated by a NULL character.  The parameter cpp references a
 * pointer to storage; If the pointer is null, then the necessary
 * storage is allocated.  The last parameter is the max allowed length
 * of the string as specified by a protocol.
 */
bool_t
xdr_string(xdrs, cpp, maxsize)
    register XDR *xdrs;
    char **cpp;
    u_int maxsize;
{
    register char *sp = *cpp;  /* sp is the actual string pointer */
    u_int size;
    u_int nodesize;

    /*
    * first deal with the length since xdr strings are counted-strings
    */
    switch (xdrs->x_op) {
    case XDR_DECODE: break; /* keep gcc happy */
    case XDR_FREE:
        if (sp == NULL) {
            return(TRUE);    /* already free */
        }
        /* fall through... */
    case XDR_ENCODE:
        size = strlen(sp);
        break;
    }
    if (! xdr_u_int(xdrs, &size)) {
        return (FALSE);
    }
    if (size > maxsize) {
        return (FALSE);
    }
    nodesize = size + 1;

    /*
    * now deal with the actual bytes
    */
    switch (xdrs->x_op) {

    case XDR_DECODE:
        if (nodesize == 0) {
            return (TRUE);
        }
        if (sp == NULL)
            *cpp = sp = (char *)mem_alloc(nodesize);
        if (sp == NULL) {
            (void) fprintf(stderr, "xdr_string: out of memory\n");
            return (FALSE);
        }
        sp[size] = 0;
        /* fall into ... */

    case XDR_ENCODE:
        return (xdr_opaque(xdrs, sp, size));

    case XDR_FREE:
        mem_free(sp, nodesize);
        *cpp = NULL;
        return (TRUE);
    }
    return (FALSE);
}

/*
 * Wrapper for xdr_string that can be called directly from
 * routines like clnt_call
 */
bool_t
xdr_wrapstring(xdrs, cpp)
    XDR *xdrs;
    char **cpp;
{
    if (xdr_string(xdrs, cpp, LASTUNSIGNED)) {
        return (TRUE);
    }
    return (FALSE);
}
