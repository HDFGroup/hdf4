#if !defined(lint)
static char rcsid[] = "$Id$";
#endif

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
 *
 * -glenn
 */

/*
 * 32-bit integer on the host architecture (on the CRAY, this is actually 64
 * bits; however, a pointer to this type is still passed to the XDR functions
 * x_getlong() and x_putlong(), so, on that platform, it doesn't matter if
 * the following isn't 32-bits):
 */
#ifdef CRAY
#   undef  NETLONG
#   define NETLONG	long
#endif
#ifndef	NETLONG
#   define NETLONG	long
#endif
typedef	NETLONG		netlong;
#undef	NETLONG

#ifdef vms
#   include <unixio.h>
#   include <file.h>
#else
#   ifdef MSDOS
#       include <io.h>
#   else
#       ifndef macintosh
#            include <unistd.h>
#       endif
#   endif
#   include <fcntl.h>
#endif
#ifdef macintosh
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <string.h>
#include "netcdf.h" /* NC_ */
#include "local_nc.h" /* prototypes for NCadvis, nc_error */
		      /* also obtains <stdio.h>, <rpc/types.h>, &
		       * <rpc/xdr.h> */
#include "mfhdf.h"
#ifdef macintosh
typedef long off_t;
#endif

#ifndef DOS_FS
typedef u_int ncpos_t ;
#else
typedef off_t ncpos_t ;
#endif

typedef struct {
	int fd;			/* the file descriptor */	
	int mode;		/* file access mode, O_RDONLY, etc */
	int isdirty ;
	off_t page ;
	int nread ;		/* number of bytes succesfully read */
	int nwrote ;	/* number of bytes last write */
	int cnt ;		/* number of valid bytes in buffer */
	unsigned char *ptr; 		/* next byte */
#ifndef CRAY
#ifdef DOS_FS
#ifdef QAK
#define BIOBUFSIZ   4096
#else
#define BIOBUFSIZ   512
#endif
#else
#define BIOBUFSIZ	8192
#endif
#else
#define BIOBUFSIZ	196608 /* stat.st_oblksize */
#endif
	unsigned char base[BIOBUFSIZ];		/* the data buffer */
} biobuf;


static void
free_biobuf(abuf)
biobuf *abuf;
{
	if(abuf == NULL)
		return;
	HDfreespace((VOIDP)abuf) ;
}


static biobuf *
new_biobuf(fd, fmode)
int fd;
int fmode;
{
	biobuf *biop ;

	biop = (biobuf *)HDgetspace(sizeof(biobuf)) ;
	if(biop == NULL)
		return NULL ;
	biop->fd = fd ;

	biop->mode = fmode ;

	biop->isdirty = 0 ;
	biop->page = 0 ;
	biop->nread = 0 ;
	biop->nwrote = 0 ;
	biop->cnt = 0 ;
	memset(biop->base, 0, ((size_t)(BIOBUFSIZ))) ;
	biop->ptr = biop->base ;

	return biop ;
}


