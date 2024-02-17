/*-
 * Copyright (c) 1990, 1993, 1994
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
 * 3. Neither the name of the University nor the names of its contributors
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
 */

/*****************************************************************************
 * File: mcache.c
 *
 * This is a modified version of the original Berkeley code for
 * manipulating a memory pool. This version however is not
 * compatible with the original Berkeley version.
 *
 * NOTE: references to pages here is the same as chunks
 *****************************************************************************/

/*
 *  NOTE:
 *    Here pages are the same thing as chunks.
 *    I just didn't bother to change all references from pages to chunks.
 *
 *    -georgev
 */

/*
#define STATISTICS
*/

#include "hdfi.h"
#include "hqueue.h"
#include "mcache.h"

/* Private routines */
static BKT *mcache_bkt(MCACHE *mp);
static BKT *mcache_look(MCACHE *mp, int32 pgno);
static intn mcache_write(MCACHE *mp, BKT *bkt);

/******************************************************************************
NAME
    mcache_get_npages - returns current number of pages for object

DESCRIPTION
    Finds current number of pages in object.

RETURNS
    Returns current number of pages in object.
******************************************************************************/
int32
mcache_get_npages(MCACHE *mp /* IN: MCACHE cookie */)
{
    if (mp != NULL)
        return mp->npages;
    else
        return 0;
} /* mcache_get_npages */

/******************************************************************************
NAME
    mcache_get_maxcache - returns current number of pages cached.

DESCRIPTION
    Finds current number of pages cached for object.

RETURNS
    Returns current number of pages cached.
******************************************************************************/
int32
mcache_get_maxcache(MCACHE *mp /* IN: MCACHE cookie */)
{
    if (mp != NULL)
        return mp->maxcache;
    else
        return 0;
} /* mcache_get_maxcache */

/******************************************************************************
NAME
    mcache_set_maxcache - sets current number of pages to cache.

DESCRIPTION
    Sets current number of pages to cached for object to 'maxcache'.

RETURNS
    Returns current number of pages cached.
******************************************************************************/
int32
mcache_set_maxcache(MCACHE *mp, /* IN: MCACHE cookie */
                    int32   maxcache /* IN: max pages to cache */)
{
    if (mp != NULL) { /* currently allow the current cache to grow up */
        if (mp->maxcache < maxcache)
            mp->maxcache = maxcache;
        else /* maxcache is less than current maxcache */
        {    /* if current number of cached pages is less than request
                then set to 'maxcache',
                else we don't currently handle decreasing
                the curcache to 'maxcache' */
            if (maxcache > mp->curcache)
                mp->maxcache = maxcache;
        }
        return mp->maxcache;
    }
    else
        return 0;
} /* mcache_set_maxcache */

/******************************************************************************
NAME
    mcache_get_pagsize - returns pagesize for object

DESCRIPTION
    Finds current pagesize used for object.

RETURNS
    returns pagesize for object.
******************************************************************************/
int32
mcache_get_pagesize(MCACHE *mp /* IN: MCACHE cookie */)
{
    if (mp != NULL)
        return mp->pagesize;
    else
        return 0;
} /* mcache_get_pagesize */

