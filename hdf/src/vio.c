#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/11/30 22:00:01  chouck
Added fixes for changing to Vstart and Vend

 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/10/06  16:19:19  chouck
 * In Vdatas version 2 LCOAL_INTS were stored as 16bits, not 32bits so
 * map_from_old_types() was messing up.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*****************************************************************************
*
* vio.c
* Part of the HDF Vset interface.
*
* VDATAs are handled by routines in here.
* PRIVATE functions manipulate vsdir and are used only within this file.
* PRIVATE data structures in here pertain to vdatas in vsdir only.
*
*************************************************************************/

#include "vg.h"
#include "hfile.h"
extern vfile_t vfile[];

/* ---------------------- DFvsetopen --------------------------------------- */
/*
    DFvsetopen and DFvsetclose
*/
#ifdef QAK
#undef Hopen
#undef Hclose

#undef DFopen
#undef DFclose
#endif

#ifdef VMS /* Redefine Hopen and Hclose for VMS linker */
#define Hclose _Hclose
#define Hopen _Hopen 
#endif /* VMS */

#ifdef PROTOTYPE
PUBLIC HFILEID  DFvsetopen (char *fname, int16 access, int16 defDDs)
#else

PUBLIC HFILEID  DFvsetopen (fname, access, defDDs)
	char 		*fname;
	int16		access, defDDs;

#endif

{
	HFILEID	f;
	char * FUNC = "DFvsetopen";

    f = Hopen (fname, access, defDDs);
    if(f == FAIL) return f;
    Vinitialize (f);
	return (f);
}
/* --------------------- DFvsetclose --------------------------------------- */
#ifdef PROTOTYPE
PUBLIC int32 DFvsetclose (HFILEID f)
#else

PUBLIC int32 DFvsetclose (f)
	HFILEID	f;

#endif

{
	int32 s;
	char * FUNC = "DFvsetclose";

	Vfinish(f);
    s = Hclose(f);
    return (s);
}

/* ------------------------------------------------------------------ */
/*
* Looks thru vstab for vsid and return the addr of the vdata instance
* where vsid is found.
* RETURNS NULL if error or not found.
* RETURNS vsinstance_t pointer if ok.
*
*/

#ifdef PROTOTYPE
vsinstance_t * vsinstance (HFILEID f, uint16 vsid)
#else

vsinstance_t * vsinstance (f,vsid)
	HFILEID	f;
	uint16 vsid;

#endif

{
  register uintn ref;
  register vsinstance_t * w;
  register vfile_t      * vf;
  char * FUNC = "vsinstance";
  
  if (!(vf = Get_vfile(f))) HRETURN_ERROR(DFE_FNF, NULL);
  
  ref = (uintn) vsid;
  for(w = vf->vstab.next; w; w = w->next) 
    if (w->ref == ref) return(w);
  
  return(NULL);
  
} /* vsinstance */

/* --------------------------- vexists --------------------------------------- */
/*
* Tests if a vdata with id vsid is in the file's vstab.
* returns FAIL if not found,
* returns TRUE if found.
*/

#ifdef PROTOTYPE
int32 vexistvs (HFILEID f, uint16 vsid)         
#else

int32 vexistvs (f, vsid)         
	HFILEID	f;
	uint16 vsid;

#endif

{
  char * FUNC = "vexistvs";
  
  if (NULL== vsinstance(f,vsid))
    return(FAIL);
  else
    return (TRUE);
  
} /* vexistvs */

/* ------------------------------------------------------------------ */
/*
The following 2 routines, vpackvs and vunpackvs, packs and unpacks
a VDATA structure into a compact form suitable for storing in the HDF file.
*/

/****
CONTENTS of VS stored in HDF file with tag VSDESCTAG:
	int16		interlace
	int32		nvertices
	int16		vsize
	int16		nfields

	int16		isize[1..nfields] (internal size of each field)
	int16		off[1..nfields] (internal offset of each field)
	char		fname[1..nfields][FIELDNAMELENMAX]
	char		vsname[VSNAMELENMAX]
****/


#define INT16SIZE 2
#define UINT16SIZE 2
#define INT32SIZE 4

