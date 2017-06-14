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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tmixed_apis.c - tests the API functions:
 *	SDidtype
 *	Vgetvgroups
 *	VSgetvdatas
 *	Vgisinternal
 * Structure of the file:
 *    test_mixed_apis - test driver
 *	  test_SDAPI_ids    - tests SDidtype on SD API ids: sd, sds, dim ids
 *	  test_nonSDAPI_ids - tests SDidtype on non SD API ids and invalid id
 *	  test_vdatavgroups - tests Vgetvgroups and VSgetvdatas
 *	  test_vgisinternal - tests Vgisinternal
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define IDTYPE_FILE     "idtypes.hdf"	/* data file to test ID types */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

/*
 Tests SDidtype on SD API ids: sd, sds, dim ids
*/
static intn
test_SDAPI_ids()
{
    int32     fid, dset1, dset2, dim_id;
    int32     dimsize[RANK];
    hdf_idtype_t id_type;
    intn      status;
    intn      num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(IDTYPE_FILE, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Create an X_LENGTH by Y_LENGTH dataset, called DataSet_1 */
    dimsize[0] = X_LENGTH;
    dimsize[1] = Y_LENGTH;
    dset1 = SDcreate(fid, "DataSet_1", DFNT_INT32, RANK, dimsize);
    CHECK(dset1, FAIL, "SDcreate");

    /* Create another X_LENGTH by Y_LENGTH dataset, called DataSet_2 */
    dset2 = SDcreate(fid, "DataSet_2", DFNT_FLOAT64, RANK, dimsize);
    CHECK(dset2, FAIL, "SDcreate");

    /* Test SDidtype on the second dataset */
    id_type = SDidtype(dset2);
    VERIFY(id_type, SDS_ID, "SDidtype: id_type");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to test SDidtype more */
    fid = SDstart(IDTYPE_FILE, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /* Test SDidtype on the SD id */
    id_type = SDidtype(fid);
    VERIFY(id_type, SD_ID, "SDidtype: id_type");

    /* Get access to the first dataset and test SDidtype on the SDS id */
    dset1 = SDselect(fid, 0);
    CHECK(dset1, FAIL, "SDselect");
    id_type = SDidtype(dset1);
    VERIFY(id_type, SDS_ID, "SDidtype: id_type");

    /* Get access to the second dataset and test SDidtype on the SDS id */
    dset2 = SDselect(fid, 1);
    CHECK(dset2, FAIL, "SDselect");
    id_type = SDidtype(dset2);
    VERIFY(id_type, SDS_ID, "SDidtype: id_type");

    /* Get dimension handle for first dimension of DataSet_1 and test
       SDidtype on the dimension id */
    dim_id = SDgetdimid(dset1, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    id_type = SDidtype(dim_id);
    VERIFY(id_type, DIM_ID, "SDidtype dim_id");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_SDAPI_ids */

/*
 Tests SDidtype on non SD API ids and invalid id.
*/
static intn
test_nonSDAPI_ids ()
{
    int32     fid, gr_id, vdata_id, ri_id;
    int32     vdata_ref;
    intn      status;
    int32     dims[2]={4,5};    /* dimensions for the empty image */
    hdf_idtype_t id_type;
    intn      num_errs = 0;     /* number of errors so far */

    /* Open the HDF file */
    fid = Hopen(IDTYPE_FILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen: tidtype.hdf");

    /* Initialize HDF for subsequent vdata access */
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    /* Create a new vdata and give it a name */
    vdata_id = VSattach(fid, -1, "w");
    CHECK(vdata_id, FAIL, "VSattach");
    status = VSsetname(vdata_id, "Vdata_1");
    CHECK(status, FAIL, "VSsetname");

    /* Terminate access to the vdata */
    status = VSdetach(vdata_id);
    CHECK(status, FAIL, "VSdetach");

    /* Terminate access to the Vxxx interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Open the HDF file again to test SDidtype */
    fid = Hopen(IDTYPE_FILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen: tidtype.hdf");

    /* Initialize HDF for subsequent vdata accesses */
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    /* Get access to the vdata "Vdata_1" via its reference number */
    vdata_ref = VSfind(fid, "Vdata_1");
    CHECK(vdata_ref, FAIL, "VSfind");
    vdata_id = VSattach(fid, vdata_ref, "w");
    CHECK(vdata_id, FAIL, "VSattach");

    /* Test SDidtype on the vdata id */
    id_type = SDidtype(vdata_id);
    VERIFY(id_type, NOT_SDAPI_ID, "SDidtype: id_type");

    /* Terminate access to the vdata */
    status = VSdetach(vdata_id);
    CHECK(status, FAIL, "VSdetach");

    /* Terminate access to the Vxxx interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Test SDidtype on a GR raster image */

    /* Open file and initialize the GR interface */
    fid = Hopen(IDTYPE_FILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen: tidtype.hdf");
    gr_id = GRstart(fid);
    CHECK(gr_id, FAIL, "GRstart");

    /* Create an empty image with default fill value */
    ri_id = GRcreate(gr_id, "Empty Image", 3, DFNT_FLOAT32, 
		MFGR_INTERLACE_PIXEL, dims);
    CHECK(ri_id, FAIL, "GRcreate");

    /* Test SDidtype on the GR raster image id */
    id_type = SDidtype(ri_id);
    VERIFY(id_type, NOT_SDAPI_ID, "SDidtype: id_type");

    /* Close the image */
    status = GRendaccess(ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Shut down the GR interface and close the file */
    status = GRend(gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_nonSDAPI_ids */


/****************************************************************************
 * test_vdatavgroups - tests the API functions Vgetvgroups and VSgetvdatas.
 *   - Create an SD file and several datasets
 *   - Set attributes to some dimensions to promote them to coordinate
 *	  variables, which will be represented by internally created vgroups.
 *	  The datasets are also represented by internally created vgroups
 *   - Close the file then re-open it with V API
 *   - Create several vgroups and vdatas then close the file
 *   - The file now contains beside the vgroups and vdatas previously created
 *	by the application.
 *	+ a vgroup of class CDF0.0 to represent the SD file
 *	+ various vgroups of class "Var0.0" for SDSs and coordinate variables
 *	+ various vdatas of class "Dim0.0" for dimensions
 *	+ various vdatas of class "Attr0.0" for attributes
 *	+ and more
 *   - Reopen the file using V API
 *   - Verify that Vgetvgroups and VSgetvdatas return only the vgroups and
 *     vdatas that were created by the application.
 * Mar 4, 2011 -BMR
****************************************************************************/
/* data file to test getting vgs/vds from an SD file */
#define VVS_FILE   "tvdatasvgroups_SD.hdf"
#define ATTR1_NAME  "Valid_range 1"
#define ATTR2_NAME  "Unit"
#define X_LENGTH    10
#define Y_LENGTH    10
#define RANK        2
#define NUM_VGS     3
#define NUM_VDS     1

static intn
test_vdatavgroups()
{
    int32   fid, dset1, dset2, dset3, dimid, vgroup_id, vdata_id;
    int32   num_allvgroups, num_allvdatas;
    intn    num_vgroups, num_vdatas;
    int32   dimsize[RANK];
    float32 att1_values[2] = {2., 10.};
    char8   att2_values[]  = "Seconds";
    uint16 *refarray=NULL;
    uint16  name_len=0;
    intn    ii, status;
    char   *vg_name=NULL, vd_name[10];
    const char   *check_vg_names[3] = {"Vgroup_1", "Vgroup_2", "Vgroup_3"};
    const char   *check_vd_names[1] = {"Vdata_1"};
    intn    num_errs = 0;     /* number of errors so far */

    /* Create a file */
    fid = SDstart(VVS_FILE, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Create an X_LENGTH by Y_LENGTH dataset, called DataSet_1 */
    dimsize[0] = X_LENGTH;
    dimsize[1] = Y_LENGTH;
    dset1 = SDcreate(fid, "DataSet_1", DFNT_INT32, RANK, dimsize);
    CHECK(dset1, FAIL, "SDcreate");

    /* Set attribute to first dimension of first data set */
    dimid = SDgetdimid (dset1, 0);
    CHECK(dimid, FAIL, "SDgetdimid");
    status = SDsetattr (dimid, ATTR1_NAME, DFNT_FLOAT32, 2, (VOIDP)att1_values);
    CHECK(status, FAIL, "SDsetattr");

    /* Create another X_LENGTH by Y_LENGTH dataset, called DataSet_2 */
    dset2 = SDcreate(fid, "DataSet_2", DFNT_FLOAT64, RANK, dimsize);
    CHECK(dset2, FAIL, "SDcreate");

    /* Set attribute to second dimension of second data set */
    dimid = SDgetdimid (dset2, 1);
    CHECK(dimid, FAIL, "SDgetdimid");
    status = SDsetattr (dimid, ATTR2_NAME, DFNT_CHAR, 7, (VOIDP)att2_values);
    CHECK(status, FAIL, "SDsetattr");

    /* Create another X_LENGTH by Y_LENGTH dataset, called DataSet_2 */
    dset3 = SDcreate(fid, "DataSet_3", DFNT_FLOAT64, RANK, dimsize);
    CHECK(dset3, FAIL, "SDcreate");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset3);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Open the HDF file and initialize the V interface */
    fid = Hopen(VVS_FILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen: tidtype.hdf");
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    /*   num_allvgroups = Hnumber(fid, DFTAG_VG);
    CHECK(num_allvgroups, FAIL, "Hnumber");
    keep for debugging only */ 

    /* Get the number of user-created vgroups */
    num_vgroups = Vgetvgroups(fid, 0, 0, NULL);
    CHECK(num_vgroups, FAIL, "Vgetvgroups");
    VERIFY(num_vgroups, 0, "Vgetvgroups");

    /* There are internally created vds because of attributes and dimensions */
    num_allvdatas = Hnumber(fid, DFTAG_VH);
    CHECK(num_allvdatas, FAIL, "Hnumber");
    CHECK(num_allvdatas, 0, "Hnumber");  /* so num_allvdatas shouldn't be 0 */

    /* Get the number of user-created vdatas */
    num_vdatas = VSgetvdatas(fid, 0, 0, NULL);
    CHECK(num_vdatas, FAIL, "VSgetvdatas");
    VERIFY(num_vdatas, 0, "VSgetvdatas");

    /* Create several vgroups and vdatas and give them a name */
    vgroup_id = Vattach(fid, -1, "w");
    CHECK(vgroup_id, FAIL, "Vattach");
    status = Vsetname(vgroup_id, "Vgroup_1");
    CHECK(status, FAIL, "Vsetname");
    status = Vdetach(vgroup_id);
    CHECK(status, FAIL, "Vdetach");

    vgroup_id = Vattach(fid, -1, "w");
    CHECK(vgroup_id, FAIL, "Vattach");
    status = Vsetname(vgroup_id, "Vgroup_2");
    CHECK(status, FAIL, "Vsetname");
    status = Vdetach(vgroup_id);
    CHECK(status, FAIL, "Vdetach");

    vgroup_id = Vattach(fid, -1, "w");
    CHECK(vgroup_id, FAIL, "Vattach");
    status = Vsetname(vgroup_id, "Vgroup_3");
    CHECK(status, FAIL, "Vsetname");
    status = Vdetach(vgroup_id);
    CHECK(status, FAIL, "Vdetach");

    vdata_id = VSattach(fid, -1, "w");
    CHECK(vdata_id, FAIL, "VSattach");
    status = VSsetname(vdata_id, "Vdata_1");
    CHECK(status, FAIL, "VSsetname");
    status = VSdetach(vdata_id);
    CHECK(status, FAIL, "VSdetach");

    /* Terminate access to the Vxxx interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Open the HDF file again to test Vgetvgroups */
    fid = Hopen(VVS_FILE, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen: tidtype.hdf");
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    /* Get the number of user-created vgroups */
    num_vgroups = Vgetvgroups(fid, 0, 0, NULL);
    CHECK(num_vgroups, FAIL, "Vgetvgroups");
    VERIFY(num_vgroups, NUM_VGS, "Vgetvgroups");

    /* Allocate sufficient memory to hold the list of user-created vg refs */
    refarray = (uint16 *)HDmalloc(sizeof(uint16) * num_vgroups);
    CHECK(refarray, NULL, "HDmalloc: no space");

    /* Now, get the user-created vgroup refs */
    status = Vgetvgroups(fid, 0, num_vgroups, refarray);
    CHECK(status, FAIL, "Vgetvgroups");
    VERIFY(status, num_vgroups, "Vgetvgroups");

    /* Verify the name of these user-created vgroups */
    for (ii = 0; ii < num_vgroups; ii++)
    {
	vgroup_id = Vattach(fid, refarray[ii], "r");
	CHECK(vgroup_id, FAIL, "Vattach");

	status = Vgetnamelen(vgroup_id, &name_len);
	CHECK(status, FAIL, "Vgetnamelen");

	vg_name = (char *)HDmalloc((sizeof(char) * name_len) + 1);
	CHECK(vg_name, NULL, "HDmalloc: no space");

	status = Vgetname(vgroup_id, vg_name);
	CHECK(status, FAIL, "Vgetname");

	VERIFY_CHAR(vg_name, check_vg_names[ii], "");

	if (strncmp(vg_name, check_vg_names[ii], name_len) != 0)
	    fprintf(stderr, "vg %d: name is %s, should be %s\n", ii, vg_name, check_vg_names[ii]);
	status = Vdetach(vgroup_id);
	CHECK(status, FAIL, "Vdetach");
    }

    /* Get the number of user-created vdatas */
    num_vdatas = VSgetvdatas(fid, 0, 0, NULL);
    CHECK(num_vdatas, FAIL, "VSgetvdatas");
    VERIFY(num_vdatas, NUM_VDS, "VSgetvdatas");

    /* Allocate sufficient memory to hold the list of user-created vd refs */
    refarray = (uint16 *)HDmalloc(sizeof(uint16) * num_vdatas);
    CHECK(refarray, NULL, "HDmalloc: no space");

    /* Now, get the user-created vdata refs */
    status = VSgetvdatas(fid, 0, num_vdatas, refarray);
    CHECK(num_vdatas, FAIL, "VSgetvdatas");
    VERIFY(status, num_vdatas, "VSgetvdatas");

    /* Verify the name of these user-created vdatas */
    for (ii = 0; ii < num_vdatas; ii++)
    {
	vdata_id = VSattach(fid, refarray[ii], "r");
	CHECK(vdata_id, FAIL, "VSattach");

	status = VSgetname(vdata_id, vd_name);
	CHECK(status, FAIL, "VSgetname");

	VERIFY_CHAR(vd_name, check_vd_names[ii], "");

	if (strncmp(vd_name, check_vd_names[ii], name_len) != 0)
	    fprintf(stderr, "vd %d: name is %s, should be %s\n", ii, vd_name, check_vd_names[ii]);
	status = VSdetach(vdata_id);
	CHECK(status, FAIL, "VSdetach");
    }

    /* Terminate access to the Vxxx interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_vdatavgroups */


/****************************************************************************
 * test_vgisinternal - tests the API function Vgisinternal
 *   - Use the existing files created by test_vdatavgroups
 *   - Get the number of vgroups in the file
 *   - For each vgroup, verify that it is internal or not
 * Jan 6, 2012 -BMR
****************************************************************************/

#define VVS_FILE   "tvdatasvgroups_SD.hdf"
#define GR_FILE    "grtdfui83.hdf"
static intn
test_vgisinternal()
{
    int32   fid, vgroup_id;
    intn    is_internal = FALSE;
    int32   vref = -1;
    intn    ii, status;
    char    internal_array1[20] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
                         TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE};
    char    internal_array2[9] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
			 TRUE, TRUE};
    intn    num_errs = 0;     /* number of errors so far */

    /* Open the HDF file and initialize the V interface */
    fid = Hopen(VVS_FILE, DFACC_READ, 0);
    CHECK(fid, FAIL, "Hopen: tvdatasvgroups_SD.hdf");
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    ii = 0;
    while ((vref = Vgetid(fid, vref)) != FAIL)
    {     /* until no more vgroups */
	vgroup_id = Vattach(fid, vref, "r"); /* attach to vgroup */

	/* Test that the current vgroup is or is not internal as specified in
	   the array */
	is_internal = Vgisinternal(vgroup_id);
	CHECK(is_internal, FAIL, "Vgisinternal");
	VERIFY(is_internal, internal_array1[ii], "Vgisinternal");

	status = Vdetach(vgroup_id);
	CHECK(status, FAIL, "Vdetach");

	ii++;
    }

    /* Terminate access to the V interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Use a GR file to test Vgisinternal on internal vgroups */

    /* Open the old HDF file and initialize the V interface */
    fid = Hopen(IDTYPE_FILE, DFACC_READ, 0);
    CHECK(fid, FAIL, "Hopen: idtypes.hdf");
    status = Vstart(fid);
    CHECK(status, FAIL, "Vstart");

    ii = 0;
    while ((vref = Vgetid(fid, vref)) != FAIL)
    {     /* until no more vgroups */
	vgroup_id = Vattach(fid, vref, "r"); /* attach to vgroup */

	/* Test that the current vgroup is or is not internal as specified in
	   the array */
	is_internal = Vgisinternal(vgroup_id);
	CHECK(is_internal, FAIL, "Vgisinternal");
  	VERIFY(is_internal, internal_array2[ii], "Vgisinternal");

	status = Vdetach(vgroup_id);
	CHECK(status, FAIL, "Vdetach");

	ii++; /* increment vgroup index */
    }

    /* Terminate access to the V interface and close the file */
    status = Vend(fid);
    CHECK(status, FAIL, "Vend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_vgisinternal */


/* Test driver for testing the API functions SDidtype, Vgetvgroups, 
   VSgetvdatas, and Vgisinternal. */
extern int
test_mixed_apis()
{
    intn num_errs = 0;         /* number of errors */

    /* Output message about test being performed */
    TESTING("a mix of SD, V, and VS functions (tmixed_apis.c)");

    num_errs = num_errs + test_SDAPI_ids();
    num_errs = num_errs + test_nonSDAPI_ids();

    /* Test Vgetvgroups and VSgetvdatas */
    num_errs = num_errs + test_vdatavgroups();

    /* Test Vgisinternal */
    num_errs = num_errs + test_vgisinternal();

    if (num_errs == 0) PASSED();
    return num_errs;
}

#endif /* HDF */
