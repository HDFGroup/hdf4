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
* vio.c
* Part of the HDF Vset interface.
*
* VDATAs are handled by routines in here.
* PRIVATE functions manipulate vsdir and are used only within this file.
* PRIVATE data structures in here pertain to vdatas in vsdir only.
*

LOCAL ROUTINES
 None
EXPORTED ROUTINES
 vinstance     -- Looks thru vstab for vsid and return the addr of the vdata 
                   instance where vsid is found.
 vexistvs      -- Tests if a vdata with id vsid is in the file's vstab.
 vpackvs       -- Packs a VDATA structure into a compact form suitable for 
                   storing in the HDF file.
 vunpackvs     -- Convert a packed form(from HDF file) to a VDATA structure.
                   This routine will also initalize the VDATA structure as 
                   much as it can.
 vsdestroynode -- Frees B-Tree nodes.
 VSattach      -- Attaches/Creates a new vs in vg depending on "vsid" value.
 VSdetach      -- Detaches vs from vstab.
 VSappendable  -- Make it possible to append unlimitedly to an existing VData.
 VSgetid       -- Returns the id of the next VDATA from the file.
 VSQuerytag    -- Return the 'otag' of the given Vdata.
 VSQueryref    -- Return the ref of the given Vdata.
 vswritelist   -- Return the writelist of a Vdata.
 VSgetversion  -- Return the version number of a Vdata.
 VSdelete      -- Remove a Vdata from its file.  This function will both 
                   remove the Vdata from the internal Vset data structures 
                   as well as from the file.
*************************************************************************/

#include "vg.h"

/* Private Function Prototypes */
PRIVATE intn vunpackvs
            (VDATA * vs, uint8 buf[], int32 len);

/* vpackvs is prototyped in vg.h since vconv.c needs to call it */

/* ------------------------------------------------------------------ */
/*
   * Looks thru vstab for vsid and return the addr of the vdata instance
   * where vsid is found.
   * RETURNS NULL if error or not found.
   * RETURNS vsinstance_t pointer if ok.
   *
 */
vsinstance_t *
vsinst(HFILEID f, uint16 vsid)
{
    CONSTR(FUNC, "vsinstance");
    VOIDP      *t;
    vfile_t *vf;
    int32       key;
    vsinstance_t *ret_value = NULL; /* FAIL */

    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, NULL);

    /* tbbtdfind returns a pointer to the vsinstance_t pointer */
    key = (int32)vsid;
    if (( t = (VOIDP *) tbbtdfind(vf->vstree, (VOIDP) &key, NULL))== NULL)
        HGOTO_ERROR(DFE_NOMATCH,NULL);
    ret_value = ((vsinstance_t *) * t);  /* return the actual vsinstance_t ptr */

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* vsinstance */

/* --------------------------- vexists --------------------------------------- */
/*
   * Tests if a vdata with id vsid is in the file's vstab.
   * returns FAIL if not found,
   * returns TRUE if found.
 */

int32
vexistvs(HFILEID f, uint16 vsid)
{
  return ((NULL==vsinst(f, vsid)) ? FAIL : TRUE);
}   /* vexistvs */

/* ------------------------------------------------------------------ */
/*
   The following 2 routines, vpackvs and vunpackvs, packs and unpacks
   a VDATA structure into a compact form suitable for storing in the HDF file.
 */

/****
CONTENTS of VS stored in HDF file with tag VSDESCTAG:
    int16       interlace
    int32       nvertices
    uint16       vsize
    int16       nfields

    uint16       isize[1..nfields] (internal size of each field)
    uint16       off[1..nfields] (internal offset of each field)
    char        fname[1..nfields][FIELDNAMELENMAX]
    char        vsname[VSNAMELENMAX]
    char        vsclass[VSNAMELENMAX]
    uint16      extag, exref
    uint32      flags (for vset version 4 or higher )
    int32       nattrs (if bit0 of flags is set)
    uint16      atags[1..nattrs], arefs[1..nattrs]
    int16       version, more
****/

/* ------------------------------- vpackvs ----------------------------------- */
/*
   The following 2 PRIVATE routines, vpackvs and vunpackvs, packs and unpacks
   a VDATA structure into a compact form suitable for storing in the HDF file.
 */

