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

LOCAL ROUTINES
 Load_vfile   -- loads vgtab table with info of all vgroups in file.
 Remove_vfile -- removes the file ptr from the vfile[] table. 
 vginstance   -- Looks thru vgtab for vgid and return the addr of the vg 
                  instance where vgid is found. 
 vunpackvg    -- Unpacks the fields from a buf (ie a DFTAG_VG data object 
                  just read in from the HDF file), into a VGROUP structure vg.

EXPORTED ROUTINES
 Following 4 routines are solely for B-tree routines.
 vcompare     -- Compares two B-tree keys for equality.  Similar to memcmp.
 vprint       -- Prints out the key and reference number of VDatas and Vgroups
 vdestroynode -- Frees B-Tree nodes
 vtfreekey    -- Frees B-Tree index (actually doesn't anything at all)

 Vinitialize  --
 Vfinish      --
 vexistvg     -- Tests if a vgroup with id "vgid" is in the file's vgtab.
 vpackvg      -- Extracts fields from a VGROUP struct "vg" and packs the 
                  fields into array buf in preparation for storage in the 
                  HDF file.
 Vattach      -- Attaches to an existing vgroup or creates a new vgroup.
 Vdetach      -- Detaches access to vg.    
 Vinsert      -- Inserts a velt (vs or vg) into a vg 
 Vflocate     -- Checks to see if the given field exists in a vdata 
                  belonging to this vgroup.
 Vinqtagref   -- Checks whether the given tag/ref pair already exists 
                  in the vgroup.
 Vntagrefs    -- Returns the number (0 or +ve integer) of tag/ref pairs 
                  in a vgroup.
 Vgettagrefs  -- Returns n tag/ref pairs from the vgroup into the 
                  caller-supplied arrays(tagrarray and refarray).
 Vgettagref   -- Returns a specified tag/ref pair from the vgroup.
 VQuerytag    -- Return the tag of this Vgroup.
 VQueryref    -- Return the ref of this Vgroup.
 Vaddtagref   -- Inserts a tag/ref pair into the attached vgroup vg.
 vinsertpair  -- Inserts a tag/ref pair into the attached vgroup vg.
 Ventries     -- Returns the num of entries (+ve integer) in the vgroup vgid.
 Vsetname     -- Gives a name to the VGROUP vg.
 Vsetclass    -- Assigns a class name to the VGROUP vg.
 Visvg        -- Tests if the given entry in the vgroup vg is a VGROUP.
 Visvs        -- Checks if an id in a vgroup refers to a VDATA.
 Vgetid       -- Given a vgroup's id, returns the next vgroup's id in the file.
 Vgetnext     -- Given the id of an entry from a vgroup vg, looks in vg 
                  for the next entry after it, and returns its id.
 Vgetname     -- Returns the vgroup's name.
 Vgetclass    -- Returns the vgroup's class name .
 Vinquire     -- General inquiry routine for VGROUP. 
 Vopen        -- This routine opens the HDF file and initializes it for 
                  Vset operations.(i.e." Hopen(); Vinitialize(f)").
 Vclose       -- This routine closes the HDF file, after it has freed 
                  all memory and updated the file.
                  (i.e." Vfinish(f); Hclose(f);").
 Vdelete      -- Remove a Vgroup from its file.  This function will both 
                  remove the Vgoup from the internal Vset data structures 
                  as well as from the file.
*************************************************************************/

#include <ctype.h>  /* for tolower() */
#include "vg.h"

/* Prototypes */
extern VOID vprint(VOIDP k1);

PRIVATE intn Load_vfile
            (HFILEID f);

PRIVATE intn Remove_vfile
            (HFILEID f);

PRIVATE void vunpackvg
            (VGROUP * vg, uint8 buf[], uintn len);

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

vfile_t *vfile = NULL;

/* -------------------------- Load_vfile ------------------------ */
/*
   *** Only called by Vinitialize()  ***

   loads vgtab table with info of all vgroups in file f.
   Will allocate a new vfile_t, then proceed to load vg instances.
   RETURNS FAIL if error or no more file slots available.
   RETURNS SUCCEED if ok.
 */

PRIVATE intn
Load_vfile(HFILEID f)
{
    vfile_t    *vf;
    vginstance_t *v;
    vsinstance_t *w;
    int32       aid, ret;
    uint16      tag, ref;
    intn        ret_value = SUCCEED;
    CONSTR(FUNC, "Load_vfile");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
          vfile = (vfile_t *) HDmalloc(MAX_VFILE * sizeof(vfile_t));
          if (vfile == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* zero the space */
          HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t)));
      }

    /* allocate a new vfile_t structure */
    if ((vf = Get_vfile(f)) == NULL)
        HGOTO_ERROR(DFE_FNF, FAIL);

    /* the file is already loaded (opened twice) do nothing */
    if (vf->access++)
      {
        ret_value = SUCCEED;
        goto done;
      }

    /* load all the vg's  tag/refs from file */
    vf->vgtabn = 0;
    vf->vgtree = tbbtdmake(vcompare, sizeof(int32));
    if (vf->vgtree == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    ret = aid = Hstartread(f, DFTAG_VG, DFREF_WILDCARD);
    while (ret != FAIL)
      {
          HQuerytagref(aid, &tag, &ref);
          if (NULL == (v = (vginstance_t *) HDmalloc(sizeof(vginstance_t))))
            {
                tbbtdfree(vf->vgtree, vdestroynode, NULL);
                HGOTO_ERROR(DFE_NOSPACE, FAIL);
            }

          vf->vgtabn++;
          v->key = (int32) VGSLOT2ID(f, ref);   /* set the key for the node */
          v->ref = (intn) ref;
          v->vg = VPgetinfo(f,ref);  /* get the header information */
          v->nattach = 0;
          v->nentries = 0;
          tbbtdins(vf->vgtree, (VOIDP) v, NULL);    /* insert the vg instance in B-tree */
          ret = Hnextread(aid, DFTAG_VG, DFREF_WILDCARD, DF_CURRENT);
      }
    Hendaccess(aid);

    /* load all the vs's  tag/refs from file */
    vf->vstabn = 0;
    vf->vstree = tbbtdmake(vcompare, sizeof(int32));
    if (vf->vstree == NULL)
      {
          tbbtdfree(vf->vgtree, vdestroynode, NULL);
          HGOTO_ERROR(DFE_NOSPACE, FAIL);
      }     /* end if */

    ret = aid = Hstartread(f, VSDESCTAG, DFREF_WILDCARD);
    while (ret != FAIL)
      {
          HQuerytagref(aid, &tag, &ref);
          if (NULL == (w = (vsinstance_t *) HDmalloc(sizeof(vsinstance_t))))
            {
                tbbtdfree(vf->vgtree, vdestroynode, NULL);
                tbbtdfree(vf->vstree, vsdestroynode, NULL);
                HGOTO_ERROR(DFE_NOSPACE, FAIL);
            }

          vf->vstabn++;
          w->key = (int32) VSSLOT2ID(f, ref);   /* set the key for the node */
          w->ref = (intn) ref;
          w->vs = VSPgetinfo(f,ref);  /* get the header information */
          w->nattach = 0;
          w->nvertices = 0;
          tbbtdins(vf->vstree, (VOIDP) w, NULL);    /* insert the vg instance in B-tree */
          ret = Hnextread(aid, VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
      }
    Hendaccess(aid);

    /* file may be incompatible with vset version 2.x. Need to check it */
    if (((int32) 0 == vf->vgtabn) && ((int32) 0 == vf->vstabn))
        if ((int32) 0 == vicheckcompat(f))
          {     /* not compatible */
#if 0
              nvfile--;     /* delete the structure for that file */
#endif
              tbbtdfree(vf->vgtree, vdestroynode, NULL);
              tbbtdfree(vf->vstree, vsdestroynode, NULL);
              HGOTO_ERROR(DFE_BADOPEN, FAIL);
          }

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Load_vfile */

/* ---------------------------- Remove_vfile ------------------------- */
/*
   removes the file ptr from the vfile[] table.
   *** Only called by Vfinish() ***
 */
PRIVATE intn
Remove_vfile(HFILEID f)
{
    vfile_t    *vf = NULL;
    intn       ret_value = SUCCEED;
    CONSTR(FUNC, "Remove_vfile");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

    /* Figure out what file to work on */
    if ((vf = Get_vfile(f)) == NULL)
        HGOTO_ERROR(DFE_FNF, FAIL);

    /* someone still has an active pointer to this file */
    if (--vf->access)
      {
        ret_value = (SUCCEED);
        goto done;
      }

    /* clear out the tbbt's */
    tbbtdfree(vf->vgtree, vdestroynode, NULL);
    tbbtdfree(vf->vstree, vsdestroynode, NULL);

    HDmemset(vf, 0, sizeof(vfile_t));   /* reset values of structure */


done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Remove_vfile */

/* ---------------------------- vcompare ------------------------- */
/*
   Compares two B-tree keys for equality.  Similar to memcmp.

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
intn
vcompare(VOIDP k1, VOIDP k2, intn cmparg)
{
  intn  ret_value;
  /* shut compiler up */
  cmparg = cmparg;

  ret_value = ((intn) ((*(int32 *) k1) - (*(int32 *) k2)));    /* valid for integer keys */

  return ret_value;
}   /* vcompare */

/* ---------------------------- vprint ------------------------- */
/*
   Prints out the key and reference number of VDatas and Vgroups

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
VOID
vprint(VOIDP k1)
{
    printf("Ptr=%p, key=%d, ref=%d\n", k1, (int) ((vginstance_t *) k1)->key, (int) ((vginstance_t *) k1)->ref);
}   /* vprint */

/* ---------------------------- vdestroynode ------------------------- */
/*
   Frees B-Tree nodes

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
VOID
vdestroynode(VOIDP n)
{
    VGROUP     *vg;

    vg = ((vginstance_t *) n)->vg;
    if (vg != NULL)
      {
          HDfree((VOIDP) vg->tag);
          HDfree((VOIDP) vg->ref);
          HDfree((VOIDP) vg);
      }

    HDfree((VOIDP) n);

}   /* vdestroynode */

#ifdef NOTNEEDED
/* ---------------------------- vtfreekey ------------------------- */
/*
   Frees B-Tree index (actually doesn't anything at all)

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
VOID
vtfreekey(VOIDP k)
{
    k = k;  /* i.e. do nothing */
}   /* vtfreekey */
#endif

/* ---------------------------- Vinitialize ------------------------- */

intn
Vinitialize(HFILEID f)
{
  intn   ret_value = SUCCEED;
  CONSTR(FUNC, "Vinitialize");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vinitialize);
#endif /* HAVE_PABLO */

    if (!VALIDFID(f))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (Load_vfile(f) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vinitialize);
#endif /* HAVE_PABLO */

  return ret_value;
}

