#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/11/24 16:49:49  chouck
Fixed a return value in VSsetfields()

 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*****************************************************************************
* Likkai Ng NCSA Feb 92 - update to use H-routines

* Likkai Ng May 1991  NCSA 
*
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
	    "PX", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,
	    "PY", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,
	    "PZ", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,

	    "IX", DFNT_INT32,	SIZE_INT32, 1,
	    "IY", DFNT_INT32,	SIZE_INT32, 1,
	    "IZ", DFNT_INT32,	SIZE_INT32, 1,

	    "NX", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,
	    "NY", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,
	    "NZ", DFNT_FLOAT32,	 SIZE_FLOAT32, 1,

};

#define NRESERVED ( sizeof(rstab)/sizeof(SYMDEF) )
#define LOCALSIZETAB_SIZE sizeof(local_sizetab)/sizeof(int)
#define HDFSIZETAB_SIZE sizeof(hdf_sizetab)/sizeof(int)

/*
 ** returns the machine size of a field type
 ** returns FAIL if error
 */
#ifdef PROTOTYPE
int16 SIZEOF (int16 x)
#else
int16 SIZEOF (x)
	int16 x;
#endif
{
  if (x < 0 || x > LOCALSIZETAB_SIZE-1) {
    return(FAIL);
  } else {
    return(local_sizetab[x]);
  }
} /* SIZEOF */

/*
 ** returns the HDF file size of a field type
 ** returns FAIL if error.  
 ** USE ONLY FOR BACKWARD COMPATABILITY
 */
#ifdef PROTOTYPE
int16 HDFSIZEOF (int16 x)
#else
int16 HDFSIZEOF (x)
     int16 x; 
#endif
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

#ifdef PROTOTYPE
PUBLIC int32 VSsetfields (VDATA *vs, char *fields)  
#else
PUBLIC int32 VSsetfields (vs,fields)      
     VDATA   *vs;
     char    *fields;
