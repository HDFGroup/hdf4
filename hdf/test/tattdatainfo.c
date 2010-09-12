/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
#define FLDNAME2 "Field2"	/* contains three chars */
#define FLDNAME3 "Field3"	/* contains two ints */
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
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

    /* Reset offset/length */
    offset = length = 0;

    /* Get data info of the second attributes from vgroup0 */
    status = Vgetattdatainfo(vgroup0_id, 1, &offset, &length);
    CHECK_VOID(status, FAIL, "VSgetattdatainfo");

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
#define	N_COMPS		2	/* number of components in the image */

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
    int16 image_buf[Y_LENGTH][X_LENGTH][N_COMPS];
    int32 offsets[4], lengths[4];
    /* each array element is associated with an individual attribute, not
       block of a data element, like when data element has multiple blocks */

    /********************** End of variable declaration **********************/

    /* Create and open the file. */
    file_id = Hopen (ATTRFILE, DFACC_CREATE, 0);
    CHECK_VOID(file_id, FAIL, "Hopen");

    /* Initialize the GR interface. */
    gr_id = GRstart (file_id);
    CHECK_VOID(gr_id, FAIL, "GRstart");

    /* Set the number type, interlace mode, and dimensions of the image. */
    num_type = DFNT_INT16;
    interlace_mode = MFGR_INTERLACE_LINE;
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;

    /* Create the raster image array. */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, num_type, 
   				interlace_mode, dimsizes);
    CHECK_VOID(ri_id, FAIL, "GRcreate");

    /* Fill the first image data buffer with values. */
    for (ii = 0; ii < Y_LENGTH; ii++)
    {
      for (jj = 0; jj < X_LENGTH; jj++)
      {
         image_buf[ii][jj][0] = (ii + jj) + 1;     /* first component */
         image_buf[ii][jj][1] = (ii + jj) + 1;     /* second component */
      }
    }

    /* Define the size of the data to be written, i.e., start from the origin
    * and go as long as the length of each dimension. */
    start[0] = start[1] = 0;
    edges[0] = X_LENGTH;
    edges[1] = Y_LENGTH;

    /* Write the data in the buffer into the image array. */
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

    /* Obtain the identifier of this image. */
    ri_id = GRselect(gr_id, 0);
    CHECK_VOID(ri_id, FAIL, "GRselect index 0");

    /* Set two attribute to the image with names, data types, numbers of 
    * values, and values of the attributes specified. */
    status = GRsetattr(ri_id, RI_ATT1_NAME, DFNT_CHAR8, RI_ATT1_COUNT, RI_ATT1_VAL);
    CHECK_VOID(status, FAIL, "GRsetattr RI_ATT1_NAME");

    status = GRsetattr(ri_id, RI_ATT2_NAME, DFNT_INT16, RI_ATT2_COUNT, 
   					(VOIDP)ri_att2_val);

    CHECK_VOID(status, FAIL, "GRsetattr RI_ATT2_NAME");

    /* Terminate access to the raster image and to the GR interface and 
    * close the HDF file. */
    status = GRendaccess (ri_id);
    CHECK_VOID(status, FAIL, "GRendaccess");
    status = GRend (gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK_VOID(status, FAIL, "Hclose");

    /* Open the HDF file. */
    file_id = Hopen(ATTRFILE, DFACC_RDONLY, 0);
    CHECK_VOID(file_id, FAIL, "Hopen ATTRFILE");

    /* Initialize the GR interface. */
    gr_id = GRstart(file_id);
    CHECK_VOID(gr_id, FAIL, "GRstart ATTRFILE");

    ri_id = GRselect(gr_id, 0);
    CHECK_VOID(ri_id, FAIL, "GRselect index 0");

    status = GRgetattdatainfo(gr_id, 0, NULL, &offsets[0], &lengths[0]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");
    status = GRgetattdatainfo(gr_id, -1, F_ATT2_NAME, &offsets[1], &lengths[1]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");
    status = GRgetattdatainfo(ri_id, -1, RI_ATT1_NAME, &offsets[2], &lengths[2]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");
    status = GRgetattdatainfo(ri_id, 1, NULL, &offsets[3], &lengths[3]);
    CHECK_VOID(status, FAIL, "GRgetattdatainfo");

    /* Terminate access to the image and to the GR interface and close the
     * HDF file. */
    status = GRend(gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose(file_id);
    CHECK_VOID(status, FAIL, "Hclose");

    status = readnoHDF_char(ATTRFILE, offsets[0], lengths[0], F_ATT1_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    status = readnoHDF_char(ATTRFILE, offsets[1], lengths[1], F_ATT2_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    status = readnoHDF_char(ATTRFILE, offsets[2], lengths[2], RI_ATT1_VAL);
    CHECK_STATUS(status, FAIL, "Verifying data without HDF4 library failed");
    /* Note: readnoHDF_char should be modified to move fopen/fclose back out of
	the function like before. */

} /* test_grattrs() */

/* Test driver for testing the public functions VSgetattdatainfo and
   Vgetattdatainfo. */
void
test_attdatainfo()
{
    /* Test attributes on vgroups and vdatas */
    test_vvsattrs();

    /* Test GR API attributes */
    test_grattrs();
}