/* ------------------------------- vpackvs ----------------------------------- */
/*
The following 2 PRIVATE routines, vpackvs and vunpackvs, packs and unpacks
a VDATA structure into a compact form suitable for storing in the HDF file.
*/

/****
CONTENTS of VS stored in HDF file with tag DFTAG_VH:
	int16		interlace
	int32		nvertices
	int16		vsize
	int16		nfields

	int16		isize[1..nfields] (internal size of each field)
	int16		off[1..nfields] (internal offset of each field)
	char		fname[1..nfields][FIELDNAMELENMAX]
	char		vsname[VSNAMELENMAX]
	char     vsclass[VSNAMELENMAX]

****/

/* 
convert a vs struct to a vspack suitable for storage in a HDF file 
*/

#ifdef PROTOTYPE
void vpackvs (VDATA *vs, BYTE buf[], int32 *size)
#else

void vpackvs (vs, buf, size)
	VDATA 	*vs;
	int32   	*size;
	BYTE		buf[];

#endif

{
	register int32          i;
	register BYTE		*b, *bb;
	register int16         int16var;
	char * FUNC = "vpackvs";

	bb = &buf[0];

	/* save the interlace */
	b = bb;
	INT16ENCODE(b,vs->interlace);
	bb+=INT16SIZE;

	/* save nvertices */
	b = bb;
	INT32ENCODE(b,vs->nvertices);
	bb+=INT32SIZE;

	/* save ivsize */
	b = bb;
	INT16ENCODE(b,vs->wlist.ivsize);
	bb+=INT16SIZE;

	/* save nfields */
	b = bb;
	INT16ENCODE(b,vs->wlist.n);
	bb+=INT16SIZE;

	for (i=0;i<vs->wlist.n;i++) { /* save the type */
		b = bb;
		INT16ENCODE(b,vs->wlist.type[i]);
		bb+=INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++) { /* save the isize */
		b = bb;
		INT16ENCODE(b,vs->wlist.isize[i]);
		bb+=INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++) { /* save the offset */
		b = bb;
		INT16ENCODE(b,vs->wlist.off[i]);
		bb+=INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++)  { /* save the order */
		b = bb;
		INT16ENCODE(b,vs->wlist.order[i]);
		bb+=INT16SIZE;
	}

	/* save each field length and name - omit the null */
	for (i=0;i<vs->wlist.n;i++) {
		b = bb;
        int16var = HDstrlen(vs->wlist.name[i]);
		INT16ENCODE(b,int16var);
		bb+=INT16SIZE;

        HDstrcpy((char*) bb, vs->wlist.name[i]);
        bb += HDstrlen(vs->wlist.name[i]);
	}

	/* save the vsnamelen and vsname - omit the null */
	b = bb;
    int16var = HDstrlen(vs->vsname);
	INT16ENCODE(b,int16var);
	bb+=INT16SIZE;

    HDstrcpy((char*) bb,vs->vsname);
    bb += HDstrlen(vs->vsname);

	/* save the vsclasslen and vsclass- omit the null */
	b = bb;
    int16var = HDstrlen(vs->vsclass);
	INT16ENCODE(b,int16var);
	bb+=INT16SIZE;

    HDstrcpy((char*) bb,vs->vsclass);
    bb += HDstrlen(vs->vsclass);

	/* save the expansion tag/ref pair */
	b= bb;
	UINT16ENCODE(b,vs->extag);
	bb +=UINT16SIZE;

	b= bb;
	UINT16ENCODE(b,vs->exref);
	bb +=UINT16SIZE;

	/* save the version field - init to version_2 now */
	b= bb;
	INT16ENCODE(b,vs->version);
	bb +=INT16SIZE;

	/* save the 'more' field - NONE now */
	b= bb;
	INT16ENCODE(b,vs->more);
	bb +=INT16SIZE;

    *size = (int32) (bb - buf) + 1;

} /* vpackvs */


/* ----------------------- map_from_old_types ------------------------------- */
/*
Convert an old type (i.e. LOCAL_INT to DFNT_ based types
*/
PRIVATE
#ifdef PROTOTYPE
int16 map_from_old_types(int16 type)
#else
int16 map_from_old_types(type)
     int16 type;