/* ---------------------------- Vfinish ------------------------- */

intn
Vfinish(HFILEID f)
{
  intn    ret_value = SUCCEED;
  CONSTR(FUNC, "Vfinish");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vfinish);
#endif /* HAVE_PABLO */

    if (!VALIDFID(f))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (Remove_vfile(f) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vfinish);
#endif /* HAVE_PABLO */

  return ret_value;
}

/* ---------------------------- vginstance ----------------------------- */
/*
   * Looks thru vgtab for vgid and return the addr of the vg instance
   * where vgid is found.
   * RETURNS NULL if error or not found.
   * RETURNS vginstance_t pointer if ok.
   *
 */
vginstance_t *
vginstance(HFILEID f, uint16 vgid)
{
    VOIDP      *t;
    vfile_t    *vf;
    int32       key;
    vginstance_t *ret_value;
    CONSTR(FUNC, "vginstance");

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
          vfile = (vfile_t *) HDmalloc(MAX_VFILE * sizeof(vfile_t));
          if (vfile == NULL)
              HGOTO_ERROR(DFE_NOSPACE, NULL);
          /* zero the space */
          vfile = (vfile_t *) HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t)));
      }

    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, NULL);

    /* tbbtdfind returns a pointer to the vginstance_t pointer */
    key = VGSLOT2ID(f, vgid);
    t = (VOIDP *) tbbtdfind(vf->vgtree, (VOIDP) &key, NULL);
    if (t != NULL)
      {
        ret_value = ((vginstance_t *) * t);  /* return the actual vginstance_t ptr */
        goto done;
      }

    HGOTO_ERROR(DFE_NOMATCH, NULL);

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* vginstance */

