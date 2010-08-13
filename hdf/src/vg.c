/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/* vg.c,v 1.15.4.1 1993/10/26 19:25:07 georgev Exp */

/*
FILE  
     vg.c
     HDF vdata routines and some vgroup routines
EXPORTED ROUTINES
     VSelts         -- number of elements in a vdata
     VSgetinterlace -- returns the interlace type of the vdata
     VSsetinterlace -- sets the vdata's interlace to full or none
     VSgetfields    -- returns the fieldnames in a vdata
     VSfexist       -- tests for existence of one or more fields in a vdata
     VSsizeof       -- computes the byte size of the field(s) if a vdata
     VSdump         -- prints contents of a vdata (for debugging)
     VSsetname      -- associate a name with a vdata
     VSsetclass     -- assigns a class name to a vdata
     VSgetname      -- gets the vdata's name
     VSgetclass     -- gets the vdata's class name
     VSinquire      -- gets information about a vdata
     VSlone         -- returns an array of refs of all lone vdatas in the file
     Vlone          -- returns an array of refs of all lone vgroups in the file
     Vfind          -- looks in the file for a vgroup with a given name 
     VSfind         -- looks in the file for a vdata with a given name
     Vfindclass     -- looks in the file and returns the ref of 
                       the vgroup with the specified class
     VSfindclass    -- looks in the file and returns the ref of the vdata 
                       with specified class
     VSsetblocksize -- sets the block size of the linked-block element.
     VSsetnumblocks -- sets the number of blocks for a linked-block element.
     VSgetblockinfo -- retrieves the block size and the number of blocks 
		       of a linked-block element.
     Vsetzap        -- maintaind for back compatibility
PRIVATE FUNCTIONS
     matchnocase    -- compares to strings, ignoring case

PRIVATE functions manipulate vsdir and are used only within this file.
PRIVATE data structures in here pertain to vdata in vsdir only.
 */

#define VSET_INTERFACE
#include "hdf.h"

/* These are used to determine whether a vdata had been created by the
   library internally, that is, not created by user's application */
#define HDF_NUM_INTERNAL_VDS	8
const char *HDF_INTERNAL_VDS[] = {DIM_VALS, DIM_VALS01, _HDF_ATTRIBUTE,
	_HDF_SDSVAR, _HDF_CRDVAR, "_HDF_CHK_TBL_", RIGATTRNAME, RIGATTRCLASS};

/* Private functions */
#ifdef VDATA_FIELDS_ALL_UPPER
PRIVATE int32 matchnocase
            (char *strx, char *stry);
#endif /* VDATA_FIELDS_ALL_UPPER */

#ifdef VDATA_FIELDS_ALL_UPPER
/*-----------------------------------------------------------------
NAME
   matchnocase -  (PRIVATE) compares 2 strings, ignoring case 

DESCRIPTION
   Private routine. 

RETURNS
   if strings match return TRUE, 
   else FALSE
--------------------------------------------------------------------*/
PRIVATE int32 
matchnocase(char *strx, /* IN: first string to be compared */
            char *stry  /* IN: second string to be compared */)
{
    int32  i;
    int32  nx, ny; /* length of strings */

    nx = HDstrlen(strx);
    ny = HDstrlen(stry);

    if (nx != ny)
        return (FALSE);     /* different lengths */

    for (i = 0; i < nx; i++, strx++, stry++)
      {
        if (toupper(*strx) != toupper(*stry))
            return (FALSE); /* not identical */
      }

    return (TRUE);
}   /* matchnocase */
#endif /* VDATA_FIELDS_ALL_UPPER */

/* ------------------------------------------------------------------
NAME
   VSelts -- get number of elements in a vdata

DESCRIPTION
   Gets the number of elements in the specified vdata

RETURNS
   On success returns the number of elements in the VDATA vkey; 
   returns FAIL  on error.
--------------------------------------------------------------------- */
int32 
VSelts(int32 vkey /* IN: vdata key */)
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = SUCCEED;
    CONSTR(FUNC, "VSelts");

    /* make sure vdata key is part of vdata group */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if ((vs == NULL) || (vs->otag != DFTAG_VH))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* return number of elements in vdata */
    ret_value = (vs->nvertices);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSelts */

/* ------------------------------------------------------------------
NAME
   VSgetinterlace -- gets interlace of the vdata vkey.

DESCRIPTION
   Gets the interlace scheme for the specified vdata.

RETURNS
   On success returns the interlace scheme used in the fil) of the 
   specified vdata vkey.   returns FAIL on error.
----------------------------------------------------------------------- */
int32 
VSgetinterlace(int32 vkey /* IN: vdata key */)
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetinterlace");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* return interlace of vdata */
    ret_value = ((int32) (vs->interlace));

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetinterlace */

