/*-
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/*****************************************************************************
 * File: fmpool.h
 *
 * This is a modfied version of the original Berkley code for
 * manipulating a memory pool. This version however is not 
 * compatible with the original Berkley version.
 *
 *****************************************************************************/ 

/* $Id$ */

#ifndef _FMPOOL_H
#define _FMPOOL_H

#include "fmpconf.h"
#include "fmptypes.h" /* all the defined types */
#include "queue.h"    /* Circluar queue functions(Macros) */

/*
 * The memory pool scheme is a simple one.  Each in-memory page is referenced
 * by a bucket which is threaded in up to two (three?) ways.  All active pages
 * are threaded on a hash chain (hashed by page number) and an lru chain.
 * (Inactive pages are threaded on a free chain?).  Each reference to a memory
 * pool is handed an opaque MPOOL cookie which stores all of this information.
 */

/* Current Hash table size. Page numbers start with 0 
* Going try start with 1 (i.e 0 will denote invalid page number) */
#define	HASHSIZE	128
#if 0
#define	HASHKEY(pgno)	(pgno % HASHSIZE)
#endif
#define	HASHKEY(pgno)	((pgno -1) % HASHSIZE)

/* Default pagesize and max # of pages to cache */
#define DEF_PAGESIZE   8192
#define DEF_MAXCACHE   2

/* The BKT structures are the elements of the queues. */
typedef struct _bkt 
{
  CIRCLEQ_ENTRY(_bkt) hq;	/* hash queue */
  CIRCLEQ_ENTRY(_bkt) q;	/* lru queue */
  void    *page;		/* page */
  pageno_t   pgno;		/* page number */

#define	MPOOL_DIRTY	0x01	/* page needs to be written */
#define	MPOOL_PINNED	0x02	/* page is pinned into memory */
  u_int8_t flags;		/* flags */
} BKT;

/* The element structure for every page referenced(read/written) in file */
typedef struct _lelem
{
  CIRCLEQ_ENTRY(_lelem) hl;	/* hash list */
  pageno_t        pgno;           /* page number */
#ifdef STATISTICS
  u_int32_t	elemhit;        /* # of hits on page */
#endif
#define ELEM_READ       0x01
#define ELEM_WRITTEN    0x02
#define ELEM_SYNC       0x03
  u_int8_t      eflags;         /* 0, 1= read, 2=written */
} L_ELEM;

#define	MPOOL_EXTEND    0x10	/* increase number of pages 
                                   i.e extend file */

/* Memory pool */
typedef struct MPOOL 
{
  CIRCLEQ_HEAD(_lqh, _bkt)    lqh;	      /* lru queue head */
  CIRCLEQ_HEAD(_hqh, _bkt)    hqh[HASHSIZE];  /* hash queue array */
  CIRCLEQ_HEAD(_lhqh, _lelem) lhqh[HASHSIZE]; /* hash of all elements */
  pageno_t	curcache;		      /* current number of cached pages */
  pageno_t	maxcache;		      /* max number of cached pages */
  pageno_t	npages;			      /* number of pages in the file */
  u_int32_t	lastpagesize;	              /* page size of last page */
  u_int32_t	pagesize;		      /* file page size */
  fmp_file_t    fd;			      /* file handle */
  void (*pgin) __P((void *, pageno_t, void *)); /* page in conversion routine */
  void (*pgout) __P((void *, pageno_t, void *));/* page out conversion routine */
  void	*pgcookie;                            /* cookie for page in/out routines */
#ifdef STATISTICS
  u_int32_t	listhit;                /* # of list hits */
  u_int32_t	listalloc;              /* # of list elems allocated */
  u_int32_t	cachehit;               /* # of cache hits */
  u_int32_t	cachemiss;              /* # of cache misses */
  u_int32_t	pagealloc;              /* # of pages allocated */
  u_int32_t	pageflush;              /* # of pages flushed */
  u_int32_t	pageget;                /* # of pages requested from pool */
  u_int32_t	pagenew;                /* # of new pages */
  u_int32_t	pageput;                /* # of pages put back into pool */
  u_int32_t	pageread;               /* # of pages read from file */
  u_int32_t	pagewrite;              /* # of pages written to file */
#endif /* STATISTICS */
} MPOOL;

__BEGIN_DECLS
MPOOL	*mpool_open __P((void *, fmp_file_t, pageno_t, pageno_t));
void	 mpool_filter __P((MPOOL *, void (*)(void *, pageno_t, void *),
                           void (*)(void *, pageno_t, void *), void *));
void	*mpool_new __P((MPOOL *, pageno_t *, pageno_t, u_int32_t));
void	*mpool_get __P((MPOOL *, pageno_t, u_int32_t));
int	 mpool_put __P((MPOOL *, void *, u_int32_t));
int	 mpool_sync __P((MPOOL *));
int	 mpool_page_sync __P((MPOOL *, pageno_t, u_int32_t));
int	 mpool_close __P((MPOOL *));
int	 mpool_set_lastpagesize __P((MPOOL *,pageno_t));
pageno_t  mpool_get_lastpagesize __P((MPOOL *));
pageno_t  mpool_get_pagesize __P((MPOOL *));
pageno_t  mpool_get_npages __P((MPOOL *));
#ifdef STATISTICS
void	 mpool_stat __P((MPOOL *));
#endif /* STATISTICS */
__END_DECLS

#endif /* _FMPOOL_H */
