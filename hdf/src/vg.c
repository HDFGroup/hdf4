#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/01/19 05:56:12  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*
*
* vg.c
* Part of the HDF VSet interface
* This file contains routine to handle VDATAs.
*
* Most routines return -1 (FAIL) on error.  Some must return 0 on error.
* VSattach returns NULL on error.
*
* PRIVATE functions manipulate vsdir and are used only within this file.
* PRIVATE data structures in here pertain to vdata in vsdir only.
*
**************************************************************************/

#include "vg.h"

/* ------------------------------------------------------------------
*	Vnewref
*	utility routine. returns a unique reference number.
*
*	RETURNS a unique ref (+ve unsigned 16-bit integer) ,
*	RETURNS 0 if error
*
*  undocumented
*
*/

#ifdef PROTOTYPE
uint16 vnewref (HFILEID f)		
#else
uint16 vnewref (f)		

	HFILEID	f;
#endif

{
	uint16 r;
	char * FUNC = "vnewref";

    r = (uint16) Hnewref (f);
	if (r == 0)  HERROR(DFE_NOFREEDD);

	return (r);

} /* vnewref */

/* ================================================================== */

/* matchnocase -  (PRIVATE) compares 2 strings, ignoring case 
*	               returns TRUE if match, else FALSE
*/

#ifdef PROTOTYPE
int32 matchnocase (char *strx, char *stry)   
#else
int32 matchnocase (strx, stry) 

	char *strx,*stry;
#endif

{
	int32 	i,nx,ny;
	int16		tx,ty;
	char 		*sx, *sy;

    nx = HDstrlen(strx);
    ny = HDstrlen(stry);
	if (nx != ny) return(FALSE);  /* different lengths */

	for (sx=strx, sy=stry, i=0;i<nx;i++,sx++,sy++) {
		tx= *sx; 
		ty= *sy;
		if (islower(tx)) tx=toupper(tx);
		if (islower(ty)) ty=toupper(ty);
		if (tx != ty) 	  return (FALSE);
	}

	return (TRUE);

} /* matchnocase */


/* ------------------------------------------------------------------
* VSelts
* returns the number of elements in the VDATA vs 
* returns FAIL  on error.
*
* undocumented
* 28-MAR-91 Jason NG NCSA
*
*/

#ifdef PROTOTYPE
PUBLIC int32 VSelts (VDATA *vs)  
#else
PUBLIC int32 VSelts (vs)  

	VDATA * vs;
#endif
{
	char * FUNC = "VSelts";

	return( (vs->otag==VSDESCTAG) ?  vs->nvertices : FAIL);

} /* VSelts */


/* ------------------------------------------------------------------
*	VSgetinterlace 
*  returns the interlace (in the file) of the vdata vs.
*  returns FAIL on error.
*
*  undocumented
*
*/

#ifdef PROTOTYPE
PUBLIC int32 VSgetinterlace (VDATA *vs) 
#else
PUBLIC int32 VSgetinterlace (vs) 

	VDATA * vs;
#endif

{
	char * FUNC = "VSgetinterlace";

    return( (vs==NULL) ? (int32)FAIL : (int32) vs->interlace );

} /* VSgetinterlace */

/* ------------------------------------------------------------------
*	VSsetinterlace 
* 	sets the vdata's interlace to full or none.
*	returns FAIL on error.
*/

#ifdef PROTOTYPE
PUBLIC int32 VSsetinterlace (VDATA *vs, int32 interlace)
#else
PUBLIC int32 VSsetinterlace (vs, interlace)

	VDATA * vs;
	int32 interlace;
#endif

{
	char * FUNC = "VSsetinterlace";

	if(vs == NULL)        {HERROR(DFE_BADPTR);  return(FAIL);}
	if(vs->access == 'r') {HERROR(DFE_RDONLY);  return(FAIL);}
   if(vs->nvertices > 0) {HERROR(DFE_NORESET); return(FAIL);}

	/* currently only 2 kinds allowed */

	if ( interlace == FULL_INTERLACE || 
	    interlace == NO_INTERLACE ) {
                vs->interlace = (int16)interlace;
				return (TRUE); /* ok */
	} 
	else  return (FAIL);  	  

} /* VSsetinterlace */


