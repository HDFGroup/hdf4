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

  mpfs->curp = 0; /* set current page to none */
  mpfs->curpr = 0;
  mpfs->poff = 0;
  mpfs->foff = 0;
  mpfs->rpage = NULL;

  /* create private memory pool for file 
  * currently we are sharing the pool*/
  if ((mpfs->mp = fmpool_open(NULL,mpfs->fd,0,0)) == NULL)
    {
      ret = FAIL;
      goto done;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"MPopen: mp->npages =%d\n",fmpool_get_npages(mpfs->mp));
  fprintf(stderr,"MPopen: mp->pagesize =%d\n",fmpool_get_pagesize(mpfs->mp));
  fprintf(stderr,"MPopen: mp->lastpagesize=%d\n",fmpool_get_lastpagesize(mpfs->mp));
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
  fprintf(stderr,"MPclose: mp->npages =%d\n",fmpool_get_npages(mpfs->mp));
  fprintf(stderr,"MPclose: mp->lastpagesize =%d\n",fmpool_get_lastpagesize(mpfs->mp));
#endif
  /* sync pages and then close mpool*/
  if (fmpool_sync(mpfs->mp) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

#ifdef STATISTICS
  fmpool_stat(mpfs->mp);
#endif 
  if (fmpool_close(mpfs->mp) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

  /* Close the file */
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
  fprintf(stderr,"MPflush: mp->npages =%d\n",fmpool_get_npages(mpfs->mp));
  fprintf(stderr,"MPflush: mp->lastpagesize =%d\n",fmpool_get_lastpagesize(mpfs->mp));
#endif
  /* sync pages */
  if (fmpool_sync(mpfs->mp) == RET_ERROR)
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
  pageno_t   new_pgno = 0;
  pageno_t   pagesize = 0;
  pageno_t   oddpagesize = 0;
  pageno_t   lastpagesize = 0;
  pageno_t   npages = 0;
  off_t      cur_off = 0;
  u_int32_t  flags = 0;
  void       *mypage = NULL;
  int        ret = SUCCEED;

  if (mpfs == NULL || offset < 0)
    {
      ret = FAIL;
      goto done;
    }
#ifdef MP_DEBUG
  fprintf(stderr,"ENTER->MPseek: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"ENTER->MPseek: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"ENTER->MPseek: mpfs->poff =%d\n",mpfs->poff);
#endif
  pagesize = fmpool_get_pagesize(mpfs->mp);
  npages   = fmpool_get_npages(mpfs->mp);
  lastpagesize = fmpool_get_lastpagesize(mpfs->mp);

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
      if (pagesize != lastpagesize)
        cur_off -= (pagesize - lastpagesize);

      cur_off += offset; /* add offset from end of file */
      break ;
    }
#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: seeking to cur_off =%d\n",cur_off);
  fprintf(stderr,"MPseek: mp->npages=%u, mp->pagesize=%u\n",
          npages,pagesize);
#endif
  /* calculate which page number this offset refers to */
  new_pgno = (cur_off / pagesize);
  new_pgno++;
  oddpagesize = (cur_off % pagesize);
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
  if ((mypage = fmpool_get(mpfs->mp, new_pgno, 0)) == NULL)
    { /* need to extend file and set lastpagesize*/
#ifdef MP_DEBUG
      fprintf(stderr,"MPseek: page =%u does not exist\n",new_pgno);
      fprintf(stderr,"MPseek: oddpagesize=%u \n",oddpagesize);
#endif
      if ((mypage = fmpool_new(mpfs->mp, &new_pgno, oddpagesize, MPOOL_EXTEND)) 
          == NULL)
        {
          ret = FAIL;
          goto done;
        }
      flags = MPOOL_DIRTY; /* mark page as dirty */
    }
  else
    flags = 0;

#ifdef MP_DEBUG
  fprintf(stderr,"MPseek: put page back \n");
#endif

  /* put page back */
  if (fmpool_put(mpfs->mp, mypage, flags) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

  skip_sget:
  mpfs->curp = new_pgno;    /* current page */
  mpfs->poff = oddpagesize; /* offset into current page */
  mpfs->foff = cur_off;     /* file offset */

  /* Is is this the last page? */
  if (new_pgno == fmpool_get_npages(mpfs->mp))
    { /* set last page size */
      if (mpfs->poff && (mpfs->poff > fmpool_get_lastpagesize(mpfs->mp)))
        {
          if (fmpool_set_lastpagesize(mpfs->mp,oddpagesize) == RET_ERROR)
            {
              ret = FAIL;
              goto done;
            }
#ifdef MP_DEBUG
          fprintf(stderr,"MPseek last page now,  mpfs->mp->lastpagesize =%u\n",
                  fmpool_get_lastpagesize(mpfs->mp));
#endif
        }
    }

  done:
  if(ret == FAIL)
    { /* error cleanup */
      offset = FAIL;
#ifdef MP_DEBUG
      fprintf(stderr,"EXIT->Error with MPseek: \n");
#endif
    }
  /* Normal cleanup */
#ifdef MP_DEBUG
  fprintf(stderr,"EXIT->MPseek: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"EXIT->MPseek: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"EXIT->MPseek: mpfs->poff =%d\n",mpfs->poff);
#endif
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
  pageno_t npageno = 0;
  pageno_t pagesize = 0;
  pageno_t npages = 0;
  pageno_t oddpagesize = 0;
  off_t    end_off = 0;
  void *mypage = NULL;
  void *cptr = NULL;
  void *bptr = buf;
  int   ret = SUCCEED;
  int   skip_first_put = 0;

  if (mpfs == NULL || buf == NULL)
    {
      ret = FAIL;
      goto done;
    }

  pagesize = fmpool_get_pagesize(mpfs->mp);
  npages   = fmpool_get_npages(mpfs->mp);

  /* calculate bytes left, number of pages*/
  nbl = nbytes;
  end_off = (off_t)(mpfs->foff + nbytes);
  npageno = (pageno_t)(end_off / pagesize);
  npageno++;
  oddpagesize = (pageno_t)(end_off % pagesize);
  if (!oddpagesize && npageno != 1)
    { /* we are even multiple of page sizes */
      oddpagesize = pagesize;
      npageno--;
    }

#ifdef MP_DEBUG
  fprintf(stderr,"ENTER->MPread: npageno =%u\n",npageno);
  fprintf(stderr,"ENTER->MPread: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"ENTER->MPread: mpfs->foff =%u\n",mpfs->foff);
  fprintf(stderr,"ETNER->MPread: mpfs->poff =%u\n",mpfs->poff);
#endif 

  /* Check to see if this page is the last page read */
  if (mpfs->curpr != 0 && mpfs->curp == mpfs->curpr )
    {
      mypage = mpfs->rpage;
      skip_first_put = 1;
      goto skip_rget; /* we don't need to get it */
    }

  /* copy First page */
  if ((mypage = fmpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
    {
      ret = FAIL;
      goto done;
    }

skip_rget:
  cptr = (char *)mypage + mpfs->poff; /* adjust into current page */

  /* set number of bytes read */
  if (nbl > (pagesize - mpfs->poff))
    nbr = pagesize - mpfs->poff; 
  else
    nbr = nbl;

  memcpy(bptr,cptr,nbr); /* copy from first page */
  mpfs->poff = (off_t)(mpfs->poff + nbr);

#ifdef MP_DEBUG
  fprintf(stderr,"MPread: read %d bytes, mpfs->poff =%u\n",nbr,mpfs->poff);
#endif

  /* return page */
  if (!skip_first_put)
    {
      if (fmpool_put(mpfs->mp, mypage, 0) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
    }
  bptr = (char *)bptr + nbr; /* increment buffer ptr */
  nbl -= nbr;  /* decrement bytes left to read */
  nr += nbr;

#ifdef MP_DEBUG
  fprintf(stderr,"MPread: %u pages to process\n",npageno - mpfs->curp);
#endif
  /* deal with all other pages */
  while (npageno && (npageno - mpfs->curp))
    {
      mpfs->curp++; /* next page */
      if ((mypage = fmpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      if (nbl > pagesize)
        nbr = pagesize;
      else
        nbr = nbl;
      memcpy(bptr,mypage,nbr); /* copy first page */
      mpfs->poff = nbr;
      /* return page */
      if (fmpool_put(mpfs->mp, mypage, 0) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
      bptr = (char *)bptr + nbr; /* increment buffer ptr */
      nbl -= nbr;  /* decrement bytes left to read */
      nr += nbr;
    } /* end while */

  mpfs->foff += nr; /* set file offset */

  /* point last page read */
  if (mpfs->curp != mpfs->curpr )
   {
     mpfs->rpage = mypage;
     mpfs->curpr = mpfs->curp; 
   }

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
  pageno_t pagesize = 0;
  pageno_t npages = 0;
  pageno_t oddpagesize = 0;
  off_t    cur_off = 0;
  void *mypage = NULL;
  void *cptr = NULL;
  void *bptr = buf;
  int   ret = SUCCEED;

  if (mpfs == NULL)
    {
      ret = FAIL;
      goto done;
    }

  pagesize = fmpool_get_pagesize(mpfs->mp);
  npages   = fmpool_get_npages(mpfs->mp);

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
          nbytes,pagesize,npages);
  fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPwrite: mpfs->poff =%d\n",mpfs->poff);
  fprintf(stderr,"MPwrite: getting pageno =%u, npagno=%u\n",pageno, npagno);
#endif

  /* Check if this was the last page read */
  if (mpfs->curpr >= pageno &&  mpfs->curpr <= npagno)
     mpfs->curpr = 0; /* reset last page read to invalid */

  /* Check to see if this is the current page */
  if (mpfs->curp != pageno)
    mpfs->poff = 0; /* reset page offset since not current page */

  /* get First page */
  if ((mypage = fmpool_get(mpfs->mp, pageno, 0)) == NULL)
    {
      if ((mypage = fmpool_new(mpfs->mp, &new_pgno, pagesize, 0)) == NULL)
        {
          ret = FAIL;
          goto done;
        }
      mpfs->curp = new_pgno;
      mpfs->poff = 0;
      if (fmpool_set_lastpagesize(mpfs->mp,0) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }
#ifdef MP_DEBUG
      fprintf(stderr,"MPwrite: page =%u does not exist\n",pageno);
      fprintf(stderr,"MPwrite: pagesize=%u \n",pagesize);
#endif
    }
  else
    mpfs->curp = pageno;

#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: now mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: now mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr,"MPwrite: now mpfs->poff =%d\n",mpfs->poff);
#endif

  cptr = (char *)mypage + mpfs->poff; /* adjust into current page */

  /* set number of bytes to write */
  if (nbl > (pagesize - mpfs->poff))
    nbw = (pagesize - mpfs->poff); 
  else
    nbw = nbl;

  memcpy(cptr,bptr,nbw); /* copy into first page */
  mpfs->poff = (off_t)(mpfs->poff + nbw);

#ifdef MP_DEBUG
  fprintf(stderr,"MPwrite: copied %d bytes\n",nbw);
  fprintf(stderr,"MPwrite: mark dirty page=%u\n",mpfs->curp);
  fprintf(stderr,"MPwrite: after write mpfs->poff =%d\n",mpfs->poff);
#endif
  /* Mark page as dirty */
  if (fmpool_put(mpfs->mp, mypage, MPOOL_DIRTY) == RET_ERROR)
    {
      ret = FAIL;
      goto done;
    }

  bptr = (char *)bptr + nbw; /* increment buffer ptr */
  nbl -= nbw;  /* decrement bytes left to write */
  nw += nbw;

  /* Is is this the last page? */
  if (mpfs->curp == fmpool_get_npages(mpfs->mp))
    { /* set last page size */
      if (mpfs->poff && (mpfs->poff > fmpool_get_lastpagesize(mpfs->mp)))
        {
          if (fmpool_set_lastpagesize(mpfs->mp,mpfs->poff) == RET_ERROR)
            {
              ret = FAIL;
              goto done;
            }
#ifdef MP_DEBUG
          fprintf(stderr,"MPwrite: now lastpagesize=%u \n",
                  fmpool_get_lastpagesize(mpfs->mp));
#endif
        }
    }

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
      if ((mypage = fmpool_get(mpfs->mp, mpfs->curp, 0)) == NULL)
        { /* Is the page we requested still less than total number of pages */
          if (mpfs->curp <= fmpool_get_npages(mpfs->mp))
            {
#ifdef MP_DEBUG
              fprintf(stderr,"EXIT_ERROR->MPwrite: wrote %d bytes\n",nw);
              fprintf(stderr,"MPwrite: mpfs->curp =%u\n",mpfs->curp);
              fprintf(stderr,"MPwrite: mpfs->foff =%u\n",mpfs->foff);
              fprintf(stderr,"MPwrite: mpfs->poff =%u\n\n",mpfs->poff);
#endif
              ret = FAIL;
              goto done;
            }
          else
            {
              if ((mypage = fmpool_new(mpfs->mp, &new_pgno, pagesize, 0)) 
                  == NULL)
                {
                  ret = FAIL;
                  goto done;
                }
#ifdef MP_DEBUG
              fprintf(stderr,"MPwrite: page =%u does not exist\n",mpfs->curp);
              fprintf(stderr,"MPwrite: new page =%u \n",new_pgno);
              fprintf(stderr,"MPwrite: pagesize=%u \n",pagesize);
#endif
              mpfs->curp = new_pgno;
              mpfs->poff = 0;
              if (fmpool_set_lastpagesize(mpfs->mp,0) == RET_ERROR)
                {
                  ret = FAIL;
                  goto done;
                }
            }
        } /* end if mypage */

      if (nbl > pagesize)
        nbw = pagesize;
      else
        nbw = nbl;
      memcpy(mypage,bptr,nbw); /* copy page */
      mpfs->poff = nbw;
#ifdef MP_DEBUG
      fprintf(stderr,"MPwrite: copied %d bytes, mpfs->poff=%u\n",nbw,mpfs->poff);
      fprintf(stderr,"MPwrite: mark dirty page=%u\n",mpfs->curp);
#endif
      /* Mark page as dirty */
      if (fmpool_put(mpfs->mp, mypage, MPOOL_DIRTY) == RET_ERROR)
        {
          ret = FAIL;
          goto done;
        }

      bptr = (char *)bptr + nbw; /* increment buffer ptr */
      nbl -= nbw;  /* decrement bytes left to write */
      nw += nbw;
      /* Is is this the last page? */
      if (mpfs->curp == fmpool_get_npages(mpfs->mp))
        { /* set last page size */
          if (mpfs->poff && (mpfs->poff > fmpool_get_lastpagesize(mpfs->mp)))
            {
              if (fmpool_set_lastpagesize(mpfs->mp,mpfs->poff) == RET_ERROR)
                {
                  ret = FAIL;
                  goto done;
                }
#ifdef MP_DEBUG
              fprintf(stderr,"MPwrite: now lastpagesize=%u \n",
                      fmpool_get_lastpagesize(mpfs->mp));
#endif
            }
        }
    } /* end while */

  mpfs->foff += nw; /* set file offset */

  done:
  if(ret == FAIL)
    { /* error cleanup */
      nw = FAIL;
#ifdef MP_DEBUG
      fprintf(stderr,"EXIT->MPwrite Error\n");
#endif
    }
  /* Normal cleanup */
#ifdef MP_DEBUG
  fprintf(stderr,"EXIT->MPwrite: wrote %d bytes\n",nw);
  fprintf(stderr," MPwrite: mpfs->curp =%u\n",mpfs->curp);
  fprintf(stderr," MPwrite: mpfs->foff =%d\n",mpfs->foff);
  fprintf(stderr," MPwrite: mpfs->poff =%d\n",mpfs->poff);
  fprintf(stderr," MPwrite: now lastpagesize=%u \n\n",
          fmpool_get_lastpagesize(mpfs->mp));
#endif
  return nw;
} /* MPwrite() */
