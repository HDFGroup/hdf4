/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                      *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/****************************************************************************e
*
* vrw.c
* Part of the HDF VSet interface.
*
************************************************************************/

#include "vg.h"
#include "hfile.h"

PRIVATE void Knumin
    PROTO((uint8 *src,uint8 * dst,uint32 n,uint32 sdel,uint32 ddel));

PRIVATE void Knumout
    PROTO((uint8 *src,uint8 * dst,uint32 n,uint32 sdel,uint32 ddel));

#ifdef PROTOTYPE
PRIVATE void Knumin (uint8 *src, uint8 *dst, uint32 n, uint32 sdel, uint32 ddel)
#else
PRIVATE void Knumin (src, dst, n, sdel, ddel)
uint8        *src, *dst;
uint32      n, sdel, ddel;
#endif
{
  (*DFKnumin) (src, dst, n, sdel, ddel);
}

#ifdef PROTOTYPE
PRIVATE void Knumout (uint8 *src, uint8 *dst, uint32 n, uint32 sdel, uint32 ddel)
#else
PRIVATE void Knumout (src, dst, n, sdel, ddel)
uint8        *src, *dst;
uint32      n, sdel, ddel;
#endif
{
  (*DFKnumout) (src, dst, n, sdel, ddel);
}

/* --------------------------- VSseek -------------------------------------- */

