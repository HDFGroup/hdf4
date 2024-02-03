/*
 * Copyright (c) 2009, Sun Microsystems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Sun Microsystems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * xdr.c, Generic XDR routines implementation.
 *
 * Copyright (C) 1986, Sun Microsystems, Inc.
 *
 * These are the "generic" xdr routines used to serialize and de-serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include "h4config.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef H4_HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef H4_HAVE_NETINET_IN_H
#include <netinet/in.h> /* for htonl() */
#endif

#include "mfhdf.h"
#include "xdr.h"

/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT (4)

/*
 * for unit alignment
 */
static const char xdr_zero[BYTES_PER_XDR_UNIT] = {0, 0, 0, 0};

static bool_t   xdrposix_getlong(XDR *xdrs, long *lp);
static bool_t   xdrposix_putlong(XDR *xdrs, const long *lp);
static bool_t   xdrposix_getbytes(XDR *xdrs, char *addr, unsigned len);
static bool_t   xdrposix_putbytes(XDR *xdrs, const char *addr, unsigned len);
static unsigned xdrposix_getpos(XDR *xdrs);
static bool_t   xdrposix_setpos(XDR *xdrs, unsigned pos);
static void     xdrposix_destroy(XDR *xdrs);

/*
 * Operations defined on an XDR handle
 */

/* NOTE: These deal with raw bytes, not "counted bytes", like xdr_bytes()
 *       does, below!
 */
bool_t
xdr_getbytes(XDR *xdrs, char *addr, unsigned len)
{
    return xdrposix_getbytes(xdrs, addr, len);
}

bool_t
xdr_putbytes(XDR *xdrs, const char *addr, unsigned len)
{
    return xdrposix_putbytes(xdrs, addr, len);
}

/*
 * Set/Get the file position
 */
unsigned
xdr_getpos(XDR *xdrs)
{
    return xdrposix_getpos(xdrs);
}

bool_t
xdr_setpos(XDR *xdrs, unsigned pos)
{
    return xdrposix_setpos(xdrs, pos);
}

/*
 * Close the file
 */
void
xdr_destroy(XDR *xdrs)
{
    xdrposix_destroy(xdrs);
}

/*
 * XDR integers
 */