/* ------------------------------------------------------------------
*	VSgetfields 
*  returns the fieldnames in a vdata.
*  RETURNS  -1 on error, else the no of fields in the vdata.
*
*	undocumented
*
*/

#ifdef PROTOTYPE
PUBLIC int32 VSgetfields (VDATA *vs, char *fields)		
#else
PUBLIC int32 VSgetfields (vs, fields)		

	VDATA * vs;
	char  * fields;			/* fieldnames are returned in this var */
#endif

{
	int32 i;
    char * FUNC = "VSgetfields";

	if (vs==NULL) {
       HERROR(DFE_BADPTR);
       return(FAIL);
       }

	fields[0] = '\0';
	for (i=0;i<vs->wlist.n;i++) { /* build the comma-separated string */
        HDstrcat(fields,vs->wlist.name[i]);
		if ( i < vs->wlist.n - 1 )
            HDstrcat(fields,",");
	}

	return ((int32) vs->wlist.n);

} /* VSgetfields */

/* ------------------------------------------------------------------
*	VSfexist
* 	tests for existence of 1 or more fields in a vdata.
*
*	RETURNS -1 if false, or error
*	RETURNS 1 if true
*/

#ifdef PROTOTYPE
PUBLIC int32 VSfexist (VDATA *vs, char *fields)
#else
PUBLIC int32 VSfexist (vs, fields)      

	VDATA * vs;
	char	* fields;
#endif

{
	char   		**av, *s;
	int32			ac,i,j,found;
	VWRITELIST	*w;
	char * FUNC = "VSfexist";

	/* call scanattrs to parse the string */

	if (scanattrs(fields,&ac,&av) < 0) {
          HERROR(DFE_BADFIELDS);
          return (FAIL);
        }

	if ((vs == NULL) || (ac<1)) {
          HERROR(DFE_ARGS);
          return (FAIL); 	
        }

	/* now check in vs's field table */

	w = &vs->wlist;
	for (i=0;i<ac;i++) {
		for (found=0,s=av[i],j=0;j<w->n;j++) {
			if ( matchnocase(s,w->name[j]) ) {
				found = 1;
				break;
			}
		}
		if (!found) return (FAIL);
	}

	return (1);

} /* VSfexist */


/* ================================================================== */
/*
*	VSsizeof - computes the byte size of the field(s) of a vdata.
*	         - Note that the size is the actual size for the local machine.
*
*		 - RETURNS FAIL on error, else the field(s) size (+ve integer).
*/

#ifdef PROTOTYPE
PUBLIC int32 VSsizeof (VDATA *vs, char *fields) 
#else
PUBLIC int32 VSsizeof (vs, fields) 

	VDATA *vs;
	char  *fields;
#endif

{
	int32 	totalsize, ac, i,j,found;
	char   	**av;
	char * FUNC = "VSsizeof";

    if((vs==NULL) || (scanattrs(fields,&ac,&av) < 0) || (ac<1)) {
          HERROR(DFE_ARGS);
          return(FAIL);
        }

	if (vjv) { 
		sprintf(sjs,"#VSsizeof: fields are [%s]\n",fields);
		zj; 
	}

	totalsize=0;
	for (i=0;i<ac;i++) {
		for (found=0,j=0;j<vs->wlist.n;j++)
                  /* check fields in vs */
            if (!HDstrcmp(av[i], vs->wlist.name[j])) {
				totalsize += vs->wlist.esize[j];
				found=1;
				break;
			}

		if (!found) {
                        HERROR(DFE_ARGS);
                        HEreport("VSsizeof:[%s] not in vs", av[i]);
			return(FAIL);
		}
	}

	return(totalsize);

} /* VSsizeof */

/* ================================================================== */

/*
*	VSdump - prints contents of a vdata (for debugging) 
*				no return codes.
*/

#ifdef PROTOTYPE
void VSdump (VDATA *vs)                       
#else
void VSdump (vs)                       

	VDATA *vs;
#endif

