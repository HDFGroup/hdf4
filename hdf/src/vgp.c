#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/11/24 17:43:26  chouck
Fixed memory over-write when VGroups have lots of members

 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/08/27  19:54:56  likkai
 * Vclose now returns an (intn) status.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*****************************************************************************
* 
* vgp.c
* Part of the vertex-set interface.
* VGROUPs are handled by routines in here.
*
*************************************************************************/

#include "vg.h"
#include "hfile.h"
/* ------------------------------------------------------------ */
/* 
*
* G L O B A L S 
*
* These are the *only* globals in the Vset interface!!  
* They are used only for debugging.  No globals in other files. 
* The 2 functions below turn debugging on and off.
*
*/

PRIVATE int32 Load_vfile
  PROTO((HFILEID f));

PRIVATE void Remove_vfile
  PROTO((HFILEID f));

PUBLIC	char 		sjs[200];
PUBLIC	int16   vjv	=	0;

/*
turn debug on 
*/
#ifdef PROTOTYPE
PUBLIC void	setjj (void) 	     
#else

PUBLIC void	setjj() 	     

#endif

{ 
	vjv=1; 
}

/*
turn debug off 
*/
#ifdef PROTOTYPE
PUBLIC void	setnojj (void) 	     
#else

PUBLIC void	setnojj() 	     
#endif

{ 
	vjv=0; 
}


/*
* -------------------------------------------------------------------- 
* PRIVATE  data structure and routines.
* 
* Info about all vgroups in the file are loaded into vgtab  at start;
* and the vg field set to NULL until that vgroup is attached,
* and reset back to NULL when that vgroup is detached. 
* Info about all vdatas in the file are loaded into vstab  at start;
* and the vs field set to NULL until that vdata is attached,
* and reset back to NULL when that vdata is detached. 
* -------------------------------------------------------------------- 
*/


PUBLIC vfile_t  vfile [MAX_FILE];

/* -------------------------- Load_vfile ------------------------ */
/*
  *** Only called by Vinitialize()  ***   

loads vgtab table with info of all vgroups in file f.
Will allocate a new vfile_t, then proceed to load vg instances.
RETURNS -1 if error or no more file slots available.
RETURNS 1 if ok.
*/

#ifdef PROTOTYPE
PRIVATE int32 Load_vfile (HFILEID f)    
#else

PRIVATE int32 Load_vfile (f)              

	HFILEID	f;

#endif

