#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/05/29 19:13:01  mfolk
Added int32 casts to make a couple of routines work with Convex.

 * Revision 1.2  1992/05/18  22:11:07  sxu
 * modified constants for number types
 *
 * Revision 1.1  1992/03/01  22:29:07  dilg
 * Initial revision
 *
 * Revision 1.1  1992/02/29  20:19:13  likkai
 * Initial revision
 *
*/
/*
	===============================
	SAMPLE VSET PROGRAM
	Jason Ng NCSA FEB-28-91
	===============================

	This program demonstrates the use of the high-level VSET write routines.
	It shows how data can be stored in vdatas, and how vgroups can be created
	from vdatas, vgroups and other HDF elements.

	This example creates a file "egchi.hdf" that may be viewed using the
	vset utility "vshow".

	DETAILS

	This example shows how pressure data  and color data can be stored as a 
	vset.  To store pressure data, a field named "PRES", is defined of 
	float type. The routine VHstoredata() stores the pressure values in a vdata.

	Color data comprises 3 components (red, green, blue). These can be stored as
	3 different fields in 3 vdatas. But this example shows that they can be
	treated as ONE field, and stored together as a compound field called "COLOR".
	The number of components of a field is called its order.  In this case, 
	"COLOR" has order=3. (whereas "PRES" above has order=1).

	Note that VHstoredatam() must be used to store values of a compound field.
	This is similar to VHstoredata() but has an extra argument for spefcifying
	the order.

	Finally a vgroup is created, and the ids of the created vdatas are stored
	in the vgroup. This effectively links the vdatas together. This example
	also shows that you can insert another vgroup, as well as a non-vset element
	(in this case, some element with tag=7777 and ref=1) into a vgroup.
*/

#include "vg.h"

#define FS  "egchi.hdf"
#define NP 100
#define NC 60
#define ORDERC 3 /* 3 color components: rgb */

main (ac,av)   int ac; 
char**av; 
{
	int n = ac;

	printf("%s: demonstrates and tests high-level VSET routines\n", av[0]);
	printf("It also shows how you can link other HDF elements\n");
	printf("(eg rasters, palettes) into vgroups\n");
	printf("\n");
	printf("It creates 2 vdatas, a vgroup, and a non-vset element\n");
	printf("then link them all into one vgroup\n");

	printf("Vdata 1 contains an order-1 float field PRES\n");
	printf("Vdata 2 contains an order-3 integer field COLOR\n\n");
	printf("The non-vset element has tag=7777, ref=1\n");

	doit();

	printf("\nsuccess: created file %s\n", FS);
	printf(" Uset the utility vshow to view this file: \n");
	printf("\t\t vshow %s +\n", FS);
	printf("\t\t\t or\n");
	printf("\t\t vshow %s \n", FS);
	printf("results should be as in %s.result\n",av[0]);
}

/* ------------------------------------------------------------------ */

doit() {
	HFILEID f;
	float pvals[NP];
	int cvals[NC][ORDERC];
	int i,j;

	int pid, cid; 		/* refs of vdatas */
	int eid; 			/* empty vgroup's ref  */
	int gid; 			/* vgroup's ref  */
	int32 tags[10], refs[10];
	char * CLASS = "EXAMPLE";

	/* --- generate data here --- */
	for(i=0;i<NP;i++)  pvals[i] = 100.0  + i * 0.001;
	for(i=0;i<NC;i++) for(j=0;j<3;j++)  cvals[i][j] =   i + j*100;

	/* ---- open a new file --- */
	if (FAIL==(f=Hopen(FS ,DFACC_ALL,0))) 
	{ printf("open %s err\n",FS); exit(0); }

	/* ---- create 2 vdatas --- */
	pid = VHstoredata (f, "PRES", (unsigned char*) pvals ,NP, DFNT_FLOAT32, "pressure vals",CLASS);
	if (pid  == -1) { printf(" VHstoredata store PRES err. "); }

	cid  = VHstoredatam(f, "COLOR",(unsigned char*) cvals, NC, DFNT_INT32, "rgb colors", CLASS, 3);
	if (cid  == -1) { printf(" VHstoredata store COLOR err. "); }

	eid  = VHmakegroup (f, tags, refs, (int32) 0, 
                                       "This is an EMPTY vgroup", CLASS);
	if (eid  == -1) { printf(" VHmakegroup err\n"); }

	/* --- create a new vgroup to store the 2 vdatas , the empty vgroup 
	and the non-vset element -- */

		tags[0] = VSDESCTAG;  refs[0] = pid;
		tags[1] = VSDESCTAG;  refs[1] = cid;
		tags[2] = VGDESCTAG;  refs[2] = eid;
		tags[3] = 7777 ;      refs[3] = 1;

	gid  = VHmakegroup (f, tags, refs, (int32) 4, 
                                     "here is a vset with 4 links", CLASS);

	if (eid  == -1) { printf(" VHmakegroup err\n"); }

	/* --- close the file --- */
	Hclose (f);
}
