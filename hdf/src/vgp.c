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

/*****************************************************************************
* 
* vgp.c
* Part of the vertex-set interface.
* VGROUPs are handled by routines in here.
*
*************************************************************************/

#include "vg.h"
#include "hfile.h"

PRIVATE intn Load_vfile
    (HFILEID f);

PRIVATE VOID Remove_vfile
    (HFILEID f);

PRIVATE vginstance_t *vginstance
    (HFILEID f, uint16 vgid);

PRIVATE void vunpackvg
    (VGROUP *vg, uint8 buf[]);

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

#if 0
PUBLIC vfile_t  vfile [MAX_VFILE] = {0};
#endif
PUBLIC vfile_t  *vfile = NULL;


/* -------------------------- Load_vfile ------------------------ */
/*
  *** Only called by Vinitialize()  ***   

loads vgtab table with info of all vgroups in file f.
Will allocate a new vfile_t, then proceed to load vg instances.
RETURNS FAIL if error or no more file slots available.
RETURNS SUCCEED if ok.
*/

PRIVATE intn Load_vfile (HFILEID f)
{
    vfile_t		  * vf;
    vginstance_t  * v;
    vsinstance_t  * w;
    int32 			aid, ret;
    uint16			tag, ref;
    CONSTR(FUNC,"Load_vfile");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    /* allocate a new vfile_t structure */
    if((vf = Get_vfile(f))==NULL)
        return FAIL;

    /* the file is already loaded (opened twice) do nothing */
    if(vf->access++) 
        return SUCCEED;

    /* load all the vg's  tag/refs from file */
    vf->vgtabn = 0;
    vf->vgtree = tbbtdmake(vcompare, sizeof(int32));
    if(vf->vgtree == NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
        
    ret = aid = Hstartread(f, DFTAG_VG,  DFREF_WILDCARD);
    while (ret != FAIL) {
        HQuerytagref (aid, &tag, &ref);
        if (NULL== (v = (vginstance_t*) HDgetspace (sizeof(vginstance_t)))) {
            tbbtdfree(vf->vgtree, vdestroynode, NULL);
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
          }
          
        vf->vgtabn++;
        v->key      = (int32) VGSLOT2ID(f,ref); /* set the key for the node */
        v->ref      = (intn) ref;
        v->vg       = (VGROUP*) NULL; /* ie not attached yet */
        v->nattach  = 0;
        v->nentries = 0;
        tbbtdins(vf->vgtree,(VOIDP)v,NULL);    /* insert the vg instance in B-tree */
        ret = Hnextread (aid, DFTAG_VG, DFREF_WILDCARD, DF_CURRENT);
	}
    Hendaccess (aid);

    /* load all the vs's  tag/refs from file */
    vf->vstabn = 0;
    vf->vstree = tbbtdmake(vcompare, sizeof(int32));
    if(vf->vstree==NULL) {
        tbbtdfree(vf->vgtree, vdestroynode, NULL);
        HRETURN_ERROR(DFE_NOSPACE,FAIL);
      } /* end if */

    ret = aid = Hstartread(f, VSDESCTAG,  DFREF_WILDCARD);
    while (ret != FAIL) {
        HQuerytagref (aid, &tag, &ref);
        if (NULL == (w = (vsinstance_t*) HDgetspace (sizeof(vsinstance_t)))) {
            tbbtdfree(vf->vgtree, vdestroynode, NULL);
            tbbtdfree(vf->vstree, vsdestroynode, NULL);
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
          }
          
        vf->vstabn++;
        w->key      = (int32) VSSLOT2ID(f,ref); /* set the key for the node */
        w->ref      = (intn) ref;
        w->vs       = (VDATA*) NULL; /* ie not attached yet */
        w->nattach  = 0;
        w->nvertices= 0;
        tbbtdins(vf->vstree,(VOIDP)w,NULL);    /* insert the vg instance in B-tree */
        ret = Hnextread (aid, VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
	}
    Hendaccess (aid);

	/* file may be incompatible with vset version 2.x. Need to check it */
    if ( ((int32)0 == vf->vgtabn) && ((int32)0 == vf->vstabn) )
        if ( (int32)0 == vicheckcompat (f) ) { /* not compatible */
#if 0
            nvfile--; 	/* delete the structure for that file */
#endif
            tbbtdfree(vf->vgtree, vdestroynode, NULL);
            tbbtdfree(vf->vstree, vsdestroynode, NULL);
            HRETURN_ERROR(DFE_BADOPEN,FAIL);
          }
        
	/* otherwise, success */
	return (SUCCEED);
} /* Load_vfile */

/* ---------------------------- Remove_vfile ------------------------- */
/*
  removes the file ptr from the vfile[] table. 
  *** Only called by Vfinish() ***
*/
PRIVATE VOID Remove_vfile (HFILEID f)
{
    vfile_t      *vf=NULL;
    CONSTR(FUNC,"Remove_vfile");
    
    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HERROR(DFE_NOSPACE);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    /* Figure out what file to work on */
    if((vf = Get_vfile(f)) == NULL)
        return;
    
    /* someone still has an active pointer to this file */
    if(--vf->access) 
        return;

    tbbtdfree(vf->vgtree, vdestroynode, NULL);
    tbbtdfree(vf->vstree, vsdestroynode, NULL);

	HDmemset(vf,0,sizeof(vfile_t));	/* reset values of structure */
}  /* Remove_vfile */

/* ---------------------------- vcompare ------------------------- */
/*
  Compares two B-tree keys for equality.  Similar to memcmp.

  *** Only called by B-tree routines, should _not_ be called externally ***
*/
PUBLIC intn vcompare(VOIDP k1,VOIDP k2,intn cmparg)
{
    /* shut compiler up */
    cmparg=cmparg;

    return((intn)((*(int32 *)k1) - (*(int32 *)k2)));  /* valid for integer keys */
}  /* vcompare */

/* ---------------------------- vprint ------------------------- */
/*
  Prints out the key and reference number of VDatas and Vgroups

  *** Only called by B-tree routines, should _not_ be called externally ***
*/
PUBLIC VOID vprint(VOIDP k1)
{
    printf("Ptr=%p, key=%d, ref=%d\n",k1,(int)((vginstance_t *)k1)->key,(int)((vginstance_t *)k1)->ref);
}  /* vprint */

/* ---------------------------- vdestroynode ------------------------- */
/*
  Frees B-Tree nodes

  *** Only called by B-tree routines, should _not_ be called externally ***
*/
PUBLIC VOID vdestroynode(VOIDP n)
{
    VGROUP       * vg;

    vg = ((vginstance_t *)n)->vg;
    if(vg != NULL) {
        HDfreespace((VOIDP)vg->tag);
        HDfreespace((VOIDP)vg->ref);
        HDfreespace((VOIDP)vg);
    }

    HDfreespace((VOIDP)n);

}  /* vdestroynode */

#ifdef NOTNEEDED
/* ---------------------------- vtfreekey ------------------------- */
/*
  Frees B-Tree index (actually doesn't anything at all)

  *** Only called by B-tree routines, should _not_ be called externally ***
*/
PUBLIC VOID vtfreekey(VOIDP k)
{
    k=k;    /* i.e. do nothing */
}  /* vtfreekey */
#endif

/* ---------------------------- Vinitialize ------------------------- */

PUBLIC intn Vinitialize(HFILEID f)
{
#ifdef LATER
    CONSTR(FUNC,"Vinitialize");
#endif

    return(Load_vfile (f));
}

/* ---------------------------- Vfinish ------------------------- */

PUBLIC intn Vfinish (HFILEID f)
{
#ifdef LATER
    CONSTR(FUNC,"Vfinish");
#endif
    
    Remove_vfile (f);
    return(SUCCEED);
}


/* ---------------------------- vginstance ----------------------------- */
/*
* Looks thru vgtab for vgid and return the addr of the vg instance
* where vgid is found.
* RETURNS NULL if error or not found.
* RETURNS vginstance_t pointer if ok.
*
*/
PRIVATE vginstance_t * vginstance (HFILEID f, uint16 vgid)
{
    VOIDP *t;
    vfile_t      * vf;
    int32 key;
    CONSTR(FUNC,"vginstance");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HRETURN_ERROR(DFE_NOSPACE, NULL);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    if (NULL== (vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF, NULL);

    /* tbbtdfind returns a pointer to the vginstance_t pointer */
    key=VGSLOT2ID(f,vgid);
    t=(VOIDP *)tbbtdfind(vf->vgtree,(VOIDP)&key,NULL);
    if(t!=NULL)
        return((vginstance_t *)*t);     /* return the actual vginstance_t ptr */

    HRETURN_ERROR(DFE_NOMATCH,NULL);
} /* vginstance */

/* ------------------------ vexistvg --------------------------- */
/* 
* Tests if a vgroup with id vgid is in the file's vgtab.
* returns FAIL if not found,
* returns TRUE if found.
*/
int32 vexistvg (HFILEID f, uint16 vgid)
{
#ifdef LATER
    CONSTR(FUNC,"vexistvg");
#endif
  
    if (NULL== (vginstance_t *) vginstance(f,vgid))
        return(FAIL);
    else
        return(TRUE);
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

/* ==================================================================== */
/* 
*	vpackvg
*	extracts fields from  a VGROUP struct vg and pack the fields
*  into array buf in preparation for storage in the HDF file.
*
*  NO RETURN VALUES.
*/

void vpackvg (VGROUP *vg, uint8 buf[], int32 *size)
{
	register uint16 	i;
    register uint8      *bb;
#ifdef LATER
	CONSTR(FUNC,"vpackvg");
#endif

	bb = &buf[0];

	/* save nvelt */
    UINT16ENCODE(bb,vg->nvelt);

	/* save all tags */
    for(i=0; i<vg->nvelt; i++)
        UINT16ENCODE(bb,vg->tag[i]);

	/* save all refs */
    for(i=0; i<vg->nvelt; i++)
        UINT16ENCODE(bb,vg->ref[i]);

	/* save the vgnamelen and vgname - omit the null */
    UINT16ENCODE(bb,HDstrlen(vg->vgname));

    HDstrcpy((char*) bb,vg->vgname);
    bb +=  HDstrlen(vg->vgname) ;

	/* save the vgclasslen and vgclass- omit the null */
    UINT16ENCODE(bb,HDstrlen(vg->vgclass));

    HDstrcpy((char*) bb,vg->vgclass);
    bb +=  HDstrlen(vg->vgclass) ;

	/* save the expansion tag/ref pair */
    UINT16ENCODE(bb,vg->extag);   /* the vg's expansion tag */
    UINT16ENCODE(bb,vg->exref);   /* the vg's expansion ref */

	/*  save the vg's version field */
    UINT16ENCODE(bb,vg->version);

	/* save the vg's more field */
    UINT16ENCODE(bb,vg->more);

	/* returns the size of total fields saved */
    *size = (int32) (bb - buf) + 1;
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

PRIVATE void vunpackvg (VGROUP *vg, uint8 buf[])
{
    register uint8   *bb;
    register uintn   u;
    register uint16  uint16var;
#ifdef LATER
    CONSTR(FUNC,"vunpackvg");
#endif
    
    bb = &buf[0];

    /* retrieve nvelt */
    UINT16DECODE(bb,vg->nvelt);
    
    vg->msize = (vg->nvelt > MAXNVELT ? vg->nvelt : MAXNVELT);
    vg->tag  = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));
    vg->ref  = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));
    
    if((vg->tag == NULL) || (vg->ref == NULL))
        return;
    
    /* retrieve the tags */
    for (u = 0; u < vg->nvelt; u++)
        UINT16DECODE(bb,vg->tag[u]);
    
    /* retrieve the refs */
    for (u = 0; u < vg->nvelt; u++)
        UINT16DECODE(bb,vg->ref[u]);

    /* retrieve vgname (and its len)  */
    UINT16DECODE(bb,uint16var);
    
    HIstrncpy(vg->vgname, (char*) bb, (int32) uint16var + 1);
    bb += uint16var;
    
    /* retrieve vgclass (and its len)  */
    UINT16DECODE(bb,uint16var);
    
    HIstrncpy(vg->vgclass, (char*) bb, (int32) uint16var + 1);
    bb += uint16var;
    
    UINT16DECODE(bb,vg->extag); /* retrieve the vg's expansion tag */
    UINT16DECODE(bb,vg->exref); /* retrieve the vg's expansion ref */
    
    UINT16DECODE(bb,vg->version); /* retrieve the vg's version field */
    
    UINT16DECODE(bb,vg->more); /* retrieve the vg's more field */
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

PUBLIC int32 Vattach (HFILEID f, int32 vgid, const char *accesstype)
{
	VGROUP			*vg;
    int16           acc_mode;
    uint8           * vgpack;
    vginstance_t    * v;
	vfile_t			* vf;
	CONSTR(FUNC,"Vattach");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    if (f == FAIL)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF,FAIL);

    if(tolower(accesstype[0])=='r')
        acc_mode = 'r';
    else if(tolower(accesstype[0])=='w')
        acc_mode = 'w';
    else
        HRETURN_ERROR(DFE_BADACC, FAIL);

    if (vgid == -1) {           /******* create a NEW vg in vgdir ******/
        if (acc_mode=='r')
            HRETURN_ERROR(DFE_ARGS,FAIL);

      /* allocate space for vg, & zero it out */
        if ( (vg = (VGROUP*) HDgetspace (sizeof(VGROUP)) ) == NULL)
            HRETURN_ERROR(DFE_NOSPACE,FAIL);

      /* initialize new vg */
        vg->msize = MAXNVELT;
        vg->tag   = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));
        vg->ref   = (uint16 *) HDgetspace(vg->msize * sizeof(uint16));

        if((vg->tag == NULL) || (vg->ref == NULL))
            HRETURN_ERROR(DFE_NOSPACE,FAIL);

        vg->nvelt   = 0;
        vg->vgname[0]   = '\0';
        vg->f           = f;
        vg->otag        = DFTAG_VG;
        vg->oref    = vnewref(f);  /* create a new unique ref for it */
        if( vg->oref == 0 )
            HRETURN_ERROR(DFE_NOREF,FAIL);

        vg->access    = acc_mode;

        vg->marked        = 0;
        vg->vgclass[0]    = '\0';
        vg->extag     = 0;
        vg->exref     = 0;
        vg->more          = 0;
        vg->version       = VSET_VERSION;

      /* attach new vg to file's vgtab  */
        if ( NULL == (v = (vginstance_t*) HDgetspace (sizeof(vginstance_t))))
            HRETURN_ERROR(DFE_NOSPACE,FAIL);

        vf->vgtabn++;
        v->key      = (int32) VGSLOT2ID(f,vg->oref); /* set the key for the node */
        v->ref      = (intn) vg->oref;
        v->vg       = vg;
        v->nattach  = 1;
        v->nentries = 0;
        tbbtdins(vf->vgtree,(VOIDP)v,NULL);    /* insert the vg instance in B-tree */

        return(v->key);     /* return key instead of VGROUP ptr */
	}
	else { 		
          /******* access an EXISTING vg *********/
        uint32 len;
          
        if (NULL == (v= vginstance (f,(uint16)vgid)))
            HRETURN_ERROR(DFE_NOMATCH,FAIL);
          
        /*
         * vg already attached.  inc nattach and return existing ptr
         */

        if (v->vg != NULL) {
            v->vg->access = MAX(v->vg->access, acc_mode);
            v->nattach++;
            return(v->key);     /* return key instead of VGROUP ptr */
        }
          
          /* else vg not attached, must fetch vg from file */
          
        len = Hlength(f, DFTAG_VG, (uint16) vgid);
        if (len == FAIL)
            return(FAIL);

        vgpack = (uint8 *) HDgetspace(len);
        if(vgpack == NULL)
            return(FAIL);


        if (Hgetelement(f, DFTAG_VG, (uint16)vgid, vgpack) == (int32)FAIL)
            HRETURN_ERROR(DFE_NOMATCH,FAIL);
          
          /* allocate space for vg, & zero it out */
          
        if (NULL == (vg =(VGROUP*) HDgetspace (sizeof(VGROUP))) )
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
          
        /* unpack vgpack into structure vg, and init  */
        vunpackvg(vg,vgpack);
        vg->f             = f;
        vg->oref            = (uint16)vgid;
        vg->otag      = DFTAG_VG;
        vg->access        = acc_mode;
        vg->marked        = 0;
          
        /* attach vg to file's vgtab at the vg instance v */
        v->vg             = vg;
        v->nattach        = 1;
        v->nentries    = vg->nvelt;
        HDfreespace((VOIDP)vgpack);
          
        return(v->key);     /* return key instead of VGROUP ptr */
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
PUBLIC int32 Vdetach (int32 vkey)
{
    VGROUP       *vg;
    int32         vgpacksize;
    uint8         * vgpack;
    vginstance_t  * v;
    CONSTR(FUNC,"Vdetach");

    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if ((vg == NULL) || (vg->otag != DFTAG_VG))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* update vgroup to file if it has write-access */

  /* if its marked flag is 1 */
  /* - OR - */
  /* if that vgroup is empty */
    if (vg->access == 'w') {
        if ((vg->nvelt==0) || (vg->marked == 1)) {
            vgpack = (uint8 *) HDgetspace((uint32) sizeof(VGROUP) + vg->nvelt * 4);
            vpackvg(vg,vgpack,&vgpacksize);

          /*
           *  For now attempt to blow away the old one.  This is a total HACK
           *    but the H-level needs to stabilize first
           */
            Hdeldd(vg->f, DFTAG_VG, vg->oref);

            if(Hputelement(vg->f, DFTAG_VG, vg->oref, vgpack, vgpacksize) == FAIL)
                HERROR(DFE_WRITEERROR);
            HDfreespace((VOIDP)vgpack);
            vg->marked = 0;
          }
      }
    v->nattach--;
    return(SUCCEED);
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

PUBLIC int32 Vinsert (int32 vkey, int32 insertkey)
{
    VGROUP *vg;
    vginstance_t  * v;
    vsinstance_t  * w;
    vginstance_t  * x;
    register uintn u;
    CONSTR(FUNC,"Vinsert");
    uint16 newtag=0, newref=0;
    int32 newfid;

    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    if (vg->otag != DFTAG_VG)
        HRETURN_ERROR(DFE_ARGS,FAIL);
    
    if (vg->otag != DFTAG_VG) {
        HERROR(DFE_ARGS);
        return(FAIL);
    }
    
    newfid = FAIL;
    if (VALIDVSID(insertkey)) {

        /* locate vs's index in vstab */
        if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(insertkey),(uint16)VSID2SLOT(insertkey))))
            HRETURN_ERROR(DFE_NOVS,FAIL);

        if (w->vs == NULL)
            HRETURN_ERROR(DFE_ARGS,FAIL);
     
        newtag = DFTAG_VH;
        newref = w->vs->oref;
        newfid = w->vs->f;

    } else {
        
        if(VALIDVGID(insertkey)) {
            
            /* locate vs's index in vgtab */
            if(NULL==(x=(vginstance_t*)vginstance(VGID2VFILE(insertkey),(uint16)VGID2SLOT(insertkey))))
                HRETURN_ERROR(DFE_NOVS,FAIL);

            if (x->vg == NULL)
                HRETURN_ERROR(DFE_ARGS,FAIL);

            newtag = DFTAG_VG;
            newref = x->vg->oref;
            newfid = x->vg->f;
            
        }

    }

    /* make sure we found something */
    if(newfid == FAIL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if (vg->f != newfid)
        HRETURN_ERROR(DFE_DIFFFILES,FAIL);

    /* check and prevent duplicate links */
    for(u = 0; u < vg->nvelt; u++)
        if((vg->ref[u] == newref) && (vg->tag[u] == newtag))
            HRETURN_ERROR(DFE_DUPDD,FAIL);

    /* Finally, ok to insert */
    vinsertpair(vg, newtag, newref);

    return(vg->nvelt - 1);
} /* Vinsert */

