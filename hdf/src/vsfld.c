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
************************************************************************/

#include "vg.h"

/* 
** ==================================================================
** PRIVATE data areas and routines
** ==================================================================
**/

/*
stores sizes of local machine's known types 
*/

PRIVATE int16 local_sizetab[] = 
{
	    LOCAL_UNTYPEDSIZE,
	    LOCAL_CHARSIZE,
	    LOCAL_INTSIZE,
	    LOCAL_FLOATSIZE,
	    LOCAL_LONGSIZE,
	    LOCAL_BYTESIZE,
	    LOCAL_SHORTSIZE,
	    LOCAL_DOUBLESIZE
};

PRIVATE int16 hdf_sizetab[] = 
{
	    SIZE_UCHAR8,  /* untyped */
		 SIZE_CHAR8,   /* text char */
	    SIZE_INT32,   /* maps 16 bits (in mem) to 32 bits (in file) */
		 SIZE_FLOAT32, /* float */
		 SIZE_INT32, 	/* long */
	    SIZE_UCHAR8,  /* byte (non-text) */
		 SIZE_INT16, 	/* short */
	    SIZE_FLOAT64  /* double */
};

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
#define LOCALSIZETAB_SIZE sizeof(local_sizetab)/sizeof(int)
#define HDFSIZETAB_SIZE sizeof(hdf_sizetab)/sizeof(int)

/*
 ** returns the machine size of a field type
 ** returns FAIL if error
 */
int16 VSIZEOF (int16 x)
{
  if (x < 0 || x > LOCALSIZETAB_SIZE-1) {
    return(FAIL);
  } else {
    return(local_sizetab[x]);
  }
} /* VSIZEOF */

/*
 ** returns the HDF file size of a field type
 ** returns FAIL if error.  
 ** USE ONLY FOR BACKWARD COMPATABILITY
 */
int16 HDFSIZEOF (int16 x)
{
  
  if (x<0 || x>HDFSIZETAB_SIZE-1) {
    return(FAIL);
  } else {
    return(hdf_sizetab[x]);
  }
} /* HDFSIZEOF */


/* ------------------------------------------------------------------ */
/*
** sets the fields in a vdata for reading or writing
** RETURNS FAIL if error, and SUCCEED if ok.
** truncates each field to max length of  FIELDNAMELENMAX.
*/