/* ------------------------ vexistvg --------------------------- */
/*
   * Tests if a vgroup with id vgid is in the file's vgtab.
   * returns FAIL if not found,
   * returns TRUE if found.
 */
int32
vexistvg(HFILEID f, uint16 vgid)
{
  int32   ret_value;
#ifdef LATER
    CONSTR(FUNC, "vexistvg");
#endif

    if (NULL == (vginstance_t *) vginstance(f, vgid))
        ret_value = (FAIL);
    else
        ret_value =  (TRUE);
  
  return ret_value;
}   /* vexistvg */

/* ==================================================================== */
/*
   * vpackvg() and vunpackvg() : Packing and unpacking routines.
   * For use in retrieving and storing vgroups to/from the HDF file.
   *
   *    Fields of VGROUP  that gets stored in HDF as a DFTAG_VG data object:
   *            int16           nvelt (no of entries )
   *            char            vgname[MAXVGNAMELEN]
   *     char     vgclass[MAXVGNAMELEN]
   *            int16           tag[1..nvelt]
   *            int16           ref[1..nvelt]
 */
/* ==================================================================== */

/* ==================================================================== */
/*
   *    vpackvg
   *    extracts fields from  a VGROUP struct vg and pack the fields
   *  into array buf in preparation for storage in the HDF file.
   *
   *  NO RETURN VALUES.
 */

void
vpackvg(VGROUP * vg, uint8 buf[], int32 *size)
{
    uint16 i;
    uint8 *bb;
#ifdef LATER
    CONSTR(FUNC, "vpackvg");
#endif

    bb = &buf[0];

    /* save nvelt */
    UINT16ENCODE(bb, vg->nvelt);

    /* save all tags */
    for (i = 0; i < vg->nvelt; i++)
        UINT16ENCODE(bb, vg->tag[i]);

    /* save all refs */
    for (i = 0; i < vg->nvelt; i++)
        UINT16ENCODE(bb, vg->ref[i]);

    /* save the vgnamelen and vgname - omit the null */
    UINT16ENCODE(bb, HDstrlen(vg->vgname));

    HDstrcpy((char *) bb, vg->vgname);
    bb += HDstrlen(vg->vgname);

    /* save the vgclasslen and vgclass- omit the null */
    UINT16ENCODE(bb, HDstrlen(vg->vgclass));

    HDstrcpy((char *) bb, vg->vgclass);
    bb += HDstrlen(vg->vgclass);

    /* save the expansion tag/ref pair */
    UINT16ENCODE(bb, vg->extag);    /* the vg's expansion tag */
    UINT16ENCODE(bb, vg->exref);    /* the vg's expansion ref */

    /*  save the vg's version field */
    UINT16ENCODE(bb, vg->version);

    /* save the vg's more field */
    UINT16ENCODE(bb, vg->more);

    /* returns the size of total fields saved */
    *size = (int32) (bb - buf) + 1;     
    *bb = 0;       /* NOTE: the '+1' part shouldn't be there */
    /* but since files have been created with */
    /* it there (and the size calc. wrong) it */
    /* has to be left alone -QAK */
}   /* vpackvg */

/* ==================================================================== */
/*
   *    vunpackvg:
   *    Unpacks the fields from a buf (ie a DFTAG_VG data object just
   *    read in from the HDF file), into a VGROUP structure vg.
   *
   *    Will first zero out vg, unpack fields, then inits as much of
   *  vg as it can.
   *
   *    NO RETURN VALUES
   *
 */

PRIVATE void
vunpackvg(VGROUP * vg, uint8 buf[], uintn len)
{
    uint8 *bb;
    uintn u;
    uint16 uint16var;
#ifdef LATER
    CONSTR(FUNC, "vunpackvg");
#endif

    /* '5' is a magic number, the exact amount of space for 2 uint16's */
    /* the magic number _should_ be '4', but the size of the Vgroup */
    /* information is incorrectly calculated (in vpackvg() above) when the */
    /* info is written to the file and it's too late to change it now :-( */
    bb = &buf[len - 5];
    UINT16DECODE(bb, vg->version);  /* retrieve the vg's version field */

    UINT16DECODE(bb, vg->more);     /* retrieve the vg's more field */

    bb = &buf[0];

    /* retrieve nvelt */
    if (vg->version <= 3)
      {     /* current Vset version number */
          UINT16DECODE(bb, vg->nvelt);

          vg->msize = (vg->nvelt > MAXNVELT ? vg->nvelt : MAXNVELT);
          vg->tag = (uint16 *) HDmalloc(vg->msize * sizeof(uint16));
          vg->ref = (uint16 *) HDmalloc(vg->msize * sizeof(uint16));

          if ((vg->tag == NULL) || (vg->ref == NULL))
              return;

          /* retrieve the tags */
          for (u = 0; u < vg->nvelt; u++)
              UINT16DECODE(bb, vg->tag[u]);

          /* retrieve the refs */
          for (u = 0; u < vg->nvelt; u++)
              UINT16DECODE(bb, vg->ref[u]);

          /* retrieve vgname (and its len)  */
          UINT16DECODE(bb, uint16var);

          HIstrncpy(vg->vgname, (char *) bb, (int32) uint16var + 1);
          bb += uint16var;

          /* retrieve vgclass (and its len)  */
          UINT16DECODE(bb, uint16var);

          HIstrncpy(vg->vgclass, (char *) bb, (int32) uint16var + 1);
          bb += uint16var;

          UINT16DECODE(bb, vg->extag);  /* retrieve the vg's expansion tag */
          UINT16DECODE(bb, vg->exref);  /* retrieve the vg's expansion ref */
      }     /* end if */
}   /* vunpackvg */

