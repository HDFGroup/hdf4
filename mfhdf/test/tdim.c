/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

/********************************************************************
   HISTORY:
   - 04/18/01
   This test is added to test SDsetdimscale while fixing bug #172.
   Beside the fact that the main program was already very long, adding
   this separate test routine will also define a place for additional
   dimension tests to be appended in the future.  Also, hopefully, some
   day, the main program can be shortened and some of its
   dimension-related tests can be moved into this test routine.

   - 07/28/09 
   Moved tests in test_dimensions into smaller subtests and added test
   for SDgetdimstrs.  The structure of this file becomes:
	test_dimensions
	    test_dim_basics
	    test_dim_scales
	    test_dim_strs
*********************************************************************/

/********************************************************************
   Name: test_dim_basics()

   Description: 
	This test routine is used to test various dimension operations.
	The main contents include:
	- creates SDS #1 of size LENGTH0xLENGTH1, and sets values to its dims
	- creates SDS #2 of size LENGTH2xLENGTH3, and sets values to its 
		first dimension

	The followings are included in this test routine:
	- SDgetdimid
	- SDsetdimname
	- SDdiminfo

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
#define BASIC_FILE  "dim.hdf"
#define LENGTH0 15		/* dimensions of SDSs #1 and #3 */
#define LENGTH1 10
#define LENGTH2 2		/* dimensions of SDSs #2 and #4 */
#define LENGTH3 3
#define RANK2 	2		/* ranks */
#define DS0_NAME "HDF Data 0"	/* SDS #1 name */
#define DS1_NAME "HDF Data 1"	/* SDS #2 name */
#define DIM0_NAME "Dimension 0"	/* name of first SDS' first dim */
#define DIM1_NAME "Dimension 1"	/* name of first SDS' second dim */
#define DIM2_NAME "Dimension 2"	/* name of second SDS' first dim */
#define DIM3_NAME "Dimension 3"	/* name of third SDS' first dim */