#endif
{
  char  **av;
  int32   ac, found;
  register intn j, i;
  int16   order;
  VREADLIST     rlist;
  VWRITELIST  wlist;
  char  * FUNC = "VSsetfields";
  
  if (vs==NULL) return(FAIL);
  if (scanattrs(fields,&ac,&av) < 0) {
    sprintf(sjs,"@bad fields string [%s]\n",fields);
    zj;
    return(FAIL); /* bad fields string */
  }
  if (ac==0) return(FAIL);
  
  /* 
   * write to an empty vdata : set the write list but do not set the 
   *   read list cuz there is nothing there to read yet...
   */
  if(vs->access == 'w' && vs->nvertices == 0) {
    wlist        = vs->wlist;
    wlist.ivsize = 0;
    wlist.n      = 0;
    for(i = 0; i < ac; i++) {
      /* --- first look in the reserved symbol table --- */
      for(found = 0, j = 0; j < NRESERVED; j++)
        if (!HDstrcmp(av[i], rstab[j].name)) {
          found = 1;
          
          HDstrcpy( wlist.name[wlist.n],rstab[j].name);
          order = rstab[j].order;
          wlist.type[wlist.n]  =  rstab[j].type;
          wlist.order[wlist.n] =  order;
          wlist.esize[wlist.n] =  order * DFKNTsize(rstab[j].type | DFNT_NATIVE);
          wlist.isize[wlist.n] =  order * rstab[j].isize;
          wlist.ivsize  += wlist.isize[wlist.n];
          wlist.n++;
          break;
        }
      
      /* --- now look in the user's symbol table --- */
      if(!found) {
        for(found = 0,j = 0; j < vs->nusym; j++)
          if (!HDstrcmp(av[i], vs->usym[j].name)) {
            found = 1;
            
            HDstrcpy (wlist.name[wlist.n], vs->usym[j].name);
            order = vs->usym[j].order;
            wlist.type[wlist.n]  = vs->usym[j].type;
            wlist.order[wlist.n] = order;
            wlist.esize[wlist.n] = order * DFKNTsize(vs->usym[j].type | DFNT_NATIVE);
            wlist.isize[wlist.n] = order * vs->usym[j].isize;
            wlist.ivsize+= wlist.isize[wlist.n];
            wlist.n++;
            break;
          }
      }
      if (!found) {    /* field is not a defined field - error  */
        sprintf(sjs,"@Vsetfield:field [%s] unknown\n",av[i]); zj;
        return(FAIL);
      }
    }
    
    /* *********************************************************** */
    /* ensure fields with order > 1 are alone  */
    for (j=0,i=0;i<wlist.n;i++)
      if (wlist.order[i] >1 && wlist.n != 1) {
        sprintf(sjs,"@Vsetf: [%s] in [%s] has order %d. error.\n",
                wlist.name[i], fields, wlist.order[i]); zj;
        return(FAIL);
      }
    /* *********************************************************** */
    
    /* compute and save the fields' offsets */
    for (j=0,i=0;i<wlist.n;i++) {
      wlist.off[i] =j;
      j += wlist.isize[i];
    }
    
    /* copy from wlist (temp) into vdata */
    HDmemcpy((VOIDP) &(vs->wlist), (VOIDP) &(wlist), sizeof(wlist));
    
    return(SUCCEED); 
    
  } /* writing to empty vdata */

  /*
   *   No matter the access mode, if there are elements in the VData 
   *      we should set the read list
   */
  if(vs->nvertices > 0) {
    rlist   = vs->rlist;
    rlist.n = 0;
    for (i=0;i<ac;i++) {
      for (found=0, j=0; j<vs->wlist.n; j++)
        if (!HDstrcmp(av[i], vs->wlist.name[j]) ) { /*  see if field exist */
          found = 1;
          
          rlist.item[rlist.n] = j; /* save as index into wlist->name */
          rlist.n++;
          break;
        }
      if (!found)  {    /* field does not exist - error */
        sprintf(sjs,"@Vsetfield: field [%s] does not exist in vs\n",
                av[i]); zj;
        return(FAIL);
      }
    }
    
    /* copy from rlist (temp) into vdata */
    HDmemcpy((VOIDP) &(vs->rlist), (VOIDP) &(rlist), sizeof(rlist));
    
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

#ifdef PROTOTYPE
PUBLIC int32 VSfdefine (VDATA *vs, char *field, int32 localtype, int32 order)   
#else
PUBLIC int32 VSfdefine (vs, field, localtype, order)   
     VDATA   *vs;
     char    *field;
     int32   localtype, order;
#endif
{
  char 	**av;
  int32 ac;
  char 	*ss;
  int16 usymid, replacesym;
  register intn j;
  char  * FUNC = "VSfdefine";
  
  if ((vs == NULL) || (scanattrs(field,&ac,&av) < 0) || (ac != 1)) {
    HRETURN_ERROR(DFE_ARGS, FAIL);
  }
  
  if (order <1 || order > 1000) {
    sprintf(sjs,"@VSfdefine: error order %ld\n",order);
    zj;
    return(FAIL);
  }
  
  /* 
   ** check for any duplicates 
   */
  /* --- first look in the reserved symbol table --- */
  for (j=0;j<NRESERVED;j++)
    if (!HDstrcmp(av[0], rstab[j].name)) {
      if (localtype != rstab[j].type && order != rstab[j].order) {
        sprintf(sjs,"@VSfdefine warning: predefined field [%s] redefined.\n", av[0]); zj;
        break;
      }
    }

  /* --- then look in the user's symbol table --- */
  for (replacesym = 0,j = 0; j < vs->nusym; j++)
    if (!HDstrcmp(av[0], vs->usym[j].name))  {
      if (localtype != rstab[j].type && order != rstab[j].order) {
        sprintf(sjs,"@VSfdefine warning: user field [%s] redefined.\n",av[0]); zj; 
        replacesym = 1;
        break;
      }
    }
  
  if (replacesym)  
    usymid = j; /* new definition will replace old at this index */
  else 
    usymid = vs->nusym;
  
  if ((vs->usym[usymid].isize = DFKNTsize( (int32) localtype)) == FAIL) {
    sprintf(sjs,"@Vfdefine: bad type (=%ld) for [%s]\n", localtype, av[0]); zj;
    return(FAIL);
  }
  
  j  = HDstrlen(av[0]) + 1;
  
  if( (ss = (char*) HDgetspace (j))==NULL)
    HRETURN_ERROR(DFE_NOSPACE, FAIL);
  
  HDstrcpy(ss, av[0]);
  vs->usym[usymid].name  = ss;
  vs->usym[usymid].type  = (int16)localtype;
  vs->usym[usymid].order = (int16)order;
  
  /* prevent user-symbol table overflow */
  if (vs->nusym >= USYMMAX) {
    sprintf(sjs,"@VSFDEFINE: %d too many symbols\n",vs->nusym); zj;
    for(j = 0; j < vs->nusym; j++)
      { sprintf(sjs,"@ sym: %d [%s]\n",j,vs->usym[j].name); zj; }
    return(FAIL);
  }
  
  /* increment vs->nusym only if no user field has been redefined */
  if (!replacesym) vs->nusym++;
  
  return(SUCCEED); 
  
} /* VSfdefine */

/* ------------------------------------------------------------------ */

