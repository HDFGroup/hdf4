/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Tests for vgroup's and vdata's name and class name.
 */
#include "hdf.h"
#include "hfile.h"
#include "tproto.h"
#include "tutils.h"

#define LONGNAMES "tlongnames.hdf"
#define NONAMECLASS "tundefined.hdf"
#define VGROUP1 "VGROUP1"
#define VG_LONGNAME "Vgroup with more than 64 characters in length, 74 characters to be exact!"
#define VG_LONGCLASS "Very long class name to classify all Vgroups with more than 64 characters in name"

static void test_vglongnames(void);
static void test_novgclass(void);

static void
test_vglongnames(void)
{
    int32 file_id;      /* File ID */
    int32 vg1;          /* Vdata ID */
    int32 ref;          /* Vdata ref */
    uint16 name_len;	/* Length of a vgroup's name or class name */
    char *vgname, *vgclass;
    intn is_internal;
    int32 status;       /* Status values from routines */

    /* Open the HDF file. */
    file_id = Hopen(LONGNAMES, DFACC_CREATE, 0);
    CHECK_VOID(file_id,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Create a new vgroup. */
    vg1 = Vattach(file_id, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetname(vg1, VG_LONGNAME);
    CHECK_VOID(status,FAIL,"VSsetname");

    status=Vsetclass(vg1, VG_LONGCLASS);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Create another vgroup of the same class. */
    vg1 = Vattach(file_id, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetname(vg1, VGROUP1);
    CHECK_VOID(status,FAIL,"VSsetname");

    status=Vsetclass(vg1, VG_LONGCLASS);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(file_id);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(file_id);
    CHECK_VOID(status,FAIL,"Hclose");

    /* Re-open the HDF file. */
    file_id = Hopen(LONGNAMES, DFACC_RDWR, 0);
    CHECK_VOID(file_id,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the long name vgroup. */
    ref=Vfind(file_id,VG_LONGNAME);
    CHECK_VOID(ref,FAIL,"VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* Test Vgisinternal */
    is_internal = Vgisinternal(vg1);
    CHECK_VOID(is_internal, FAIL, "Vgisinternal");
    VERIFY_VOID(is_internal, FALSE, "Vgisinternal");
    
    /* get the vgroup's name */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames" );

    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"VSgetname");
    VERIFY_CHAR_VOID(vgname, VG_LONGNAME, "Vgetname");

    if (vgname != NULL)
        HDfree(vgname);

    /* get the vgroup's class */
    status = Vgetclassnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgclass = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames" );

    status=Vgetclass(vg1, vgclass);
    CHECK_VOID(status,FAIL,"VSgetclass");
    VERIFY_CHAR_VOID(vgclass, VG_LONGCLASS, "Vgetclass");

    if (vgclass != NULL)
        HDfree(vgclass);

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Find the vgroup VGROUP1. */
    ref=Vfind(file_id,VGROUP1);
    CHECK_VOID(ref,FAIL,"VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* get the vgroup's name */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames" );

    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"VSgetname");

    if (HDstrcmp(vgname, VGROUP1)) {
        num_errs++;
        printf(">>> Got bogus Vgroup name : %s\n", vgname);
    }

    if (vgname != NULL)
        HDfree(vgname);

    /* get the vgroup's class */
    status = Vgetclassnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgclass = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgclass, "vgclass", "test_vglongnames" );

    status=Vgetclass(vg1, vgclass);
    CHECK_VOID(status,FAIL,"VSgetclass");

    if (HDstrcmp(vgclass, VG_LONGCLASS)) {
        num_errs++;
        printf(">>> Got bogus Vgroup class : %s\n", vgclass);
    }

    if (vgclass != NULL)
        HDfree(vgclass);

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(file_id);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(file_id);
    CHECK_VOID(status,FAIL,"Hclose");

} /* test_vglongnames() */

static void
test_undefined(void)
{
    int32 status;       /* Status values from routines */
    int32 file_id;          /* File ID */
    int32 vg1;          /* Vdata ID */
    int32 ref;          /* Vdata ref */
    intn is_internal;   /* to test Vgisinternal */
    uint16 name_len;	/* Length of a vgroup's name or class name */
    /* to simulate calls to Vgetclass/Vgetname in older applications */
    char vgname[VGNAMELENMAX+1], vgclass[VGNAMELENMAX+1];

    /* Open the HDF file. */
    file_id = Hopen(NONAMECLASS, DFACC_CREATE, 0);
    CHECK_VOID(file_id,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Create a vgroup which will have name set but not class */
    vg1 = Vattach(file_id, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetname(vg1, VG_LONGNAME);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Create another vgroup which will have class but not name */
    vg1 = Vattach(file_id, -1, "w");
    CHECK_VOID(vg1,FAIL,"VSattach");

    status=Vsetclass(vg1, VG_LONGCLASS);
    CHECK_VOID(status,FAIL,"VSsetname");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(file_id);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(file_id);
    CHECK_VOID(status,FAIL,"Hclose");

    /* Re-open the HDF file. */
    file_id = Hopen(NONAMECLASS, DFACC_RDWR, 0);
    CHECK_VOID(file_id,FAIL,"Hopen");

    /* Initialize HDF for subsequent vgroup/vdata access. */
    status = Vstart(file_id);
    CHECK_VOID(status,FAIL,"Vstart");

    /* Find the long name vgroup. */
    ref=Vfind(file_id,VG_LONGNAME);
    CHECK_VOID(ref,FAIL,"VSfind");

    vg1 = Vattach(file_id, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* Test Vgisinternal */
    is_internal = Vgisinternal(vg1);
    CHECK_VOID(is_internal, FAIL, "Vgisinternal");
    VERIFY_VOID(is_internal, FALSE, "Vgisinternal");
    
    /* Try getting the vgroup's class without calling first Vgetclassnamelen.
       This shows that bug HDFFR-1288 is fixed. */
    status=Vgetclass(vg1, vgclass);
    CHECK_VOID(status,FAIL,"Vgetclass");
    VERIFY_VOID(HDstrlen(vgclass), 0, "VSgetclass");

    /* The length of the class name should be 0 */
    status = Vgetclassnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetclassnamelen");
    VERIFY_VOID(name_len, 0, "VSgetclassnamelen");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    /* Find the vgroup with class VG_LONGCLASS.  This vgroup doesn't have a name */
    ref=Vfindclass(file_id, VG_LONGCLASS);
    CHECK_VOID(ref,FAIL,"VSfindclass");

    vg1 = Vattach(file_id, ref, "r");
    CHECK_VOID(vg1,FAIL,"VSattach");

    /* Try getting the vgroup's name without calling first Vgetclassnamelen.
       Similar to class name in bug HDFFR-1288. */
    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"Vgetname");
    VERIFY_VOID(HDstrlen(vgname), 0, "VSgetname");

    /* The length of the name should be 0 */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");
    VERIFY_VOID(name_len, 0, "VSgetnamelen");

    status = Vdetach(vg1);
    CHECK_VOID(status,FAIL,"Vdetach");
    
    status = Vend(file_id);
    CHECK_VOID(status,FAIL,"Vend");

    status = Hclose(file_id);
    CHECK_VOID(status,FAIL,"Hclose");

} /* test_undefined() */


/****************************************************************************
 * test_vgisinternal - tests the API function Vgisinternal
 *   - Use an existing GR file created during the period when GR vgroup had no
 *	class name, and had name set to GR_NAME
 *   - Get each vgroup, verify that it is internal or not
 * Jan 6, 2012 -BMR
****************************************************************************/

#define GR_FILE    "test_files/grtdfui83.hdf"
static void
test_vgisinternal()
{
    int32   fid, vgroup_id;
    intn    is_internal = FALSE;
    int32   vref = -1;
    intn    ii, status;
    char testfile[H4_MAX_NC_NAME] = "";
    char    internal_array2[2] = {TRUE, TRUE};
    intn    num_errs = 0;     /* number of errors so far */

    /* Use a GR file to test Vgisinternal on internal vgroups */

    /* The file GR_FILE is an existing file in the test_files directory,
       make_datafilename builds the file name with correct path */
    if (make_datafilename(GR_FILE, testfile, H4_MAX_NC_NAME) != FAIL)
    {
	/* Open the old GR file and initialize the V interface */
	fid = Hopen(testfile, DFACC_READ, 0);
	CHECK_VOID(fid, FAIL, "Hopen: grtdfui83.hdf");
	status = Vstart(fid);
	CHECK_VOID(status, FAIL, "Vstart");

	ii = 0;
	while ((vref = Vgetid(fid, vref)) != FAIL)
	{     /* until no more vgroups */
	    vgroup_id = Vattach(fid, vref, "r"); /* attach to vgroup */

	    /* Test that the current vgroup is or is not internal as specified
	       in the array internal_array2 */
	    is_internal = Vgisinternal(vgroup_id);
	    CHECK_VOID(is_internal, FAIL, "Vgisinternal");
  	    VERIFY_VOID(is_internal, internal_array2[ii], "Vgisinternal");

	    status = Vdetach(vgroup_id);
	    CHECK_VOID(status, FAIL, "Vdetach");

	    ii++; /* increment vgroup index */
	}

	/* Terminate access to the V interface and close the file */
	status = Vend(fid);
	CHECK_VOID(status, FAIL, "Vend");
	status = Hclose(fid);
	CHECK_VOID(status, FAIL, "Hclose");
    }
    else
    {
	fprintf(stderr, "ERROR>>> Unable to make filename for %s\n", GR_FILE);
	H4_FAILED()
    }
}   /* test_vgisinternal */

void
test_vnameclass(void)
{
    int32       status;

    /* test Vgroups with name and class that have more than 64 characters */
    test_vglongnames();

    /* test Vgetname and Vgetclass when either name or class is not defined. */
    test_undefined();

    /* test Vgisinternal when there is no class name */
    test_vgisinternal();
}   /* test_vnameclass */