{
	VWRITELIST	*w;
	int32 		i;
	char * FUNC = "VSdump";

	if (!vs) { 
          sprintf(sjs,"@VSdump: vs is null\n"); zj; 
          return; 
        }

	sprintf(sjs,"@tag=%d ref=%d i=%d ",vs->otag, vs->oref,vs->interlace); zj;
    sprintf(sjs,"@nv=%ld\n ",vs->nvertices); zj;

	w = (VWRITELIST*) &vs->wlist;
	sprintf(sjs,"@vsize(hdf)=%d fields=%d [%s]\n",w->ivsize,w->n,vs->vsname); zj;

	for (i=0;i<w->n;i++) { 
		sprintf(sjs,"@<%s>      type:%d esize=%d isize=%d off=%d\n",
		    w->name[i], w->type[i], w->esize[i],w->isize[i],w->off[i]);
		zj;
	}

	sprintf(sjs,"\n"); zj;

} /* VSdump */


/* ======================================================= */
/*
*	VSsetname - give a name to a vdata.
*	          - NO RETURN VALUES
*	          - truncates name to max length of VSNAMELENMAX
*      
*/

#ifdef PROTOTYPE
PUBLIC void VSsetname (VDATA *vs, char *vsname)
#else
PUBLIC void VSsetname (vs, vsname)

	VDATA *vs;
	char	*vsname;
#endif

{
	char * FUNC = "VSsetname";

	if (vs == NULL) return;
    if ( HDstrlen(vsname) > VSNAMELENMAX) {
        HDstrncpy(vs->vsname, vsname,VSNAMELENMAX);
		vs->vsname[VSNAMELENMAX]='\0';
	}
	else 
        HDstrcpy(vs->vsname, vsname);
	vs->marked = TRUE;
	return;

} /* VSsetname */

/* ======================================================= */
/*
*	VSsetclass- assigns a class name to a vdata.
*	          - NO RETURN VALUES
*	          - truncates to max length of VSNAMELENMAX
*      
*/

#ifdef PROTOTYPE
PUBLIC void VSsetclass (VDATA *vs, char *vsclass)
#else

PUBLIC void VSsetclass (vs, vsclass)

	VDATA *vs;
	char	*vsclass;
#endif

{
	char * FUNC = "VSsetclass";

	if (vs == NULL) return;
    if ( HDstrlen(vsclass) > VSNAMELENMAX) {
        HDstrncpy(vs->vsclass, vsclass,VSNAMELENMAX);
		vs->vsclass[VSNAMELENMAX]='\0';
	}
	else 
        HDstrcpy(vs->vsclass, vsclass);
	vs->marked = TRUE;
	return;

} /* VSsetclass*/

/* ======================================================= */

/*
*	VSgetname - gets the vdata's name.
*				 - NO RETURN VALUES
*/

#ifdef PROTOTYPE
PUBLIC void VSgetname (VDATA *vs, char *vsname)      
#else

PUBLIC void VSgetname (vs, vsname)      

	VDATA *vs;
	char	*vsname;
#endif

{
	char * FUNC = "VSgetname";

    if (vs != NULL) HDstrcpy(vsname, vs->vsname);
	return;

} /* VSgetname */

/* ======================================================= */

/*
*	VSgetclass - gets the vdata's class name.
*				 - NO RETURN VALUES
*/

#ifdef PROTOTYPE
PUBLIC void VSgetclass (VDATA *vs, char *vsclass)   
#else

PUBLIC void VSgetclass (vs, vsclass)   

	VDATA *vs;
	char	*vsclass;
#endif

{
	char * FUNC = "VSgetclass";

    if (vs != NULL) HDstrcpy(vsclass, vs->vsclass);
	return;

} /* VSgetclass */


/* ================================================================== */
/*
*	VSinquire - gets info about a vdata vs:
*
*		  nvertices: 	no of vertices in it.
*		  interlace: 	its interlcae
*		  fields : 	a comma separated string listing the field(s). 
*                                                 (eg "PX,PY")
*		  eltsize: 	size of elmt (all field(s)) on local machine.
*		  vsname: 	vdata's name, if any.
*
*	RETURNS FAIL if error
*	RETURNS 1 if ok
*
*/


#ifdef PROTOTYPE
PUBLIC int32 VSinquire (VDATA *vs, int32 *nelt, int32 *interlace,
        char *fields, int32 *eltsize, char *vsname)
#else