/****
CONTENTS of VS stored in HDF file with tag DFTAG_VH:
    int16       interlace
    int32       nvertices
    uint16      vsize
    int16       nfields

    uint16      isize[1..nfields] (internal size of each field)
    uint16      off[1..nfields] (internal offset of each field)
    char        fname[1..nfields][FIELDNAMELENMAX]
    char        vsname[VSNAMELENMAX]
    char        vsclass[VSNAMELENMAX]

****/

/*
   convert a vs struct to a vspack suitable for storage in a HDF file
 */

intn
vpackvs(VDATA * vs, uint8 buf[], int32 *size)
{
    CONSTR(FUNC, "vpackvg");

    int32 i;
    uint8 *bb;
    intn  ret_value = SUCCEED;

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_vpackvs);
#endif /* HAVE_PABLO */

    HEclear();
    bb = &buf[0];

    /* save the interlace */
    INT16ENCODE(bb, vs->interlace);

    /* save nvertices */
    INT32ENCODE(bb, vs->nvertices);

    /* save ivsize */
    UINT16ENCODE(bb, vs->wlist.ivsize);

    /* save nfields */
    INT16ENCODE(bb, vs->wlist.n);

    for (i = 0; i < vs->wlist.n; i++)   /* save the type */
        INT16ENCODE(bb, vs->wlist.type[i]);

    /* save the isize */
    for (i = 0; i < vs->wlist.n; i++)
        UINT16ENCODE(bb, vs->wlist.isize[i]);

    for (i = 0; i < vs->wlist.n; i++)   /* save the offset */
        UINT16ENCODE(bb, vs->wlist.off[i]);

    for (i = 0; i < vs->wlist.n; i++)   /* save the order */
        UINT16ENCODE(bb, vs->wlist.order[i]);

    /* save each field length and name - omit the null */
    for (i = 0; i < vs->wlist.n; i++)
      {
          INT16ENCODE(bb, HDstrlen(vs->wlist.name[i]));

          HDstrcpy((char *) bb, vs->wlist.name[i]);
          bb += HDstrlen(vs->wlist.name[i]);
      }

    /* save the vsnamelen and vsname - omit the null */
    INT16ENCODE(bb, HDstrlen(vs->vsname));

    HDstrcpy((char *) bb, vs->vsname);
    bb += HDstrlen(vs->vsname);

    /* save the vsclasslen and vsclass- omit the null */
    INT16ENCODE(bb, HDstrlen(vs->vsclass));

    HDstrcpy((char *) bb, vs->vsclass);
    bb += HDstrlen(vs->vsclass);

    /* save the expansion tag/ref pair */
    UINT16ENCODE(bb, vs->extag);
    UINT16ENCODE(bb, vs->exref);

    /* save the version field - to version_3 now if no new feature */
    INT16ENCODE(bb, vs->version);

    /* save the 'more' field - NONE now */
    INT16ENCODE(bb, vs->more);

    if (vs->flags != 0)   {  /* save the flags and update version # */
       UINT32ENCODE(bb, vs->flags);
       if (vs->flags & VS_ATTR_SET) { /* save attributes */
          INT32ENCODE(bb, vs->nattrs);
          for (i=0; i<vs->nattrs; i++)  {
              INT32ENCODE(bb, vs->alist[i].findex);
              UINT16ENCODE(bb, vs->alist[i].atag);
              UINT16ENCODE(bb, vs->alist[i].aref);
          }   /* for */
       }  /* attr set */ 
    }     /* flags set */   
   /* duplicate 'version' and 'more' - for new version of libraries */
   /* see the documentation in vattr.c */
    INT16ENCODE(bb, vs->version);

    /* save the 'more' field - NONE now */
    INT16ENCODE(bb, vs->more);

    *size = (int32) (bb - buf) + 1;
    *bb = 0;
done:
    if (ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(V_mask, ID_vpackvs);
#endif /* HAVE_PABLO */

  return ret_value;

}   /* vpackvs */

/* ----------------------------- vunpackvs ------------------------------------- */
/*
   Convert a packed form(from HDF file)  to a VDATA structure.
   This routine will also initalize the VDATA structure as much as it can.
 */

