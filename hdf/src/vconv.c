#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

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

#define INT16SIZE 2
#define INT32SIZE 4

/* ------------------------------------------------------------------ */

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

	int16		foundold, foundnew;
	int32 	aid;	

	foundold = 0;
	foundnew = 0;
	/* locate any OLD vgs */
         aid = QQstartread (f, (uint16)OLD_VGDESCTAG,  DFREF_WILDCARD);
		 if (aid != FAIL) foundold++;
		 QQendaccess (aid);

	/* locate any OLD vdatas */
        aid = QQstartread(f, (uint16)OLD_VSDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundold++;
		QQendaccess (aid);


   /* locate any NEW vgs */
		aid = QQstartread(f, NEW_VGDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundnew++;
		QQendaccess (aid);

	/* locate any NEW vdatas */
		aid = QQstartread(f, NEW_VSDESCTAG,  DFREF_WILDCARD);
		if (aid != FAIL) foundnew++;
		QQendaccess (aid);


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

	VGROUP 	tempvgroup;
	VDATA		tempvdata;

	VGROUP	* vg = &tempvgroup;
	VDATA		*vs = &tempvdata;
	BYTE		buf[5000]; /* to store an old vdata or vgroup descriptor  */
	int32 	bsize, aid;
    int32   stat;
    uint16 u;
	uint16	tag, ref;

	/* =============================================  */
	/* --- read all vgs and convert each --- */

    stat = aid = QQstartread (f, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD);
	while (stat != FAIL) {
		QQQuerytagref (aid, &tag, &ref);
		QQQuerylength (aid, &bsize);
       stat = QQgetelement (f, (uint16)OLD_VGDESCTAG, ref, (uint8*)buf);
		if (stat == FAIL) {
			sprintf(sjs,"getvgroup_old. getelement err. \n"); zj;
			return (0);
			}

		oldunpackvg (vg, buf, &bsize);
		/* add new items */
			vg->vgclass[0] = '\0';
			vg->extag = 0;
			vg->exref = 0;
			vg->version = 2; /* version 2 */
			vg->more = 0;
		/* inside each vgroup, change the old tags to new */
            for(u=0;u<vg->nvelt;u++)
                if (vg->tag[u] == OLD_VGDESCTAG) vg->tag[u] = NEW_VGDESCTAG;
                else if (vg->tag[u] == OLD_VSDESCTAG) vg->tag[u] = NEW_VSDESCTAG;
				else { sprintf(sjs,"vimakecompat: unknown tag %d] in vgroup!\n",
                                vg->tag[u]); zj;
						}
		vpackvg (vg, buf, &bsize);

     	stat = QQputelement (f, VGDESCTAG, ref, (uint8*)buf, bsize);
		if (stat == FAIL) {
			sprintf(sjs,"put vgroup desc error.\n"); zj;
			return (0);
			}

        stat = QQnextread (aid, (uint16)OLD_VGDESCTAG, DFREF_WILDCARD, DF_CURRENT);
		} /* while */
	QQendaccess (aid);

	/* =============================================  */
	/* --- read all vdata descs  and convert each --- */
	/* --- then dup a tag for each vdata data elt --- */

    stat = aid = QQstartread (f, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD);
	while (stat != FAIL) {

		QQQuerytagref (aid, &tag, &ref);
		QQQuerylength (aid, &bsize);
		stat = QQgetelement (f, tag, ref, (uint8*)buf);
		if (stat == FAIL) {
			sprintf(sjs,"getvdata_old. getelement err. \n"); zj;
			return (0);
			}

		oldunpackvs (vs, buf, &bsize);

		/* add new items */
			vs->vsclass[0] = '\0';
			vs->extag = 0;
			vs->exref = 0;
			vs->version = 2; /* version 2 */
			vs->more = 0;
		vpackvs (vs, buf, &bsize);

      stat = QQputelement (f, VSDESCTAG, ref, (uint8*)buf, bsize);
		if (stat == FAIL) {
			sprintf(sjs ,"put vdata desc error.\n"); zj;
			return (0);
			}

		/* duplicate a tag to point to vdata data */
            stat = QQdupdd (f, NEW_VSDATATAG, ref, (uint16)OLD_VSDATATAG, ref);
			 if (stat == FAIL) {
				 	sprintf(sjs,"Hdupdd - cannot duplicate vdata.\n"); zj;
					return (0);
					}
        stat = QQnextread (aid, (uint16)OLD_VSDESCTAG, DFREF_WILDCARD, DF_CURRENT);
		} /* while */

	QQendaccess (aid);

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

	f = QQopen (fs,DFACC_ALL,0);
	if (f == FAIL) {
		sprintf(sjs,"vcheckcompat: cannot open %s\n",fs); zj;
		return (-1);
		}	 	
	stat = vicheckcompat(f); 
	QQclose (f);	

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

	f = QQopen (fs,DFACC_ALL,0);
   if (f == FAIL) { 
		sprintf(sjs,"vmakecompat: cannot open %s\n",fs); zj;
		return (0);
		}	 	
	stat = vimakecompat(f);
	QQclose (f);
	return (stat);

} /* vmakecompat */

/* ==================================================================== */

#ifdef PROTOTYPE
void oldunpackvg (VGROUP *vg,BYTE  buf[], int32 *size)   
#else

void oldunpackvg (vg, buf, size)   

	VGROUP*    	vg;		/* vgroup to be loaded with file data */
	BYTE			buf[]; 	/* must contain a VGDESCTAG data object from file */
	int32*     	size;  	/* ignored, but included to look like packvg() */

#endif

{

	BYTE			*b, *bb;
    uint32       i;
	char * FUNC = "oldunpackvg";

    *size = *size; /* dummy, so that compiler thinks it is used  */

	bb = &buf[0];

	/* retrieve nvelt */
	b = bb;
	INT16DECODE(b,vg->nvelt);
	bb+=INT16SIZE;

	/* retrieve the tags */
	for (i=0;i<vg->nvelt;i++) {
		b= bb;
		INT16DECODE(b,vg->tag[i]);
		bb +=INT16SIZE;
	}

	/* retrieve the refs */
	for (i=0;i<vg->nvelt;i++) {
		b= bb;
		INT16DECODE(b,vg->ref[i]);
		bb +=INT16SIZE;
	}

	/* retrieve vgname */
	strcpy(vg->vgname, (char*) bb);
    bb += ( DFIstrlen(vg->vgname)+1 );

	if (vjv) {
		sprintf(sjs,"unpackvg: vgname is [%s]\n",vg->vgname);
		zj;
	}

} /* unpackvg */

/* ================================================================= */

#ifdef PROTOTYPE
void oldunpackvs (VDATA *vs, BYTE buf[], int32 *size)       
#else
void oldunpackvs (vs, buf, size)       

	VDATA 	*vs;
	int32   	*size;	/* UNUSED, but retained for compatibility with packvs */
	BYTE		buf[];

#endif

{
	BYTE		*b, *bb;
	int16    i;
	char * FUNC = "oldunpackvs";

    *size = *size; /* dummy */

	bb = &buf[0];

	b = bb;
	INT16DECODE(b,vs->interlace);
	bb += INT16SIZE;

	b = bb;
	INT32DECODE(b,vs->nvertices);
	bb += INT32SIZE;

	b = bb;
	INT16DECODE(b,vs->wlist.ivsize);
	bb += INT16SIZE;

	b = bb;
	INT16DECODE(b,vs->wlist.n);
	bb += INT16SIZE;

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the type */
		b = bb;
		INT16DECODE(b,vs->wlist.type[i]);
		bb += INT16SIZE;
	}

	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the isize */
		b = bb;
		INT16DECODE(b,vs->wlist.isize[i]);
		bb += INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the off */
		b = bb;
		INT16DECODE(b,vs->wlist.off[i]);
		bb += INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++)  { /* retrieve the order */
		b = bb;
		INT16DECODE(b,vs->wlist.order[i]);
		bb += INT16SIZE;
	}
	for (i=0;i<vs->wlist.n;i++) {
		strcpy(vs->wlist.name[i],  (char*) bb);
        bb += ( DFIstrlen(vs->wlist.name[i]) + 1 );
	}

	strcpy(vs->vsname, (char*) bb);
    bb += ( DFIstrlen(vs->vsname) + 1);

	/* **EXTRA**  fill in the machine-dependent size fields */
	for (i=0;i<vs->wlist.n;i++) {
		vs->wlist.esize[i] = vs->wlist.order[i] * SIZEOF(vs->wlist.type[i]);
	}

} /* unpackvs */

/* ------------------------------------------------------------------ */
