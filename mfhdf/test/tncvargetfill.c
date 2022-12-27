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
 * tncvargetfill.c - tests that ncvarget fills user buffers with fill-values
 *		where data is not present.
 *		The tests show that the fill-values are filled correctly in
 *		the following situations:
 *		+ reading passed written data of a variable but still less
 *		  than the maximum number of records in the file
 *		+ reading passed the maximum number of records in the file
 *
 * Structure of the file:
 *    test_vargetfill - test driver
 *        test_1dim_multivars - tests on multiple variables with only 1 dimension
 *        test_multidims      - tests on variables with multiple dimensions
 *	  test_readings       - tests reading passed written data and max number
 *				of records in the file.
 *
 * BMR - Aug, 2013
****************************************************************************/

#include "mfhdf.h"
#include "hdftest.h"

#define DIM0 20	/* this value is used to declare larger buffer for when
		   reading past the maximum number of records in the file */

/********************************************************************
   Name: test_1dim_multivars() - tests reading/writing on multiple variables,
	each has 1 dimension

   Description:
        The main contents include:
        - write 4 elements to var #1, starting at index 0
        - write 2 elements to var #2, starting at index 0
        - add 2 more elements to var #1, starting at idx 6, skipping 2 elems
	- close file and reopen for using nc API
	- read var #1, data should show fill-values are in the skipping elems
	- read var #2, data should show all fill-values after first 2 elems
	  written previously, until idx 6, which is the max numrecs in the file

   Return value:
        The number of errors occurred in this test.
   BMR - Sep 4, 2013
*********************************************************************/

