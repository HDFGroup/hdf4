#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.10  1993/08/28 22:58:47  georgev
Fixed cast problem for HDgetspace.

 * Revision 1.9  1993/08/28  22:58:07  georgev
 * Fixed a few VOIDP casts.
 *
 * Revision 1.8  1993/08/20  22:38:37  koziol
 * Reduced the static memory of a couple of functions to make the PC happier...
 *
 * Revision 1.7  1993/08/19  16:45:45  chouck
 * Added code and tests for multi-order Vdatas
 *
 * Revision 1.6  1993/08/16  21:46:32  koziol
 * Wrapped in changes for final, working version on the PC.
 *
 * Revision 1.5  1993/04/19  22:48:23  koziol
 * General Code Cleanup to reduce/remove errors on the PC
 *
 * Revision 1.4  1993/03/29  16:50:25  koziol
 * Updated JPEG code to new JPEG 4 code.
 * Changed VSets to use Threaded-Balanced-Binary Tree for internal
 * 	(in memory) representation.
 * Changed VGROUP * and VDATA * returns/parameters for all VSet functions
 * 	to use 32-bit integer keys instead of pointers.
 * Backed out speedups for Cray, until I get the time to fix them.
 * Fixed a bunch of bugs in the little-endian support in DFSD.
 *
 * Revision 1.3  1993/01/19  05:56:10  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.2  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
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

static void oldunpackvg PROTO((VGROUP *vg,uint8  buf[], int32 *size));
static void oldunpackvs PROTO((VDATA *vs, uint8 buf[], int32 *size));

/*
*  this routine checks that the given OPENED file is compatible with
*	version 2.0 or later of the HDF Vset library .
*  RETURNS 1  if file already compatible with r2.
*          0  if not compatible.
*          -1 if error.
*/
#ifdef PROTOTYPE
int32 vicheckcompat(HFILEID f)
#else
int32 vicheckcompat(f)
HFILEID f;
#endif
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


#ifdef PROTOTYPE
int32 vimakecompat(HFILEID f)
#else
int32 vimakecompat(f)
HFILEID f;
#endif
{
	VGROUP	* vg;
	VDATA	*vs;
    uint8   *buf=NULL; /* to store an old vdata or vgroup descriptor  */
	int32 	old_bsize=0,bsize,
	        aid;
    int32   stat;
    uint16  u;
	uint16	tag, ref;
    char * FUNC = "vimakecompat";

	/* =============================================  */
	/* --- read all vgs and convert each --- */

	vg = (VGROUP *)HDgetspace(sizeof(VGROUP));/*allocate space for the VGroup */
    stat = aid = Hstartread (f, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD);
	while (stat != FAIL) {
        HQuerytagref (aid, &tag, &ref);
        HQuerylength (aid, &bsize);
        if(buf==NULL || bsize>old_bsize) {
            if(buf!=NULL)
                HDfreespace((VOIDP)buf);
            if ( (buf= (uint8 *) HDgetspace (bsize)) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            old_bsize=bsize;
          } /* end if */
       stat = Hgetelement (f, (uint16)OLD_VGDESCTAG, ref, (uint8*)buf);
        if (stat == FAIL) {
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

        stat = Hputelement (f, VGDESCTAG, ref, (uint8*)buf, bsize);
        HDfreespace((VOIDP)buf);
        if (stat == FAIL)
            HRETURN_ERROR(DFE_WRITEERROR,0);

        stat = Hnextread (aid, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD, DF_CURRENT);
	  } /* while */
    Hendaccess (aid);
    HDfreespace((VOIDP)vg);

	/* =============================================  */
	/* --- read all vdata descs  and convert each --- */
	/* --- then dup a tag for each vdata data elt --- */

    old_bsize=0;    /* reset state variables */
    buf=NULL;
	vs = (VDATA *)HDgetspace(sizeof(VDATA));  /* allocate space for the VData */
    stat = aid = Hstartread (f, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD);
	while (stat != FAIL) {

        HQuerytagref (aid, &tag, &ref);
        HQuerylength (aid, &bsize);
        if(buf==NULL || bsize>old_bsize) {
            if(buf!=NULL)
                HDfreespace((VOIDP)buf);
            if ( (buf= (uint8 *) HDgetspace (bsize)) == NULL)
                HRETURN_ERROR(DFE_NOSPACE, FAIL);
            old_bsize=bsize;
          } /* end if */
        stat = Hgetelement (f, tag, ref, (uint8*)buf);
        if (stat == FAIL) {
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

        stat = Hputelement (f, VSDESCTAG, ref, (uint8*)buf, bsize);
        if (stat == FAIL) {
            HDfreespace((VOIDP)buf);
            HRETURN_ERROR(DFE_WRITEERROR,0);
          } /* end if */

		/* duplicate a tag to point to vdata data */
        stat = Hdupdd (f, NEW_VSDATATAG, ref, (uint16)OLD_VSDATATAG, ref);
        HDfreespace((VOIDP)buf);
         if (stat == FAIL)
            HRETURN_ERROR(DFE_DUPDD,0);
        stat = Hnextread (aid, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
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

#ifdef PROTOTYPE
int32 vcheckcompat(char *fs)
#else
int32 vcheckcompat(fs)
char * fs;
#endif
{

	HFILEID	f;
	int32 	stat;
    char * FUNC = "vcheckcompat";

    f = Hopen (fs,DFACC_ALL,0);
    if (f == FAIL)
        HRETURN_ERROR(DFE_BADOPEN,FAIL);
	stat = vicheckcompat(f);
    Hclose (f);

	return (stat);
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

#ifdef PROTOTYPE
int32 vmakecompat(char * fs) 
#else
int32 vmakecompat(fs)
char * fs;
#endif
{
	HFILEID 	f;
	int32 	stat;
    char * FUNC = "vmakecompat";

    f = Hopen (fs,DFACC_ALL,0);
    if (f == FAIL)
        HRETURN_ERROR(DFE_BADOPEN,FAIL);
	stat = vimakecompat(f);
    Hclose (f);
	return (stat);
} /* vmakecompat */

/* ==================================================================== */

#ifdef PROTOTYPE
static void oldunpackvg (VGROUP *vg,uint8  buf[], int32 *size)
#else
static void oldunpackvg (vg, buf, size)
VGROUP*     vg;     /* vgroup to be loaded with file data */
uint8            buf[];  /* must contain a VGDESCTAG data object from file */
int32*      size;   /* ignored, but included to look like packvg() */
#endif
{
    uint8       *bb;
    uint32       i;
	char * FUNC = "oldunpackvg";

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

#ifdef PROTOTYPE
static void oldunpackvs (VDATA *vs, uint8 buf[], int32 *size)
#else
static void oldunpackvs (vs, buf, size)
VDATA   *vs;
int32       *size;  /* UNUSED, but retained for compatibility with packvs */
uint8        buf[];
#endif
{
    uint8   *bb;
	int16    i;
	char * FUNC = "oldunpackvs";

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
        vs->wlist.esize[i]=(int16)vs->wlist.order[i] * (SIZEOF((int16)vs->wlist.type[i]));

} /* oldunpackvs */

/* ------------------------------------------------------------------ */