/*--------------------------------------------------------------------------
 NAME
    VPgetinfo
 PURPOSE
    Read in the "header" information about the Vgroup.
 USAGE
    VGROUP *VPgetinfo(f,ref)
        HFILEID f;              IN: the HDF file id
        uint16 ref;             IN: the tag & ref of the Vgroup 
 RETURNS
    Return a pointer to a VGROUP filled with the Vgroup information on success,
    NULL on failure.
 DESCRIPTION
    This routine pre-reads the header information for a Vgroup into memory
    so that it can be accessed more quickly by the routines that need it.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
VGROUP _HUGE *VPgetinfo(HFILEID f,uint16 ref)
{
    CONSTR(FUNC, "VPgetinfo");
	VGROUP         *vg;
    uint8          *vgpack;
    uint32          len;
    VGROUP _HUGE *ret_value = NULL; /* FAIL */
          
    /* Find out how long the VGroup information is */
    if (( len = Hlength(f, DFTAG_VG, (uint16) ref)) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL,NULL);
 
    /* Get space for the raw Vgroup info */
    if(( vgpack = (uint8 *) HDmalloc(len)) == NULL)
        HGOTO_ERROR(DFE_NOSPACE,NULL);
 
    /* Get the raw Vgroup info */
    if (Hgetelement(f, DFTAG_VG, (uint16)ref, vgpack) == (int32)FAIL) 
        HGOTO_ERROR(DFE_NOMATCH,NULL);
       
    /* allocate space for vg */
    if (NULL == (vg =(VGROUP*) HDmalloc (sizeof(VGROUP)))) 
        HGOTO_ERROR(DFE_NOSPACE,NULL);
       
    /* unpack vgpack into structure vg, and init  */
    vunpackvg(vg,vgpack,len);
    vg->f             = f;
    vg->new_vg        = 0;
    vg->oref          = ref;
    vg->otag          = DFTAG_VG;
      
    HDfree((VOIDP)vgpack);

    ret_value = vg;

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end VPgetinfo */

/* ----------------------------- Vattach --------------------------- */

/*
   *     Vattach:
   *
   *   attaches to an existing vgroup or creates a new vgroup.
   *     returns NULL if  error, else ptr to vgroup.
   *
   *    IGNORE accesstype. (but save it)
   *  if vgid == -1,
   *      create a NEW vg if vgdir is not full.
   *      Also set nattach =1, nentries=0.
   *  if vgid +ve,
   *      look in vgdir to see if already attached,
   *      if yes, incr nattach
   *      if not, fetch from file. attach, set nattach=1, netries= val from file
   *
   *    In any case, set marked flag to 0.
 */

