/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Tests for vgroup's and vdata's name and class name.
 */
#include "hdf.h"
#include "hfile.h"
#include "tproto.h"

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
    int32 status;       /* Status values from routines */
    int32 file_id;          /* File ID */
    int32 vg1;          /* Vdata ID */
    int32 ref;          /* Vdata ref */
    uint16 name_len;	/* Length of a vgroup's name or class name */
    char *vgname, *vgclass;

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

    /* get the vgroup's name */
    status = Vgetnamelen(vg1, &name_len);
    CHECK_VOID(status,FAIL,"Vgetnamelen");

    vgname = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(vgname, "vgname", "test_vglongnames" );

    status=Vgetname(vg1, vgname);
    CHECK_VOID(status,FAIL,"VSgetname");

    if (HDstrcmp(vgname, VG_LONGNAME)) {
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

/* main test driver */
void
test_vnameclass(void)
{
    int32       status;

    /* test Vgroups with name and class that have more than 64 characters */
    test_vglongnames();

    /* test Vgetname and Vgetclass when either name or class is not defined. */
    test_undefined();
}   /* test_vnameclass */