{
	vfile_t		  * vf;
	vginstance_t  * v;
	vsinstance_t  * w;
	int32 			aid, stat;
	uint16			tag, ref;
	char * FUNC = "Load_vfile";

	if (vjv) { sprintf(sjs,"@Load_vfile \n"); zj; }

	/* allocate a new vfile_t structure */
        vf = Get_vfile(f);
        if(!vf) return FAIL;

	/* load all the vg's  tag/refs from file */
	vf->vgtabn    = -1;
	vf->vgtabtail = &(vf->vgtab);

	vf->vgtab.ref      = -1;
	vf->vgtab.nattach  = -1;
	vf->vgtab.nentries = -1;
	vf->vgtab.vg       = NULL;
	vf->vgtab.next     = NULL;

	vf->vgtabn = 0;

	stat = aid = QQstartread(f, DFTAG_VG,  DFREF_WILDCARD);
	while (stat != FAIL) {
          QQQuerytagref (aid, &tag, &ref);
#ifdef DEBUG
          if (vjv) {
            sprintf(sjs,"#LOADVGDIR:FOUND (%d) <%d/%d>\n ",
                    vf->vgtabn, tag, ref); zj;
          }
#endif          
          if (!(v = (vginstance_t*) HDgetspace (sizeof(vginstance_t)))) {
            HERROR(DFE_NOSPACE);
            return(FAIL);
          }
          
          vf->vgtabtail->next  = v;
          vf->vgtabtail	     = v;
          v->next     = NULL;
          v->ref      = (intn) ref;
          v->vg       = (VGROUP*) NULL; /* ie not attached yet */
          v->nattach  = 0;
          v->nentries = 0;
          vf->vgtabn++;
          stat = QQnextread (aid, DFTAG_VG, DFREF_WILDCARD, DF_CURRENT);
	}
	QQendaccess (aid);


	/* load all the vs's  tag/refs from file */
	vf->vstabn    = -1;
	vf->vstabtail = &(vf->vstab);

	vf->vstab.ref      = -1;
	vf->vstab.nattach  = -1;
	vf->vstab.nvertices= -1;
	vf->vstab.vs       = NULL;
	vf->vstab.next     = NULL;

	vf->vstabn = 0;
	stat = aid = QQstartread(f, VSDESCTAG,  DFREF_WILDCARD);
	while (stat != FAIL) {

          QQQuerytagref (aid, &tag, &ref);
          
#ifdef DEBUG
          if (vjv) {
            sprintf(sjs,"#LOADVSDIR:FOUND (%d) <%d/%d>\n ",
                    vf->vstabn, tag,ref); zj;
          }
#endif
          if (!(w = (vsinstance_t*) HDgetspace (sizeof(vsinstance_t)))) {
            HERROR(DFE_NOSPACE);
            return(FAIL);
          }
          
          vf->vstabtail->next  = w;
          vf->vstabtail        = w;
          w->next     = NULL;
          w->ref      = (intn) ref;
          w->vs       = (VDATA*) NULL; /* ie not attached yet */
          w->nattach  = 0;
          w->nvertices= 0;
          vf->vstabn++;
          stat = QQnextread (aid, VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
	}
	QQendaccess (aid);

	/* file may be incompatible with vset version 2.x. Need to check it */
        if ( ((int32)0 == vf->vgtabn) && ((int32)0 == vf->vstabn) )
          if ( (int32)0 == vicheckcompat (f) ) { /* not compatible */
            sprintf(sjs,"********************************************\n\n"); zj;
            sprintf(sjs,"%cFile incompatible with vset version 2.0\n",7); zj;
            sprintf(sjs,"%cFirst use the utility vcompat on this file\n",7); zj;
            sprintf(sjs,"********************************************\n\n"); zj;
#if 0
            nvfile--; 	/* delete the structure for that file */
#endif
            HERROR(DFE_BADOPEN);
            HEreport("This file is incompatible with the current release");
            return(FAIL);
          }
        
	/* otherwise, success */
	return (SUCCEED);
        
} /* Load_vfile */

/* ---------------------------- Remove_vfile ------------------------- */
/*
  removes the file ptr from the vfile[] table. 
  *** Only called by Vfinish() ***
*/
#ifdef PROTOTYPE
PRIVATE void Remove_vfile (HFILEID f)
#else
PRIVATE void Remove_vfile (f)
	HFILEID f;
#endif
{
    vginstance_t *vginst, *vg1;
    vsinstance_t *vsinst, *vs1;
    vfile_t      *vf=NULL;
    
    char * FUNC = "Remove_vfile";
    
    
    /* Figure out what file to work on */
    vf = Get_vfile(f);
    
    if(vf == NULL) return;
    
    /* free vstab and vgtab link-list entries */
    vginst = vf->vgtab.next;
    while (vginst) {
        vg1 = vginst->next;
        if (vginst->vg)  {
            HDfreespace (vginst->vg);
            HDfreespace (vginst);
        }
        vginst = vg1;
    }
    
    vsinst = vf->vstab.next;
    while (vsinst) {
        vs1 = vsinst->next; 
        if (vsinst->vs) {
            HDfreespace (vsinst->vs);
            HDfreespace (vsinst);
        }
        vsinst = vs1; 
    }

    vf->vgtab.next = NULL;
    vf->vstab.next = NULL;

}  /* Remove_vfile */

/* ---------------------------- Vinitialize ------------------------- */
/* should be replaced by HDFopen() or such */
/* is called by DFvsetopen() only */

#ifdef PROTOTYPE
PUBLIC void Vinitialize (HFILEID f)
#else

PUBLIC void Vinitialize (f)
	HFILEID f;

#endif

{
    char * FUNC = "Vinitialize";
    
    Load_vfile (f);
    if (vjv) { sprintf(sjs,"@Vinitialize: f=%ld\n", f); zj; }
}

/* ---------------------------- Vfinish ------------------------- */
/* should be replaced by HDFclose() or such */
/* is called by DFvsetclose() only */

#ifdef PROTOTYPE
PUBLIC void Vfinish (HFILEID f)
#else

PUBLIC void Vfinish (f)
	HFILEID f;

#endif

{
    char * FUNC = "Vfinish";
    
    Remove_vfile (f);
    if (vjv) { sprintf(sjs,"@Vfinish: f=%ld\n", f); zj; }
}


/* ---------------------------- vginstance ----------------------------- */
/*
* Looks thru vgtab for vgid and return the addr of the vg instance
* where vgid is found.
* RETURNS NULL if error or not found.
* RETURNS vginstance_t pointer if ok.
*
*/

#ifdef PROTOTYPE
vginstance_t * vginstance (HFILEID f, uint16 vgid)
#else

vginstance_t * vginstance (f, vgid)
	HFILEID 	f;
	uint16 	vgid;

#endif

{
  register uintn ref;
  register vginstance_t * v;
  vfile_t      * vf;
  char * FUNC = "vginstance";
  
  if (NULL== (vf = Get_vfile(f))) HRETURN_ERROR(DFE_FNF, NULL);
  
  ref = (uintn) vgid;
  for(v = vf->vgtab.next; v; v = v->next) 
    if (v->ref == ref) return(v);
  
  HERROR(DFE_NOMATCH);
  return(NULL);
  
} /* vginstance */

/* ------------------------ vexistvg --------------------------- */
/* 
* Tests if a vgroup with id vgid is in the file's vgtab.
* returns FAIL if not found,
* returns TRUE if found.
*/

#ifdef PROTOTYPE
int32 vexistvg (HFILEID f, uint16 vgid)          
#else

int32 vexistvg (f, vgid)          
	HFILEID 	f;
	uint16 	vgid;

#endif

{
  char * FUNC = "vexistvg";
  
  if (NULL== (vginstance_t *) vginstance(f,vgid))
    return(FAIL);
  else
    return (TRUE);
  
} /* vexistvg */

/* ==================================================================== */
/*
* vpackvg() and vunpackvg() : Packing and unpacking routines.
* For use in retrieving and storing vgroups to/from the HDF file.
*
*	Fields of VGROUP  that gets stored in HDF as a DFTAG_VG data object:
*		int16		nvelt (no of entries )
*		char		vgname[MAXVGNAMELEN]
*     char     vgclass[MAXVGNAMELEN]
*		int16		tag[1..nvelt]		
*		int16		ref[1..nvelt]		
*/
/* ==================================================================== */

#define INT16SIZE 2
#define UINT16SIZE 2

/* ==================================================================== */
/* 
*	vpackvg
*	extracts fields from  a VGROUP struct vg and pack the fields
*  into array buf in preparation for storage in the HDF file.
*
*  NO RETURN VALUES.
*/

#ifdef PROTOTYPE
void vpackvg (VGROUP *vg, BYTE buf[], int32 *size)
#else

void vpackvg (vg, buf, size)
    VGROUP          *vg;    /* vgroup to be saved to file */
    BYTE            buf[];  /* buffer to receive the packed fields */
	int32         	*size;	/* the size of buf is returned here */

#endif

{
    register uint16 	i;
    register BYTE		*b, *bb;
    register uint16		uint16var;
    char * FUNC = "vpackvg";
    
    bb = &buf[0];
    
    /* save nvelt */
    b= bb;
    UINT16ENCODE(b,vg->nvelt);
    bb +=UINT16SIZE;
    
    /* save all tags */
    for(i = 0; i < vg->nvelt; i++) {
        b= bb;
        UINT16ENCODE(b,vg->tag[i]);
        bb +=UINT16SIZE;
    }
    
    /* save all refs */
    for(i = 0; i < vg->nvelt; i++) {
        b= bb;
        UINT16ENCODE(b,vg->ref[i]);
        bb +=UINT16SIZE;
    }
    
    /* save the vgnamelen and vgname - omit the null */
    b= bb;
    uint16var = HDstrlen(vg->vgname);
    UINT16ENCODE(b,uint16var);
    bb +=UINT16SIZE;
    
    HDstrcpy((char*) bb,vg->vgname);
    bb +=  HDstrlen(vg->vgname) ;
    
    /* save the vgclasslen and vgclass- omit the null */
    b= bb;
    uint16var = HDstrlen(vg->vgclass);
    UINT16ENCODE(b,uint16var);
    bb +=INT16SIZE;
    
    HDstrcpy((char*) bb,vg->vgclass);
    bb +=  HDstrlen(vg->vgclass) ;
    
    /* save the expansion tag/ref pair */
    b= bb;
    UINT16ENCODE(b,vg->extag);   /* the vg's expansion tag */
    bb +=UINT16SIZE;
    
    b= bb;
    UINT16ENCODE(b,vg->exref);   /* the vg's expansion ref */
    bb +=UINT16SIZE;
    
    /*  save the vg's version field */
    b= bb;
    UINT16ENCODE(b,vg->version);
    bb +=UINT16SIZE;
    
    /* save the vg's more field */
    b= bb;
    UINT16ENCODE(b,vg->more);
    bb +=UINT16SIZE;
    
    if (vjv) {
        sprintf(sjs,"vpackvg: vgname is [%s]\n",vg->vgname);
        zj;
    }
    
    /* returns the size of total fields saved */
    *size = (int32) (bb - buf) + 1;
    
    if (vjv) {
        sprintf(sjs,"#vpackvg: vg->nvelt=%d\n",vg->nvelt);
        zj;
    }
    
} /* vpackvg */

/* ==================================================================== */
/*
*	vunpackvg:
*	Unpacks the fields from a buf (ie a DFTAG_VG data object just 
*	read in from the HDF file), into a VGROUP structure vg.
*
* 	Will first zero out vg, unpack fields, then inits as much of 
*  vg as it can.
*
*	NO RETURN VALUES
*
*/

#ifdef PROTOTYPE
void vunpackvg (VGROUP *vg, BYTE buf[])      
#else
void vunpackvg (vg, buf)      
	VGROUP*    	  	vg;	/* vgroup to be loaded with file data */
	BYTE			buf[]; 	/* must contain a DFTAG_VG data object from file */
#endif

{

	register BYTE	  *b, *bb;
	register uint16   i;
	register uint16	  uint16var;
	char * FUNC = "vunpackvg";


	bb = &buf[0];

	/* retrieve nvelt */
	b = bb;
	UINT16DECODE(b,vg->nvelt);
	bb+=UINT16SIZE;

        vg->msize = vg->nvelt;
        vg->tag  = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));
        vg->ref  = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));

        if((vg->tag == NULL) || (vg->ref == NULL)) return;

	/* retrieve the tags */
	for (i = 0; i < vg->nvelt; i++) {
		b= bb;
		UINT16DECODE(b, vg->tag[i]);
		bb +=UINT16SIZE;
	}

	/* retrieve the refs */
	for (i = 0; i < vg->nvelt; i++) {
		b= bb;
		UINT16DECODE(b,vg->ref[i]);
		bb +=UINT16SIZE;
	}

	/* retrieve vgname (and its len)  */
	b= bb;
	UINT16DECODE(b,uint16var);
	bb +=UINT16SIZE;

        HIstrncpy(vg->vgname, (char*) bb, (int32) uint16var + 1);
	bb += uint16var;

	if (vjv) {
		sprintf(sjs,"vunpackvg: vgname is [%s]\n",vg->vgname);
		zj;
	}

	/* retrieve vgclass (and its len)  */
	b= bb;
	UINT16DECODE(b,uint16var);
	bb +=UINT16SIZE;
        
        HIstrncpy(vg->vgclass, (char*) bb, (int32) uint16var + 1);
	bb += uint16var;

	b = bb;
	UINT16DECODE(b,vg->extag); /* retrieve the vg's expansion tag */
	bb += UINT16SIZE;

	b = bb;
	UINT16DECODE(b,vg->exref); /* retrieve the vg's expansion ref */
	bb += UINT16SIZE;

	b = bb;
	UINT16DECODE(b,vg->version); /* retrieve the vg's version field */
	bb += UINT16SIZE;

	b = bb;
	UINT16DECODE(b,vg->more); /* retrieve the vg's more field */
	bb += UINT16SIZE;

} /* vunpackvg */


