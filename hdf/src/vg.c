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
/* vg.c,v 1.15.4.1 1993/10/26 19:25:07 georgev Exp */
/*
FILE  
     vg.c
     HDF vdata routines and some vgroup routines
EXPORTED ROUTINES
     vnewref        -- returns a unique reference number
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
     Vsetzap        -- maintaind for back compatibility
     VSgetclass     -- gets the vdata's class name
PRIVATE FUNCTIONS
     matchnocase    -- compares to strings, ignoring case

PRIVATE functions manipulate vsdir and are used only within this file.
PRIVATE data structures in here pertain to vdata in vsdir only.
 */

#include "vg.h"

/* Private functions */
#ifdef VDATA_FIELDS_ALL_UPPER
PRIVATE int32 matchnocase
            (char *strx, char *stry);
#endif /* VDATA_FIELDS_ALL_UPPER */

/*------------------------------------------------------------------
NAME 
   Vnewref --  utility routine. returns a unique reference number.
USAGE
   uint16 vnewref(f)
   HFILEID f;      IN: file id
RETURNS
   On success returns a unique ref (+ve unsigned 16-bit integer) ,
   returns 0 if error
DESCRIPTION
   Utility routine. Undocumented
-----------------------------------------------------------------------*/
uint16 
vnewref(HFILEID f)
{
    uint16      r;
    CONSTR(FUNC, "vnewref");

    r = (uint16) Hnewref(f);
    if (r == 0)
	HERROR(DFE_NOFREEDD);

    return (r);
}	/* vnewref */

#ifdef VDATA_FIELDS_ALL_UPPER
/*-----------------------------------------------------------------
NAME
   matchnocase -  (PRIVATE) compares 2 strings, ignoring case 
USAGE
   int32 matchnocase(strx, stry)
   char *strx, *stry;   IN: strings to be compared.
RETURNS
   TRUE if match, else FALSE
DESCRIPTION
   Private routine. 
--------------------------------------------------------------------*/
PRIVATE int32 
matchnocase(char *strx, char *stry)
{
    int32       i, nx, ny;

    nx = HDstrlen(strx);
    ny = HDstrlen(stry);
    if (nx != ny)
	return (FALSE);		/* different lengths */

    for (i = 0; i < nx; i++, strx++, stry++)
	if (toupper(*strx) != toupper(*stry))
	    return (FALSE);

    return (TRUE);
}	/* matchnocase */
#endif /* VDATA_FIELDS_ALL_UPPER */

/* ------------------------------------------------------------------
NAME
   VSelts -- get number of elements in a vdata
USAGE
   int32 VSelts(vkey)
   int32 vkey;      IN: vdata key
RETURNS
   On success returns the number of elements in the VDATA vkey; 
   returns FAIL  on error.
DESCRIPTION
--------------------------------------------------------------------- */
PUBLIC int32 
VSelts(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSelts");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if ((vs == NULL) || (vs->otag != DFTAG_VH))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    return (vs->nvertices);
}	/* VSelts */

/* ------------------------------------------------------------------
NAME
   VSgetinterlace -- gets interlace of the vdata vkey.
USAGE
   int32 VSgetinterlace(vkey)
   int32 vkey;     IN: vdata key
RETURNS
   On success returns the interlace (in the file) of the vdata vkey.
   returns FAIL on error.
DESCRIPTION
----------------------------------------------------------------------- */
PUBLIC int32 
VSgetinterlace(int32 vkey)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSgetinterlace");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_ARGS, FAIL);

    return ((int32) (vs->interlace));
}	/* VSgetinterlace */

/*------------------------------------------------------------------
NAME
   VSsetinterlace --  sets the vdata's interlace to full or none.
USAGE
   int32 VSsetinterlace(vkey, interlace)
   int32 vkey;       IN: vdata key
   int32 interlace;  IN: interlace for storing data in an HDF file.
RETURNS
   On success returns SUCCESS, returns FAIL on error.
DESCRIPTION
   The interlace may be one of FULL_INTERLACE or NO_INTERLACE.
--------------------------------------------------------------------*/
PUBLIC intn 
VSsetinterlace(int32 vkey, int32 interlace)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSsetinterlace");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    if (vs->access == 'r')
	HRETURN_ERROR(DFE_RDONLY, FAIL);
    if (vs->nvertices > 0)
	HRETURN_ERROR(DFE_NORESET, FAIL);

    /* currently only 2 kinds allowed */
    if (interlace == FULL_INTERLACE || interlace == NO_INTERLACE)
      {
	  vs->interlace = (int16) interlace;
	  return (TRUE);	/* ok */
      }
    else
	return (FAIL);
}	/* VSsetinterlace */