PUBLIC intn VSsetfields (int32 vkey, const char *fields)
{
    char          **av;
    int32         ac, found;
    register intn j, i;
    int16         order;
    int32         value;
    VREADLIST     * rlist;
    VWRITELIST    * wlist;
    vsinstance_t  * w;
    VDATA         * vs;
    CONSTR(FUNC,"VSsetfields");

    if (!VALIDVSID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vs=w->vs;
    if (vs == NULL)
        HRETURN_ERROR(DFE_ARGS,FAIL);

    if((scanattrs(fields, &ac, &av) == FAIL) || (ac == 0))
        HRETURN_ERROR(DFE_BADFIELDS,FAIL);

    /*
     * write to an empty vdata : set the write list but do not set the
     *   read list cuz there is nothing there to read yet...
     */
    if(vs->access == 'w' && vs->nvertices == 0) {
        wlist = &(vs->wlist);  /* use a shorter name to make code cleaner */
        wlist->ivsize = 0;
        wlist->n      = 0;
        for(i = 0; i < ac; i++) {

            found = FALSE;
            /* --- first look in the user's symbol table --- */
            for(j = 0; j < vs->nusym; j++)
                if (!HDstrcmp(av[i], vs->usym[j].name)) {
                    found = TRUE;
                    
                    HDstrcpy (wlist->name[wlist->n], vs->usym[j].name);
                    order = vs->usym[j].order;
                    wlist->type[wlist->n]  = vs->usym[j].type;
                    wlist->order[wlist->n] = order;

                    value = order * DFKNTsize(vs->usym[j].type | DFNT_NATIVE);
                    if(value > MAX_FIELD_SIZE)
			HRETURN_ERROR(DFE_BADFIELDS,FAIL);
                    wlist->esize[wlist->n] = (int16) value;

                    value = order * vs->usym[j].isize;
                    if(value > MAX_FIELD_SIZE)
			HRETURN_ERROR(DFE_BADFIELDS,FAIL);
                    wlist->isize[wlist->n] = (int16) value;

                    value = (int32)wlist->ivsize + (int32)(wlist->isize[wlist->n]);
                    if(value > MAX_FIELD_SIZE)
			HRETURN_ERROR(DFE_BADFIELDS,FAIL);
                    wlist->ivsize = (int16) value;

                    wlist->n++;
                    break;
                }
            
            /* --- now look in the reserved symbol table --- */
            if(!found) {
                for(j = 0; j < NRESERVED; j++)
                    if (!HDstrcmp(av[i], rstab[j].name)) {
                        found = TRUE;

                        HDstrcpy( wlist->name[wlist->n],rstab[j].name);
                        order = rstab[j].order;
                        wlist->type[wlist->n]  =  rstab[j].type;
                        wlist->order[wlist->n] =  order;
                        wlist->esize[wlist->n] =  (int16) (order * DFKNTsize((int32)rstab[j].type | DFNT_NATIVE));
                        wlist->isize[wlist->n] =  (int16) (order * rstab[j].isize);
                        wlist->ivsize+= (int16)(wlist->isize[wlist->n]);
                        wlist->n++;
                        break;
                    }
            }
            if (!found)      /* field is not a defined field - error  */
                HRETURN_ERROR(DFE_BADFIELDS,FAIL);
        }
    
    /* *********************************************************** */
    /* ensure fields with order > 1 are alone  */
#ifdef CHOUCK
        for (i = 0; i < wlist->n; i++)
            if (wlist->order[i] > 1 && wlist->n != 1)
                HRETURN_ERROR(DFE_BADORDER,FAIL);
#endif /* CHOUCK */

    /* *********************************************************** */
    /* compute and save the fields' offsets */
        for (j=0, i=0; i < wlist->n; i++) {
            wlist->off[i] = (int16)j;
            j += wlist->isize[i];
        }
        
        return(SUCCEED); /* ok */
    } /* writing to empty vdata */
    
    /*
     *   No matter the access mode, if there are elements in the VData
     *      we should set the read list
     */
    if(vs->nvertices > 0) {
        rlist = &(vs->rlist);
        rlist->n = 0;
        for (i = 0; i < ac; i++) {
            found = FALSE;
            for (j = 0; j < vs->wlist.n; j++)
                if (!HDstrcmp(av[i], vs->wlist.name[j]) ) { /*  see if field exist */
                    found = TRUE;
                    
                    rlist->item[rlist->n] = j; /* save as index into wlist->name */
                    rlist->n++;
                    break;
                }
            if (!found)       /* field does not exist - error */
                HRETURN_ERROR(DFE_BADFIELDS,FAIL);
        }
        
        return(SUCCEED);
        
  } /* setting read list */

  return (FAIL);
} /* VSsetfields */

/* ------------------------------------------------------------------ */
/*
** defines a (one) new field within the vdata 
** return FAIL if error
** return SUCCEED if success
*/

PUBLIC intn VSfdefine (int32 vkey, const char *field, int32 localtype, int32 order)
{
    char  **av;
    int32 ac;
    char  *ss;
    int16 usymid, replacesym;
    register intn j;
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VSfdefine");

    if (!VALIDVSID(vkey))
        HRETURN_ERROR(DFE_ARGS,FAIL);

  /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey))))
        HRETURN_ERROR(DFE_NOVS,FAIL);

    vs=w->vs;
    if ((vs == NULL) || (scanattrs(field,&ac,&av) == FAIL) || (ac != 1))
        HRETURN_ERROR(DFE_ARGS, FAIL);
  
    if (order < 1 || order > MAX_ORDER)
        HRETURN_ERROR(DFE_BADORDER,FAIL);
  
  /* 
   ** check for any duplicates 
   */
  /* --- first look in the reserved symbol table --- */
    for (j = 0; j < NRESERVED; j++)
        if (!HDstrcmp(av[0], rstab[j].name)) {
            if (localtype != rstab[j].type && order != rstab[j].order)
                break;
        }

  /* --- then look in the user's symbol table --- */
    for (replacesym = 0,j = 0; j < vs->nusym; j++)
        if (!HDstrcmp(av[0], vs->usym[j].name))  {
            if (localtype != rstab[j].type && order != rstab[j].order) {
                replacesym = 1;
                break;
            }
        }
  
    if (replacesym)
        usymid = (int16)j; /* new definition will replace old at this index */
    else
        usymid = vs->nusym;
  
    if ((vs->usym[usymid].isize = (int16)DFKNTsize( (int32) localtype)) == FAIL)
        HRETURN_ERROR(DFE_BADTYPE,FAIL);
  
    if( (ss = (char*) HDgetspace (HDstrlen(av[0]) + 1))==NULL)
        HRETURN_ERROR(DFE_NOSPACE, FAIL);
  
    HDstrcpy(ss, av[0]);
    vs->usym[usymid].name  = ss;
    vs->usym[usymid].type  = (int16)localtype;
    vs->usym[usymid].order = (int16)order;
  
  /* prevent user-symbol table overflow */
    if (vs->nusym >= USYMMAX)
        HRETURN_ERROR(DFE_SYMSIZE,FAIL);
  
  /* increment vs->nusym only if no user field has been redefined */
    if (!replacesym)
        vs->nusym++;
  
    return(SUCCEED);
} /* VSfdefine */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* erstwhile macros - Use these for accessing user-defined fields in a vdata. */
/*
#define VFnfields(vdata)        (vdata->wlist.n)
#define VFfieldname(vdata,t) 	(vdata->wlist.name[t])
#define VFfieldtype(vdata,t) 	(vdata->wlist.type[t])
#define VFfieldisize(vdata,t) 	(vdata->wlist.isize[t])
#define VFfieldesize(vdata,t) 	(vdata->wlist.esize[t])
#define VFfieldorder(vdata,t) 	(vdata->wlist.order[t]) 
*/
/* ------------------------------------------------------------------------ */