/* ----------------------------- Vflocate -------------------------------- */
/*
Checks to see if the given field exists in a vdata belonging to this vgroup.
If found, returns the ref of the vdata.
If not found, or error, returns FAIL
28-MAR-91 Jason Ng NCSA
*/
PUBLIC int32 Vflocate (int32 vkey, char *field)
{
    int32   s;
    register uintn  u;
    vginstance_t  * v;
    VGROUP *vg;
    int32 vskey;
    CONSTR(FUNC,"Vflocate");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);
    
    for (u = 0; u < vg->nvelt; u++)  {
        if(vg->tag[u]!=VSDESCTAG)
            continue;
        vskey=VSattach(vg->f,vg->ref[u],"r");
        if(vskey==FAIL)
            return(FAIL);
        s=VSfexist(vskey, field);
        VSdetach(vskey);
        if(s==1)
            return (vg->ref[u]); /* found. return vdata's ref */
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
PUBLIC intn Vinqtagref (int32 vkey, int32 tag, int32 ref)
{
    register uintn   u;
    register uint16     ttag, rref;
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vinqtagref");

    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FALSE);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FALSE);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FALSE);
    ttag = (uint16) tag;
    rref = (uint16) ref;

    for (u=0; u<vg->nvelt; u++)
        if ((ttag==vg->tag[u]) && (rref==vg->ref[u]))
            return (TRUE); /* exist */
    
    return (FALSE); /* does not exist */
} /* Vinqtagref */