int32
Vattach(HFILEID f, int32 vgid, const char *accesstype)
{
    VGROUP     *vg;
    int16       acc_mode;
    vginstance_t *v;
    vfile_t    *vf;
    filerec_t  *file_rec;       /* file record */
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "Vattach");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vattach);
#endif /* HAVE_PABLO */

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
          vfile = (vfile_t *) HDmalloc(MAX_VFILE * sizeof(vfile_t));
          if (vfile == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          /* zero the space */
          vfile = (vfile_t *) HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t)));
      }

    if (f == FAIL)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, FAIL);

    if (tolower(accesstype[0]) == 'r')
        acc_mode = 'r';
    else if (tolower(accesstype[0]) == 'w')
        acc_mode = 'w';
    else
        HGOTO_ERROR(DFE_BADACC, FAIL);

    /* convert file id to file record and check for write-permission */
    file_rec = FID2REC(f);
    if(acc_mode=='w' && !(file_rec->access&DFACC_WRITE))
        HGOTO_ERROR(DFE_BADACC, FAIL);

    if (vgid == -1)
      {
          /******* create a NEW vg in vgdir ******/
          if (acc_mode == 'r')
              HGOTO_ERROR(DFE_ARGS, FAIL);

          /* allocate space for vg, & zero it out */
          if ((vg = (VGROUP *) HDmalloc(sizeof(VGROUP))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          /* initialize new vg */
          vg->msize = MAXNVELT;
          vg->tag = (uint16 *) HDmalloc(vg->msize * sizeof(uint16));
          vg->ref = (uint16 *) HDmalloc(vg->msize * sizeof(uint16));

          if ((vg->tag == NULL) || (vg->ref == NULL))
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          vg->nvelt = 0;
          vg->vgname[0] = '\0';
          vg->f = f;
          vg->otag = DFTAG_VG;
          vg->oref = Htagnewref(f,vg->otag);    /* create a new unique ref for it */
          if (vg->oref == 0)
              HGOTO_ERROR(DFE_NOREF, FAIL);

          vg->access = acc_mode;

          vg->marked = 1;
          vg->new_vg = 1;
          vg->vgclass[0] = '\0';
          vg->extag = 0;
          vg->exref = 0;
          vg->more = 0;
          vg->version = VSET_VERSION;

          /* attach new vg to file's vgtab  */
          if (NULL == (v = (vginstance_t *) HDmalloc(sizeof(vginstance_t))))
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          vf->vgtabn++;
          v->key = (int32) VGSLOT2ID(f, vg->oref);  /* set the key for the node */
          v->ref = (intn) vg->oref;
          v->vg = vg;
          v->nattach = 1;
          v->nentries = 0;
          tbbtdins(vf->vgtree, (VOIDP) v, NULL);    /* insert the vg instance in B-tree */

          ret_value = (v->key);  /* return key instead of VGROUP ptr */
          goto done;
      }
    else
      {
          /******* access an EXISTING vg *********/
          if (NULL == (v = vginstance(f, (uint16) vgid)))
              HGOTO_ERROR(DFE_NOMATCH, FAIL);

          /*
           * vg already attached.  inc nattach and return existing ptr
           */

          if (v->nattach > 0)
            {
                v->vg->access = MAX(v->vg->access, acc_mode);
                v->nattach++;
                ret_value = (v->key);    /* return key instead of VGROUP ptr */
                goto done;
            }

          vg=v->vg;
          vg->access = acc_mode;
          vg->marked = 0;

          /* attach vg to file's vgtab at the vg instance v */
          v->nattach = 1;
          v->nentries = vg->nvelt;

          ret_value = (v->key);  /* return key instead of VGROUP ptr */
          goto done;
      }

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vattach);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vattach */

/* ---------------------------- Vdetach ---------------------------- */
/*
   *    Vdetach
   *    Detaches access to vg.
   *    NO RETURN VALUES
   *
   *  if marked flag is 1, write out vg to file.
   *    if vg still has velts attached to it, cannot detach vg.
   *    decr  nattach. if (nattach is 0), free vg from vg instance.
   *    (check that no velts are still attached to vg before freeing)
   *
   *  if attached with read access, just return.
   *
   * after detach, set marked flag to 0.
   *
 */
int32
Vdetach(int32 vkey)
{
    VGROUP     *vg;
    int32       vgpacksize;
    uint8      *vgpack;
    vginstance_t *v;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vdetach");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vdetach);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if ((vg == NULL) || (vg->otag != DFTAG_VG))
        HGOTO_ERROR(DFE_ARGS, FAIL);

      /* Now, only update the Vgroup if it has actually changed. */
      /* Since only Vgroups with write-access are allowed to change, there is */
      /* no reason to check for access... (I hope) -QAK */
      if (vg->marked == 1)
        {
            vgpack = (uint8 *) HDmalloc((uint32) sizeof(VGROUP) + vg->nvelt * 4 + 1);
            vpackvg(vg, vgpack, &vgpacksize);

            /*
             *  For now attempt to blow away the old one.  This is a total HACK
             *    but the H-level needs to stabilize first
             */
            if(!vg->new_vg)
                Hdeldd(vg->f, DFTAG_VG, vg->oref);

            if (Hputelement(vg->f, DFTAG_VG, vg->oref, vgpack, vgpacksize) == FAIL)
                HERROR(DFE_WRITEERROR);
            HDfree((VOIDP) vgpack);
            vg->marked = 0;
            vg->new_vg = 0;
        }
    v->nattach--;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vdetach);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vdetach */

/* ------------------------------ Vinsert ----------------------------- */
/*
   *    Vinsert
   *  inserts a velt (vs or vg)  into a vg
   *    RETURNS entry position within vg (0 or +ve) or FAIL on error.
   *
   *    checks and prevents duplicate links.
   *
   * Since multiple files are now possible, check that both vg and velt
   * are from the same file. else error.
 */

int32
Vinsert(int32 vkey, int32 insertkey)
{
    VGROUP     *vg;
    vginstance_t *v;
    vsinstance_t *w;
    vginstance_t *x;
    uintn u;
    CONSTR(FUNC, "Vinsert");
    uint16      newtag = 0, newref = 0;
    int32       newfid;
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vinsert);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    if (vg->otag != DFTAG_VG || vg->access!='w')
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vg->otag != DFTAG_VG)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    newfid = FAIL;
    if (VALIDVSID(insertkey))
      {   /* locate vs's index in vstab */
	  if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(insertkey), (uint16) VSID2SLOT(insertkey))))
	      HGOTO_ERROR(DFE_NOVS, FAIL);

	  if (w->vs == NULL)
	      HGOTO_ERROR(DFE_ARGS, FAIL);

	  newtag = DFTAG_VH;
	  newref = w->vs->oref;
	  newfid = w->vs->f;
      }
    else
      {
	  if (VALIDVGID(insertkey))
	    {   /* locate vs's index in vgtab */
		if (NULL == (x = (vginstance_t *) vginstance(VGID2VFILE(insertkey), (uint16) VGID2SLOT(insertkey))))
		    HGOTO_ERROR(DFE_NOVS, FAIL);

		if (x->vg == NULL)
		    HGOTO_ERROR(DFE_ARGS, FAIL);

		newtag = DFTAG_VG;
		newref = x->vg->oref;
		newfid = x->vg->f;
	    }
      }

    /* make sure we found something */
    if (newfid == FAIL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vg->f != newfid)
        HGOTO_ERROR(DFE_DIFFFILES, FAIL);

    /* check and prevent duplicate links */
    for (u = 0; u < vg->nvelt; u++)
        if ((vg->ref[u] == newref) && (vg->tag[u] == newtag))
            HGOTO_ERROR(DFE_DUPDD, FAIL);

    /* Finally, ok to insert */
    vinsertpair(vg, newtag, newref);

    ret_value = (vg->nvelt - 1);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vinsert);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vinsert */

/* ----------------------------- Vflocate -------------------------------- */
/*
   Checks to see if the given field exists in a vdata belonging to this vgroup.
   If found, returns the ref of the vdata.
   If not found, or error, returns FAIL
   28-MAR-91 Jason Ng NCSA
 */
int32
Vflocate(int32 vkey, char *field)
{
    int32       s;
    uintn u;
    vginstance_t *v;
    VGROUP     *vg;
    int32       vskey;
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "Vflocate");

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    for (u = 0; u < vg->nvelt; u++)
      {
          if (vg->tag[u] != VSDESCTAG)
              continue;
          vskey = VSattach(vg->f, vg->ref[u], "r");
          if (vskey == FAIL)
            {
              ret_value = (FAIL);
              goto done;
            }
          s = VSfexist(vskey, field);
          VSdetach(vskey);
          if (s == 1)
            {
              ret_value = (vg->ref[u]);  /* found. return vdata's ref */
              goto done;
            }
      }

    ret_value = (FAIL);  /* field not found */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Vflocate */

/* ----------------------- Vinqtagref ------------------------------------- */
/*
   * Checks whether the given tag/ref pair already exists in the vgroup.
   * RETURNS TRUE if exist
   * RETURNS FALSE if not.
   * 28-MAR-91 Jason Ng NCSA
 */