#endif
{

  switch(type) {
  case LOCAL_CHARTYPE:
    return DFNT_CHAR;
  case LOCAL_BYTETYPE:
    return DFNT_INT8;
  case LOCAL_SHORTTYPE:
  case LOCAL_INTTYPE:
    return DFNT_INT16;
  case LOCAL_LONGTYPE:
    return DFNT_INT32;
  case LOCAL_FLOATTYPE:
    return DFNT_FLOAT32;
  case LOCAL_DOUBLETYPE:
    return DFNT_FLOAT32;
  default:
    return type;
  }

} /* map_from_old_types */


/* ----------------------------- vunpackvs ------------------------------------- */
/* 
Convert a packed form(from HDF file)  to a VDATA structure.
This routine will also initalize the VDATA structure as much as it can.
*/

#ifdef PROTOTYPE
void vunpackvs (VDATA *vs, BYTE buf[], int32 *size)
#else

void vunpackvs (vs, buf, size)
	VDATA 	*vs;
	BYTE		buf[];
	int32   	*size;	/* UNUSED, but retained for compatibility with vpackvs */
#endif

{
	BYTE		*b, *bb;
	int32    i;
	int16		int16var;
	char * FUNC = "vunpackvs";

	i = *size; /* dum */

	bb = &buf[0];

	/* retrieve interlace */
	b = bb;
	INT16DECODE(b,vs->interlace);
	bb += INT16SIZE;

	/* retrieve nvertices */
	b = bb;
	INT32DECODE(b,vs->nvertices);
	bb += INT32SIZE;

	/* retrieve tore ivsize */
	b = bb;
	INT16DECODE(b,vs->wlist.ivsize);
	bb += INT16SIZE;

	/* retrieve nfields */
	b = bb;
	INT16DECODE(b,vs->wlist.n);
	bb += INT16SIZE;

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the type */
		b = bb;
		INT16DECODE(b,vs->wlist.type[i]);
		bb += INT16SIZE;
	}

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the isize */
		b = bb;
		INT16DECODE(b,vs->wlist.isize[i]);
		bb += INT16SIZE;
	}

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the offset */
		b = bb;
		INT16DECODE(b,vs->wlist.off[i]);
		bb += INT16SIZE;
	}

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the order */
		b = bb;
		INT16DECODE(b,vs->wlist.order[i]);
		bb += INT16SIZE;
	}

	/* retrieve the field names (and each field name's length)  */
	for (i=0;i<vs->wlist.n;i++) {
		b = bb;
		INT16DECODE(b,int16var); /* this gives the length */
		bb += INT16SIZE;

        HIstrncpy(vs->wlist.name[i], (char*) bb, int16var + 1);
		bb += int16var;
	}

	/* retrieve the vsname (and vsnamelen)  */
	b = bb;
	INT16DECODE(b, int16var); /* this gives the length */
	bb += INT16SIZE;

    HIstrncpy(vs->vsname, (char*) bb, int16var + 1);
	bb += int16var;

	/* retrieve the vsclass (and vsclasslen)  */
	b = bb;
	INT16DECODE(b, int16var); /* this gives the length */
	bb += INT16SIZE;

    HIstrncpy(vs->vsclass, (char*) bb, int16var + 1);
	bb += int16var;

	/* retrieve the expansion tag and ref */
	b = bb;
	UINT16DECODE(b, vs->extag);
	bb += UINT16SIZE;

	b = bb;
	UINT16DECODE(b, vs->exref);
	bb += UINT16SIZE;

	/* retrieve the version field */
	b = bb;
	INT16DECODE(b, vs->version);
	bb += INT16SIZE;

	/* retrieve the 'more' field */
	b = bb;
	INT16DECODE(b, vs->more);
	bb += INT16SIZE;

        if(vs->version <= VSET_OLD_TYPES)
          for (i = 0; i < vs->wlist.n; i++) { /* save the type */
            vs->wlist.type[i] = map_from_old_types(vs->wlist.type[i]);
          }
        
	/* --- EXTRA --- fill in the machine-dependent size fields */
	for (i = 0; i < vs->wlist.n; i++) {
          vs->wlist.esize[i] = vs->wlist.order[i] * DFKNTsize(vs->wlist.type[i] | DFNT_NATIVE);
	}
        
} /* vunpackvs */

/* ------------------------------------------------------------------ */