/* ------------------------- Vntagrefs ------------------------------- */
/*
* Returns the number (0 or +ve integer) of tag/ref pairs in a vgroup.
* If error, returns FAIL
* 28-MAR-91 Jason Ng NCSA.
*/
PUBLIC int32 Vntagrefs (int32 vkey)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vntagrefs");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);
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
PUBLIC int32 Vgettagrefs (int32 vkey, int32 tagarray[], int32 refarray[], int32 n)
{
    int32 i;
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgettagrefs");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey)))) 
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    if (n > (int32)vg->nvelt)
        n = vg->nvelt;

    for (i=0; i<n; i++) {
        tagarray[i] = (int32)vg->tag[i];
        refarray[i] = (int32)vg->ref[i];
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
PUBLIC intn Vgettagref (int32 vkey, int32 which, int32 *tag, int32 *ref)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgettagref");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey)))) 
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    if (which < 0 || which > (int32)(vg->nvelt-1))
        HRETURN_ERROR(DFE_RANGE,FAIL); /* range err */
    
    *tag  = (int32) vg->tag[which];
    *ref  = (int32) vg->ref[which];
    return (SUCCEED); /* ok */
} /* Vgettagref */


/* -------------------------- VQuerytag -------------------------------- */
/*
 * Return the tag of this Vgroup.
 * Return 0 on failure
 */
PUBLIC int32 VQuerytag(int32 vkey)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgettagref");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    return ((int32) vg->otag);
} /* VQuerytag */


