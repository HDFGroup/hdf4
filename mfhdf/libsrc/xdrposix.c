/*
 * XDR implementation on POSIX file interface, with buffering
 *
 * Copyright (C) 1992, University Corp for Atmospheric Research
 *
 * This set of routines implements a XDR on a POSIX file descriptor.
 * XDR_ENCODE serializes onto the descriptor, XDR_DECODE de-serializes
 * from the descriptor. As in stdio, we buffer i/o. This XDR is most
 * useful when the descriptor actually represents a file. It
 * corrects some performance problems with xdrstdio_getpos() and
 * xdrstdio_getpos() in the xdr_stdio implementation.
 */
#include "h4config.h"

#include <inttypes.h>
#include <string.h>

#ifdef H4_HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef H4_HAVE_NETINET_IN_H
#include <netinet/in.h> /* for htonl() */
#endif

#include "mfhdf.h"
#include "xdr.h"

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

static bool_t   xdrposix_getlong(XDR *xdrs, long *lp);
static bool_t   xdrposix_putlong(XDR *xdrs, const long *lp);
static bool_t   xdrposix_getbytes(XDR *xdrs, char *addr, unsigned len);
static bool_t   xdrposix_putbytes(XDR *xdrs, const char *addr, unsigned len);
static unsigned xdrposix_getpos(XDR *xdrs);
static bool_t   xdrposix_setpos(XDR *xdrs, unsigned pos);
static void     xdrposix_destroy(XDR *xdrs);

/*
 * POSIX XDR operations
 */
static struct xdr_ops xdrposix_ops = {
    xdrposix_getlong,  /* deserialize a 32-bit int */
    xdrposix_putlong,  /* serialize a 32-bit int */
    xdrposix_getbytes, /* deserialize counted bytes */
    xdrposix_putbytes, /* serialize counted bytes */
    xdrposix_getpos,   /* get offset in the stream */
    xdrposix_setpos,   /* set offset in the stream */
    xdrposix_destroy,  /* destroy stream */
};

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

    xdrs->x_ops     = &xdrposix_ops;
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
    xdrs->x_ops     = &xdrposix_ops;
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
