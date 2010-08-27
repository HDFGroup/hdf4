/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id: tattdatainfo.c 5334 2010-01-28 06:10:50Z bmribler $ */

#include "hdf.h"

#define DATAINFO_TESTER
#include "hdatainfo.h"
#include "tdatainfo.h"
#include "tutils.h"

/************ Draft - still need a little more of clean up ***********/

/***********************************************************************
  NOTE: At this time, many of the offsets in these tests are verified
	by hard-coding.  When adding new data to the files in exiting
	tests, please either add data at the bottom of the files or
	change the hard-coded offsets appropriately, using the values
	from the error messages.  However, you'll need to verify the
	correctness of these values first using the command 'od' on
	the hdf file.
*************************************************************************/

static void test_vvsattrs(void);
 /* static void test_getvdatas(void);
 */ 

/****************************************************************************
   Name: test_vvsattrs() - tests getting attribute data information from
			   vdatas and vgroups

   Description:
	This routine performs the following steps:
	- Create a new file with the V interface
	- Creates several vgroups and vdatas, and forms some arbitrary
	  structures among these elements.
	- Add some attributes to various vgroups and vdatas.
	- Retrieve space information of the data of some attributes.
	- Reopen the file without HDF4 library.
	- Read in attribute's data using the previously recorded
	  offsets/lengths and verify the data
   BMR - Aug 2010
 ****************************************************************************/
#define ATTRFILE "tattdatainfo.hdf"
#define NUM_VDATAS 3
#define NUM_VGROUPS 3
#define ATTNAME1 "Attribute 1"
#define ATTNAME2 "Attribute 2"
#define ATTNAME3 "Attribute 3"
#define ATTNAME4 "Attribute 4"

#define FLDNAME1 "Field1"	/* contains one char */
#define FLDNAME2 "Field2"	/* contains three char */
#define FLDNAME3 "Field3"	/* contains two int */
#define FLDNAME4 "Field4"	/* contains one float */
#define	FIELD_NAME_LIST1	"Field1,Field2"
#define	FIELD_NAME_LIST2	"Field3,Field4"

