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
 * tsdsprops.c - tests functionality regarding an SDS' properties, such
 *		 name, dimensions, type, size,...
 * Structure of the file:
 *    test_SDSprops - test driver
 *	  test_SDSnames    - tests that an SDS name can be more than 
 *		previously hardcoded at 64 characters; now length is variable.
 *	  test_unlim_dim - tests that writing two unlimited 1D datasets,
 *		without closing the file between writes, will not cause the
 *		second dataset to contain garbage anymore. (bug 525)
 *	  test_unlim_inloop - tests that appending unlimited data to more 
 *		than one dataset within a loop stores data correctly. (bug 801)
 *	  test_valid_args - tests that when some invalid argments were passed
 *		into an API, they can be caught and handled properly.
 *		(bugzilla 150)
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

/***************************************************************************
   Name: test_SDSnames() - tests that SDS name is no longer restricted to 
			   64 characters only. (bugzilla 516)
   Description:
	The main contents include:
	- create data sets with very short and very long names
	- close the file, then reopen it, get SDS' info, and verify the names

   Return value:
        The number of errors occurred in this routine.

   BMR - Oct 2006
****************************************************************************/

#define FILE_NAME     "SDSlongname.hdf"	/* data file to test SDS names */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2
#define NAME_LEN1     93	/* the length of longname_ds below */
#define NAME_LEN2     1		/* the length of shortname_ds below */

#define	longname_ds	"The name of this dataset is long, and it is used to test the new variable length name feature"
#define	shortname_ds	"A"