static int
rdbuf(biop)
biobuf *biop;
{
	memset(biop->base, 0, ((size_t)(BIOBUFSIZ))) ;

	if(biop->mode & O_WRONLY)
	{
		biop->cnt = 0 ;
	}
	else
	{
		if(biop->nwrote != BIOBUFSIZ)
		{
			/* last write wasn't a full block, adjust position ahead */
			if(lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
				return -1 ;
		}
		biop->nread = biop->cnt = read(biop->fd, biop->base, BIOBUFSIZ) ;
	}
	biop->ptr = biop->base ;
	return biop->cnt ;
}


static int
wrbuf(biop)
biobuf *biop;
{

	if(!((biop->mode & O_WRONLY) || (biop->mode & O_RDWR))
		|| biop->cnt == 0) 
	{
		biop->nwrote = 0 ;
	}
	else
	{
		if(biop->nread != 0)
		{
			/* if we read something, we have to adjust position back */
			if(lseek(biop->fd, biop->page * BIOBUFSIZ, SEEK_SET) == ((off_t)-1))
				return -1 ;
		}
		biop->nwrote = write(biop->fd, biop->base, biop->cnt) ;
	}
	biop->isdirty = 0 ;

	return biop->nwrote ;
}

static int
nextbuf(biop)
biobuf *biop;
{
	if(biop->isdirty)
	{
		if(wrbuf(biop) < 0)
			return -1 ;
	}

	biop->page++ ;

	/* read it in */
	if(rdbuf(biop) < 0 )
		return -1 ;

	return biop->cnt ;
}


#define CNT(p) ((p)->ptr - (p)->base)
/* # of unread bytes in buffer */
#define REM(p) ((p)->cnt - CNT(p)) 
/* available space for write in buffer */
#define BREM(p) (BIOBUFSIZ - CNT(p))

static int
bioread(biop, ptr, nbytes)
biobuf *biop;
unsigned char *ptr;
int nbytes;
{
	int ngot = 0 ;
	size_t rem ; 

	if(nbytes == 0)
		return 0 ;	

	while(nbytes > (rem = REM(biop)))
	{
		if(rem > 0)
		{
			(void) memcpy(ptr, biop->ptr, rem) ;
			ptr += rem ;
			nbytes -= rem ;
			ngot += rem ;
		}
		if(nextbuf(biop) <= 0)
			return ngot ;
	}
	/* we know nbytes <= REM at this point */
	(void) memcpy(ptr, biop->ptr, (size_t)nbytes) ;
	biop->ptr += nbytes ;
	ngot += nbytes ;
	return ngot ;
}


static int
biowrite(biop, ptr, nbytes)
biobuf *biop;
unsigned char *ptr;
int nbytes;
{
	size_t rem ;
	int nwrote = 0 ;
	int cnt ;

	if(!((biop->mode & O_WRONLY) || (biop->mode & O_RDWR)))
		return -1 ;

	while(nbytes > (rem = BREM(biop)))
	{
		if(rem > 0)
		{
			(void) memcpy(biop->ptr, ptr, rem) ;
			biop->isdirty = !0 ;
			biop->cnt = BIOBUFSIZ ;
			ptr += rem ;
			nbytes -= rem ;
			nwrote += rem ;
		}
		if(nextbuf(biop) < 0)
			return nwrote ;
	}
	/* we know nbytes <= BREM at this point */
	(void) memcpy(biop->ptr, ptr, (size_t)nbytes) ;
	biop->isdirty = !0 ;
	biop->ptr += nbytes ;
	if((cnt = CNT(biop)) > biop->cnt)
		biop->cnt = cnt ;
	nwrote += nbytes ;

	return nwrote ;
}


static bool_t	xdrposix_getlong();
static bool_t	xdrposix_putlong();
static bool_t	xdrposix_getbytes();
static bool_t	xdrposix_putbytes();
static ncpos_t	xdrposix_getpos();
static bool_t	xdrposix_setpos();
#ifdef CRAY
static inline_t *	xdrposix_inline();
#else
static netlong *	xdrposix_inline();
#endif
static void	xdrposix_destroy();

/*
 * Ops vector for posix type XDR
 */
static struct xdr_ops	xdrposix_ops = {
	xdrposix_getlong,	/* deserialize a 32-bit int */
	xdrposix_putlong,	/* serialize a 32-bit int */
	xdrposix_getbytes,	/* deserialize counted bytes */
	xdrposix_putbytes,	/* serialize counted bytes */
	xdrposix_getpos,	/* get offset in the stream */
	xdrposix_setpos,	/* set offset in the stream */
	xdrposix_inline,	/* prime stream for inline macros */
	xdrposix_destroy	/* destroy stream */
};


/*
 * Fake an XDR initialization for HDF files
 */
hdf_xdrfile_create(xdrs, ncop)
     XDR *xdrs;
     int ncop;
{
    biobuf *biop = new_biobuf(-1, 0) ;
    
    if(ncop & NC_CREAT)
        xdrs->x_op = XDR_ENCODE;
    else
        xdrs->x_op = XDR_DECODE;
        
    xdrs->x_ops = &xdrposix_ops;
    xdrs->x_private = (caddr_t) biop;
    
} /* hdf_xdrfile_create */


/*
 * Initialize a posix xdr stream.
 * Sets the xdr stream handle xdrs for use on the file descriptor fd.
 * Operation flag is set to op.
 */
static int
xdrposix_create(xdrs, fd, fmode, op)
	XDR *xdrs;
	int fd;
	int fmode;
	enum xdr_op op;
{

#ifndef macintosh
	biobuf *biop = new_biobuf(fd, fmode) ;
#endif /* !macintosh */
#ifdef XDRDEBUG
fprintf(stderr,"xdrposix_create(): xdrs=%p, fd=%d, fmode=%d, op=%d\n",xdrs,fd,fmode,(int)op);
fprintf(stderr,"xdrposix_create(): after new_biobuf(), biop=%p\n",biop);
#endif
    xdrs->x_op = op;
#ifndef macintosh 
	xdrs->x_ops = &xdrposix_ops;
	xdrs->x_private = (caddr_t) biop ;
	/* unused */
	xdrs->x_handy = 0;
	xdrs->x_base = 0;
	if(biop == NULL)
		return -1 ;
	
	/* if write only, or just created (empty), done */
	if((biop->mode & O_WRONLY)
			|| (biop->mode & O_CREAT))
		return 0 ;

#ifdef XDRDEBUG
fprintf(stderr,"xdrposix_create(): before rdbuf()\n");
#endif
	/* else, read the first bufferful */
	return( rdbuf(biop) ) ;
#else /* macintosh */
    return 0; /* Just return since we don't handle XDR files */
#endif /* macintosh */
}

/*
 * "sync" a posix xdr stream.
 */
static int
xdrposix_sync(xdrs)
	XDR *xdrs;
{
	biobuf *biop = (biobuf *)xdrs->x_private ;
	if(biop->isdirty)
	{
		/* flush */
		if(wrbuf(biop) < 0)
			return -1 ;
	}

	biop->nwrote = 0 ; /* force seek in rdbuf */

	/* read it in */
	if(rdbuf(biop) < 0 )
		return -1 ;

	return biop->cnt ;
}


/*
 * Destroy a posix xdr stream.
 * Cleans up the xdr stream handle xdrs previously set up by xdrposix_create.
 */
static void
xdrposix_destroy(xdrs)
	XDR *xdrs;
{
	/* flush */
	biobuf *biop = (biobuf *)xdrs->x_private ;
	if(biop->isdirty)
	{
		(void) wrbuf(biop) ;
	}
	if(biop->fd != -1) 
            (void) close(biop->fd) ;
	free_biobuf(biop);
}

static bool_t
xdrposix_getlong(xdrs, lp)
	XDR *xdrs;
	netlong *lp;
{
	unsigned char *up = (unsigned char *)lp ;
#ifdef CRAY
	*lp = 0 ;
	up += (sizeof(long) - 4) ;
#endif
	if(bioread((biobuf *)xdrs->x_private, up, 4) < 4)
		return (FALSE);
#ifdef SWAP
	*lp = ntohl(*lp);
#endif
	return (TRUE);
}

static bool_t
xdrposix_putlong(xdrs, lp)
	XDR *xdrs;
	netlong *lp;
{

	unsigned char *up = (unsigned char *)lp ;
#ifdef SWAP
	netlong mycopy = htonl(*lp);
	up = (unsigned char *)&mycopy;
#endif
#ifdef CRAY
	up += (sizeof(long) - 4) ;
#endif

	if (biowrite((biobuf *)xdrs->x_private, up, 4) < 4)
		return (FALSE);
	return (TRUE);
}

static bool_t
xdrposix_getbytes(xdrs, addr, len)
	XDR *xdrs;
	caddr_t addr;
	u_int len;
{

	if ((len != 0)
			&& (bioread((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
		return (FALSE);
	return (TRUE);
}

static bool_t
xdrposix_putbytes(xdrs, addr, len)
	XDR *xdrs;
	caddr_t addr;
	u_int len;
{

	if ((len != 0)
			&& (biowrite((biobuf *)xdrs->x_private, (unsigned char *)addr, (int)len) != len))
		return (FALSE);
	return (TRUE);
}

static ncpos_t
xdrposix_getpos(xdrs)
	XDR *xdrs;
{

	biobuf *biop = (biobuf *)xdrs->x_private ;
	return (BIOBUFSIZ * biop->page + CNT(biop));
}

static bool_t
xdrposix_setpos(xdrs, pos) 
	XDR *xdrs;
	ncpos_t pos;
{ 
	biobuf *biop = (biobuf *)xdrs->x_private ;
	off_t page ;
	int index ;
	int nread ;
	page = pos / BIOBUFSIZ ;
	index = pos % BIOBUFSIZ ;
	if(page != biop->page)
	{
		if(biop->isdirty)
		{
			if( wrbuf(biop) < 0)
				return FALSE ;
		}
		if(page != biop->page +1)
			biop->nwrote = 0 ; /* force seek in rdbuf */

		biop->page = page ;
	
		nread = rdbuf(biop) ;
		if(nread < 0
				|| ((biop->mode & O_RDONLY) && nread < index))
			return FALSE ;
	}
	biop->ptr = biop->base + index ;
	return TRUE ;
}

/*ARGSUSED*/
#ifdef CRAY
static inline_t *
#else
static netlong *
#endif
xdrposix_inline(xdrs, len)
	XDR *xdrs;
	u_int len;
{

	/*
	 * Must do some work to implement this: must insure
	 * enough data in the underlying posix buffer,
	 * that the buffer is aligned so that we can indirect through a
	 * netlong *, and stuff this pointer in xdrs->x_buf.
	 */
	return (NULL);
}


int
NCxdrfile_sync(xdrs)
XDR *xdrs ;
{
	return xdrposix_sync(xdrs) ;
}

int
NCxdrfile_create(xdrs, path, ncmode)
XDR *xdrs ;
const char *path ;
int ncmode ;
{
	int fmode ;
	int	 fd ;
	enum xdr_op op ;

#ifdef XDRDEBUG
fprintf(stderr,"NCxdrfile_create(): XDR=%p, path=%s, ncmode=%d\n",xdrs,path,ncmode);
#endif
	switch(ncmode & 0x0f) {
	case NC_NOCLOBBER :
		fmode = O_RDWR | O_CREAT | O_EXCL ;
		break ;
	case NC_CLOBBER :
		fmode = O_RDWR | O_CREAT | O_TRUNC ;
		break ;
	case NC_WRITE :
		fmode = O_RDWR ;
		break ;
	case NC_NOWRITE :
		fmode = O_RDONLY ;
		break ;
	default:
		NCadvise(NC_EINVAL, "Bad flag %0x", ncmode & 0x0f) ;
		return(-1) ;
	}
#ifdef CRAY
	fmode |= O_RAW ;
#endif
#ifdef DOS_FS
	/*
 	 * set default mode to binary to suppress the expansion of
	 * 0x0f into CRLF
	 */
	if(_fmode != O_BINARY)
		_fmode = O_BINARY ;
#endif
#ifdef macintosh 
    /* fd = open(path, fmode); */
	fd = 1; /* We fake a file descriptor for error purposes */
#else /* !macintosh  */
	fd = open(path, fmode, 0666) ;
#endif /* !macintosh */
#ifdef XDRDEBUG
fprintf(stderr,"NCxdrfile_create(): fmode=%d, fd=%d\n",fmode,fd);
#endif
    if( fd == -1 )
	{
		nc_serror("filename \"%s\"", path) ;
		return(-1) ;
	} /* else */

	if( ncmode & NC_CREAT )
	{
		op = XDR_ENCODE ;
	} else {
		op = XDR_DECODE ;
	}
	
#ifdef XDRDEBUG
fprintf(stderr,"NCxdrfile_create(): before xdrposix_create()\n");
#endif
    if(xdrposix_create(xdrs, fd, fmode, op) < 0)
		return -1 ;
	/* else */
#ifdef XDRDEBUG
fprintf(stderr,"NCxdrfile_create(): after xdrposix_create()\n");
#endif
    return fd ;
}
