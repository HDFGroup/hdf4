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
* vsetf.c
* Part of the HDF VSet interface.
*

LOCAL ROUTINES

EXPORTED ROUTINES
 VSIZEOF      -- returns the machine size of a field type.
 VSsetfields  -- sets the fields in a vdata for reading or writing.
                 Truncates each field to max length of FIELDNAMELENMAX.
 VSfdefine    -- Defines a (one) new field within the vdata.
 VFnfields    -- Return the number of fields in this Vdata.
 VFfieldname  -- Return the name of the given field in this Vdata. 
 VFfieldtype  -- Return the type of the given field in this Vdata.
 VFfieldisize -- Return the internal (HDF) size of the given 
                  field in this Vdata. 
 VFfieldesize -- Return the external (local machine) size of the given 
                  field in this Vdata.
 VFfieldorder -- Return the order of the given field in this Vdata.
************************************************************************/

#include "vg.h"

/*
   ** ==================================================================
   ** PRIVATE data areas and routines
   ** ==================================================================
   * */

PRIVATE SYMDEF rstab[] =
{
    {"PX", DFNT_FLOAT32, SIZE_FLOAT32, 1},
    {"PY", DFNT_FLOAT32, SIZE_FLOAT32, 1},
    {"PZ", DFNT_FLOAT32, SIZE_FLOAT32, 1},

    {"IX", DFNT_INT32, SIZE_INT32, 1},
    {"IY", DFNT_INT32, SIZE_INT32, 1},
    {"IZ", DFNT_INT32, SIZE_INT32, 1},

    {"NX", DFNT_FLOAT32, SIZE_FLOAT32, 1},
    {"NY", DFNT_FLOAT32, SIZE_FLOAT32, 1},
    {"NZ", DFNT_FLOAT32, SIZE_FLOAT32, 1},

};

#define NRESERVED ( sizeof(rstab)/sizeof(SYMDEF) )

/* ------------------------------------------------------------------ */
/*
   ** sets the fields in a vdata for reading or writing
   ** RETURNS FAIL if error, and SUCCEED if ok.
   ** truncates each field to max length of  FIELDNAMELENMAX.
 */