static intn test_basic_dim()
{
    int32  fid, sds_id, status, dim0_id, dim1_id, sds_idx;
    int32  dims[2], start[2], edges[2], rank;
    int16  array1_data[LENGTH0][LENGTH1];	/* data for first SDS */
    uint32 array2_data[LENGTH2][LENGTH3];	/* data for second SDS */
    int32  dim_sizes[H4_MAX_VAR_DIMS];		/* read dimensions */
    intn   i, j;
    int32  array_rank, num_type, attributes;
    char   dim_name[H4_MAX_NC_NAME], name[H4_MAX_NC_NAME];
    uint8  scale0 [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,255};
    int16  scale1 [] = {0,1,2,3,4,5,6,7,8,9};
    char8  scale2 [] = {'d','i','m','2'}, scale2_out[4];
    float32 scalef[] = {1., 2., 3., 4.};
    int32  size, dim_data_type, dim_num_attrs;
    int    num_errs = 0;    /* number of errors so far */

    /* Create the file defined by BASIC_FILE and initiate the SD interface. */
    fid = SDstart(BASIC_FILE, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /*
     * Add a LENGTH0 x LENGTH1 array, which is named by DS0_NAME and of
     * type 16-bit signed integer, to the file... 
     */

    /* Define the rank and dimensions of the data set. */
    rank = RANK2;
    dims[0] = LENGTH0;
    dims[1] = LENGTH1;

    /* Create the array data set. */
    sds_id = SDcreate(fid, DS0_NAME, DFNT_INT16, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /*
     * Settings for the first dimension.  Note that all these settings
     * are done before writing the data to the dataset; just to make sure
     * that it's possible...
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to the name defined by DIM0_NAME */
    status = SDsetdimname(dim0_id, DIM0_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Set scale of type unsigned int-8 for the first dimension */
    status = SDsetdimscale(dim0_id, dims[0], DFNT_UINT8, scale0);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the first dimension and verify its information */
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(strcmp(dim_name, DIM0_NAME), 0, "SDdiminfo");
    VERIFY(size, dims[0], "SDdiminfo");
    VERIFY(dim_data_type, DFNT_UINT8, "SDdiminfo"); /* bug #172 is fixed! */
    VERIFY(dim_num_attrs, 0, "SDdiminfo");

    /* 
     * Write the data to the dataset...
     */

    /* Fill the buffer with values. */
    for (j = 0; j < LENGTH0; j++) {
        for (i = 0; i < LENGTH1; i++)
            array1_data[j][i] = (i + j) + 1;
    }
    /* Define the area for writing to the dataset */
    for (i = 0; i < rank; i++) {
        start[i] = 0;
        edges[i] = dims[i];
    }
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array1_data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open BASIC_FILE in W mode */
    fid = SDstart(BASIC_FILE, DFACC_WRITE);
    CHECK(fid, FAIL, "SDstart");

    /* Look for DS0_NAME dataset */
    sds_idx = SDnametoindex(fid, DS0_NAME);
    CHECK(sds_idx, FAIL, "SDnametoindex");

    /* Select that dataset */
    sds_id = SDselect(fid, sds_idx);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get info of the dataset and verify them: it is a LENGTH0 x LENGTH1 
       array of type DFNT_INT16 and is named by DS0_NAME */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(strcmp(name, DS0_NAME), 0, "SDgetinfo");
    VERIFY(array_rank, rank, "SDgetinfo");
    VERIFY(num_type, DFNT_INT16, "SDgetinfo");
    VERIFY(dim_sizes[0], LENGTH0, "SDgetinfo");
    VERIFY(dim_sizes[1], LENGTH1, "SDgetinfo");

    /*
     * Setting name for the second dimension
     */

    /* Get the second dimension id */
    dim1_id = SDgetdimid(sds_id, 1);
    CHECK(dim1_id, FAIL, "SDgetdimid");

    /* Set the second dimension name to the name defined by DIM1_NAME */
    status = SDsetdimname(dim1_id, DIM1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Read the second dimension and verify its information; since the
       scale of this dimension is not set yet, its number type should be 0 */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(strcmp(dim_name, DIM1_NAME), 0, "SDdiminfo");
    VERIFY(size, dims[1], "SDdiminfo");
    VERIFY(dim_data_type, 0, "SDdiminfo");
    VERIFY(dim_num_attrs, 0, "SDdiminfo");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Add another dataset to the file; this dataset is a LENGTH2 x LENGTH3
     * array, named by DS1_NAME and of type 32-bit unsigned integer...
     */

    /* Define the rank and dimensions of the data set */
    rank = RANK2;
    dims[0] = LENGTH2;
    dims[1] = LENGTH3;

    for (i = 0; i < rank; i++)
        edges[i] = dims[i];

    /* Create the data set */
    sds_id = SDcreate(fid, DS1_NAME, DFNT_UINT32, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /*
     * Verify the default name of the first dimension, then set it to the
     * name DIM2_NAME and verify its new name.
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(HDstrcmp(dim_name, "fakeDim2"), 0, "SDdiminfo");

    /* Set the first dimension name to DIM2_NAME */
    status = SDsetdimname(dim0_id, DIM2_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Verify new name. */
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(strcmp(dim_name, DIM2_NAME), 0, "SDdiminfo");
    VERIFY(size, dims[0], "SDdiminfo");
    VERIFY(dim_data_type, 0, "SDdiminfo");
    VERIFY(dim_num_attrs, 0, "SDdiminfo");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "test_dimensions: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;

} /* test_dim_basics */


/********************************************************************
   Name: test_dim_scales()

   Description: 
	This test routine is used to test dimension scale operations.
	The main contents include:
	- creates SDS #1 of size LENGTH0xLENGTH1, and sets values to its dims
	- creates SDS #2 of size LENGTH2xLENGTH3, and sets values to its 
		first dimension

	The followings are included in this test routine:
	- SDgetdimscale
        - SDsetdimscale with the following situations:  (NOT all done yet)
        	+ not called before SDdiminfo 
		+ called with number type=0 -> type is then set to DFNT_FLOAT32
        	+ called with an unsigned number type
 		+ called with the dataset's number type 
 		+ called before writing data to dataset
 		+ called after closing dataset and file, and then 
		  reopening

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
#define	SCALES_FILE	"scaletst.hdf"	/* file to test dimension scales */

static intn test_dim_scales()
{
    int32  fid, sds_id, status, dim0_id, dim1_id, sds_idx;
    int32  dims[2], start[2], edges[2], rank;
    int16  array1_data[LENGTH0][LENGTH1];	/* data for first SDS */
    uint32 array2_data[LENGTH2][LENGTH3];	/* data for second SDS */
    int32  dim_sizes[H4_MAX_VAR_DIMS];		/* read dimensions */
    intn   i, j;
    int32  array_rank, num_type, attributes;
    char   dim_name[H4_MAX_NC_NAME], name[H4_MAX_NC_NAME];
    uint8  scale0 [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,255};
    int16  scale1 [] = {0,1,2,3,4,5,6,7,8,9};
    char8  scale2 [] = {'d','i','m','2'}, scale2_out[4];
    float32 scalef[] = {1., 2., 3., 4.};
    int32  size, dim_data_type, dim_num_attrs;
    int    num_errs = 0;    /* number of errors so far */

    /* Create the file defined by SCALES_FILE and initiate the SD interface. */
    fid = SDstart(SCALES_FILE, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /*
     * Add a LENGTH0 x LENGTH1 array, which is named by DS0_NAME and of
     * type 16-bit signed integer, to the file... 
     */

    /* Define the rank and dimensions of the data set. */
    rank = RANK2;
    dims[0] = LENGTH0;
    dims[1] = LENGTH1;

    /* Create the array data set. */
    sds_id = SDcreate(fid, DS0_NAME, DFNT_INT16, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to DIM0_NAME */
    status = SDsetdimname(dim0_id, DIM0_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Set scale of type unsigned int-8 for the first dimension */
    status = SDsetdimscale(dim0_id, dims[0], DFNT_UINT8, scale0);
    CHECK(status, FAIL, "SDsetdimscale");

    /* 
     * Write the data to the dataset...
     */

    /* Fill the buffer with values. */
    for (j = 0; j < LENGTH0; j++) {
        for (i = 0; i < LENGTH1; i++)
            array1_data[j][i] = (i + j) + 1;
    }
    /* Define the area for writing to the dataset */
    for (i = 0; i < rank; i++) {
        start[i] = 0;
        edges[i] = dims[i];
    }
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array1_data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open SCALES_FILE in W mode */
    fid = SDstart(SCALES_FILE, DFACC_WRITE);
    CHECK(fid, FAIL, "SDstart");

    /* Look for DS0_NAME dataset */
    sds_idx = SDnametoindex(fid, DS0_NAME);
    CHECK(sds_idx, FAIL, "SDnametoindex");

    /* Select that dataset */
    sds_id = SDselect(fid, sds_idx);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get info of the dataset and verify its dimensions */
    status = SDgetinfo(sds_id, NULL, &array_rank, dim_sizes, &num_type, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(array_rank, rank, "SDgetinfo");
    VERIFY(num_type, DFNT_INT16, "SDgetinfo");
    VERIFY(dim_sizes[0], LENGTH0, "SDgetinfo");
    VERIFY(dim_sizes[1], LENGTH1, "SDgetinfo");

    /*
     * Check dimension's number type before setting scales
     */

    /* Get the second dimension id */
    dim1_id = SDgetdimid(sds_id, 1);
    CHECK(dim1_id, FAIL, "SDgetdimid");

    /* Set the second dimension name to DIM1_NAME */
    status = SDsetdimname(dim1_id, DIM1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Read the second dimension and verify its information; since the
       scale of this dimension is not set yet, the number type should be 0 */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(dim_data_type, 0, "SDdiminfo");

    /* Set dimension scale for the second dimension; its type will be the
       same as that of the dataset */
    status = SDsetdimscale(dim1_id, dim_sizes[1], num_type, scale1);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the second dimension and verify its number type; since the
       scale of this dimension is now set, the number type should be the
       same as its dataset, i.e. num_type above */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(dim_data_type, num_type, "SDdiminfo");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Add another dataset to the file; this dataset is a LENGTH2 x LENGTH3
     * array, named by DS1_NAME and of type 32-bit unsigned integer...
     */

    /* Define the rank and dimensions of the data set */
    rank = RANK2;
    dims[0] = LENGTH2;
    dims[1] = LENGTH3;

    for (i = 0; i < rank; i++)
        edges[i] = dims[i];

    /* Create the data set */
    sds_id = SDcreate(fid, DS1_NAME, DFNT_UINT32, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Fill the stored-data array with values. */
    for (j = 0; j < LENGTH2; j++) {
        for (i = 0; i < LENGTH3; i++)
            array2_data[j][i] = 10 * (j + i);
    }

    /* Write the data stored in the array 'array2_data' to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array2_data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Get info of the dataset and verify its type */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(strcmp(name, DS1_NAME), 0, "SDgetinfo");
    VERIFY(array_rank, rank, "SDgetinfo");
    VERIFY(num_type, DFNT_UINT32, "SDgetinfo");
    VERIFY(dim_sizes[0], LENGTH2, "SDgetinfo");
    VERIFY(dim_sizes[1], LENGTH3, "SDgetinfo");

    /*
     * Verify that when 0 is passed into SDsetdimscale for number type, then
     * the number type of the dimension scale will be set to DFNT_FLOAT32.
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to DIM2_NAME */
    status = SDsetdimname(dim0_id, DIM2_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Pass 0 into SDsetdimscale for number type */
    status = SDsetdimscale(dim0_id, dims[0], 0, scalef); 
    CHECK(status, FAIL, "SDsetdimscale");

    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(strcmp(dim_name, DIM2_NAME), 0, "SDdiminfo");
    VERIFY(dim_data_type, DFNT_FLOAT32, "SDdiminfo");

    /* 
     * Now, set dimension scale for that first dimension again, but this
     * time, to 8-bit signed char; then verify the type and the scale values 
     */

    /* Set scale of type 8-bit signed char for this dimension */
    status = SDsetdimscale(dim0_id, dims[0], DFNT_CHAR, scale2);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the info of this dimension and verify its new number type */ 
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(dim_data_type, DFNT_CHAR, "SDdiminfo");

    /* Read dimension scale values and verify them */
    status = SDgetdimscale(dim0_id, (VOIDP)scale2_out);
    CHECK(status, FAIL, "SDgetdimscale");
    for(i=0; i < LENGTH2; i++)
	VERIFY(scale2_out[i], scale2[i], "SDgetdimscale");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "test_dimensions: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;

} /* test_dim_scales */

/********************************************************************
   Name: test_dim_strs()

   Description: 
	This test routine is used to test SDsetdimstrs and SDgetdimstrs.i
	It uses BASIC_FILE.
	The main contents include:
	- creates SDS #3 of size LENGTH0xLENGTH1 and named DS2_NAME
	- then sets/gets all predefined attributes to/from first dimension
	- tries to get predefined attributes from second dimension,
	  should get '\0' as first character in the parameters
	- now sets just 1 predefined attribute to second dimension then
	  tries to call SDgetdimstrs again
	- creates SDS #4 of size LENGTH4 x LENGTH5 and named DS3_NAME
	- set dim scales to the first dimension, named DIM3_NAME
	- now, test that SDgetdimstrs returns strings containing '\0' for
	  this coordinate variable because SDsetdimstrs had not been called
	  on it.

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
#define LENGTH4 4
#define LENGTH5 5
#define RANK4 	4
#define DS2_NAME "HDF Data 2"	/* SDS #3 name */
#define DS3_NAME "HDF Data 3"	/* SDS #4 name */
#define LABEL	"DimLabel"
#define UNITS	"Units"
#define FORMAT	"TheFormat"
#define DIMSTRS_LEN	20	/* length of the dimension string buffers */
static intn test_dim_strs()
{
    int32  fid, sds_id, status, dim0_id, dim1_id, sds_idx, coord_var_id;
    int32  dims[2], start[2], edges[2], rank;
    int16  array1_data[LENGTH0][LENGTH1];	/* data for first SDS */
    uint32 array2_data[LENGTH2][LENGTH3];	/* data for second SDS */
    int32  dim_sizes[H4_MAX_VAR_DIMS];		/* read dimensions */
    intn   i, j;
    int32  array_rank, num_type, attributes;
    char   dim_name[H4_MAX_NC_NAME], name[H4_MAX_NC_NAME];
    uint8  scale0 [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,255};
    int16  scale1 [] = {0,1,2,3,4,5,6,7,8,9};
    char8  scale2 [] = {'d','i','m','2'}, scale2_out[4];
    float32 scalef[] = {1., 2., 3., 4.};
    int32  size, dim_data_type, dim_num_attrs;
    char   label[DIMSTRS_LEN], unit[DIMSTRS_LEN], format[DIMSTRS_LEN];
    int    num_errs = 0;    /* number of errors so far */

    /* Open the file defined by BASIC_FILE and initiate the SD interface. */
    fid = SDstart(BASIC_FILE, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /*
     * Add a LENGTH0 x LENGTH1 array, which is named by DS2_NAME and of
     * type 16-bit signed integer, to the file... 
     */

    /* Define the rank and dimensions of the data set. */
    rank = RANK2;
    dims[0] = LENGTH0;
    dims[1] = LENGTH1;

    /* Create the array data set. */
    sds_id = SDcreate(fid, DS2_NAME, DFNT_INT16, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /*
     * Settings predefined attributes for the first dimension.
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the dimension strings for the dimension */
    status = SDsetdimstrs(dim0_id, LABEL, UNITS, FORMAT);
    CHECK(status, FAIL, "SDsetdimstrs");

    /* Read the first dimension and verify its information */
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(size, dims[0], "SDdiminfo");
    VERIFY(dim_data_type, DFNT_NONE, "SDdiminfo"); /* number type not set */
    VERIFY(dim_num_attrs, 3, "SDdiminfo");

    /* 
     * Write the data stored in the array 'array1_data' to the dataset...
     */

    /* Fill the buffer with values. */
    for (j = 0; j < LENGTH0; j++) {
        for (i = 0; i < LENGTH1; i++)
            array1_data[j][i] = (i + j) + 1;
    }

    /* Define the area for writing to the dataset */
    for (i = 0; i < rank; i++) {
        start[i] = 0;
        edges[i] = dims[i];
    }

    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array1_data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open BASIC_FILE in W mode and select the dataset named by DS0_NAME */
    fid = SDstart(BASIC_FILE, DFACC_WRITE);
    CHECK(fid, FAIL, "SDstart");

    /* Look for DS0_NAME dataset */
    sds_idx = SDnametoindex(fid, DS2_NAME);
    CHECK(sds_idx, FAIL, "SDnametoindex");

    /* Select that dataset */
    sds_id = SDselect(fid, sds_idx);
    CHECK(sds_id, FAIL, "SDselect");

    /*
     * Verify predefined attributes of the first dimension of this dataset.
     * They were set previously.
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Get and verify the predefined attributes label, unit, and format */
    status = SDgetdimstrs(dim0_id, label, unit, format, DIMSTRS_LEN);
    CHECK(status, FAIL, "SDgetdimstrs");
    VERIFY_CHAR(label, LABEL, "SDgetdimstrs");
    VERIFY_CHAR(unit, UNITS, "SDgetdimstrs");
    VERIFY_CHAR(format, FORMAT, "SDgetdimstrs");

    /*
     * Verify that the second dimension of SDS DS2_NAME does not have
     * predefined attributes 
     */

    /* Get the second dimension id */
    dim1_id = SDgetdimid(sds_id, 1);
    CHECK(dim1_id, FAIL, "SDgetdimid");

    /* Get and verify that predefined attributes label, unit, and format 
       are not assigned to this dimension */
    HDmemset(label, 0, DIMSTRS_LEN);
    HDmemset(unit, 0, DIMSTRS_LEN);
    HDmemset(format, 0, DIMSTRS_LEN);
    status = SDgetdimstrs(dim1_id, label, unit, format, DIMSTRS_LEN);
    CHECK(status, FAIL, "SDgetdimstrs");
    VERIFY(label[0], '\0', "SDgetdimstrs");
    VERIFY(unit[0], '\0', "SDgetdimstrs");
    VERIFY(format[0], '\0', "SDgetdimstrs");

    /* Assign predefined attribute UNITS to this dimension and check again. */
    status = SDsetdimstrs(dim1_id, NULL, UNITS, NULL);
    CHECK(status, FAIL, "SDsetdimstrs");

    /* Get and verify that only the predefined attributes unit is assigned
       to this dimension */
    status = SDgetdimstrs(dim1_id, label, unit, format, DIMSTRS_LEN);
    CHECK(status, FAIL, "SDgetdimstrs");
    VERIFY(label[0], '\0', "SDgetdimstrs");
    VERIFY_CHAR(unit, UNITS, "SDgetdimstrs");
    VERIFY(format[0], '\0', "SDgetdimstrs");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Test that a dimension is a coordinate variable but does not have
     * predefined attributes Label, Unit, or Format assigned to it.
     */

    /*
     * Add another dataset to the file; this dataset is a LENGTH4 x LENGTH5
     * array, named by DS3_NAME and of type 32-bit unsigned integer...
     */

    /* Define the rank and dimensions of the data set */
    rank = RANK2;
    dims[0] = LENGTH4;
    dims[1] = LENGTH5;

    for (i = 0; i < rank; i++)
        edges[i] = dims[i];

    /* Create the data set */
    sds_id = SDcreate(fid, DS3_NAME, DFNT_UINT32, rank, dims);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Fill the stored-data array with values. */
    for (j = 0; j < LENGTH2; j++) {
        for (i = 0; i < LENGTH3; i++)
            array2_data[j][i] = 10 * (j + i);
    }

    /* Write the data stored in the array 'array2_data' to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)array2_data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Get the first dimension id */
    dim0_id = SDgetdimid(sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to the name defined by DIM3_NAME */
    status = SDsetdimname(dim0_id, DIM3_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Set dimension scale for that first dimension.  It is now should be
       a coordinate variable. */
    status = SDsetdimscale(dim0_id, dims[0], DFNT_CHAR, scale2);
    CHECK(status, FAIL, "SDsetdimscale");

    /*
     * Get access to that coordinate variable and verify its information
     */
    /* Look for DIM3_NAME dataset */
    sds_idx = SDnametoindex(fid, DIM3_NAME);
    CHECK(sds_idx, FAIL, "SDnametoindex");

    /* Select that coordinate variable */
    coord_var_id = SDselect(fid, sds_idx);
    CHECK(coord_var_id, FAIL, "SDselect");

    /* Get info of the coord var and verify them: it is a LENGTH4 x LENGTH5 
       array of type DFNT_CHAR and is named by DIM3_NAME */
    status = SDgetinfo(coord_var_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(strcmp(name, DIM3_NAME), 0, "SDgetinfo");
    VERIFY(array_rank, 1, "SDgetinfo");
    VERIFY(num_type, DFNT_CHAR, "SDgetinfo");
    VERIFY(dim_sizes[0], dims[0], "SDgetinfo");

    /* Terminate access to the array. */
    status = SDendaccess(coord_var_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Get and verify that predefined attributes label, unit, and format 
       are not assigned to this dimension */
    HDmemset(label, 0, DIMSTRS_LEN);
    HDmemset(unit, 0, DIMSTRS_LEN);
    HDmemset(format, 0, DIMSTRS_LEN);
    status = SDgetdimstrs(dim0_id, label, unit, format, DIMSTRS_LEN);
    CHECK(status, FAIL, "SDgetdimstrs");
    VERIFY(label[0], '\0', "SDgetdimstrs");
    VERIFY(unit[0], '\0', "SDgetdimstrs");
    VERIFY(format[0], '\0', "SDgetdimstrs");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "test_dimensions: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;

} /* test_dim_strs */

/* Test driver for testing dimension functionality */
extern int test_dimensions()
{
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("functionality of dimensions (tdim.c)");

    /* Test basic dimension functionality */
    num_errs = num_errs + test_basic_dim();

    /* Test dimension scale functionality */
    num_errs = num_errs + test_dim_scales();

    /* Test SD[set/get]dimstrs */
    num_errs = num_errs + test_dim_strs();

    if (num_errs == 0) PASSED();
    return num_errs;
}
    
#endif /* HDF */