/* ----------------------------- Vattach --------------------------- */

/*
*	 Vattach:
*
*   attaches to an existing vgroup or creates a new vgroup.
*	 returns NULL if  error, else ptr to vgroup.
*
*	IGNORE accesstype. (but save it)  
*  if vgid == -1,
*	  create a NEW vg if vgdir is not full.
*	  Also set nattach =1, nentries=0.
*  if vgid +ve, 
*	  look in vgdir to see if already attached,
*	  if yes, incr nattach 
*	  if not, fetch from file. attach, set nattach=1, netries= val from file 
*
*	In any case, set marked flag to 0.
*/

#ifdef PROTOTYPE
PUBLIC VGROUP *Vattach (HFILEID f, int32 vgid, char *accesstype)
#else

PUBLIC VGROUP *Vattach (f, vgid, accesstype)
	int32 	vgid;		/* actual vgroup's vgid or -1 for new vgroup */
	char   	*accesstype; 	/* access mode */
	HFILEID	f; 		/* HDF file handle */

#endif

{
    VGROUP			*vg;
    int16           access;
    BYTE            * vgpack;
    vginstance_t	* v;
    vfile_t			* vf;
    char * FUNC = "Vattach";
    
    if (f == FAIL)             {HERROR(DFE_ARGS); return(NULL);}
    if (!(vf = Get_vfile(f)))  {HERROR(DFE_FNF); return(NULL);}
    
    if ( accesstype[0]=='R' || accesstype[0]=='r')      access = 'r';
    else if ( accesstype[0]=='W' || accesstype[0]=='w') access = 'w';
    else HRETURN_ERROR(DFE_BADACC, NULL);
    
    if (vgid == -1) {           /******* create a NEW vg in vgdir ******/
        
        if (access=='r') {
            HERROR(DFE_ARGS);
            return(NULL);
        }
        
        /* allocate space for vg, & zero it out */
        if ( (vg = (VGROUP*) HDgetspace (sizeof(VGROUP)) ) == NULL) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        
        /* initialize new vg */
        vg->msize = MAXNVELT;
        vg->tag   = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));
        vg->ref   = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));

        if((vg->tag == NULL) || (vg->ref == NULL)) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }

        vg->nvelt 	= 0;
        vg->vgname[0]	= '\0';
        vg->f           = f;
        vg->otag        = DFTAG_VG;
        vg->oref	= vnewref(f);  /* create a new unique ref for it */
        if( vg->oref == 0 ) { HERROR(DFE_NOREF); return(NULL); }
        
        vg->access  	= access;
        
        vg->marked	= 0;
        vg->vgclass[0]	= '\0';
        vg->extag	= 0;
        vg->exref	= 0;
        vg->more	= 0;
        vg->version	= VSET_VERSION;
        
        /* attach new vg to file's vgtab  */
        if ( NULL == (v = (vginstance_t*) HDgetspace (sizeof(vginstance_t)))) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        
        vf->vgtabtail->next = v;
        vf->vgtabtail       = v;
        vf->vgtabn++;
        v->next     = NULL;
        v->ref      = (intn) vg->oref;
        v->vg       = vg;
        v->nattach  = 1;
        v->nentries = 0;
        
        return(vg);
    }
    
    else { 		
          /******* access an EXISTING vg *********/
        uint32 len;
        
        if (NULL == (v= vginstance (f,(uint16)vgid))) {
            HERROR(DFE_NOMATCH);
            HEreport("Vgid (%d) is not in vgtab[]", vgid);
            return(NULL);
        }
        
        /*
         * vg already attached.  inc nattach and return existing ptr
         */
        if (v->vg != NULL) {  
            v->nattach++;
            return(v->vg);
        }
        
        /* else vg not attached, must fetch vg from file */
        
        len = Hlength(f, DFTAG_VG, (uint16) vgid);
        if (len == FAIL) return NULL;

        vgpack = (BYTE *) HDgetspace(len);
        if(vgpack == NULL) return NULL;

        if (QQgetelement(f, DFTAG_VG, (uint16)vgid, vgpack) == (int32)FAIL) {
            HERROR(DFE_NOMATCH);
            return(NULL);
        }
        
        /* allocate space for vg, & zero it out */
        
        if (NULL == (vg =(VGROUP*) HDgetspace (sizeof(VGROUP))) ) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }
        
        /* unpack vgpack into structure vg, and init  */
        
        vunpackvg(vg,vgpack);
        vg->f	     = f;
        vg->oref     = (uint16)vgid;
        vg->otag     = DFTAG_VG;
        vg->access   = access;
        vg->marked   = 0;
        
        /* attach vg to file's vgtab at the vg instance v */
        v->vg       = vg;
        v->nattach  = 1;
        v->nentries = vg->nvelt;
        HDfreespace(vgpack);
        
        return(vg);
    }
        
        
} /* Vattach */

