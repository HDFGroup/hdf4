#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.25  1993/10/06 20:27:57  koziol
More compression fixed, and folded Doug's suggested change into VSappendable.

 * Revision 1.24  1993/09/30  19:05:35  koziol
 * Added basic compressing functionality for special tags.
 *
 * Revision 1.23  1993/09/28  18:05:04  koziol
 * Removed OLD_WAY & QAK ifdef's.  Removed oldspecial ifdef's for special
 * tag handling.  Added new compression special tag type.
 *
 * Revision 1.22  1993/09/02  21:15:50  chouck
 * Was losing AIDs in VSappendable()
 *
 * Revision 1.21  1993/08/19  16:45:49  chouck
 * Added code and tests for multi-order Vdatas
 *
 * Revision 1.20  1993/08/16  21:46:43  koziol
 * Wrapped in changes for final, working version on the PC.
 *
 * Revision 1.19  1993/07/23  20:49:20  sxu
 * Changed 'void' to 'VOID' VSdump, Vinitialize, Vsetzap, Remove_vfile and unpackvs.
 *
 * Revision 1.18  1993/07/15  01:26:26  koziol
 * Final Whammy on the VSet memory Leak bug, mostly for maintenance purposes
 *
 * Revision 1.17  1993/07/14  20:53:04  chouck
 * Plugged memory leak on Vdelete() and VSdelete()
 *
 * Revision 1.16  1993/07/14  11:55:51  koziol
 * Fixed memory leaks in freeing trees
 *
 * Revision 1.15  1993/04/22  16:46:58  koziol
 * Fixed a type in VSappendable
 *
 * Revision 1.14  1993/04/22  16:05:54  chouck
 * Minor Vset fixes
 *
 * Revision 1.13  1993/04/19  22:48:36  koziol
 * General Code Cleanup to reduce/remove errors on the PC
 *
 * Revision 1.12  1993/04/15  19:18:48  koziol
 * Fixed bug introduced into the tbbt routines with the last bugfix (sigh)
 *
 * Revision 1.11  1993/04/14  23:09:59  koziol
 * Added counts to the tbbt trees and fixed a bug in VSdelete()
 *
 * Revision 1.10  1993/04/14  21:39:31  georgev
 * Had to add some VOIDP casts to some functions to make the compiler happy.
 *
 * Revision 1.9  1993/04/13  16:50:27  georgev
 * Casting problems on SGI's for two calls to the new balanced tree stuff.
 *
 * Revision 1.8  1993/04/08  18:33:59  chouck
 * Various Vset modifications (additions of Vdelete and VSdelete)
 *
 * Revision 1.7  1993/04/06  17:23:46  chouck
 * Added Vset macros
 *
 * Revision 1.6  1993/03/29  16:50:43  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.5  1993/01/19  05:56:27  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.4  1992/11/30  22:00:01  chouck
 * Added fixes for changing to Vstart and Vend
 *
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

/* Private Function Prototypes */
PRIVATE VOID vunpackvs
    PROTO((VDATA *vs, uint8 buf[], int32 *size));

/* External (within Vset routines) variables */
extern vfile_t vfile[];

/* ------------------------------------------------------------------ */
/*
* Looks thru vstab for vsid and return the addr of the vdata instance
* where vsid is found.
* RETURNS NULL if error or not found.
* RETURNS vsinstance_t pointer if ok.
*
*/
#ifdef PROTOTYPE
vsinstance_t _HUGE * vsinstance (HFILEID f, uint16 vsid)
#else
vsinstance_t _HUGE * vsinstance (f,vsid)
HFILEID f;
uint16 vsid;
#endif
{
    VOIDP *t;
    register vfile_t      * vf;
    int32 key;
    char * FUNC = "vsinstance";
  
    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF, NULL);
  
    /* tbbtdfind returns a pointer to the vsinstance_t pointer */
    key=VSSLOT2ID(f,vsid);
    t=(VOIDP *)tbbtdfind(vf->vstree,(VOIDP)&key,NULL);
    if(t!=NULL)
        return((vsinstance_t *)*t);     /* return the actual vsinstance_t ptr */
  
    HERROR(DFE_NOMATCH);
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
HFILEID f;
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


/* ------------------------------- vpackvs ----------------------------------- */
/*
The following 2 PRIVATE routines, vpackvs and vunpackvs, packs and unpacks
a VDATA structure into a compact form suitable for storing in the HDF file.
*/