/* -------------------------- VQueryref -------------------------------- */
/*
  Return the ref of this Vgroup.
  Return FAIL on failure
*/
PUBLIC int32 VQueryref(int32 vkey)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgettagref");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
    /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    return ((int32) vg->oref);
} /* VQueryref */


/* ------------------------ Vaddtagref ---------------------------------- */
/*
 * Inserts a tag/ref pair into the attached vgroup vg.
 * First checks that the tag/ref is unique.
 * If error, returns FAIL or tag/ref is not inserted.
 * If OK, returns the total number of tag/refs in the vgroup (a +ve integer).
 * 28-MAR-91 Jason Ng NCSA.
 */

PUBLIC int32 Vaddtagref (int32 vkey, int32 tag, int32 ref)
{
    int32  n;
    vginstance_t  * v;
    VGROUP *vg;
#ifdef NO_DUPLICATES
    int32 i;
    uint16 ttag, rref;
#endif
    CONSTR(FUNC,"Vaddtagref");

    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

#ifdef NO_DUPLICATES
    /* make sure doesn't already exist in the Vgroup */
    ttag = (uint16) tag;
    rref = (uint16) ref;
    for (i = 0; i < vg->nvelt; i++)
        if ((ttag == vg->tag[i]) && (rref == vg->ref[i]))
            return(FAIL); /* exists */
#endif /* NO_DUPLICATES */

    n = vinsertpair(vg, (uint16) tag, (uint16) ref);

    return (n);

} /* Vaddtagref */