/* ---------------------------- Vdetach ---------------------------- */
/* 
*	Vdetach
*	Detaches access to vg.    
*	NO RETURN VALUES
*
*  if marked flag is 1, write out vg to file.
*	if vg still has velts attached to it, cannot detach vg.
*	decr  nattach. if (nattach is 0), free vg from vg instance.
*	(check that no velts are still attached to vg before freeing)
*
*  if attached with read access, just return.
*
* after detach, set marked flag to 0.	
*
*/

#ifdef PROTOTYPE
PUBLIC void Vdetach (VGROUP *vg)
#else

PUBLIC void Vdetach (vg)
	VGROUP *vg;

#endif

{

  int32         vgpacksize;
  BYTE	        * vgpack;
  vginstance_t 	* v;
  char * FUNC = "Vdetach";
  
  if ((vg == NULL) || (vg->otag != DFTAG_VG)) {
    HERROR(DFE_ARGS);
    HEprint(stderr, 0);
    return;
  }
  
  /* locate vg's index in vgtab */
  if (NULL ==( v = (vginstance_t*) vginstance (vg->f, vg->oref))) {
    HERROR(DFE_NOVS);
    HEprint(stderr, 0);
    return;
  }
  
  /* update vgroup to file if it has write-access */
  
  /* if its marked flag is 1 */
  /* - OR - */
  /* if that vgroup is empty */
  if (vg->access == 'w') {
    if ((vg->nvelt == 0) || (vg->marked == 1)) {
      if (vjv) {
        sprintf(sjs,"@VDETACH: added %d entries in vg..update vg to file\n",
                vg->nvelt - v->nentries); zj;
      }

      vgpack = (BYTE *) HDgetspace((int32) sizeof(VGROUP) + vg->nvelt * 4);
      vpackvg(vg,vgpack,&vgpacksize);

      /* 
       *  For now attempt to blow away the old one.  This is a total HACK
       *    but the H-level needs to stabilize first
       */
      Hdeldd(vg->f, DFTAG_VG, vg->oref);
      if(vjv) {sprintf(sjs,"OLD VGROUP deleted.  Now writing again\n"); zj;}

      if(QQputelement(vg->f, DFTAG_VG, vg->oref, vgpack, vgpacksize) == FAIL) {
        HERROR(DFE_WRITEERROR);
        HEprint(stderr, 0);
      }
      HDfreespace(vgpack);
      vg->marked = 0;
/*      return;   */
    }
  }
  
  v->nattach--;
  if (vjv) { sprintf(sjs,"#Vdetach: nattach is now %d\n", v->nattach); zj; }
  
  if (v->nattach > 0)
    return;    /* ok */
  
  v->vg = NULL;             /* detach vg from vgdir */
  
  HDfreespace(vg->tag);
  HDfreespace(vg->ref);
  HDfreespace(vg);
  
  return; /* ok */
  
} /* Vdetach */


