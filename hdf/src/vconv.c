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

/* obsolete code for HDF 3.2. 26/march/92 jason ng */
/* except for the following routines: 
*	vicheckcompat()
* 	movebytes ()
* 	oldunpackvg ()
* 	oldunpackvs ()
*/


/*
*
* vconv.c
* Part of the HDF Vset interface.
*/

#include "vg.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*                                                                    */
/* routines for converting from vsets in v1.0 to v2.x                 */
/*                                                                    */
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* ------------------------------------------------------------------ */

PRIVATE void oldunpackvg (VGROUP *vg,uint8  buf[], int32 *size);
PRIVATE void oldunpackvs (VDATA *vs, uint8 buf[], int32 *size);

/*
*  this routine checks that the given OPENED file is compatible with
*	version 2.0 or later of the HDF Vset library .
*  RETURNS 1  if file already compatible with r2.
*          0  if not compatible.
*          -1 if error.
*/
int32 vicheckcompat(HFILEID f)
{
    int16   foundold, foundnew;
	int32 	aid;

	foundold = 0;
	foundnew = 0;
	/* locate any OLD vgs */
         aid = Hstartread (f, (uint16)OLD_VGDESCTAG,  DFREF_WILDCARD);
		 if (aid != FAIL) foundold++;
         Hendaccess (aid);

	/* locate any OLD vdatas */
        aid = Hstartread(f, (uint16)OLD_VSDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundold++;
        Hendaccess (aid);


   /* locate any NEW vgs */
        aid = Hstartread(f, NEW_VGDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundnew++;
        Hendaccess (aid);

	/* locate any NEW vdatas */
        aid = Hstartread(f, NEW_VSDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundnew++;
        Hendaccess (aid);


	if ( foundold == 0 ) /* has no old vset elements */
		return (1); /* just assume compatible */

	if ( foundnew > 0 ) 
		return (1); /* file is already compatible */
	else
		return (0); /* file is not compatible */
} /* vicheckcompat */

/* ------------------------------------------------------------------ */
/*
* This routine will modify a given OPENED file so that is becomes compatible
* with version 2.0 or later of the HDF Vset library.
* Note that the file is assumed to be not compatible to begin with.
* This routine will not check to see if the file is already compatible,
* but it is harmless to run an already-compatible file through again.
*
* However, be aware that each time, the file gets larger.
* Also, file must be opened with DFACC_ALL access.
*
* returns  1 if successful. if error, returns 0
*/


int32 vimakecompat(HFILEID f)
{
	VGROUP	* vg;
	VDATA	*vs;
    uint8   *buf=NULL; /* to store an old vdata or vgroup descriptor  */
	int32 	old_bsize=0,bsize;
	int32 	aid;
    int32   ret;
    uint16  u;
	uint16	tag, ref;
    CONSTR(FUNC,"vimakecompat");

	/* =============================================  */
	/* --- read all vgs and convert each --- */

	vg = (VGROUP *)HDgetspace(sizeof(VGROUP));/*allocate space for the VGroup */
    ret = aid = Hstartread (f, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD);
	while (ret != FAIL) {
        HQuerytagref (aid, &tag, &ref);
        HQuerylength (aid, &bsize);
        if(buf==NULL || bsize>old_bsize) {
            if(buf!=NULL)
                HDfreespace((VOIDP)buf);
            if ( (buf= (uint8 *) HDgetspace (bsize)) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            old_bsize=bsize;
          } /* end if */
       ret = Hgetelement (f, (uint16)OLD_VGDESCTAG, ref, (uint8*)buf);
        if (ret == FAIL) {
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_READERROR,0);
          } /* end if */

		oldunpackvg (vg, buf, &bsize);
		/* add new items */
			vg->vgclass[0] = '\0';
			vg->extag = 0;
			vg->exref = 0;
			vg->version = 2; /* version 2 */
			vg->more = 0;
		/* inside each vgroup, change the old tags to new */
            for(u=0;u<vg->nvelt;u++)
                if (vg->tag[u] == OLD_VGDESCTAG)
                    vg->tag[u] = NEW_VGDESCTAG;
                else if (vg->tag[u] == OLD_VSDESCTAG)
                    vg->tag[u] = NEW_VSDESCTAG;
                else   /* BAD */
                    HERROR(DFE_NOTINSET);
		vpackvg (vg, buf, &bsize);

        ret = Hputelement (f, VGDESCTAG, ref, (uint8*)buf, bsize);
        HDfreespace((VOIDP)buf);
        if (ret == FAIL)
            HRETURN_ERROR(DFE_WRITEERROR,0);

        ret = Hnextread (aid, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD, DF_CURRENT);
	  } /* while */
    Hendaccess (aid);
    HDfreespace((VOIDP)vg);

	/* =============================================  */
	/* --- read all vdata descs  and convert each --- */
	/* --- then dup a tag for each vdata data elt --- */

    old_bsize=0;    /* reset state variables */
    buf=NULL;
	vs = (VDATA *)HDgetspace(sizeof(VDATA));  /* allocate space for the VData */
    ret = aid = Hstartread (f, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD);
	while (ret != FAIL) {

        HQuerytagref (aid, &tag, &ref);
        HQuerylength (aid, &bsize);
        if(buf==NULL || bsize>old_bsize) {
            if(buf!=NULL)
                HDfreespace((VOIDP)buf);
            if ( (buf= (uint8 *) HDgetspace (bsize)) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            old_bsize=bsize;
          } /* end if */
        ret = Hgetelement (f, tag, ref, (uint8*)buf);
        if (ret == FAIL) {
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_READERROR,0);
          } /* end if */

		oldunpackvs (vs, buf, &bsize);

		/* add new items */
			vs->vsclass[0] = '\0';
			vs->extag = 0;
			vs->exref = 0;
			vs->version = 2; /* version 2 */
			vs->more = 0;
		vpackvs (vs, buf, &bsize);

        ret = Hputelement (f, VSDESCTAG, ref, (uint8*)buf, bsize);
        if (ret == FAIL) {
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_WRITEERROR,0);
          } /* end if */

		/* duplicate a tag to point to vdata data */
        ret = Hdupdd (f, NEW_VSDATATAG, ref, (uint16)OLD_VSDATATAG, ref);
        HDfreespace((VOIDP)buf);
         if (ret == FAIL)
            HRETURN_ERROR(DFE_DUPDD,0);
        ret = Hnextread (aid, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
	  } /* while */

    Hendaccess (aid);
    HDfreespace((VOIDP)vg);

	return(1);

} /* vimakecompat */


/* ================================================================== */
/*
*  this routine checks that the given file is compatible with
*	version 2.0 or later of the HDF Vset library .
*
*  All it does is to open the file, call vicheckcompat to do all the
*  checking, and then closes it.
*	See comments for vicheckcompat().

*  returns 1 if file already compatible with r2.
*          0 if not compatible.
*          -1 if error.
*/

int32 vcheckcompat(char *fs)
{

	HFILEID	f;
	int32 	ret;
    CONSTR(FUNC,"vcheckcompat");

    f = Hopen (fs,DFACC_ALL,0);
    if (f == FAIL)
        HRETURN_ERROR(DFE_BADOPEN,FAIL);
	ret = vicheckcompat(f);
    Hclose (f);

	return (ret);
} /* vcheckcompat */

/* ================================================================== */
/*
* This routine will modify a given file so that is becomes compatible
* with version 2.0 or later of the HDF Vset library.
*
* All this routine does is to open the file, call vimakecompat to
* do all the conversion, and then to close the file.
* See comments for vimakecompat().
*
* returns  1 if successful. if error, returns 0
*/

int32 vmakecompat(char * fs) 
{
	HFILEID 	f;
	int32 	ret;
    CONSTR(FUNC,"vmakecompat");

    f = Hopen (fs,DFACC_ALL,0);
    if (f == FAIL)
        HRETURN_ERROR(DFE_BADOPEN,FAIL);
	ret = vimakecompat(f);
    Hclose (f);
	return (ret);
} /* vmakecompat */

/* ==================================================================== */

static void oldunpackvg (VGROUP *vg,uint8  buf[], int32 *size)
{
    uint8       *bb;
    uint32       i;
	CONSTR(FUNC,"oldunpackvg");

    *size = *size; /* dummy, so that compiler thinks it is used  */

	bb = &buf[0];

	/* retrieve nvelt */
    INT16DECODE(bb,vg->nvelt);

	/* retrieve the tags */
    for (i=0;i<vg->nvelt;i++)
        INT16DECODE(bb,vg->tag[i]);

	/* retrieve the refs */
    for (i=0;i<vg->nvelt;i++)
        INT16DECODE(bb,vg->ref[i]);

	/* retrieve vgname */
    HDstrcpy(vg->vgname, (char*) bb);
} /* oldunpackvg */

/* ================================================================= */

static void oldunpackvs (VDATA *vs, uint8 buf[], int32 *size)
{
    uint8   *bb;
	int16    i;
	CONSTR(FUNC,"oldunpackvs");

    *size = *size; /* dummy */

	bb = &buf[0];

    INT16DECODE(bb,vs->interlace);

    INT32DECODE(bb,vs->nvertices);

    INT16DECODE(bb,vs->wlist.ivsize);

    INT16DECODE(bb,vs->wlist.n);

    for (i=0;i<vs->wlist.n;i++)     /* retrieve the type */
        INT16DECODE(bb,vs->wlist.type[i]);

    for (i=0;i<vs->wlist.n;i++)    /* retrieve the isize */
        INT16DECODE(bb,vs->wlist.isize[i]);

    for (i=0;i<vs->wlist.n;i++)    /* retrieve the off */
        INT16DECODE(bb,vs->wlist.off[i]);

    for (i=0;i<vs->wlist.n;i++)    /* retrieve the order */
        INT16DECODE(bb,vs->wlist.order[i]);
    
    for (i=0;i<vs->wlist.n;i++) {
        HDstrcpy(vs->wlist.name[i],(char*)bb);
        bb += ( HDstrlen(vs->wlist.name[i]) + 1 );
    }
    
    HDstrcpy(vs->vsname, (char*) bb);
    bb += ( HDstrlen(vs->vsname) + 1);

    /* **EXTRA**  fill in the machine-dependent size fields */
    for (i=0;i<vs->wlist.n;i++)
        vs->wlist.esize[i]=(int16)(vs->wlist.order[i] * VSIZEOF((int16)vs->wlist.type[i]));

} /* oldunpackvs */

/* ------------------------------------------------------------------ */