/*------------------------------------------------------------------
NAME
   VSsetinterlace --  sets the vdata's interlace to full or none.

DESCRIPTION
   Sets the interlace scheme for the vdata to one of
   FULL_INTERLACE or NO_INTERLACE.

RETURNS
   SUCCEED/FAIL
--------------------------------------------------------------------*/
intn 
VSsetinterlace(int32 vkey,     /* IN: vdata key */
               int32 interlace /* IN: interlace for storing records */ )
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    intn         ret_value = SUCCEED;
    CONSTR(FUNC, "VSsetinterlace");

    /* check key is valid vdata */
    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* check if only read access, this is a failure */
    if (vs->access == 'r')
        HGOTO_ERROR(DFE_RDONLY, FAIL);

    /* check if records have already been written out,
       cannot reset interlace in this case */
    if (vs->nvertices > 0)
        HGOTO_ERROR(DFE_NORESET, FAIL);

    /* currently only 2 kinds of interlaced schemes allowed. */
    if (interlace == FULL_INTERLACE || interlace == NO_INTERLACE)
      {
          vs->interlace = (int16) interlace;
          ret_value = SUCCEED;    /* ok */
      }
    else
        ret_value = FAIL;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSsetinterlace */

/*------------------------------------------------------------------
NAME
   VSgetfields -- returns the name (if any) of all the fields in the vdata

DESCRIPTION
   Returns the names of all the fields in comma seperated string in
   the argument 'fields'. (e.g., "PX,PY").

RETURNS
   Returns FAIL on error, 
   else the number of fields in the vdata.
----------------------------------------------------------------------*/
int32 
VSgetfields(int32 vkey,   /* IN: vdata key */
            char *fields  /* OUT: comman seperated field name list */)
{
    int32        i;
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetfields");

    /* check if a NULL field list is passed in, then return with
       error (found while fixing bug #554) - BMR 4/30/01 */
    if (fields == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* Got through Vdata and build the comman seperated string of field names */
    fields[0] = '\0';
    /* No special handling for 0-field vdatas, this algorithm should work fine. */
    for (i = 0; i < vs->wlist.n; i++)
      {   /* build the comma-separated string */
          HDstrcat(fields, vs->wlist.name[i]);
          if (i < vs->wlist.n - 1)
              HDstrcat(fields, ",");
      }

    /* return number of fields */
    ret_value = ((int32) vs->wlist.n);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetfields */

/*------------------------------------------------------------------
NAME
   VSfexist -- tests for existence of 1 or more fields in a vdata.

DESCRIPTION
   The argument 'fields' is a string of comma-separated fieldnames 
   (e.g. "PX,PY,PZ") to check for in vdata.

RETURNS 
   Returns SUCCEED if all fields exist 
   else otherwise FAIL is returned.
--------------------------------------------------------------------*/
intn 
VSfexist(int32 vkey,  /* IN: vdata key */
         char *fields /* IN: names of fields to check for */)
{
    char          **av = NULL;
    char           *s = NULL;
    DYN_VWRITELIST *w = NULL;
    vsinstance_t   *wi = NULL;
    VDATA          *vs = NULL;
    int32         ac;
    int32         i, j;
    int32         found;
    intn          ret_value = SUCCEED;
    CONSTR(FUNC, "VSfexist");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (wi = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* call scanattrs to parse the string */
    if (scanattrs(fields, &ac, &av) < 0)
        HGOTO_ERROR(DFE_BADFIELDS, FAIL);

    /* get vdata itself and check it 
       check also that more than one field in 'fields' */    
    vs = wi->vs;
    if ((vs == NULL) || (ac < 1))
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* now check in vdata's field table for these field names */
    w = &vs->wlist;
    for (i = 0; i < ac; i++)
      {
          found = 0;
          s = av[i];
          /* No changes for 0-field Vdatas, this algorithm should work fine. */
          for (j = 0; j < w->n; j++)
            {
#ifdef VDATA_FIELDS_ALL_UPPER
                if (matchnocase(s, w->name[j]))
                  {
                      found = 1;
                      break;
                  }
#else
                if (HDstrcmp(s, w->name[j]) == 0)
                  {
                      found = 1;
                      break;
                  }
#endif /* VDATA_FIELDS_ALL_UPPER */
            }
          if (!found)
            HGOTO_DONE(FAIL);
      }

    ret_value = TRUE;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}	/* VSfexist */

/* -----------------------------------------------------------------
NAME
   VSsizeof - computes the byte size of the field(s) of a vdata.

DESCRIPTION
   The size is the actual size for the local architecture

RETURNS
   The byte size of the field(s), positive integer, on success; 
   otherwise, returns FAIL.
----------------------------------------------------------------- */
int32 
VSsizeof(int32 vkey,   /* IN vdata key */
         char *fields  /* IN: Name(s) of the fields to check size of */ )
{
    int32       totalsize;
    int32       i, j;
    int32       found;
    int32       ac;
    char        **av = NULL;
    vsinstance_t *w  = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = SUCCEED;
    CONSTR(FUNC, "VSsizeof");

    /* check key is valid vdata */
    if (HAatom_group(vkey)!=VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    totalsize = 0;
    if (fields == NULL) /* default case? */
      {   /* count all field sizes in vdata */
        for (j = 0; j < vs->wlist.n; j++)	
            totalsize += vs->wlist.esize[j];
      }		
    else
      {  /* parse field string */
        if ((scanattrs(fields, &ac, &av) < 0) || (ac < 1))
            HGOTO_ERROR(DFE_ARGS, FAIL);

        for (i = 0; i < ac; i++)
          {   /* check fields in vs */
            for (found = 0, j = 0; j < vs->wlist.n; j++)	
                if (!HDstrcmp(av[i], vs->wlist.name[j]))
                  {
                    totalsize += vs->wlist.esize[j];
                    found = 1;
                    break;
                  }

            if (!found)
                HGOTO_ERROR(DFE_ARGS, FAIL);
          }	/* end for */
      }		/* end else */

    /* return total size of vdata fields specified */
    ret_value = totalsize;

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSsizeof */

/*----------------------------------------------------------------- 
NAME
   VSdump - prints contents of a vdata (for debugging) 

DESCRIPTION
    Prints contents of a vdata (for debugging) 

RETURNS
   No return codes.
-------------------------------------------------------------------*/
VOID 
VSdump(int32 vkey /* IN: vdata key */)
{
    vkey = vkey;    /* suppress warning */
}   /* VSdump */

/*-------------------------------------------------------
NAME
   VSsetname - give a name to a vdata.

DESCRIPTION
   Truncates name to max length of VSNAMELENMAX
   If new name is longer than the current name set new_h_sz,
      so that VSdetach will delete the original vdata header
      and write a new header. 

RETURNS
   SUCCEED/FAIL
----------------------------------------------------------*/
int32 
VSsetname(int32 vkey,        /* IN: Vdata key */
          const char *vsname /* IN: name to set for vdata*/)
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32       curr_len;
    int32       slen;
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "VSsetname");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check for null vdata name */
    if (vsname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* get current length of vdata name */
    curr_len = HDstrlen(vs->vsname);

    /* check length of new name against MAX length */
    if ((slen = HDstrlen(vsname)) > VSNAMELENMAX)
      { /* truncate name */
          HDstrncpy(vs->vsname, vsname, VSNAMELENMAX);
          vs->vsname[VSNAMELENMAX] = '\0';
      }
    else /* copy whole name */
        HDstrcpy(vs->vsname, vsname);

    vs->marked = TRUE; /* mark vdata as being modified */

    if (curr_len < slen) 
        vs->new_h_sz = TRUE; /* mark vdata header size being changed */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSsetname */

/* ------------------------------------------------------
NAME
   VSsetclass - give a class name to a vdata.

DESCRIPTION
   Truncates class name to max length of VSNAMELENMAX
   If new class is longer than the current class set new_h_sz,
   so that VSdetach will delete the original vdata header
   and write a new header.

RETURNS
   SUCCEED/FAIL
----------------------------------------------------------*/
int32 
VSsetclass(int32 vkey,         /* IN: vdata key */
           const char *vsclass /* IN: class name to set for vdata */)
{
    vsinstance_t *w = NULL;
    VDATA       *vs = NULL;
    int32       curr_len;
    int32       slen;
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "VSsetclass");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check for null vdata class */
    if (vsclass == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* get current length of vdata class name */
    curr_len = (intn)HDstrlen(vs->vsclass);

    /* check length of new class name against MAX length */
    if ((slen = (intn)HDstrlen(vsclass)) > VSNAMELENMAX)
      {
          HDstrncpy(vs->vsclass, vsclass, VSNAMELENMAX);
          vs->vsclass[VSNAMELENMAX] = '\0';
      }
    else
        HDstrcpy(vs->vsclass, vsclass);

    vs->marked = TRUE; /* mark vdata as being modified */

    if (curr_len < slen)
        vs->new_h_sz = TRUE; /* mark vdata header size being changed */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSsetclass */

/*------------------------------------------------------ 
NAME
   VSgetname - gets the vdata's name. 

DESCRIPTION
   Get vdata's name and comy it to 'vsname'. Space for
   'vsname' must be allocated by user.

RETURNS
   SUCCEED/FAIL
----------------------------------------------------------*/
int32 
VSgetname(int32 vkey,   /* IN: vdata key */
          char *vsname  /* OUT: vdata name (allocated by user)*/)
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetname");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check user allocated space */
    if (vsname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* copy vdata name over */
    HDstrcpy(vsname, vs->vsname);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetname */

/*------------------------------------------------------ 
NAME
   VSgetclass - gets the vdata's class name.

DESCRIPTION
   Get vdata class name and copy over to 'vsclass'. Space
   for 'vsclass' must be allocated by user.

RETURNS
   SUCCEED/FAIL
---------------------------------------------------------- */
int32 
VSgetclass(int32 vkey,    /* IN: vdata key */
           char *vsclass  /* OUT: class name for vdata (allocated by user) */ )
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    int32     ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetclass");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* check user allocated space */
    if (vsclass == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if (vs == NULL)
        HGOTO_ERROR(DFE_BADPTR, FAIL);

    /* copy class name over */
    HDstrcpy(vsclass, vs->vsclass);

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetclass */

/* ------------------------------------------------------------------ 
NAME
   VSinquire - gets info about a vdata vkey:

DESCRIPTION
   Get information about Vdata. Space for 'fields' and 'vsname'
   must be allocated by user.

RETURNS
   SUCCEED/FAIL

------------------------------------------------------------------------*/
intn 
VSinquire(int32 vkey,       /* IN: vdata key */
          int32 *nelt,      /* OUT: number of elements in vdata */
          int32 *interlace, /* OUT: interlace scheme of vdata */
          char *fields,     /* OUT: comma seperated string of field names */
          int32 *eltsize,   /* OUT: total size of all fields in bytes */
          char *vsname      /* OUT: name of vdata */)
{
  intn ret_value = SUCCEED;
  intn status;
  CONSTR(FUNC, "VSinquire");

    /* check key is valid vdata */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* obtain the value for each parameter although the previous one
       fails; ret_value should be FAIL if any of the parameters fails */
    if (fields)
      { /* we assume 'fields' space has been pre-allocated by user? */
        status = VSgetfields(vkey, fields);
        ret_value = (status == FAIL)? FAIL: ret_value;
      }
    if (nelt)
      {
        *nelt = VSelts(vkey);
        ret_value = (*nelt == FAIL)? FAIL: ret_value;
      }
    if (interlace)
      {
        *interlace = VSgetinterlace(vkey);
        ret_value = (*interlace == FAIL)? FAIL: ret_value;
      }
    if (eltsize)
      {
        *eltsize = VSsizeof(vkey, fields);
        ret_value = (*eltsize == FAIL)? FAIL: ret_value;
      }
    if (vsname)
      { /* we assume 'vsname' space as been pre-allocated by user? */
        status = VSgetname(vkey, vsname);
        ret_value = (status == FAIL)? FAIL: ret_value;
      }

done:
  if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
    return ret_value;   /* ok */

}   /* VSinquire */

/*-----------------------------------------------------------------
NAME
   VSlone - returns an array of refs of all lone vdatas in the file.

DESCRIPTION
   If 'idarray' is too small, routine will only fill 'idarray' with up
   to 'asize' worth of refs.

RETURNS
   Returns FAIL if error else returns the total number of lone 
   vdatas in the file.
---------------------------------------------------------------------*/
int32 
VSlone(HFILEID f,     /* IN: file id */
       int32 *idarray,/* OUT: array to return refs of lone vdatas? */
       int32 asize    /* IN: size of 'idarray' */)
{
    int32   i;
    int32   vgid;
    int32   vsid;
    int32   vstag;
    int32   vkey;
    uint8  *lonevdata = NULL;  /* lcl wrk area: stores flags of vdatas */
    int32   nlone;             /* total number of lone vdatas */
    int32   ret_value = SUCCEED;
    CONSTR(FUNC, "VSlone");

    /* -- allocate local space for vdata refs, init to zeros -- */
    if (NULL == (lonevdata = (uint8 *) HDcalloc(MAX_REF , sizeof(uint8))))
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* -- look for all vdatas in the file, and flag (1) each -- */
    vsid = -1;
    while (FAIL != (vsid = VSgetid(f, vsid)))    /* until no more vdatas */
        lonevdata[vsid] = 1;

    /* -- Look through all vgroups, searching for vdatas -- */
    /* -- increment its index in lonevdata if found -- */
    vgid = -1;
    while (FAIL != (vgid = Vgetid(f, vgid)))
      {     /* until no more vgroups */
        vkey = Vattach(f, vgid, "r"); /* attach to vgroup */
        for (i = 0; i < Vntagrefs(vkey); i++)
          {
            Vgettagref(vkey, i, &vstag, &vsid); /* get tag/ref of element in vg */
            if (vstag == (int32) DFTAG_VH)
                lonevdata[vsid] = 0; /* set vdata as not lone vdata */
          }
        Vdetach(vkey); /* detach from vgroup */
      }

    /* -- check in lonevdata: it's a lone vdata if its flag is still 1 -- */
    nlone = 0;
    for (i = 0; i < (int32)MAX_REF; i++)
      {
        if (lonevdata[i])
          { /* insert into idarray up till asize */
            if (nlone < asize)  
                idarray[nlone] = i; /* insert ref of vdata into idarray */
            nlone++;
          }
      }

    /* free up locally allocated space */
    HDfree((VOIDP) lonevdata);

    ret_value = nlone; /* return the TOTAL # of lone vdatas */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSlone */

/*----------------------------------------------------------------- 
NAME
   Vlone  - returns an array of refs of all lone vgroups in the file.

DESCRIPTION
   If 'idarray' is too small, routine will only fill 'idarray' with up
   to 'asize' worth of refs.

RETURNS
   Returns FAIL if error else returns the total number of lone
   vgroups in the file
---------------------------------------------------------------------*/
int32 
Vlone(HFILEID f,      /* IN: file id */
      int32 *idarray, /* OUT: array to return refs of lone vgroups? */
      int32 asize     /* IN: size of 'idarray' */)
{
    int32       i;
    int32       vgid;
    int32       vstag;
    int32       id;
    int32       vkey;
    uint8      *lonevg = NULL;  /* local wrk area: stores flags of vgroups */
    int32       nlone;          /* total number of lone vgroups */
    int32       ret_value = SUCCEED;
    CONSTR(FUNC, "Vlone");

    /* -- allocate space for vgroup refs, init to zeroes -- */
    if (NULL == (lonevg = (uint8 *) HDcalloc(MAX_REF , sizeof(uint8))))
        HGOTO_ERROR(DFE_NOSPACE, FAIL);

    /* -- look for all vgroups in the file, and flag (1) each -- */
    id = -1;
    while (FAIL != (id = Vgetid(f, id)))     /* until no more vgroups */
        lonevg[id] = 1;

    /* -- Look through all vgs, searching for vgroups -- */
    /* -- increment its index in lonevg if found -- */
    vgid = -1;
    while (FAIL != (vgid = Vgetid(f, vgid)))
      {     /* until no more vgroups */
        vkey = Vattach(f, vgid, "r"); /* attach to vgroup */
        id = -1;
        for (i = 0; i < Vntagrefs(vkey); i++)
          {
            Vgettagref(vkey, i, &vstag, &id); /* get tag/ref of element in vg */
            if (vstag == DFTAG_VG)
                lonevg[id] = 0; /* set vgroup as not lone vgroup */
          }
        Vdetach(vkey); /* detach from vgroup */
      }

    /* -- check in lonevg: it's a lone vgroup if its flag is still 1 -- */
    nlone = 0;
    for (i = 0; i < (int32)MAX_REF; i++)
      {
        if (lonevg[i])
          { /* insert into idarray up till asize */
            if (nlone < asize)  
                idarray[nlone] = i; /* insert ref of vgroup into idarray */
            nlone++;
          }
      }

    /* free up locally allocated space */
    HDfree((VOIDP) lonevg);

    ret_value = nlone; /* return the TOTAL # of lone vgroups */

done:
  if(ret_value == FAIL)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* Vlone */

/* ----------------------------------------------------------------- 
NAME
   Vfind -- looks in the file and returns the ref of 
            the vgroup with name vgname 

DESCRIPTION
   Finds the vgroup with the specified name and returns the ref of
   the vgroup if successful.

RETURNS
   Returns 0 if not found or on error. Otherwise, returns the 
   vgroup's ref (a positive integer).
-----------------------------------------------------------------------*/
int32 
Vfind(HFILEID f,          /* IN: file id */
      const char *vgname  /* IN: name of vgroup to find */)
{
    int32        vgid = -1;
    vginstance_t *v = NULL;
    VGROUP       *vg = NULL;
    int32       ret_value = 0;
    CONSTR(FUNC, "Vfind");

    /* check for null vgroup name */
    if (vgname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* process through Vgroups in file */
    while (FAIL != (vgid = Vgetid(f, vgid)))
      { /* get instance of vgroup */
        if((v = vginst(f,(uint16)vgid)) == NULL)
            HGOTO_DONE(0);

        /* get vgroup itself and check it */
        vg = v->vg;
        if (vg == NULL)
            HGOTO_DONE(0);

        /* compare vgroup name to 'vgname' */
        if (!HDstrcmp(vgname, vg->vgname)) 
            HGOTO_DONE((int32)(vg->oref));  /* found the vgroup */
      }

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}	/* Vfind */

/*------------------------------------------------------------------
NAME
   VSfind -- looks in the file and returns the ref of the vdata 
             with name vsname 

DESCRIPTION
   Finds the vdata with the specified name and returns the ref of
   the vdata if successfull.

RETURNS
   Returns 0 if not found, or on error. Otherwise, returns the vdata's 
   ref (a positive integer).
---------------------------------------------------------------------*/
int32 
VSfind(HFILEID f,          /* IN: file id */
       const char *vsname  /* IN: name of vdata to find */)
{
    int32        vsid = -1;
    vsinstance_t *v = NULL;
    VDATA        *vs = NULL;
    int32 ret_value = 0;
    CONSTR(FUNC, "VSfind");

    /* check for null vdata name */
    if (vsname == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* process through Vdatas in file */
    while (FAIL != (vsid = VSgetid(f, vsid)))
      { /* get instance of vdata */
        if((v = vsinst(f,(uint16)vsid)) == NULL)
            HGOTO_DONE(0);

        /* get vdata itself and check it */
        vs = v->vs;
        if (vs == NULL)
            HGOTO_DONE(0);

        /* compare vdata name to 'vsname' */
        if (!HDstrcmp(vsname, vs->vsname)) 
            HGOTO_DONE((int32)(vs->oref));  /* found the vdata */
      }

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSfind */

/* ----------------------------------------------------------------- 
NAME
   Vfindclass -- looks in the file and returns the ref of 
                 the vgroup with class vgclass 

DESCRIPTION
   Finds the vgroup with the specified class and returns the ref
   of the vgroup if successful.

RETURNS
   Returns 0 if not found, or error. Otherwise, returns the 
   vgroup's ref (a positive integer).
-----------------------------------------------------------------------*/
int32 
Vfindclass(HFILEID f,           /* IN: file id */
           const char *vgclass  /* IN: class of vgroup to find */)
{
    int32        vgid = -1;
    vginstance_t *v = NULL;
    VGROUP       *vg = NULL;
    int32       ret_value = 0;
    CONSTR(FUNC, "Vfindclass");

    /* check for null vgroup class */
    if (vgclass == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* process through Vgroups in file */
    while (FAIL != (vgid = Vgetid(f, vgid)))
      { /* get instance of vgroup */
        if((v = vginst(f,(uint16)vgid)) == NULL)
            HGOTO_DONE(0);

        /* get vgroup itself and check it */
        vg = v->vg;
        if (vg == NULL)
            HGOTO_DONE(0);

        /* compare vgroup class to 'vgclass' */
        if (!HDstrcmp(vgclass, v->vg->vgclass)) 
            HGOTO_DONE((int32)(v->vg->oref));  /* found the vgroup */
      }

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;    
}	/* Vfindclass */

/*------------------------------------------------------------------
NAME
   VSfindclass -- looks in the file and returns the ref of the vdata 
                  with class vsclass 

DESCRIPTION
   Finds the vdata with the specified class and returns the ref of 
   the vdata if successful.

RETURNS
   Returns 0 if not found, or error. Otherwise, returns the vdata's 
   ref (a positive integer).
---------------------------------------------------------------------*/
int32 
VSfindclass(HFILEID f,           /* IN: file id */
            const char *vsclass  /* IN: class of vdata to find */)
{
    int32        vsid = -1;
    vsinstance_t *v = NULL;
    VDATA        *vs = NULL;
    int32        ret_value = 0;
    CONSTR(FUNC, "VSfindclass");

    /* check for null vdata class */
    if (vsclass == NULL)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* process through Vdatas in file */
    while (FAIL != (vsid = VSgetid(f, vsid)))
      { /* get instance of vdata */
        if((v = vsinst(f,(uint16)vsid)) == NULL)
            HGOTO_DONE(0);          /* error */

        /* get vdata itself and check it */
        vs = v->vs;
        if (vs == NULL)
            HGOTO_DONE(0);

        /* compare vdata class to 'vsclass' */
        if (!HDstrcmp(vsclass, v->vs->vsclass)) 
            HGOTO_DONE((int32)(v->vs->oref));  /* found the vdata */
      }

done:
  if(ret_value == 0)   
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSfindclass */

/* ----------------------------------------------------------------- 
NAME
   VSsetblocksize -- sets the block size of the linked-block element.

USAGE
   intn VSsetblocksize(vkey, block_size)
   int32 vkey;		IN: vdata key
   int32 block_size	IN: length to be used for each linked-block
   
DESCRIPTION
   Sets the size of the blocks, that are after the first block, of a 
   linked-block element used for storing a vdata.  This routine is 
   to be called before the first write to the vdata to change the block 
   size from the default value HDF_APPENDABLE_BLOCK_LEN (4096).  Once the
   linked-block element is created, the block size cannot be changed.

RETURNS
   Returns SUCCEED/FAIL

MODIFICATION
   BMR - added in June 2001 to fix bug# 267

---------------------------------------------------------------------*/
intn
VSsetblocksize(	int32 vkey, /* IN: vdata key */
		int32 block_size) /* length to be used for each linked-block */
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    intn        ret_value = SUCCEED;
    CONSTR(FUNC, "VSsetblocksize");

    /* clear error stack */
    HEclear();

    /* check if the given vdata id is part of vdata group */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* internal routine handles the actual setting */
    if (HLsetblockinfo(vs->aid, block_size, -1) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}       /* VSsetblocksize */

/* ----------------------------------------------------------------- 
NAME
   VSsetnumblocks -- sets the number of blocks for a linked-block element.

USAGE
   intn VSsetnumblocks(vkey, num_blocks)
   int32 vkey;		IN: vdata key
   int32 num_blocks	IN: number of blocks to be used for the linked-block
			    element
   
DESCRIPTION
   Sets the number of blocks of a linked-block element used for storing
   a vdata.  This routine is to be called before the first write to the
   vdata to change the number of blocks from the default value
   HDF_APPENDABLE_BLOCK_NUM (16).  Once the linked-block element is
   created, the number of blocks cannot be changed.

RETURNS
   Returns SUCCEED/FAIL

MODIFICATION
   BMR - added in June 2001 to fix bug# 267

---------------------------------------------------------------------*/
intn
VSsetnumblocks( int32 vkey, /* IN: vdata key */
		int32 num_blocks) /* number of blocks the element can have */
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    intn        ret_value = SUCCEED;
    CONSTR(FUNC, "VSsetnumblocks");

    /* clear error stack */
    HEclear();

    /* check if the given vdata id is part of vdata group */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* internal routine handles the actual setting */
    if (HLsetblockinfo(vs->aid, -1, num_blocks) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}       /* VSsetnumblocks */

/* ----------------------------------------------------------------- 
NAME
   VSgetblockinfo -- retrieves the block size and the number of blocks 
		     of a linked-block element.

USAGE
   intn VSgetblockinfo(vkey, block_size, num_blocks)
   int32 vkey		IN: vdata key
   int32* block_size	OUT: the linked-block size
   int32* num_blocks	OUT: the number of blocks the element has
   
DESCRIPTION
   Retrieves the block size and the number of blocks of a linked-block
   element used for storing a vdata.  A NULL can be passed in for
   unwanted value.

RETURNS
   Returns SUCCEED/FAIL

MODIFICATION
   BMR - added in June 2001 to fix bug# 267

---------------------------------------------------------------------*/
intn
VSgetblockinfo(int32 vkey, /* IN: vdata id */
               int32* block_size, /* OUT: length used for each linked-block */
               int32* num_blocks) /* OUT: number of blocks the element has */
{
    vsinstance_t *w = NULL;
    VDATA        *vs = NULL;
    intn        ret_value = SUCCEED;
    CONSTR(FUNC, "VSgetblockinfo");

    /* clear error stack */
    HEclear();

    /* check if vdata is part of vdata group */
    if (HAatom_group(vkey) != VSIDGROUP)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* get vdata instance */
    if (NULL == (w = (vsinstance_t *) HAatom_object(vkey)))
        HGOTO_ERROR(DFE_NOVS, FAIL);

    /* get vdata itself and check it */
    vs = w->vs;
    if ((vs == NULL) || (vs->otag != VSDESCTAG))
      HGOTO_ERROR(DFE_ARGS,FAIL);

    /* internal routine handles the actual retrieval */
    if (HLgetblockinfo(vs->aid, block_size, num_blocks) == FAIL)
        HGOTO_ERROR(DFE_INTERNAL, FAIL);

done:
  if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}       /* VSgetblockinfo */

/*******************************************************************************
 NAME
    VSisinternal  --  Determine if a vdata's class name is for internal only

 RETURNS
    Returns TRUE (1) if "classname" is one of the class names used for
    vdata created by the the library and FALSE (0) otherwise.

*******************************************************************************/
intn
VSisinternal(const char *classname)
{
    int  i;
    intn ret_value = FALSE;

    /* Check if this class name is one of the internal class name and return
        TRUE, otherwise, return FALSE */
    for (i=0; i < HDF_NUM_INTERNAL_VDS; i++) {
        if (HDstrncmp(HDF_INTERNAL_VDS[i], classname, HDstrlen(HDF_INTERNAL_VDS[i])) == 0 ) {
            ret_value = TRUE;
            break;
        }
    }
    return ret_value;
}

/*******************************************************************************
NAME
   VSgetvdatas -- Get user-created vdata in a file or in a vgroup.

USAGE
    intn VSgetvdatas(id, start_vd, n_vds, *refarray)
	int32 id	  IN: file id or vgroup id
	uintn start_vd	  IN: vdata number to start retrieving
        uintn n_vds	  IN: number of user-created vds to return
        uint16 *refarray  IN/OUT: ref array to fill

DESCRIPTION
    VSgetvdatas retrieves n_vds vdatas by their reference numbers via the
    caller-supplied array refarray.  When a vgroup id is specified, VSgetvdatas
    will only retrieve the vdatas immediately belong to the specified vgroup,
    and not any sub-vgroups.

    The parameter n_vds provides the number of values that the refarray list
    can hold and can be any positive number smaller than MAX_REF (65535).  If
    n_vds is larger than the actual number of user-created vdatas, then only
    the actual number of reference numbers will be retrieved.

    The parameter start_vd indicates the vdata number to start at.
    - When start_vd is 0, the retrieval starts at the beginning.
    - When start_vd is between 0 and the number of user-created vdatas in
      the file or the vgroup, VSgetvdatas will start retrieving vdatas from
      the vdata number start_vd.
    - When start_vd is greater than the number of user-created vdatas in the
      file or the vgroup, VSgetvdatas will return FAIL.

    When refarray argument is NULL, VSgetvdatas will return the actual number
    of user-created vdatas without further processing.  This allows application
    to find out the size of the array for proper allocation.
   
RETURNS
    The number of user-created vdatas if successful and FAIL, otherwise.
    BMR - 2010/07/10
*******************************************************************************/
intn
VSgetvdatas(int32 id,		/* IN: file id or vgroup id */
	    uintn start_vd,	/* IN: vdata number to start retrieving */
            uintn n_vds,	/* IN: number of user-created vds to return */
            uint16 *refarray	/* IN/OUT: ref array to fill */)
{
    CONSTR(FUNC, "VSgetvdatas");
    int32       vkey;
    vsinstance_t *vsinst = NULL, *subvsinst = NULL;
    vginstance_t *vginst = NULL;
    intn          nactual_vds, user_vds, ii;
    VGROUP     *vg = NULL;
    VDATA      *vs = NULL;
    vfile_t    *vf = NULL;
    int32	vd_ref;
    int32      ret_value = SUCCEED;

    /* Make sure that proper size is passed in for the non-null array */
    if (refarray != NULL && n_vds == 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);
	
    /* clear error stack */
    HEclear();

    /* Make sure that proper size is passed in for the non-null array */
    if (refarray != NULL && n_vds == 0)
        HGOTO_ERROR(DFE_ARGS, FAIL);

    /* Check if id is a file id */
    if (HAatom_group(id) == FIDGROUP)
    {
	/* Get file record */
	if (NULL == (vf = Get_vfile(id)))
	    HGOTO_ERROR(DFE_FNF, FAIL);

	/* Look through all vds, searching for user-created vdatas until
	   no more vdatas in the file or the number of vdatas to be
	   retrieved is reached */
	nactual_vds = 0;/* number of user-created vdatas to be retrieved */
	user_vds = 0;	/* number of user-created vdatas */
	vd_ref = VSgetid(id, -1);  /* get ref number of first vd in the file */
	while ((vd_ref != FAIL) && ((nactual_vds < n_vds) || (n_vds == 0))
			&& (nactual_vds <= user_vds))
	{
	    vkey = VSattach(id, vd_ref, "r"); /* attach to current vdata */
	    if (vkey == FAIL)
		HGOTO_ERROR(DFE_CANTATTACH, FAIL);

	    /* get instance of vdata */
	    if (NULL == (vsinst = (vsinstance_t *) HAatom_object(vkey)))
		HGOTO_ERROR(DFE_NOVS, FAIL);

	    /* get vdata itself and check */
	    vs = vsinst->vs;
	    if (vs == NULL)
		HGOTO_ERROR(DFE_BADPTR, FAIL);

	    /* If this vdata is internally created by the lib, then just skip
		it; otherwise, record its ref# according to caller's
		specification of where to start and how many to retrieve */
	    if (vs->vsclass != NULL)
	    {
		if (VSisinternal(vs->vsclass) == FALSE)
		{
		    /* make sure to count only from vdata number start_vd */
		    if (user_vds >= start_vd)
			/* if caller requests for reference numbers */
			if (refarray != NULL)
			{
			    refarray[nactual_vds] = (uint16)vd_ref;

			    /* increment the actual number of user-created vds
			       to be retrieved */
			    nactual_vds++;
			}
		    /* increment the number of user-created vds */
		    user_vds++;
		}
	    }
	    if (VSdetach(vkey) == FAIL)
		HGOTO_ERROR(DFE_CANTDETACH, FAIL);

	    /* Move forward to the next vdata in the file */
	    vd_ref = VSgetid(id, vd_ref);
	} /* while more vdatas in file */

	/* Flag if start_vd is beyond the number of user-created vdatas */
	if (user_vds < start_vd)
            HGOTO_ERROR(DFE_ARGS, FAIL);

	/* If caller is asking for the number of vdatas only, return the
	   number of user-created vdatas, otherwise, return the number of
	   vdatas that are actually stored in refarray */
	if (refarray == NULL)
	    ret_value = user_vds - start_vd;
	else
	    ret_value = nactual_vds;
    } /* file id is given */

    /* check if the given id is a vgroup id */
    else if (HAatom_group(id)==VGIDGROUP)
    { /* vgroup id is given */

	/* get the number of sub-vdatas belong to this vgroup */
	int32 n_elements = Vntagrefs(id);
	if (n_elements == FAIL)
	    HGOTO_ERROR(DFE_GENAPP, FAIL);

	/* get instance of vgroup */
	if (NULL == (vginst = (vginstance_t *) HAatom_object(id)))
	    HGOTO_ERROR(DFE_NOVS, FAIL);

	/* get vgroup itself and check */
	vg = vginst->vg;
	if (vg == NULL)
	    HGOTO_ERROR(DFE_BADPTR, FAIL);

	/* Go through the tag list vg->tag and find user-created vgroups, until
	   no more sub-vgroups or the number of vgroups to be retrieved is
	   reached */
	nactual_vds = 0;/* number of user-created vgroups to be retrieved */
	user_vds = 0;	/* number of user-created vgroups */
        for (ii = 0; ii < n_elements && ((nactual_vds < n_vds) || (n_vds == 0))
			&& nactual_vds <= user_vds; ii++)
        {
	    /* If an element is a vgroup, then get access to it */
            if (vg->tag[ii] == DFTAG_VH)
	    {
		int32 subvkey;
		VDATA        *subvs = NULL;

		/* attach to sub-vdata */
	    	subvkey = VSattach(vg->f, vg->ref[ii], "r");
		if (subvkey == FAIL)
		    HGOTO_ERROR(DFE_CANTATTACH, FAIL);

		/* get instance of vdata */
		if (NULL == (subvsinst=(vsinstance_t *)HAatom_object(subvkey)))
		    HGOTO_ERROR(DFE_NOVS, FAIL);

		/* get vdata itself and check */
		subvs = subvsinst->vs;
		if (subvs == NULL)
		    HGOTO_ERROR(DFE_BADPTR, FAIL);

		/* If this vdata is internally created by the lib, then just
		   skip it; otherwise, record its ref# according to caller's
		   specification of where to start and how many to retrieve */
		if (subvs->vsclass != NULL)
		{
		    /* make sure this vdata is not an internal one */
		    if (VSisinternal(subvs->vsclass) == FALSE)
		    {
			/* make sure to count only from vs number start_vd */
			if (user_vds >= start_vd)
			    /* if caller requests for reference numbers */
			    if (refarray != NULL)
			    {
				refarray[nactual_vds] = (uint16)vg->ref[ii];

				/* increment the actual number of user-created
				   vds to be retrieved */
				nactual_vds++;
			    }

			/* increment the number of user-created vds */
			user_vds++;
		    }
		}
		if (VSdetach(subvkey) == FAIL)
		    HGOTO_ERROR(DFE_CANTDETACH, FAIL);
	    } /* this sub element is a vgroup */
        } /* for */

	/* Flag if start_vd is beyond the number of user-created vdatas */
	if (user_vds < start_vd)
            HGOTO_ERROR(DFE_ARGS, FAIL);

	/* If caller is asking for the number of vdatas only, return the
	   number of user-created vdatas, otherwise, return the number of
	   vdatas that are actually stored in refarray */
	if (refarray == NULL)
	    ret_value = user_vds - start_vd;
	else
	    ret_value = nactual_vds;
    } /* vgroup id is given */
    else
    {
 	fprintf(stderr, "The given ID must be a file ID or a vgroup ID\n");
        HGOTO_ERROR(DFE_ARGS, FAIL);
    }

done:
  if(ret_value == FAIL)
    { /* Error condition cleanup */

    } /* end if */

  /* Normal function cleanup */
  return ret_value;
}   /* VSgetvdatas */

/* ------------------------------- Vsetzap -------------------------------- */
/*
 * Vsetzap: Useless now. Maintained for back compatibility.
 */
VOID 
Vsetzap(void)
{
}