/* ------------------------------ Vinsert ----------------------------- */
/*
*	Vinsert
*  inserts a velt (vs or vg)  into a vg 
*	RETURNS entry position within vg (0 or +ve) or FAIL on error.
*
*	checks and prevents duplicate links.
*
* Since multiple files are now possible, check that both vg and velt
* are from the same file. else error.
*/

#ifdef PROTOTYPE
PUBLIC int32 Vinsert (VGROUP *vg, VDATA *velt)             
#else

PUBLIC int32 Vinsert (vg, velt)             
	VGROUP	*vg;
	VDATA 	*velt;			/* (VGROUP*) or (VDATA*), doesn't matter */

#endif

{
    uint16 u;
	char * FUNC = "Vinsert";

	if (vg == NULL || velt == NULL) {
          HERROR(DFE_BADPTR);
          return(FAIL);
        }

	if (vg->otag != DFTAG_VG) {
          HERROR(DFE_ARGS);
          return(FAIL);
        }

        if(vg->nvelt >= vg->msize) {
          HERROR(DFE_VGSIZE);
          return(FAIL);
        }

	if ( Get_vfile(vg->f) != Get_vfile(velt->f) ) {
          HERROR(DFE_DIFFFILES);
          return(FAIL);
        }

	/* check in vstab  or vgtab that velt actually exist in file */

	switch (velt->otag) {
		case VSDESCTAG:
			if (vexistvs (vg->f,velt->oref) == FAIL) 
                  {HERROR(DFE_NOVS); return(FAIL);}
			break;

		case DFTAG_VG:
			if (vexistvg (vg->f,velt->oref) == FAIL) 
                  {HERROR(DFE_NOVS); return(FAIL);}
			break;

      default:
          HERROR(DFE_ARGS);
          return(FAIL);
	} /* switch */

    /* check and prevent duplicate links */
    for(u = 0; u < vg->nvelt; u++)
        if ( (vg->tag[u] == velt->otag) && (vg->ref[u] == velt->oref) ) {
            HERROR(DFE_DUPDD);
            HEreport("Vinsert: duplicate link <%d/%d>", velt->otag,velt->oref);
            return(FAIL);
        }
    
    /* Finally, ok to insert */
    vinsertpair (vg, velt->otag, velt->oref);
    
    if (vjv) {
        sprintf(sjs,"#Vinsert:inserted <%d/%d> at nvelt=%d\n",
                velt->otag, velt->oref, vg->nvelt); zj;
    }
    
    vg->marked = TRUE;
    return(vg->nvelt - 1);
    
} /* Vinsert */

/* ----------------------------- Vflocate -------------------------------- */
/*
Checks to see if the given field exists in a vdata belonging to this vgroup.
If found, returns the ref of the vdata.
If not found, or error, returns FAIL
28-MAR-91 Jason Ng NCSA
*/

#ifdef PROTOTYPE
PUBLIC int32 Vflocate (VGROUP *vg, char *field)
#else

PUBLIC int32 Vflocate (vg, field)
	VGROUP * vg;
	char * field;
#endif

{
    int32   s;
    uint16  u;
	VDATA 	*vs;
	char * FUNC = "Vflocate";

    for (u=0;u<vg->nvelt;u++)  {
        if (vg->tag[u] != VSDESCTAG) continue;
        vs = (VDATA*) VSattach (vg->f,vg->ref[u],"r");
		if (vs==NULL) return (FAIL);
		s = VSfexist (vs, field);
		VSdetach (vs);
        if (s==1) return (vg->ref[u]); /* found. return vdata's ref */
	}

	return (FAIL); /* field not found */

} /* Vflocate */

/* ----------------------- Vinqtagref ------------------------------------- */
/*
* Checks whether the given tag/ref pair already exists in the vgroup.
* RETURNS TRUE if exist
* RETURNS FALSE if not.
* 28-MAR-91 Jason Ng NCSA
*/
#ifdef PROTOTYPE
PUBLIC int32 Vinqtagref (VGROUP *vg, int32 tag, int32 ref)
#else

PUBLIC int32 Vinqtagref (vg, tag, ref)
	VGROUP 	* vg;
	int32 	tag, ref;

#endif

{
    register intn   i;
    register uint16     ttag, rref;
    char * FUNC = "Vinqtagref";

    ttag = (uint16) tag;
    rref = (uint16) ref;

    for (i=0; i < vg->nvelt; i++)
        if ((ttag == vg->tag[i]) && (rref == vg->ref[i])) 
            return (TRUE); /* exist */

	return (FALSE); /* does not exist */

} /* Vinqtagref */

