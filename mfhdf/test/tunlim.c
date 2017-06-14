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
 * tunlim.c - tests reading/writing variables with unlimited dimension using
 *		SD API.  The tests here are added to test the behavior of the
 *		library when dealing with unlimited dimensions with nc API.  The
 *		tests show that the data is correctly written when:
 *		+ data added immediately after last record
 *		+ data added skipping one or more records
 *		+ data overridden existing data
 *		+ data read pass the end of that variable but not the max in 
 *		  all the variables in the file
 *
 * Structure of the file:
 *    test_unlim - test driver
 *        test_1dim_singlevar - tests on a single variable with only 1 dimension
 *        test_1dim_multivars - tests on multiple variables with only 1 dimension
 *        test_multidim_singlevar - tests on single variable with multiple
 *		dimensions
 *	  test_1dim_multivars_addon - tests multiple 1-dim variables that were
 *		added on to existing file
 *
 * BMR - Dec 30, 2008
****************************************************************************/

#include "mfhdf.h"
#include "hdftest.h"

#define DIM0 20

/********************************************************************
   Name: verify_info_data() - utility function that verifies the unlimited
		dimension's size and the variable's data.

   Description:
	This utility function carries out the following tasks:
        - SDgetinfo to get the size of the unlimited dimension
	- SDreaddata to read the variable up to the unlimited dimension's size,
	  that is the max number of records in the file
	- HDmemcmp to verify that the read buffer is as expected

	It is used by only test_1dim_singlevar, test_1dim_multivars, and 
	test_1dim_multivars_addon because it only handles 1-dim dataset and
	number type int16.  It can be revised for multi-dim datasets or 
	any number types, if desired.

   Return value:
        The number of errors occurred in this routine.
   BMR - Dec 30, 2008

   Note: This function will be moved to tutils.c; must decide what
	 to do about the data type and sizes, so that the approach
	 will be consistent within the test suite.  Right now, test
	 tncvargetfill.c is using it from here.
   BMR - Sep 4, 2013
*********************************************************************/

int verify_info_data(
	int32 sds_id,
	int32 expected_dimsize,
	int16 *result)
{
    int32 dimsizes[1];		/* dimension size buffer */
    int32 start[1],		/* where to start reading */
	  edges[1];		/* length of data to be read/written */
    char  info[40];		/* holds brief info where failure occurs */
    int16 outdata[DIM0];	/* data read back */
    char  ds_name[20];		/* dataset name */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors */

    /* Get information of the first dataset, and verify its dimension */
    status = SDgetinfo(sds_id, ds_name, NULL, dimsizes, NULL, NULL);
    sprintf(info, "%s on <%s>", "SDgetinfo", ds_name);
    CHECK(status, FAIL, info);
    VERIFY(dimsizes[0], expected_dimsize, info);

    /* Read back data and verify */
    start[0] = 0;
    edges[0] = dimsizes[0];
    status = SDreaddata(sds_id, start, NULL, edges, (VOIDP) outdata);
    sprintf(info, "%s on <%s>", "SDreaddata", ds_name);
    CHECK(status, FAIL, info);

    /* Verify read data by comparing the output buffer against expected data */
    status = HDmemcmp(outdata, result, edges[0] * SIZE_INT16);
    if (status != 0)
	fprintf(stderr,"For SDS %s: Read data doesn't match input\n", ds_name);

    /* Return the number of errors occurred here */
    return(num_errs);
}

/********************************************************************
   Name: test_1dim_singlevar() - tests on a single variable with only 1 dimension

   Description:
        The main contents include:
        - write 4 elements starting at index 0
        - try reading 2 elements pass the end, should fail
        - append 2 elements starting at index 6, that is indices 4 and 5 will
	  be written with fill value
        - append 3 elements immediately at the end of the data
        - overwrite indices 0 and 1

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 30, 2008
*********************************************************************/