/*------------------------------------------------------------------
NAME
   VSgetfields -- returns the name (if any) of all the fields in the vdata
USAGE 
   int32 VSgetfields(vkey, fields)
   int32 vkey;      IN: vdata key
   char *fields;    OUT: storage for field names
RETURNS
   Returns -1 on error, else the no of fields in the vdata.
DESCRIPTION
   The fields are returned as a comma-separated string (e.g., "PX,PY").
----------------------------------------------------------------------*/
PUBLIC int32 
VSgetfields(int32 vkey, char *fields)
{
    int32       i;
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSgetfields");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    fields[0] = '\0';
    for (i = 0; i < vs->wlist.n; i++)
      {		/* build the comma-separated string */
	  HDstrcat(fields, vs->wlist.name[i]);
	  if (i < vs->wlist.n - 1)
	      HDstrcat(fields, ",");
      }

    return ((int32) vs->wlist.n);
}	/* VSgetfields */

/*------------------------------------------------------------------
NAME
   VSfexist -- tests for existence of 1 or more fields in a vdata.
USAGE
   int32 VSfexist(vkey, fields)
   int32 vkey;    IN: vdata key
   char *fields;  IN: Names of the fields to check for
RETURNS 
   RETURNS 1 if all fields exist; otherwise -1 is returned.
DESCRIPTION
   The argument 'fields' is a string of comma-separated fieldnames 
   (e.g. "PX,PY,PZ").
--------------------------------------------------------------------*/
PUBLIC intn 
VSfexist(int32 vkey, char *fields)
{
    char      **av, *s;
    int32       ac, i, j, found;
    VWRITELIST *w;
    vsinstance_t *wi;
    VDATA      *vs;
    CONSTR(FUNC, "VSfexist");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (wi = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = wi->vs;

    /* call scanattrs to parse the string */
    if (scanattrs(fields, &ac, &av) < 0)
	HRETURN_ERROR(DFE_BADFIELDS, FAIL);

    if ((vs == NULL) || (ac < 1))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* now check in vs's field table */
    w = &vs->wlist;
    for (i = 0; i < ac; i++)
      {
	  found = 0;
	  s = av[i];
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
	      return (FAIL);
      }

    return (TRUE);
}	/* VSfexist */

/* -----------------------------------------------------------------
NAME
   VSsizeof - computes the byte size of the field(s) of a vdata.
USAGE
   int32 VSsizeof(vkey, fields)
   int32 vkey;   IN: vdata key.
   char *fields; IN: Name(s) of the fields to check.
RETURNS
   The byte size of the field(s), positive integer, on success; 
   otherwise, returns FAIL.
DESCRIPTION
   The size is the actual size for the local machine.

----------------------------------------------------------------- */
PUBLIC int32 
VSsizeof(int32 vkey, char *fields)
{
    int32       totalsize, ac, i, j, found;
    char      **av;
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSsizeof");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_ARGS, FAIL);

    totalsize = 0;
    if (fields == NULL)
      {   /* count fieldsizes in vs */
	  for (j = 0; j < vs->wlist.n; j++)	
	      totalsize += vs->wlist.esize[j];
      }		
    else
      {
	  if ((scanattrs(fields, &ac, &av) < 0) || (ac < 1))
	      HRETURN_ERROR(DFE_ARGS, FAIL);
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
		    HRETURN_ERROR(DFE_ARGS, FAIL);
	    }	/* end for */
      }		/* end else */
    return (totalsize);
}	/* VSsizeof */