/* 
	VSseek  - now allows seeks for reads AND writes. 30/mar/92 jason ng

	Seeks to an element boundary within a vdata
	Vdata must be attached with "r" or "w" access.
	Specify eltpos = 0 for 1st element, 1 for 2nd element etc.
	RETURNS -1 on error
	RETURNS position of element seeked to (0 or a +ve integer)
        (eg  returns 5 if seek to the 6th element, etc)
*/
#ifdef PROTOTYPE
PUBLIC int32 VSseek (int32 vkey, int32 eltpos)
#else
PUBLIC int32 VSseek (vkey, eltpos)
int32 vkey;
int32      eltpos;
#endif
{
	int32 	stat, offset;
    vsinstance_t    *w;
    VDATA           *vs;
    char *  FUNC = "VSseek";

    if (!VALIDVSID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vs=w->vs;
    if ((vs==NULL) || (eltpos < 0))
        HRETURN_ERROR(DFE_ARGS,FAIL);

	offset  = eltpos * vs->wlist.ivsize;

    stat = Hseek (vs->aid, offset, DF_START);
	if (stat==FAIL)
        HRETURN_ERROR(DFE_BADSEEK,FAIL);

	return(eltpos);
} /* Vseek */

/* ------------------------------------------------------------------------ */

/*
	VSread
	Reads a specified number of elements' worth of data from a vdata.
	Data will be returned to you interlaced in the way you specified.
	RETURNS -1 if error
	RETURNS the number of elements read (0 or a +ve integer).
*/

PRIVATE int32 Vtbufsize = 0;
PRIVATE uint8 *Vtbuf = NULL;

#ifdef PROTOTYPE
PUBLIC int32 VSread (int32 vkey,uint8 buf[], int32 nelt, int32 interlace)
#else
PUBLIC int32 VSread (vkey, buf, nelt, interlace)
int32 vkey;
int32   nelt;
int32   interlace;
uint8    buf[];
#endif
{
    register intn isize = 0;
    register intn order = 0;
    register intn index = 0;
    register int16 esize = 0;
    register int16 hsize = 0;
    register uint8   *b1,*b2;
	int32 			i,j, nv, offset, type;
	VWRITELIST 		*w;
	VREADLIST  		*r;
	int32 			uvsize; /* size of "element" as NEEDED by user */
    vsinstance_t    *wi;
    VDATA           *vs;
    char *  FUNC = "VSread";

    if (!VALIDVSID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if(NULL==(wi=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vs=wi->vs;
    if(vs == NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    if(vs->aid == NO_ID)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(vs->nvertices == 0)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(vexistvs(vs->f,vs->oref) == FAIL)
        HRETURN_ERROR(DFE_NOVS,FAIL);

    if(interlace != FULL_INTERLACE  && interlace != NO_INTERLACE)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    w = &(vs->wlist);
    r = &(vs->rlist);
    hsize = vs->wlist.ivsize; 		/* size as stored in HDF */

    /* alloc space (Vtbuf) for reading in the raw data from vdata */
    if(Vtbufsize < nelt * hsize) {
        Vtbufsize = nelt * hsize;
        if(Vtbuf)
            HDfreespace((VOIDP)Vtbuf);
        if((Vtbuf = (uint8 *) HDgetspace ( Vtbufsize )) == NULL)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
    }

    /* ================ start reading ============================== */
    /* ================ start reading ============================== */
    
    nv = Hread (vs->aid, nelt * hsize, (uint8*) Vtbuf);
    
    if ( nv != nelt * hsize )
        HRETURN_ERROR(DFE_READERROR,FAIL);

    /* ================ done reading =============================== */
    /* ================ done reading =============================== */


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
      (E) SPECIAL CASE when field has order>1.

      Cases (A)-(D) handles multiple fields.
      Case (E) handles reading from a Vdata with a single field.
      */

    /* ----------------------------------------------------------------- */
    /* CASE  (E): Only a single field in the Vdata */

    if (w->n == 1) {
        b1 = buf;
        b2 = Vtbuf;
        type = w->type[0];

        /* Errr WORKS */
        DFKsetNT(type);
        DFKnumin (b2, b1, (uint32) w->order[0] * nelt, 0, 0);

        return(nelt);
    } /* case (e) */


    /* ----------------------------------------------------------------- */
    /* CASE  (A):  user=none, vdata=full */

    if (interlace==NO_INTERLACE && vs->interlace==FULL_INTERLACE) {
        b1 = buf;
        for(j = 0; j < r->n; j++) {
            i     = r->item[j];
            b2    = Vtbuf + w->off[i];
            type  = w->type[i];
            isize = w->isize[i];
            esize = w->esize[i];
            order = w->order[i];
            
            /* Arrr ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                DFKnumin (b2, b1, (uint32) nelt, (uint32) hsize, (uint32) esize);
                b2 += isize / order;
                b1 += esize / order;
            }
            b1 += ((nelt - 1) * esize);
        }
    } /* case (a) */

    /* ----------------------------------------------------------------- */
    /* CASE  (B):  user=none, vdata=none */
    else if (interlace==NO_INTERLACE && vs->interlace==NO_INTERLACE) {
        b1 = buf;
        for(j = 0; j < r->n; j++) {
            i     = r->item[j];
            b2    = Vtbuf + w->off[i] * nelt;
            type  = w->type[i];
            esize = w->esize[i];
            isize = w->isize[i];
            order = w->order[i];
            
            /* Brrr ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                DFKnumin (b2, b1, (uint32) nelt, (uint32) isize, (uint32) esize);
                b1 += esize / order ;
                b2 += isize / order;
            }
            b1 += ((nelt - 1) * esize);
        }
    } /* case (b) */
    
    /* ----------------------------------------------------------------- */
    /* CASE  (C):  iu=full, iv=full */
    else if (interlace==FULL_INTERLACE && vs->interlace==FULL_INTERLACE) {
        for (uvsize=0, j=0;j<r->n;j++)
            uvsize += w->esize[r->item[j]];
        
        offset = 0;
        for(j = 0; j < r->n; j++) {
            i     = r->item[j];
            b1    = buf + offset;
            b2    = Vtbuf + w->off[i];
            type  = w->type[r->item[j]];
            esize = w->esize[i];
            isize = w->isize[i];
            order = w->order[i];
            
            /* Crrr ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
/*
printf("C: from : %d  to: %d esize: %d isize: %d order: %d nt: %d\n", 
       (int) b2, (int) b1, (int) esize, (int) isize, (int) order, (int) type);
*/
                DFKnumin (b2, b1, (uint32) nelt, (uint32) hsize, (uint32) uvsize);
                b1 += (int) esize / order;
                b2 += (int) isize / order;
            }
            offset += esize;
        }
    } /* case (c) */
    
    
    /* ----------------------------------------------------------------- */
    /* CASE  (D):  user=full, vdata=none */
    else if(interlace==FULL_INTERLACE && vs->interlace==NO_INTERLACE) {
        
        for (uvsize=0, j=0;j<r->n;j++)
            uvsize += w->esize[r->item[j]];
        
        offset = 0;
        for(j = 0; j < r->n; j++) {
            i     = r->item[j];
            b1    = buf + offset;
            b2    = Vtbuf + w->off[i] * nelt;
            type  = w->type[i];
            isize = w->isize[i];
            order = w->order[i];
            
            /* Drrr ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                DFKnumin (b2, b1, (uint32) nelt, (uint32) isize, (uint32) uvsize);
                b1 += esize / order;
                b2 += isize / order;
            }
            offset += isize;
        }
    } /* case (d) */

    return(nv/hsize);
} /* VSread */

#ifndef WIN3
/* ------------------------------------------------------------------ */
/* debugging routine */

#ifdef PROTOTYPE
void bytedump (char *ss, uint8 buf[], int32 n)
#else
void bytedump (ss, buf, n)
char        *ss;
uint8        buf[];
int32   n;
#endif
{
    int32 i;

    printf("uint8DUMP at %ld [%s %ld]: ",buf, ss, n);
    for(i=0;i<n;i++)
        printf(" %x", buf[i]);
    printf("\n");
    fflush(stdout);
}
#endif  /* WIN3 */

/* ------------------------------------------------------------------ */
/*
	VSwrite
	Writes a specified number of elements' worth of data to a vdata.
	You must specify how your data in your buffer is interlaced.

	RETURNS -1 if error
	RETURNS the number of elements written (0 or a +ve integer).

	NEW
	create an aid, and write out if this is the first time.
	(otherwise) subsequent writes result in link-blocks.

*/

#ifdef PROTOTYPE
PUBLIC int32 VSwrite (int32 vkey, uint8 buf[], int32 nelt, int32 interlace)
#else
PUBLIC int32 VSwrite (vkey, buf, nelt, interlace)
int32 vkey;
int32       nelt;
int32       interlace;
uint8        buf[];
#endif
{
    register intn  isize = 0;
    register intn  order = 0;
    register intn  index = 0;
    register int16 esize = 0;
    register int16 hsize = 0;
    register uint8 *b1,*b2;

    int32 		j,type, offset;
    int16       special;
    int32           position, new_size;
	VWRITELIST	*w;
	int32 		uvsize;		/* size of "element" as needed by user */
    vsinstance_t    *wi;
    VDATA           *vs;
    char *  FUNC = "VSwrite";

    if (!VALIDVSID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vs's index in vstab */
    if(NULL==(wi=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vs=wi->vs;
    if ((nelt <= 0) || (vs == NULL))
        HRETURN_ERROR(DFE_ARGS,FAIL);
    if (vs->access != 'w')
        HRETURN_ERROR(DFE_BADACC,FAIL);
    if ( -1L == vexistvs(vs->f,vs->oref) )
        HRETURN_ERROR(DFE_NOVS,FAIL);
    if (vs->wlist.ivsize == 0)
        HRETURN_ERROR(DFE_NOVS,FAIL);
    if (interlace != NO_INTERLACE && interlace != FULL_INTERLACE )
        HRETURN_ERROR(DFE_ARGS,FAIL);

	w = (VWRITELIST*) &vs->wlist;
	hsize = w->ivsize; 		/* as stored in HDF file */

	/* alloc space (Vtbuf) for writing out the data */
    if(Vtbufsize < nelt * hsize) {
        Vtbufsize = nelt * hsize;
        if(Vtbuf)
            HDfreespace((VOIDP)Vtbuf);
        if((Vtbuf = (uint8 *) HDgetspace ( Vtbufsize )) == NULL) {
            HERROR(DFE_NOSPACE);
            return(FAIL);
        }
    }


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
		(E) SPECIAL CASE when field has order>1. 

		Cases (A)-(D) handles multiple fields
		Case (E) handles single field Vdatas

    --------------------------------------------------------------------- */
    /* CASE  (E): Special Case For single field Vdatas */
    
    if (w->n == 1) {
        b1    = buf;
        b2    = Vtbuf;
        esize = w->esize[0];
        isize = w->isize[0];
        type  = w->type[0];
        
	/* Ewww ? */
        DFKsetNT(type); 
        DFKnumout(b1, b2, (uint32) w->order[0] * nelt, (uint32) 0, (uint32) 0);

    } /* case (e) */
    
    /* ----------------------------------------------------------------- */
    /* CASE  (A):  user=none, vdata=full */
    else if (interlace == NO_INTERLACE && vs->interlace == FULL_INTERLACE) {
        
        b1 = buf;
        for(j = 0; j < w->n; j++) {
            b2    = Vtbuf + w->off[j];
            type  = w->type[j];
            esize = w->esize[j];
            order = w->order[j];
            
            /* Awww ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                Knumout (b1, b2, (uint32) nelt, (uint32) esize, (uint32) hsize);
                b1 += esize / order;
                b2 += isize / order;
            }
            b1 += ((nelt - 1) * esize);
        }
        
    } /* case (a) */
    
    /* --------------------------------------------------------------------- */
    /* CASE  (B):  user=none, vdata=none */
    else if (interlace == NO_INTERLACE && vs->interlace == NO_INTERLACE) {
        
        b1 = buf;
        for(j = 0; j < w->n; j++) {
            b2    = Vtbuf + w->off[j] * nelt;
            type  = w->type[j];
            esize = w->esize[j];
            isize = w->isize[j];
            order = w->order[j];
            
            /* Bwww ? works sometimes */
            DFKsetNT(type);  
            for(index = 0; index < order; index++) {
                Knumout (b1, b2, (uint32) nelt, (uint32) esize, (uint32) isize);
                b2 += isize / order;
                b1 += esize / order;
            }
            b1 += ((nelt - 1) * esize);
        }
        
    } /* case (b) */
    
    /* ----------------------------------------------------------------- */
    /* CASE  (C):  user=full, vdata=full */
    else if (interlace==FULL_INTERLACE && vs->interlace==FULL_INTERLACE) {
        for(uvsize = 0, j = 0; j < w->n; j++)
            uvsize += w->esize[j];
        
        offset = 0;
        for(j = 0; j < w->n; j++) {
            b1    = buf + offset;
            b2    = Vtbuf + w->off[j];
            type  = w->type[j];
            esize = w->esize[j];
            isize = w->isize[j];
            order = w->order[j];
            
            /* Cwww WORKS  */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                Knumout (b1, b2, (uint32) nelt, (uint32) uvsize, (uint32) hsize);
                b2 += isize / order;
                b1 += esize / order;
            }
            offset += esize;
        }
    } /* case (c) */
    
    /* ----------------------------------------------------------------- */
    /* CASE  (D):  user=full, vdata=none */
    else if (interlace==FULL_INTERLACE && vs->interlace==NO_INTERLACE) {
        
        for(uvsize=0, j=0; j<w->n; j++)
            uvsize += w->esize[j];
        
        offset = 0;
        for(j = 0; j < w->n; j++) {
            b1    = buf  + offset;
            b2    = Vtbuf + w->off[j] * nelt;
            type  = w->type[j];
            isize = w->isize[j];
            esize = w->esize[j];
            order = w->order[j];
            
            /* Dwww ? */
            DFKsetNT(type); 
            for(index = 0; index < order; index++) {
                Knumout (b1, b2, (uint32) nelt, (uint32) uvsize, (uint32) isize);
                b2 += isize / order;
                b1 += esize / order;
            }
            offset +=esize;
        }
    } /* case (d) */
    
    
    /* ================ start writing ============================== */
    /* ================ start writing ============================== */
    
#if 1
    if (vs->aid == 0) { /* aid not allocated yet */
        vs->aid = Hstartwrite (vs->f,VSDATATAG, vs->oref, (int32) nelt * hsize);
        if (vs->aid == FAIL) { HERROR(DFE_BADAID); return(FAIL); }
	}

	/* 
     * promote to link-block if vdata exists and is not already one
     *  AND we are incresing its size
     */
    HQueryposition(vs->aid, &position);
    new_size = (position / vs->wlist.ivsize) + nelt;

	if (vs->nvertices > 0) {
        HQueryspecial(vs->aid, &special);
        if (!special && (new_size > vs->nvertices)) {
            
            int32 blk_size;

            blk_size = (new_size > VDEFAULTBLKSIZE ? new_size : VDEFAULTBLKSIZE);

            Hendaccess(vs->aid);
            vs->aid = HLcreate(vs->f, VSDATATAG , vs->oref, 
                               blk_size, VDEFAULTNBLKS);

            /* seek back to correct point */
            j = Hseek(vs->aid, position, DF_START);
        }
    }
	  
    j = Hwrite (vs->aid,  nelt * hsize, (uint8*) Vtbuf);
    if (j != nelt * hsize)
        HRETURN_ERROR(DFE_WRITEERROR,FAIL);

    if(new_size > vs->nvertices) vs->nvertices = new_size;

	vs->marked = 1;
	return (nelt);
#endif

} /* VSwrite */

/* ------------------------------------------------------------------ */
