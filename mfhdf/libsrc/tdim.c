/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "mfhdf.h"

#ifdef macintosh
    #include <LowMem.h>
#endif

#ifdef HDF

#include "hdftest.h"

/* This test is added to test SDsetdimscale while fixing bug #172.
   Beside the fact that the main program was already very long, adding
   this separate test routine will also define a place for additional
   dimension tests to be appended in the future.  Also, hopefully, some
   day, the main program can be shortened and some of its
   dimension-related tests can be moved into this test routine - 04/18/01
*/

/********************************************************************
   Name: test_dimensions()

   Description: 
	This test routine is used to test various dimension operations.
	The main contents include:
	- creates the file dim.hdf
	- creates an SDS of size 15 x 10, and sets values to its dimensions
	- creates another SDS of size 4 x 3, and sets values to its 
		first dimension

	The followings are included in this test routine:
	- SDgetdimid
	- SDsetdimname
	- SDdiminfo
	- SDgetdimscale
        - SDsetdimscale with the following situations:
        	+ not called before SDdiminfo 
		+ called with number type = 0
        	+ called with an unsigned number type
 		+ called with the dataset's number type 
 		+ called before writing data to dataset
 		+ called after closing dataset and file, and then 
		  reopening

   Return value:
	The number of errors occurred in this routine.

*********************************************************************/
#define FILE_DIM  "dim.hdf"	/* created and used by test_dimensions */
#define LENGTH0 15		/* first SDS dimension */
#define LENGTH1 10
#define LENGTH2 4		/* second SDS dimension */
#define LENGTH3 3
#define RANK 	2		/* both SDS' rank */
#define DS0_NAME "HDF Data 0"	/* first SDS name */
#define DS1_NAME "HDF Data 1"	/* second SDS name */
#define DIM0_NAME " Dimension 0"	/* name of first SDS' first dim */
#define DIM1_NAME " Dimension 1"	/* name of first SDS' second dim */
#define DIM2_NAME " Dimension 2"	/* name of second SDS' first dim */

extern int
test_dimensions()
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

    /* Create the file defined by FILE_DIM and initiate the SD interface. */
    fid = SDstart(FILE_DIM, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /*
     * Add a LENGTH0 x LENGTH1 array, which is named by DS0_NAME and of
     * type 16-bit signed integer, to the file... 
     */

    /* Define the rank and dimensions of the data set. */
    rank = RANK;
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
    dim0_id = SDgetdimid (sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to the name defined by DIM0_NAME */
    status = SDsetdimname (dim0_id, DIM0_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Set scale of type unsigned int-8 for the first dimension */
    status = SDsetdimscale (dim0_id, dims[0], DFNT_UINT8, scale0);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the first dimension and verify its information */
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY (strcmp(dim_name, DIM0_NAME), 0, "SDdiminfo");
    VERIFY (size, dims[0], "SDdiminfo");
    VERIFY (dim_data_type, DFNT_UINT8, "SDdiminfo"); /* bug #172 is fixed! */
    VERIFY (dim_num_attrs, 0, "SDdiminfo");

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

    /* Re-open FILE_DIM in W mode and select the dataset named by DS0_NAME */
    fid = SDstart (FILE_DIM, DFACC_WRITE);
    CHECK(fid, FAIL, "SDstart");

    /* Look for DS0_NAME dataset */
    sds_idx = SDnametoindex (fid, DS0_NAME);
    CHECK(sds_idx, FAIL, "SDnametoindex");

    /* Select that dataset */
    sds_id = SDselect (fid, sds_idx);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get info of the dataset and verify them: it is a LENGTH0 x LENGTH1 
       array of type DFNT_INT16 and is named by DS0_NAME */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY (strcmp(name, DS0_NAME), 0, "SDgetinfo");
    VERIFY (array_rank, RANK, "SDgetinfo");
    VERIFY (num_type, DFNT_INT16, "SDgetinfo");
    VERIFY (dim_sizes[0], LENGTH0, "SDgetinfo");
    VERIFY (dim_sizes[1], LENGTH1, "SDgetinfo");

    /*
     * Setting name for the second dimension, but not scale yet...
     */

    /* Get the second dimension id */
    dim1_id = SDgetdimid (sds_id, 1);
    CHECK(dim1_id, FAIL, "SDgetdimid");

    /* Set the second dimension name to the name defined by DIM1_NAME */
    status = SDsetdimname (dim1_id, DIM1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Read the second dimension and verify its information; since the
       scale of this dimension is not set yet, the datatype should be 0 */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY (strcmp(dim_name, DIM1_NAME), 0, "SDdiminfo");
    VERIFY (size, dims[1], "SDdiminfo");
    VERIFY (dim_data_type, 0, "SDdiminfo");
    VERIFY (dim_num_attrs, 0, "SDdiminfo");

    /* Set dimension scale for the second dimension; its type will be the
       same as that of the of the dataset */
    status = SDsetdimscale (dim1_id, dim_sizes[1], num_type, scale1);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the second dimension and verify its information; since the
       scale of this dimension is now set, the datatype should be DFNT_INT16 */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY (dim_data_type, DFNT_INT16, "SDdiminfo");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Add a data array named by DS1_NAME to the file... 
     */

    /*
     * Add another dataset to the file; this dataset is a LENGTH2 x LENGTH3
     * array, named by DS1_NAME and of type 32-bit unsigned integer...
     */

    /* Define the rank and dimensions of the data set */
    rank = RANK;
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
    VERIFY (strcmp(name, DS1_NAME), 0, "SDgetinfo");
    VERIFY (array_rank, RANK, "SDgetinfo");
    VERIFY (num_type, DFNT_UINT32, "SDgetinfo");
    VERIFY (dim_sizes[0], LENGTH2, "SDgetinfo");
    VERIFY (dim_sizes[1], LENGTH3, "SDgetinfo");

    /*
     * Settings for the first dimension of the dataset named by DS1_NAME... 
     */

    /* Get the first dimension id */
    dim0_id = SDgetdimid (sds_id, 0);
    CHECK(dim0_id, FAIL, "SDgetdimid");

    /* Set the first dimension name to the name defined by DIM2_NAME */
    status = SDsetdimname (dim0_id, DIM2_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Verify that when 0 is passed into SDsetdimscale for number type,
       then the dimension scale will be set to DFNT_FLOAT32 */
    status = SDsetdimscale (dim0_id, dims[0], 0, scalef); 
    CHECK(status, FAIL, "SDsetdimscale");

    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY (strcmp(dim_name, DIM2_NAME), 0, "SDdiminfo");
    VERIFY (size, dims[0], "SDdiminfo");
    VERIFY (dim_data_type, DFNT_FLOAT32, "SDdiminfo");
    VERIFY (dim_num_attrs, 0, "SDdiminfo");

    /* 
     * Now, set dimension scale for that first dimension again, but this
     * time, to 8-bit signed char; then verify the type and the scale values 
     */

    /* Set scale of type 8-bit signed char for this dimension */
    status = SDsetdimscale (dim0_id, dims[0], DFNT_CHAR, scale2);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Read the info of this dimension its new number type */ 
    status = SDdiminfo(dim0_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY (dim_data_type, DFNT_CHAR, "SDdiminfo");

    /* Read dimension scale values and verify them */
    status = SDgetdimscale (dim0_id, (VOIDP)scale2_out);
    CHECK(status, FAIL, "SDgetdimscale");
    for(i=0; i < LENGTH2; i++)
	VERIFY (scale2_out[i], scale2[i], "SDgetdimscale");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_dimensions */

#endif /* HDF */
