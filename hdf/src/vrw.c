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
/***********************************************************************
*
* vrw.c
* Part of the HDF VSet interface.
* This module handles reading and writing of Vdatas.
*

LOCAL ROUTINES
 None
EXPORTED ROUTINES
 VSseek  -- Seeks to an element boundary within a vdata i.e. 2nd element.
 VSread  -- Reads a specified number of elements' worth of data from a vdata.
             Data will be returned to you interlaced in the way you specified.
 VSwrite -- Writes a specified number of elements' worth of data to a vdata.
	     You must specify how your data in your buffer is interlaced.
             Creates an aid, and writes it out if this is the first time.

************************************************************************/

#include "vg.h"

#ifndef MIN
#define MIN(a,b)     ((a) < (b) ? (a) : (b))
#endif /* MIN */

#if 0
/*
 * Private conversion buffer stuff
 * VDATA_BUFFER_MAX is the largest buffer that can be allocated for
 *   writing (haven't implemented reading yet).
 * Vtbuf is the buffer
 * Vtbufsize is the buffer size in bytes at any given time.
 * Vtbuf is increased in size as need be
 * BUG: the final Vtbuf never gets freed
 */
#define VDATA_BUFFER_MAX 1000000
#endif /* if 0 */

PRIVATE uint32 Vtbufsize = 0;
PRIVATE uint8 *Vtbuf = NULL;

/*--------------------------------------------------------------------------
 NAME
    VSPfreebuf
 PURPOSE
    Free the Vtbuf buffer.
 USAGE
    intn VSPfreebuf()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    For completeness, when the VSet interface is shut-down, free the Vtbuf.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn VSPfreebuf(void)
{
  intn  ret_value = SUCCEED;

  if(Vtbuf!=NULL)
    {
      HDfree(Vtbuf);
      Vtbuf=NULL;
      Vtbufsize = 0;
    } /* end if */

  return ret_value;
} /* end VSPfreebuf() */

/* --------------------------- VSseek -------------------------------------- */

/*
   VSseek

   Seeks to an element boundary within a vdata
   Vdata must be attached with "r" or "w" access.
   Specify eltpos = 0 for 1st element, 1 for 2nd element etc.
   RETURNS FAIL on error
   RETURNS position of element seeked to (0 or a +ve integer)
   (eg  returns 5 if seek to the 6th element, etc)
 */
