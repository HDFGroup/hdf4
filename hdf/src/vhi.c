#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/03/29 16:50:41  koziol
Updated JPEG code to new JPEG 4 code.
Changed VSets to use Threaded-Balanced-Binary Tree for internal
	(in memory) representation.
Changed VGROUP * and VDATA * returns/parameters for all VSet functions
	to use 32-bit integer keys instead of pointers.
Backed out speedups for Cray, until I get the time to fix them.
Fixed a bunch of bugs in the little-endian support in DFSD.

 * Revision 1.2  1993/01/19  05:56:24  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/* 
* ============================================
* VHxxx  routines:
* HDF Vset high-level access routines
* 28-MAR-91 Jason Ng NCSA
* Likkai Ng NCSA Feb 92 - update to use H-routines

* ============================================
*/

#include "vg.h"

/* ------------------------ VHstoredata ------------------------------- */
/*
Stores 'n' elements of data from 'buf' as a field 'field' in a new vdata 
called 'vsname' into the already opened HDF file (with pointer 'f').

The datatype variable must be specified as a valid HDF type

NOTES:
n should not be zero or negative.
RETURN:
      -1 if error.
      ref of that new vdata (a +ve integer) if successful.
*/

#ifdef PROTOTYPE
PUBLIC int32 VHstoredata (HFILEID f, char *field, uint8 buf[], int32 n, int32 datatype, char *vsname, char *vsclass)
#else

PUBLIC int32 VHstoredata (f, field, buf, n, datatype, vsname, vsclass)
	HFILEID 	f;
	char * 	field;
    uint8        buf[];
	int32 	n;
	char * 	vsname, * vsclass;
	int32 	datatype;
#endif

{
  int32	ref;
  int32 order = 1;
  char * FUNC = "VHstoredata";
  
  ref = (int32) VHstoredatam (f, field, buf, n, datatype, vsname, vsclass, order);
  return (ref);
}

/* ----------------------- VHstoredatam ---------------------------- */
/*
Same as VHstoredata but allows aggregate-typed field.
Stores 'n' elements of data from 'buf' as a
field 'field' in a new vdata called 'vsname' into the
already opened HDF file (with pointer 'f').

The datatype variable must be specified as a valid HDF type

NOTES:
n should not be zero or negative.
RETURN:
      -1 if error.
      ref of that new vdata (a +ve integer) if successful.
*/
#ifdef PROTOTYPE
PUBLIC int32 VHstoredatam (HFILEID f, char *field, uint8 buf[], int32  n, int32 datatype, char *vsname, char *vsclass, int32 order)
#else
PUBLIC int32 VHstoredatam (f, field, buf, n, datatype, vsname, vsclass, order)
HFILEID     f;
char    *   field;
uint8        buf[];
int32   n;
int32   datatype;
int32   order;
char *  vsname, * vsclass;
#endif
{
	int32 s;
	int32 ref;
#ifdef OLD_WAY
	VDATA * vs;
#else
    int32 vskey;
#endif
	char * FUNC = "VHstoredatam";

#ifdef OLD_WAY
    vs = (VDATA*) VSattach (f,-1,"w");
    if (vs==NULL)
        return (FAIL);
    s = VSfdefine (vs, field, datatype, order);
    if (s == -1)
        return (FAIL);
    s = VSsetfields (vs,field);
    if (s == -1)
        return (FAIL);
    s = VSwrite (vs, buf, n, FULL_INTERLACE);
    if (n != s)
        return (FAIL);
	VSsetname (vs, vsname);
	VSsetclass(vs, vsclass);
	ref = VSQueryref(vs);
	VSdetach(vs);
#else
    vskey = VSattach (f,-1,"w");
    if (vskey==FAIL)
        return (FAIL);
    s = VSfdefine (vskey, field, datatype, order);
    if (s == -1)
        return (FAIL);
    s = VSsetfields (vskey,field);
    if (s == -1)
        return (FAIL);
    s = VSwrite (vskey, buf, n, FULL_INTERLACE);
    if (n != s)
        return (FAIL);
    VSsetname (vskey, vsname);
    VSsetclass(vskey, vsclass);
    VSgetoref(vskey,&ref);
    VSdetach(vskey);
#endif

	return ((int32) ref);
} /* VHstoredatam */

/* ------------------------ VHmakegroup ------------------------------- */
/* 
Takes an array of tags and and array of refs and create a vgroup to
store them. You tell it how many tag/ref pairs there are. You must 
also give the vgroup a name.
NOTE:
Does bot check if a tag/ref is valid or exist, but ALL tag/ref 
pairs MUST be unique.
Creating EMPTY vgroups is allowed.

RETURN: 
      -1 if error
      ref of the new vgroup (a +ve integre) if ok.
*/
#ifdef PROTOTYPE
PUBLIC int32 VHmakegroup (HFILEID f, int32 tagarray[], int32 refarray[] , int32 n, char *vgname, char *vgclass)
#else
PUBLIC int32 VHmakegroup (f, tagarray, refarray , n, vgname, vgclass)
HFILEID     f;
int32   tagarray[], refarray[];
int32   n;
char        * vgname, * vgclass;
#endif
{
	int32 	ref, i, s;	
#ifdef OLD_WAY
	VGROUP *	vg;
#else
    int32 vgkey;
#endif
	char * FUNC = "VHmakegroup";

#ifdef OLD_WAY
    vg = (VGROUP*) Vattach (f, -1, "w");
    if (vg==NULL)
        return (FAIL);
	Vsetname (vg, vgname);
	Vsetclass(vg, vgclass);

	for (i=0; i<n; i++) {
		s = Vaddtagref (vg, tagarray[i], refarray[i]);
		if (s == -1) return (FAIL);
		}

	ref = VQueryref(vg);
	Vdetach (vg);
#else
    vgkey = Vattach (f, -1, "w");
    if (vgkey==FAIL)
        return (FAIL);
    Vsetname (vgkey, vgname);
    Vsetclass(vgkey, vgclass);

	for (i=0; i<n; i++) {
        s = Vaddtagref (vgkey, tagarray[i], refarray[i]);
        if (s == -1)
            return (FAIL);
    }

    Vgetoref(vgkey,&ref);
    Vdetach(vgkey);
#endif
	return (ref);
} /* VHmakegroup */

/* ------------------------------------------------------------------ */