/* ------------------------ vinsertpair --------------------------------- */
/*
* Inserts a tag/ref pair into the attached vgroup vg.
* Expand the tag/ref space if necessary
* Returns the total number of tag/refs in the vgroup.
*/

int32 vinsertpair (VGROUP *vg, uint16 tag, uint16 ref)
{
    CONSTR(FUNC,"vinsertpair");
    
    if(vg->nvelt >= (uintn)vg->msize) {
        vg->msize *= 2;
        vg->tag  = (uint16 *) 
            HDregetspace((VOIDP)vg->tag, vg->msize * sizeof(uint16));
        vg->ref  = (uint16 *) 
            HDregetspace((VOIDP)vg->ref, vg->msize * sizeof(uint16));
        
        if((vg->tag == NULL) || (vg->ref == NULL))
            HRETURN_ERROR(DFE_NOSPACE,FAIL);
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
int32 Ventries (HFILEID f, int32 vgid)    
{
    uint8   * vgpack;
	VGROUP 	vg;
    int32   len;
    CONSTR(FUNC,"Ventries");

	if (vgid < 1)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    len = Hlength(f, DFTAG_VG, (uint16) vgid);
    if(len == FAIL)
        HRETURN_ERROR(DFE_NOSUCHTAG,FAIL);

    vgpack = (uint8 *) HDgetspace(len);
    if(vgpack == NULL)
        HRETURN_ERROR(DFE_NOSPACE,FAIL);

    if ( Hgetelement(f, DFTAG_VG, (uint16)vgid, vgpack) == FAIL)
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vunpackvg(&vg,vgpack);

    HDfreespace((VOIDP)vg.tag);
    HDfreespace((VOIDP)vg.ref);

    return( (int32) vg.nvelt);
} /* Ventries */

/* ==================================================================== */
/*
*	Vsetname
* 	gives a name to the VGROUP vg.
*
* RETURN VALUES: SUCCEED for success, FAIL for failure (big suprise, eh?)
*
*	truncates to max length of VGNAMELENMAX 
*/
PUBLIC int32 Vsetname (int32 vkey, const char *vgname)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vsetname");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey)))) 
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);
  
    HIstrncpy(vg->vgname, vgname, VGNAMELENMAX);
    vg->marked = TRUE;
    return(SUCCEED);
} /* Vsetname */

