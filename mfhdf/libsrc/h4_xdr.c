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

#include "local_nc.h"

/* Which header files include htonl() ? */

#ifdef H4_HAVE_WIN32_API
/* Use winsock on Win32 (including MinGW)
 *
 * This is included in hdfi.h to ensure winsock2.h is included before
 * windows.h.
 */
#else
/* And normal network headers everywhere else */
#ifdef H4_HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef H4_HAVE_NETINET_IN_H
#include <netinet/in.h> /* for htonl() */
#endif
#endif

#include "h4_xdr.h"

/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT (4)

/*
 * for unit alignment
 */
static const char xdr_zero[BYTES_PER_XDR_UNIT] = {0, 0, 0, 0};

/* Forward declarations */
static bool_t h4_xdr_get32(XDR *xdrs, uint32_t *up);
static bool_t h4_xdr_put32(XDR *xdrs, const uint32_t *up);

/******************/
/* XDR Type Calls */
/******************/

/* These per-type API calls encode/decode/free depending on the op field
 * in the XDR struct. They use the lower-level XDR calls found after the
 * I/O buffering code, below.
 */

/*
 * XDR integers
 */
bool_t
h4_xdr_int(XDR *xdrs, int *ip)
{
    switch (xdrs->x_op) {

        case XDR_ENCODE:
            return h4_xdr_put32(xdrs, (uint32_t *)ip);

        case XDR_DECODE:
            return h4_xdr_get32(xdrs, (uint32_t *)ip);

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
h4_xdr_u_int(XDR *xdrs, unsigned *up)
{
    switch (xdrs->x_op) {

        case XDR_ENCODE:
            return h4_xdr_put32(xdrs, up);

        case XDR_DECODE:
            return h4_xdr_get32(xdrs, up);

        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR opaque data
 *
 * Allows the specification of a fixed size sequence of opaque bytes.
 * cp points to the opaque object and cnt gives the byte length.
 */
bool_t
h4_xdr_opaque(XDR *xdrs, char *cp, unsigned cnt)
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
        if (!h4_xdr_getbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return h4_xdr_getbytes(xdrs, (char *)crud, rndup);
    }

    if (xdrs->x_op == XDR_ENCODE) {
        if (!h4_xdr_putbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return h4_xdr_putbytes(xdrs, xdr_zero, rndup);
    }

    if (xdrs->x_op == XDR_FREE)
        return TRUE;

    return FALSE;
}

/*
 * XDR counted bytes
 *
 * This is a special data structure that starts with the size, followed
 * by the raw bytes (as an opaque XDR type).
 *
 * For raw, uninterpreted byte I/O, see the xdr_put/getbytes() calls.
 *
 * *cpp is a pointer to the bytes, *sizep is the count.
 * If *cpp is NULL maxsize bytes are allocated
 */
bool_t
h4_xdr_bytes(XDR *xdrs, char **cpp, unsigned *sizep, unsigned maxsize)
{
    char    *sp = *cpp; /* sp is the actual string pointer */
    unsigned nodesize;
    bool_t   ret;
    bool_t   allocated = FALSE;

    /*
     * first deal with the length since xdr bytes are counted
     */
    if (!h4_xdr_u_int(xdrs, sizep))
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
                fprintf(stderr, "h4_xdr_bytes: out of memory\n");
                return FALSE;
            }
            /* FALLTHROUGH */

        case XDR_ENCODE:
            ret = h4_xdr_opaque(xdrs, sp, nodesize);
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

/*
 * XDR 4-byte floats
 */
bool_t
h4_xdr_float(XDR *xdrs, float *fp)
{
    switch (xdrs->x_op) {

        case XDR_ENCODE:
            return h4_xdr_int(xdrs, (int *)fp);

        case XDR_DECODE:
            return h4_xdr_int(xdrs, (int *)fp);

        case XDR_FREE:
            return TRUE;
    }
    return FALSE;
}

/*
 * XDR 8-byte doubles
 *
 * Performed in two 32-bit operations because XDR does not have a 64-bit
 * I/O operation.
 */
bool_t
h4_xdr_double(XDR *xdrs, double *dp)
{
    int   *ip;
    bool_t rv;

    if (!dp)
        return FALSE;

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            ip = (int *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = h4_xdr_int(xdrs, ip);
            if (!rv)
                return rv;
            rv = h4_xdr_int(xdrs, ip + 1);
#else
            rv = h4_xdr_int(xdrs, ip + 1);
            if (!rv)
                return rv;
            rv = h4_xdr_int(xdrs, ip);
#endif
            return rv;
            break;

        case XDR_DECODE:
            ip = (int *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = h4_xdr_int(xdrs, ip);
            if (!rv)
                return rv;
            rv = h4_xdr_int(xdrs, ip + 1);
#else
            rv = h4_xdr_int(xdrs, ip + 1);
            if (!rv)
                return rv;
            rv = h4_xdr_int(xdrs, ip);
#endif
            return rv;
            break;

        case XDR_FREE:
            return TRUE;
    }

    return FALSE;
}

/****************************/
/* I/O Buffer Functionality */
/****************************/

/* As an optimization, the XDR I/O layer uses a single I/O buffer to
 * speed up nearby reads and writes ("bio" == Buffered I/O).
 *
 * TODO: This is not a very big cache! Bumping it might enhance I/O
 *       performance.
 */

/* Size of the I/O buffer in bytes */
#define BIOBUFSIZ 8192

/* # of valid bytes in the buffer */
#define CNT(p) ((p)->ptr - (p)->base)

/* # of unread bytes in buffer */
#define REM(p) ((p)->cnt - CNT(p))

/* Available space for write in buffer */
#define BREM(p) (BIOBUFSIZ - CNT(p))

/* POSIX and I/O buffer info
 *
 * Stored in the XDR struct's private data
 *
 * NOTE: off_t is 32-bits on Windows, but that shouldn't be a problem since
 *       HDF4 files are limited to 4 GB.
 */
typedef struct biobuf {
    int      fd;              /* POSIX file descriptor */
    int      mode;            /* File access mode, O_RDONLY, etc */
    int      isdirty;         /* Dirty buffer flag */
    off_t    page;            /* Location in the file */
    int      nread;           /* Number of bytes successfully read */
    int      nwrote;          /* Number of bytes last written */
    int      cnt;             /* Number of valid bytes in buffer */
    uint8_t *ptr;             /* Next byte (pointer into base) */
    uint8_t  base[BIOBUFSIZ]; /* Data buffer */
} biobuf;

/*
 * Initialize the POSIX/buffer XDR state
 */
static biobuf *
bio_get_new(int fd, int fmode)
{
    biobuf *biop = NULL;

    if (NULL == (biop = (biobuf *)calloc(1, sizeof(biobuf))))
        return NULL;

    biop->fd   = fd;
    biop->mode = fmode;
    biop->ptr  = biop->base;

    return biop;
}

/*
 * Read a page from the file into the buffer
 */
static int
bio_read_page(biobuf *biop)
{
    /* Clear out the buffer */
    memset(biop->base, 0, BIOBUFSIZ);

    if (biop->mode & O_WRONLY) {
        /* If we're only writing, the buffer is empty */
        biop->cnt = 0;
    }
    else {
        if (biop->nwrote != BIOBUFSIZ) {
            /* Last write wasn't a full block, adjust position ahead */
            if (lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
                return -1;
        }

        /* Read from storage into the buffer */
        biop->nread = biop->cnt = read(biop->fd, (void *)biop->base, BIOBUFSIZ);
    }

    biop->ptr = biop->base;

    return biop->cnt;
}

/*
 * Write a page from the buffer to the file
 */
static int
bio_write_page(biobuf *biop)
{

    if (!((biop->mode & O_WRONLY) || (biop->mode & O_RDWR)) || biop->cnt == 0) {
        biop->nwrote = 0;
    }
    else {
        if (biop->nread != 0) {
            /* If we read something, we have to adjust position back */
            if (lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
                return -1;
        }
        biop->nwrote = write(biop->fd, (void *)biop->base, biop->cnt);
    }
    biop->isdirty = 0;

    return biop->nwrote;
}

/*
 * Get the next page from the file
 *
 * Returns the number of valid bytes in the buffer
 */
static int
bio_get_next_page(biobuf *biop)
{
    /* Flush if dirty */
    if (biop->isdirty) {
        if (bio_write_page(biop) < 0)
            return -1;
    }

    biop->page++;

    /* Read in the next page */
    if (bio_read_page(biop) < 0)
        return -1;

    return biop->cnt;
}

/*
 * Read bytes from the file through the buffer
 */
static int
bio_read(biobuf *biop, unsigned char *ptr, int nbytes)
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
        if (bio_get_next_page(biop) <= 0)
            return ngot;
    }

    /* We know nbytes <= REM at this point */
    (void)memcpy(ptr, biop->ptr, (size_t)nbytes);
    biop->ptr += nbytes;
    ngot += nbytes;
    return ngot;
}

/*
 * Write bytes to the file through the buffer
 */
static int
bio_write(biobuf *biop, unsigned char *ptr, int nbytes)
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
        if (bio_get_next_page(biop) < 0)
            return nwrote;
    }

    /* We know nbytes <= BREM at this point */
    (void)memcpy(biop->ptr, ptr, (size_t)nbytes);
    biop->isdirty = !0;
    biop->ptr += nbytes;
    if ((cnt = CNT(biop)) > biop->cnt)
        biop->cnt = cnt;
    nwrote += nbytes;

    return nwrote;
}

/***********************/
/* Low-Level XDR Calls */
/***********************/

/*
 * Read/write the number of bytes in a C long to the XDR file. 'uint32_t' is
 * being used in the sense of '4 bytes' and not a long integer value.
 */

static bool_t
h4_xdr_get32(XDR *xdrs, uint32_t *up)
{
    uint8_t *p = (uint8_t *)up;

    if (bio_read((biobuf *)xdrs->x_private, p, 4) < 4)
        return FALSE;

#ifndef H4_WORDS_BIGENDIAN
    /* Reminder that ntohl returns a uint32_t on modern systems */
    *up = ntohl(*up);
#endif

    return TRUE;
}

static bool_t
h4_xdr_put32(XDR *xdrs, const uint32_t *up)
{

    uint8_t *p = (uint8_t *)up;

#ifndef H4_WORDS_BIGENDIAN
    /* Reminder that htonl returns a uint32_t on modern systems */
    uint32_t mycopy = htonl(*up);
    p               = (uint8_t *)&mycopy;
#endif

    if (bio_write((biobuf *)xdrs->x_private, p, 4) < 4)
        return FALSE;
    return TRUE;
}

/*
 * Read/Write a bunch of bytes to/from an XDR file
 *
 * NOTE: These deal with raw bytes, not "counted bytes", like h4_xdr_bytes() does!
 */

bool_t
h4_xdr_getbytes(XDR *xdrs, char *addr, unsigned len)
{
    if ((len != 0) && (bio_read((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
        return FALSE;
    return TRUE;
}

bool_t
h4_xdr_putbytes(XDR *xdrs, const char *addr, unsigned len)
{
    if ((len != 0) && (bio_write((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
        return FALSE;
    return TRUE;
}

/*
 * Get/set the position in an XDR file. Goes through the I/O buffer.
 */

unsigned
h4_xdr_getpos(XDR *xdrs)
{
    biobuf *biop = (biobuf *)xdrs->x_private;
    return BIOBUFSIZ * biop->page + CNT(biop);
}

bool_t
h4_xdr_setpos(XDR *xdrs, unsigned pos)
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
                if (bio_write_page(biop) < 0)
                    return FALSE;
            }
            if (page != biop->page + 1)
                biop->nwrote = 0; /* force seek in bio_read_page */

            biop->page = page;

            nread = bio_read_page(biop);
            if (nread < 0 || ((biop->mode & O_RDONLY) && nread < index))
                return FALSE;
        }
        biop->ptr = biop->base + index;
        return TRUE;
    }
    else
        return FALSE;
}

/************************/
/* XDR File State Calls */
/************************/

/*
 * Set up the I/O buffer in the passed-in XDR struct
 *
 * This is used in the netCDF code, which has a different way of setting
 * up the file.
 */
void
h4_xdr_setup_nofile(XDR *xdrs, int ncop)
{
    biobuf *biop = bio_get_new(-1, 0);

    if (ncop & NC_CREAT)
        xdrs->x_op = XDR_ENCODE;
    else
        xdrs->x_op = XDR_DECODE;

    xdrs->x_private = biop;
}

/*
 * Initialize a POSIX XDR file
 *
 * Sets up XDR to use the POSIX file descriptor fd
 *
 * Operation flag is initialized to op
 */
int
h4_xdr_create(XDR *xdrs, int fd, int fmode, enum xdr_op op)
{
    biobuf *biop    = bio_get_new(fd, fmode);
    xdrs->x_op      = op;
    xdrs->x_private = biop;
    if (biop == NULL)
        return -1;

    /* If write only, or just created (empty), done */
    if ((biop->mode & O_WRONLY) || (biop->mode & O_CREAT))
        return 0;

    /* Else, read the first bufferful */
    return bio_read_page(biop);
}

/*
 * Flush the I/O buffer to the file
 */
int
h4_xdr_sync(XDR *xdrs)
{
    biobuf *biop = (biobuf *)xdrs->x_private;
    if (biop->isdirty) {
        /* Flush */
        if (bio_write_page(biop) < 0)
            return -1;
    }

    biop->nwrote = 0; /* Force seek in bio_read_page */

    /* Read it in */
    if (bio_read_page(biop) < 0)
        return -1;

    return biop->cnt;
}

/*
 * Destroy a POSIX XDR stream
 *
 * Cleans up after h4_xdr_create()
 */
void
h4_xdr_destroy(XDR *xdrs)
{
    /* Flush */
    biobuf *biop = (biobuf *)xdrs->x_private;
    if (biop != NULL) {
        if (biop->isdirty) {
            (void)bio_write_page(biop);
        }
        if (biop->fd != -1)
            (void)close(biop->fd);
        free(biop);
    }
}
