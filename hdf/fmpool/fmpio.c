/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include <errno.h>
#include <string.h>
#include "fmpio.h"

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
MPFILE * 
MPopen(const char * path, int flags)
{
  MPFILE *mpfs = NULL; /* File struct */
  int   ret    = SUCCEED;

  if (path == NULL)
    {
      ret = FAIL;
      goto done;
    }

  /* allocate space for file struct */
  if ((mpfs = (MPFILE *)calloc(1,sizeof(MPFILE))) == NULL)
    {
      ret = FAIL;
      goto done;
    }

  /* open file */
  switch(flags)
    {
    case DFACC_CREATE:
      if ((mpfs->fd = FMPI_CREATE(path)) == FMPI_FAIL)
        {
          ret = FAIL;
          goto done;
        } 
      break;
    case DFACC_READ:
    case DFACC_WRITE:
    case DFACC_RDWR:
    case DFACC_ALL:
    default:
      if ((mpfs->fd = FMPI_OPEN(path,flags)) == FMPI_FAIL)
        {
          ret = FAIL;
          goto done;
        } 
      break;
    }

  mpfs->curp = 0;

  /* create private memory pool for file 
  * currently we are sharing the pool*/
  if ((mpfs->mp = mpool_open(NULL,mpfs->fd,0,0)) == NULL)
    {
      ret = FAIL;
      goto done;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPopen: mp->npages =%d\n",mpfs->mp->npages);
#endif

done:
  if(ret == FAIL)
    { /* error cleanup */
      if (mpfs != NULL)
        free(mpfs);
      
      mpfs = NULL; /* return value */
    }
  /* Normal cleanup */

  return mpfs;
} /* MPopen() */

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
int
MPclose(MPFILE *mpfs)
{
  int ret = SUCCEED;

  if (mpfs == NULL)
    {
      ret = FAIL;
      goto done;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPclose: sync the file\n");
  fprintf(stderr,"MPclose: mp->npages =%d\n",mpfs->mp->npages);
#endif
  /* sync pages and then close mpool*/
  if (mpool_sync(mpfs->mp) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

#ifdef STATISTICS
  mpool_stat(mpfs->mp);
#endif 
  if (mpool_close(mpfs->mp) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

   FMPI_CLOSE(mpfs->fd);

done:
  if(ret == FAIL)
    { /* error cleanup */
            
    }
  /* Normal cleanup */
  /* free file struct */
  free(mpfs);
  return ret;
} /* MPclose() */

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
int
MPflush(MPFILE *mpfs)
{
  int ret = SUCCEED;

  if (mpfs == NULL)
    {
      ret = FAIL;
      goto done;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPflush: sync the file\n");
  fprintf(stderr,"MPflush: mp->npages =%d\n",mpfs->mp->npages);
#endif
  /* sync pages */
  if (mpool_sync(mpfs->mp) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

done:
  if(ret == FAIL)
    { /* error cleanup */
            
    }
  /* Normal cleanup */

  return ret;
} /* MPflush() */

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
int 
MPseek(MPFILE *mpfs, off_t offset, int whence )
{
  pageno_t     new_pgno = 0;
  pageno_t     pagesize = 0;
  pageno_t     oddpagesize = 0;
  pageno_t     npages = 0;
  off_t      cur_off = 0;
  u_int32_t  flags = 0;
  void       *mypage = NULL;
  int        ret = SUCCEED;

  if (mpfs == NULL || offset < 0)
    {
      ret = FAIL;
      goto done;
    }

  pagesize = mpfs->mp->pagesize;
  npages   = mpfs->mp->npages;

  /* Adjust offset deepending on seek flag */
  switch( whence )
    {
    case 0:  /* SEEK_SET */
      cur_off = offset;
      break ;
    case 1: /* SEEK_CUR */
      cur_off = offset + mpfs->foff; /* add current offset in file */
      break ;
    case 2: /* SEEK_END */
      cur_off = pagesize * npages; 
      /* Adjust for odd size last page */
      if (pagesize != mpfs->mp->lastpagesize)
        cur_off -= (pagesize - mpfs->mp->lastpagesize);

      cur_off += offset; /* add offset from end of file */
      break ;
    }
#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: seeking to cur_off =%d\n",cur_off);
  fprintf(stderr,"MPseek: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPseek: mp->npages=%u, mp->pagesize=%u\n",
         mpfs->mp->npages,mpfs->mp->pagesize);
#endif
  /* calculate which page number this offset refers to */
  new_pgno = (pageno_t)(cur_off / pagesize);
  new_pgno++;
  oddpagesize = (pageno_t)(cur_off % pagesize);
  if (!oddpagesize && new_pgno != 1)
    { /* we are even multiple of page sizes */
      oddpagesize = pagesize;
      new_pgno--;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: this offset corresponds to new_pgno =%u\n",new_pgno);
#endif

  /* Check to see if this page is the current page */
  if (mpfs->curp != 0 && new_pgno == mpfs->curp)
    {
      ret = SUCCEED;
      goto skip_sget; /* we don't need to get it */
    }

  /* Check to see if page exists */
#ifndef USE_INLINE
  if ((mypage = mpool_get(mpfs->mp, new_pgno, 0)) == NULL)
    { /* need to extend file */
#ifdef MP_DEBUG
      fprintf(stderr,"MPseek: page =%u does not exist\n",new_pgno);
#endif
      if ((mypage = mpool_new(mpfs->mp, &new_pgno, oddpagesize, MPOOL_EXTEND)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      flags = MPOOL_DIRTY; /* mark page as dirty */
    }
  else
    flags = 0;
#else
  FMPOOL_GET(mpfs->mp, new_pgno, flags, mypage, get1, STATISTICS, MPOOL_DEBUG);
  if (mypage == NULL)
    { /* need to extend file */
#ifdef MP_DEBUG
      fprintf(stderr,"MPseek: page =%d does not exist\n",new_pgno);
#endif
      if ((mypage = mpool_new(mpfs->mp, &new_pgno, oddpagesize, MPOOL_EXTEND)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      flags = MPOOL_DIRTY; /* mark page as dirty */
    }
  else
    flags = 0;
#endif

#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: put page back \n");
#endif

#ifndef USE_INLINE
  if (mpool_put(mpfs->mp, mypage, flags) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#else /* USE_INLINE */
  FMPOOL_PUT(mpfs->mp, mypage, flags, ret, put1, STATISTICS, MPOOL_DEBUG);
  if (ret == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#endif /* USE_INLINE */

skip_sget:
  mpfs->curp = new_pgno;
  if (cur_off != 0)
    mpfs->poff = oddpagesize; /* offset into current page */
  else /* else we are 1st page */
    mpfs->poff = 0;

  mpfs->foff = cur_off;

  /* Is is this the last page? */
  if (new_pgno == mpfs->mp->npages)
    {
      mpfs->mp->lastpagesize = oddpagesize; /* set last page size */
#ifdef NEED_SYNC_LASTPAGE
#ifdef MP_DEBUG
      fprintf(stderr,"MPseek last page, need to sync: mpfs->mp->lastpagesize =%u\n",mpfs->mp->lastpagesize);
#endif
      if (mpool_page_sync(mpfs->mp, new_pgno,0) == RET_ERROR) /* sync last page */
        {
          ret = FAIL;
          goto done;
        }
#endif
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPseek: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPseek: mpfs->poff =%d\n",mpfs->poff);
#endif
done:
  if(ret == FAIL)
    { /* error cleanup */
      offset = FAIL;
    }
  /* Normal cleanup */

  return offset;
} /* MPseek() */

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
int 
MPread(MPFILE *mpfs, void *buf, size_t nbytes )
{
  size_t nr = 0;
  size_t nbr = 0;
  size_t nbl = 0;
  pageno_t pageno = 0;
  pageno_t     pagesize = 0;
  pageno_t     npages = 0;
  pageno_t     oddpagesize = 0;
  off_t      cur_off = 0;
  void *mypage = NULL;
  void *cptr = NULL;
  void *bptr = buf;
  int   ret = SUCCEED;
#ifdef USE_INLINE
  int   pflags = 0;
#endif

  if (mpfs == NULL || buf == NULL)
    {
      ret = FAIL;
      goto done;
    }

  pagesize = mpfs->mp->pagesize;
  npages   = mpfs->mp->npages;

  /* calculate bytes left, number of pages*/
  nbl = nbytes;
  cur_off = (off_t)(mpfs->foff + nbytes);
  pageno = (pageno_t)(cur_off / pagesize);
  pageno++;
  oddpagesize = (pageno_t)(cur_off % pagesize);
  if (!oddpagesize && pageno != 1)
    { /* we are even multiple of page sizes */
      oddpagesize = pagesize;
      pageno--;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"ENTER->MPread: pageno =%u\n",pageno);
  fprintf(stderr,"MPread: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPread: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPread: mpfs->poff =%d\n",mpfs->poff);
#endif 
  /* copy First page */
#ifndef USE_INLINE
  if ((mypage = mpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
    {
      ret = FAIL;
      goto done;
    }
#else
  FMPOOL_GET(mpfs->mp, mpfs->curp, flags, mypage, get1, STATISTICS, MPOOL_DEBUG);
  if (mypage == NULL)
    {
      ret = FAIL;
      goto done;
    }
#endif

  cptr = (char *)mypage + mpfs->poff; /* adjust into current page */
#if 0
  cptr += mpfs->poff; 
#endif

  /* set number of bytes read */
  if (nbl > (pagesize - mpfs->poff))
    nbr = pagesize - mpfs->poff; 
  else
    nbr = nbl;

  memcpy(bptr,cptr,nbr); /* copy from first page */
  mpfs->poff = (off_t)((mpfs->poff + nbr) % pagesize);

  /* return page */
#ifndef USE_INLINE
  if (mpool_put(mpfs->mp, mypage, 0) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#else /* USE_INLINE */
  FMPOOL_PUT(mpfs->mp, mypage, pflags, ret, put1, STATISTICS, MPOOL_DEBUG);
  if (ret == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#endif /* USE_INLINE */

  bptr = (char *)bptr + nbr; /* increment buffer ptr */
  nbl -= nbr;  /* decrement bytes left to read */
  nr += nbr;

#ifdef MP_DEBUG
  fprintf(stderr,"MPread: %u pages to process\n",pageno - mpfs->curp);
#endif
  /* deal with all other pages */
  while (pageno && (pageno - mpfs->curp))
    {
      mpfs->curp++; /* next page */
#ifndef USE_INLINE
      if ((mypage = mpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
#else
  FMPOOL_GET(mpfs->mp, mpfs->curp, flags, mypage, get2, STATISTICS, MPOOL_DEBUG);
  if (mypage == NULL)
    {
      ret = FAIL;
      goto done;
    }
#endif
      if (nbl > pagesize)
        nbr = pagesize;
      else
        nbr = nbl;
      memcpy(bptr,mypage,nbr); /* copy first page */
      mpfs->poff = (off_t)(nbr % pagesize);
      /* return page */
  /* return page */
#ifndef USE_INLINE
      if (mpool_put(mpfs->mp, mypage, 0) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
#else /* USE_INLINE */
      FMPOOL_PUT(mpfs->mp, mypage, pflags, ret, put2, STATISTICS, MPOOL_DEBUG);
      if (ret == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
#endif /* USE_INLINE */
      bptr = (char *)bptr + nbr; /* increment buffer ptr */
      nbl -= nbr;  /* decrement bytes left to read */
      nr += nbr;
    }

  /* Is is this the last page? */
  if (mpfs->curp == mpfs->mp->npages)
    {
      if (mpfs->poff)
        mpfs->mp->lastpagesize = mpfs->poff; /* set last page size */
      else
        mpfs->mp->lastpagesize = mpfs->mp->pagesize; /* set last page size */
#ifdef MP_DEBUG
    fprintf(stderr,"EXIT->MPread: lastpagesize=%u \n",mpfs->mp->lastpagesize);
#endif
    }

  mpfs->foff += nr; /* set file offset */

done:
  if(ret == FAIL)
    { /* error cleanup */
#ifdef MP_DEBUG
  fprintf(stderr,"EXIT->MPread: FAILING\n");
#endif
      nr = FAIL;
    }
  /* Normal cleanup */
#ifdef MP_DEBUG
  fprintf(stderr,"EXIT->MPread: read %d bytes\n",nr);
  fprintf(stderr,"EXIT->MPread: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"EXIT->MPread: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"EXIT->MPread: mpfs->poff =%d\n\n",mpfs->poff);
#endif 
  return nr;
} /* MPread() */

/*-----------------------------------------------------------------------------
NAME

USAGE

RETURNS

DESCRIPTION

---------------------------------------------------------------------------- */
int 
MPwrite(MPFILE *mpfs, void *buf, size_t nbytes )
{
  size_t nw = 0;
  size_t nbw = 0;
  size_t nbl = 0;
  pageno_t npagno = 0;
  pageno_t pageno = 0;
  pageno_t new_pgno = 0;
  pageno_t     pagesize = 0;
  pageno_t     npages = 0;
  pageno_t     oddpagesize = 0;
  off_t      cur_off = 0;
  void *mypage = NULL;
  void *cptr = NULL;
  void *bptr = buf;
  int   ret = SUCCEED;
#ifdef USE_INLINE
  int   pflags = 0;
#endif

  if (mpfs == NULL)
    {
      ret = FAIL;
      goto done;
    }

  pagesize = mpfs->mp->pagesize;
  npages   = mpfs->mp->npages;

  /* calculate bytes left, number of pages*/
  nbl = nbytes;
  cur_off = (off_t)(mpfs->foff + nbytes);
  pageno = (pageno_t)(mpfs->foff / pagesize);
  pageno++;
  npagno = (pageno_t)(cur_off / pagesize);
  npagno++;
  oddpagesize = (pageno_t)(cur_off % pagesize);
  if (!oddpagesize && npagno != 1)
    { /* we are even multiple of page sizes */
      oddpagesize = pagesize;
      npagno--;
    }
  
#ifdef MP_DEBUG
  fprintf(stderr,"Enter->MPwrite: nbytes =%d, mp->pagesize=%u, mp->npages=%u\n",
         nbytes,mpfs->mp->pagesize,mpfs->mp->npages);
  fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPwrite: mpfs->poff =%d\n",mpfs->poff);
  fprintf(stderr,"MPwrite: getting pageno =%u, npagno=%u\n",pageno, npagno);
#endif

  /* get First page */
#ifndef USE_INLINE
  if ((mypage = mpool_get(mpfs->mp, pageno, 0)) == NULL)
    {
      if ((mypage = mpool_new(mpfs->mp, &new_pgno, mpfs->mp->pagesize, 0)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      mpfs->curp = new_pgno;
      mpfs->poff = 0;
    }
  else
    mpfs->curp = pageno;
#else
  FMPOOL_GET(mpfs->mp, pageno, flags, mypage, get1, STATISTICS, MPOOL_DEBUG);
  if (mypage == NULL)
    {
      if ((mypage = mpool_new(mpfs->mp, &new_pgno, mpfs->mp->pagesize, 0)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      mpfs->curp = new_pgno;
      mpfs->poff = 0;
    }
  else
    mpfs->curp = pageno;
#endif
#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPwrite: mpfs->poff =%d\n",mpfs->poff);
#endif

  cptr = (char *)mypage + mpfs->poff; /* adjust into current page */

  /* set number of bytes to write */
  if (nbl > (pagesize - mpfs->poff))
    nbw = (pagesize - mpfs->poff); 
  else
    nbw = nbl;

  memcpy(cptr,bptr,nbw); /* copy into first page */
  mpfs->poff = (off_t)((mpfs->poff + nbw) % pagesize);

#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: copied %d bytes\n",nbw);
  fprintf(stderr,"MPwrite: mark dirty page=%u\n",mpfs->curp);
#endif
  /* Mark page as dirty */
#ifndef USE_INLINE
  if (mpool_put(mpfs->mp, mypage, MPOOL_DIRTY) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#else /* USE_INLINE */
  pflags = MPOOL_DIRTY;
  FMPOOL_PUT(mpfs->mp, mypage, pflags, ret, put1, STATISTICS, MPOOL_DEBUG);
  if (ret == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }
#endif /* USE_INLINE */

  bptr = (char *)bptr + nbw; /* increment buffer ptr */
  nbl -= nbw;  /* decrement bytes left to write */
  nw += nbw;

  /* deal with all other pages */
#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: %u pages to process\n",npagno - mpfs->curp);
#endif
  while (npagno && (npagno - mpfs->curp))
    {
      mpfs->curp++; /* next page */
#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: processing page %u \n",mpfs->curp);
#endif
#ifndef USE_INLINE
      if ((mypage = mpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
        { /* Is the page we requested still less than total number of pages */
          if (mpfs->curp <= mpfs->mp->npages )
            {
#ifdef MP_DEBUG
              fprintf(stderr,"EXIT_ERROR->MPwrite: wrote %d bytes\n",nw);
              fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
              fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
              fprintf(stderr,"MPwrite: mpfs->poff =%d\n\n",mpfs->poff);
#endif
              ret = FAIL;
              goto done;
            }
          else
            {
              if ((mypage = mpool_new(mpfs->mp, &new_pgno, pagesize, 0)) 
                  == NULL)
                {
                  ret = FAIL;
                  goto done;
                }
              mpfs->curp = new_pgno;
              mpfs->poff = 0;
            }
        }
#else
      FMPOOL_GET(mpfs->mp, mpfs->curp, flags, mypage, get2, STATISTICS, MPOOL_DEBUG);
      if (mypage == NULL)
        {
          if (mpfs->curp <= mpfs->mp->npages )
            {
#ifdef MP_DEBUG
              fprintf(stderr,"EXIT_ERROR->MPwrite: wrote %d bytes\n",nw);
              fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
              fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
              fprintf(stderr,"MPwrite: mpfs->poff =%d\n\n",mpfs->poff);
#endif
              ret = FAIL;
              goto done;
            }
          else
            {
              if ((mypage = mpool_new(mpfs->mp, &new_pgno, pagesize, 0)) 
                  == NULL)
                {
                  ret = FAIL;
                  goto done;
                }
              mpfs->curp = new_pgno;
              mpfs->poff = 0;
            }
        }
#endif
      if (nbl > pagesize)
        nbw = pagesize;
      else
        nbw = nbl;
      memcpy(mypage,bptr,nbw); /* copy page */
      mpfs->poff = (off_t)(nbw % pagesize);
#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: copied %d bytes\n",nbw);
  fprintf(stderr,"MPwrite: mark dirty page=%u\n",mpfs->curp);
#endif
      /* Mark page as dirty */
#ifndef USE_INLINE
      if (mpool_put(mpfs->mp, mypage, MPOOL_DIRTY) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
#else /* USE_INLINE */
      pflags = MPOOL_DIRTY;
      FMPOOL_PUT(mpfs->mp, mypage, pflags, ret, put2, STATISTICS, MPOOL_DEBUG);
      if (ret == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
#endif /* USE_INLINE */

      bptr = (char *)bptr + nbw; /* increment buffer ptr */
      nbl -= nbw;  /* decrement bytes left to write */
      nw += nbw;
    }

  /* Is is this the last page? */
  if (mpfs->curp == mpfs->mp->npages)
    {
      if (mpfs->poff)
        mpfs->mp->lastpagesize = mpfs->poff; /* set last page size */
      else
        mpfs->mp->lastpagesize = pagesize; /* set last page size */
#ifdef MP_DEBUG
    fprintf(stderr,"EXIT->MPwrite: lastpagesize=%u \n",mpfs->mp->lastpagesize);
#endif
    }

  mpfs->foff += nw; /* set file offset */
#ifdef MP_DEBUG
  fprintf(stderr,"EXIT->MPwrite: wrote %d bytes\n",nw);
  fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPwrite: mpfs->poff =%d\n\n",mpfs->poff);
#endif
done:
  if(ret == FAIL)
    { /* error cleanup */
      nw = FAIL;
    }
  /* Normal cleanup */

  return nw;
} /* MPwrite() */