/* ==================================================================== */
/*
*	Vsetclass
* 	assigns a class name to the VGROUP vg.
*
* RETURN VALUES: SUCCEED for success, FAIL for failure (big suprise, eh?)
*
*	truncates to max length of VGNAMELENMAX 
*/

PUBLIC int32 Vsetclass (int32 vkey, const char *vgclass)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vsetclass");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    HIstrncpy(vg->vgclass, vgclass,VGNAMELENMAX);
    vg->marked = TRUE;
    return(SUCCEED);
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
PUBLIC intn Visvg (int32 vkey, int32 id)
{
    register uintn u;
    register uint16 ID;
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Visvg");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey)))) 
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);
  
    ID = (uint16) id;

    for(u = 0; u < vg->nvelt; u++)
        if (vg->ref[u] == ID   &&   /* if the ids match, */
                vg->tag[u] == DFTAG_VG)     /* and it is a vgroup */
            return (TRUE);
  
    return (FALSE);
} /* Visvg */

/* -------------------------- Visvs -------------------------------- */

/* Visvs
*  checks if an id in a vgroup refers to a VDATA
*  RETURNS 1 if so
*  RETURNS 0 if not, or if error.
*/

PUBLIC intn Visvs (int32 vkey, int32 id)
{
    register intn i;
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"VSisvs");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FALSE);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FALSE);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FALSE);

    i = vg->nvelt;
    while(i)
        if (vg->ref[--i] == (uint16)id && vg->tag[i]==VSDESCTAG)
            return(TRUE);

    return(FALSE);
} /* Visvs */

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