/*----------------------------------------------------------------- 
NAME
   VSdump - prints contents of a vdata (for debugging) 
USAGE
   VOID VSdump(vkey)
   int32 vkey;   IN: vdata key.
RETURNS
   No return codes.
-------------------------------------------------------------------*/
VOID 
VSdump(int32 vkey)
{
#ifdef OLD_WAY
    VWRITELIST *w;
    int32       i;
    CONSTR(FUNC, "VSdump");

    if (!vs)
      {
	  sprintf(sjs, "@VSdump: vs is null\n");
	  zj;
	  return;
      }

    sprintf(sjs, "@tag=%d ref=%d i=%d ", vs->otag, vs->oref, vs->interlace);
    zj;
    sprintf(sjs, "@nv=%ld\n ", vs->nvertices);
    zj;

    w = (VWRITELIST *) & vs->wlist;
    sprintf(sjs, "@vsize(hdf)=%d fields=%d [%s]\n", w->ivsize, w->n, vs->vsname);
    zj;

    for (i = 0; i < w->n; i++)
      {
	  sprintf(sjs, "@<%s>      type:%d esize=%d isize=%d off=%d\n",
	       w->name[i], w->type[i], w->esize[i], w->isize[i], w->off[i]);
	  zj;
      }

    sprintf(sjs, "\n");
    zj;
#else
    vkey = vkey;	/* suppress warning */
#endif /* OLD_WAY */
}	/* VSdump */

/*-------------------------------------------------------
NAME
   VSsetname - give a name to a vdata.
USAGE
   int32 VSsetname(vkey, vsname)
   int32 vkey;    IN: vdata key.
   char *vsname;  IN: name for the vdata.
RETURNS
   No Return values.
DESCRIPTION
   Truncates name to max length of VSNAMELENMAX
----------------------------------------------------------*/
PUBLIC int32 
VSsetname(int32 vkey, const char *vsname)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSsetname");
    int         slen;

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    if ((slen = HDstrlen(vsname)) > VSNAMELENMAX)
      {
	  HDstrncpy(vs->vsname, vsname, VSNAMELENMAX);
	  vs->vsname[VSNAMELENMAX] = '\0';
      }
    else
	HDstrcpy(vs->vsname, vsname);
    vs->marked = TRUE;
    return (SUCCEED);
}	/* VSsetname */

/* ------------------------------------------------------
NAME
   VSsetclass - give a class name to a vdata.
USAGE
   int32 VSsetclass(vkey, vsclass)
   int32 vkey;    IN: vdata key.
   char *vsclass;  IN: class name for the vdata.
RETURNS
   No Return values.
DESCRIPTION
   Truncates class name to max length of VSNAMELENMAX
----------------------------------------------------------*/
PUBLIC int32 
VSsetclass(int32 vkey, const char *vsclass)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSsetclass");
    int         slen;

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    if ((slen = HDstrlen(vsclass)) > VSNAMELENMAX)
      {
	  HDstrncpy(vs->vsclass, vsclass, VSNAMELENMAX);
	  vs->vsclass[VSNAMELENMAX] = '\0';
      }
    else
	HDstrcpy(vs->vsclass, vsclass);
    vs->marked = TRUE;
    return (SUCCEED);
}	/* VSsetclass */

/*------------------------------------------------------ 
NAME
   VSgetname - gets the vdata's name. 
USAGE
   int32 VSsetname(vkey, vsname)
   int32 vkey;    IN: vdata key.
   char *vsname;  OUT: storage for vdata name. 
RETURNS
   No Return values.
DESCRIPTION
----------------------------------------------------------*/
PUBLIC int32 
VSgetname(int32 vkey, char *vsname)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSgetname");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    HDstrcpy(vsname, vs->vsname);
    return (SUCCEED);
}	/* VSgetname */

/*------------------------------------------------------ 
NAME
   VSgetclass - gets the vdata's class name.
USAGE
   int32 VSgetclass(vkey, vsclass)
   int32 vkey;      IN: vdata key.
   char *vsclass;  OUT: class name for the vdata.
RETURNS
   No Return values.
DESCRIPTION
---------------------------------------------------------- */
PUBLIC int32 
VSgetclass(int32 vkey, char *vsclass)
{
    vsinstance_t *w;
    VDATA      *vs;
    CONSTR(FUNC, "VSgetclass");

    if (!VALIDVSID(vkey))
	HRETURN_ERROR(DFE_ARGS, FAIL);

    /* locate vg's index in vgtab */
    if (NULL == (w = (vsinstance_t *) vsinstance(VSID2VFILE(vkey), (uint16) VSID2SLOT(vkey))))
	HRETURN_ERROR(DFE_NOVS, FAIL);

    vs = w->vs;
    if (vs == NULL)
	HRETURN_ERROR(DFE_BADPTR, FAIL);

    HDstrcpy(vsclass, vs->vsclass);
    return (SUCCEED);
}	/* VSgetclass */