PRIVATE     intn
vunpackvs(VDATA * vs, uint8 buf[], int32 len)
{
    CONSTR(FUNC, "vunpackvs");
    uint8      *bb;
    int32       i;
    int16       int16var, temp;
    int32       ret_value = SUCCEED;

#ifdef HAVE_PABLO
  TRACE_ON(V_mask, ID_vunpackvs);
#endif /* HAVE_PABLO */

    HEclear();
    /* '5' is a magic number, the exact amount of space for 2 uint16's *
/
    /* the magic number _should_ be '4', but the size of the Vdata */
    /* information is incorrectly calculated (in vpackvs() above) when t
he */
    /* info is written to the file and it's too late to change it now :-
( */
    /* get version number first -- this is different from version 3
       vdata interface */
    bb = &buf[len - 5];
    UINT16DECODE(bb, vs->version); /* retrieve the vg's version field */
    UINT16DECODE(bb, vs->more);     /* retrieve the vg's more field */
    bb = &buf[0];

    if (vs->version <= 4)   { 
       /* retrieve interlace */
       INT16DECODE(bb, vs->interlace);
       /* retrieve nvertices */
       INT32DECODE(bb, vs->nvertices);
       /* retrieve tore ivsize */
       UINT16DECODE(bb, vs->wlist.ivsize);
       /* retrieve nfields */
       INT16DECODE(bb, vs->wlist.n);
       /* Can't really check for malloc failure... -QAK */
       vs->wlist.type=HDmalloc(sizeof(int16)*vs->wlist.n);
       for (i = 0; i < vs->wlist.n; i++)   /* retrieve the type */
           INT16DECODE(bb, vs->wlist.type[i]);
       vs->wlist.isize=HDmalloc(sizeof(uint16)*vs->wlist.n);
       for (i = 0; i < vs->wlist.n; i++)   /* retrieve the isize */
           UINT16DECODE(bb, vs->wlist.isize[i]);

        vs->wlist.off=HDmalloc(sizeof(uint16)*vs->wlist.n);
        for (i = 0; i < vs->wlist.n; i++)   /* retrieve the offset */
            UINT16DECODE(bb, vs->wlist.off[i]);

        vs->wlist.order=HDmalloc(sizeof(uint16)*vs->wlist.n);
        for (i = 0; i < vs->wlist.n; i++)   /* retrieve the order */
            UINT16DECODE(bb, vs->wlist.order[i]);

        /* retrieve the field names (and each field name's length)  */
        vs->wlist.name=HDmalloc(sizeof(char *)*vs->wlist.n);
        for (i = 0; i < vs->wlist.n; i++) {
            INT16DECODE(bb, int16var);    /* this gives the length */
            vs->wlist.name[i]=HDmalloc((int16var+1)*sizeof(char));
            HIstrncpy(vs->wlist.name[i], (char *) bb, int16var + 1);
            bb += int16var;
        }

       /* retrieve the vsname (and vsnamelen)  */
       INT16DECODE(bb, int16var);  /* this gives the length */

       HIstrncpy(vs->vsname, (char *) bb, int16var + 1);
       bb += int16var;

       /* retrieve the vsclass (and vsclasslen)  */
       INT16DECODE(bb, int16var);  /* this gives the length */

       HIstrncpy(vs->vsclass, (char *) bb, int16var + 1);
       bb += int16var;

       /* retrieve the expansion tag and ref */
       UINT16DECODE(bb, vs->extag);
       UINT16DECODE(bb, vs->exref);

       /* retrieve the middle version field */
       INT16DECODE(bb, temp);
       if (temp != vs->version) 
           HGOTO_ERROR(DFE_BADVH, FAIL);
       /* retrieve the 'more' field */
       INT16DECODE(bb, temp);
       if (temp != vs->more) 
           HGOTO_ERROR(DFE_BADVH, FAIL);
       if (vs->version == VSET_NEW_VERSION) { /* new features exist */
           UINT32DECODE(bb, vs->flags);
           if (vs->flags & VS_ATTR_SET)  {    /* get attr info */
              INT32DECODE(bb, vs->nattrs);
if (NULL==(vs->alist=(vs_attr_t *)HDmalloc(vs->nattrs*sizeof(vs_attr_t))))
                  HGOTO_ERROR(DFE_NOSPACE, FAIL);
              for (i=0; i<vs->nattrs; i++)  {
                  INT32DECODE(bb, vs->alist[i].findex);
                  UINT16DECODE(bb, vs->alist[i].atag); 
                  UINT16DECODE(bb, vs->alist[i].aref); 
              }  /* for */
           }     /* attr set */
       }   /* new version */
       if (vs->version <= VSET_OLD_TYPES)
          for (i = 0; i < vs->wlist.n; i++)   /* save the type */
            vs->wlist.type[i] = map_from_old_types(vs->wlist.type[i]);
    /* --- EXTRA --- fill in the machine-dependent size fields */
       vs->wlist.esize=HDmalloc(sizeof(uint16)*vs->wlist.n);
       for (i = 0; i < vs->wlist.n; i++)
           vs->wlist.esize[i] = (uint16) (vs->wlist.order[i] *
              DFKNTsize((int32) vs->wlist.type[i] | (int32) DFNT_NATIVE));
    }  /* if version <= 4 */
done:
    if (ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(V_mask, ID_vunpackvs);
#endif /* HAVE_PABLO */

  return ret_value;


}   /* vunpackvs */

/* ---------------------------- vsdestroynode ------------------------- */
/*
   Frees B-Tree nodes

   *** Only called by B-tree routines, should _not_ be called externally ***
 */
VOID
vsdestroynode(VOIDP n)
{
    VDATA      *vs;
    intn i;

    vs = ((vsinstance_t *) n)->vs;
    if (vs != NULL)
      {
        /* Free the dynamicly allocated VData fields */
        for(i=0; i<vs->wlist.n; i++)
            HDfree(vs->wlist.name[i]);
        HDfree(vs->wlist.name);
        HDfree(vs->wlist.type);
        HDfree(vs->wlist.off);
        HDfree(vs->wlist.isize);
        HDfree(vs->wlist.order);
        HDfree(vs->wlist.esize);
        if(vs->rlist.item!=NULL)
            HDfree(vs->rlist.item);
        if (vs->alist != NULL)
            HDfree(vs->alist);
        HDfree((VOIDP) vs);
      } /* end if */

    HDfree((VOIDP) n);

}   /* vsdestroynode */

/*--------------------------------------------------------------------------
 NAME
    VSPgetinfo
 PURPOSE
    Read in the "header" information about the Vdata.
 USAGE
    VDATA *VSPgetinfo(f,ref)
        HFILEID f;              IN: the HDF file id
        uint16 ref;             IN: the tag & ref of the Vdata 
 RETURNS
    Return a pointer to a VDATA filled with the Vdata information on success,
    NULL on failure.
 DESCRIPTION
    This routine pre-reads the header information for a Vdata into memory
    so that it can be accessed more quickly by the routines that need it.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
VDATA *VSPgetinfo(HFILEID f,uint16 ref)
{
    CONSTR(FUNC, "VSPgetinfo");
	VDATA 		*vs;  			 /* new vdata to be returned */
    uint8       *vspack;
    int32       vh_length;      /* length of the vdata header */
    VDATA *ret_value = NULL; /* FAIL */
 
    /* allocate space for vs,  & zero it out  */
    if ( (vs=(VDATA*) HDmalloc (sizeof(VDATA))) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, NULL);
 
    /* need to fetch from file */
    if ((vh_length= Hlength (f,DFTAG_VH,ref))==FAIL)
        HGOTO_ERROR(DFE_BADLEN, NULL);
    if ( (vspack= (uint8 *) HDmalloc (vh_length)) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, NULL);
    if (Hgetelement(f,DFTAG_VH,ref,vspack) == FAIL) {
        HDfree((VOIDP)vspack);
        HGOTO_ERROR(DFE_NOVS, NULL);
      } /* end if */
 
    vs->wlist.n = vs->rlist.n = 0;
    vs->rlist.item=NULL;
 
    /* unpack the vs, then init all other fields in it */
    vs->otag    = DFTAG_VH;
    vs->oref    = ref;
    vs->f   = f;
    vs->marked  = 0;
    vs->nusym   = 0;
    vs->usym=NULL;
    vs->flags = 0;
    vs->nattrs = 0;
    vs->alist = NULL;
    if (FAIL == vunpackvs (vs,vspack, vh_length))
       HGOTO_ERROR(DFE_INTERNAL, NULL);
 
    HDfree((VOIDP)vspack);
 
    ret_value = (vs);

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
} /* end VSPgetinfo() */