/******************************************************************************
NAME
   mcache_open -- Open a memory pool on the given object

DESCRIPTION
   Initialize a memory pool for object using the given pagesize
   and size of object.

   Note for 'flags' input only '0' should be used for now.

RETURNS
   A memory pool cookie if successful else NULL

NOTE:
      The key string byte for sharing buffers is not implemented.
******************************************************************************/
MCACHE *
mcache_open(void *key,       /* IN: byte string used as handle to share buffers */
            int32 object_id, /* IN: object handle */
            int32 pagesize,  /* IN: chunk size in bytes  */
            int32 maxcache,  /* IN: maximum number of pages to cache at any time */
            int32 npages,    /* IN: number of chunks currently in object */
            int32 flags /* IN: 0= object exists, 1= does not exist  */)
{
    struct _lhqh *lhead     = NULL; /* head of an entry in list hash chain */
    MCACHE       *mp        = NULL; /* MCACHE cookie */
    L_ELEM       *lp        = NULL;
    intn          ret_value = RET_SUCCESS;
    intn          entry; /* index into hash table */
    int32         pageno;

    (void)key;

    /* Set the pagesize and max # of pages to cache */
    if (pagesize == 0)
        pagesize = (int32)DEF_PAGESIZE;
    if (maxcache == 0)
        maxcache = (int32)DEF_MAXCACHE;

    /* Allocate and initialize the MCACHE cookie. */
    if ((mp = (MCACHE *)calloc(1, sizeof(MCACHE))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    H4_CIRCLEQ_INIT(&mp->lqh);
    for (entry = 0; entry < HASHSIZE; ++entry) {
        H4_CIRCLEQ_INIT(&mp->hqh[entry]);
        H4_CIRCLEQ_INIT(&mp->lhqh[entry]);
    }

    /* Initialize max # of pages to cache and number of pages in object */
    mp->maxcache = (int32)maxcache;
    mp->npages   = npages;

    /* Set pagesize and object handle and current object size */
    mp->pagesize    = pagesize;
    mp->object_id   = object_id;
    mp->object_size = pagesize * npages;

    /* Initialize list hash chain */
    for (pageno = 1; pageno <= mp->npages; ++pageno) {
        lhead = &mp->lhqh[HASHKEY(pageno)];
        if ((lp = (L_ELEM *)malloc(sizeof(L_ELEM))) == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);
        lp->pgno = (int32)pageno; /* set page number */

        /* check if object exists already
           The usefulness of this flag is yet to be
           determined. Currently '0' should be used */
        if (flags == 0)
            lp->eflags = (uint8)ELEM_SYNC; /* valid page exists on disk */
        else
            lp->eflags = (uint8)0; /* page does not exist on disk */
#ifdef STATISTICS
        lp->elemhit = 0;
        ++(mp->listalloc);
#endif
        H4_CIRCLEQ_INSERT_HEAD(lhead, lp, hl); /* add to list */
    }                                          /* end for pageno */

    /* initialize input/output filters and cookie to NULL */
    mp->pgin     = NULL;
    mp->pgout    = NULL;
    mp->pgcookie = NULL;
#ifdef STATISTICS
    mp->listhit   = 0;
    mp->cachehit  = 0;
    mp->cachemiss = 0;
    mp->pagealloc = 0;
    mp->pageflush = 0;
    mp->pageget   = 0;
    mp->pagenew   = 0;
    mp->pageput   = 0;
    mp->pageread  = 0;
    mp->pagewrite = 0;
#endif

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        free(mp);
        /* free up list elements */
        for (entry = 0; entry < HASHSIZE; ++entry) {
            while ((lp = mp->lhqh[entry].cqh_first) != (void *)&mp->lhqh[entry]) {
                H4_CIRCLEQ_REMOVE(&mp->lhqh[entry], mp->lhqh[entry].cqh_first, hl);
                free(lp);
            }
        } /* end for entry */

        mp = NULL; /* return value */
    }
#ifdef STATISTICS
    fprintf(stderr, "mcache_open: mp->listalloc=%lu\n", mp->listalloc);
#endif

    return mp;
} /* mcache_open () */

/******************************************************************************
NAME
   mcache_filter -- Initialize input/output filters.

DESCRIPTION
   Initialize input/output filters for user page processing.
   These are used to read/write the chunks.

RETURNS
   Nothing

******************************************************************************/
void
mcache_filter(MCACHE *mp, /* IN: MCACHE cookie */
              int32 (*pgin)(void * /* cookie */, int32 /* pgno */,
                            void * /* page */), /* IN: page in filter */
              int32 (*pgout)(void * /* cookie */, int32 /* pgno */,
                             const void * /*page */), /* IN: page out filter */
              void *pgcookie /* IN: filter cookie */)
{
    mp->pgin     = pgin;
    mp->pgout    = pgout;
    mp->pgcookie = pgcookie;
} /* mcache_filter() */

/******************************************************************************
NAME
   mcache_get - get a specified page by page number.

DESCRIPTION
    Get a page specified by 'pgno'. If the page is not cached then
    we need to create a new page. All returned pages are pinned.

RETURNS
   The specified page if successful and NULL otherwise
******************************************************************************/
void *
mcache_get(MCACHE *mp,   /* IN: MCACHE cookie */
           int32   pgno, /* IN: page number */
           int32   flags /* IN: XXX not used? */)
{
    struct _hqh  *head      = NULL; /* head of lru queue */
    struct _lhqh *lhead     = NULL; /* head of an entry in list hash chain */
    BKT          *bp        = NULL; /* bucket element */
    L_ELEM       *lp        = NULL;
    intn          ret_value = RET_SUCCESS;
    intn          list_hit; /* hit flag */

    (void)flags;

    /* check inputs */
    if (mp == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check for attempting to retrieve a non-existent page.
     *  remember pages go from 1 ->npages  */
    if (pgno > mp->npages)
        HE_REPORT_GOTO("attempting to get a non-existent page from cache", FAIL);

#ifdef STATISTICS
    ++mp->pageget;
#endif

    /* Check for a page that is cached. */
    if ((bp = mcache_look(mp, pgno)) != NULL) {
        /*
         * Move the page to the head of the hash chain and the tail
         * of the lru chain.
         */
        head = &mp->hqh[HASHKEY(bp->pgno)];
        H4_CIRCLEQ_REMOVE(head, bp, hq);
        H4_CIRCLEQ_INSERT_HEAD(head, bp, hq);
        H4_CIRCLEQ_REMOVE(&mp->lqh, bp, q);
        H4_CIRCLEQ_INSERT_TAIL(&mp->lqh, bp, q);
        /* Return a pinned page. */
        bp->flags |= MCACHE_PINNED;

        /* update this page reference */
        lhead = &mp->lhqh[HASHKEY(bp->pgno)];
        for (lp = lhead->cqh_first; lp != (void *)lhead; lp = lp->hl.cqe_next)
            if (lp->pgno == bp->pgno) { /* hit */
#ifdef STATISTICS
                ++mp->listhit;
                ++lp->elemhit;
#endif
                break;
            } /* end if lp->pgno */

        /* we are done */
        ret_value = RET_SUCCESS;
        goto done;
    } /* end if bp */

    /* Page not cached so
     * Get a page from the cache to use or create one. */
    if ((bp = mcache_bkt(mp)) == NULL)
        HE_REPORT_GOTO("unable to get a new page from bucket", FAIL);

    /* Check to see if this page has ever been referenced */
    list_hit = 0;
    lhead    = &mp->lhqh[HASHKEY(pgno)];
    for (lp = lhead->cqh_first; lp != (void *)lhead; lp = lp->hl.cqe_next)
        if (lp->pgno == pgno && lp->eflags != 0) { /* hit */
#ifdef STATISTICS
            ++mp->listhit;
            ++lp->elemhit;
#endif
            list_hit = 1;
            break;
        } /* end if lp->pgno */

    /* If there is no hit then we allocate a new element
     *  and insert into hash table */
    if (!list_hit) { /* NO hit, new list element
                      * no need to read this page from disk */
        if ((lp = (L_ELEM *)malloc(sizeof(L_ELEM))) == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);

        lp->pgno   = pgno;
        lp->eflags = 0;
#ifdef STATISTICS
        ++mp->listalloc;
        lp->elemhit = 1;
#endif
        H4_CIRCLEQ_INSERT_HEAD(lhead, lp, hl); /* add to list */
    }                                          /*end if !list_hit */
    else {                                     /* list hit, need to read page */
        lp->eflags = ELEM_READ;                /* Indicate we are reading this page */

#ifdef STATISTICS
        ++mp->pageread;
#endif

        /* Run through the user's filter.
           we use this fcn to read in the data chunk/page.
           Not the original intention. */
        if (mp->pgin != NULL) { /* Note page numbers in HMCPxxx are 0 based not 1 based */
            if (((mp->pgin)(mp->pgcookie, pgno - 1, bp->page)) == FAIL) {
                HEreport("mcache_get: error reading chunk=%d\n", (intn)pgno - 1);
                lp        = NULL; /* don't clobber the cache! */
                ret_value = RET_ERROR;
                goto done;
            }
        }
        else {
            HEreport("mcache_get: reading fcn not set,chunk=%d\n", (intn)pgno - 1);
            lp        = NULL;
            ret_value = RET_ERROR;
            goto done;
        }
    } /* end else list hit */

    /* Set the page number, pin the page. */
    bp->pgno  = pgno;
    bp->flags = MCACHE_PINNED;

    /*
     * Add the page to the head of the hash chain and the tail
     * of the lru chain.
     */
    head = &mp->hqh[HASHKEY(bp->pgno)];
    H4_CIRCLEQ_INSERT_HEAD(head, bp, hq);
    H4_CIRCLEQ_INSERT_TAIL(&mp->lqh, bp, q);

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        free(lp);
        return NULL;
    }
    return bp->page;
} /* mcache_get() */

/******************************************************************************
NAME
   mcache_put -- put a page back into the memory buffer pool

DESCRIPTION
    Return a page to the buffer pool. Unpin it and mark it
    appropriately i.e. MCACHE_DIRTY

RETURNS
    RET_SUCCESS if successful and RET_ERROR otherwise
******************************************************************************/
intn
mcache_put(MCACHE *mp,   /* IN: MCACHE cookie */
           void   *page, /* IN: page to put */
           int32   flags /* IN: flags = 0, MCACHE_DIRTY */)
{
    struct _lhqh *lhead     = NULL; /* head of an entry in list hash chain */
    L_ELEM       *lp        = NULL;
    BKT          *bp        = NULL; /* bucket element ptr */
    intn          ret_value = RET_SUCCESS;

    /* check inputs */
    if (mp == NULL || page == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

#ifdef STATISTICS
    ++mp->pageput;
#endif
    /* get pointer to bucket element */
    bp = (BKT *)((char *)page - sizeof(BKT));

    /* Unpin the page and mark it appropriately */
    bp->flags &= ~MCACHE_PINNED;
    bp->flags |= flags & MCACHE_DIRTY;

    if (bp->flags & MCACHE_DIRTY) { /* update this page reference */
        lhead = &mp->lhqh[HASHKEY(bp->pgno)];
        for (lp = lhead->cqh_first; lp != (void *)lhead; lp = lp->hl.cqe_next)
            if (lp->pgno == bp->pgno) { /* hit */
#ifdef STATISTICS
                ++mp->listhit;
                ++lp->elemhit;
#endif
                lp->eflags = ELEM_WRITTEN;
                break;
            } /* end if lp->pgno */
    }

done:
    return ret_value;
} /* mcache_put () */

/******************************************************************************
NAME
   mcache_close - close the memory buffer pool

DESCRIPTION
   Close the buffer pool.  Frees the buffer pool.
   Does not sync the buffer pool.

RETURNS
   RET_SUCCESS if successful and RET_ERROR otherwise
******************************************************************************/
intn
mcache_close(MCACHE *mp /* IN: MCACHE cookie */)
{
    L_ELEM *lp        = NULL;
    BKT    *bp        = NULL; /* bucket element */
    intn    nelem     = 0;
    intn    ret_value = RET_SUCCESS;
    intn    entry; /* index into hash table */

    /* check inputs */
    if (mp == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Free up any space allocated to the lru pages. */
    while ((bp = mp->lqh.cqh_first) != (void *)&mp->lqh) {
        H4_CIRCLEQ_REMOVE(&mp->lqh, mp->lqh.cqh_first, q);
        free(bp);
    }

    /* free up list elements */
    for (entry = 0; entry < HASHSIZE; ++entry) {
        while ((lp = mp->lhqh[entry].cqh_first) != (void *)&mp->lhqh[entry]) {
            H4_CIRCLEQ_REMOVE(&mp->lhqh[entry], mp->lhqh[entry].cqh_first, hl);
            free(lp);
            nelem++;
        }
    } /* end for entry */

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        return ret_value;
    }

    /* Free the MCACHE cookie. */
    free(mp);

    return ret_value;
} /* mcache_close() */

/******************************************************************************
NAME
   mcache_sync -- sync the memory buffer pool

DESCRIPTION
   Sync the pool to disk. Does NOT Free the buffer pool.

RETURNS
   RET_SUCCESS if successful and RET_ERROR otherwise
******************************************************************************/
intn
mcache_sync(MCACHE *mp /* IN: MCACHE cookie */)
{
    BKT *bp        = NULL; /* bucket element */
    intn ret_value = RET_SUCCESS;

    /* check inputs */
    if (mp == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Walk the lru chain, flushing any dirty pages to disk. */
    for (bp = mp->lqh.cqh_first; bp != (void *)&mp->lqh; bp = bp->q.cqe_next) {
        if (bp->flags & MCACHE_DIRTY && mcache_write(mp, bp) == RET_ERROR)
            HE_REPORT_GOTO("unable to flush a dirty page", FAIL);
    } /* end for bp */

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        return ret_value;
    }

    return ret_value;
} /* mcache_sync() */

/******************************************************************************
NAME
   mcache_bkt - Get a page from the cache (or create one).

DESCRIPTION
   Private routine. Get a page from the cache (or create one).

RETURNS
   A page if successful and NULL otherwise.

NOTE: Note that the size of the page allocated is equal to
      sizeof(bucket element) + pagesize. We only return the
      pagesize fragment to the user. The only caveat here is
      that a user could inadvertently clobber the bucket element
      information by writing out of the page size bounds.
******************************************************************************/
static BKT *
mcache_bkt(MCACHE *mp /* IN: MCACHE cookie */)
{
    struct _hqh *head      = NULL; /* head of hash chain */
    BKT         *bp        = NULL; /* bucket element */
    intn         ret_value = RET_SUCCESS;

    /* check inputs */
    if (mp == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* If under the max cached, always create a new page. */
    if ((int32)mp->curcache < (int32)mp->maxcache)
        goto new;

    /*
     * If the cache is max'd out, walk the lru list for a buffer we
     * can flush.  If we find one, write it (if necessary) and take it
     * off any lists.  If we don't find anything we grow the cache anyway.
     * The cache never shrinks.
     */
    for (bp = mp->lqh.cqh_first; bp != (void *)&mp->lqh; bp = bp->q.cqe_next)
        if (!(bp->flags & MCACHE_PINNED)) { /* Flush if dirty. */
            if (bp->flags & MCACHE_DIRTY && mcache_write(mp, bp) == RET_ERROR)
                HE_REPORT_GOTO("unable to flush a dirty page", FAIL);
#ifdef STATISTICS
            ++mp->pageflush;
#endif
            /* Remove from the hash and lru queues. */
            head = &mp->hqh[HASHKEY(bp->pgno)];
            H4_CIRCLEQ_REMOVE(head, bp, hq);
            H4_CIRCLEQ_REMOVE(&mp->lqh, bp, q);

            /* done */
            ret_value = RET_SUCCESS;
            goto done;
        } /* end if bp->flags */

    /* create a new page */
    new : if ((bp = (BKT *)malloc(sizeof(BKT) + (uintn)mp->pagesize)) == NULL) HGOTO_ERROR(DFE_NOSPACE, FAIL);

#ifdef STATISTICS
    ++mp->pagealloc;
#endif

    /* set page ptr past bucket element section */
    bp->page = (char *)bp + sizeof(BKT);
    ++mp->curcache; /* increase number of cached pages */

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        free(bp);
        return NULL;
    }

    return bp; /* return only the pagesize fragment */
} /* mcache_bkt() */

/******************************************************************************
NAME
   mcache_write - write a page to disk given it's bucket handle.

DESCRIPTION
   Private routine. Write a page to disk given it's bucket handle.

RETURNS
   RET_SUCCESS if successful and RET_ERROR otherwise
******************************************************************************/
static int
mcache_write(MCACHE *mp, /* IN: MCACHE cookie */
             BKT    *bp /* IN: bucket element */)
{
    struct _lhqh *lhead     = NULL; /* head of an entry in list hash chain */
    L_ELEM       *lp        = NULL;
    intn          ret_value = RET_SUCCESS;

    /* check inputs */
    if (mp == NULL || bp == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

#ifdef STATISTICS
    ++mp->pagewrite;
#endif

    /* update this page reference */
    lhead = &mp->lhqh[HASHKEY(bp->pgno)];
    for (lp = lhead->cqh_first; lp != (void *)lhead; lp = lp->hl.cqe_next)
        if (lp->pgno == bp->pgno) { /* hit */
#ifdef STATISTICS
            ++mp->listhit;
            ++lp->elemhit;
#endif
            lp->eflags = ELEM_SYNC;
            break;
        }

    /* Run page through the user's filter.
       we use this to write the data chunk/page out.
       This deviates from the original purpose of the filter. */
    if (mp->pgout) { /* Note page numbers in HMCPxxx are 0 based not 1 based */
        if (((mp->pgout)(mp->pgcookie, bp->pgno - 1, bp->page)) == FAIL) {
            HEreport("mcache_write: error writing chunk=%d\n", (intn)bp->pgno);
            ret_value = RET_ERROR;
            goto done;
        }
    }
    else {
        HEreport("mcache_write: writing fcn not set,chunk=%d\n", (intn)bp->pgno);
        ret_value = RET_ERROR;
        goto done;
    }

    /* mark page as clean */
    bp->flags &= ~MCACHE_DIRTY;

done:
    if (ret_value == RET_ERROR) { /* error cleanup */
        return ret_value;
    }

    return ret_value;
} /* mcache_write() */

/******************************************************************************
NAME
   mcache_look - lookup a page in the cache.

DESCRIPTION
   Private routine. Lookup a page in the cache and return pointer to it.

RETURNS
   Page if successful and NULL otherwise.
******************************************************************************/
static BKT *
mcache_look(MCACHE *mp, /* IN: MCACHE cookie */
            int32   pgno /* IN: page to look up in cache */)
{
    struct _hqh *head = NULL; /* head of hash chain */
    BKT         *bp   = NULL; /* bucket element */

    /* check inputs */
    if (mp == NULL) {
        HERROR(DFE_ARGS);
        goto done;
    }

    /* Check for attempt to look up a non-existent page. */
    if (pgno > mp->npages) {
        HEreport("attempting to get a nonexistent page from cache");
        goto done;
    }

    /* search through hash chain */
    head = &mp->hqh[HASHKEY(pgno)];
    for (bp = head->cqh_first; bp != (void *)head; bp = bp->hq.cqe_next)
        if (bp->pgno == pgno) { /* hit....found page in cache */
#ifdef STATISTICS
            ++mp->cachehit;
#endif
            /* done */
            goto done;
        }

    /* Well didn't find page in cache so mark return
     * value as NULL */
    bp = NULL;

#ifdef STATISTICS
    ++mp->cachemiss;
#endif
done:
    return bp;
} /* mcache_look() */

#ifdef STATISTICS
#ifdef H4_HAVE_GETRUSAGE

#include <sys/resource.h>

/******************************************************************************
NAME
   myrusage - print some process usage statistics

DESCRIPTION
   prints some process usage statistics to STDERR

RETURNS
   Nothing
******************************************************************************/
void
myrusage()
{
    struct rusage r;
    double        sys, user, idle;
    double        per;
    double        timespent();

    getrusage(RUSAGE_SELF, &r);
    fprintf(stderr, "USAGE: shmem=%ld, unshdata=%ld, unshstack=%ld\n", r.ru_ixrss, r.ru_idrss, r.ru_isrss);
    fprintf(stderr, "       pager=%ld, pagef=%ld, nswap=%ld\n", r.ru_minflt, r.ru_majflt, r.ru_nswap);
    fprintf(stderr, "       block_in=%ld, block_out=%ld\n", r.ru_inblock, r.ru_oublock);
    fprintf(stderr, "       mesgs=%ld, mesgr=%ld, nsignals=%ld\n", r.ru_msgsnd, r.ru_msgrcv, r.ru_nsignals);
}
#endif /* H4_HAVE_GETRUSAGE */

/******************************************************************************
NAME
   mcache_stat - print out cache statistics

DESCRIPTION
   Print out cache statistics to STDERR.

RETURNS
   Nothing
******************************************************************************/
void
mcache_stat(MCACHE *mp /* IN: MCACHE cookie */)
{
    struct _lhqh *lhead = NULL; /* head of an entry in list hash chain */
    BKT          *bp    = NULL; /* bucket element */
    L_ELEM       *lp    = NULL;
    char         *sep   = NULL;
    intn          entry; /* index into hash table */
    intn          cnt;
    intn          hitcnt;

#ifdef H4_HAVE_GETRUSAGE
    myrusage();
#endif

    /* check inputs */
    if (mp != NULL) {
        fprintf(stderr, "%u pages in the object\n", mp->npages);
        fprintf(stderr, "page size %u, caching %u pages of %u page max cache\n", mp->pagesize, mp->curcache,
                mp->maxcache);
        fprintf(stderr, "%u page puts, %u page gets, %u page new\n", mp->pageput, mp->pageget, mp->pagenew);
        fprintf(stderr, "%u page allocs, %u page flushes\n", mp->pagealloc, mp->pageflush);
        if (mp->cachehit + mp->cachemiss)
            fprintf(stderr, "%.0f%% cache hit rate (%u hits, %u misses)\n",
                    ((double)mp->cachehit / (mp->cachehit + mp->cachemiss)) * 100, mp->cachehit,
                    mp->cachemiss);
        fprintf(stderr, "%u page reads, %u page writes\n", mp->pageread, mp->pagewrite);
        fprintf(stderr, "%u listhits, %u listallocs\n", mp->listhit, mp->listalloc);
        fprintf(stderr, "sizeof(MCACHE)=%zu, sizeof(BKT)=%zu, sizeof(L_ELEM)=%zu\n", sizeof(MCACHE),
                sizeof(BKT), sizeof(L_ELEM));
        fprintf(stderr, "memory pool used %u bytes\n",
                (int32)(sizeof(MCACHE) + (sizeof(BKT) + mp->pagesize) * mp->curcache +
                        (sizeof(L_ELEM) * mp->npages)));
        sep = "";
        cnt = 0;
        for (bp = mp->lqh.cqh_first; bp != (void *)&mp->lqh; bp = bp->q.cqe_next) {
            fprintf(stderr, "%s%u", sep, bp->pgno);
            if (bp->flags & MCACHE_DIRTY)
                fprintf(stderr, "d");
            if (bp->flags & MCACHE_PINNED)
                fprintf(stderr, "P");
            if (++cnt == 10) {
                sep = "\n";
                cnt = 0;
            }
            else
                sep = ", ";
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "Element hits\n");
        sep    = "";
        cnt    = 0;
        hitcnt = 0;
        for (entry = 0; entry < HASHSIZE; ++entry) {
            lhead = &mp->lhqh[entry];
            for (lp = lhead->cqh_first; lp != (void *)lhead; lp = lp->hl.cqe_next) {
                cnt++;
                fprintf(stderr, "%s%u(%u)", sep, lp->pgno, lp->elemhit);
                hitcnt += lp->elemhit;
                if (cnt >= 8) {
                    sep = "\n";
                    cnt = 0;
                }
                else
                    sep = ", ";
            }
            if (cnt >= 8) {
                fprintf(stderr, "\n");
                cnt = 0;
            }
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "Total num of elemhits=%d\n", hitcnt);
    } /* end if mp */
}
#endif /* STATISTICS */