/****
CONTENTS of VS stored in HDF file with tag DFTAG_VH:
    int16       interlace
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
void vpackvs (VDATA *vs, uint8 buf[], int32 *size)
#else
void vpackvs (vs, buf, size)
VDATA   *vs;
int32       *size;
uint8        buf[];
#endif
{
    register int32      i;
    register uint8      *bb;
	char * FUNC = "vpackvs";

	bb = &buf[0];

	/* save the interlace */
    INT16ENCODE(bb,vs->interlace);

	/* save nvertices */
    INT32ENCODE(bb,vs->nvertices);

	/* save ivsize */
    INT16ENCODE(bb,vs->wlist.ivsize);

	/* save nfields */
    INT16ENCODE(bb,vs->wlist.n);

    for (i=0;i<vs->wlist.n;i++)     /* save the type */
        INT16ENCODE(bb,vs->wlist.type[i]);

    /* save the isize */
    for (i=0;i<vs->wlist.n;i++)     
        INT16ENCODE(bb, vs->wlist.isize[i]);

    for (i=0;i<vs->wlist.n;i++)     /* save the offset */
        INT16ENCODE(bb,vs->wlist.off[i]);

    for (i=0;i<vs->wlist.n;i++)     /* save the order */
        INT16ENCODE(bb,vs->wlist.order[i]);

	/* save each field length and name - omit the null */
    for (i=0; i<vs->wlist.n; i++) {
        INT16ENCODE(bb,HDstrlen(vs->wlist.name[i]));

        HDstrcpy((char*) bb, vs->wlist.name[i]);
        bb += HDstrlen(vs->wlist.name[i]);
	}

	/* save the vsnamelen and vsname - omit the null */
    INT16ENCODE(bb,HDstrlen(vs->vsname));

    HDstrcpy((char*) bb,vs->vsname);
    bb += HDstrlen(vs->vsname);

	/* save the vsclasslen and vsclass- omit the null */
    INT16ENCODE(bb,HDstrlen(vs->vsclass));

    HDstrcpy((char*) bb,vs->vsclass);
    bb += HDstrlen(vs->vsclass);

	/* save the expansion tag/ref pair */
    UINT16ENCODE(bb,vs->extag);

    UINT16ENCODE(bb,vs->exref);

	/* save the version field - init to version_2 now */
    INT16ENCODE(bb,vs->version);

	/* save the 'more' field - NONE now */
    INT16ENCODE(bb,vs->more);

    *size = (int32) (bb - buf) + 1;
} /* vpackvs */


/* ----------------------- map_from_old_types ------------------------------- */
/*
Convert an old type (i.e. LOCAL_INT to DFNT_ based types
*/
PRIVATE
#ifdef PROTOTYPE
intn map_from_old_types(intn type)
#else
intn map_from_old_types(type)
intn type;
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
PRIVATE VOID vunpackvs (VDATA *vs, uint8 buf[], int32 *size)
#else
PRIVATE VOID vunpackvs (vs, buf, size)
VDATA   *vs;
uint8   buf[];
int32   *size;  /* UNUSED, but retained for compatibility with vpackvs */
#endif
{
    uint8   *bb;
    int32   i;
    int16   int16var;
	char * FUNC = "vunpackvs";

	i = *size; /* dum */

	bb = &buf[0];

	/* retrieve interlace */
    INT16DECODE(bb,vs->interlace);

	/* retrieve nvertices */
    INT32DECODE(bb,vs->nvertices);

	/* retrieve tore ivsize */
    INT16DECODE(bb,vs->wlist.ivsize);

	/* retrieve nfields */
    INT16DECODE(bb,vs->wlist.n);

    for (i=0;i <vs->wlist.n; i++)    /* retrieve the type */
        INT16DECODE(bb,vs->wlist.type[i]);

    for (i=0; i<vs->wlist.n; i++)    /* retrieve the isize */
        INT16DECODE(bb,vs->wlist.isize[i]);

    for (i=0; i<vs->wlist.n; i++)    /* retrieve the offset */
        INT16DECODE(bb,vs->wlist.off[i]);

    for (i=0; i<vs->wlist.n; i++)    /* retrieve the order */
        INT16DECODE(bb,vs->wlist.order[i]);

	/* retrieve the field names (and each field name's length)  */
    for (i=0; i<vs->wlist.n; i++) {
        INT16DECODE(bb,int16var); /* this gives the length */

        HIstrncpy(vs->wlist.name[i], (char*) bb, int16var + 1);
        bb += int16var;
    }
    
	/* retrieve the vsname (and vsnamelen)  */
    INT16DECODE(bb, int16var); /* this gives the length */

    HIstrncpy(vs->vsname, (char*) bb, int16var + 1);
	bb += int16var;

	/* retrieve the vsclass (and vsclasslen)  */
    INT16DECODE(bb, int16var); /* this gives the length */

    HIstrncpy(vs->vsclass, (char*) bb, int16var + 1);
	bb += int16var;

	/* retrieve the expansion tag and ref */
    UINT16DECODE(bb, vs->extag);
    UINT16DECODE(bb, vs->exref);

	/* retrieve the version field */
    INT16DECODE(bb, vs->version);

	/* retrieve the 'more' field */
    INT16DECODE(bb, vs->more);

    if(vs->version <= VSET_OLD_TYPES)
        for (i = 0; i < vs->wlist.n; i++)   /* save the type */
            vs->wlist.type[i] = map_from_old_types(vs->wlist.type[i]);
        
    /* --- EXTRA --- fill in the machine-dependent size fields */
    for (i = 0; i < vs->wlist.n; i++)
        vs->wlist.esize[i] = (int16) vs->wlist.order[i] * DFKNTsize(vs->wlist.type[i] | DFNT_NATIVE);
    
} /* vunpackvs */