/* ------------------------------------------------------------------ */

/* ***************************************************************
   NEW VSattach:
   (a)  if vsid == -1
   if "r" access return error.
   if "w" access
   create a new vs in vg and attach it.
   add to vsdir, set nattach= 1, nvertices = 0.

   (b)  if (vsid > 0)
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
int32 
VSattach(HFILEID f, int32 vsid, const char *accesstype)
{
    CONSTR(FUNC, "VSattach");
    VDATA      *vs;             /* new vdata to be returned */
    int32       acc_mode;
    vsinstance_t *w;
    vfile_t    *vf;
    int32      ret_value = FAIL;

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSattach);
#endif /* HAVE_PABLO */

    if ((f == FAIL) || (vsid < -1))
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, FAIL);

    if (accesstype[0] == 'R' || accesstype[0] == 'r')
        acc_mode = 'r';
    else if (accesstype[0] == 'W' || accesstype[0] == 'w')
        acc_mode = 'w';
    else
        HGOTO_ERROR(DFE_BADACC, FAIL);

    if (vsid == -1)
      {     /* ---------- VSID IS -1 ----------------------- */
          if (acc_mode == 'r')
              HGOTO_ERROR(DFE_BADACC, FAIL);

          /* otherwise 'w' */
          /* allocate space for vs,  & zero it out  */
          if ((vs = (VDATA *) HDmalloc(sizeof(VDATA))) == NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          vs->nvertices = 0;
          vs->wlist.n = vs->rlist.n = 0;
          vs->rlist.item=NULL;
          vs->islinked = FALSE;
          vs->nusym = 0;
          vs->usym=NULL;

          vs->otag = DFTAG_VH;
          vs->oref = Hnewref(f);
          if (vs->oref == 0)
            {
                HERROR(DFE_NOREF);
                HDfree((VOIDP) vs);
                ret_value = (FAIL);
                goto done;
            }

          vs->vsname[0] = '\0';
          vs->interlace = FULL_INTERLACE;   /* DEFAULT */
          vs->access = 'w';
          vs->f = f;
          vs->marked = 0;
          vs->new_h_sz = 0;
          vs->vsclass[0] = '\0';
          vs->extag = 0;
          vs->exref = 0;
          vs->more = 0;
          vs->version = VSET_VERSION;
          vs->flags = 0;
          vs->nattrs = 0;
          vs->alist = NULL;
          vs->aid = 0;

          /* attach new vs to file's vstab */
          if (NULL == (w = (vsinstance_t *) HDmalloc(sizeof(vsinstance_t))))
              HGOTO_ERROR(DFE_NOSPACE, FAIL);

          vf->vstabn++;
          w->key = (int32) vs->oref;  /* set the key for the node */
          w->ref = (intn) vs->oref;
          w->vs = vs;
          w->nattach = 1;
          w->nvertices = 0;
          tbbtdins(vf->vstree, (VOIDP) w, NULL);    /* insert the vs instance in B-tree */

          vs->instance = w;
      }     /* end of case where vsid is -1 */
    else
      { /*  --------  VSID IS NON_NEGATIVE ------------- */
        if (NULL == (w = vsinst(f, (uint16) vsid)))
            HGOTO_ERROR(DFE_VTAB, FAIL);

        if (acc_mode == 'r')
          {     /* reading an existing vdata */
              /* this vdata is already attached for 'r', ok to do so again */
              if (w->nattach && w->vs->access == 'r')
                  w->nattach++;
              else
                {
                  vs = w->vs;

                  vs->access = 'r';
                  vs->aid = Hstartread(vs->f, VSDATATAG, vs->oref);
                  if (vs->aid == FAIL)
                    HGOTO_ERROR(DFE_BADAID, FAIL);

                  vs->instance = w;

                  /* attach vs to vsdir  at the vdata instance w */
                  w->nattach = 1;
                  w->nvertices = vs->nvertices;
                } /* end else */
          }		/* end of case where vsid is positive, and "r"  */
        else
          {		/* writing to an existing vdata */
              if (w->nattach)	/* vdata previously attached before */
                  HGOTO_ERROR(DFE_BADATTACH, FAIL);

              vs = w->vs;

              vs->access = 'w';
              vs->aid = Hstartwrite(vs->f, VSDATATAG, vs->oref, 0);
              if (vs->aid == FAIL)
                HGOTO_ERROR(DFE_BADAID, FAIL);

              vs->instance = w;
              vs->new_h_sz = 0;

              /* attach vs to vsdir  at the vdata instance w */
              w->nattach = 1;
              w->nvertices = vs->nvertices;
          }		/* end of case where vsid is positive, and "w"  */
      } /* end else */

    ret_value = HAregister_atom(VSIDGROUP,w);

    /* Make VDatas appendable by default */
    if (FAIL == VSappendable(ret_value,VDEFAULTBLKSIZE))
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSattach);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSattach */

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
int32 
VSdetach(int32 vkey)
{
    int32       i, ret, vspacksize;
    uint8      *vspack;
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSdetach");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSdetach);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    w->nattach--;

    /* --- case where access was 'r' --- */
    if (vs->access == 'r')
      {
          if (w->nattach == 0)
            {
              Hendaccess(vs->aid);
              if(HAremove_atom(vkey)==NULL)
                  HGOTO_ERROR(DFE_INTERNAL, FAIL);
            } /* end if */
          HGOTO_DONE(SUCCEED);
      }

    /* --- case where access was 'w' --- */
    if (w->nattach != 0)
        HGOTO_ERROR(DFE_CANTDETACH, FAIL);

    if (vs->marked)
      {	  /* if marked , write out vdata's VSDESC to file */
        if ((vspack = (uint8 *) HDmalloc(sizeof(VWRITELIST) + 
           vs->nattrs*sizeof(vs_attr_t) + sizeof(VDATA) + 1)) == NULL)
            HGOTO_ERROR(DFE_NOSPACE, FAIL);
        if (FAIL == vpackvs(vs, vspack, &vspacksize))
            HGOTO_ERROR(DFE_INTERNAL, FAIL);
          /* the old one should be blown away if VH size changed  */
        if (vs->new_h_sz)
            Hdeldd(vs->f, DFTAG_VH, vs->oref);
        ret = Hputelement(vs->f, VSDESCTAG, vs->oref, vspack, vspacksize);
        HDfree((VOIDP) vspack);
        if (ret == FAIL)
            HGOTO_ERROR(DFE_WRITEERROR, FAIL);
        vs->marked = 0;
        vs->new_h_sz = 0;
      }

    /* remove all defined symbols */
    for (i = 0; i < vs->nusym; i++)
        HDfree((VOIDP) vs->usym[i].name);
    if(vs->usym!=NULL)
        HDfree(vs->usym);   /* free the actual array */
    vs->nusym = 0;
    vs->usym=NULL;

    Hendaccess(vs->aid);

    if(HAremove_atom(vkey)==NULL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSdetach);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSdetach */

/* -------------------------- VSappendable -------------------------------- */
/*
 * make it possible to append unlimitedly to an existing VData
 *
 *  Returns: SUCCEED, or FAIL for error
 *
 * undocumented
 *
 */
int32 
VSappendable(int32 vkey, int32 blk)
{
    int32       blksize, curr_size=0;
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSappendable");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSappendable);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if(vs->nvertices!=0)
        curr_size = vs->nvertices * vs->wlist.ivsize;

    if (blk > 0)
        blksize = blk;
    else if (vs->nvertices!=0 && (curr_size > VDEFAULTBLKSIZE))
        blksize = curr_size;
    else
        blksize = VDEFAULTBLKSIZE;

#ifdef OLD_WAY
    if(vs->aid!=0)
        Hendaccess(vs->aid);

    vs->aid = HLcreate(vs->f, VSDATATAG, vs->oref, blksize, VDEFAULTNBLKS);
    if (vs->aid == FAIL)
        ret_value = FAIL;
#else /* OLD_WAY */
    if(vs->aid==0)
        vs->aid = Hstartaccess(vs->f, VSDATATAG, vs->oref, DFACC_RDWR|DFACC_APPENDABLE);
    else
        ret_value = Happendable(vs->aid);
#endif /* OLD_WAY */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSappendable);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VSappendable */

/* ======================================================= */

/*
   returns the id of the next  VDATA from the file f .
   (vsid = -1 gets the 1st vDATA).
   RETURNS -1 on error.
   RETURNS vdata id (0 or +ve integer)
 */
int32
VSgetid(HFILEID f, int32 vsid)
{
    vsinstance_t *w;
    vfile_t    *vf;
    VOIDP      *t;
    int32       key;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetid");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSgetid);
#endif /* HAVE_PABLO */

    if (vsid < -1)
        HGOTO_ERROR(DFE_ARGS, FAIL);
    if (NULL == (vf = Get_vfile(f)))
        HGOTO_ERROR(DFE_FNF, FAIL);

    if (vsid == -1)
      {
        if ((vf->vstree==NULL) || (NULL == (t = (VOIDP *) tbbtfirst((TBBT_NODE *) * (vf->vstree)))))
          {
            HGOTO_DONE(FAIL);
          }
        else
          {
              w = (vsinstance_t *) * t;   /* get actual pointer to the vsinstance_t */
              HGOTO_DONE(w->ref);    /* rets 1st vdata's ref */
          }   /* end else */
      }

    /* tbbtdfind returns a pointer to the vsinstance_t pointer */
    key = (int32)vsid;
    t = (VOIDP *) tbbtdfind(vf->vstree, (VOIDP) &key, NULL);
    if (t == NULL)  /* couldn't find the old vsid */
        ret_value = (FAIL);
    else if (NULL == (t = (VOIDP *) tbbtnext((TBBT_NODE *) t)))     /* get the next node in the tree */
        ret_value = (FAIL);
    else
      {
          w = (vsinstance_t *) * t;     /* get actual pointer to the vsinstance_t */
          ret_value = (w->ref);  /* rets vdata's ref */
      }     /* end else */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSgetid);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VSgetid */

/* -------------- Return the otag of a VData----------------- */
/*
   Return the 'otag' of the given Vdata
   Return FAIL on failure
 */

int32
VSQuerytag(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSQuerytag");

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->otag);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}	/* VSQuerytag */

/* -------------- Return the oref of a VData----------------- */
/*
   Return the ref of the given Vdata
   Return FAIL on failure
 */
int32
VSQueryref(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSQueryref");

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->oref);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}	/* VSQueryref */

