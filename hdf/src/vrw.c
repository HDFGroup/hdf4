#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/11/30 22:00:01  chouck
Added fixes for changing to Vstart and Vend

 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/****************************************************************************e
*
* vrw.c
* Part of the HDF VSet interface.
*
************************************************************************/

#include "vg.h"
#include "hfile.h"

PRIVATE void Knumin
    PROTO((BYTE *src,BYTE * dst,uint32 n,uint32 sdel,uint32 ddel));

PRIVATE void Knumout
    PROTO((BYTE *src,BYTE * dst,uint32 n,uint32 sdel,uint32 ddel));

#ifdef PROTOTYPE
PRIVATE void Knumin (BYTE *src, BYTE *dst, uint32 n, uint32 sdel, uint32 ddel)
#else
PRIVATE void Knumin (src, dst, n, sdel, ddel)
	BYTE 		*src, *dst;
	uint32  	n, sdel, ddel;
#endif

{
  if (vjv) {
    sprintf(sjs, "  -->> Knumin: n=%ld sdel=%ld ddel=%ld src=%lx dst=%lx\n",
            n, sdel, ddel, src, dst); zj;
  }
  
  (*DFKnumin) (src, dst, n, sdel, ddel);
  
}

#ifdef PROTOTYPE
PRIVATE void Knumout (BYTE *src, BYTE *dst, uint32 n, uint32 sdel, uint32 ddel)
#else
PRIVATE void Knumout (src, dst, n, sdel, ddel)
	BYTE 		*src, *dst;
	uint32  	n, sdel, ddel;
#endif