/* ---------------------------- vsdestroynode ------------------------- */
/*
  Frees B-Tree nodes

  *** Only called by B-tree routines, should _not_ be called externally ***
*/
#ifdef PROTOTYPE
PUBLIC VOID vsdestroynode(VOIDP n)
#else
PUBLIC VOID vsdestroynode(n)
VOIDP n;
#endif
{
    VDATA       *vs;

    vs = ((vsinstance_t *)n)->vs;
    if(vs != NULL)
        HDfreespace((VOIDP)vs);

    HDfreespace((VOIDP)n);

}  /* vsdestroynode */

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
PUBLIC int32 VSattach (HFILEID f, int32 vsid, char *accesstype)
#else
PUBLIC int32 VSattach (f, vsid, accesstype)
HFILEID f;
int32   vsid;
char *  accesstype;
#endif
{
	VDATA 		*vs;  			 /* new vdata to be returned */
	int32 		vspacksize;
    uint8       *vspack;
    int32       access;
	vsinstance_t	* w;
	vfile_t			* vf;
	char * FUNC = "VSattach";

    if ((f == FAIL)  || (vsid < -1))
        HRETURN_ERROR(DFE_ARGS, FAIL);
    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF, FAIL);

    if ( accesstype[0]=='R' || accesstype[0]=='r')
        access = 'r';
    else if ( accesstype[0]=='W' || accesstype[0]=='w')
        access = 'w';
    else
        HRETURN_ERROR(DFE_BADACC, FAIL);

	if (vsid == -1) {  /* ---------- VSID IS -1 ----------------------- */
        if (access == 'r') {
            HERROR(DFE_BADACC);
            HEreport("VSattach: may not read vsid of -1");
            return(FAIL);
          }

          /* otherwise 'w' */
          /* allocate space for vs,  & zero it out  */
        if ( (vs= (VDATA*) HDgetspace (sizeof(VDATA))) == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);

        vs->nvertices = 0;
        vs->wlist.n = vs->rlist.n = 0;
        vs->islinked = FALSE;
        vs->nusym = 0;
          
        vs->oref      = vnewref(f);
        if (vs->oref == 0) {
            HERROR(DFE_NOREF);
            HDfreespace((VOIDP)vs);
            return(FAIL);
          }
          
        vs->otag      = DFTAG_VH;
        vs->vsname[0] = '\0';
        vs->interlace = FULL_INTERLACE; /* DEFAULT */
        vs->access    = 'w';
        vs->f         = f;
        vs->marked    = 0;
          
        vs->vsclass[0]= '\0';
        vs->extag     = 0;
        vs->exref     = 0;
        vs->more      = 0;
        vs->version   = VSET_VERSION;
          
        vs->vm        = (VMBLOCK*) NULL;
          
        vs->aid       = 0;
          
          /* attach new vs to file's vstab */
        if ( NULL == (w = (vsinstance_t*) HDgetspace (sizeof(vsinstance_t))))
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
          
        vf->vstabn++;
        w->key       = (int32) VSSLOT2ID(f,vs->oref); /* set the key for the node */
        w->ref       = (intn) vs->oref;
        w->vs        = vs;
        w->nattach   = 1;
        w->nvertices = 0;
        tbbtdins(vf->vstree,(VOIDP)w,NULL);    /* insert the vs instance in B-tree */
          
        vs->instance = w;

        return (w->key);
	} /* end of case where vsid is -1 */

	/*  --------  VSID IS NON_NEGATIVE ------------- */

	if (access == 'r') { /* reading an existing vdata */

        if (NULL == (w =  vsinstance (f, (uint16) vsid)) )
            HRETURN_ERROR(DFE_VTAB, FAIL);

        /* this vdata is already attached for 'r', ok to do so again */
        if (w->nattach && w->vs->access == 'r') {
            w->nattach++;
            return (w->key);
          }

        if (w->vs) {    /* use existing vs record */
            vs = w->vs;
        } else {

            /* allocate space for vs,  & zero it out  */
            if ( (vs=(VDATA*) HDgetspace (sizeof(VDATA))) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
        }

          /* need to fetch from file */
        if ( (vspack= (uint8 *) HDgetspace (sizeof(VWRITELIST))) == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
        if (Hgetelement(f,DFTAG_VH,(uint16)vsid,vspack) == FAIL) {
            HDfreespace((VOIDP)vspack);
            HRETURN_ERROR(DFE_NOVS, FAIL);
          } /* end if */

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
            HDfreespace((VOIDP)vs);
            HDfreespace((VOIDP)vspack);
            HRETURN_ERROR(DFE_BADAID, FAIL);
        }

        vs->instance = w;

        /* attach vs to vsdir  at the vdata instance w */
        w->vs        = vs;
        w->nattach   = 1;
        w->nvertices = vs->nvertices;

        HDfreespace((VOIDP)vspack);
        return (w->key);
 	} /* end of case where vsid is positive, and "r"  */


	if (access == 'w') { /* writing to an existing vdata */

        if ((w = vsinstance(f, (uint16) vsid)) == NULL)
            HRETURN_ERROR(DFE_VTAB, FAIL);

        if (w->nattach)  /* vdata previously attached before */
            HRETURN_ERROR(DFE_BADATTACH,FAIL);

          /* free old record (should reuse....) */
        if(w->vs) {
            vs = w->vs;
        } else {
            /* allocate space */
            if( (vs=(VDATA*) HDgetspace(sizeof(VDATA))) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
        }

          /* need to fetch from file */
        if ( (vspack= (uint8 *) HDgetspace (sizeof(VWRITELIST))) == NULL)
            HRETURN_ERROR(DFE_NOSPACE, FAIL);
        if (Hgetelement(f, DFTAG_VH, (uint16)vsid, vspack) == FAIL) {
            HDfreespace((VOIDP)vspack);
            HRETURN_ERROR(DFE_NOMATCH, FAIL);
          } /* end if */
          
        vs->wlist.n = vs->rlist.n = 0;
        vs->nusym = 0;
          
          /* unpack the vs, then init all other fields in it */
        vunpackvs (vs,vspack,&vspacksize);
        vs->otag  = DFTAG_VH;
        vs->oref    = (uint16)vsid;
        vs->access    = 'w';
        vs->f     = f;
        vs->marked    = 0;
        vs->vm    = (VMBLOCK*) NULL;

        vs->aid   = Hstartwrite(vs->f, VSDATATAG, vs->oref, 0);
        if(vs->aid == FAIL) {
            HDfreespace((VOIDP)vs);
            HDfreespace((VOIDP)vspack);
            HRETURN_ERROR(DFE_BADAID, FAIL);
        }

        vs->instance = w;

          /* attach vs to vsdir  at the vdata instance w */
        w->vs        = vs;
        w->nattach   = 1;
        w->nvertices = vs->nvertices;

        HDfreespace((VOIDP)vspack);
        return(w->key);
	} /* end of case where vsid is positive, and "w"  */
    return (FAIL);
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
PUBLIC void VSdetach (int32 vkey)
#else
PUBLIC void VSdetach (vkey)
int32 vkey;
#endif
{
	int32			i, stat, vspacksize;
    uint8            *vspack;
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSdetach";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        HEprint(stderr, 0);
        return;
    }

  /* locate vg's index in vgtab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        HEprint(stderr, 0);
        return;
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          HEprint(stderr,0);
          return;
        }

	w->nattach--;

	/* --- case where access was 'r' --- */
	if (vs->access =='r') {
#if 0
          if (w->nattach == 0) {
            w->vs = NULL; /* detach vs from vsdir */
            HDfreespace((VOIDP)vs);
          }
#endif
          Hendaccess (vs->aid);
          vs->aid = NO_ID;
          return;
	}

	/* --- case where access was 'w' --- */
	if (w->nattach != 0) {
        HERROR(DFE_CANTDETACH);
        return;
    }

	if (vs->marked)  { /* if marked , write out vdata's VSDESC to file */
        if(vs->nvertices==0) {
            /* sprintf(sjs,"VSdetach: Empty vdata detached\n"); zj; */
        }
        if ( (vspack= (uint8 *) HDgetspace (sizeof(VWRITELIST))) == NULL) {
            HERROR(DFE_NOSPACE);
            return;
          } /* end if */
        vpackvs(vs,vspack,&vspacksize);
        stat = Hputelement (vs->f,VSDESCTAG,vs->oref,vspack,vspacksize);
        HDfreespace((VOIDP)vspack);
        if (stat == FAIL) {
            HERROR(DFE_WRITEERROR);
            return;
        }
        vs->marked = 0;
    }

	/* remove all defined symbols */
    for(i=0; i<vs->nusym; i++)
        HDfreespace((VOIDP)vs->usym[i].name);
	vs->nusym = 0;

#if 0
		{{ /* THIS VERSION WITH VMBLOCKS */
                  VMBLOCK * t, *p;
                  int32 aid, stat, cursize, totalsize = 0;
                  uint8 * vwhole;
                  
                  /* count total byte size */
                  t = vs->vm;
                  while (t != NULL) {
                    totalsize += t->n;
                    t = t->next;
                  }
                  vwhole = (uint8*) HDgetspace( totalsize );
                  if (vwhole==NULL) {
                    HERROR(DFE_NOSPACE);
                    return;
                  }
                  /* coalesce all VMBLOCKS into vwhole */
                  cursize = 0;
                  t = vs->vm;
                  while (t != NULL) {
                    HDmemcpy(&vwhole[cursize], t->mem, t->n);
                    HDfreespace((VOIDP)t->mem);
                    cursize+= t->n;
                    t = t->next;
                  }
                  /* free all VMBLOCKS */
                  t = vs->vm;
                  while (t != NULL) { p = t; t = t->next; HDfreespace((VOIDP)p); }
                  vs->vm = (VMBLOCK*) NULL;

                  /* write out vwhole to file as 1 vdata */
                  stat = aid =Hstartwrite(vs->f,VSDATATAG,vs->oref, totalsize);
                  Hwrite(aid,  totalsize , vwhole);
                  Hendaccess (aid);
                  HDfreespace((VOIDP)vwhole);
                  /* END OF VMBLOCK VERSION */ 	}}
#endif

    Hendaccess (vs->aid);
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
PUBLIC int32 VSappendable (int32 vkey, int32 blk)
#else
PUBLIC int32 VSappendable (vkey, blk)
int32 vkey;
int32 blk;
#endif
{
    int32           status;
    int32           blksize, curr_size;
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSappendable";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        HEprint(stderr, 0);
        return(FAIL);
    }
  
    /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        HEprint(stderr, 0);
        return(FAIL);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          HEprint(stderr,0);
          return(FAIL);
    }
  
    curr_size = vs->nvertices * vs->wlist.ivsize;

#ifdef OLD_WAY
    if(vs->nvertices && (curr_size > VDEFAULTBLKSIZE))
        blksize = curr_size;
    else
        blksize = VDEFAULTBLKSIZE;

    if(blk && blk > blksize) blksize = blk;
#else
    if(blk>0)
	blksize=blk;
    else
        if(vs->nvertices && (curr_size > VDEFAULTBLKSIZE))
            blksize = curr_size;
        else
            blksize = VDEFAULTBLKSIZE;
#endif

    Hendaccess(vs->aid);

    vs->aid = HLcreate(vs->f, VSDATATAG, vs->oref, blksize, VDEFAULTNBLKS);
    if(vs->aid == FAIL)
        return FAIL;

    return SUCCEED;

} /* VSappendable */

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
int32   vsid;
HFILEID f;
#endif
{
	vsinstance_t   * w;
	vfile_t	       * vf;
    VOIDP *t;
    int32 key;
    char * FUNC = "VSgetid";

    if (vsid < -1)
        HRETURN_ERROR(DFE_ARGS, FAIL);
    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF, FAIL);

	if (vsid == -1) {
        if (NULL == (t=(VOIDP *)tbbtfirst((TBBT_NODE *)*(vf->vstree))))
            return (FAIL);
        else {
            w=(vsinstance_t *)*t;   /* get actual pointer to the vsinstance_t */
            return( w->ref); /* rets 1st vdata's ref */
          } /* end else */
	}

    /* tbbtdfind returns a pointer to the vsinstance_t pointer */
    key=VSSLOT2ID(f,vsid);
    t=(VOIDP *)tbbtdfind(vf->vstree,(VOIDP)&key,NULL);
    if(t==NULL)     /* couldn't find the old vsid */
        return(FAIL);
    else
        if (NULL==(t=(VOIDP *)tbbtnext((TBBT_NODE *)t)))      /* get the next node in the tree */
            return(FAIL);
        else {
            w=(vsinstance_t *)*t;   /* get actual pointer to the vsinstance_t */
            return(w->ref); /* rets vdata's ref */
          } /* end else */
} /* VSgetid */