intn
Vinqtagref(int32 vkey, int32 tag, int32 ref)
{
    uintn u;
    uint16 ttag, rref;
    vginstance_t *v;
    VGROUP     *vg;
    intn       ret_value = TRUE;
    CONSTR(FUNC, "Vinqtagref");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vinqtagref);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FALSE);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FALSE);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FALSE);
    ttag = (uint16) tag;
    rref = (uint16) ref;

    for (u = 0; u < vg->nvelt; u++)
        if ((ttag == vg->tag[u]) && (rref == vg->ref[u]))
          {
            ret_value = (TRUE);  /* exist */
            goto done;
          }

    ret_value = (FALSE);     /* does not exist */

done:
  if(ret_value == FALSE)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vinqtagref);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vinqtagref */

/* ------------------------- Vntagrefs ------------------------------- */
/*
   * Returns the number (0 or +ve integer) of tag/ref pairs in a vgroup.
   * If error, returns FAIL
   * 28-MAR-91 Jason Ng NCSA.
 */
int32
Vntagrefs(int32 vkey)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vntagrefs");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vntagrefs);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    ret_value = ((vg->otag == DFTAG_VG) ? (int32) vg->nvelt : FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vntagrefs);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vntagrefs */

/* ---------------------------- Vnrefs ------------------------------- */
/*
   * Returns the number (0 or +ve integer) of tags of a given type in a vgroup.
   * If error, returns FAIL
   * 05-NOV-94 Quincey Koziol.
 */
int32
Vnrefs(int32 vkey,int32 tag)
{
    CONSTR(FUNC, "Vnrefs");
    vginstance_t *v;
    VGROUP     *vg;
    uint16 ttag=(uint16)tag;    /* alias for faster comparison */
    int32 count=0;              /* number of matching tags in the Vgroup */
    uintn u;                    /* local counting variable */
    int32     ret_value = SUCCEED;

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    for (u = 0; u < vg->nvelt; u++)
        if (ttag == vg->tag[u])
            count++;

    ret_value = (count);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Vnrefs */

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
int32
Vgettagrefs(int32 vkey, int32 tagarray[], int32 refarray[], int32 n)
{
    int32       i;
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vgettagrefs");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vgettagrefs);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    if (n > (int32) vg->nvelt)
        n = vg->nvelt;

    for (i = 0; i < n; i++)
      {
          tagarray[i] = (int32) vg->tag[i];
          refarray[i] = (int32) vg->ref[i];
      }

    ret_value = (n);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vgettagrefs);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vgettagrefs */

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
intn
Vgettagref(int32 vkey, int32 which, int32 *tag, int32 *ref)
{
    vginstance_t *v;
    VGROUP     *vg;
    intn       ret_value = SUCCEED;
    CONSTR(FUNC, "Vgettagref");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vgettagref);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    if (which < 0 || which > (int32) (vg->nvelt - 1))
        HGOTO_ERROR(DFE_RANGE, FAIL);     /* range err */

    *tag = (int32) vg->tag[which];
    *ref = (int32) vg->ref[which];

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vgettagref);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vgettagref */

/* -------------------------- VQuerytag -------------------------------- */
/*
 * Return the tag of this Vgroup.
 * Return 0 on failure
 */
int32
VQuerytag(int32 vkey)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vgettagref");

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    ret_value = ((int32) vg->otag);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* VQuerytag */

/* -------------------------- VQueryref -------------------------------- */
/*
   Return the ref of this Vgroup.
   Return FAIL on failure
 */
int32
VQueryref(int32 vkey)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32     ret_value = SUCCEED;
    CONSTR(FUNC, "Vgettagref");

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    ret_value = ((int32) vg->oref);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* VQueryref */

/* ------------------------ Vaddtagref ---------------------------------- */
/*
 * Inserts a tag/ref pair into the attached vgroup vg.
 * First checks that the tag/ref is unique.
 * If error, returns FAIL or tag/ref is not inserted.
 * If OK, returns the total number of tag/refs in the vgroup (a +ve integer).
 * 28-MAR-91 Jason Ng NCSA.
 */

int32
Vaddtagref(int32 vkey, int32 tag, int32 ref)
{
    int32       n;
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
#ifdef NO_DUPLICATES
    int32       i;
    uint16      ttag, rref;
#endif
    CONSTR(FUNC, "Vaddtagref");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vaddtagref);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

#ifdef NO_DUPLICATES
    /* make sure doesn't already exist in the Vgroup */
    ttag = (uint16) tag;
    rref = (uint16) ref;
    for (i = 0; i < vg->nvelt; i++)
        if ((ttag == vg->tag[i]) && (rref == vg->ref[i]))
          {
            ret_value = (FAIL);  /* exists */
            goto done;
          }
#endif /* NO_DUPLICATES */

    n = vinsertpair(vg, (uint16) tag, (uint16) ref);

    ret_value = (n);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vaddtagref);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vaddtagref */

/* ------------------------ vinsertpair --------------------------------- */
/*
   * Inserts a tag/ref pair into the attached vgroup vg.
   * Expand the tag/ref space if necessary
   * Returns the total number of tag/refs in the vgroup.
 */

int32
vinsertpair(VGROUP * vg, uint16 tag, uint16 ref)
{
    CONSTR(FUNC, "vinsertpair");
    int32    ret_value = SUCCEED;

    if (vg->nvelt >= (uintn) vg->msize)
      {
          vg->msize *= 2;
          vg->tag = (uint16 *)
              HDrealloc((VOIDP) vg->tag, vg->msize * sizeof(uint16));
          vg->ref = (uint16 *)
              HDrealloc((VOIDP) vg->ref, vg->msize * sizeof(uint16));

          if ((vg->tag == NULL) || (vg->ref == NULL))
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
      }
    vg->tag[vg->nvelt] = tag;
    vg->ref[vg->nvelt] = ref;
    vg->nvelt++;

    vg->marked = TRUE;
    ret_value = ((int32) vg->nvelt);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}

/* ==================================================================== */
/*
   *    Ventries
   *    returns the no of entries (+ve integer) in the vgroup vgid.
   *  vgid must be an actual id
   *  RETURNS FAIL if error
   *
   *  undocumented
   *
 */