/* -------------- Return the writelist of a VData----------------- */
DYN_VWRITELIST *
vswritelist(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    DYN_VWRITELIST *ret_value = NULL; /* FAIL */
    CONSTR(FUNC, "VSwritelist");

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, NULL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, NULL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
        HGOTO_ERROR(DFE_ARGS, NULL);

    ret_value = (&(vs->wlist));

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* end vswritelist() */

/* -------------- Return the version number of a VData----------------- */

int32
VSgetversion(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetversion");

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, 0);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, 0);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
        HGOTO_ERROR(DFE_ARGS, 0);

    ret_value = (vs->version);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* end VSgetversion() */

/* ------------------------------- VSdelete -------------------------------- */
/*
   Remove a Vdata from its file.  This function will both remove the Vdata
   from the internal Vset data structures as well as from the file.

   (i.e. it calls tbbt_delete() and Hdeldd())

   Return FAIL / SUCCEED
 */
int32
VSdelete(int32 f, int32 vsid)
{
    VOIDP       v;
    vfile_t    *vf;
    VOIDP      *t;
    int32       key;
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "VSdelete");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSdelete);
#endif /* HAVE_PABLO */

    if (vsid < -1)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    if (NULL == (vf = Get_vfile(f)))
      HGOTO_ERROR(DFE_FNF,FAIL);

    key = vsid;
    if (( t = (VOIDP *) tbbtdfind(vf->vstree, (VOIDP) &key, NULL))== NULL)
        HGOTO_DONE(FAIL);

    v = tbbtrem((TBBT_NODE **) vf->vstree, (TBBT_NODE *) t, NULL);
    if (v)
        vsdestroynode((VOIDP) v);

    Hdeldd(f, DFTAG_VS, (uint16) vsid);
    Hdeldd(f, DFTAG_VH, (uint16) vsid);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSdelete);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSdelete */

/* -------------- Return the number of fields of a VData----------------- */
intn 
VSQuerynfields(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    intn ret_value = FAIL; /* FAIL */
    CONSTR(FUNC, "VSQuerynflds");

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, NULL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, NULL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
        HGOTO_ERROR(DFE_ARGS, NULL);
    ret_value = vs->wlist.n;

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */

  return ret_value;
}   /* end VSQuerynfields() */