/* ***************************************************************
   NEW VSattach: 
	(a)	if vsid == -1 
			if "r" access return error.
			if "w" access 
				create a new vs in vg and attach it.
				add to vsdir, set nattach= 1, nvertices = 0.

	(b)	if (vsid > 0)  
			if "r" access => look in vsdir
				if not found,
					fetch  vs from file, add to vsdir,  
					set nattach= 1, nvertices = val from file.
				if found,
					check access of found vs
					if "w" => being written, unstable! forbidden
					if "r" => ok. incr nattach.

			if "w" access => new data may be added BUT must be same format
					as existing vdata.
					(ie. VSsetfields must match old format exactly!!)

					Allows for seeks to write.

	in all cases, set the marked flag to 0.
	returns NULL if error.

   OLD VSattach: 
	if vsid == -1, then
	(a) if vg is "w", create a new vs in vg and attach it.
					add to vsdir, set nattach= 1, nvertices = 0.
	(b) if vg is "r", forbidden.
   if vsid is +ve, then
	(a) if vg is "w"  => new data may be added BUT must be same format
				as existing vdata.
				(ie. VSsetfields must match old format exactly!!)

	(b) if vg is "r"  => look in vsdir
				if not found,
					fetch  vs from file, add to vsdir,  
					set nattach= 1, nvertices = val from file.
				if found,
					check access of found vs
					if "w" => being written, unstable! forbidden
					if "r" => ok. incr nattach.
	
	in all cases, set the marked flag to 0.
	returns NULL if error.
   *************************************************************** */

#ifdef PROTOTYPE
PUBLIC VDATA * VSattach (HFILEID f, int32 vsid, char *accesstype) 
#else

PUBLIC VDATA * VSattach (f, vsid, accesstype) 
	HFILEID	f;
	int32 	vsid;
	char *	accesstype;

#endif