PUBLIC int32 Vgetid (HFILEID f,int32 vgid)         
{
	vginstance_t * v;
	vfile_t		* vf;
    VOIDP *t;
	int32 key;
	CONSTR(FUNC,"Vgetid");

    if(vgid < -1 )
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF,FAIL);

    if (vgid == (-1)) { /* check for magic value to return the first group */
        if (NULL == (t=(VOIDP *)tbbtfirst((TBBT_NODE *)*(vf->vgtree))))
            return (FAIL);
        else {
            v=(vginstance_t *)*t;   /* get actual pointer to the vginstance_t */
            return( v->ref); /* rets 1st vgroup's ref */
          } /* end else */
      } /* end if */

	/* look in vgtab for vgid */
    /* tbbtdfind returns a pointer to the vginstance_t pointer */
    key=VGSLOT2ID(f,vgid);
    t=(VOIDP *)tbbtdfind(vf->vgtree,(VOIDP)&key,NULL);
    if(t == NULL || 
       t == (VOIDP *) tbbtlast((TBBT_NODE *)*(vf->vgtree))) /* couldn't find the old vgid */
        return(FAIL);                                       /* or at the end */
    else
        if (NULL==(t=(VOIDP *)tbbtnext((TBBT_NODE *)t)))      /* get the next node in the tree */
            return (FAIL);
        else {
            v=(vginstance_t *)*t;   /* get actual pointer to the vginstance_t */
            return(v->ref); /* rets 1st vgroup's ref */
          } /* end else */
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

