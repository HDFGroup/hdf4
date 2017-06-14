/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tattdatainfo.c - tests the functions VSgetattdatainfo, Vgetattdatainfo,
 *		    and GRgetattdatainfo.
 * Structure of the file:
 *    test_attdatainfo - test driver
 *	test_vvsattrs     - tests attributes on vgroups and vdatas
 *	test_vgmixedattrs - tests handling vgroup attributes created with and 
 *			    without Vsetattr
 *	test_grattrs      - tests attributes on GR file and raster images
 * -BMR, Aug 2010
 ****************************************************************************/

#include "hdf.h"
#include "tdatainfo.h"
#include "tutils.h"

static void test_vvsattrs();
static void test_vgmixedattrs();
static void test_grattrs();

/****************************************************************************
   Name: test_vvsattrs() - tests getting attribute data information from
			   vdatas and vgroups

   Description:
	This routine performs the following steps:
	- Create a new file with the V interface
	- Creates several vgroups and vdatas, and forms some arbitrary
	  structures among these elements.
	- Add some attributes to various vgroups and vdatas.
	- Retrieve offset/length of the data of some attributes.
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
#define ATTNAME5 "Attribute 5"
#define ATTNAME6 "Attribute 6"
#define ATTNAME7 "Attribute 7"

#define FLDNAME1 "Field1"	/* contains one char */
#define FLDNAME2 "Field2"	/* contains three chars */
#define FLDNAME3 "Field3"	/* contains two ints */
#define FLDNAME4 "Field4"	/* contains one float */
#define	FIELD_NAME_LIST1	"Field1,Field2"
#define	FIELD_NAME_LIST2	"Field3,Field4"