{
	VDATA 		*vs;  			 /* new vdata to be returned */
	int32 		vspacksize;
	BYTE			vspack[sizeof(VWRITELIST)];
	int32			access;
	vsinstance_t	* w;
	vfile_t			* vf;
	char * FUNC = "VSattach";

	if ((f == FAIL)  || (vsid < -1)) HRETURN_ERROR(DFE_ARGS, NULL);
	if (!(vf = Get_vfile(f)))        HRETURN_ERROR(DFE_FNF, NULL);

	if      ( accesstype[0]=='R' || accesstype[0]=='r') { access = 'r'; }
	else if ( accesstype[0]=='W' || accesstype[0]=='w') { access = 'w'; }
	else HRETURN_ERROR(DFE_BADACC, NULL);

	if (vjv) {
          sprintf(sjs,"#VSATTACH:vsid=%ld access=%s\n",vsid, accesstype); zj;
        }

	if (vsid == -1) {  /* ---------- VSID IS -1 ----------------------- */
          if (access == 'r') {
            HERROR(DFE_BADACC);
            HEreport("VSattach: may not read vsid of -1");
            return(NULL);
          }

          /* otherwise 'w' */
          /* allocate space for vs,  & zero it out  */
          if ( (vs= (VDATA*) HDgetspace (sizeof(VDATA))) == NULL)
            HRETURN_ERROR(DFE_NOSPACE, NULL);

          vs->nvertices = 0;
          vs->wlist.n = vs->rlist.n = 0;
          vs->islinked = FALSE;
          vs->nusym = 0;
          
          vs->oref		= vnewref(f);
          if (vs->oref == 0) {HERROR(DFE_NOREF); HDfreespace (vs); return(NULL);}
          
          vs->otag		= DFTAG_VH;
          vs->vsname[0] 	= '\0';
          vs->interlace		= FULL_INTERLACE; /* DEFAULT */
          vs->access		= 'w';
          vs->f			= f;
          vs->marked		= 0;
          
          vs->vsclass[0]	= '\0';
          vs->extag		= 0;
          vs->exref		= 0;
          vs->more			= 0;
          vs->version		= VSET_VERSION;
          
          vs->vm		= (VMBLOCK*) NULL;
          
          vs->aid		= 0;
          
          /* attach new vs to file's vstab */
          if ( NULL == (w = (vsinstance_t*) HDgetspace (sizeof(vsinstance_t))))
            HRETURN_ERROR(DFE_NOSPACE, NULL);
          
          vf->vstabtail->next = w;
          vf->vstabtail       = w;
          vf->vstabn++;
          
          w->next      = NULL;
          w->ref       = (intn) vs->oref;
          w->vs        = vs;
          w->nattach   = 1;
          w->nvertices = 0;
          
          vs->instance = w;

          return (vs);
	} /* end of case where vsid is -1 */

	/*  --------  VSID IS NON_NEGATIVE ------------- */

	if (access == 'r') { /* reading an existing vdata */

        if (NULL == (w =  vsinstance (f, (uint16) vsid)) )
            HRETURN_ERROR(DFE_VTAB, NULL);
          
        if (w->nattach && w->vs->access == 'r') {
            /* this vdata is already attached for 'r', ok to do so again */
            w->nattach++;
            return (w->vs);
          }
          
        if (w->vs) {
            /* use existing vs record */
            vs = w->vs;

        } else {

            /* allocate space for vs,  & zero it out  */
            if ( (vs=(VDATA*) HDgetspace (sizeof(VDATA))) == NULL)
              HRETURN_ERROR(DFE_NOSPACE, NULL);

        }

          /* need to fetch from file */
        if (Hgetelement(f, DFTAG_VH,(uint16)vsid,vspack) == FAIL)
            HRETURN_ERROR(DFE_NOVS, NULL);
          
        vs->wlist.n = vs->rlist.n = 0;

        /* unpack the vs, then init all other fields in it */
        vunpackvs (vs,vspack,&vspacksize);
        vs->otag    = DFTAG_VH;
        vs->oref    = (uint16)vsid;
        vs->access  = 'r';
        vs->f   = f;
        vs->marked  = 0;
        vs->nusym   = 0;

        vs->vm      = (VMBLOCK*) NULL; /* always NULL for "r" */

        vs->aid     = Hstartread(vs->f, VSDATATAG, vs->oref);
        if(vs->aid == FAIL) {
          HDfreespace(vs);
          HRETURN_ERROR(DFE_BADAID, NULL);
        }

        vs->instance = w;

        /* attach vs to vsdir  at the vdata instance w */
        w->vs        = vs;
        w->nattach   = 1;
        w->nvertices = vs->nvertices;
        return (vs);
 	} /* end of case where vsid is positive, and "r"  */


	if (access == 'w') { /* writing to an existing vdata */

          if ((w = vsinstance(f, (uint16) vsid)) == NULL) 
            HRETURN_ERROR(DFE_VTAB, NULL);
          
          if (w->nattach)  /* vdata previously attached before */
            RTNULL("VSattach: May not write to a previously attached vdata!");

          /* free old record (should reuse....) */
          if(w->vs) {
            vs = w->vs;
          } else {
            /* allocate space */
            if( (vs=(VDATA*) HDgetspace(sizeof(VDATA))) == NULL)
              HRETURN_ERROR(DFE_NOSPACE, NULL);
          }
          
          /* need to fetch from file */
          if (Hgetelement(f, DFTAG_VH, (uint16)vsid, vspack) == FAIL)
            HRETURN_ERROR(DFE_NOMATCH, NULL);
          
          vs->wlist.n = vs->rlist.n = 0;
          vs->nusym = 0;
          
          /* unpack the vs, then init all other fields in it */
          vunpackvs (vs,vspack,&vspacksize);
          vs->otag	= DFTAG_VH;
          vs->oref    = (uint16)vsid;
          vs->access	= 'w';
          vs->f		= f;
          vs->marked	= 0;
          vs->vm	= (VMBLOCK*) NULL; 
          
          vs->aid   = Hstartwrite(vs->f, VSDATATAG, vs->oref, 0);
          if(vs->aid == FAIL) { 
            HDfreespace(vs);
            HRETURN_ERROR(DFE_BADAID, NULL);
          }
          
          vs->instance = w;

          /* attach vs to vsdir  at the vdata instance w */
          w->vs        = vs;
          w->nattach   = 1;
          w->nvertices = vs->nvertices;
          
          return (vs);
          
	} /* end of case where vsid is positive, and "w"  */
	return (NULL);

} /* VSattach */