PUBLIC int32 VSinquire (vs, nelt, interlace, fields, eltsize, vsname)   

	VDATA 	*vs;
	char  	*fields, *vsname;
	int32 	*nelt, *interlace, *eltsize;
#endif

{
	char * FUNC = "VSinquire";

	if (!vs) {
          HERROR(DFE_BADPTR);
          return(FAIL);
        }
	
	if(fields)
	  VSgetfields (vs,fields);
	if(nelt) 
	  *nelt       = (int32) vs->nvertices;
	if(interlace)
	  *interlace  = (int32) vs->interlace;
	if(eltsize)
	  *eltsize    =  (int32) VSsizeof (vs,fields);
	if(vsname)
      HDstrcpy(vsname,vs->vsname);

	return (SUCCEED); /* ok */

} /* VSinquire */

/* ================================================================== */
/*
* VSlone - returns an array of refs of all lone vdatas in the file.
*      - returns -1 if error
*      - otherwise returns the total number of lone vdatas in the file 
*
*		If idarray is too small, routine will only fill idarray with up
*		 to asize worth of refs.
*
*		INPUT idarray: user supplies  an int array.
*	   INPUT asize: integer specifying how many ints in idarray[];
*		INPUT f: HDF file pointer.
*
*/

#ifdef PROTOTYPE
PUBLIC int32 VSlone(HFILEID f, int32 idarray[], int32 asize) 
#else

PUBLIC int32 VSlone(f, idarray, asize) 

	HFILEID	f;
	int32  	idarray[];		 /* array to contain the refs */
	int32 	asize;            /* input: size of idarray */
#endif

{
	int16 	*lonevdata; /* lcl wrk area: stores flags of vdatas */
	int32 	i,vgid, vsid, vstag;
	VGROUP 	* vg;
	int32 	nlone; /* total number of lone vdatas */
	char * FUNC = "VSlone";


/* -- allocate space for vdata refs, init to zeroes -- */
    if (NULL == (lonevdata = (int16*) HDgetspace( 65000 * sizeof(int16))))
      { HERROR(DFE_NOSPACE); return(FAIL); }
    for(i=0;i<65000;i++) lonevdata[i] = 0;

/* -- look for all vdatas in the file, and flag (1) each -- */
    vsid = -1;
    while( -1L != (vsid = VSgetid (f, vsid)))   /* until no more vdatas */
        lonevdata[vsid ] = 1;

/* -- Look through all vgs, searching for vdatas -- */
/* -- increment its index in lonevdata if found -- */
    vgid = -1;
    while( -1L != (vgid = Vgetid (f, vgid))) { /* until no more vgroups */
        vg = (VGROUP*) Vattach(f,vgid,"r");
        for (i=0; i< Vntagrefs(vg); i++) {
            Vgettagref (vg, i, &vstag, &vsid);
            if (vstag==VSDESCTAG)
                { lonevdata[vsid]++;  }
            }
        Vdetach(vg);
        }

/* -- check in lonevdata: it's a lone vdata if its flag is still 1 -- */
    nlone = 0;
    for(i=0;i<65000;i++) {
        if (1 == lonevdata[i]) {
            if (nlone < asize) { /* insert into idarray up till asize */
                idarray[nlone] = i;
                }
            nlone ++;
            }
       }
    HDfreespace(lonevdata);

    return (nlone); /* return the TOTAL # of lone vdatas */

} /* VSlone */

/* ================================================================== */
/*
* Vlone  - returns an array of refs of all lone vgroups in the file.
* 	      - returns -1 if error
*	      - otherwise returns the total number of lone vgroups in the file 
*
*			If idarray is too small, routine will only fill idarray with up
*			 to asize worth of refs.
*
*			INPUT idarray: user supplies  an int array.
*		   INPUT asize: integer specifying how many ints in idarray[];
*			INPUT f: HDF file pointer.
*
*/

#ifdef PROTOTYPE
PUBLIC int32 Vlone (HFILEID f, int32 idarray[], int32 asize) 
#else

PUBLIC int32 Vlone (f, idarray, asize) 

	HFILEID	f;
	int32 	idarray[];		 /* array to contain the refs */
	int32   asize;            /* input: size of idarray */
#endif