int32
VSseek(int32 vkey, int32 eltpos)
{
    int32       ret, offset;
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSseek");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSseek);
#endif /* HAVE_PABLO */

    if (!VALIDVSID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if ((vs == NULL) || (eltpos < 0))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    offset = eltpos * vs->wlist.ivsize;

    ret = Hseek(vs->aid, offset, DF_START);
    if (ret == FAIL)
        HGOTO_ERROR(DFE_BADSEEK, FAIL);

    ret_value = (eltpos);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSseek);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSseek */

/* ------------------------------------------------------------------------ */
/*
   VSread
   Reads a specified number of elements' worth of data from a vdata.
   Data will be returned to you interlaced in the way you specified.
   RETURNS FAIL if error
   RETURNS the number of elements read (0 or a +ve integer).
 */
int32 
VSread(int32 vkey, uint8 buf[], int32 nelt, int32 interlace)
{
    intn isize = 0;
    intn order = 0;
    intn index = 0;
    int16 esize = 0;
    int16 hsize = 0;
    uint8 *b1, *b2;
    int32       i, j, nv, offset, type;
    DYN_VWRITELIST *w;
    DYN_VREADLIST  *r;
    int32       uvsize;         /* size of "element" as NEEDED by user */
    vsinstance_t *wi;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSread");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSread);
#endif /* HAVE_PABLO */

    if (!VALIDVSID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (wi = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = wi->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if ((vs->aid == NO_ID) || (vs->nvertices == 0))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vexistvs(vs->f, vs->oref) == FAIL)
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if (interlace != FULL_INTERLACE && interlace != NO_INTERLACE)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    w = &(vs->wlist);
    r = &(vs->rlist);
    hsize = vs->wlist.ivsize;   /* size as stored in HDF */

    /* alloc space (Vtbuf) for reading in the raw data from vdata */
    if (Vtbufsize < nelt * (uint32) hsize)
      {
          Vtbufsize = nelt * (uint32) hsize;
          if (Vtbuf)
              HDfree((VOIDP) Vtbuf);
          if ((Vtbuf = (uint8 *) HDmalloc(Vtbufsize)) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
      }

    /* ================ start reading ============================== */

    nv = Hread(vs->aid, nelt * hsize, (uint8 *) Vtbuf);

    if (nv != nelt * hsize)
      {
          HERROR(DFE_READERROR);
          HEreport("Tried to read %d, only read %d", nelt * hsize, nv);
          ret_value = FAIL;
          goto done;
      }

    /*
       Now, convert and repack field(s) from Vtbuf into buf.

       This section of the code deals with interlacing. In all cases
       the items for each of the fields are converted and shuffled
       around from the internal buffer "Vtbuf" to the user's buffer
       "buf".

       There are 5 cases :
       (A) user=NO_INTERLACE   & vdata=FULL_INTERLACE)
       (B) user=NO_INTERLACE   & vdata=NO_INTERLACE)
       (C) user=FULL_INTERLACE & vdata=FULL_INTERLACE)
       (D) user=FULL_INTERLACE & vadat=NO_INTERLACE)
       (E) SPECIAL CASE when only one field.

       Cases (A)-(D) handles multiple fields.
       Case (E) handles reading from a Vdata with a single field.
     */

    /* ----------------------------------------------------------------- */
    /* CASE  (E): Only a single field in the Vdata */
    if (w->n == 1)
      {
          b1 = buf;
          b2 = Vtbuf;
          DFKsetNT(w->type[0]);

          DFKnumin(b2, b1, (uint32) w->order[0] * nelt, 0, 0);

          ret_value = (nelt);
          goto done;
      }     /* case (e) */

    /* ----------------------------------------------------------------- */
    /* CASE  (A):  user=none, vdata=full */
    if (interlace == NO_INTERLACE && vs->interlace == FULL_INTERLACE)
      {
          b1 = buf;
          for (j = 0; j < r->n; j++)
            {
                i = r->item[j];
                b2 = Vtbuf + w->off[i];
                type = w->type[i];
                isize = w->isize[i];
                esize = w->esize[i];
                order = w->order[i];

                DFKsetNT(type);
                for (index = 0; index < order; index++)
                  {
                      DFKnumin(b2, b1, (uint32) nelt, (uint32) hsize, (uint32) esize);
                      b2 += isize / order;
                      b1 += esize / order;
                  }
                b1 += ((nelt - 1) * esize);
            }
      }     /* case (a) */

    /* ----------------------------------------------------------------- */
    /* CASE  (B):  user=none, vdata=none */
    else if (interlace == NO_INTERLACE && vs->interlace == NO_INTERLACE)
      {
          b1 = buf;
          for (j = 0; j < r->n; j++)
            {
                i = r->item[j];
                b2 = Vtbuf + w->off[i] * nelt;
                type = w->type[i];
                esize = w->esize[i];
                isize = w->isize[i];
                order = w->order[i];

                DFKsetNT(type);
                for (index = 0; index < order; index++)
                  {
                      DFKnumin(b2, b1, (uint32) nelt, (uint32) isize, (uint32) esize);
                      b1 += esize / order;
                      b2 += isize / order;
                  }
                b1 += ((nelt - 1) * esize);
            }
      }     /* case (b) */

    /* ----------------------------------------------------------------- */
    /* CASE  (C):  iu=full, iv=full */
    else if (interlace == FULL_INTERLACE && vs->interlace == FULL_INTERLACE)
      {
          for (uvsize = 0, j = 0; j < r->n; j++)
              uvsize += w->esize[r->item[j]];

          offset = 0;
          for (j = 0; j < r->n; j++)
            {
                i = r->item[j];
                b1 = buf + offset;
                b2 = Vtbuf + w->off[i];
                type = w->type[r->item[j]];
                esize = w->esize[i];
                isize = w->isize[i];
                order = w->order[i];

                DFKsetNT(type);
                for (index = 0; index < order; index++)
                  {
                      DFKnumin(b2, b1, (uint32) nelt, (uint32) hsize, (uint32) uvsize);
                      b1 += (int) esize / order;
                      b2 += (int) isize / order;
                  }
                offset += esize;
            }
      }     /* case (c) */

    /* ----------------------------------------------------------------- */
    /* CASE  (D):  user=full, vdata=none */
    else if (interlace == FULL_INTERLACE && vs->interlace == NO_INTERLACE)
      {

          for (uvsize = 0, j = 0; j < r->n; j++)
              uvsize += w->esize[r->item[j]];

          offset = 0;
          for (j = 0; j < r->n; j++)
            {
                i = r->item[j];
                b1 = buf + offset;
                b2 = Vtbuf + w->off[i] * nelt;
                type = w->type[i];
                isize = w->isize[i];
                esize = w->esize[i];
                order = w->order[i];

                DFKsetNT(type);
                for (index = 0; index < order; index++)
                  {
                      DFKnumin(b2, b1, (uint32) nelt, (uint32) isize, (uint32) uvsize);
                      b1 += esize / order;
                      b2 += isize / order;
                  }
                offset += isize;
            }
      }     /* case (d) */

    ret_value = (nv / hsize);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSread);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VSread */

/* ------------------------------------------------------------------ */
/*
   VSwrite
   Writes a specified number of elements' worth of data to a vdata.
   You must specify how your data in your buffer is interlaced.
   RETURNS FAIL if error
   RETURNS the number of elements written (0 or a +ve integer).

   NEW
   create an aid, and write out if this is the first time.
   (otherwise) subsequent writes result in link-blocks.
 */
int32 
VSwrite(int32 vkey, uint8 buf[], int32 nelt, int32 interlace)
{
    intn isize = 0;
    intn order = 0;
    intn index = 0;
    int16 esize = 0;
    uint8 *src, *dest, *Src;

    int32       j, type, offset;
    int16       special;
    int32       position, new_size;
    int32       status;
    int32       total_bytes;    /* total number of bytes that need to be written out */
    DYN_VWRITELIST *w;
    int32       int_size;       /* size of "element" as needed by user in memory */
    intn        hdf_size = 0;   /* size of record in HDF file */
    vsinstance_t *wi;
    VDATA      *vs;
    int32       bytes;          /* number of elements / bytes to write next time */
    int32       chunk, done;    /* number of records to do / done */
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "VSwrite");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSwrite);
#endif /* HAVE_PABLO */

    if (!VALIDVSID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (wi = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = wi->vs;

    if ((nelt <= 0) || (vs == NULL))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vs->access != 'w')
        HGOTO_ERROR(DFE_BADACC, FAIL);

    if (-1L == vexistvs(vs->f, vs->oref))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    w = & vs->wlist;
    if (w->n == 0)
      {
          HERROR(DFE_NOVS);
          HEreport("No fields set for writing");
          ret_value = (FAIL);
          goto done;
      }

    if (interlace != NO_INTERLACE && interlace != FULL_INTERLACE)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    hdf_size = w->ivsize;   /* as stored in HDF file */
    total_bytes = hdf_size * nelt;

    /* make sure we have a valid AID */
    if (vs->aid == 0)
      {
          vs->aid = Hstartwrite(vs->f, DFTAG_VS, vs->oref, total_bytes);
          if (vs->aid == FAIL)
              HGOTO_ERROR(DFE_BADAID, FAIL);
      }

    /*
     * promote to link-block if vdata exists and is not already one
     *  AND we are incresing its size
     */
    HQueryposition(vs->aid, &position);
    new_size = (position / vs->wlist.ivsize) + nelt;

    if ((vs->nvertices > 0) && (new_size > vs->nvertices))
      {
          HQueryspecial(vs->aid, &special);
          if (!special)
            {

                int32       blk_size;

                blk_size = ((new_size*vs->wlist.ivsize) > VDEFAULTBLKSIZE ? (new_size*vs->wlist.ivsize) : VDEFAULTBLKSIZE);

                Hendaccess(vs->aid);
                vs->aid = HLcreate(vs->f, VSDATATAG, vs->oref,
                                   blk_size, VDEFAULTNBLKS);

                /* seek back to correct point */
                j = Hseek(vs->aid, position, DF_START);
            }
      }

    /* this should really be cached in the Vdata structure */
    for (int_size = 0, j = 0; j < w->n; j++)
        int_size += w->esize[j];

    /*
       First, convert and repack field(s) from Vtbuf into buf.

       This section of the code deals with interlacing. In all cases
       the items for each of the fields are converted and shuffled
       around from the user's buffer "buf" to the internal's buffer
       "Vtbuf".  The data in "Vtbuf" is then written out to the vdata.

       There are 5 cases :
       (A) user=NO_INTERLACE   & vdata=FULL_INTERLACE)
       (B) user=NO_INTERLACE   & vdata=NO_INTERLACE)
       (C) user=FULL_INTERLACE & vdata=FULL_INTERLACE)
       (D) user=FULL_INTERLACE & vadat=NO_INTERLACE)
       (E) SPECIAL CASE when only one field.

       Cases (A)-(D) handles multiple fields
       Case (E) handles single field Vdatas

       Cases (E) and (C) are the most frequently used.  Limit buffer
       allocations to VDATA_BUFFER_MAX size so that we conserve
       memory.  Doing this involves a certain degree of added code
       complexity so don't bother doing it for the less frequent
       cases.  Cases E and C have been rolled together since they are
       very similar and both need the incremental writing.

       --------------------------------------------------------------------- */
    /* CASE  (E + C): Easy to unroll case */
    if ((w->n == 1) || (interlace == FULL_INTERLACE && vs->interlace == FULL_INTERLACE))
      {

          /*
           * figure out how many elements we can move at a time and
           * make sure our buffer is big enough
           */

          if ((uint32) total_bytes < Vtbufsize)
            {
                chunk = nelt;
            }
          else
            {
                int32       buf_size;

                /* we are bounded above by VDATA_BUFFER_MAX */
                buf_size = MIN(total_bytes, VDATA_BUFFER_MAX);

                /* make sure there is at least room for one record in our buffer */
                chunk = buf_size / hdf_size + 1;

                /* get a buffer big enough to hold the values */
                Vtbufsize = chunk * hdf_size;
                if (Vtbuf)
                    HDfree((VOIDP) Vtbuf);
                if ((Vtbuf = (uint8 *) HDmalloc(Vtbufsize)) == NULL)
                    HGOTO_ERROR(DFE_NOSPACE, FAIL);
            }

          done = 0;

          /* set loop invariant parameters */
          Src = buf;
          dest = Vtbuf;
          bytes = hdf_size * chunk;

          while (done < nelt)
            {

                /* chunk has changed so update the byte counts */
                if (nelt - done < chunk)
                  {
                      chunk = nelt - done;
                      bytes = hdf_size * chunk;
                  }
/*
   printf("Case E/C: [%d,%d] writing %d (elems) %d bytes\n", done, nelt, chunk, bytes);
 */

                offset = 0;
                for (j = 0; j < w->n; j++)
                  {
                      src = Src + offset;
                      dest = Vtbuf + w->off[j];
                      type = w->type[j];
                      esize = w->esize[j];
                      isize = w->isize[j];
                      order = w->order[j];

                      DFKsetNT(type);
                      for (index = 0; index < order; index++)
                        {
                            DFKnumout(src, dest, (uint32) chunk, (uint32) int_size, (uint32) hdf_size);
                            dest += isize / order;
                            src += esize / order;
                        }
                      offset += esize;
                  }

                /* write the converted data to the file */
                status = Hwrite(vs->aid, bytes, (uint8 *) Vtbuf);
                if (status != bytes)
                    HGOTO_ERROR(DFE_WRITEERROR, FAIL);

                /* record what we've done and move to next group */
                done += chunk;
                Src += chunk * int_size;
            }

      }     /* case (C + E) */

    else
      {

	  /* 
	   * Handle the other cases now.
	   * These cases are less frequent so don't bother unrolling
	   *   the loops for now.  As a result, we may get into memory
	   *   problems since we may end up allocating a huge buffer
	   */

	  /* alloc space (Vtbuf) for writing out the data */
	  if (Vtbufsize < (uint32) total_bytes)
	    {
		Vtbufsize = total_bytes;
		if (Vtbuf)
		    HDfree((VOIDP) Vtbuf);
		if ((Vtbuf = (uint8 *) HDmalloc(Vtbufsize)) == NULL)
		    HGOTO_ERROR(DFE_NOSPACE, FAIL);
	    }

	  /* ----------------------------------------------------------------- */
	  /* CASE  (A):  user=none, vdata=full */
	  if (interlace == NO_INTERLACE && vs->interlace == FULL_INTERLACE)
	    {

		src = buf;
		for (j = 0; j < w->n; j++)
		  {
		      dest = Vtbuf + w->off[j];
		      type = w->type[j];
		      esize = w->esize[j];
		      isize = w->isize[j];
		      order = w->order[j];

		      DFKsetNT(type);
		      for (index = 0; index < order; index++)
			{
			    DFKnumout(src, dest, (uint32) nelt, (uint32) esize, (uint32) hdf_size);
			    src += esize / order;
			    dest += isize / order;
			}
		      src += ((nelt - 1) * esize);
		  }

	    }	/* case (a) */

	  /* --------------------------------------------------------------------- */
	  /* CASE  (B):  user=none, vdata=none */
	  else if (interlace == NO_INTERLACE && vs->interlace == NO_INTERLACE)
	    {

		src = buf;
		for (j = 0; j < w->n; j++)
		  {
		      dest = Vtbuf + w->off[j] * nelt;
		      type = w->type[j];
		      esize = w->esize[j];
		      isize = w->isize[j];
		      order = w->order[j];

		      DFKsetNT(type);
		      for (index = 0; index < order; index++)
			{
			    DFKnumout(src, dest, (uint32) nelt, (uint32) esize, (uint32) isize);
			    dest += isize / order;
			    src += esize / order;
			}
		      src += ((nelt - 1) * esize);
		  }

	    }	/* case (b) */

	  /* ----------------------------------------------------------------- */
	  /* CASE  (D):  user=full, vdata=none */
	  else if (interlace == FULL_INTERLACE && vs->interlace == NO_INTERLACE)
	    {
		offset = 0;
		for (j = 0; j < w->n; j++)
		  {
		      src = buf + offset;
		      dest = Vtbuf + w->off[j] * nelt;
		      type = w->type[j];
		      isize = w->isize[j];
		      esize = w->esize[j];
		      order = w->order[j];

		      DFKsetNT(type);
		      for (index = 0; index < order; index++)
			{
			    DFKnumout(src, dest, (uint32) nelt, (uint32) int_size, (uint32) isize);
			    dest += isize / order;
			    src += esize / order;
			}
		      offset += esize;
		  }
	    }	/* case (d) */

	  status = Hwrite(vs->aid, total_bytes, (uint8 *) Vtbuf);
	  if (status != total_bytes)
	      HGOTO_ERROR(DFE_WRITEERROR, FAIL);

      }     /* cases a, b, and d */

    /* update the internal structure to reflect write */
    if (new_size > vs->nvertices)
        vs->nvertices = new_size;
    vs->marked = 1;

    ret_value = (nelt);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSwrite);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSwrite */

/* ------------------------------------------------------------------ */