/* ------------------------ VSdetach ----------------------------- */

/* *************************************************************** 
 	Detach vs from vstab. 

	if vs has "w" access,   ( <=> only attached ONCE! )
		decr nattach.
		if (nattach is not  0)  => bad error in code.
		if nvertices (in vs) is 0) just free vs from vstab.

		if marked flag is 1
			write out vs to file and set marked flag to 0.
		   free vs from vsdir.

	if vs has "r" access,   ( <=> only attached ONCE! )
		decr nattach.
		if (nattach is 0)   just free vs from vstab.
			
   *************************************************************** */

#ifdef PROTOTYPE
PUBLIC void VSdetach (VDATA *vs)
#else

PUBLIC void VSdetach (vs)
	VDATA  *vs;

#endif

{
	int32			i, stat, vspacksize;
	BYTE			vspack[sizeof(VWRITELIST)];
	vsinstance_t	*w;
	char * FUNC = "VSdetach";

	if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          HEprint(stderr,0);
          return;
        }

	/* locate vs's entry in vstab */
	if (!(w = vs->instance)) {
          sprintf(sjs,"@VSdetach: vs not found\n"); zj;
          return;
	}

	w->nattach--;

	/* --- case where access was 'r' --- */
	if (vs->access =='r') {
          if (w->nattach == 0) {
#if 0
            w->vs = NULL; /* detach vs from vsdir */
            HDfreespace(vs);
#endif
          }
          Hendaccess (vs->aid);
          vs->aid = NO_ID;
          return;
	}

	/* --- case where access was 'w' --- */
	if (w->nattach != 0) {
		sprintf(sjs,"BUG IN VSET CODE. VSdetach on a vdata with 'w' access\n"); zj;
		sprintf(sjs,"nattach is %d (should be ZERO) \n",w->nattach); zj;
		}

	if (vs->marked)  { /* if marked , write out vdata's VSDESC to file */
        if(vs->nvertices==0) {sprintf(sjs,"VSdetach: Empty vdata detached\n"); zj; }
        vpackvs(vs,vspack,&vspacksize);
        stat = Hputelement (vs->f,VSDESCTAG,vs->oref,vspack,vspacksize);
        if (stat == FAIL)  RT("VSdetach: cannot write out vs");
        vs->marked = 0;
    }

	if (vjv && vs->nvertices<=0){
        sprintf(sjs,"#VSdetach:vs->nver=%ld!\n",vs->nvertices); zj;
	}

	/* remove all defined symbols */
    for (i=0;i<vs->nusym;i++) HDfreespace(vs->usym[i].name);
	vs->nusym = 0;

#if 0
		{{ /* THIS VERSION WITH VMBLOCKS */
                  VMBLOCK * t, *p;
                  int32 aid, stat, cursize, totalsize = 0;
                  BYTE * vwhole;
                  
                  /* count total byte size */
                  t = vs->vm;
                  while (t != NULL) { totalsize += t->n; t = t->next; }
                  if (vjv) {
                    sprintf(sjs,"VMBLOCKS total size = %ld\n", totalsize);zj;
                  }
                  vwhole = (BYTE*) HDgetspace( totalsize );
                  if (vwhole==NULL) {
                    sprintf(sjs,"VSdetach: no mem for VWHOLE\n"); zj;
                    return;
                  }
                  /* coalesce all VMBLOCKS into vwhole */
                  cursize = 0;
                  t = vs->vm;
                  while (t != NULL) { 
                    HDmemcpy(&vwhole[cursize], t->mem, t->n);
                    HDfreespace(t->mem);
                    cursize+= t->n;
                    t = t->next; 
                  }
                  /* free all VMBLOCKS */
                  t = vs->vm;
                  while (t != NULL) { p = t; t = t->next; HDfreespace(p); }
                  vs->vm = (VMBLOCK*) NULL;
                  
                  /* write out vwhole to file as 1 vdata */
                  if (vjv) {
                    sprintf(sjs,"---- QQstartwrite nbytes = %ld\n", totalsize);zj; }
                  stat = aid =QQstartwrite(vs->f,VSDATATAG,vs->oref, totalsize);
                  QQwrite(aid,  totalsize , vwhole);
                  QQendaccess (aid);
                  HDfreespace(vwhole);
                  /* END OF VMBLOCK VERSION */ 	}}