static void
test_vvsattrs(void)
{
    int32 fid;          /* File ID */
    int32 vgroup0_id, vgroup1_id, vgroup2_id; /* Various vgroup IDs */
    int32 vdata0_id, vdata1_id, vdata2_id;    /* Various vdata IDs */
    int32 fldindex;
    int32 ref_list[NUM_VGROUPS], vdref_list[NUM_VDATAS];
    int32 offset, length;
    char vgclass[20];
    int ii;
    int32 status;       /* Status values from routines */
    intn  status_n;	/* returned status for functions returning an intn  */

    /* Attributes to be set for various elements */
    uint32 attr1[4] = {4, 5, 6, 7};
    char attr2[8] = {'V','g','r','o','u','p','0','\0'};
    char attr3[7] = {'V','d','a', 't', 'a', '0', '\0'};
    char attr4[7] = {'F','i','e', 'l', 'd', '1', '\0'};

    /* Create HDF file and initialize the interface. */
    fid = Hopen(ATTRFILE, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Create NUM_VGROUPS vgroups and set classname */
    for (ii = 0; ii < NUM_VGROUPS; ii++)
    {
	int32 vgroup_id, vgroup_ref;

	/* Create a vgroup. */
	vgroup_id = Vattach(fid, -1, "w");
	CHECK_VOID(vgroup_id, FAIL, "Vattach");

	/* Record its reference number for later access */
	vgroup_ref = VQueryref(vgroup_id);
	CHECK_VOID(vgroup_ref, FAIL, "VQueryref:vgroup_id");
	ref_list[ii] = vgroup_ref;

	/* Set its class name */
	sprintf(vgclass, "VG-CLASS-%d", ii);
	status = Vsetclass(vgroup_id, vgclass);
	CHECK_VOID(status, FAIL, "Vsetclass");

	/* Detach it */
	status = Vdetach(vgroup_id);
	CHECK_VOID(status, FAIL, "Vdetach");
    }

    /* Create NUM_VDATAS vgroups and set classname */
    for (ii = 0; ii < NUM_VDATAS; ii++)
    {
	int32 vdata_id, vdata_ref;

	/* Create a vdata. */
	vdata_id = VSattach(fid, -1, "w");
	CHECK_VOID(vdata_id, FAIL, "VSattach");

	/* Record its reference number for later access */
	vdata_ref = VSQueryref(vdata_id);
	CHECK_VOID(vdata_ref, FAIL, "VSQueryref:vdata_id");
	vdref_list[ii] = vdata_ref;

	/* Set its class name */
	sprintf(vgclass, "VS-CLASS-%d", ii);
	status = VSsetclass(vdata_id, vgclass);
	CHECK_VOID(status, FAIL, "VSsetclass");

	/* Detach it */
	status = VSdetach(vdata_id);
	CHECK_VOID(status, FAIL, "VSdetach");
    }

    /* Insert some vdatas/vgroups into some other vgroups to build some sort of
	vgroup/vdata structure */

    /* Insert "VD-CLASS-1" and "VD-CLASS-2" into "VG-CLASS-0" */
    vgroup0_id = Vattach(fid, ref_list[0], "w");  /* "VG-CLASS-0" */
    CHECK_VOID(vgroup0_id, FAIL, "Vattach");
    vdata1_id = VSattach(fid, vdref_list[1], "w");  /* "VD-CLASS-1" */
    CHECK_VOID(vdata1_id, FAIL, "VSattach");
    vdata2_id = VSattach(fid, vdref_list[2], "w");  /* "VD-CLASS-2" */
    CHECK_VOID(vdata2_id, FAIL, "VSattach");

    /* Define fields for vdata1 and vdata2 */
    status = VSfdefine(vdata1_id, FLDNAME1, DFNT_CHAR8, 1);
    CHECK_VOID(vdata1_id, FAIL, "VSfdefine");
    status = VSfdefine(vdata1_id, FLDNAME2, DFNT_CHAR8, 3);
    CHECK_VOID(vdata1_id, FAIL, "VSfdefine");
    status = VSsetfields(vdata1_id, FIELD_NAME_LIST1);
    CHECK_VOID(status, FAIL, "VSsetfields");

    status = VSfdefine(vdata2_id, FLDNAME3, DFNT_INT32, 2);
    CHECK_VOID(vdata2_id, FAIL, "VSfdefine");
    status = VSfdefine(vdata2_id, FLDNAME4, DFNT_FLOAT, 1);
    CHECK_VOID(vdata2_id, FAIL, "VSfdefine");
    status = VSsetfields(vdata2_id, FIELD_NAME_LIST2);
    CHECK_VOID(status, FAIL, "VSsetfields");

    status = Vinsert(vgroup0_id, vdata1_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata1_id into vgroup0_id");
    status = Vinsert(vgroup0_id, vdata2_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata2_id into vgroup0_id");

    /* Insert "VD-CLASS-0", "VG-CLASS-0", and "VG-CLASS-1" into "VG-CLASS-2" */
    vdata0_id = VSattach(fid, vdref_list[0], "w");
    CHECK_VOID(vdata0_id, FAIL, "Vattach");
    vgroup1_id = Vattach(fid, ref_list[1], "w");  /* "VG-CLASS-1" */
    CHECK_VOID(vgroup1_id, FAIL, "Vattach");
    vgroup2_id = Vattach(fid, ref_list[2], "w");  /* "VG-CLASS-2" */
    CHECK_VOID(vgroup2_id, FAIL, "Vattach");

    status = Vinsert(vgroup2_id, vdata0_id);
    CHECK_VOID(status, FAIL, "Vinsert vdata0_id into vgroup2_id");
    status = Vinsert(vgroup2_id, vgroup0_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup0_id into vgroup2_id");
    status = Vinsert(vgroup2_id, vgroup1_id);
    CHECK_VOID(status, FAIL, "Vinsert vgroup1_id into vgroup2_id");

    /***************************************/
    /* Set attributes for various elements */
    /***************************************/

    /* Set two attributes for vgroup0 */
    status = Vsetattr(vgroup0_id, ATTNAME1, DFNT_UINT32, 4, attr1);
    CHECK_VOID(status, FAIL, "Vsetattr vgroup0_id");
    status = Vsetattr(vgroup0_id, ATTNAME2, DFNT_CHAR8, 8, attr2);
    CHECK_VOID(status, FAIL, "Vsetattr vgroup0_id");

    /* Set attribute for vdata0 */
    status = VSsetattr(vdata0_id, _HDF_VDATA, ATTNAME3, DFNT_CHAR8, 7, attr3);
    CHECK_VOID(status, FAIL, "VSsetattr vdata0_id");

    /* Set attribute for vdata1/field1 */
    status = VSfindex(vdata1_id, FLDNAME1, &fldindex);
    CHECK_VOID(status, FAIL, "VSfindex vdata1_id");
    VERIFY_VOID(fldindex, 0, "VSfindex vdata1_id");

    /* Set attribute to field FLDNAME1*/
    status = VSsetattr(vdata1_id, fldindex, ATTNAME4, DFNT_CHAR8, 7, attr4);

    /* Change values of existing attribute */
     /* Vsetattr(vgid, ATTNAME1, DFNT_UINT32, 2, &attr1[2]))
 */ 
    /* Terminate access to any opened elements */
    status = Vdetach(vgroup0_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup0_id");
    status = Vdetach(vgroup1_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup1_id");
    status = Vdetach(vgroup2_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup2_id");

    status = VSdetach(vdata0_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata0_id");
    status = VSdetach(vdata1_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata1_id");
    status = VSdetach(vdata2_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata2_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status_n = Hclose (fid);
    CHECK_VOID(status_n, FAIL, "Hclose");

    /**************************************************************
	The following elements have attributes:
	vgroup0: attr1 and attr2
	vdata0: attr3
	FLDNAME1 of vdata1: attr4
    ***************************************************************/

    /* Open the file to test Vgetattdatainfo and VSgetattdatainfo */
    fid = Hopen(ATTRFILE, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Attach to vgroup0, vdata0, and vdata1 for attribute's data info */
    vgroup0_id = Vattach(fid, ref_list[0], "w");  /* "VG-CLASS-0" */
    CHECK_VOID(vgroup0_id, FAIL, "Vattach");
    vdata0_id = VSattach(fid, vdref_list[0], "w");  /* "VD-CLASS-0" */
    CHECK_VOID(vdata0_id, FAIL, "Vattach");
    vdata1_id = VSattach(fid, vdref_list[1], "w");  /* "VD-CLASS-1" */
    CHECK_VOID(vdata1_id, FAIL, "VSattach");

    /* Get data info of the first attribute from vdata0 */
    status = VSgetattdatainfo(vdata0_id, _HDF_VDATA, 0, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Read and verify an attribute without using HDF4 library */
    status_n = readnoHDF_char(ATTRFILE, offset, length, attr3);
    if (status_n == FAIL)
	fprintf(stderr, "Attempt reading data without HDF4 library failed at line %d\n", __LINE__);

    /* Reset offset/length */
    offset = length = 0;

    /* Get data info of the first attribute from vdata1/FLDNAME1 */
    status = VSgetattdatainfo(vdata1_id, 0, 0, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Read and verify an attribute without using HDF4 library */
    status_n = readnoHDF_char(ATTRFILE, offset, length, attr4);
    if (status_n == FAIL)
	fprintf(stderr, "Attempt reading data without HDF4 library failed at line %d\n", __LINE__);

    /* Get data info of the first attributes from vgroup0 */
    status = Vgetattdatainfo(vgroup0_id, 0, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Reset offset/length */
    offset = length = 0;

    /* Get data info of the second attributes from vgroup0 */
    status = Vgetattdatainfo(vgroup0_id, 1, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Read and verify an attribute without using HDF4 library */
    status_n = readnoHDF_char("tattdatainfo.hdf", offset, length, attr2);
    if (status_n == FAIL)
	fprintf(stderr, "Attempt reading data without HDF4 library failed at line %d\n", __LINE__);

    /* Terminate access to any opened elements */
    status = Vdetach(vgroup0_id);
    CHECK_VOID(status, FAIL, "Vdetach vgroup0_id");
    status = VSdetach(vdata0_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata0_id");
    status = VSdetach(vdata1_id);
    CHECK_VOID(status, FAIL, "VSdetach vdata1_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status_n = Vend (fid);
    CHECK_VOID(status_n, FAIL, "Vend");
    status_n = Hclose (fid);
    CHECK_VOID(status_n, FAIL, "Hclose");
} /* test_vvsattrs() */

/* Test driver for testing the public functions VSgetattdatainfo and
   Vgetattdatainfo. */
void
test_attdatainfo()
{
    /* Test attributes on vgroups and vdatas */
    test_vvsattrs();

    /* Test file attributes */
     /* test_grattrs();
 */ 
}