static void
test_vvsattrs()
{
    int32 fid;          /* File ID */
    int32 vgroup0_id, vgroup1_id, vgroup2_id; /* Various vgroup IDs */
    int32 vdata0_id, vdata1_id, vdata2_id;    /* Various vdata IDs */
    int32 fldindex;
    int32 ref_list[NUM_VGROUPS], vdref_list[NUM_VDATAS];
    int32 offset, length;
    char vgclass[20];
    int ii;
    intn  status;	/* returned status */
    intn  status_32;	/* returned status for functions returning an int32! */

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
	status_32 = Vsetclass(vgroup_id, vgclass);
	CHECK_VOID(status_32, FAIL, "Vsetclass");

	/* Detach it */
	status_32 = Vdetach(vgroup_id);
	CHECK_VOID(status_32, FAIL, "Vdetach");
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
	status_32 = VSsetclass(vdata_id, vgclass);
	CHECK_VOID(status_32, FAIL, "VSsetclass");

	/* Detach it */
	status_32 = VSdetach(vdata_id);
	CHECK_VOID(status_32, FAIL, "VSdetach");
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
    CHECK_VOID(status, FAIL, "VSfdefine");
    status = VSfdefine(vdata1_id, FLDNAME2, DFNT_CHAR8, 3);
    CHECK_VOID(status, FAIL, "VSfdefine");
    status = VSsetfields(vdata1_id, FIELD_NAME_LIST1);
    CHECK_VOID(status, FAIL, "VSsetfields");

    status = VSfdefine(vdata2_id, FLDNAME3, DFNT_INT32, 2);
    CHECK_VOID(status, FAIL, "VSfdefine");
    status = VSfdefine(vdata2_id, FLDNAME4, DFNT_FLOAT, 1);
    CHECK_VOID(status, FAIL, "VSfdefine");
    status = VSsetfields(vdata2_id, FIELD_NAME_LIST2);
    CHECK_VOID(status, FAIL, "VSsetfields");

    status_32 = Vinsert(vgroup0_id, vdata1_id);
    CHECK_VOID(status_32, FAIL, "Vinsert vdata1_id into vgroup0_id");
    status_32 = Vinsert(vgroup0_id, vdata2_id);
    CHECK_VOID(status_32, FAIL, "Vinsert vdata2_id into vgroup0_id");

    /* Insert "VD-CLASS-0", "VG-CLASS-0", and "VG-CLASS-1" into "VG-CLASS-2" */
    vdata0_id = VSattach(fid, vdref_list[0], "w");
    CHECK_VOID(vdata0_id, FAIL, "Vattach");
    vgroup1_id = Vattach(fid, ref_list[1], "w");  /* "VG-CLASS-1" */
    CHECK_VOID(vgroup1_id, FAIL, "Vattach");
    vgroup2_id = Vattach(fid, ref_list[2], "w");  /* "VG-CLASS-2" */
    CHECK_VOID(vgroup2_id, FAIL, "Vattach");

    status_32 = Vinsert(vgroup2_id, vdata0_id);
    CHECK_VOID(status_32, FAIL, "Vinsert vdata0_id into vgroup2_id");
    status_32 = Vinsert(vgroup2_id, vgroup0_id);
    CHECK_VOID(status_32, FAIL, "Vinsert vgroup0_id into vgroup2_id");
    status_32 = Vinsert(vgroup2_id, vgroup1_id);
    CHECK_VOID(status_32, FAIL, "Vinsert vgroup1_id into vgroup2_id");

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
    CHECK_VOID(status, FAIL, "VSsetattr vdata1_id");

    /* Change values of existing attribute */
     /* Vsetattr(vgid, ATTNAME1, DFNT_UINT32, 2, &attr1[2]))
 */ 
    /* Terminate access to any opened elements */
    status_32 = Vdetach(vgroup0_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup0_id");
    status_32 = Vdetach(vgroup1_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup1_id");
    status_32 = Vdetach(vgroup2_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup2_id");

    status_32 = VSdetach(vdata0_id);
    CHECK_VOID(status_32, FAIL, "VSdetach vdata0_id");
    status_32 = VSdetach(vdata1_id);
    CHECK_VOID(status_32, FAIL, "VSdetach vdata1_id");
    status_32 = VSdetach(vdata2_id);
    CHECK_VOID(status_32, FAIL, "VSdetach vdata2_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status = Vend (fid);
    CHECK_VOID(status, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

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
    status = readnoHDF_char(ATTRFILE, offset, length, attr3);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");

    /* Reset offset/length */
    offset = length = 0;

    /* Get data info of the first attribute from vdata1/FLDNAME1 */
    status = VSgetattdatainfo(vdata1_id, 0, 0, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Read and verify an attribute without using HDF4 library */
    status = readnoHDF_char(ATTRFILE, offset, length, attr4);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");

    /* Get data info of the first attributes from vgroup0 */
    status = Vgetattdatainfo(vgroup0_id, 0, &offset, &length);
    CHECK_VOID(status, FAIL, "Vgetattdatainfo");

    /* Reset offset/length */
    offset = length = 0;

    /* Get data info of the second attributes from vgroup0 */
    status = Vgetattdatainfo(vgroup0_id, 1, &offset, &length);
    CHECK_VOID(status, FAIL, "Vgetattdatainfo");

    /* Read and verify an attribute without using HDF4 library */
    status = readnoHDF_char("tattdatainfo.hdf", offset, length, attr2);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");

    /* Terminate access to any opened elements */
    status_32 = Vdetach(vgroup0_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup0_id");
    status_32 = VSdetach(vdata0_id);
    CHECK_VOID(status_32, FAIL, "VSdetach vdata0_id");
    status_32 = VSdetach(vdata1_id);
    CHECK_VOID(status_32, FAIL, "VSdetach vdata1_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status = Vend (fid);
    CHECK_VOID(status, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");
} /* test_vvsattrs() */

static void
test_vgmixedattrs()
{
    int32 fid;          /* File ID */
    int32 vgroup_id, vgroup_ref;
    int32 n_attrs;
    int32 ref_list[NUM_VGROUPS], vdref_list[NUM_VDATAS];
    int32 offsets[10], lengths[10];  /* offsets and lengths of attrs' data */
    /* Note: each array element is associated with an individual attribute, not
       a data element block; data of an attribute only has one pair of off/len*/
    char vgclass[20];
    int ii;
    int32 attr_ref;
    intn  status;	/* returned status */
    intn  status_32;	/* returned status for functions returning an int32! */

    /* Attribute names to be checked against */
    char aname_check[5][20] = {ATTNAME1, ATTNAME2, ATTNAME5, ATTNAME6, ATTNAME7};

    /* Attributes to be set for various elements */
    uint32 attr1[4] = {4, 5, 6, 7}; /* copied from test_vvsattrs, only to
					verify previously added attribute */
    char attr2[12] = {'V','G','0',' ','n','e','w','a','t','t','r','\0'};
    char attr3[13] = {'V','G','0',' ','o','l','d','a','t','t','r','0','\0'};
    char attr4[13] = {'V','G','0',' ','o','l','d','a','t','t','r','1','\0'};

    /* Create HDF file and initialize the interface. */
    fid = Hopen(ATTRFILE, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

	/* Get access to first vgroup. */
    vgroup_ref = Vgetid(fid, -1);
    CHECK_VOID(vgroup_ref, FAIL, "Vgetid");
    vgroup_id = Vattach(fid, vgroup_ref, "w");
    CHECK_VOID(vgroup_id, FAIL, "Vattach");

    /* Current number of attributes belong to this vgroup */
    n_attrs = Vnattrs(vgroup_id);
    CHECK_VOID(n_attrs, FAIL, "Vnattrs");
    VERIFY_VOID(n_attrs, 2, "Vnattrs");
    for (ii = 0; ii < n_attrs; ii++)
    {
	char aname[20];
	int32 atype, acount, asize;
	status = Vattrinfo(vgroup_id, ii, aname, &atype, &acount, &asize);
        /* HDstrncmp(iattrname, ATTNAME1, HDstrlen(ATTNAME1)) != 0) */
    }
    n_attrs = Vnoldattrs(vgroup_id);
    VERIFY_VOID(n_attrs, 0, "Vnoldattrs");

    /* Now, add one attribute with Vsetattr, and two attributes with
	VHstoredatam/Vaddtagref combination */

    /* Add one new-style attribute */
    status = Vsetattr(vgroup_id, ATTNAME5, DFNT_CHAR8, 12, attr2);
    CHECK_VOID(status, FAIL, "Vsetattr vgroup_id");

    /* Add two old-style attributes */
    attr_ref = VHstoredatam(fid, ATTR_FIELD_NAME, (unsigned char *) attr3, 1,
                          DFNT_CHAR8, ATTNAME6, _HDF_ATTRIBUTE, 13);
    CHECK_VOID(attr_ref, FAIL, "VHstoredatam");
    status = Vaddtagref(vgroup_id, DFTAG_VH, attr_ref);
    CHECK_VOID(status, FAIL, "Vaddtagref");

    attr_ref = VHstoredatam(fid, ATTR_FIELD_NAME, (unsigned char *) attr4, 1,
                          DFNT_CHAR8, ATTNAME7, _HDF_ATTRIBUTE, 13);
    CHECK_VOID(attr_ref, FAIL, "VHstoredatam");
    status = Vaddtagref(vgroup_id, DFTAG_VH, attr_ref);
    CHECK_VOID(status, FAIL, "Vaddtagref");

    /* Terminate access to any opened elements */
    status_32 = Vdetach(vgroup_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status = Vend (fid);
    CHECK_VOID(status, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");

    /**************************************************************
	The following element has changed:
	vgroup0: 2 attrs from previous test function, then attr2 of
	new style and attr3 and attr4 of old style here
    ***************************************************************/


    /* Re-open the file to test Vnattrs2, Vattrinfo2, Vgetattr2, and
	Vgetattdatainfo */
    fid = Hopen(ATTRFILE, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");
    status = Vstart(fid);
    CHECK_VOID(status, FAIL, "Vstart");

    /* Attach to vgroup0 for attribute info and data */
    vgroup_id = Vattach(fid, vgroup_ref, "w");  /* "VG-CLASS-0" */
    CHECK_VOID(vgroup_id, FAIL, "Vattach");

    /* Vnattrs returns number of attributes added by Vsetattr */
    n_attrs = Vnattrs(vgroup_id);
    VERIFY_VOID(n_attrs, 3, "Vnattrs");

    /* Vnoldattrs returns number of attrs added by VHstoredatam/Vaddtagref */
    n_attrs = Vnoldattrs(vgroup_id);
    VERIFY_VOID(n_attrs, 2, "Vnoldattrs");

    /* Vnattrs2 returns total number of attrs, regardless how they were added */
    n_attrs = Vnattrs2(vgroup_id);
    VERIFY_VOID(n_attrs, 5, "Vnattrs2");

    /* Test Vattrinfo2 and Vgetattr2 on each attribute */
    for (ii = 0; ii < n_attrs; ii++)
    {
	char aname[20], values[100];
	int32 atype, acount, asize, n_fields;
	uint16 refnum;
	int jj;
	char cvalues[20];
	int32 ivalues[10];
	char *check_attr_names[5] = {"Attribute 6", "Attribute 7", "Attribute 1"
					, "Attribute 2", "Attribute 5"};
	char *check_attr_values[5] = {"VG0 oldattr0", "VG0 oldattr1",
			"", /* int values, added by test_vvsattrs with attr1 */
			"Vgroup0", /* added by test_vvsattrs with attr2 */
			"VG0 newattr"};

	/* Get attribute information and verify its name and number of fields,
	   which should be 1 */
	status = Vattrinfo2(vgroup_id, ii, aname, &atype, &acount, &asize,
			&n_fields, &refnum);
	VERIFY_CHAR_VOID(aname, check_attr_names[ii], "Vattrinfo2");
	VERIFY_VOID(n_fields, 1, "Vattrinfo2");

	/* Test Vgetattr2 to make sure it works with mixed attributes */
	switch (atype)
	{
	  case DFNT_CHAR:
	    /* Get and verify values of a char attribute */
	    status = Vgetattr2(vgroup_id, ii, (void *)cvalues);
	    VERIFY_CHAR_VOID(cvalues, check_attr_values[ii], "Vgetattr2 char");

	    /* Add an extra test for Vgetattdatainfo */

	    /* Get data info and verify number of data block */
	    status = Vgetattdatainfo(vgroup_id, ii, &offsets[ii], &lengths[ii]);
	    CHECK_VOID(status, FAIL, "Vgetattdatainfo");
	    VERIFY_VOID(status, 1, "Vgetattdatainfo");

	    /* Read and verify data of an attr without using HDF4 library */
	    status = readnoHDF_char(ATTRFILE, offsets[ii], lengths[ii], check_attr_values[ii]);
	    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");

	    break;
	  case DFNT_UINT32:
	    /* Get and verify values of an int attribute */
	    status = Vgetattr2(vgroup_id, ii, (void *)ivalues);
	    for (jj = 0; jj < acount; jj++)
		VERIFY_VOID(ivalues[jj], attr1[jj], "Vgetattr2 int");
	    break;
	  default:
	    fprintf(stderr, "type %d is not handled!\n", atype);
	    break;
	}
    }

    /* Terminate access to the vgroup */
    status_32 = Vdetach(vgroup_id);
    CHECK_VOID(status_32, FAIL, "Vdetach vgroup_id");

    /* Terminate access to the V interface and close the HDF file.  */
    status = Vend (fid);
    CHECK_VOID(status, FAIL, "Vend");
    status = Hclose (fid);
    CHECK_VOID(status, FAIL, "Hclose");
} /* test_vgmixedattrs() */


/****************************************************************************
   Name: test_grattrs() - tests getting attribute data information from
			   GR file and raster images

   Description:
	This routine performs the following steps:
	- Create a new file with the GR interface
	- Creates and write two raster images with GRcreate and GRwritedata
	- Set two attributes to the file
	- Set one attribute to each of the images
	- Terminate access to images and file
	- Reopen the file and images to retrieve their attribute's data info
	- Call readnoHDF_char to open the file without HDF library then
	  read in attributes having char data using the previously recorded
	  offsets/lengths and verify the data
   BMR - Sep 2010
 ****************************************************************************/
#define GRATTRFILE	"tgrattrdatainfo.hdf"
#define	IMAGE_NAME	"Image Array 1"
#define	F_ATT1_NAME	"File Attribute 1"
#define	F_ATT2_NAME	"File Attribute 2"
#define	RI_ATT1_NAME	"Image Attribute 1"
#define	RI_ATT2_NAME	"Image Attribute 2"
#define	F_ATT1_VAL	"Contents of First FILE attribute"
#define	F_ATT2_VAL	"Contents of Second FILE attribute"
#define	F_ATT1_COUNT	32
#define	F_ATT2_COUNT	33
#define	RI_ATT1_VAL	"Contents of IMAGE's First Attribute"
#define	RI_ATT1_COUNT	35
#define	RI_ATT2_COUNT	6
#define	X_LENGTH	10	/* number of columns in the image */
#define	Y_LENGTH	5	/* number of rows in the image */
#define	N_COMPS		1	/* number of components in the image */

static void
test_grattrs()
{
    int32 file_id,	/* HDF file identifier */
	  gr_id,	/* GR interface identifier */
	  ri_id,	/* raster image identifier */
	  num_type,	/* number type of the image values */
          start[2],	/* where to start to write for each dimension  */
          edges[2],	/* how long to write for each dimension */
	  dimsizes[2],	/* sizes of the two dimensions of the image array   */
	  interlace_mode,/* interlace mode of the image */
	  ii, jj;
    intn  status;
    int16 ri_att2_val[RI_ATT2_COUNT] = {1, 2, 3, 4, 5, 6};
    int8  image_buf[Y_LENGTH][X_LENGTH][N_COMPS];
    int32 offsets[4], lengths[4];  /* offsets and lengths of attrs' data */
    /* Note: each array element is associated with an individual attribute, not
       a data element block; data of an attribute only has one pair of off/len*/

    /********************** End of variable declaration **********************/

    /* Create and open the file. */
    file_id = Hopen (GRATTRFILE, DFACC_CREATE, 0);
    CHECK_VOID(file_id, FAIL, "Hopen");

    /* Initialize the GR interface. */
    gr_id = GRstart (file_id);
    CHECK_VOID(gr_id, FAIL, "GRstart");

    /* Set the number type, interlace mode, and dimensions of the image. */
    interlace_mode = MFGR_INTERLACE_LINE;
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;

    /* Create a raster image array. */
    ri_id = GRcreate(gr_id, IMAGE_NAME, N_COMPS, DFNT_INT8, 
   				interlace_mode, dimsizes);
    CHECK_VOID(ri_id, FAIL, "GRcreate");

    /* Fill the image data buffer with values. */
    for (ii = 0; ii < Y_LENGTH; ii++)
    {
      for (jj = 0; jj < X_LENGTH; jj++)
      {
         image_buf[ii][jj][0] = (ii + jj) + 1;     /* first component */
      }
    }

    /* Define the size of the data to be written, i.e., start from the origin
    * and go as long as the length of each dimension. */
    start[0] = start[1] = 0;
    edges[0] = X_LENGTH;
    edges[1] = Y_LENGTH;

    /* Write the data into the image array and terminate access to the RI. */
    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)image_buf);
    CHECK_VOID(status, FAIL, "GRwriteimage");
    status = GRendaccess(ri_id);
    CHECK_VOID(status, FAIL, "GRendaccess");

    /* Set two file attributes to the file with names, data types, numbers of 
    * values, and values of the attributes specified. */
    status = GRsetattr(gr_id, F_ATT1_NAME, DFNT_CHAR8, F_ATT1_COUNT, F_ATT1_VAL); 
    CHECK_VOID(status, FAIL, "GRsetattr F_ATT1_NAME");

    status = GRsetattr(gr_id, F_ATT2_NAME, DFNT_CHAR8, F_ATT2_COUNT, F_ATT2_VAL);
    CHECK_VOID(status, FAIL, "GRsetattr F_ATT2_NAME");

    /* Get access to the first and only image in the file */
    ri_id = GRselect(gr_id, 0);
    CHECK_VOID(ri_id, FAIL, "GRselect index 0");

    /* Set two attribute to the image with names, data types, numbers of 
    * values, and values of the attributes specified. */
    status = GRsetattr(ri_id, RI_ATT1_NAME, DFNT_CHAR8, RI_ATT1_COUNT, RI_ATT1_VAL);
    CHECK_VOID(status, FAIL, "GRsetattr RI_ATT1_NAME");

    status = GRsetattr(ri_id, RI_ATT2_NAME, DFNT_INT16, RI_ATT2_COUNT, (VOIDP)ri_att2_val);
    CHECK_VOID(status, FAIL, "GRsetattr RI_ATT2_NAME");

    /* Terminate access to the raster image and to the GR interface and 
    * close the HDF file. */
    status = GRendaccess (ri_id);
    CHECK_VOID(status, FAIL, "GRendaccess");
    status = GRend (gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK_VOID(status, FAIL, "Hclose");

    /* Re-open the file, get data info of some attributes, then verify their
       data */

    /* Open the HDF file and initialize the GR interface. */
    file_id = Hopen(GRATTRFILE, DFACC_RDONLY, 0);
    CHECK_VOID(file_id, FAIL, "Hopen GRATTRFILE");
    gr_id = GRstart(file_id);
    CHECK_VOID(gr_id, FAIL, "GRstart GRATTRFILE");

    /* Get access to the image, first and only */
    ri_id = GRselect(gr_id, 0);
    CHECK_VOID(ri_id, FAIL, "GRselect index 0");

    /* Get data info of file attr and store them in offsets/lengths[0,1] */
    status = GRgetattdatainfo(gr_id, 0, &offsets[0], &lengths[0]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");
    status = GRgetattdatainfo(gr_id, 1, &offsets[1], &lengths[1]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");

    /* Get data info of image attr and store them in offsets/lengths[2,3] */
    status = GRgetattdatainfo(ri_id, 0, &offsets[2], &lengths[2]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");
    status = GRgetattdatainfo(ri_id, 1, &offsets[3], &lengths[3]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");

    /* This image should be mapped-able by the HDF4 map writer because even
       though it was created by GR, it has 8-bit data, 1 compnonent, and no
       compression */
    {
	intn is_mappedable;
	intn name_generated;

        status = GR2bmapped(ri_id, &is_mappedable, &name_generated);
        CHECK_VOID(status, FAIL, "GR2bmapped");
        VERIFY_VOID(is_mappedable, TRUE, "GR2bmapped");
        VERIFY_VOID(name_generated, FALSE, "GR2bmapped");
    }

    /* Terminate access to the image and to the GR interface and close the
     * HDF file. */
    status = GRendaccess(ri_id);
    CHECK_VOID(status, FAIL, "GRendaccess");
    status = GRend(gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose(file_id);
    CHECK_VOID(status, FAIL, "Hclose");

    /* Verify data of attributes without the use of HDF4 library */
    status = readnoHDF_char(GRATTRFILE, offsets[0], lengths[0], F_ATT1_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    status = readnoHDF_char(GRATTRFILE, offsets[1], lengths[1], F_ATT2_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    status = readnoHDF_char(GRATTRFILE, offsets[2], lengths[2], RI_ATT1_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    /* Note: readnoHDF_char is defined in tdatainfo.c */
} /* test_grattrs() */

/* Test driver for testing the public functions VSgetattdatainfo, 
   Vgetattdatainfo, and GRgetattdatainfo. */
void
test_attdatainfo()
{
    /* Test attributes on vgroups and vdatas */
    test_vvsattrs();

    /* Test handling vgroup attributes created without Vsetattr */
    test_vgmixedattrs();

    /* Test GR API attributes */
    test_grattrs();
}
