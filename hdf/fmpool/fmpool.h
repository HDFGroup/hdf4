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
#define	HASHSIZE	128
#define	HASHKEY(pgno)	(pgno % HASHSIZE)
#if 0
#define	HASHKEY(pgno)	((pgno - 1) % HASHSIZE)
#endif

/* Default pagesize and max # of pages to cache */
#define DEF_PAGESIZE   8192
#define DEF_MAXCACHE   128

/* The BKT structures are the elements of the queues. */
typedef struct _bkt 
{
  CIRCLEQ_ENTRY(_bkt) hq;		/* hash queue */
  CIRCLEQ_ENTRY(_bkt) q;		/* lru queue */
  void    *page;			/* page */
  pgno_t   pgno;			/* page number */

#define	MPOOL_DIRTY	0x01		/* page needs to be written */
#define	MPOOL_PINNED	0x02		/* page is pinned into memory */
  u_int8_t flags;			/* flags */
} BKT;

/* The element structure for every page referenced(read/written) in file */
typedef struct _lelem
{
  CIRCLEQ_ENTRY(_lelem) hl;		/* hash list */
  pgno_t pgno;                          /* page number */

#define ELEM_READ    0x01
#define ELEM_WRITTEN 0x02
#define ELEM_SYNC    0x03
  u_int8_t eflags;                      /* 0, 1= read, 2=written */
#ifdef STATISTICS
  u_int	elemhit;                /* # of hits on page */
#endif
} L_ELEM;

#define	MPOOL_EXTEND	0x10		/* increase number of pages 
                                           i.e extend file */
/* Memory pool */
typedef struct MPOOL 
{
  CIRCLEQ_HEAD(_lqh, _bkt) lqh;	/* lru queue head */
                                /* hash queue array */
  CIRCLEQ_HEAD(_hqh, _bkt) hqh[HASHSIZE];
  CIRCLEQ_HEAD(_lhqh, _lelem)  lhqh[HASHSIZE];
  pgno_t	curcache;		/* current number of cached pages */
  pgno_t	maxcache;		/* max number of cached pages */
  pgno_t	npages;			/* number of pages in the file */
  u_int	lastpagesize;		/* page size of last page */
  u_int	pagesize;		/* file page size */
  fmp_file_t    fd;			/* file descriptor */
                                /* page in conversion routine */
  void    (*pgin) __P((void *, pgno_t, void *));
                                /* page out conversion routine */
  void    (*pgout) __P((void *, pgno_t, void *));
  void	*pgcookie;		/* cookie for page in/out routines */
#ifdef STATISTICS
  u_int	listhit;                /* # of list hits */
  u_int	listalloc;              /* # of list elems allocated */
  u_int	cachehit;               /* # of cache hits */
  u_int	cachemiss;              /* # of cache misses */
  u_int	pagealloc;              /* # of pages allocated */
  u_int	pageflush;              /* # of pages flushed */
  u_int	pageget;                /* # of pages requested from pool */
  u_int	pagenew;                /* # of new pages */
  u_int	pageput;                /* # of pages put back into pool */
  u_int	pageread;               /* # of pages read from file */
  u_int	pagewrite;              /* # of pages written to file */
#endif /* STATISTICS */
} MPOOL;

#ifdef USE_INLINE

#define FMPOOL_PUT(mp, page, fflags, status, put_done, do_stats, do_mpool_debug) \
 { \
  BKT *bp = NULL; /* bucket element ptr */ \
  int ret_value = RET_SUCCESS; \
\
  /* check inputs */ \
  if (mp == NULL || page == NULL) \
    { \
      ret_value = RET_ERROR; \
      goto put_done; \
    } \
  if (do_stats) \
    ++mp->pageput; \
  /* get pointer to bucket element */ \
  bp = (BKT *)((char *)page - sizeof(BKT)); \
  if(do_mpool_debug) \
    { \
     (void)fprintf(stderr,"mpool_put: putting page=%d\n",bp->pgno); \
     if (!(bp->flags & MPOOL_PINNED))  \
      { \
        (void)fprintf(stderr, \
                    "mpool_put: page %d not pinned\n", bp->pgno); \
        abort(); \
     } \
   } \
  /* Unpin the page and mark it appropriately */ \
  bp->flags &= ~MPOOL_PINNED; \
  bp->flags |= fflags & MPOOL_DIRTY; \
\
put_done: \
  if(ret_value == RET_ERROR) \
    { /* error cleanup */ \
      status = RET_ERROR; \
    } \
  /* Normal cleanup */ \
\
   status = RET_SUCCESS; \
} /* mpool_put () */