#endif

        Hendaccess (vs->aid);
#if 0
        w->vs = NULL; /* detach vs from vsdir */
        HDfreespace (vs);
#endif
	return;

} /* VSdetach */

/* -------------------------- VSappendable -------------------------------- */
/*
 * make it possible to append unlimitedly to an existing VData
 *
 *  Returns: SUCCEED, or FAIL for error
 *
 * undocumented 
 *
 */

#ifdef PROTOTYPE
PUBLIC int32 VSappendable (VDATA *vs)
#else

PUBLIC int32 VSappendable (vs)
	VDATA  *vs;

#endif

{
  int32		status;
  int32         blksize, curr_size;
  char * FUNC = "VSappendable";
  
  if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
    HERROR(DFE_ARGS);
    HEprint(stderr,0);
    return (FAIL);
  }

  curr_size = vs->nvertices * vs->wlist.ivsize;

  if(vs->nvertices && (curr_size > VDEFAULTBLKSIZE))
      blksize = curr_size;
  else
      blksize = VDEFAULTBLKSIZE;

  status = HLcreate(vs->f, VSDATATAG, vs->oref, blksize, VDEFAULTNBLKS);
  if(status == FAIL)
    return FAIL;

  Hendaccess(status);

  return SUCCEED;

} /* VSappendable */

/* -------------------------- Visvs -------------------------------- */

/* Visvs
*  checks if an id in a vgroup refers to a VDATA
*  RETURNS 1 if so
*  RETURNS 0 if not, or if error.
*/

#ifdef PROTOTYPE
PUBLIC int32 Visvs (VGROUP *vg, int32 id)       
#else

PUBLIC int32 Visvs (vg, id)       
	VGROUP  *vg;
	int32   id;

#endif

{
  register intn i;
  char * FUNC = "VSisvs";

  i = vg->nvelt;
  while(i) 
    if (vg->ref[--i] == (uint16)id && vg->tag[i]==VSDESCTAG) return(1);
  
  return(0);
  
} /* Visvs */

/* ======================================================= */

/* 
returns the id of the next  VDATA from the file f .
(vsid = -1 gets the 1st vDATA). 
RETURNS -1 on error.
RETURNS vdata id (0 or +ve integer) 
*/

#ifdef PROTOTYPE
PUBLIC int32 VSgetid (HFILEID f, int32 vsid)
#else

PUBLIC int32 VSgetid (f, vsid)
	int32 	vsid;
	HFILEID	f;

#endif

{
	vsinstance_t   * w;
	vfile_t	       * vf;
	char * FUNC = "VSgetid";

	if (vsid < -1)            HRETURN_ERROR(DFE_ARGS, FAIL);
	if (!(vf = Get_vfile(f))) HRETURN_ERROR(DFE_FNF, FAIL);

	if (vjv) {
        sprintf(sjs,"#VSgetid:vstabn is %ld\n",vf->vstabn); zj;
	}

	if (vsid == -1) {
          if (NULL == vf->vstab.next)
            return (FAIL);
          else
            return((int32) (vf->vstab.next)->ref); /* rets 1st vdata's ref */
	}

	/* look in vstab  for vsid */

        if ((w = vsinstance(f, (uint16) vsid)) == NULL) 
          HRETURN_ERROR(DFE_VTAB, FAIL);

	if (w->next == NULL)
          return(FAIL);			/* this is the last vdata, no more after it */
	else
          return( (int32) (w->next)->ref);  /* success, ret the next vdata's ref */

} /* VSgetid */

/* ------------------------------------------------------------------ */
/*
zero out n bytes in array x.
*/
#if 0
#ifdef PROTOTYPE
void zerofill (BYTE x[], int16 n)    
#else

void zerofill (x, n)    
	BYTE 	x[];
	int16 n;

#endif
{
	for( ; n; n--) *x++ = 0;

} /* zerofill */
#endif

/* -------------- Return the version number of a VData----------------- */

PUBLIC 
#ifdef PROTOTYPE
int32 VSgetversion(VDATA *vs)
#else
int32 VSgetversion(vs)
     VDATA *vs;
#endif
{

  if(!vs) return 0;
  
  return (vs->version);

}