int32
Ventries(HFILEID f, int32 vgid)
{
    vginstance_t *v;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Ventries");

    if (vgid < 1)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if((v=vginstance(f,(uint16)vgid))==NULL)
        HGOTO_ERROR(DFE_NOMATCH,FAIL);          /* error */

    ret_value = (v->vg->nvelt);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Ventries */

/* ==================================================================== */
/*
   *    Vsetname
   *    gives a name to the VGROUP vg.
   *
   * RETURN VALUES: SUCCEED for success, FAIL for failure (big suprise, eh?)
   *
   *    truncates to max length of VGNAMELENMAX
 */
int32
Vsetname(int32 vkey, const char *vgname)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vsetname");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vsetname);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL || vg->access!='w')
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    HIstrncpy(vg->vgname, vgname, VGNAMELENMAX);
    vg->marked = TRUE;
    ret_value = (SUCCEED);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vsetname);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vsetname */

/* ==================================================================== */
/*
   *    Vsetclass
   *    assigns a class name to the VGROUP vg.
   *
   * RETURN VALUES: SUCCEED for success, FAIL for failure (big suprise, eh?)
   *
   *    truncates to max length of VGNAMELENMAX
 */

int32
Vsetclass(int32 vkey, const char *vgclass)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vsetclass");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vsetclass);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL || vg->access!='w')
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    HIstrncpy(vg->vgclass, vgclass, VGNAMELENMAX);
    vg->marked = TRUE;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vsetclass);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vsetclass */

/* -------------------------------- Visvg --------------------------------- */
/*
   *    Visvg
   *    tests if an entry in the vgroup vg is a VGROUP, given the entry's id.
   *
   *    RETURNS TRUE if so
   *    RETURNS FALSE if not, or if error
   *
 */
intn
Visvg(int32 vkey, int32 id)
{
    uintn u;
    uint16 ID;
    vginstance_t *v;
    VGROUP     *vg;
    intn      ret_value = TRUE;
    CONSTR(FUNC, "Visvg");

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    ID = (uint16) id;

    for (u = 0; u < vg->nvelt; u++)
        if (vg->ref[u] == ID &&     /* if the ids match, */
            vg->tag[u] == DFTAG_VG)     /* and it is a vgroup */
          {
            ret_value =  (TRUE);
            goto done;
          }

    ret_value = (FALSE);

done:
  if(ret_value == FALSE)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Visvg */

/* -------------------------- Visvs -------------------------------- */

/* Visvs
   *  checks if an id in a vgroup refers to a VDATA
   *  RETURNS 1 if so
   *  RETURNS 0 if not, or if error.
 */

intn
Visvs(int32 vkey, int32 id)
{
    intn i;
    vginstance_t *v;
    VGROUP     *vg;
    intn       ret_value = TRUE;
    CONSTR(FUNC, "VSisvs");

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FALSE);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FALSE);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FALSE);

    i = vg->nvelt;
    while (i)
        if (vg->ref[--i] == (uint16) id && vg->tag[i] == VSDESCTAG)
          {
            ret_value = (TRUE);
            goto done;
          }

done:
  if(ret_value == FALSE)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Visvs */

/* ======================================================= */
/*
   *    Vgetid
   *
   *    Given a vgroup's id, returns the next vgroup's id in the file f .
   *    The call Vgetid(f,-1) returns the id of the FIRST vgroup in the file.
   *
   *    RETURNS -1 if error
   *    RETURNS the next vgroup's id (0 or +ve integer).
   *
   *    This id is actually the "ref" of the vgroup "tag/ref".
 */

int32
Vgetid(HFILEID f, int32 vgid)
{
    vginstance_t *v;
    vfile_t    *vf;
    VOIDP      *t;
    int32       key;
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "Vgetid");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vgetid);
#endif /* HAVE_PABLO */

    if (vgid < -1)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
          vfile = (vfile_t *) HDmalloc(MAX_VFILE * sizeof(vfile_t));
          if (vfile == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          /* zero the space */
          vfile = (vfile_t *) HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t)));
      }

    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, FAIL);

    if (vgid == (-1))
      {     /* check for magic value to return the first group */
          if (NULL == (t = (VOIDP *) tbbtfirst((TBBT_NODE *) * (vf->vgtree))))
            {
              ret_value = (FAIL);
              goto done;
            }
          else
            {
                v = (vginstance_t *) * t;   /* get actual pointer to the vginstance_t */
                ret_value = (v->ref);    /* rets 1st vgroup's ref */
                goto done;
            }   /* end else */
      }     /* end if */

    /* look in vgtab for vgid */
    /* tbbtdfind returns a pointer to the vginstance_t pointer */
    key = VGSLOT2ID(f, vgid);
    t = (VOIDP *) tbbtdfind(vf->vgtree, (VOIDP) &key, NULL);
    if (t == NULL ||
        t == (VOIDP *) tbbtlast((TBBT_NODE *) * (vf->vgtree)))  /* couldn't find the old vgid */
        ret_value = (FAIL);  /* or at the end */
    else if (NULL == (t = (VOIDP *) tbbtnext((TBBT_NODE *) t)))     /* get the next node in the tree */
        ret_value = (FAIL);
    else
      {
          v = (vginstance_t *) * t;     /* get actual pointer to the vginstance_t */
          ret_value = (v->ref);  /* rets 1st vgroup's ref */
      }     /* end else */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vgetid);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vgetid */

/* ================================================================= */
/*
   *    Vgetnext
   *
   *    Given the id of an entry from a vgroup vg, looks in vg for the next
   *    entry after it, and returns its id.
   *    The call Vgetnext (vg,-1) returns the id of the FIRST entry in the vgroup.
   *
   *  Vgetnext will look at only VSET elements in the vgroup.
   *  To look at all links in a vgroup, use Vgettagrefs instead.
   *
   *    RETURNS -1 if error
   *    RETURNS the id of the next entry( 0 or +ve integer)  in the vgroup.
   *
   *    This id is actually the "ref" of the entry's "tag/ref".
   *
 */