#define FILENAME1 "onedimmultivars.hdf"
#define SDS1_NAME "Variable 1"
#define SDS2_NAME "Variable 2"
#define	D1_NUMRECS	8
#define	D2_NUMRECS	2
static int test_1dim_multivars()
{
    int32 fid;			/* file id */
    int32 dset1, dset2;		/* dataset ids */
    int32 dimsizes[1];		/* dimension size buffer */
    int32 start[1],		/* where to start writing */
	  edges[1];		/* length of data to be read/written */
    int32 rank = 1;		/* rank of the 1-D data sets */
    int16 outdata[DIM0];	/* data read back */
    int16 fillval1 = -1;	/* fill value for the variable */
    int16 fillval2 = -2;	/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */
    int16 sdresult1[] = {300,301,302,303,-1,-1,306,307};
    int16 sdresult2[] = {102,104};
    int16 ncresult1[] = {300,301,302,303,-1,-1,306,307}; /* same as sd result */
    int16 ncresult2[] = {102,104,-2,-2,-2,-2,-2,-2};

    /* Create a new file */
    fid = SDstart(FILENAME1, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define an unlimited dimension and rank for the data sets */
    dimsizes[0] = SD_UNLIMITED;
    rank = 1;

    /* Create two 1-D array data sets */
    dset1 = SDcreate(fid, SDS1_NAME, DFNT_INT16, rank, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");
    dset2 = SDcreate(fid, SDS2_NAME, DFNT_INT16, rank, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Data sets will be filled with fill-values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval1);
    CHECK(status, FAIL, "SDsetfillvalue");
    status = SDsetfillvalue(dset2, (VOIDP)&fillval2);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data */
	int16 data[] = {300, 301, 302, 303};
	int16 appdata[] = {306, 307};
	int16 data2[] = {102, 104};

	/* Write 4 elements to first data set starting at index 0 */
	start[0] = 0;
	edges[0] = 4;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");

	/* Write 2 elements to second data set starting at index 0 */
	start[0] = 0;
	edges[0] = 2;
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data2);
	CHECK(status, FAIL, "SDwritedata");

	/* Add 2 more elements to first dataset starting at index 6, i.e.,
	   skipping 2 elements, which will be filled with fill-values. */
	start[0] = 6;
	edges[0] = 2;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)appdata);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get info of the first data set, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset1, D1_NUMRECS, sdresult1);

    /* Get info of the second dataset, and verify its data and dimsize */
    num_errs = num_errs + verify_info_data(dset2, D2_NUMRECS, sdresult2);

    /* Close the data sets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /*****************************************************
     * Read and verify data of the variables with nc API *
     *****************************************************/
    {
	long start[1];		/* where to start reading */
	long edges[1];		/* length of data to be read */
	int   ncid;		/* file id */
	int   var1id, var2id;	/* variable ids */
	long  dimsize = 0;	/* dimension size buffer */
	short outdata[DIM0];	/* data read back */
	int   rh_ndims;		/* number of dims */
	int   rh_dims[H4_MAX_VAR_DIMS];	/* variable shape */
	char  varname[H4_MAX_NC_NAME];	/* variable name */
	int   ii;

	/* Open the file for reading and writing with nc API */
	ncid = ncopen(FILENAME1, NC_RDWR);
	CHECK(ncid, -1, "ncopen");

	/* Verify variable info and data of first variable */

	/* Get access to the first variable, named SDS1_NAME */
	var1id = ncvarid(ncid, SDS1_NAME);
	CHECK(var1id, -1, "ncvarid");

	/* Get and verify variable info and size of the unlim dimension again */
	status = ncvarinq(ncid, var1id, varname, NULL, &rh_ndims, rh_dims, NULL);
	CHECK(status, -1, "ncvarinq");
	status = ncdiminq(ncid, rh_dims[0], NULL, &dimsize);
	CHECK(status, -1, "ncdiminq");
	VERIFY(dimsize, D1_NUMRECS, "ncdiminq");

	/* Read the entire variable */
	start[0] = 0;
	edges[0] = dimsize;
	HDmemset(outdata, 0, edges[0]);
	status = ncvarget(ncid, var1id, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* Verify data, should be as ncresult1: 300 301 301 302 -1 -1 306 307 */
	for (ii = 0; ii < dimsize; ii++) {
  	    if (outdata[ii] != ncresult1[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d\n", outdata[ii], ncresult1[ii], ii);
	}

	/* Verify variable info and data of second variable */

	/* Get access to the second variable, named SDS2_NAME */
	var2id = ncvarid(ncid, SDS2_NAME);
	CHECK(var2id, -1, "ncvarid");

	/* Get and verify variable info and size of the unlim dimension again */
	status = ncvarinq(ncid, var2id, varname, NULL, &rh_ndims, rh_dims, NULL);
	CHECK(status, -1, "ncvarinq");
	status = ncdiminq(ncid, rh_dims[0], NULL, &dimsize);
	CHECK(status, -1, "ncdiminq");
	VERIFY(dimsize, D1_NUMRECS, "ncdiminq"); /* Note: only 2 written, but
		netCDF uses the max number of records in the file for all unlim
		dims in the file */

	start[0] = 0;
	edges[0] = dimsize;
	HDmemset(outdata, 0, edges[0]);
	status = ncvarget(ncid, var2id, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be as in ncresult2: "102 104 -2 -2 -2 -2" */
	for (ii = 0; ii < dimsize; ii++) {
  	    if (outdata[ii] != ncresult2[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d\n", outdata[ii], ncresult2[ii], ii);
	}
    } /* end read data */

    return 0;
}   /* test_1dim_multivars */


/********************************************************************
   Name: test_multidims() - tests on variables with more than one dimension

   Description:
        The main contents include:
	- create a 3-D variable, named VAR3D, with the first dimension
	  unlimited and the others have sizes 3x2
	- create 2 1-D variables, named VAR1D and VAREMPTY, both have UDim
        - write 4x1x1 elements to VAR3D, starting at index {0,0,0}
	- write 4 elements to VAR1D starting at index 2
	- close and reopen the file
        - append 1x3x2 elements along the unlimited dimension starting at 
	  index {7,0,0}, i.e., slabs at indices 4, 5 and 6 will be written with
	  fill-value
	- create another 1-D variable, named VARDOZEN, and write 12 values
	  to it -> 12 became the new max number of records in the file
	- close and reopen the file in nc API
	- read and verify VAR3D, its buffer should have fill-values between
	  elements 9-12
	- read and verify VAR1D, its buffer should have fill-values after the
	  6th element
	- read and verify VAREMPTY, its buffer should contain all 12 fill-values

   Return value:
        The number of errors occurred in this routine.
   BMR - Sep 4, 2013
*********************************************************************/

#define DIM00 10
#define DIM1 3
#define DIM2 2
#define RANK3 3
#define RANK1 1
#define MAX_NUMRECS 12
#define FILENAME2 "multidimmixed.hdf"
#define VAR3D "Variable 3D"
#define VAR1D "Variable 1D"
#define VAREMPTY "Variable EMPTY"
#define VARDOZEN "Variable Dozen Records"
static int test_multidims()
{
    int32 fid;			/* file id */
    int32 dset1, dset2, dset3;	/* dataset ids */
    int32 dset_index;		/* dataset index */
    int32 dimsizes3D[3];		/* dimension size buffer for first SDS */
    int32 dimsize1D[1];		/* dimension size buffer for second SDS */
    int32 start[3],		/* where to start reading */
	  edges[3];		/* length of data to be read */
    int16 outdata3D[DIM0][DIM1][DIM2];	/* 3-D data read back */
    int16 outdata1D[DIM0];	/* 1-D data read back */
    int16 *outdata3;		/* for dynamically allocated buffer */
    int16 fillval1 = -3;	/* fill value for the 3-D variable */
    int16 fillval2 = -1;	/* fill value for the 1-D variable */
    int16 fillval3 = -10;	/* fill value for the 1-D variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */
    int16 result3D[DIM00][DIM1][DIM2] = {300,-3,-3,-3,-3,-3,301,-3,-3,-3,-3,-3,302,-3,-3,-3,-3,-3,303,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,800,801,802,803,804,805};
    int16 ncresult1D[] = {-1,-1,300,301,302,303,-1,-1,-1};
    int16 sdresult1D[] = {-1,-1,300,301,302,303};

    /* Create a new file */
    fid = SDstart(FILENAME2, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define dimensions of the data sets to be created */
    dimsizes3D[0] = SD_UNLIMITED;
    dimsizes3D[1] = DIM1;
    dimsizes3D[2] = DIM2;
    dimsize1D[0] = SD_UNLIMITED;

    /* Create array data sets */
    dset1 = SDcreate(fid, VAR3D, DFNT_INT16, RANK3, dimsizes3D);	/* 3-D */
    CHECK(dset1, FAIL, "SDcreate");
    dset2 = SDcreate(fid, VAR1D, DFNT_INT16, RANK1, dimsize1D);	/* 1-D */
    CHECK(dset2, FAIL, "SDcreate");
    dset3 = SDcreate(fid, VAREMPTY, DFNT_INT16, RANK1, dimsize1D);	/* 1-D */
    CHECK(dset3, FAIL, "SDcreate");

    /* Datasets will be filled with fill values when data is missing */
    status = SDsetfillvalue(dset1, (VOIDP)&fillval1);
    CHECK(status, FAIL, "SDsetfillvalue");
    status = SDsetfillvalue(dset2, (VOIDP)&fillval2);
    CHECK(status, FAIL, "SDsetfillvalue");
    status = SDsetfillvalue(dset3, (VOIDP)&fillval3);
    CHECK(status, FAIL, "SDsetfillvalue");

    { /* Add data to first data set */
	int16 data[] = {300, 301, 302, 303};

	/* Write 4 elements starting at index 0 */
	start[0] = start[1] = start[2] = 0; /* starting writing at {0,0,0} */
	edges[0] = 4;	/* 4x1x1 slab */
	edges[1] = 1;
	edges[2] = 1;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Get information of the dataset, and verify its dimension */
    status = SDgetinfo(dset1, NULL, NULL, dimsizes3D, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(dimsizes3D[0], 4, "SDgetinfo");

    /* Read and verify data of the dataset */
    start[0] = start[1] = start[2] = 0;
    edges[0] = dimsizes3D[0];
    edges[1] = dimsizes3D[1];
    edges[2] = dimsizes3D[2];
    outdata3 = (int16 *)HDmalloc(edges[0]*edges[1]*edges[2]*sizeof(int16));
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata3);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata3, result3D, edges[0]*edges[1]*edges[2]*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

    { /* Add data to second data set, i.e. 1-D var */
	int16 data[] = {300, 301, 302, 303};

	/* Write 4 elements starting at index 2 */
	start[0] = 2;	/* skip first two elements */
	edges[0] = 4;	/* write 4 elements */
	status = SDwritedata(dset2, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Close the data sets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset3);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Reopen file and data set VAR3D */
    fid = SDstart(FILENAME2, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");
    dset_index = SDnametoindex(fid, VAR3D);
    CHECK(dset_index, FAIL, "SDnametoindex");
    dset1 = SDselect(fid, dset_index);
    CHECK(dset1, FAIL, "SDselect");

    { /* Append data to the dataset pass the end */
	int16 data[] = {800,801,802,803,804,805};

	start[0] = 7;
	start[1] = start[2] = 0; /* writing at {7,0,0} */
	edges[0] = 1;
	edges[1] = DIM1;
	edges[2] = DIM2; 

	/* Write 1 slab starting at index 7 */
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* VAR3D now has 8 (D1_NUMRECS) records */

    /* Get information of the dataset, and verify its dimension */
    status = SDgetinfo(dset1, NULL, NULL, dimsizes3D, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(dimsizes3D[0], D1_NUMRECS, "SDgetinfo");

    /* Read and verify data of the dataset */
    start[0] = start[1] = start[2] = 0;
    edges[0] = dimsizes3D[0];
    edges[1] = dimsizes3D[1];
    edges[2] = dimsizes3D[2];
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata3D);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata3D, result3D, edges[0]*edges[1]*edges[2]*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

	/* Data should be
		300  -3     301  -3     302  -3     303  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...

		... -3  -3      -3  -3      -3  -3      800 801
		... -3  -3      -3  -3      -3  -3      802 803
		... -3  -3      -3  -3      -3  -3      804 805
		unlimited dimension ---->
		(-3 is fill value)
	*/

    /* Reopen the 1-D dataset VAR1D */
    dset_index = SDnametoindex(fid, VAR1D);
    CHECK(dset_index, FAIL, "SDnametoindex");
    dset2 = SDselect(fid, dset_index);
    CHECK(dset2, FAIL, "SDselect");

    /* Read and verify data of the dataset.  Data should be: -1 -1 300 301 302 303 */
    start[0] = 0;
    edges[0] = dimsize1D[0];
    status = SDreaddata(dset2, start, NULL, edges, (VOIDP) outdata1D);
    CHECK(status, FAIL, "SDreaddata");

    status = HDmemcmp(outdata1D, sdresult1D, edges[0]*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Create the fourth data set, named VARDOZEN and with unlimited
       dimension.  This data set will have more records than the current
       number of records in the file. */
    dimsize1D[0] = SD_UNLIMITED;
    dset1 = SDcreate(fid, VARDOZEN, DFNT_INT16, RANK1, dimsize1D);
    CHECK(dset3, FAIL, "SDcreate");

    { /* Write data to the fourth dataset, exceeding the current number of
	 records in the file */
	int16 data[] = {1,2,3,4,5,6,7,8,9,10,11,12};

	/* Write 12 elements starting at index 0 */
	start[0] = 0;
	edges[0] = 12;
	status = SDwritedata(dset1, start, NULL, edges, (VOIDP)data);
	CHECK(status, FAIL, "SDwritedata");
    }

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");


    /*****************************************************
     * Read and verify data of the variables with nc API *
     *****************************************************/
    {
	long  start[3];
	long  edges[3];
	int   ncid;                 /* file id */
	int   var1id, var2id, var3id;       /* variable ids */
	long  dims[3];              /* dimension size buffer */
	int   rh_ndims;		/* number of dims */
	int   rh_dims[H4_MAX_VAR_DIMS];	/* variable shape */
	char  varname[H4_MAX_NC_NAME];	/* variable name */
	int   ii;

	/* After the fourth data set (VARDOZEN) was added, the maximum number
	   of records became 12.  Thus, the results must be changed to reflect
	   the behavior in nc API. */
    int16 result3D[DIM0][DIM1][DIM2] = {300,-3,-3,-3,-3,-3,301,-3,-3,-3,-3,-3,302,-3,-3,-3,-3,-3,303,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,800,801,802,803,804,805, -3,-3,-3,-3,-3,-3, -3,-3,-3,-3,-3,-3, -3,-3,-3,-3,-3,-3, -3,-3,-3,-3,-3,-3};
    int16 ncresult1D[] = {-1,-1,300,301,302,303,-1,-1,-1,-1,-1,-1};
    int16 ncresult1Ddozen[] = {-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10};
    int16 sdresult1D[] = {-1,-1,300,301,302,303};

	/* Open the file with nc API */
	ncid = ncopen(FILENAME2, NC_RDWR);
	CHECK(ncid, -1, "ncopen");

	/* Verify variable info and data of first data set */

	/* Get access to the variable VAR3D */
	var1id = ncvarid(ncid, VAR3D);
	CHECK(var1id, -1, "ncvarid");

	/* Get variable info */
	status = ncvarinq(ncid, var1id, varname, NULL, &rh_ndims, rh_dims, NULL);
	CHECK(status, -1, "ncvarinq");
	VERIFY(rh_ndims, RANK3, "ncvarinq");
	for (ii = 0; ii < rh_ndims; ii++)
	{
	    status = ncdiminq(ncid, rh_dims[ii], NULL, &dims[ii]);
	    CHECK(status, -1, "ncdiminq");

	    /* Initialize parameters for reading data */
	    edges[ii] = dims[ii];
	    start[ii] = 0;
	}

	/* Get data */
	HDmemset(outdata3D, 0, edges[0]*edges[1]*edges[2]*sizeof(int16));
	status = ncvarget(ncid, var1id, start, edges, outdata3D);
	CHECK(status, -1, "ncvarget");

	/* Verify data */
	status = HDmemcmp(outdata3D, result3D, edges[0]*edges[1]*edges[2]*sizeof(int16));
	VERIFY(status, 0, "HDmemcmp");

	/* Verify variable info and data of second data set */

	/* Get access to the second variable VAR1D */
	var2id = ncvarid(ncid, VAR1D);
	CHECK(var2id, -1, "ncvarid");

	/* Get and verify variable info */
	status = ncvarinq(ncid, var2id, varname, NULL, &rh_ndims, rh_dims, NULL);
	CHECK(status, -1, "ncvarinq");

	status = ncdiminq(ncid, rh_dims[0], NULL, &dims[0]);
	CHECK(status, -1, "ncdiminq");
	VERIFY(dims[0], MAX_NUMRECS, "ncdiminq"); /* Note: only written up
		to 6th element, but netCDF uses the max number of records in
		the file, which is currently 12, due to data set VARDOZEN */

	/* Get data */
	edges[0] = dims[0];
	HDmemset(outdata1D, 0, edges[0]*sizeof(int16));
	status = ncvarget(ncid, var2id, start, edges, outdata1D);
	CHECK(status, -1, "ncvarget");

	/* Verify data, should be "-1,-1,300,301,302,303,-1,-1,-1,-1,-1,-1".
	   The first two -1s are due to the skipped elements by SDwritedata.
	   The last six -1s are added by nc API to match the max numrecs in
	   the file. */
	for (ii = 0; ii < dims[0]; ii++) {
  	    if (outdata1D[ii] != ncresult1D[ii])
                fprintf(stderr,"test_1dims: at line %d- Read data %d doesn't match input %d at index %d\n", __LINE__, outdata1D[ii], ncresult1D[ii], ii);
	}

	/* Get access to the second variable */
	var3id = ncvarid(ncid, VAREMPTY);
	CHECK(var3id, -1, "ncvarid");

	/* Get and verify variable info */
	status = ncvarinq(ncid, var3id, varname, NULL, &rh_ndims, rh_dims, NULL);
	CHECK(status, -1, "ncvarinq");

	status = ncdiminq(ncid, rh_dims[0], NULL, &dims[0]);
	CHECK(status, -1, "ncdiminq");
	VERIFY(dims[0], MAX_NUMRECS, "ncdiminq");	/* Note: no data
		written, but netCDF uses the max number of records in the file
		for all unlim dims in the file */

	/* Get data */
	start[0] = 0;
	edges[0] = dims[0];
	HDmemset(outdata1D, 0, edges[0]*sizeof(int16));
	status = ncvarget(ncid, var3id, start, edges, outdata1D);
	CHECK(status, -1, "ncvarget");

	/* Verify data */
	status = HDmemcmp(outdata1D, ncresult1Ddozen, edges[0]*sizeof(int16));
	VERIFY(status, 0, "HDmemcmp");
    } /* end read data with nc API */

    return 0;
}


/***************************************************************************
   Name: test_readings() - tests reading passed actually written data and then
		passed the maximum number of records in the file with nc API.

   Description:
        - open the file and data sets using nc API
	- read VAR3D passing actually written data, then verify that data is
	  read and fill-values are filled correctly
	- read VAR3D passing the max number of records, and function will fail
	- do the same for VAR1D

   Return value:
        The number of errors occurred in this test.
   BMR - Sep 4, 2013
*********************************************************************/
static int test_readings(long max_numrecs)
{
    int   ncid;                 /* file id */
    int   var1id, var2id, var3id; /* variable ids */
    long  start[3];
    long  edges[3];
    long  dims[3];              /* dimension size buffer */
    int   rh_ndims;             /* number of dims */
    int   rh_dims[H4_MAX_VAR_DIMS];     /* variable shape */
    char  varname[H4_MAX_NC_NAME];	/* variable name */
    int   ii, jj, kk;
    int16 outdata3D[DIM0][DIM1][DIM2];	/* 3-D data read back */
    int16 outdata1D[DIM0];	/* 1-D data read back */
    int32 dimsizes3D[3];		/* dimension size buffer for first SDS */
    int32 dimsize1D[1];		/* dimension size buffer for second SDS */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data */

    /* data resulted from reading at start=[4,0,0] for edges=[6,1,1] */
    int16 result3D_start400_edge611[DIM0][DIM1][DIM2] = {-3,-3,-3,800,-3,-3};

    /* data resulted from reading at start=[4] for edges=[6] */
    int16 result1D_start4_edge6[] = {302,303,-1,-1,-1,-1};

    /* Open the file for reading and writing with nc API */
    ncid = ncopen(FILENAME2, NC_RDWR);
    CHECK(ncid, -1, "ncopen");

    /* Verify variable info and data of first data set */

    /* Get access to the variable VAR3D */
    var1id = ncvarid(ncid, VAR3D);
    CHECK(var1id, -1, "ncvarid");

    /* Initialize dimensions to compare */
    dimsizes3D[0] = max_numrecs;
    dimsizes3D[1] = DIM1;
    dimsizes3D[2] = DIM2;

    /* Get variable dimensions */
    status = ncvarinq(ncid, var1id, varname, NULL, &rh_ndims, rh_dims, NULL);
    CHECK(status, -1, "ncvarinq");
    VERIFY(rh_ndims, RANK3, "ncvarinq");
    for (ii = 0; ii < rh_ndims; ii++)
    {
	    status = ncdiminq(ncid, rh_dims[ii], NULL, &dims[ii]);
	    CHECK(status, -1, "ncdiminq");
	    VERIFY(dims[ii], dimsizes3D[ii], "ncdiminq");

	    /* Initialize parameters for reading data */
	    edges[ii] = dims[ii];
	    start[ii] = 0;
    }

    /* Data written by SD API:
		{300,-3,-3,-3,-3,-3,
		 301,-3,-3,-3,-3,-3,
		 302,-3,-3,-3,-3,-3,
		 303,-3,-3,-3,-3,-3,
		 -3,-3,-3,-3,-3,-3,
		 -3,-3,-3,-3,-3,-3,
		 -3,-3,-3,-3,-3,-3,
		 800,801,802,803,804,805} */

    /* Read data pass the written data, but before max numrecs in the file */
    start[0] = 4;
    start[1] = start[2] = 0;
    edges[0] = 6;
    edges[1] = 1; edges[2] = 1;
    HDmemset(outdata3D, 0, edges[0]*edges[1]*edges[2]*sizeof(int16));
    status = ncvarget(ncid, var1id, start, edges, outdata3D);
    CHECK(status, -1, "ncvarget");

    /* Verify data, should be "-3,-3,-3,800,-3,-3".  The first
	   four values are the actual values from the variable.  The last
	   two -3s are added by nc API to fill up to the provided buffer.
	   Note that the first three -3s were filled at the writing time due
	   to skipping during writing */

    /* Verify data */
    status = HDmemcmp(outdata3D, result3D_start400_edge611, edges[0]*edges[1]*edges[2]*sizeof(int16));
    VERIFY(status, 0, "HDmemcmp");

    /* Read data pass the max numrecs in the file, ncvarget should fail */
    start[0] = 4;
    start[1] = start[2] = 0;
    edges[0] = 10;
    edges[1] = edges[2] = 1;
    HDmemset(outdata3D, 0, edges[0]*edges[1]*edges[2]*sizeof(int16));
    status = ncvarget(ncid, var1id, start, edges, outdata3D);
    VERIFY(status, -1, "ncvarget");

    /* Read 2nd variable passing its written data and then passing the
	file's max numrecs */

    /* Get access to the second variable */
    var2id = ncvarid(ncid, VAR1D);
    CHECK(var2id, -1, "ncvarid");

    /* Get and verify variable info */
    status = ncvarinq(ncid, var2id, varname, NULL, &rh_ndims, rh_dims, NULL);
    CHECK(status, -1, "ncvarinq");

    status = ncdiminq(ncid, rh_dims[0], NULL, &dims[0]);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dims[0], MAX_NUMRECS, "ncdiminq"); /* Note: only written up to 6th
		element, but netCDF uses the max number of records in the file,
		which is 12 currently */

    /* Data written by SD API: {-1,-1,300,301,302,303} */

    /* Read data pass the written data, but before max numrecs in the file */
    start[0] = 4;
    edges[0] = 6;
    HDmemset(outdata1D, 0, edges[0]*sizeof(int16));
    status = ncvarget(ncid, var2id, start, edges, outdata1D);
    CHECK(status, -1, "ncvarget");

    /* Verify data, should be "302,303,-1,-1,-1,-1".  The first
	   two values are the actual values from the variable.  The last
	   four -1s are added by nc API to fill up to the provided buffer. */

    /* Read data pass the max numrecs in the file, ncvarget should fail */
    start[0] = 4;
    edges[0] = 10;
    HDmemset(outdata1D, 0, edges[0]*sizeof(int16));
    status = ncvarget(ncid, var2id, start, edges, outdata1D);
    VERIFY(status, -1, "ncvarget");

    return 0;
}

/* Test driver for testing ncvarget in filling fill-values where appropriate */
extern int test_ncvargetfill()
{
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("ncvarget fills buffer when reading UDim HDF data sets (tncvarget.c)");

    /* Test multiple variables with 1 unlimited dimension */
    num_errs = num_errs + test_1dim_multivars();

    /* Test multiple variables with multiple dimensions */
    num_errs = num_errs + test_multidims();

    /* Test reading passed written data and maximum numrecs in the file */
    /* Note: MAX_NUMRECS is the number of records written to data set VARDOZEN
	in test_multidims; it is the maximum number of records in FILENAME2
	that is going to be read by this test_readings */
    num_errs = num_errs + test_readings(MAX_NUMRECS);

    if (num_errs == 0) PASSED();
    return num_errs;
}