/* ------------------------------------------------------------------ 
NAME
   VSinquire - gets info about a vdata vkey:
USAGE
   int32 VSinquire(vkey, nelt, interlace, fields,eltsize, vsname)
   int32 vkey;         IN: vdata key.
   int32 *nelt;       OUT: number of vertices in the vdata.
   int32 *interlace;  OUT: interlace of the vdata.
   char  *fields;     OUT: field(s) name.
   int32 *eltsize;    OUT: byte size of elements (all fields) on local
                           machine. 
   char *vsname;      OUT: vdata's name.
RETURNS
   Returns FAIL if error;       Returns 1 if ok.

------------------------------------------------------------------------*/
PUBLIC intn 
VSinquire(int32 vkey, int32 *nelt, int32 *interlace,
	  char *fields, int32 *eltsize, char *vsname)
{
#ifdef LATER
    CONSTR(FUNC, "VSinquire");
#endif

    if (fields)
	VSgetfields(vkey, fields);
    if (nelt)
	*nelt = VSelts(vkey);
    if (interlace)
	*interlace = VSgetinterlace(vkey);
    if (eltsize)
	*eltsize = VSsizeof(vkey, fields);
    if (vsname)
	VSgetname(vkey, vsname);

    return (SUCCEED);	/* ok */
}	/* VSinquire */

/*-----------------------------------------------------------------
NAME
   VSlone - returns an array of refs of all lone vdatas in the file.
USAGE
   int32 VSlone(f, idarray, asize)
   HFILEID f;        IN: file id.
   int32 *idarray;   IN: array to contain the refs.
   int32 asize;      IN: size of idarray.
RETURNS
   Returns -1 if error; otherwise returns the total number of lone 
   vdatas in the file
DESCRIPTION
   If idarray is too small, routine will only fill idarray with up
   to asize worth of refs.
---------------------------------------------------------------------*/
PUBLIC int32 
VSlone(HFILEID f, int32 *idarray, int32 asize)
{
    uint8      *lonevdata;	/* lcl wrk area: stores flags of vdatas */
    int32       i, vgid, vsid, vstag;
    int32       vkey;
    int32       nlone;		/* total number of lone vdatas */
    CONSTR(FUNC, "VSlone");

    /* -- allocate space for vdata refs, init to zeroes -- */
    if (NULL == (lonevdata = (uint8 *) HDgetspace(65000L * sizeof(uint8))))
	            HRETURN_ERROR(DFE_NOSPACE, FAIL);
    HDmemset(lonevdata, 0, 65000L * sizeof(uint8));

    /* -- look for all vdatas in the file, and flag (1) each -- */
    vsid = -1;
    while (-1L != (vsid = VSgetid(f, vsid)))	/* until no more vdatas */
	lonevdata[vsid] = 1;

    /* -- Look through all vgs, searching for vdatas -- */
    /* -- increment its index in lonevdata if found -- */
    vgid = -1;
    while (-1L != (vgid = Vgetid(f, vgid)))
      {		/* until no more vgroups */
	  vkey = Vattach(f, vgid, "r");
	  for (i = 0; i < Vntagrefs(vkey); i++)
	    {
		Vgettagref(vkey, i, &vstag, &vsid);
		if (vstag == (int32) DFTAG_VH)
		    lonevdata[vsid] = 0;
	    }
	  Vdetach(vkey);
      }

    /* -- check in lonevdata: it's a lone vdata if its flag is still 1 -- */
    nlone = 0;
    for (i = 0; i < 65000; i++)
      {
	  if (lonevdata[i])
	    {
		if (nlone < asize)	/* insert into idarray up till asize */
		    idarray[nlone] = i;
		nlone++;
	    }
      }
    HDfreespace((VOIDP) lonevdata);

    return (nlone);	/* return the TOTAL # of lone vdatas */
}	/* VSlone */