#define FILENAME1 "onedimonevar.nc"
static int test_1dim_singlevar()
{
    int32 fid;			/* file id */
    int32 dset1;		/* dataset ids */
    int32 dimsizes[1];		/* dimension size buffer */
    int32 start[1],		/* where to start writing */
	  edges[1];		/* length of data to be read/written */
    int16 fillval = 99;		/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data; the first two elements will
	be changed to "1,2" later for the last test. */
    int16 result[] = {300,301,302,303,99,99,30,31,801,802,803};

    /* enter define mode */
    fid = SDstart(FILENAME1, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define dimension of the data set to be created. */
    dimsizes[0] = SD_UNLIMITED;

    /* Create an array data sets. */
    dset1 = SDcreate(fid, "Variable 1", DFNT_INT16, 1, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data */
	int16 data[] = {300, 301, 302, 303};

	/* Write 4 elements starting at index 0 */
	start[0] = 0;
	edges[0] = 4;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its data and dim size */
    num_errs = num_errs + verify_info_data(dset1, 4, result);

    { /* Append data to dataset pass the end */
	int16 data[] = {30, 31};

	/* Write 2 values starting at index 6 */
	start[0] = 6;
	edges[0] = 2;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    num_errs = num_errs + verify_info_data(dset1, 8, result);

    { /* Append data to dataset at the end */
	int16 data[] = {801,802,803};

	/* Write 3 values starting at index 8, at the end */
	start[0] = 8;
	edges[0] = 3;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    num_errs = num_errs + verify_info_data(dset1, 11, result);

    { /* Override the first 2 elements */
	int16 data[] = {1,2};

	/* Write 2 values starting at 0 */
	start[0] = 0;
	edges[0] = 2;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Modify the first 2 elements of the result buffer appropriately */
    result[0] = 1;
    result[1] = 2;

    /* Get information of the dataset, and verify its dimension */
    num_errs = num_errs + verify_info_data(dset1, 11, result);

    /* Close the dataset */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    return 0;
}

/********************************************************************
   Name: test_1dim_multivars() - tests reading/writing on multiple variables,
	each has 1 dimension

   Description:
        The main contents include:
        - write 4 elements starting at index 0 to variable #1
        - write 2 elements starting at index 0 to variable #2
	- close file and reopen for read/write
	- append 2 elements to variable #1, immediately after last written record
	- write 3 elements to variable #2, starting at index 4
	- append 3 elements to variable #1, skipping 5 records after the last
	  written record

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 22, 2008
*********************************************************************/

#define FILENAME2 "onedimmultivars.nc"
static int test_1dim_multivars()
{
    int32 fid;			/* file id */
    int32 dset1, dset2;		/* dataset ids */
    int32 dimsizes[1];		/* dimension size buffer */
    int32 start[1],		/* where to start writing */
	  edges[1];		/* length of data to be read/written */
    int16 outdata[DIM0];	/* data read back */
    int16 fillval1 = -1;	/* fill value for the variable */
    int16 fillval2 = -2;	/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */
    int16 result1[] = {300,301,302,303,-1,-1,400,401,500,501,502};
    int16 result2[] = {102,104,-2,-2,-2,-2,-2,-2,200,201,202,800,801,802,803};

    /* enter define mode */
    fid = SDstart(FILENAME2, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define dimension of the data set to be created. */
    dimsizes[0] = SD_UNLIMITED;

    /* Create two array data sets. */
    dset1 = SDcreate(fid, "Variable 1", DFNT_INT16, 1, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");
    dset2 = SDcreate(fid, "Variable 2", DFNT_INT16, 1, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval1);
    CHECK(status, FAIL, "SDsetfillvalue");
    status = SDsetfillvalue(dset2, (VOIDP)&fillval2);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data */
	int16 data[] = {300, 301, 302, 303};
	int16 data2[] = {102, 104};

	/* Write 4 elements to first dataset starting at index 0 */
	start[0] = 0;
	edges[0] = 4;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");

	/* Write 2 elements to second dataset starting at index 0 */
	edges[0] = 2;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data2);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the first dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset1, 4, result1);

    /* Get information of the second dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset2, 2, result2);

    /* Try reading pass the end, should fail */
    edges[0] = 6;
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata);
    VERIFY(status, FAIL, "SDreaddata");

    { /* Append data to first dataset pass the end */
	int16 data[] = {400, 401};

	/* Write 2 values starting at index 6 */
	start[0] = 6;
	edges[0] = 2;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the first dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset1, 8, result1);

    /* Get information of the second dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset2, 2, result2);

    { /* Append data to first dataset at the end */
	int16 data[] = {500,501,502};

	/* Write 3 values starting at index 8 */
	start[0] = 8;
	edges[0] = 3;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the first dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset1, 11, result1);

    { /* Append data to second dataset pass its end */
	int16 data[] = {200, 201, 202};

	/* Write 2 values starting at index 8 */
	start[0] = 8;
	edges[0] = 3;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the second dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset2, 11, result2);

    { /* Append data to second dataset at its end */
	int16 data[] = {800, 801, 802, 803};

	/* Write 2 values starting at index 11 */
	start[0] = 11;
	edges[0] = 4;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the first dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset1, 11, result1);

    /* Get information of the second dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset2, 15, result2);

    /* Close the dataset */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    return 0;
}


/********************************************************************
   Name: test_multidim_singlevar() - tests on a single variable with more
	than one dimension

   Description:
        The main contents include:
	- create a 3-D variable with 1 unlimited dimension and the others 3x2
        - write 4x1x1 elements starting at index {0,0,0}
        - append 2x1x1 elements along the unlimited dimension starting at 
	  index 6, that is slabs at indices 4 and 5 will be written with fill 
	  value
        - append 1x3x2 elements immediately at the end of the data

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 22, 2008
*********************************************************************/

#define DIM00 10
#define DIM1 3
#define DIM2 2
#define FILENAME3 "multidimvar.nc"
static int test_multidim_singlevar()
{
    int32 fid;			/* file id */
    int32 dset1;		/* dataset id */
    int32 dset_index;		/* dataset index */
    int32 dimsizes[3];		/* dimension size buffer */
    int32 start[3],		/* where to start reading */
	  edges[3];		/* length of data to be read */
    int16 outdata[DIM0][DIM1][DIM2];	/* data read back */
    int16 fillval = -3;		/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */
    int16 result[DIM00][DIM1][DIM2] = {300,-3,-3,-3,-3,-3,301,-3,-3,-3,-3,-3,302,-3,-3,-3,-3,-3,303,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,400,-3,-3,-3,-3,-3,401,-3,-3,-3,-3,-3,800,801,802,803,804,805};

    /* result data to compare against read data; the first two elements will
	be changed to "1,2" later for the last test. */

    /* enter define mode */
    fid = SDstart(FILENAME3, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define dimension of the data set to be created. */
    dimsizes[0] = SD_UNLIMITED;
    dimsizes[1] = DIM1;
    dimsizes[2] = DIM2;

    /* Create an array data sets. */
    dset1 = SDcreate(fid, "Variable 3D", DFNT_INT16, 3, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data */
	int16 data[] = {300, 301, 302, 303};
	int32 startw[3], edgesw[3];

	/* Write 4 elements starting at index 0 */
	startw[0] = startw[1] = startw[2] = 0; /* starting writing at {0,0,0} */
	edgesw[0] = 4;	/* 4x1x1 slab */
	edgesw[1] = 1;
	edgesw[2] = 1;
	status = SDwritedata(dset1, startw, NULL, edgesw, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    status = SDgetinfo(dset1, NULL, NULL, dimsizes, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(dimsizes[0], 4, "SDgetinfo");

    /* Read and verify data of the dataset */
    start[0] = start[1] = start[2] = 0;
    edges[0] = dimsizes[0];
    edges[1] = DIM1;
    edges[2] = DIM2;
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata, result, edges[0]*DIM1*DIM2*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

    /* Close the dataset */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Reopen file and dataset */
    fid = SDstart(FILENAME3, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");
    dset_index = SDnametoindex(fid, "Variable 3D");
    CHECK(dset_index, FAIL, "SDnametoindex");
    dset1 = SDselect(fid, dset_index);
    CHECK(dset1, FAIL, "SDselect");

    { /* Append data to the dataset pass the end */
	int16 data[] = {400, 401};
	int32 startw[3], edgesw[3];

	startw[0] = 6;	/* the end is at 3 */
	startw[1] = startw[2] = 0; /* writing at {6,0,0} */
	edgesw[0] = 2;	/* two slabs */
	edgesw[1] = edgesw[2] = 1; 

	/* Write 2 slabs starting at index 6 */
	status = SDwritedata(dset1, startw, NULL, edgesw, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    status = SDgetinfo(dset1, NULL, NULL, dimsizes, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(dimsizes[0], 8, "SDgetinfo");

    /* Read and verify data of the dataset */
    /* start[], edges[1], and edges[2] same as last reading */
    edges[0] = dimsizes[0];
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata, result, edges[0]*DIM1*DIM2*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

    { /* Append data to the dataset at the end */
	int16 data[] = {800,801,802,803,804,805};
	int32 startw[3], edgesw[3];

	startw[0] = dimsizes[0];	/* writing at the end */
	startw[1] = startw[2] = 0; /* writing at {8,0,0} */
	edgesw[0] = 1;	/* 1 slab */
	edgesw[1] = DIM1;
	edgesw[2] = DIM2; 

	/* Write 2 slabs starting at index 8 */
	status = SDwritedata(dset1, startw, NULL, edgesw, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    status = SDgetinfo(dset1, NULL, NULL, dimsizes, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(dimsizes[0], 9, "SDgetinfo");

    /* Read and verify data of the dataset */
    /* start[], edges[1], and edges[2] same as last reading */
    edges[0] = dimsizes[0];
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata, result, edges[0]*DIM1*DIM2*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

	/* data should be
		300  -3     301  -3     302  -3     303  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...

		... -3  -3      -3  -3     400  -3     401  -3     800 801
		... -3  -3      -3  -3      -3  -3      -3  -3     802 803
		... -3  -3      -3  -3      -3  -3      -3  -3     804 805
		unlimited dimension ---->
		(-3 is fill value)
	*/

    /* Close the dataset */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    return 0;
}


/********************************************************************
   Name: test_1dim_multivars_addon() - tests adding unlimited dimension
		dataset to an existing file

   Description:
        The main contents include:
	- open FILENAME2
	- create a 1-dim unlimited dimension dataset
	- close the file, then reopen it
	- open the previous dataset and create another one
        - write 4 elements to first and 3 to second, starting at index 0
        - append 2 elements to first dataset, starting at index 6, that 
	  is passed the end
        - append 3 elements to first dataset, immediately at the end of
	  the data
	- append 3 elements to second dataset, skipping 5 elements
	- verify data in various places

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 30, 2008
*********************************************************************/

static int test_1dim_multivars_addon()
{
    int32 fid;			/* file id */
    int32 dset1, dset2;		/* dataset ids */
    int32 dset_index;		/* dataset index */
    int32 dimsizes[1];		/* dimension size buffer */
    int32 start[1],		/* where to start writing */
	  edges[1];		/* length of data to be read/written */
    int16 fillval = -3;		/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */
    int16 result3[] = {300,301,302,303,-3,-3,30,31,801,802,803};
    int16 result4[] = {300,301,302,-3,-3,-3,-3,-3,801,802,803};

    /* enter define mode */
    fid = SDstart(FILENAME2, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /* Define dimension of the data set to be created. */
    dimsizes[0] = SD_UNLIMITED;

    /* Create an array data sets. */
    dset1 = SDcreate(fid, "Variable 3", DFNT_INT16, 1, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Close dataset and file */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Reopen file and dataset */
    fid = SDstart(FILENAME2, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");
    dset_index = SDnametoindex(fid, "Variable 3");
    CHECK(dset_index, FAIL, "SDnametoindex");
    dset1 = SDselect(fid, dset_index);
    CHECK(dset1, FAIL, "SDselect");

    /* Create another dataset */
    dset2 = SDcreate(fid, "Variable 4", DFNT_INT16, 1, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset2, (VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data */
	int16 data[] = {300, 301, 302, 303};

	/* Write 4 elements to "Variable 3" starting at index 0 */
	start[0] = 0;
	edges[0] = 4;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");

	/* Write 3 elements to "Variable 4" starting at index 0 */
	edges[0] = 3;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the datasets, and verify their data and dim sizes */
    num_errs = num_errs + verify_info_data(dset1, 4, result3);
    num_errs = num_errs + verify_info_data(dset2, 3, result4);

    { /* Append data to "Variable 3" pass the end */
	int16 data[] = {30, 31};

	/* Write 2 values starting at index 6, skipping indices 4 and 5 */
	start[0] = 6;
	edges[0] = 2;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its data and dim size */
    num_errs = num_errs + verify_info_data(dset1, 8, result3);

    { /* Append data to "Variable 3" again, but right at the end */
	int16 data[] = {801,802,803};

	/* Write 3 values starting at index 8, at the end */
	start[0] = 8;
	edges[0] = 3;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of "Variable 3", and verify its dimension */
    num_errs = num_errs + verify_info_data(dset1, 11, result3);

    { /* Append data to "Variable 4" at an arbitrary index, skipping a few */
	int16 data[] = {801,802,803};

	/* Write 3 values starting at index 8, at the end */
	start[0] = 8;
	edges[0] = 3;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the "Variable 4" and verify its data and dim size */
    num_errs = num_errs + verify_info_data(dset2, 11, result4);

    { /* Read and verify data from dataset "Variable 2" that was created in
	 test_1dim_multivars */
	int32 dset;
	int16 result2[] = {102,104,-2,-2,-2,-2,-2,-2,200,201,202,800,801,802,803};

	/* Get access to dataset "Variable 2" */
	dset_index = SDnametoindex(fid, "Variable 2");
	CHECK(dset_index, FAIL, "SDnametoindex");
	dset = SDselect(fid, dset_index);
	CHECK(dset, FAIL, "SDselect");

	/* Get its information and verify its data and dimension size */
	num_errs = num_errs + verify_info_data(dset, 15, result2);

	/* Close "Variable 2" */
	status = SDendaccess(dset);
	CHECK(status, FAIL, "SDendaccess");
    } /* end reading "Variable 2" */

    /* Close all datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    return 0;
} /* test_1dim_multivars_addon */


/* Test driver for testing reading/writing variables with unlimited dimension 
   using SD API. */
extern int test_unlim()
{
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("read/write datasets with unlimited dims via SD API (tunlim.c)");

    /* Test single variable with 1 unlimited dimension */
    num_errs = num_errs + test_1dim_singlevar();

    /* Test multiple variables with 1 unlimited dimension */
    num_errs = num_errs + test_1dim_multivars();

    /* Test multiple variables with multiple dimensions */
    num_errs = num_errs + test_multidim_singlevar();

    /* Test multiple variables created at different time */
    num_errs = num_errs + test_1dim_multivars_addon();

    if (num_errs == 0) PASSED();
    return num_errs;
}