int32
Vgetnext(int32 vkey, int32 id)
{
    uintn u;
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vgetnext");

    if (!VALIDVGID(vkey) || id < (-1))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;

    if ((vg == NULL) || (vg->otag != DFTAG_VG))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vg->nvelt == 0)
      {
        ret_value = (FAIL);  /* nothing in vg */
        goto done;
      }

    if (id == -1)
      {
          if ((vg->tag[0] == DFTAG_VG) || (vg->tag[0] == VSDESCTAG))
            {
              ret_value = (vg->ref[0]);  /* id of first entry */
              goto done;
            }
      }     /* end if */

    /* look in vg for id */
    for (u = 0; u < vg->nvelt; u++)
        if ((vg->tag[u] == DFTAG_VG) || (vg->tag[u] == VSDESCTAG))
          {
              if (vg->ref[u] == (uint16) id)
                {
                    if (u == (vg->nvelt - 1))
                      {
                        ret_value = (FAIL);
                        goto done;
                      }
                    else
                      {
                          if ((vg->tag[u + 1] == DFTAG_VG) || (vg->tag[u + 1] == VSDESCTAG))
                            {
                              ret_value = (vg->ref[u + 1]);  /* return the id of next entry */
                              goto done;
                            }
                          else
                            {
                              ret_value = (FAIL);
                              goto done;
                            }
                      }     /* end else */
                }   /* end if */
          }     /* end if */

    ret_value = (FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Vgetnext  */

/* ================================================================= */
/*
   *    Vgetname
   *    returns the vgroup's name
   *   ASSUME that vgname has been allocated large enough to hold
   *   the name
   *
 */

int32
Vgetname(int32 vkey, char *vgname)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vgetname");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vgetname);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey) || vgname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    HDstrcpy(vgname, vg->vgname);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vgetname);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vgetname */

/* ================================================================= */
/*
   *    Vgetclass
   *    returns the vgroup's class name
   *   ASSUME that vgclass has been allocated large enough to hold
   *   the name
   *
 */

int32
Vgetclass(int32 vkey, char *vgclass)
{
    vginstance_t *v;
    VGROUP     *vg;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "Vgetclass");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vgetclass);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey) || vgclass == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    HDstrcpy(vgclass, vg->vgclass);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vgetclass);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vgetclass */

/* ================================================================= */
/*
   *    Vinquire
   *
   *    General inquiry routine for VGROUP.
   *
   *    output parameters:
   *                    nentries - no of entries in the vgroup
   *                    vgname  - the vgroup's name
   *
   *    RETURNS FAIL if error
   *    RETURNS SUCCEED if ok
   *
 */

intn
Vinquire(int32 vkey, int32 *nentries, char *vgname)
{
    vginstance_t *v;
    VGROUP     *vg;
    intn    ret_value = SUCCEED;
    CONSTR(FUNC, "Vinquire");

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_Vinquire);
#endif /* HAVE_PABLO */

    if (!VALIDVGID(vkey))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (v = (vginstance_t *) vginstance(VGID2VFILE(vkey), (uint16) VGID2SLOT(vkey))))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vg = v->vg;
    if (vg == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    if (vg->otag != DFTAG_VG)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if (vgname != NULL)
        HDstrcpy(vgname, vg->vgname);
    if (nentries != NULL)
        *nentries = vg->nvelt;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
  TRACE_OFF(V_mask, ID_Vinquire);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* Vinquire */

/* ================================================================= */

/* ---------------------------- Vopen ------------------------- */
/*
   *
   * This routine will replace the code segment " Hopen(); Vinitialize(f)".
   * Thus, if Vopen is used, do not call Vinitialize after that.
   *
   * Similar to Hopen().
   * INPUTS:
   *            char * path     - file name.
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

HFILEID
Vopen(char *path, intn acc_mode, int16 ndds)
{
#ifdef LATER
    CONSTR(FUNC, "Vopen");
#endif
    HFILEID    ret_value;
    HFILEID     f;

    f = Hopen(path, acc_mode, ndds);
    if (f == FAIL)
      {
        ret_value = (FAIL);
        goto done;
      }

    Vinitialize(f);
    ret_value = (f);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
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

intn
Vclose(HFILEID f)
{
#ifdef LATER
    CONSTR(FUNC, "Vclose");
#endif
    intn ret_value;

    Vfinish(f);
    ret_value = (Hclose(f));

    return ret_value;
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
    VOIDP       v;
    vfile_t    *vf;
    VOIDP      *t;
    int32       key;
    filerec_t  *file_rec;       /* file record */
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "Vdelete");

    if (vgid < 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* convert file id to file record and check for write-permission */
    file_rec = FID2REC(f);
    if(!(file_rec->access&DFACC_WRITE))
        HGOTO_ERROR(DFE_BADACC, FAIL);

    /* Check if vfile buffer has been allocated */
    if (vfile == NULL)
      {
          vfile = (vfile_t *) HDmalloc(MAX_VFILE * sizeof(vfile_t));
          if (vfile == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          /* zero the space */
          vfile = (vfile_t *) HDmemset(vfile, 0, (MAX_VFILE * sizeof(vfile_t)));
      }

    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, FAIL);

    key = VGSLOT2ID(f, vgid);

    t = (VOIDP *) tbbtdfind(vf->vgtree, (VOIDP) &key, NULL);

    if (t == NULL)
      {
        ret_value = (FAIL);
        goto done;
      }

    v = tbbtrem((TBBT_NODE **) vf->vgtree, (TBBT_NODE *) t, NULL);
    if (v)
        vdestroynode((VOIDP) v);

    Hdeldd(f, DFTAG_VG, (uint16) vgid);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* Vdelete */

/*--------------------------------------------------------------------------
 NAME
    VPshutdown
 PURPOSE
    Terminate various static buffers.
 USAGE
    intn VPshutdown()
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Free various buffers allocated in the V routines.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    Should only ever be called by the "atexit" function HDFend
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn VPshutdown(void)
{
    if(vfile!=NULL)
      {
          HDfree(vfile);
          vfile=NULL;
      } /* end if */
    return(SUCCEED);
} /* end VPshutdown() */