/* -------------- Return the otag of a VData----------------- */
/*
  
  Return the 'otag' of the given Vdata
  Return FAIL on failure

*/

PUBLIC 
#ifdef PROTOTYPE
int32 VSQuerytag(int32 vkey)
#else
int32 VSQuerytag(vkey)
int32 vkey;
#endif
{
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSQuerytag";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        return(FAIL);
    }
  
    /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        return(FAIL);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          return(FAIL);
    }

    return ((int32) vs->otag);

} /* VSQuerytag */

/* -------------- Return the oref of a VData----------------- */
/*

  Return the ref of the given Vdata
  Return FAIL on failure

*/

PUBLIC 
#ifdef PROTOTYPE
int32 VSQueryref(int32 vkey)
#else
int32 VSQueryref(vkey)
int32 vkey;
#endif
{
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSQueryref";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        return(FAIL);
    }
  
  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        return(FAIL);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          return(FAIL);
    }

    return ((int32) vs->oref);

} /* VSQueryref */

/* -------------- Return the writelist of a VData----------------- */

#ifdef PROTOTYPE
VWRITELIST _HUGE *vswritelist(int32 vkey)
#else
VWRITELIST _HUGE *vswritelist(vkey)
int32 vkey;
#endif
{
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSgetversion";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        HEprint(stderr, 0);
        return(NULL);
    }
  
  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        HEprint(stderr, 0);
        return(NULL);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          HEprint(stderr,0);
          return(NULL);
    }

    return (&(vs->wlist));
}   /* end vswritelist() */