bool_t
xdr_int(XDR *xdrs, int *ip)
{
    long l;

    switch (xdrs->x_op) {

        case XDR_ENCODE:
            l = (long)*ip;
            return xdrposix_putlong(xdrs, &l);

        case XDR_DECODE:
            if (!xdrposix_getlong(xdrs, &l))
                return FALSE;
            *ip = (int)l;
            return TRUE;

        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR unsigned integers
 */
bool_t
xdr_u_int(XDR *xdrs, unsigned *up)
{
    unsigned long l;

    switch (xdrs->x_op) {

        case XDR_ENCODE:
            l = (unsigned long)*up;
            return xdrposix_putlong(xdrs, (long *)&l);

        case XDR_DECODE:
            if (!xdrposix_getlong(xdrs, (long *)&l))
                return FALSE;
            *up = (unsigned)l;
            return TRUE;

        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR long integers
 */
bool_t
xdr_long(XDR *xdrs, long *lp)
{
    switch (xdrs->x_op) {
        case XDR_ENCODE:
            return xdrposix_putlong(xdrs, lp);
        case XDR_DECODE:
            return xdrposix_getlong(xdrs, lp);
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR unsigned long integers
 */
bool_t
xdr_u_long(XDR *xdrs, unsigned long *ulp)
{
    switch (xdrs->x_op) {
        case XDR_ENCODE:
            return xdrposix_putlong(xdrs, (long *)ulp);
        case XDR_DECODE:
            return xdrposix_getlong(xdrs, (long *)ulp);
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR opaque data
 * Allows the specification of a fixed size sequence of opaque bytes.
 * cp points to the opaque object and cnt gives the byte length.
 */
bool_t
xdr_opaque(XDR *xdrs, char *cp, unsigned cnt)
{
    unsigned   rndup;
    static int crud[BYTES_PER_XDR_UNIT];

    /*
     * if no data we are done
     */
    if (cnt == 0)
        return TRUE;

    /*
     * round byte count to full xdr units
     */
    rndup = cnt % BYTES_PER_XDR_UNIT;
    if (rndup > 0)
        rndup = BYTES_PER_XDR_UNIT - rndup;

    if (xdrs->x_op == XDR_DECODE) {
        if (!xdr_getbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return xdr_getbytes(xdrs, (char *)crud, rndup);
    }

    if (xdrs->x_op == XDR_ENCODE) {
        if (!xdr_putbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return xdr_putbytes(xdrs, xdr_zero, rndup);
    }

    if (xdrs->x_op == XDR_FREE)
        return TRUE;

    return FALSE;
}

/*
 * XDR counted bytes
 * *cpp is a pointer to the bytes, *sizep is the count.
 * If *cpp is NULL maxsize bytes are allocated
 */
bool_t
xdr_bytes(XDR *xdrs, char **cpp, unsigned *sizep, unsigned maxsize)
{
    char    *sp = *cpp; /* sp is the actual string pointer */
    unsigned nodesize;
    bool_t   ret;
    bool_t   allocated = FALSE;

    /*
     * first deal with the length since xdr bytes are counted
     */
    if (!xdr_u_int(xdrs, sizep))
        return FALSE;
    nodesize = *sizep;
    if ((nodesize > maxsize) && (xdrs->x_op != XDR_FREE))
        return FALSE;

    /*
     * now deal with the actual bytes
     */
    switch (xdrs->x_op) {

        case XDR_DECODE:
            if (nodesize == 0)
                return TRUE;
            if (sp == NULL) {
                *cpp = sp = calloc(1, nodesize);
                allocated = TRUE;
            }
            if (sp == NULL) {
                fprintf(stderr, "xdr_bytes: out of memory\n");
                return FALSE;
            }
            /* FALLTHROUGH */

        case XDR_ENCODE:
            ret = xdr_opaque(xdrs, sp, nodesize);
            if ((xdrs->x_op == XDR_DECODE) && (ret == FALSE)) {
                if (allocated == TRUE) {
                    free(sp);
                    *cpp = NULL;
                }
            }
            return ret;

        case XDR_FREE:
            if (sp != NULL) {
                free(sp);
                *cpp = NULL;
            }
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

bool_t
xdr_float(XDR *xdrs, float *fp)
{
    switch (xdrs->x_op) {

        case XDR_ENCODE:
            return xdr_int(xdrs, (int *)fp);

        case XDR_DECODE:
            return xdr_int(xdrs, (int *)fp);

        case XDR_FREE:
            return TRUE;
    }
    return FALSE;
}

bool_t
xdr_double(XDR *xdrs, double *dp)
{
    int   *ip;
    bool_t rv;

    if (!dp)
        return FALSE;

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            ip = (int *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = xdr_int(xdrs, ip);
            if (!rv)
                return rv;
            rv = xdr_int(xdrs, ip + 1);
#else
            rv = xdr_int(xdrs, ip + 1);
            if (!rv)
                return rv;
            rv = xdr_int(xdrs, ip);
#endif
            return rv;
            break;

        case XDR_DECODE:
            ip = (int *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = xdr_int(xdrs, ip);
            if (!rv)
                return rv;
            rv = xdr_int(xdrs, ip + 1);
#else
            rv = xdr_int(xdrs, ip + 1);
            if (!rv)
                return rv;
            rv = xdr_int(xdrs, ip);
#endif
            return rv;
            break;

        case XDR_FREE:
            return TRUE;
    }

    return FALSE;
}

/*********************/
/* FORMER xdrposix.c */
/*********************/

/* NOTE: "bio" == Buffered I/O */

typedef struct {
    int            fd;   /* the file descriptor */
    int            mode; /* file access mode, O_RDONLY, etc */
    int            isdirty;
    off_t          page;
    int            nread;  /* number of bytes successfully read */
    int            nwrote; /* number of bytes last write */
    int            cnt;    /* number of valid bytes in buffer */
    unsigned char *ptr;    /* next byte */
#define BIOBUFSIZ 8192
    unsigned char base[BIOBUFSIZ]; /* the data buffer */
} biobuf;

static biobuf *
new_biobuf(int fd, int fmode)
{
    biobuf *biop = NULL;

    if (NULL == (biop = malloc(sizeof(biobuf))))
        return NULL;
    biop->fd = fd;

    biop->mode = fmode;

    biop->isdirty = 0;
    biop->page    = 0;
    biop->nread   = 0;
    biop->nwrote  = 0;
    biop->cnt     = 0;
    memset(biop->base, 0, ((size_t)(BIOBUFSIZ)));
    biop->ptr = biop->base;

    return biop;
}

static int
rdbuf(biobuf *biop)
{
    memset(biop->base, 0, ((size_t)(BIOBUFSIZ)));

    if (biop->mode & O_WRONLY) {
        biop->cnt = 0;
    }
    else {
        if (biop->nwrote != BIOBUFSIZ) {
            /* last write wasn't a full block, adjust position ahead */
            if (lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
                return -1;
        }
        biop->nread = biop->cnt = read(biop->fd, (void *)biop->base, BIOBUFSIZ);
    }
    biop->ptr = biop->base;
    return biop->cnt;
}

static int
wrbuf(biobuf *biop)
{

    if (!((biop->mode & O_WRONLY) || (biop->mode & O_RDWR)) || biop->cnt == 0) {
        biop->nwrote = 0;
    }
    else {
        if (biop->nread != 0) {
            /* if we read something, we have to adjust position back */
            if (lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
                return -1;
        }
        biop->nwrote = write(biop->fd, (void *)biop->base, biop->cnt);
    }
    biop->isdirty = 0;

    return biop->nwrote;
}

static int
nextbuf(biobuf *biop)
{
    if (biop->isdirty) {
        if (wrbuf(biop) < 0)
            return -1;
    }

    biop->page++;

    /* read it in */
    if (rdbuf(biop) < 0)
        return -1;

    return biop->cnt;
}

#define CNT(p) ((p)->ptr - (p)->base)

/* # of unread bytes in buffer */
#define REM(p) ((p)->cnt - CNT(p))

/* available space for write in buffer */
#define BREM(p) (BIOBUFSIZ - CNT(p))

static int
bioread(biobuf *biop, unsigned char *ptr, int nbytes)
{
    int    ngot = 0;
    size_t rem;

    if (nbytes == 0)
        return 0;

    while (nbytes > (rem = REM(biop))) {
        if (rem > 0) {
            (void)memcpy(ptr, biop->ptr, rem);
            ptr += rem;
            nbytes -= rem;
            ngot += rem;
        }
        if (nextbuf(biop) <= 0)
            return ngot;
    }
    /* we know nbytes <= REM at this point */
    (void)memcpy(ptr, biop->ptr, (size_t)nbytes);
    biop->ptr += nbytes;
    ngot += nbytes;
    return ngot;
}

static int
biowrite(biobuf *biop, unsigned char *ptr, int nbytes)
{
    size_t rem;
    int    nwrote = 0;
    int    cnt;

    if (!((biop->mode & O_WRONLY) || (biop->mode & O_RDWR)))
        return -1;

    while (nbytes > (rem = BREM(biop))) {
        if (rem > 0) {
            (void)memcpy(biop->ptr, ptr, rem);
            biop->isdirty = !0;
            biop->cnt     = BIOBUFSIZ;
            ptr += rem;
            nbytes -= rem;
            nwrote += rem;
        }
        if (nextbuf(biop) < 0)
            return nwrote;
    }
    /* we know nbytes <= BREM at this point */
    (void)memcpy(biop->ptr, ptr, (size_t)nbytes);
    biop->isdirty = !0;
    biop->ptr += nbytes;
    if ((cnt = CNT(biop)) > biop->cnt)
        biop->cnt = cnt;
    nwrote += nbytes;

    return nwrote;
}

/*
 * Fake an XDR initialization for HDF files
 */
void
hdf_xdrfile_create(XDR *xdrs, int ncop)
{
    biobuf *biop = new_biobuf(-1, 0);

    if (ncop & NC_CREAT)
        xdrs->x_op = XDR_ENCODE;
    else
        xdrs->x_op = XDR_DECODE;

    xdrs->x_private = (char *)biop;

} /* hdf_xdrfile_create */

/*
 * Initialize a posix xdr stream.
 * Sets the xdr stream handle xdrs for use on the file descriptor fd.
 * Operation flag is set to op.
 */
int
xdrposix_create(XDR *xdrs, int fd, int fmode, enum xdr_op op)
{
    biobuf *biop    = new_biobuf(fd, fmode);
    xdrs->x_op      = op;
    xdrs->x_private = (char *)biop;
    if (biop == NULL)
        return -1;

    /* if write only, or just created (empty), done */
    if ((biop->mode & O_WRONLY) || (biop->mode & O_CREAT))
        return 0;

    /* else, read the first bufferful */
    return rdbuf(biop);
}

/*
 * "sync" a posix xdr stream.
 */
int
xdrposix_sync(XDR *xdrs)
{
    biobuf *biop = (biobuf *)xdrs->x_private;
    if (biop->isdirty) {
        /* flush */
        if (wrbuf(biop) < 0)
            return -1;
    }

    biop->nwrote = 0; /* force seek in rdbuf */

    /* read it in */
    if (rdbuf(biop) < 0)
        return -1;

    return biop->cnt;
}

/*
 * Destroy a posix xdr stream.
 * Cleans up the xdr stream handle xdrs previously set up by xdrposix_create.
 */
static void
xdrposix_destroy(XDR *xdrs)
{
    /* flush */
    biobuf *biop = (biobuf *)xdrs->x_private;
    if (biop != NULL) {
        if (biop->isdirty) {
            (void)wrbuf(biop);
        }
        if (biop->fd != -1)
            (void)close(biop->fd);
        free(biop);
    }
}

static bool_t
xdrposix_getlong(XDR *xdrs, long *lp)
{
    unsigned char *up = (unsigned char *)lp;
#if (defined AIX5L64 || defined __powerpc64__)
    *lp = 0;
    up += (sizeof(long) - 4);
#endif
    if (bioread((biobuf *)xdrs->x_private, up, 4) < 4)
        return FALSE;
#ifndef H4_WORDS_BIGENDIAN
    *lp = ntohl(*lp);
#endif
    return TRUE;
}

static bool_t
xdrposix_putlong(XDR *xdrs, const long *lp)
{

    unsigned char *up = (unsigned char *)lp;
#ifndef H4_WORDS_BIGENDIAN
    int32_t mycopy = htonl(*lp);
    up             = (unsigned char *)&mycopy;
#endif
#if (defined AIX5L64 || defined __powerpc64__)
    up += (sizeof(long) - 4);
#endif

    if (biowrite((biobuf *)xdrs->x_private, up, 4) < 4)
        return FALSE;
    return TRUE;
}

static bool_t
xdrposix_getbytes(XDR *xdrs, char *addr, unsigned len)
{
    if ((len != 0) && (bioread((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
        return FALSE;
    return TRUE;
}

static bool_t
xdrposix_putbytes(XDR *xdrs, const char *addr, unsigned len)
{
    if ((len != 0) && (biowrite((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
        return FALSE;
    return TRUE;
}

static unsigned
xdrposix_getpos(XDR *xdrs)
{
    biobuf *biop = (biobuf *)xdrs->x_private;
    return BIOBUFSIZ * biop->page + CNT(biop);
}

static bool_t
xdrposix_setpos(XDR *xdrs, unsigned pos)
{
    biobuf *biop = (biobuf *)xdrs->x_private;
    if (biop != NULL) {
        off_t page;
        int   index;
        int   nread;
        page  = pos / BIOBUFSIZ;
        index = pos % BIOBUFSIZ;
        if (page != biop->page) {
            if (biop->isdirty) {
                if (wrbuf(biop) < 0)
                    return FALSE;
            }
            if (page != biop->page + 1)
                biop->nwrote = 0; /* force seek in rdbuf */

            biop->page = page;

            nread = rdbuf(biop);
            if (nread < 0 || ((biop->mode & O_RDONLY) && nread < index))
                return FALSE;
        }
        biop->ptr = biop->base + index;
        return TRUE;
    }
    else
        return FALSE;
}