/* ------------------------- Vntagrefs ------------------------------- */
/*
* Returns the number (0 or +ve integer) of tag/ref pairs in a vgroup.
* If error, returns FAIL
* 28-MAR-91 Jason Ng NCSA.
*/

#ifdef PROTOTYPE
PUBLIC int32 Vntagrefs (VGROUP *vg)
#else

PUBLIC int32 Vntagrefs (vg)
	VGROUP * vg;

#endif

{
	char * FUNC = "Vntagrefs";

	return ( (vg->otag == DFTAG_VG) ? (int32) vg->nvelt : FAIL);

} /* Vntagrefs */

/* -------------------------- Vgettagrefs ----------------------------- */
/*
* Returns n tag/ref pairs from the vgroup into the caller-supplied arrays
* tagrarray and refarray.
* n can be any +ve number, but arrays must be this big.
* RETURNS the total number of (0 or +ve #)  tag/ref pairs returned.
* 28-MAR-91 Jason Ng NCSA.
*
* NOTE: Do not confuse with Vgettagref().
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Vgettagrefs (VGROUP *vg, int32 tagarray[], int32 refarray[], int32 n)
#else

PUBLIC int32 Vgettagrefs (vg, tagarray, refarray, n)
	VGROUP 	* vg;
	int32 	n;
	int32 tagarray[], refarray[];

#endif

{
    int32 i;
	char * FUNC = "Vgettagrefs";

    if (n > (int32)vg->nvelt)
        n = vg->nvelt;

    for (i=0; i<n; i++) {
		tagarray[i] = (int32) vg->tag[i];
		refarray[i] = (int32) vg->ref[i];
	}

    return (n);
} /* Vgettagrefs */

/* -------------------------- Vgettagref -------------------------------- */
/*
* Returns a specified tag/ref pair from the vgroup.
* User specifies an index. 
* RETURNS FAIL if OK.
* RETURNS SUCCEED if error.
* 12-MAY-91 Jason Ng NCSA.
*
* NOTE: Do not confuse with Vgettagrefs().
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Vgettagref (VGROUP *vg, int32 which, int32 *tag, int32 *ref)
#else

PUBLIC int32 Vgettagref (vg, which, tag, ref)
	VGROUP 	* vg;
	int32 	which;
	int32 	*tag, *ref; /* these are returned */

#endif

{
	char * FUNC = "Vgettagref";

	if (vg==NULL) return (FAIL);
    if (which < 0 || which > (int32)(vg->nvelt-1))
          return (FAIL); /* range err */

        *tag  = (int32) vg->tag[which];
        *ref  = (int32) vg->ref[which];
	return (SUCCEED); /* ok */

} /* Vgettagref */

/* ------------------------ Vaddtagref ---------------------------------- */
/*
* Inserts a tag/ref pair into the attached vgroup vg.
* First checks that the tag/ref is unique.
* If error, returns FAIL or tag/ref is not inserted.
* If OK, returns the total number of tag/refs in the vgroup (a +ve integer).
* 28-MAR-91 Jason Ng NCSA.
*/

#ifdef PROTOTYPE
PUBLIC int32 Vaddtagref (VGROUP *vg, int32 tag, int32 ref)
#else

PUBLIC int32 Vaddtagref ( vg, tag, ref)
	VGROUP 	* vg;
	int32 	tag, ref;

#endif

{
	int32  n;
	char * FUNC = "Vaddtagref";

	if (Vinqtagref (vg, tag, ref) == 1) {
          /* error, already exists */
          HERROR(DFE_DUPDD);
          return (FAIL);
        }
	n = vinsertpair (vg, (uint16) tag, (uint16) ref);
	return (n);

} /* Vaddtagref */

/* ------------------------ vinsertpair --------------------------------- */
/*
* Inserts a tag/ref pair into the attached vgroup vg.
* Expand the tag/ref space if necessary
* Returns the total number of tag/refs in the vgroup.
*/

#ifdef PROTOTYPE
int32 vinsertpair (VGROUP *vg, uint16 tag, uint16 ref)
#else

int32 vinsertpair ( vg, tag, ref)
	VGROUP		* vg;
	uint16		tag, ref;  	/* this MUST be uint16 -  private routine */

#endif

{
    char * FUNC = "vinsertpair";
    
    if(vg->nvelt >= vg->msize) {
        vg->msize *= 2;
        vg->tag  = (uint16 *) HDregetspace(vg->tag, vg->msize * sizeof(uint16));
        vg->ref  = (uint16 *) HDregetspace(vg->ref, vg->msize * sizeof(uint16));

        if((vg->tag == NULL) || (vg->ref == NULL)) {
            HERROR(DFE_NOSPACE);
            return(NULL);
        }  
    }
    
    vg->tag[vg->nvelt]   = tag;
    vg->ref[vg->nvelt]   = ref;
    vg->nvelt ++;

    vg->marked = TRUE;
    return ((int32) vg->nvelt);
}

/* ==================================================================== */
/* 
* 	Ventries
*	returns the no of entries (+ve integer) in the vgroup vgid.
*  vgid must be an actual id
*  RETURNS FAIL if error
*
*  undocumented
*
*/

#ifdef PROTOTYPE
int32 Ventries (HFILEID f, int32 vgid)    
#else

int32 Ventries (f, vgid)    
	HFILEID	f;
	int32  	vgid;

#endif