/* -------------- Return the version number of a VData----------------- */

PUBLIC 
#ifdef PROTOTYPE
int32 VSgetversion(int32 vkey)
#else
int32 VSgetversion(vkey)
int32 vkey;
#endif
{
    vsinstance_t    *w;
    VDATA           *vs;
    char * FUNC = "VSgetversion";

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        HEprint(stderr, 0);
        return(0);
    }
  
  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        HEprint(stderr, 0);
        return(0);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          HEprint(stderr,0);
          return(0);
    }
  
    return (vs->version);
}   /* end VSgetversion() */

/* ------------------------------- VSdelete -------------------------------- */
/*

  Remove a Vdata from its file.  This function will both remove the Vdata
  from the internal Vset data structures as well as from the file.

  (i.e. it calls tbbt_delete() and Hdeldd())

  Return FAIL / SUCCEED

*/
int32
#ifdef PROTOTYPE
VSdelete(int32 f, int32 vsid)
#else
VSdelete(f, vsid)
int32 f;
int32 vsid;
#endif
{
    VOIDP  	 v;
    vfile_t      * vf;
    VOIDP        * t;
    int32          key;
    char         * FUNC = "VSdelete";

    if(vsid < -1 ) {
        HERROR(DFE_ARGS);
        return(FAIL);
    } 

    if (NULL==(vf = Get_vfile(f))) {
        HERROR(DFE_FNF);
        return(FAIL);
    } 

    key=VSSLOT2ID(f,vsid);

    t = (VOIDP *)tbbtdfind(vf->vstree,(VOIDP)&key,NULL);

    if(t == NULL)
        return FAIL;

    v = tbbtrem((TBBT_NODE **)vf->vstree, (TBBT_NODE *)t, NULL);
    if(v) 
        vsdestroynode((VOIDP)v);

    Hdeldd(f, DFTAG_VS, (uint16) vsid);
    Hdeldd(f, DFTAG_VH, (uint16) vsid);

    return SUCCEED;
       
} /* VSdelete */