{
  if (vjv) {
    sprintf(sjs, "  -->> Knumout: n=%ld sdel=%ld ddel=%ld src=%ld dst=%ld\n",
            n, sdel, ddel, src, dst); zj;
  }
  
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
PUBLIC int32 VSseek (VDATA *vs, int32 eltpos)    
#else


PUBLIC int32 VSseek (vs, eltpos)    
	VDATA *	vs;
	int32	   eltpos;

#endif

{
	int32 	stat, offset;
	char * 	FUNC = "VSseek";

	if ((vs==NULL) || (eltpos < 0)) {HERROR(DFE_ARGS); return(FAIL);}

	offset  = eltpos * vs->wlist.ivsize;

	stat = QQseek (vs->aid, offset, DF_START);
	if (stat==FAIL) {
          HERROR(DFE_BADSEEK);
          return(FAIL);
        }

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
PRIVATE BYTE *Vtbuf = NULL;

#ifdef PROTOTYPE
PUBLIC int32 VSread (VDATA *vs,BYTE buf[], int32 nelt, int32 interlace)  
#else
PUBLIC int32 VSread (vs, buf, nelt, interlace)  
	VDATA   *vs;
	int32	nelt;
	int32	interlace;
	BYTE	buf[];
#endif

{
	register int16 isize,esize,hsize;
	register BYTE	*b1,*b2;
	int32 			i,j, nv, offset, type;
	VWRITELIST 		*w;
	VREADLIST  		*r;
	int32 			uvsize; /* size of "element" as NEEDED by user */
	char * 	FUNC = "VSread";

	if(vs == NULL)		{ HERROR(DFE_ARGS); return(FAIL); }
	if(vs->aid == NO_ID)    { HERROR(DFE_ARGS); return(FAIL); }
/* Allow people to read when they have write access 
	if(vs->access != 'r')	{ HERROR(DFE_BADACC); return(FAIL); }
*/
	if(vs->nvertices == 0)	{ HERROR(DFE_ARGS); return(FAIL); }

	if(vexistvs(vs->f,vs->oref) == FAIL) { HERROR(DFE_NOVS); return(FAIL); }

	if(interlace != FULL_INTERLACE  && interlace != NO_INTERLACE)
          { HERROR(DFE_ARGS); return(FAIL); }

	w = &(vs->wlist);
	r = &(vs->rlist);
	hsize = vs->wlist.ivsize; 		/* size as stored in HDF */

	/* alloc space (Vtbuf) for reading in the raw data from vdata */
        if(Vtbufsize < nelt * hsize) {
          Vtbufsize = nelt * hsize;
          if(Vtbuf) HDfreespace(Vtbuf);
          if((Vtbuf = (BYTE *) HDgetspace ( Vtbufsize )) == NULL) {
            HERROR(DFE_NOSPACE);
            return(FAIL);
          }
        }

	/* ================ start reading ============================== */
	/* ================ start reading ============================== */

	nv = QQread (vs->aid, nelt * hsize, (uint8*) Vtbuf);

	if ( nv != nelt*hsize ) {
          HERROR(DFE_READERROR);
          HEreport("Tried to read %d, only read %d", nelt * hsize, nv);
          return FAIL;
        }

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

		Cases (A)-(D) handles multiple fields of order 1.
		Case (E) handles one field of order>1. Interlace is
		irrelevant in this case. The case where there are 
		multiple fields of order>1 is prevented from
		existing by VSsetfields explicitly checking for this.
				
	*/

	/* ----------------------------------------------------------------- */
	/* CASE  (E): Special Case For one field with order >1 only */

	if (w->n == 1 && w->order[0] > 1) {
		b1 = buf;
		b2 = Vtbuf;
		esize = w->esize[0];
		isize = w->isize[0];
		type = w->type[0];

		if (vjv) {
                  sprintf(sjs,">> R  SPECIAL (E) order=%d esiz=%d isiz=%d nelt=%ld vsize %d\n",
                          w->order[0], esize, isize, nelt , hsize); zj;
                  sprintf(sjs,"@VSREAD: type is %ld\n", type); zj;
		}

		/* Errr WORKS */
		DFKsetNT(type); 
		Knumin (b2, b1, (uint32) w->order[0] * nelt, 
                        (uint32) isize/w->order[0],
                        (uint32) esize/w->order[0]);
    } /* case (e) */

	/* ----------------------------------------------------------------- */
	/* CASE  (A):  user=none, vdata=full */

	else if (interlace==NO_INTERLACE && vs->interlace==FULL_INTERLACE) {
		if (vjv) { sprintf(sjs,">> R  CASE  (A): iu=none, iv=full \n"); zj;}

		b1 = buf;
		for (j=0;j<r->n;j++) {
			i     = r->item[j];
			b2    = Vtbuf + w->off[i];
			type  = w->type[i];
            isize = w->isize[i];        /* QAK */
			esize = w->esize[i];

			/* Arrr ? */
			DFKsetNT(type); 
			Knumin (b2, b1, (uint32) w->order[0] * nelt, 
                                (uint32) isize/w->order[0],
                                (uint32) esize/w->order[0]);
			b1 += (nelt * esize);
		}
    } /* case (a) */

	/* ----------------------------------------------------------------- */
	/* CASE  (B):  user=none, vdata=none */
	else if (interlace==NO_INTERLACE && vs->interlace==NO_INTERLACE) {
		if (vjv) { sprintf(sjs,">> R  CASE  (B):  iu=none, iv=none\n"); zj; }

		b1 = buf;
		for (j=0;j<r->n;j++) {
			i     = r->item[j];
			b2    = Vtbuf + w->off[i] * nelt;
			type  = w->type[i];
			esize = w->esize[i];
			isize = w->isize[i];

			/* Brrr ? */
			DFKsetNT(type); 
            Knumin (b2, b1, (uint32) nelt, (uint32) isize, (uint32) esize);
			b1 += (nelt * esize);
		}
	} /* case (b) */

	/* ----------------------------------------------------------------- */
	/* CASE  (C):  iu=full, iv=full */
	else if (interlace==FULL_INTERLACE && vs->interlace==FULL_INTERLACE) {
		if (vjv) { sprintf(sjs,">> R  CASE  (C):  iu=full, iv=full\n"); zj; }

		for (uvsize=0, j=0;j<r->n;j++)
			uvsize += w->esize[r->item[j]];

		for (offset=0,j=0;j<r->n;j++) {
			i     = r->item[j];
			b1    = buf + offset;
			b2    = Vtbuf + w->off[i];
			type  = w->type[r->item[j]];
			esize = w->esize[i];
            /* isize = w->isize[i]; QAK */

			/* Crrr WORKS  */
			DFKsetNT(type); 
            Knumin (b2, b1, (uint32) nelt, (uint32) hsize, (uint32) uvsize);
			offset += esize;
        }
    } /* case (c) */


	/* ----------------------------------------------------------------- */
	/* CASE  (D):  user=full, vdata=none */
	else if(interlace==FULL_INTERLACE && vs->interlace==NO_INTERLACE) {

		if (vjv) { sprintf(sjs,">> R  CASE  (D):  iu=full, iv=none\n"); zj; }

		for (uvsize=0, j=0;j<r->n;j++)
			uvsize += w->esize[r->item[j]];

		for (offset=0,j=0;j<r->n;j++) {
			i     = r->item[j];
			b1    = buf + offset;
			b2    = Vtbuf + w->off[i] * nelt;
			type  = w->type[i];
			isize = w->isize[i];

			/* Drrr ? */
			DFKsetNT(type); 
			Knumin (b2, b1, (uint32) nelt, 
                                (uint32) isize, 
                                (uint32) uvsize);
			offset +=isize;
		}
	} /* case (d) */
/*
    HDfreespace (tbuf);
*/
	return(nv/hsize);

} /* VSread */


/* ------------------------------------------------------------------ */
/* debugging routine */

#ifdef PROTOTYPE
void bytedump (char *ss, BYTE buf[], int32 n)
#else

void bytedump (ss, buf, n)
	char 		*ss;
	BYTE		buf[]; 
	int32 	n; 

#endif

{
  int32 i;
  printf("BYTEDUMP at %ld [%s %ld]: ",buf, ss, n);
  for(i=0;i<n;i++) printf(" %x", buf[i]);
  printf("\n");
  fflush(stdout);
}

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
PUBLIC int32 VSwrite (VDATA *vs, BYTE buf[], int32 nelt, int32 interlace)
#else
PUBLIC int32 VSwrite (vs, buf, nelt, interlace)

	VDATA 	*vs;
	int32		nelt;
	int32		interlace;
	BYTE		buf[];

#endif

{
	register int16	isize,esize,hsize;
	register BYTE 	*b1,*b2;
/*
	register BYTE	*tbuf;
*/
	int32 		j,type, offset;
	int32 		special;
        int32           position, new_size;
	VWRITELIST	*w;
	int32 		uvsize;		/* size of "element" as needed by user */
	char * 	FUNC = "VSwrite";

	if ((nelt <= 0) || (vs == NULL))       { HERROR(DFE_ARGS); return(FAIL); }
	if (vs->access != 'w') 	               { HERROR(DFE_BADACC); return(FAIL); }
        if ( -1L == vexistvs(vs->f,vs->oref) ) { HERROR(DFE_NOVS); return(FAIL); }
	if (vs->wlist.ivsize == 0)	 { HERROR(DFE_NOVS);
                                           HEreport("w: vsize 0. fields not set for write!");
                                           return(FAIL); }

	if (interlace != NO_INTERLACE && interlace != FULL_INTERLACE )
          { HERROR(DFE_ARGS); return(FAIL); }

	w = (VWRITELIST*) &vs->wlist;
	hsize = w->ivsize; 		/* as stored in HDF file */

	/* alloc space (Vtbuf) for writing out the data */
        if(Vtbufsize < nelt * hsize) {
          Vtbufsize = nelt * hsize;
          if(Vtbuf) HDfreespace(Vtbuf);
          if((Vtbuf = (BYTE *) HDgetspace ( Vtbufsize )) == NULL) {
            HERROR(DFE_NOSPACE);
            return(FAIL);
          }
        }

/*
    if((tbuf = (BYTE *) HDgetspace ( nelt * hsize)) == NULL) {
          HERROR(DFE_NOSPACE);
          return(FAIL);
        }
*/

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

		Cases (A)-(D) handles multiple fields of order 1.
		Case (E) handles one field of order>1. Interlace is
		irrelevant in this case. The case where there are 
		multiple fields of order>1 is prevented from
		existing by VSsetfields explicitly checking for this.

    --------------------------------------------------------------------- */
	/* CASE  (E): Special Case For one field with order >1 only */

	if (w->n == 1 && w->order[0] > 1) {
		b1    = buf;
		b2    = Vtbuf;
		esize = w->esize[0];
		isize = w->isize[0];
		type  = w->type[0];

		if (vjv) {
            sprintf(sjs,">> W SPECIAL(E) order=%d esiz=%d isiz=%d nelt=%ld vsiz=%d\n",
                        w->order[0], esize, isize, nelt, hsize); zj;
            sprintf(sjs,"@VSWRITE: type = %ld\n", type); zj;
		}

	/* Ewww WORKS */
        DFKsetNT(type); 
		  Knumout(b1, b2, (uint32) w->order[0] * nelt, 
                          (uint32) esize/w->order[0],
                          (uint32) isize/w->order[0]);
	} /* case (e) */


	/* ----------------------------------------------------------------- */
	/* CASE  (A):  user=none, vdata=full */
	else if (interlace==NO_INTERLACE && vs->interlace==FULL_INTERLACE) {

		if (vjv) {sprintf(sjs, ">> W  CASE  (A):  iu=none, iv=full \n"); zj;} 

		b1 = buf;
		for (j=0;j<w->n;j++) {
			b2    = Vtbuf + w->off[j];
			type  = w->type[j];
			esize = w->esize[j];

			/* Awww ? */
			DFKsetNT(type); 
			Knumout (b1, b2, (uint32) nelt, 
                                 (uint32) esize,
                                 (uint32) hsize);
			b1 += (nelt * esize);
		}

	} /* case (a) */

	/* --------------------------------------------------------------------- */
	/* CASE  (B):  user=none, vdata=none */
	else if (interlace==NO_INTERLACE && vs->interlace==NO_INTERLACE) {

		if (vjv) { sprintf(sjs,">> W  CASE  (B):  iu=none, iv=none\n"); zj; }

		b1 = buf;
		for (j=0;j<w->n;j++) {
			b2    = Vtbuf + w->off[j] * nelt;
			type  = w->type[j];
			esize = w->esize[j];
			isize = w->isize[j];

			/* Bwww ? works sometimes */
			DFKsetNT(type); 
			Knumout (b1, b2, (uint32) nelt, 
                                 (uint32) esize, 
                                 (uint32) isize);
			b1 += (nelt * esize);
		}

	} /* case (b) */

	/* ----------------------------------------------------------------- */
	/* CASE  (C):  user=full, vdata=full */
	else if (interlace==FULL_INTERLACE && vs->interlace==FULL_INTERLACE) {
		if (vjv) { sprintf(sjs, ">> W  CASE  (C):  iu=full, iv=full\n"); zj;}

		for (uvsize=0, j=0;j<w->n;j++)
			uvsize += w->esize[j];

		for (offset=0,j=0;j<w->n;j++) {
			b1    = buf + offset;
			b2    = Vtbuf + w->off[j];
			type  = w->type[j];
			esize = w->esize[j];
			isize = w->isize[j];

			/* Cwww WORKS  */
			DFKsetNT(type); 
			Knumout (b1, b2, (uint32) nelt, 
                                 (uint32) uvsize, 
                                 (uint32) hsize);
			offset += esize;
		}

	} /* case (c) */

	/* ----------------------------------------------------------------- */
	/* CASE  (D):  user=full, vdata=none */
	else if (interlace==FULL_INTERLACE && vs->interlace==NO_INTERLACE) {

		if (vjv) { sprintf(sjs, ">> W  CASE  (D):  iu=full, iv=none\n"); zj;}

		for (uvsize=0, j=0;j<w->n;j++)
			uvsize += w->esize[j];

		for (offset=0,j=0;j<w->n;j++) {
			b1    = buf  + offset;
			b2    = Vtbuf + w->off[j] * nelt;
			type  = w->type[j];
			isize = w->isize[j];
			esize = w->esize[j];

			/* Dwww ? */
			DFKsetNT(type); 
			Knumout (b1, b2, (uint32) nelt, 
                                 (uint32) uvsize, 
                                 (uint32) isize);
			offset +=esize;
		}
	} /* case (d) */


	/* ================ start writing ============================== */
	/* ================ start writing ============================== */

#if 1
	if (vs->aid == 0) { /* aid not allocated yet */
          vs->aid = QQstartwrite (vs->f, VSDATATAG, vs->oref, 
                                  (int32) nelt * hsize);
          if (vs->aid == FAIL) { HERROR(DFE_BADAID); return(FAIL); }
	}

	/* 
         * promote to link-block if vdata exists and is not already one 
         *  AND we are incresing its size
         */
        HQueryposition(vs->aid, &position);
        new_size = (position / vs->wlist.ivsize) + nelt;

	if (vs->nvertices > 0) {
          QQQueryspecial(vs->aid, &special);
          if (!special && (new_size > vs->nvertices)) { 
            QQendaccess(vs->aid);
            vs->aid = HLcreate(vs->f, VSDATATAG , vs->oref, 
                               VDEFAULTBLKSIZE, VDEFAULTNBLKS);
            /* seek back to correct point */
            j = QQseek(vs->aid, position, DF_START); 
            if(vjv) {sprintf(sjs,"promotion to LINK-BLOCK seek stat is %ld\n",j); zj;}
          }
        }
	  
	j = QQwrite (vs->aid,  nelt * hsize, (uint8*) Vtbuf);
	if (j != nelt * hsize) { 
          sprintf(sjs,"QQwrite of %ld : %ld bytes written\n", nelt * hsize,j); zj;
        }

/*
    HDfreespace(tbuf);
*/

        if(new_size > vs->nvertices) vs->nvertices = new_size;

	vs->marked = 1;
	return (nelt);
#endif

#if 0
	{{ /* THIS VERSION WITH VMBLOCKS */
		VMBLOCK * vm, *t;
		int32 vmsize;
		vmsize = nelt * hsize;
		/* sprintf(sjs, "VMBLOCK saved: size=%ld\n", vmsize); zj; */

        vm  = (VMBLOCK*) HDgetspace (sizeof(VMBLOCK));
		if (vm==NULL) { 
			sprintf(sjs,"VSwrite: alloc vmblock err\n"); zj; 
			return(0);
			}
		vm->mem 	= tbuf;
		vm->n 	= vmsize;
		vm->next = NULL;

		t = vs->vm;
		if (t == NULL) { vs->vm = vm; }
		else {
			t = vs->vm;
			while (t->next != NULL)  t= t->next;
			t->next = vm;
			}		

		vs->nvertices += nelt;
		vs->marked = 1;
		return (nelt);
		/* END OF VMBLOCK VERSION */ }}
#endif


} /* VSwrite */

/* ------------------------------------------------------------------ */