{
    BYTE	* vgpack;
    VGROUP 	vg;
    int32   	vgpacksize, len;
    char * FUNC = "Ventries";
    
    if (vgid < 1) {
        HERROR(DFE_ARGS);
        return(FAIL);
    }
    
    len = Hlength(f, DFTAG_VG, (uint16) vgid);
    if(len == FAIL) return FAIL;

    vgpack = (BYTE *) HDgetspace(len);
    if(vgpack == NULL) return FAIL;

    if (QQgetelement(f, DFTAG_VG, (uint16)vgid, vgpack) == FAIL) {
        sprintf(sjs,"@Ventries: cannot get vg from file\n"); zj;
        return (FAIL);
    }
    
    vunpackvg(&vg,vgpack);
    
    HDfreespace(vg.tag);
    HDfreespace(vg.ref);

    return( (int32) vg.nvelt);
    
} /* Ventries */

/* ==================================================================== */
/*
*	Vsetname
* 	gives a name to the VGROUP vg.
*
* NO RETURN VALUES.
*
*	truncates to max length of VGNAMELENMAX 
*/

#ifdef PROTOTYPE
PUBLIC void Vsetname (VGROUP *vg, char *vgname)   
#else

PUBLIC void Vsetname (vg, vgname)   
	VGROUP	*vg;
	char		*vgname;

#endif

{
  char * FUNC = "Vsetname";
  
  if (vg == NULL) return;
  if ( HDstrlen(vgname) > VGNAMELENMAX ) {
    HIstrncpy(vg->vgname, vgname, VGNAMELENMAX);
    vg->vgname[VGNAMELENMAX]='\0';
  } 
  else {
    HDstrcpy(vg->vgname, vgname);
  }
  vg->marked = TRUE;
  return;
  
} /* Vsetname */

/* ==================================================================== */
/*
*	Vsetclass
* 	assigns a class name to the VGROUP vg.
*
* NO RETURN VALUES.
*
*	truncates to max length of VGNAMELENMAX 
*/

#ifdef PROTOTYPE
PUBLIC void Vsetclass (VGROUP *vg, char *vgclass)         
#else

PUBLIC void Vsetclass (vg, vgclass)         
	VGROUP	*vg;
	char		*vgclass;

#endif

{
  char * FUNC = "Vsetclass";
  
  if (vg == NULL) return;
  if ( HDstrlen(vgclass) > VGNAMELENMAX) {
    HIstrncpy(vg->vgclass, vgclass,VGNAMELENMAX);
    vg->vgclass[VGNAMELENMAX]='\0';
  }
  else
    HDstrcpy(vg->vgclass, vgclass);
  vg->marked = TRUE;
  return;
  
} /* Vsetclass*/


/* -------------------------------- Visvg --------------------------------- */
/*
* 	Visvg
*	tests if an entry in the vgroup vg is a VGROUP, given the entry's id. 
*
*	RETURNS TRUE if so
*	RETURNS FALSE if not, or if error
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Visvg (VGROUP *vg, int32 id) 
#else

PUBLIC int32 Visvg (vg, id) 
	VGROUP	*vg;
	int32 	id;		/* valid id of the entry in question */
#endif

{
  register intn u;
  register uint16 ID;
  char * FUNC = "Visvg";
  
  ID = (uint16) id;

  for(u = 0; u < vg->nvelt; u++)
    if (vg->ref[u] == ID   &&   /* if the ids match, */
        vg->tag[u] == DFTAG_VG)     /* and it is a vgroup */
      return (TRUE);
  
  return (FALSE);
  
} /* Visvg */

/* ======================================================= */
/* 
*	Vgetid
*	
*	Given a vgroup's id, returns the next vgroup's id in the file f .
*	The call Vgetid(f,-1) returns the id of the FIRST vgroup in the file. 
*
*	RETURNS -1 if error
*	RETURNS the next vgroup's id (0 or +ve integer).
*
*	This id is actually the "ref" of the vgroup "tag/ref".
*/

#ifdef PROTOTYPE
PUBLIC int32 Vgetid (HFILEID f,int32 vgid)         
#else

PUBLIC int32 Vgetid (f, vgid)         
	int32 	vgid;					/* current vgid */
	HFILEID 	f;						/* HDF file handle */

#endif

{
	vginstance_t * v;
	vfile_t		* vf;
	char * FUNC = "Vgetid";

	if ( vgid < -1 ) {
          HERROR(DFE_ARGS);
          return(FAIL);
        }

	if (NULL==(vf = Get_vfile(f))) {
          HERROR(DFE_FNF);
          return(FAIL);
        }

	if (vjv) {
        sprintf(sjs,"#Vgetid:vgtabn= %ld vgid=%ld\n",vf->vgtabn,vgid);
		zj;
	}

	if (vgid == FAIL) {
       if (NULL == vf->vgtab.next)
          return (FAIL);
       else
          return( (vf->vgtab.next)->ref); /* rets 1st vgroup's ref */
	}

	/* look in vgtab for vgid */
	v = (vf->vgtab).next;
	while(NULL != v) {
        if(v->ref == (uint16)vgid) break;
		v = v->next;
	}
	if (v==NULL)
		return (FAIL); /* none found */
	else
		if( v->next ==NULL)
			return (FAIL); /* this is the last vg, no more after it */
		else
			return((v->next)->ref); /* success, return the next vg's ref */

} /* Vgetid */


/* ================================================================= */
/*
*	Vgetnext
*
*	Given the id of an entry from a vgroup vg, looks in vg for the next
*	entry after it, and returns its id.
*	The call Vgetnext (vg,-1) returns the id of the FIRST entry in the vgroup.
*
*  Vgetnext will look at only VSET elements in the vgroup.
*  To look at all links in a vgroup, use Vgettagrefs instead.
*
*	RETURNS -1 if error
*	RETURNS the id of the next entry( 0 or +ve integer)  in the vgroup.
*
*	This id is actually the "ref" of the entry's "tag/ref".
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Vgetnext (VGROUP *vg, int32 id)    
#else

PUBLIC int32 Vgetnext (vg, id)    
	VGROUP	*vg;
	int32 	id;		/* actual id of an entry in the vgroup vg */

