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

/* 
   * File
   *       vhi.c
   *       HDF Vset high-level access routines VHxxxx
   *       Feb 92 - update to use H-routines
   * Routines
   *       VHstoredata  -- stores data in a field of a vdata in an HDF file
   *       VHstoredatam -- stores data in a aggregated-typed field of a vdata
   *       VHmakegroup  -- makes a vgroup from tag/ref pairs
 */

#include "vg.h"

/* ------------------------ VHstoredata ------------------------------- 
   NAME
   VHstoredata -- stores data in a field of a new vdata
   USAGE
   int32 VHstoredata (f, field, buf, n, datatype, vsname, vsclass)
   HFILEID f;           IN: File id, returned from Hopen.
   char *  field;       IN: Name of the field.
   uint8   buf[];       IN: Buffer of data to be stored in the field.
   int32   n;           IN: Number of elements in buf to be written.
   inter   datatype;    IN: Numter type of the data to be written.
   char *  vsname;      IN: Name of the new vdata.
   char *  vsclass;     IN: Class of the new vdata.

   RETURNS
   On success returns reference number of the new vdata, a positive integer;
   on failure retruns -1.
   DESCRIPTION
   Stores 'n' elements of data from 'buf' as a field 'field' in a new vdata 
   called 'vsname' into the already opened HDF file (with file id 'f').
   The datatype variable must be specified as a valid HDF type; n should not 
   be zero or negative.
   Returns -1 if error; ref of that new vdata (a +ve integer) if successful.
   ------------------------------------------------------------------------- */

PUBLIC int32 
VHstoredata(HFILEID f, char *field, uint8 buf[], int32 n, int32 datatype,
	    char *vsname, char *vsclass)

{
    int32       ref;
    int32       order = 1;
#ifdef LATER
    CONSTR(FUNC, "VHstoredata");
#endif

    ref = (int32) VHstoredatam(f, field, buf, n, datatype, vsname, vsclass, order);
    return (ref);
}

/* ----------------------- VHstoredatam ---------------------------- 
   NAME
   VHstoredatam -- Same as VHstoredata but allows aggregate-typed field.
   USAGE
   int32 VHstoredata (f, field, buf, n, datatype, vsname, vsclass, order)
   HFILEID f;           IN: File id, returned from Hopen.
   char *  field;       IN: Name of the field.
   uint8   buf[];       IN: Buffer of data to be stored in the field. 
   int32   n;           IN: Number of elements in buf to be written.
   inter   datatype;    IN: Numter type of the data to be written.
   char *  vsname;      IN: Name of the new vdata.
   char *  vsclass;     IN: Class of the new vdata.
   int32   order;       IN: Order of the field. 

   RETURNS
   On success returns reference number of the new vdata, a positive integer;
   on failure retruns -1.
   DESCRIPTION
   Stores 'n' elements of data from 'buf' as a field 'field' in a new vdata
   called 'vsname' into the already opened HDF file (with file id 'f').
   The datatype variable must be specified as a valid HDF type;
   n should not be zero or negative.
   Returns -1 if error; ref of that new vdata (a +ve integer) if successful.
   --------------------------------------------------------------------------- */

PUBLIC int32 
VHstoredatam(HFILEID f, char *field, uint8 buf[], int32 n, int32 datatype, char *vsname, char *vsclass, int32 order)
{
    int32       s;
    int32       ref;
    int32       vs;
#ifdef LATER
    CONSTR(FUNC, "VHstoredatam");
#endif

    vs = VSattach(f, -1, "w");
    if (vs == FAIL)
	return (FAIL);

    s = VSfdefine(vs, field, datatype, order);
    if (s == -1)
	return (FAIL);

    s = VSsetfields(vs, field);
    if (s == -1)
	return (FAIL);

    s = VSwrite(vs, buf, n, FULL_INTERLACE);
    if (n != s)
	return (FAIL);

    VSsetname(vs, vsname);
    VSsetclass(vs, vsclass);
    ref = VSQueryref(vs);
    VSdetach(vs);

    return ((int32) ref);

}	/* VHstoredatam */

/* ------------------------ VHmakegroup ------------------------------- */
/* 
   NAME
   VHmakegroup -- Creates a vgroup to store pairs of tag/ref. 
   USAGE
   int32 VHmakegroup (f, tagarray, refarray , n, vgname, vgclass)
   HFILEID f;           IN: File id, returned from Hopen.
   int32   tagarray[];  IN: Array of tags to be stored in the vgroup.
   int32   refarray[];  IN: Array of refs to be stored in the vgroup.
   int32   n;           IN: Number of tags/refs in the tagarray/refarray.
   char    * vgname;    IN: Name of the new vgroup.
   char    * vgclass;   IN: Class of the new vgroup.

   RETURNS
   On success returns reference number of the new vgroup, a positive integer;
   on failure retruns -1.
   DESCRIPTION
   Takes an array of tags and and array of refs and create a vgroup to
   store them. You tell it how many tag/ref pairs there are. You must
   also give the vgroup a name.i Creating EMPTY vgroups is allowed.
   VHmakegroup does bot check if a tag/ref is valid or exist, 
   but ALL tag/ref pairs MUST be unique.

   Returns  -1 if error; ref of the new vgroup (a +ve integre) if ok.

   --------------------------------------------------------------------------- */

PUBLIC int32 
VHmakegroup(HFILEID f, int32 tagarray[], int32 refarray[], int32 n, char *vgname, char *vgclass)
{
    int32       ref, i, s;
    int32       vg;
#ifdef LATER
    CONSTR(FUNC, "VHmakegroup");
#endif

    vg = Vattach(f, -1, "w");
    if (vg == FAIL)
	return (FAIL);

    Vsetname(vg, vgname);
    Vsetclass(vg, vgclass);

    for (i = 0; i < n; i++)
      {
	  s = Vaddtagref(vg, tagarray[i], refarray[i]);
	  if (s == -1)
	      return (FAIL);
      }

    ref = VQueryref(vg);
    Vdetach(vg);

    return (ref);

}	/* VHmakegroup */

/* ------------------------------------------------------------------ */