/* ------------------------------ VFnfields ------------------------------- */
/*
  
  Return the number of fields in this Vdata
  Return FAIL on failure

*/

PUBLIC 
int32 VFnfields(int32 vkey)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFnfeilds");

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

    return ((int32) vs->wlist.n);

} /* VFnfields */

/* ----------------------------- VFfieldname ------------------------------ */
/*
  
  Return the name of the given field in this Vdata.
  This is just a pointer to the string in local memory and is only guarenteed
  to be valid as long as we are VSattached() to this Vdata

  Return NULL on failure

*/

PUBLIC 
char * VFfieldname(int32 vkey, int32 index)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFfieldname");

    if (!VALIDVSID(vkey)) {
        HERROR(DFE_ARGS);
        return(NULL);
    }
  
    /* locate vs's index in vstab */
    if(NULL==(w=(vsinstance_t*)vsinstance(VSID2VFILE(vkey),(uint16)VSID2SLOT(vkey)))) {
        HERROR(DFE_NOVS);
        return(NULL);
    }

    vs=w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG)) {
          HERROR(DFE_ARGS);
          return(NULL);
    }

    return ((char *) vs->wlist.name[index]);

} /* VFfieldname */

/* ----------------------------- VFfieldtype ------------------------------ */
/*
  
  Return the type of the given field in this Vdata.

  Return FAIL on failure

*/

PUBLIC 
int32 VFfieldtype(int32 vkey, int32 index)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFfeildtype");

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

    return ((int32) vs->wlist.type[index]);

} /* VFfieldtype */

/* ----------------------------- VFfieldisize ------------------------------ */
/*
  
  Return the internal (i.e. in memory) size of the given field in this Vdata.

  Return FAIL on failure

*/

PUBLIC 
int32 VFfieldisize(int32 vkey, int32 index)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFfieldisize");

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

    return ((int32) vs->wlist.isize[index]);

} /* VFfieldisize */

/* ----------------------------- VFfieldesize ------------------------------ */
/*
  
  Return the external (i.e. on disk) size of the given field in this Vdata.

  Return FAIL on failure

*/

PUBLIC 
int32 VFfieldesize(int32 vkey, int32 index)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFfieldisize");

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

    return ((int32) vs->wlist.esize[index]);

} /* VFfieldesize */

/* ----------------------------- VFfieldorder ------------------------------ */
/*
  
  Return the order of the given field in this Vdata.

  Return FAIL on failure

*/

PUBLIC 
int32 VFfieldorder(int32 vkey, int32 index)
{
    vsinstance_t    *w;
    VDATA           *vs;
    CONSTR(FUNC,"VFfieldorder");

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

    return ((int32) vs->wlist.order[index]);

} /* VFfieldorder */