#endif

{
    register intn  u;
    char * FUNC = "Vgetnext";
  
    if ((id < -1) || (vg == NULL) || (vg->otag != DFTAG_VG)) {
        HERROR(DFE_ARGS);
        return(FAIL);
    }
  
    if (vjv) { sprintf(sjs,"#Vgetnext:vg->nvelt is %d\n",vg->nvelt); zj; }
  
    if (vg->nvelt == 0) return(FAIL);             /* nothing in vg */
  
    if (id == -1) {
        if ((vg->tag[0] == DFTAG_VG) || (vg->tag[0]==VSDESCTAG))
            return(vg->ref[0]);       /* id of first entry */
    }
  
    /* look in vg for id */
    for(u=0;u<vg->nvelt;u++)
        if ((vg->tag[u] == DFTAG_VG) || (vg->tag[u]==VSDESCTAG)) {
            if(vg->ref[u] == (uint16)id) {
                if (u == (vg->nvelt - 1) )
                    return(FAIL);
                else  {
                    if ((vg->tag[u+1] == DFTAG_VG) || (vg->tag[u+1]==VSDESCTAG))
                        return(vg->ref[u+1]);     /* return the id of next entry */
                    else  return (FAIL);
                }
            }
        }
    
    return (FAIL);

} /* Vgetnext  */

/* ================================================================= */
/*
*	Vgetname
*	returns the vgroup's name
*   ASSUME that vgname has been allocated large enough to hold
*   the name
*
*/

#ifdef PROTOTYPE
PUBLIC void Vgetname (VGROUP *vg, char *vgname)          
#else

PUBLIC void Vgetname (vg, vgname)          
	VGROUP	*vg;
	char	*vgname;			/* its name is returned in this var */

#endif

{
	char * FUNC = "Vgetname";

    if (vg != NULL) HDstrcpy(vgname, vg->vgname);
	return;

} /* Vgetname */
/* ================================================================= */
/*
*	Vgetclass
*	returns the vgroup's class name 
*   ASSUME that vgclass has been allocated large enough to hold
*   the name
*
*/

#ifdef PROTOTYPE
PUBLIC void Vgetclass (VGROUP *vg, char *vgclass)  
#else

PUBLIC void Vgetclass (vg, vgclass)  
	VGROUP	*vg;
	char	*vgclass;	/* its class name is returned in this var */

#endif

{
  char * FUNC = "Vgetclass";
  
  if(vg && vgclass) HIstrncpy(vgclass, vg->vgclass, HDstrlen(vg->vgclass) + 1);
  return;

} /* Vgetclass*/

/* ================================================================= */
/*
*	Vinquire
*
*	General inquiry routine for VGROUP. 
*
*	output parameters:
*			nentries - no of entries in the vgroup
*			vgname	- the vgroup's name
*
*	RETURNS FAIL if error
*	RETURNS SUCCEED if ok
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Vinquire (VGROUP *vg, int32 *nentries, char *vgname)
#else

PUBLIC int32 Vinquire (vg, nentries, vgname)
	VGROUP	*vg;
	int32 	*nentries;
	char		*vgname;

#endif

{
  char * FUNC = "Vinquire";
  
  if ((vg == NULL) || (vg->otag != DFTAG_VG)) {
    HERROR(DFE_ARGS);
    return(FAIL);
  }

  HDstrcpy(vgname, vg->vgname);
  *nentries = vg->nvelt;
  
  return(SUCCEED); 

} /* Vinquire */

/* ================================================================= */

/* ---------------------------- Vopen ------------------------- */
/* 
* 
* This routine will replace the code segment " Hopen(); Vinitialize(f)".
* Thus, if Vopen is used, do not call Vinitialize after that.
* 
* Similar to Hopen().
* INPUTS: 
*		char * path     - file name.
*     int n  access   - type of access. See Hopen().
*     int16  ndds     - no. of dd blocks. See Hopen().
*
* This routine opens the HDF file and initializes it for Vset operations.
*
* RETURN VALUE:
*  if error:  -1 (FAIL).
*  if successful: the id of the file (>0).
*
* See also Vclose().
*
* By: Jason Ng 10 Aug 92
* 
*/
#ifdef OLD_WAY
#undef Hopen
#undef Hclose
#endif

#ifdef PROTOTYPE
PUBLIC HFILEID Vopen( char *path, intn access, int16 ndds)

#else
PUBLIC HFILEID Vopen (path, access, ndds)
char 		*path;
intn 		access;
int16 	ndds;

#endif

{
	char * FUNC = "Vopen";
	HFILEID  f;

  	f = Hopen(path, access, ndds);
   if (f==FAIL) return(FAIL);

	Vinitialize(f);
   return (f);

	}


/* ---------------------------- Vclose ------------------------- */
/* 
* 
* This routine will replace the code segment " Vfinish(f); Hclose(f);".
* Thus, if Vclose is used, do not call Vfinish before that.
*
* This routine closes the HDF file, after it has freed all memory and 
* updated the file.
* 
* INPUTS: 
*     int32   f       - if of HDF file to be closed.
*
* RETURN VALUE:  intn status - result of Hopen().
*
* See also Vopen().
*
* By: Jason Ng 10 Aug 92
* 
*/

#ifdef PROTOTYPE
PUBLIC intn Vclose (HFILEID f)

#else

PUBLIC intn Vclose (f)
	HFILEID f;
#endif

{
	char * FUNC = "Vclose";
   intn status;

	Vfinish (f);
	status = Hclose (f);

	return (status);

}