/*----------------------------------------------------------------- 
NAME
   Vlone  - returns an array of refs of all lone vgroups in the file.
USAGE
   int32 Vlone(f, idarray, asize)
   HFILEID f;        IN: file id.
   int32 *idarray;   IN: array to contain the refs.
   int32 asize;      IN: size of idarray.
RETURNS
   Returns -1 if error; otherwise returns the total number of lone
   vgroups in the file
DESCRIPTION
   If idarray is too small, routine will only fill idarray with up
   to asize worth of refs.
---------------------------------------------------------------------*/
PUBLIC int32 
Vlone(HFILEID f, int32 *idarray, int32 asize)
{
    uint8      *lonevg;		/* local wrk area: stores flags of vgroups */
    int32       i;
    int32       vgid, vstag, id;
    int32       vkey;
    int32       nlone;		/* total number of lone vgroups */
    CONSTR(FUNC, "Vlone");

    /* -- allocate space for vgroup refs, init to zeroes -- */
    if (NULL == (lonevg = (uint8 *) HDgetspace(65000L * sizeof(uint8))))
	            HRETURN_ERROR(DFE_NOSPACE, FAIL);
    HDmemset(lonevg, 0, 65000L * sizeof(uint8));

    /* -- look for all vgroups in the file, and flag (1) each -- */
    id = -1;
    while (-1L != (id = Vgetid(f, id)))		/* until no more vgroups */
	lonevg[id] = 1;

    /* -- Look through all vgs, searching for vgroups -- */
    /* -- increment its index in lonevg if found -- */
    vgid = -1;
    while (-1L != (vgid = Vgetid(f, vgid)))
      {		/* until no more vgroups */
	  vkey = Vattach(f, vgid, "r");
	  id = -1;
	  for (i = 0; i < Vntagrefs(vkey); i++)
	    {
		Vgettagref(vkey, i, &vstag, &id);
		if (vstag == DFTAG_VG)
		    lonevg[id] = 0;
	    }
	  Vdetach(vkey);
      }

    /* -- check in lonevg: it's a lone vgroup if its flag is still 1 -- */
    nlone = 0;
    for (i = 0; i < 65000; i++)
      {
	  if (lonevg[i])
	    {
		if (nlone < asize)	/* insert into idarray up till asize */
		    idarray[nlone] = i;
		nlone++;
	    }
      }
    HDfreespace((VOIDP) lonevg);

    return (nlone);	/* return the TOTAL # of lone vgroups */

}	/* Vlone */

/* ----------------------------------------------------------------- 
NAME
   Vfind -- looks in the file and returns the ref of 
            the vgroup with name vgname 
USAGE
   int32 Vfind(f, vgname)
   HFILEID  f;    IN: file id.
   char *vgname;  IN: name of the vgroup.
RETURNS
   Returns 0 if not found, or error. Otherwise, returns the 
   vgroup's ref (a positive integer).
-----------------------------------------------------------------------*/
int32 
Vfind(HFILEID f, const char *vgname)
{
    int32       vgid = -1;
    int32       ret_ref;
    int32       vkey;
    char        name[512];
#ifdef LATER
    CONSTR(FUNC, "Vfind");
#endif

    while (-1L != (vgid = Vgetid(f, vgid)))
      {
	  vkey = Vattach(f, vgid, "r");
	  if (vkey == FAIL)
	      return (0);	/* error */

	  Vgetname(vkey, name);
	  if (!HDstrcmp(vgname, name))
	    {
		ret_ref = VQueryref(vkey);
		Vdetach(vkey);
		return (ret_ref);	/* found the vgroup */
	    }	/* else */

	  Vdetach(vkey);

      }
    return (0);		/* not found */

}	/* Vfind */

/*------------------------------------------------------------------
NAME
   VSfind -- looks in the file and returns the ref of the vdata 
             with name vsname 
USAGE
   int32 VSfine(f, vsname)
   HFILEID  f;      IN: file id.
   char *vsname;    IN: name of the vdata.
RETURNS
   Returns 0 if not found, or error. Otherwise, returns the vdata's 
   ref (a positive integer).
---------------------------------------------------------------------*/
int32 
VSfind(HFILEID f, const char *vsname)
{
    int32       vsid = -1;
    int32       ret_ref;
    int32       vkey;
    char        name[512];
#ifdef LATER
    CONSTR(FUNC, "VSfind");
#endif

    while (-1L != (vsid = VSgetid(f, vsid)))
      {
	  vkey = VSattach(f, vsid, "r");
	  if (vkey == FAIL)
	      return (0);	/* error */
	  VSgetname(vkey, name);
	  if (!HDstrcmp(vsname, name))
	    {
		ret_ref = VSQueryref(vkey);
		VSdetach(vkey);
		return (ret_ref);	/* found the vdata */
	    }	/* end if */
	  VSdetach(vkey);
      }
    return (0);		/* not found */
}	/* VSfind */

/* ------------------------------- Vsetzap -------------------------------- */
/*
 * Vsetzap: Useless now. Maintained for back compatibility.
 */
PUBLIC VOID 
Vsetzap(void)
{
}