{
	int16		*lonevg; /* local wrk area: stores flags of vgroups */
	int32		i;
	int32 	vgid, vstag, id;
	VGROUP 	* vg;
	int32 	nlone; /* total number of lone vgroups */
	char * FUNC = "Vlone";

/* -- allocate space for vgroup refs, init to zeroes -- */
    if (NULL == (lonevg = (int16*) HDgetspace ( 65000 * sizeof(int16))))
      { HERROR(DFE_NOSPACE); return(FAIL); }
    for(i=0;i<65000;i++) lonevg[i] = 0;

/* -- look for all vgroups in the file, and flag (1) each -- */
    id = -1;
    while( -1L != (id = Vgetid (f, id)))   /* until no more vgroups */
        lonevg[ id ] = 1;

/* -- Look through all vgs, searching for vgroups -- */
/* -- increment its index in lonevg if found -- */
    vgid = -1;
    while( -1L != (vgid = Vgetid (f, vgid))) {  /* until no more vgroups */
       printf("Vlone: vgid=%ld..attach",vgid);
        vg = (VGROUP*) Vattach(f,vgid,"r");
       printf("..attach done\n");
        id = -1;
        for (i=0; i< Vntagrefs(vg); i++) {
            Vgettagref (vg, i, &vstag, &id);
            if (vstag==VGDESCTAG) { lonevg[id]++;  }
            }
        Vdetach(vg);
        }

/* -- check in lonevg: it's a lone vgroup if its flag is still 1 -- */
nlone = 0;
for(i=0;i<65000;i++) {
	if (1 == lonevg[i]) {
	 	if (nlone < asize) { /* insert into idarray up till asize */
			idarray[nlone] = i;
			}
		nlone ++;
		}
   }
HDfreespace (lonevg);

return (nlone); /* return the TOTAL # of lone vgroups */

} /* Vlone */


/* ================================================================== */
/* new jan 3 1991 */
/* looks in the file and returns the ref of the vgroup with name vgname */
/* 
* returns 0 if not found, or error.
* otherwise, returns the vgroup's ref (a +ve integer).
*/

#ifdef PROTOTYPE
int32 Vfind (HFILEID f, char *vgname)
#else

int32 Vfind (f, vgname)

	HFILEID	f;
	char 	* vgname;
#endif

{
  	int32 	vgid = -1;
 	VGROUP* 	vg;
  	char 		name[512];
	char * 	FUNC = "Vfind";

    while ( -1L != (vgid=Vgetid(f, vgid)) ) {
		vg = (VGROUP*) Vattach(f,vgid,"r");
		if (vg==NULL) return(0); 			/* error */
		Vgetname(vg, name);
		Vdetach (vg);
        if (!HDstrcmp(vgname,name))
                  return ((int32) vg->oref);  /* found the vgroup */
  	}
  	return(0); /* not found */

} /* Vfind */

/* ================================================================== */
/* new jan 3 1991 */
/* looks in the file and returns the ref of the vdata with name vsname */
/* 
* returns 0 if not found, or error.
* otherwise, returns the vdata's ref (a +ve integer).
*/

#ifdef PROTOTYPE
int32 VSfind (HFILEID f, char *vsname)
#else

int32 VSfind (f, vsname)
	HFILEID	f;
	char * vsname;
#endif

{
  	int32 	vsid = -1;
  	VDATA * 	vs;
  	char 		name[512];
	char * 	FUNC = "VSfind";

    while ( -1L != (vsid=VSgetid(f, vsid)) ) {
		vs = (VDATA*) VSattach(f,vsid,"r");
		if (vs==NULL) return(0); 			/* error */
		VSgetname(vs, name);
		VSdetach (vs);
        if (!HDstrcmp(vsname, name))
                  return ((int32) vs->oref);  /* found the vdata */
  	}
  	return(0); /* not found */

} /* VSfind */

/* ================================================================== */

/*
* Vsetzap: Useless now. Maintained for back compatibility.
*/

#ifdef PROTOTYPE
PUBLIC void Vsetzap(void) 
#else

PUBLIC void Vsetzap() 
#endif

{

	if (vjv) { sprintf(sjs,"Vsetzap: defunct\n"); zj; }
}
/* ================================================================== */