intn 
VSsetfields(int32 vkey, const char *fields)
{
    char      **av;
    int32       ac, found;
    intn j, i;
    uint16       uj;
    uint16       order;
    int32       value;
    DYN_VREADLIST  *rlist;
    DYN_VWRITELIST *wlist;
    vsinstance_t *w;
    VDATA      *vs;
    intn       ret_value = FAIL;
    CONSTR(FUNC, "VSsetfields");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSsetfields);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    if ((scanattrs(fields, &ac, &av) == FAIL) || (ac == 0))
        HGOTO_ERROR(DFE_BADFIELDS, FAIL);

     /* check number of fields limit  */
     if (ac > VSFIELDMAX)
         HGOTO_ERROR(DFE_SYMSIZE,FAIL);

    /*
     * write to an empty vdata : set the write list but do not set the
     *   read list cuz there is nothing there to read yet...
     */
    if (vs->access == 'w' && vs->nvertices == 0)
      {
          wlist=&(vs->wlist);
          wlist->ivsize = 0;
          wlist->n = 0;

          /* allocate space for the internal WRITELIST structures */
          if((wlist->type=HDmalloc(sizeof(int16)*ac))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
          if((wlist->isize=HDmalloc(sizeof(uint16)*ac))==NULL)
            {
              HDfree(wlist->type);
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */
          if((wlist->off=HDmalloc(sizeof(uint16)*ac))==NULL)
            {
              HDfree(wlist->isize);
              HDfree(wlist->type);
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */
          if((wlist->order=HDmalloc(sizeof(uint16)*ac))==NULL)
            {
              HDfree(wlist->off);
              HDfree(wlist->isize);
              HDfree(wlist->type);
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */
          if((wlist->name=HDmalloc(sizeof(char *)*ac))==NULL)
            {
              HDfree(wlist->order);
              HDfree(wlist->off);
              HDfree(wlist->isize);
              HDfree(wlist->type);
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */
          if((wlist->esize=HDmalloc(sizeof(uint16)*ac))==NULL)
            {
              HDfree(wlist->name);
              HDfree(wlist->order);
              HDfree(wlist->off);
              HDfree(wlist->isize);
              HDfree(wlist->type);
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            } /* end if */

          for (i = 0; i < ac; i++)
            {
                found = FALSE;
                /* --- first look in the user's symbol table --- */
                for (j = 0; j < vs->nusym; j++)
                    if (!HDstrcmp(av[i], vs->usym[j].name))
                      {
                          found = TRUE;

                            
                          if((wlist->name[wlist->n]=HDstrdup(vs->usym[j].name))==NULL)
                            {
                              HDfree(wlist->esize);
                              HDfree(wlist->name);
                              HDfree(wlist->order);
                              HDfree(wlist->off);
                              HDfree(wlist->isize);
                              HDfree(wlist->type);
                              HGOTO_ERROR(DFE_NOSPACE,FAIL);
                            } /* end if */
                          order = vs->usym[j].order;
                          wlist->type[wlist->n] = vs->usym[j].type;
                          wlist->order[wlist->n] = order;

                          value = order * DFKNTsize(vs->usym[j].type | DFNT_NATIVE);
/* should be ok if field size in memory > MAX_FIELD_SIZE. ( 65535 after 40r1p1)  
                          if (value > MAX_FIELD_SIZE)
                              HGOTO_ERROR(DFE_BADFIELDS, FAIL);
*/
                          wlist->esize[wlist->n] = (uint16) value;

                          value = order * vs->usym[j].isize;
                          if (value > MAX_FIELD_SIZE)
                              HGOTO_ERROR(DFE_BADFIELDS, FAIL);
                          wlist->isize[wlist->n] = (uint16) value;

                          value = (int32) wlist->ivsize + (int32) (wlist->isize[wlist->n]);
                          if (value > MAX_FIELD_SIZE)
                              HGOTO_ERROR(DFE_BADFIELDS, FAIL);
                          wlist->ivsize = (uint16) value;

                          wlist->n++;
                          break;
                      }

                /* --- now look in the reserved symbol table --- */
                if (!found)
                  {
                      for (j = 0; j < NRESERVED; j++)
                          if (!HDstrcmp(av[i], rstab[j].name))
                            {
                                found = TRUE;

                                if((wlist->name[wlist->n]=HDstrdup(rstab[j].name))==NULL)
                                  {
                                    HDfree(wlist->esize);
                                    HDfree(wlist->name);
                                    HDfree(wlist->order);
                                    HDfree(wlist->off);
                                    HDfree(wlist->isize);
                                    HDfree(wlist->type);
                                    HGOTO_ERROR(DFE_NOSPACE,FAIL);
                                  } /* end if */
                                order = rstab[j].order;
                                wlist->type[wlist->n] = rstab[j].type;
                                wlist->order[wlist->n] = order;
                                wlist->esize[wlist->n] = (uint16) (order * DFKNTsize((int32) rstab[j].type | DFNT_NATIVE));
                                wlist->isize[wlist->n] = (uint16) (order * rstab[j].isize);
                                wlist->ivsize += (uint16) (wlist->isize[wlist->n]);
                                wlist->n++;
                                break;
                            }
                  }
                if (!found)     /* field is not a defined field - error  */
                    HGOTO_ERROR(DFE_BADFIELDS, FAIL);
            }

          /* *********************************************************** */
          /* compute and save the fields' offsets */
          for (uj = 0, i = 0; i < wlist->n; i++)
            {
                wlist->off[i] = (uint16) uj;
                uj += wlist->isize[i];
            }

          HGOTO_DONE(SUCCEED); /* OK */
      }     /* writing to empty vdata */

    /*
     *   No matter the access mode, if there are elements in the VData
     *      we should set the read list
     */
    if (vs->nvertices > 0)
      {
          rlist = &(vs->rlist);
          rlist->n = 0;
          if(rlist->item!=NULL)
              HDfree(rlist->item);
          rlist->item=NULL;

          /* Allocate enough space for the read list */
          if((rlist->item=(intn *)HDmalloc(sizeof(intn)*(ac)))==NULL)
              HGOTO_ERROR(DFE_NOSPACE, FAIL);
          for (i = 0; i < ac; i++)
            {
                found = FALSE;
                for (j = 0; j < vs->wlist.n; j++)
                    if (!HDstrcmp(av[i], vs->wlist.name[j]))
                      {     /*  see if field exist */
                          found = TRUE;

                          rlist->item[rlist->n] = j;    /* save as index into wlist->name */
                          rlist->n++;
                          break;
                      }
                if (!found)     /* field does not exist - error */
                    HGOTO_ERROR(DFE_BADFIELDS, FAIL);
            }
        ret_value=SUCCEED;
      }     /* setting read list */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSsetfields);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VSsetfields */

/* ------------------------------------------------------------------ */
/*
   ** defines a (one) new field within the vdata
   ** return FAIL if error
   ** return SUCCEED if success
 */
intn 
VSfdefine(int32 vkey, const char *field, int32 localtype, int32 order)
{
    char      **av;
    int32       ac;
    int16       isize, usymid, replacesym;
    intn j;
    vsinstance_t *w;
    VDATA      *vs;
    intn       ret_value = SUCCEED;
    CONSTR(FUNC, "VSfdefine");

#ifdef HAVE_PABLO
    TRACE_ON(VS_mask, ID_VSfdefine);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if ((vs == NULL) || (scanattrs(field, &ac, &av) == FAIL) || (ac != 1))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* The order of a variable is stored in a 16-bit number, so have to keep this limit -QAK */
    if (order < 1 || order > MAX_ORDER)
        HGOTO_ERROR(DFE_BADORDER, FAIL);
    /* don't forget to check for field size limit */
    isize = DFKNTsize(localtype);
    if ( (isize == FAIL) || (isize * order) > MAX_FIELD_SIZE )
        HGOTO_ERROR(DFE_BADFIELDS, FAIL);

    /*
     ** check for any duplicates
     */
#ifdef QAK
/* It's OK to over-ride pre-defined symbols with the user's own -QAK */
    /* --- first look in the reserved symbol table --- */
    for (j = 0; j < NRESERVED; j++)
        if (!HDstrcmp(av[0], rstab[j].name))
          {
              if (localtype != rstab[j].type && order != rstab[j].order)
                  break;
          }
#endif /* QAK */

    /* --- then look in the user's symbol table --- */
    for (replacesym = 0, j = 0; j < vs->nusym; j++)
        if (!HDstrcmp(av[0], vs->usym[j].name))
          {
              if (localtype != rstab[j].type && order != rstab[j].order)
                {
                    replacesym = 1;
                    break;
                }
          }

    if (replacesym)
        usymid = (int16) j;     /* new definition will replace old at this index */
    else
      {
          SYMDEF *tmp_sym=vs->usym;  /* temp. pointer to the new symdef list */

          usymid = vs->nusym;
          /* use temporary pointer in case we run out of memory, so we don't loose original list */
          if (tmp_sym == NULL)
           {
             if((tmp_sym=(SYMDEF *) HDmalloc(sizeof(SYMDEF)*(usymid+1)))==NULL)
              HGOTO_ERROR(DFE_NOSPACE,FAIL);
            }
          else
            {
              if((tmp_sym=(SYMDEF *) HDrealloc((VOIDP)tmp_sym,sizeof(SYMDEF)*(usymid+1)))==NULL)
                  HGOTO_ERROR(DFE_NOSPACE,FAIL);
            }
          vs->usym=tmp_sym;
      } /* end else */

      vs->usym[usymid].isize = (uint16) isize; /* ok, because number
          type sizes are smaller than max(int16) */

    /* Copy the symbol [field] information */
    if ((vs->usym[usymid].name = (char *) HDstrdup(av[0]) ) == NULL)
        HGOTO_ERROR(DFE_NOSPACE, FAIL);
    vs->usym[usymid].type = (int16) localtype;
    vs->usym[usymid].order = (uint16) order;

    /* increment vs->nusym only if no user field has been redefined */
    if (!replacesym)
        vs->nusym++;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VS_mask, ID_VSfdefine);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VSfdefine */

/* ------------------------------ VFnfields ------------------------------- */
/*
   Return the number of fields in this Vdata
   Return FAIL on failure
 */
int32
VFnfields(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VFnfields");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFnfields);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->wlist.n);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VF_mask, ID_VFnfields);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VFnfields */

/* ----------------------------- VFfieldname ------------------------------ */
/*
   Return the name of the given field in this Vdata.
   This is just a pointer to the string in local memory and is only guarenteed
   to be valid as long as we are VSattached() to this Vdata

   Return NULL on failure
 */
char       *
VFfieldname(int32 vkey, int32 index)
{
    vsinstance_t *w;
    VDATA      *vs;
    char       *ret_value = NULL; /* FAIL */
    CONSTR(FUNC, "VFfieldname");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldname);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,NULL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,NULL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,NULL);

    ret_value = ((char *) vs->wlist.name[index]);

done:
  if(ret_value == NULL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldname);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VFfieldname */

/* ----------------------------- VFfieldtype ------------------------------ */
/*
   Return the type of the given field in this Vdata.

   Return FAIL on failure
 */
int32
VFfieldtype(int32 vkey, int32 index)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VFfeildtype");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldtype);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->wlist.type[index]);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VF_mask, ID_VFfieldtype);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VFfieldtype */

/* ----------------------------- VFfieldisize ------------------------------ */
/*
   Return the internal size of the given field in this Vdata.
   (internal to HDF file, see VWRITELIST in vg.h. 4/3/96) 

   Return FAIL on failure
 */
int32
VFfieldisize(int32 vkey, int32 index)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VFfieldisize");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldisize);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->wlist.isize[index]);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VF_mask, ID_VFfieldisize);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VFfieldisize */

/* ----------------------------- VFfieldesize ------------------------------ */
/*
   Return the external size of the given field in this Vdata.
   (external to HDF file, see VWRITELIST in vg.h. 4/3/96) 

   Return FAIL on failure
 */
int32
VFfieldesize(int32 vkey, int32 index)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VFfieldesize");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldesize);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->wlist.esize[index]);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VF_mask, ID_VFfieldesize);
#endif /* HAVE_PABLO */

  return ret_value;
}   /* VFfieldesize */

/* ----------------------------- VFfieldorder ------------------------------ */
/*
   Return the order of the given field in this Vdata.

   Return FAIL on failure
 */
int32
VFfieldorder(int32 vkey, int32 index)
{
    vsinstance_t *w;
    VDATA      *vs;
    int32      ret_value = SUCCEED;
    CONSTR(FUNC, "VFfieldorder");

#ifdef HAVE_PABLO
    TRACE_ON(VF_mask, ID_VFfieldorder);
#endif /* HAVE_PABLO */

    if (HAatom_group(vkey)!=VSIDGROUP)
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
      HGOTO_ERROR(DFE_NOVS,FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    ret_value = ((int32) vs->wlist.order[index]);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
#ifdef HAVE_PABLO
    TRACE_OFF(VF_mask, ID_VFfieldorder);
#endif /* HAVE_PABLO */

  return ret_value;
}	/* VFfieldorder */

/* -------------------------- VSsetexternalfile --------------------------- */
/*

 NAME
	VSsetexternalfile -- store Vdat info in a separate file
 USAGE
	int32 VSsetexternalfile(id, filename, offset)
        int32   id;                  IN: vdata ID
        char  * filename;            IN: name of external file
        int32   offset;              IN: offset in external file
 RETURNS
        Return SUCCEED or FAIL

 DESCRIPTION
        Specify that the actual data for this Vdata be stored in a 
        separate file (an "external file" in HDF terms).

        Only the data (as in VSwrite()) will be stored externally.  
        Attributes and such will still be in the main file

        IMPORTANT:  It is the user's responsibility to see that the 
        separate files are transported with the main file.
FORTRAN
	vsfsextf

--------------------------------------------------------------------------- */

intn VSsetexternalfile(int32 vkey, char *filename, int32 offset)
{
    CONSTR(FUNC, "VSsetexternalfile");
    int32       ret_value = SUCCEED;

    vsinstance_t *w;
    VDATA      *vs;
    intn       status;

    if(!filename || offset < 0)
	HGOTO_ERROR(DFE_ARGS, FAIL);

    if (HAatom_group(vkey)!=VSIDGROUP)
	    HGOTO_ERROR(DFE_ARGS, FAIL);

    /* locate vs's index in vstab */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs->access != 'w')
        HGOTO_ERROR(DFE_BADACC, FAIL);

    if (-1L == vexistvs(vs->f, vs->oref))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    if(!w->ref)
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* no need to give a length since the element already exists */
    /* The Data portion of a Vdata is always stored in linked blocks. */
    /* So, use the special tag */
    status = (intn)HXcreate(vs->f, (uint16)VSDATATAG, (uint16) w->ref,
		      filename, offset, (int32)0);
    if(status != FAIL)
      {
        if((vs->aid != 0) && (vs->aid != FAIL))
            Hendaccess(vs->aid);
        vs->aid = status;
      }
    else
        ret_value = FAIL;

done:
    if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

    /* Normal function cleanup */

    return ret_value;
} /* VSsetexternalfile */