PUBLIC int32 Vgetnext (int32 vkey, int32 id)
{
    register uintn  u;
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgetnext");
    
    if (!VALIDVGID(vkey) || id<(-1))
        HRETURN_ERROR(DFE_ARGS,FAIL);
  
  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
  
    if ((vg == NULL) || (vg->otag != DFTAG_VG))
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if (vg->nvelt == 0)
        return(FAIL);             /* nothing in vg */

    if (id == -1) {
        if ((vg->tag[0] == DFTAG_VG) || (vg->tag[0]==VSDESCTAG))
            return(vg->ref[0]);       /* id of first entry */
      } /* end if */

    /* look in vg for id */
    for(u=0; u<vg->nvelt; u++)
        if ((vg->tag[u]==DFTAG_VG) || (vg->tag[u]==VSDESCTAG)) {
            if(vg->ref[u] == (uint16)id) {
                if (u == (vg->nvelt - 1) )
                    return(FAIL);
                else  {
                    if ((vg->tag[u+1] == DFTAG_VG) || (vg->tag[u+1]==VSDESCTAG))
                        return(vg->ref[u+1]);     /* return the id of next entry */
                    else
                        return (FAIL);
                  } /* end else */
              } /* end if */
          } /* end if */

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

PUBLIC int32 Vgetname (int32 vkey, char *vgname)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgetname");

    if (!VALIDVGID(vkey) || vgname==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    HDstrcpy(vgname, vg->vgname);
    return(SUCCEED);
} /* Vgetname */

/* ================================================================= */
/*
*	Vgetclass
*	returns the vgroup's class name 
*   ASSUME that vgclass has been allocated large enough to hold
*   the name
*
*/

PUBLIC int32 Vgetclass (int32 vkey, char *vgclass)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vgetclass");

    if (!VALIDVGID(vkey) || vgclass==NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    HDstrcpy(vgclass, vg->vgclass);
    return(SUCCEED);
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

PUBLIC intn Vinquire (int32 vkey, int32 *nentries, char *vgname)
{
    vginstance_t  * v;
    VGROUP *vg;
    CONSTR(FUNC,"Vinquire");
    
    if (!VALIDVGID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vg's index in vgtab */
    if(NULL==(v=(vginstance_t*)vginstance(VGID2VFILE(vkey),(uint16)VGID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vg=v->vg;
    if (vg == NULL)
        HRETURN_ERROR(DFE_BADPTR,FAIL);

    if(vg->otag != DFTAG_VG)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if(vgname!=NULL)
        HDstrcpy(vgname, vg->vgname);
    if(nentries!=NULL)
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

PUBLIC HFILEID Vopen( char *path, intn acc_mode, int16 ndds)
{
#ifdef LATER
	CONSTR(FUNC,"Vopen");
#endif
	HFILEID  f;

  	f = Hopen(path, acc_mode, ndds);
    if (f==FAIL)
        return(FAIL);
    
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
*/

PUBLIC intn Vclose (HFILEID f)
{
#ifdef LATER
	CONSTR(FUNC,"Vclose");
#endif

	Vfinish (f);
    return(Hclose (f));
}

/* ------------------------------- Vdelete -------------------------------- */
/*

  Remove a Vgroup from its file.  This function will both remove the Vgoup
  from the internal Vset data structures as well as from the file.

  (i.e. it calls tbbt_delete() and Hdeldd())

  Return FAIL / SUCCEED

*/
int32
Vdelete(int32 f, int32 vgid)
{
    VOIDP	   v;
    vfile_t      * vf;
    VOIDP        * t;
    int32          key;
    CONSTR(FUNC,"Vdelete");

    if(vgid < 0)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
        vfile = (vfile_t *)HDgetspace(MAX_VFILE * sizeof(vfile_t));
        if (vfile == NULL)
          HRETURN_ERROR(DFE_NOSPACE, FAIL);
        /* zero the space */
        vfile = (vfile_t *)HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t))); 
      }

    if (NULL==(vf = Get_vfile(f)))
        HRETURN_ERROR(DFE_FNF,FAIL);

    key=VGSLOT2ID(f,vgid);

    t = (VOIDP *)tbbtdfind(vf->vgtree,(VOIDP)&key,NULL);

    if(t == NULL)
        return(FAIL);

    v = tbbtrem((TBBT_NODE **)vf->vgtree, (TBBT_NODE *)t, NULL);
    if(v)
        vdestroynode((VOIDP)v);

    Hdeldd(f, DFTAG_VG, (uint16) vgid);

    return SUCCEED;
} /* Vdelete */