#define FMPOOL_GET(gmp, gpgno, gflags, gstatus, get_done, do_stats, do_mpool_debug) \
{ \
  struct _hqh *head = NULL; /* head of lru queue */ \
  BKT         *gbp   = NULL; /* bucket element */ \
  off_t       off;         /* file offset? */ \
  int         nr;          /* number of bytes read for page */ \
  int         rpagesize;   /* pagesize to read */ \
  int         gret_value = RET_SUCCESS; \
\
  /* check inputs */ \
  if (gmp == NULL) \
    { \
      gret_value = RET_ERROR; \
      goto get_done; \
    } \
\
  /* Check for attempting to retrieve a non-existent page. */ \
  /*  remember pages go from 0 ->npages -1 */ \
  if (gpgno >= gmp->npages)  \
    { \
      errno = EINVAL; \
      gret_value = RET_ERROR; \
      goto get_done; \
    } \
\
  if(do_stats) \
    ++gmp->pageget; \
\
  /* Check for a page that is cached. */  \
  if ((gbp = mpool_look(gmp, gpgno)) != NULL) \
    { \
      if (do_mpool_debug) \
        { \
         if (gbp->flags & MPOOL_PINNED) \
          { \
           (void)fprintf(stderr, \
                        "mpool_get: page %d already pinned\n", gbp->pgno); \
           abort(); \
         } \
        } \
     /* Move the page to the head of the hash chain and the tail */ \
     /* of the lru chain.*/ \
      head = &gmp->hqh[HASHKEY(gbp->pgno)]; \
      CIRCLEQ_REMOVE(head, gbp, hq); \
      CIRCLEQ_INSERT_HEAD(head, gbp, hq); \
      CIRCLEQ_REMOVE(&gmp->lqh, gbp, q); \
      CIRCLEQ_INSERT_TAIL(&gmp->lqh, gbp, q); \
      if (do_mpool_debug) \
        (void)fprintf(stderr,"mpool_get: getting cached gbp->pgno=%d,npages=%d\n", \
                      gbp->pgno,gmp->npages); \
\
      /* Return a pinned page. */ \
      gbp->flags |= MPOOL_PINNED; \
      gret_value = RET_SUCCESS; \
      goto get_done; \
    } /* end if gbp */ \
\
  /* Page not cached so */ \
  /* Get a page from the cache to use or create one. */ \
  if ((gbp = mpool_bkt(gmp)) == NULL) \
    { \
      gret_value = RET_ERROR; \
      goto get_done; \
    } \
\
  if (do_stats) \
    ++gmp->pageread; \
\
  /* Check to see if we are reading in last page */ \
  if (gpgno != (gmp->npages -1)) \
    { /* regular page */ \
      rpagesize = gmp->pagesize; \
    if(do_mpool_debug) \
     (void)fprintf(stderr,"mpool_get: reading in page=%d\n",gpgno); \
    } \
  else \
    { /* reading in last page */ \
      rpagesize = gmp->lastpagesize; \
      if(do_mpool_debug) \
      (void)fprintf(stderr,"mpool_get: reading in last page=%d\n",gpgno); \
    } \
\
  /* Get ready to read page */ \
  off = gmp->pagesize * gpgno; \
  if (FMPI_SEEK(gmp->fd, off) == FAIL) \
    { \
      ret = RET_ERROR; \
      goto get_done; \
    } \
\
  /* We do this to see if we really have reached this postion */ \
  if (FMPI_TELL(gmp->fd) != off) \
    { \
      if(do_mpool_debug) \
      (void)fprintf(stderr,"mpool_get: lseek error=%d\n",off); \
      gret_value = RET_ERROR; \
      goto get_done; \
    } \
\
  /* Read in the contents. */ \
  if ((nr = FMPI_READ(gmp->fd, gbp->page, rpagesize)) != rpagesize)  \
    { \
      if (nr >= 0) \
        errno = EFTYPE; \
      gret_value = RET_ERROR; \
      goto get_done; \
    } \
\
  /* Set the page number, pin the page. */ \
  gbp->pgno = gpgno; \
  gbp->flags = MPOOL_PINNED; \
\
  /* * Add the page to the head of the hash chain and the tail */ \
  /* of the lru chain.*/ \
  head = &gmp->hqh[HASHKEY(gbp->pgno)]; \
  CIRCLEQ_INSERT_HEAD(head, gbp, hq); \
  CIRCLEQ_INSERT_TAIL(&gmp->lqh, gbp, q); \
\
  /* Run through the user's filter. */ \
  if (gmp->pgin != NULL) \
    (gmp->pgin)(gmp->pgcookie, gbp->pgno, gbp->page); \
\
get_done: \
  if(gret_value == RET_ERROR) \
    { /* error cleanup */ \
      gstatus = NULL; \
    } \
  /* Normal cleanup */ \
\
  gstatus = gbp->page; \
} /* mpool_get() */

#endif /* USE_INLINE */

__BEGIN_DECLS
MPOOL	*mpool_open __P((void *, fmp_file_t, pgno_t, pgno_t));
void	 mpool_filter __P((MPOOL *, void (*)(void *, pgno_t, void *),
                           void (*)(void *, pgno_t, void *), void *));
void	*mpool_new __P((MPOOL *, pgno_t *, pgno_t, u_int));
void	*mpool_get __P((MPOOL *, pgno_t, u_int));
int	 mpool_put __P((MPOOL *, void *, u_int));
int	 mpool_sync __P((MPOOL *));
int	 mpool_page_sync __P((MPOOL *, pgno_t, u_int));
int	 mpool_close __P((MPOOL *));
#ifdef USE_INLINE
BKT	*mpool_look __P((MPOOL *, pgno_t));
BKT	*mpool_bkt __P((MPOOL *));
#endif
#ifdef STATISTICS
void	 mpool_stat __P((MPOOL *));
#endif /* STATISTICS */
__END_DECLS

#endif /* _FMPOOL_H */