static intn
test_SDSnames()
{
    int32  fid, dset1, dset2;
    int32  rank, dtype, nattrs, dimsizes[RANK];
    char*  ds_name;
    uint16 name_len;
    intn   status;
    intn   num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Create an X_LENGTH by Y_LENGTH dataset, called DataSet_1 */
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;
    dset1 = SDcreate(fid, longname_ds, DFNT_INT32, RANK, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    dset2 = SDcreate(fid, shortname_ds, DFNT_INT32, RANK, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to check on the SDS name */
    fid = SDstart(FILE_NAME, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /* Get access to the first dataset */
    dset1 = SDselect(fid, 0);
    CHECK(dset1, FAIL, "SDselect");

    /* Get and verify the length of its name */
    status = SDgetnamelen(dset1, &name_len);
    CHECK(status, FAIL, "SDgetnamelen");
    VERIFY(name_len, NAME_LEN1, "SDgetnamelen");

    /* Allocate buffer to get its name */
    ds_name = (char *)HDmalloc(name_len+1);
    CHECK(ds_name, NULL, "HDmalloc");

    /* Get information of the first dataset, and verify its name */
    status = SDgetinfo(dset1, ds_name, &rank, dimsizes, &dtype, &nattrs);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY_CHAR(ds_name, longname_ds, "SDgetinfo");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Release allocated memory */
    HDfree(ds_name);

    /* Get access to the second dataset */
    dset2 = SDselect(fid, 1);
    CHECK(dset2, FAIL, "SDselect");

    /* Get and verify the length of its name */
    name_len = 0;
    status = SDgetnamelen(dset2, &name_len);
    CHECK(status, FAIL, "SDgetnamelen");
    VERIFY(name_len, NAME_LEN2, "SDgetnamelen");

    /* Allocate buffer to get its name */
    ds_name = (char *)HDmalloc(name_len+1);
    CHECK(ds_name, NULL, "HDmalloc");

    /* Get information of the second dataset, and verify its name */
    status = SDgetinfo(dset2, ds_name, &rank, dimsizes, &dtype, &nattrs);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY_CHAR(ds_name, shortname_ds, "SDgetinfo");

    /* Close the datasets */
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Release allocated memory */
    HDfree(ds_name);

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
}   /* test_SDSnames */

/***************************************************************************
   Name: test_unlim_dim() - tests that writing two unlimited 1D datasets,
			    without closing the file between writes, will 
			    not cause the second dataset to contain garbage 
			    anymore. (bug 525)
   Description:
	The main contents include:
	- create and write to two datasets
	- append data to both without closing anything
	- close the datasets and the file
	- reopen the file, get the data sets and their info
	- read and verify the data

   Return value:
        The number of errors occurred in this routine.

   BMR - Oct 2005
****************************************************************************/

#define UD_FILE_NAME  "Unlim_dim.hdf"	/* data file to test unlimited dim */
#define DS1_NAME "data 1"
#define DS2_NAME "data 2"
#define DIM0 10

static intn
test_unlim_dim()
{
    int32 fid, dset1, dset2;
    int32 rank, start[1], edges[1], dtype, nattrs, dimsizes[1];
    int16 array_data[DIM0],	/* data to be written to both datasets */
	  append_data[DIM0],	/* data to be appended to both datasets */
	  outdata[DIM0+DIM0], outdata1[DIM0+DIM0]; /* data read */
    char  ds_name[20];
    intn  idx, status;
    intn  num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(UD_FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Define the rank and dimensions of the data set to be created. */
    rank = 1;
    dimsizes[0] = SD_UNLIMITED;

    /* Create two array data sets. */
    dset1 = SDcreate(fid, DS1_NAME, DFNT_INT16, rank, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    dset2 = SDcreate(fid, DS2_NAME, DFNT_INT16, rank, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Fill the stored-data array with values. */
    for (idx = 0; idx < DIM0; idx++) {
	array_data[idx] = idx + 1;
    }

    /* Define the location, pattern, and size of the data sets */
    start[0] = 0;
    edges[0] = DIM0;

    status = SDwritedata(dset1, start, NULL, edges, (VOIDP)array_data);
    CHECK(status, FAIL, "SDwritedata");
    status = SDwritedata(dset2, start, NULL, edges, (VOIDP)array_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Store array values to be appended */
    for (idx = 0; idx <DIM0; idx++)
	append_data[idx] = DIM0 + idx;

    /* Define the location of the append */
    start[0] = DIM0; /* start from the end of the previous write */
    edges[0] = DIM0; /* append the same number of elements as the write */

    /* Append data to file */
    status = SDwritedata (dset1, start, NULL, edges, (VOIDP) append_data);
    CHECK(status, FAIL, "SDwritedata");
    status = SDwritedata (dset2, start, NULL, edges, (VOIDP) append_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to check on the SDS names */
    fid = SDstart(UD_FILE_NAME, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /* Get access to the datasets */
    dset1 = SDselect(fid, 0);
    CHECK(dset1, FAIL, "SDselect");
    dset2 = SDselect(fid, 1);
    CHECK(dset1, FAIL, "SDselect");

    /* Get information of the first dataset, and verify its name */
    status = SDgetinfo(dset1, ds_name, &rank, dimsizes, &dtype, &nattrs);
    CHECK(status, FAIL, "SDgetinfo");

    /* Define the location to read */
    start[0] = 0;
    edges[0] = DIM0+DIM0;

    /* Read and check first dataset */
    status = SDreaddata(dset1, start, NULL, edges, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    for (idx = 0; idx < DIM0; idx++) {
	VERIFY(outdata[idx], array_data[idx], "SDreaddata first dataset");
    }
    for (idx = DIM0; idx < edges[0]; idx++) {
	VERIFY(outdata[idx], append_data[idx-DIM0], "SDreaddata first dataset");
    }

    /* Read and check second dataset */
    status = SDreaddata(dset2, start, NULL, edges, (VOIDP) outdata1);
    CHECK(status, FAIL, "SDreaddata");

    for (idx = 0; idx < DIM0; idx++) {
	VERIFY(outdata1[idx], array_data[idx], "SDreaddata first dataset");
    }
    for (idx = DIM0; idx < edges[0]; idx++) {
	VERIFY(outdata1[idx], append_data[idx-DIM0], "SDreaddata first dataset");
    }

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the datasets */
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
}   /* test_unlim_dim */

/***************************************************************************
   Name: test_unlim_inloop() - tests that appending unlimited data to more
			    than one dataset within a loop stores data
			    correctly. (bug 801)
   Description:
	The main contents include:
	- create two data sets 
	- write to the data sets in a loop
	- close the file, then reopen it
	- in another loop, read the data sets and verify their data

   Return value:
        The number of errors occurred in this routine.

   BMR - Oct 2005
****************************************************************************/

#define UDIL_FILE_NAME  "Unlim_inloop.hdf" /* file to test unlim dims in loop */
#define SIZE 5 
#define N_DSETS 2 
#define RANK1 1 

static intn
test_unlim_inloop()
{
    int32   fid;
    int32   sds_id[N_DSETS];
    int32   start[RANK1], edges[RANK1], rank, dimsizes[RANK1];
    int32   dtype, nattrs;
    char    sds_name[20];
    int     i, n_writes; /* number of times writing to the data sets */
    /*int32   n,m, mm;*/
    intn    status;
    float64 array_data[SIZE];
    intn    num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(UDIL_FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    dimsizes[0] = SD_UNLIMITED;

    /* Create N_DSETS data sets in a loop */
    for(i = 0; i < N_DSETS; i++) {
        sprintf(sds_name,"test%d",i);
        sds_id[i] = SDcreate (fid, sds_name, DFNT_FLOAT64, 1, dimsizes);
        CHECK(sds_id[i], FAIL, "SDcreate");

        status = SDendaccess (sds_id[i]);
        CHECK(status, FAIL, "SDendaccess");
     }

    /* Making up data to write */
    for (i = 0; i < SIZE; i++) {
	array_data[i] = i + 1;
    } 

    /* write and append to the data sets */
    n_writes = 0;
    edges[0] = SIZE;
    while (n_writes < 2)
    {
	/* i.e, start at 0 first time and at SIZE second time*/
	start[0] = SIZE * n_writes;

	for (i = 0; i < N_DSETS; i++) 
	{
	    sds_id[i] = SDselect(fid, i);
	    CHECK(sds_id[i], FAIL, "SDselect");

	    status = SDwritedata(sds_id[i], start, NULL, edges, (VOIDP)array_data);
	    CHECK(status, FAIL, "SDwritedata");

	    status = SDendaccess (sds_id[i]);
	    CHECK(status, FAIL, "SDendaccess");
	}
	n_writes++;
    }

    /* close the file */
    status = SDend (fid);
    CHECK(status, FAIL, "SDcreate");

    /* Re-open the file to check on the SDS name */
    fid = SDstart(UDIL_FILE_NAME, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    for (i = 0; i < N_DSETS; i++)
    {
	float64 outdata[SIZE*2];

	/* Get access to the datasets */
	sds_id[i] = SDselect(fid, i);

	/* Get information of the first dataset, and verify its name */
	status = SDgetinfo(sds_id[i], sds_name, &rank, dimsizes, &dtype, &nattrs);
	CHECK(status, FAIL, "SDgetinfo");

	/* Define the location to read */
	start[0] = 0;
	edges[0] = SIZE * n_writes;

	/* Read and check first dataset */
	status = SDreaddata(sds_id[i], start, NULL, edges, (VOIDP) outdata);
	CHECK(status, FAIL, "SDreaddata");

	/* Verify the read data */
	n_writes = 0;
	while (n_writes < 2)
	{
	    intn in, out;
	    for (in = 0, out = 0+(SIZE*n_writes); in < SIZE; in++, out++) {
                VERIFY(outdata[out], array_data[in], "SDreaddata");
	    }
	    n_writes++;
	}

	/* Close the datasets */
	status = SDendaccess(sds_id[i]);
	CHECK(status, FAIL, "SDendaccess");
    }

    /* close the file */
    status = SDend (fid);
    CHECK(status, FAIL, "SDcreate");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
}   /* test_unlim_inloop */

/***************************************************************************
   Name: test_valid_args() - tests that when some invalid argments were passed
			    into an API, they can be caught and handled 
			    properly. (bugzilla 150)
   Description:
	The main contents include:
	- create a data set of size X_LENGTH x Y_LENGTH
	- write to the data set
	- close the file, then reopen it
	- read the dataset giving a stride value, that goes beyong the 
	  dimension size
	- when SDreaddata failed, try to check for error code DFE_ARGS and
	  handle the failure properly
	- close all.

   Return value:
        The number of errors occurred in this routine.

   BMR - Sep 2007
****************************************************************************/

#define ARGS_FILE_NAME  "test_arguments.hdf" /* file to test validating args */

static intn
test_valid_args()
{
    int32 fid, dset1, dset2;
    int32 start[2], edges[2], dtype, nattrs, dimsizes[2], rank, strides[2];
    int16 array_data[X_LENGTH][Y_LENGTH], /* data to be written to datasets */
	  outdata[X_LENGTH][Y_LENGTH];    /* data read */
    char  ds_name[20];
    intn  idxx, idxy, status;
    intn  num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(ARGS_FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Create two array data sets. */
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;
    dset1 = SDcreate(fid, DS1_NAME, DFNT_INT16, RANK, dimsizes);
    CHECK(dset1, FAIL, "SDcreate");

    dimsizes[0] = SD_UNLIMITED;
    dset2 = SDcreate(fid, DS2_NAME, DFNT_INT16, RANK, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Fill the stored-data array with values. */
    for (idxx = 0; idxx < X_LENGTH; idxx++) {
	for (idxy = 0; idxy < Y_LENGTH; idxy++) {
	    array_data[idxx][idxy] = idxx*idxy + 1;
    }}

    /* Define the location, pattern, and size of the data sets */
    start[0] = 0;
    start[1] = 0;
    edges[0] = X_LENGTH;
    edges[1] = Y_LENGTH;

    status = SDwritedata(dset1, start, NULL, edges, (VOIDP)array_data);
    CHECK(status, FAIL, "SDwritedata");
    status = SDwritedata(dset2, start, NULL, edges, (VOIDP)array_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to check on the SDS names */
    fid = SDstart(ARGS_FILE_NAME, DFACC_RDWR);
    CHECK(fid, FAIL, "SDstart");

    /* Get access to the datasets */
    dset1 = SDselect(fid, 0);
    CHECK(dset1, FAIL, "SDselect");
    dset2 = SDselect(fid, 1);
    CHECK(dset1, FAIL, "SDselect");

    /* Get information of the first dataset, and verify its name */
    status = SDgetinfo(dset1, ds_name, &rank, dimsizes, &dtype, &nattrs);
    CHECK(status, FAIL, "SDgetinfo");

    /* Define the location to read */
    strides[0] = X_LENGTH;	/* simulate a mistake here, should have -1 */
    strides[1] = Y_LENGTH-1;

    /* Attempt to read first dataset, it should fail with invalid an 
	argument error */
    status = SDreaddata(dset1, start, strides, edges, (VOIDP) outdata);
    VERIFY(status, FAIL, "SDreaddata");
    VERIFY(HEvalue(1), DFE_ARGS, "SDreaddata");

    /* Show that the error was also caught for the dataset with unlimited 
	dimension */
    status = SDreaddata(dset2, start, strides, edges, (VOIDP) outdata);
    VERIFY(status, FAIL, "SDreaddata");
    VERIFY(HEvalue(1), DFE_ARGS, "SDreaddata");

    /* Close the datasets */
    status = SDendaccess(dset1);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the datasets */
    status = SDendaccess(dset2);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
}   /* test_valid_args */

/*******************************************************************
   Name: test_valid_args2() - tests that when some invalid argments were passed
			    into SDreaddata, causing the function to attempt
			    to read beyond the dimension size, can be caught 
			    and handled properly. (more for bugzilla 150)
   Description:
	The main contents include:
	- creates a file and three datasets:
	  + "data1" dimsizes=[1] int32 read the one element
	  + "data2" dimsizes=[202,271]
	  + "data3" dimsizes=[10,3,2]
	- close the file and reopen it to check reading data
	- read the datasets as followed to test error checkings on parameters
	  + "data1" - attempts to read with incorrect stride value
	  + "data1" - attempts to read with incorrect count value
	  + "data1" - reads again with all arguments in bounds
	  + "data2" - attempts to read with strides[1]=D2_Y (correct: D2_Y-1)
	  + "data2" - attempts to read with count[1]=3 (correct: 2)
	  + "data3" - attempts to read with incorrect combination of stride and count
	  + "data3" - reads again with all arguments in bounds
                                                                 
********************************************************************/
#define D1_X 1
#define D2_X 202
#define D2_Y 271
#define D3_X 10
#define D3_Y 3
#define D3_Z 2

/* Helper function to test_valid_args2 creates and writes to a dataset */
static intn makeSDS(int32 sd_id, char* name, int32 dtype, int32 rank, 
	     int32* dimsizes, int32* start, int32* strides, 
	     int32* count, void* data)
{
    int32 sds_id;
    intn status;
    intn  num_errs = 0;         /* number of errors so far */

    /* Create the named dataset */
    sds_id = SDcreate(sd_id, name, dtype, rank, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Write to it */
    status = SDwritedata(sds_id, start, strides, count, data); 
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    return (status);
}

static intn
test_valid_args2()
{
    int32 sd_id, sds_id;
    int32 dim[1], dims2[2], dims3[3], d1start[1], d2start[2], d3start[3];
    int32 d1count[1], d2count[2], d3count[3];
    int32 d1stride[1], d2stride[2], d3stride[3];
    float32 data1=32.0, outdata1;
    int32 data2[D2_X][D2_Y], outdata2[D2_X][D2_Y];
    int16 data3[D3_X][D3_Y][D3_Z], outdata3[D3_X][D3_Y][D3_Z];
    intn i, j, k, status;
    intn  num_errs = 0;         /* number of errors so far */

    /* Create and open the file and initiate the SD interface. */
    sd_id = SDstart("b150.hdf", DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the first dataset */
    dim[0] = D1_X;
    d1start[0] = 0;
    d1count[0] = 1;
    data1 = 32.0;
    status = makeSDS(sd_id, "data1", DFNT_FLOAT32, 1, dim, d1start, NULL, d1count, &data1);
    CHECK(status, FAIL, "makeSDS data1");

    /* Create and write to second dataset */
    dims2[0] = D2_X;
    dims2[1] = D2_Y;
    d2start[0] = d2start[1] = 0;
    d2count[0] = dims2[0];
    d2count[1] = dims2[1];
    for (i = 0; i < D2_X; i++)
	for (j = 0; j < D2_Y; j++)
	    data2[i][j] = i * j;

    status = makeSDS(sd_id, "data2", DFNT_INT32, 2, dims2, d2start, NULL, d2count, data2);
    CHECK(status, FAIL, "makeSDS data2");


    /* Create and write to third dataset */
    dims3[0] = D3_X;
    dims3[1] = D3_Y;
    dims3[2] = D3_Z;
    d3start[0] = d3start[1] = d3start[2] = 0;
    d3count[0] = dims3[0];
    d3count[1] = dims3[1];
    d3count[2] = dims3[2];
    for (i = 0; i < D3_X; i++)
	for (j = 0; j < D3_Y; j++)
	    for (k = 0; k < D3_Z; k++)
		data3[i][j][k] = i * j * k;

    status = makeSDS(sd_id, "data3", DFNT_INT16, 3, dims3, d3start, NULL, d3count, data3);
    CHECK(status, FAIL, "makeSDS data3");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Reopen the file and read in some of the data */
    sd_id = SDstart("b150.hdf", DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    /* Read first dataset with incorrect stride value, but shouldn't fail */
    sds_id = SDselect(sd_id, 0);
    d1start[0] = 0;
    d1stride[0] = 2; /* irrelevant because only 1 value to be read */
    d1count[0] = 1;
    status = SDreaddata(sds_id, d1start, d1stride, d1count, &outdata1);
    CHECK(status, FAIL, "SDreaddata");

    /* Read first dataset with incorrect count value */
    d1start[0] = 0;
    d1stride[0] = 1;
    d1count[0] = 2; /* array only has 1 value */
    status = SDreaddata(sds_id, d1start, d1stride, d1count, &outdata1);
    VERIFY(status, FAIL, "SDreaddata");

    /* Read again with "correct" arguments */
    d1count[0] = 1;
    status = SDreaddata(sds_id, d1start, d1stride, d1count, &outdata1);
    CHECK(status, FAIL, "SDreaddata");
    VERIFY(outdata1, data1, "SDreaddata first dataset");

    /* Terminate access to the first dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Read second dataset with out of bound stride */
    sds_id = SDselect(sd_id, 1);
    d2start[0] = d2start[0] = 0;
    d2stride[0] = 1;
    d2stride[1] = D2_Y;  /* should be D2_Y - 1 */
    d2count[0] = D2_X;
    d2count[1] = 2;
    status = SDreaddata(sds_id, d2start, d2stride, d2count, (VOIDP)outdata2);
    VERIFY(status, FAIL, "SDreaddata");

    /* Read second dataset with too many values requested */
    d2stride[1] = D2_Y - 1;
    d2count[1] = 3;	/* should be 2 */
    status = SDreaddata(sds_id, d2start, d2stride, d2count, (VOIDP)outdata2);
    VERIFY(status, FAIL, "SDreaddata");

    /* Terminate access to the second dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Read third dataset with too many values requested */
    sds_id = SDselect(sd_id, 2);
    d3start[0] = 1;
    d3start[1] = d3start[2] = 0;
    d3stride[0] = 3;
    d3stride[1] = 2;
    d3stride[2] = 1;
    d3count[0] = 4;  /* should be 3 max or smaller stride */
    d3count[1] = 2;
    d3count[2] = 2;
    status = SDreaddata(sds_id, d3start, d3stride, d3count, (VOIDP)outdata3);
    VERIFY(status, FAIL, "SDreaddata");

    /* Read again with "correct" arguments */
    d3start[0] = 1;
    d3start[1] = d3start[2] = 0;
    d3stride[0] = 3;
    d3stride[1] = 2;
    d3stride[2] = 1;
    d3count[0] = 3;
    d3count[1] = 2;
    d3count[2] = 2;
    status = SDreaddata(sds_id, d3start, d3stride, d3count, (VOIDP)outdata3);
    CHECK(status, FAIL, "SDreaddata");

    /* Terminate access to the third dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
} /* test_valid_args2 */

/* Test driver for testing various SDS' properties. */
extern int
test_SDSprops()
{
    intn num_errs = 0;         /* number of errors */

    /* Output message about test being performed */
    TESTING("various SDS' properties (tsdsprops.c)");

    num_errs = num_errs + test_SDSnames();
    num_errs = num_errs + test_unlim_dim();
    num_errs = num_errs + test_unlim_inloop();
    num_errs = num_errs + test_valid_args();
    num_errs = num_errs + test_valid_args2();

    if (num_errs == 0) PASSED();
    return num_errs;
}

#endif /* HDF */
