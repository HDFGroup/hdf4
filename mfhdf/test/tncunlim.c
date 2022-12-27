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
 * tncunlim.c - tests reading/writing variables with unlimited dimension using
 *		nc API.  The tests here are added to test the behavior of the
 *		library when dealing with unlimited dimensions via nc API.  The
 *		tests show that the data is correctly written/read when:
 *		+ data added immediately after last record
 *		+ data added skipping one or more records
 *		+ data overridden existing data
 *		+ data read pass the end of that variable but not the max in 
 *		  all the variables in the file (on going)
 *
 * Structure of the file:
 *    test_ncunlim - test driver
 *        test_1dim_singlevar - tests on a single variable with only 1 dimension
 *        test_1dim_multivars - tests on multiple variables with only 1 dimension
 *        test_multidim_singlevar - tests on single variable with multiple dimensions
 * BMR - Dec 22, 2008
****************************************************************************/

#include "mfhdf.h"
#include "hdftest.h"

/********************************************************************
   Name: test_1dim_singlevar() - tests on a single variable with only 1 dimension

   Description:
        The main contents include:
        - write 4 elements starting at index 0
        - append 2 elements starting at index 6, that is indices 4 and 5 will
	  be written with fill value
        - append 3 elements immediately at the end of the data
        - overwrite indices 0 and 1

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 22, 2008
*********************************************************************/

#define DIM0 20
#define FILENAME1 "onedimonevar.nc"
static int test_1dim_singlevar()
{
    int   ncid;			/* file id */
    int   varid;		/* variable id */
    int   time_dim;		/* unlimited dimension */
    int   dims[1];		/* variable shapes */
    long  dimsize = 0;		/* dimension size buffer */
    short outdata[DIM0];	/* data read back */
    char  varname[11];		/* variable name */
    nc_type rh_type;		/* variable type */
    int   rh_ndims;		/* number of dims */
    int   rh_dims[H4_MAX_VAR_DIMS];	/* variable shape */
    int   rh_natts;		/* number of attributes */
    int   ii;
    short fillval = 99;		/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data; the first two elements will
	be changed to "1,2" later for the last test. */
    short result[] = {320,301,301,302,99,99,30,31,801,802,803};

    /* Enter define mode */
    ncid = nccreate(FILENAME1, NC_CLOBBER);
    CHECK(ncid, -1, "nccreate");

    /* Variables will be filled with fill values when data is missing */
    status = ncsetfill(ncid, NC_FILL);
    CHECK(status, -1, "ncsetfill");

    /* Define the unlimited dimension */
    time_dim = ncdimdef(ncid, "time", NC_UNLIMITED);
    CHECK(status, -1, "ncdimdef");

    /* Define the variable */
    dims[0] = time_dim;
    varid = ncvardef (ncid, "Variable 1", NC_SHORT, 1, dims);
    CHECK(varid, -1, "ncvardef");

    /* write the fill value attribute */
    status = ncattput(ncid, varid, "_FillValue", NC_SHORT, 1, &fillval );
    CHECK(status, -1, "ncattput");

    /* leave define mode */
    status = ncendef (ncid);
    CHECK(status, -1, "ncendef");

    {  /* write data the first time */
	long rh_start[] = {0};	/* starting writing at beginning, index 0 */
	long rh_edges[] = {4};	/* four elements */
	short data[] = { 320,301,301,302};

	/* write 4 elements starting at index 0 */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get variable information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    /* Close file */
    status = ncclose (ncid);
    CHECK(status, -1, "ncclose");

    /* Open file for reading and writing */
    ncid = ncopen(FILENAME1, NC_RDWR);
    CHECK(ncid, -1, "ncopen");

    /* Get access to the variable */
    varid = ncvarid(ncid, "Variable 1");
    CHECK(varid, -1, "ncvarid");

    /* Get and verify variable info and size of the unlimited dimension again */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    { /* read and verify data of the variable */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be "320 301 301 302" */
	for (ii = 0; ii < dimsize; ii++) {
	    if (outdata[ii] != result[ii])
                fprintf(stderr,"test_1dim_singlevar: Read data %d doesn't match input %d at index %d\n", outdata[ii], result[ii], ii);
	}
    } /* end read data */

    {  /* Append data to variable pass the end */
	long rh_start[] = {6};	/* the end is 3 */
	long rh_edges[] = {2};	/* two elements */
	short data[] = { 30,31};

	/* write 2 values starting at index 6 */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get and verify variable info and size of the unlimited dimension again */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 8, "ncdiminq");

    { /* read variable after appending with gap */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be "320,301,301,302,99,99,30,31" where 99 is 
	   fillvalue */
	for (ii = 0; ii < edges[0]; ii++) {
	    if (outdata[ii] != result[ii])
                fprintf(stderr,"test_1dim_singlevar: Read data %d doesn't match input %d at index %d\n", outdata[ii], result[ii], ii);
	}
    } /* end read data */

    {  /* write variable immediately pass the end */
	long rh_start[] = {0};
	long rh_edges[] = {3};
	short data[] = { 801,802,803};
	rh_start[0] = dimsize;	/* starting immediatly pass dimension size */

	/* write 3 elements to the variable starting at 'dimsize' */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get and verify variable info and size of the unlimited dimension */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 11, "ncdiminq");

    { /* read variable */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0+2);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be "320,301,301,302,99,99,30,31,801,802,803"
	   where 99 is fill value */
	for (ii = 0; ii < edges[0]; ii++) {
	    if (outdata[ii] != result[ii])
                fprintf(stderr,"test_1dim_singlevar: Read data %d doesn't match input %d at index %d\n", outdata[ii], result[ii], ii);
	}
    } /* end read data */

    {  /* write variable to override current values */
	long rh_start[] = {0};	/* starting at the beginning */
	long rh_edges[] = {2};	/* two values */
	short data[] = { 1,2};

	/* write 2 elements overriding the first two values */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");

	/* change expected data too */
	result[0] = 1;
	result[1] = 2;
    }

    /* Get and verify variable info and size of the unlimited dimension */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 11, "ncdiminq");

    { /* read and verify data */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;

	HDmemset(outdata, 0, DIM0+2);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be "1,2,301,302,99,99,30,31,801,802,803"
	   where 99 is fill value, and first and second values are new */
	for (ii = 0; ii < edges[0]; ii++) {
	    if (outdata[ii] != result[ii])
                fprintf(stderr,"test_1dim_singlevar: Read data %d doesn't match input %d at index %d\n", outdata[ii], result[ii], ii);
	}
    } /* end read data */

    /* Close file */
    status = ncclose(ncid);
    CHECK(status, -1, "ncclose");

    return 0;
} /* test_1dim_singlevar */


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
    int   ncid;			/* file id */
    int   varid1, varid2;	/* variable id */
    int   time_dim;		/* unlimited dimension */
    int   dims[1];		/* variable shapes */
    long  dimsize = 0;		/* dimension size buffer */
    short outdata[DIM0];	/* data read back */
    char  varname[11];		/* variable name */
    nc_type rh_type;		/* variable type */
    int   rh_ndims;		/* number of dims */
    int   rh_dims[H4_MAX_VAR_DIMS];	/* variable shape */
    int   rh_natts;		/* number of attributes */
    int   ii;
    short var1_fillval=-1;	/* fill value for the variable */
    short var2_fillval=-2;	/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data; the first two elements will
	be changed to "1,2" later for the last test. */
    short result1[] = {300,301,302,303,400,401,-1,-1,-1,-1,-1,-1,500,501,502};
    short result2[] = {102,104,-2,-2,200,201,202,-2,-2,-2,-2,-2,-2,-2,-2};

    /* Enter define mode */
    ncid = nccreate(FILENAME2, NC_CLOBBER);
    CHECK(ncid, -1, "nccreate");

    /* Variables will be filled with fill values when data is missing */
    status = ncsetfill(ncid, NC_FILL);
    CHECK(status, -1, "ncsetfill");

    /* Define the unlimited dimension */
    time_dim = ncdimdef(ncid, "time", NC_UNLIMITED);
    CHECK(time_dim, -1, "ncdimdef");

    /* Define two 1-dim variables, named "Variable 1" and "Variable 2" */
    dims[0] = time_dim;
    varid1 = ncvardef (ncid, "Variable 1", NC_SHORT, 1, dims);
    CHECK(varid1, -1, "ncvardef");
    varid2 = ncvardef (ncid, "Variable 2", NC_SHORT, 1, dims);
    CHECK(varid2, -1, "ncvardef");

    /* write the fill value attribute of the two variables */
    status = ncattput(ncid, varid1, "_FillValue", NC_SHORT, 1, &var1_fillval);
    CHECK(status, -1, "ncattput");
    status = ncattput(ncid, varid2, "_FillValue", NC_SHORT, 1, &var2_fillval);
    CHECK(status, -1, "ncattput");

    /* leave define mode */
    status = ncendef (ncid);
    CHECK(status, -1, "ncendef");

    {  /* Write data to both variables */
	long rh_start[] = {0};
	long rh_edges[] = {4};
	short data1[] = { 300,301,302,303};
	short data2[] = { 102, 104};

	/* write 4 elements to Variable 1 starting at index 0 */
	status = ncvarput(ncid, varid1, rh_start, rh_edges, (void *)data1);
	CHECK(status, -1, "ncvarput");

	/* write 2 elements to Variable 2 starting at index 0 */
	rh_edges[0] = 2;
	status = ncvarput(ncid, varid2, rh_start, rh_edges, (void *)data2);
	CHECK(status, -1, "ncvarput");
    }

    /* Get Variable 1's information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid1, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    /* Get Variable 2's information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid2, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    /* Close file */
    status = ncclose (ncid);
    CHECK(status, -1, "ncclose");

    /* Open file for reading and writing */
    ncid = ncopen(FILENAME2, NC_RDWR);
    CHECK(ncid, -1, "ncopen");

    /* Get access to the two variables */
    varid1 = ncvarid(ncid, "Variable 1");
    CHECK(varid1, -1, "ncvarid");
    varid2 = ncvarid(ncid, "Variable 2");
    CHECK(varid2, -1, "ncvarid");

    /* Get Variable 1's information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid1, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    /* Get Variable 2's information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid2, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    { /* Read data of the variables */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0);

	/* Read and verify data of Variable 1 */
	status = ncvarget(ncid, varid1, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* Verify data of Variable 1, should be "320 301 301 302" */
	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result1[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 1\n", outdata[ii], result1[ii], ii);
	}

	/* Read and verify data of Variable 2 */
	edges[0] = 2;	/* only reading what written to this variable so far */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* Verify data of Variable 2, should be "102 104" */
	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result2[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 2\n", outdata[ii], result2[ii], ii);
	}

	/* Try to read again but up to the max number of records, 4 in this case
	   because the other variable in the file has 4 elements written */
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* Verify data of Variable 2, should be "102 104 -2 -2" */
	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result2[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 2\n", outdata[ii], result2[ii], ii);
	}
    } /* end read data */

    {  /* Write to variable immediately after last written record */
	long rh_start[] = {0};
	long rh_edges[] = {2};
	short data2[] = {400, 401};

	/* Write 2 elements to Variable 1 at index 'dimsize' */
	rh_start[0] = dimsize;
	status = ncvarput(ncid, varid1, rh_start, rh_edges, (void *)data2);
	CHECK(status, -1, "ncvarput");
    }

    /* Get information of Variable 1 and size of the unlimited dimension */
    status = ncvarinq (ncid, varid1, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 6, "ncdiminq");

    { /* Read variables */
	long start[] = {0};
	long edges[] = {0};
	/* Current data of Variable 2 should look like this, because of the 
	   extension with fill value to match the max number of records in 
	   the file */
	short tempresult[] = {102,104,-2,-2,-2,-2};

	/* Read up to max number of records in the file */
	edges[0] = dimsize;

	/* Read and verify data of Variable 1 */
	/* read data should be: 300  301  302  303  400  401 */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid1, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result1[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 1\n", outdata[ii], result1[ii], ii);
	}

	/* Read and verify data of Variable 2 */
	/* read data should be: 102  104  -2  -2  -2  -2 */
edges[0] = 2;
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != tempresult[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 2\n", outdata[ii], result2[ii], ii);
	}
    } /* end read data */

    {  /* Append to variable with gap */
	long rh_start[] = {4};
	long rh_edges[] = {3};
	short data2[] = {200, 201, 202};

	/* Write 3 elements to Variable 2 at index 4 */
	status = ncvarput(ncid, varid2, rh_start, rh_edges, (void *)data2);
	CHECK(status, -1, "ncvarget");
    }

    /* Get variable information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid2, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 7, "ncdiminq");

    { /* Read both variables */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize-1;

	/* Read and verify data of Variable 1 */
	/* read data should be: 300  301  302  303  400  401  -1 */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid1, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result1[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 1\n", outdata[ii], result1[ii], ii);
	}

	/* Read and verify data of Variable 1 */
	/* read data should be: 102  104  -2  -2  200  201  202 */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result2[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 2\n", outdata[ii], result2[ii], ii);
	}
    } /* end read data */

    {  /* Append to a variable, with gap */
	long rh_start[] = {0};
	long rh_edges[] = {3};
	short data2[] = {500, 501, 502};
	rh_start[0] = dimsize+5;

	/* Write 3 elements to Variable 1 at index 'dimsize+5', leaving a gap
	   of 5 records that will be filled with fill value */
	status = ncvarput(ncid, varid1, rh_start, rh_edges, (void *)data2);
	CHECK(status, -1, "ncvarput");
	/* data now: 300 301 302 303 400 401 -1 -1 -1 -1 -1 -1 500 501 502 */
    }

    /* Get variable information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid2, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 15, "ncdiminq");

    { /* Read both variables */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize;

	/* Read and verify data of Variable 1; read data should be: */
	/* 300 301 302 303 400 401 -1 -1 -1 -1 -1 -1 500 501 502    */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid1, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result1[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 1\n", outdata[ii], result1[ii], ii);
	}

	/* Read and verify data of Variable 2; read data should be: */
	/* 102 104 -2 -2 200 201 202 -2 -2 -2 -2 -2 -2 -2 -2 */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	for (ii = 0; ii < edges[0]; ii++) {
            if (outdata[ii] != result2[ii])
                fprintf(stderr,"test_1dim_multivars: Read data %d doesn't match input %d at index %d in Variable 2\n", outdata[ii], result2[ii], ii);
	}
    } /* end read data */

#if NOTRUN
/* The following test is to verify that attempting to read pass the maximum 
   number of records in the file will fail.  However, because ncvarget leads
   to the internal function NCcoordck which uses NCadvise to display the 
   error and exit() so the test is disabled but the code should remain here 
   for future testing. -BMR, Dec 30, 2008 */
    { /* Read both variables pass the end by 2 records, should fail */
	long start[] = {0};
	long edges[1];
	edges[0] = dimsize+2;

	/* read data of Variable 1; should fail */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid1, start, edges, outdata);
	VERIFY(status, -1, "ncvarget");

	/* read data of Variable 1; should fail */
	HDmemset(outdata, 0, DIM0);
	status = ncvarget(ncid, varid2, start, edges, outdata);
	VERIFY(status, -1, "ncvarget");

    } /* end read data */
#endif

    /* Close file */
    status = ncclose(ncid);
    CHECK(status, -1, "ncclose");

    return num_errs;
} /* test_1dim_multivars */


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

#define DIM1 3
#define DIM2 2
#define FILENAME3 "multidimvar.nc"
static int test_multidim_singlevar()
{
    int   ncid;			/* file id */
    int   varid;		/* variable id */
    int   time_dim;		/* unlimited dimension */
    int   dim1, dim2;		/* other dimension ids */
    int   dims[3];		/* variable shapes */
    long  dimsize = 0,		/* unlimited dimension size */
	  dimsize1 = 0, dimsize2 = 0; /* other dimension sizes */
    short outdata[DIM0][DIM1][DIM2];	/* data read back */
    char  varname[12];		/* variable name */
    nc_type rh_type;		/* variable type */
    int   rh_ndims;		/* number of dims */
    int   rh_dims[H4_MAX_VAR_DIMS];	/* variable shape */
    int   rh_natts;		/* number of attributes */
    int   ii, jj, kk;
    short fillval = -3;		/* fill value for the variable */
    intn  status = 0;		/* returned by called functions */
    intn  num_errs = 0;		/* number of errors so far */

    /* result data to compare against read data; the first two elements will
	be changed to "1,2" later for the last test. */
    short result[DIM0][DIM1][DIM2] = {300,-3,-3,-3,-3,-3,301,-3,-3,-3,-3,-3,302,-3,-3,-3,-3,-3,303,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,400,-3,-3,-3,-3,-3,401,-3,-3,-3,-3,-3,800,801,802,803,804,805};

    /* Enter define mode */
    ncid = nccreate(FILENAME3, NC_CLOBBER);
    CHECK(ncid, -1, "nccreate");

    /* variables will be filled with fill values when data is missing */
    status = ncsetfill(ncid, NC_FILL);
    CHECK(status, -1, "ncsetfill");

    /* Define the unlimited dimension */
    time_dim = ncdimdef(ncid, "time", NC_UNLIMITED);
    CHECK(status, -1, "ncdimdef");
    dim1 = ncdimdef(ncid, "dimension 1", 3L);
    CHECK(dim1, -1, "ncdimdef");
    dim2 = ncdimdef(ncid, "dimension 2", 2L);
    CHECK(dim2, -1, "ncdimdef");

    /* Define the variable */
    dims[0] = time_dim;
    dims[1] = dim1;
    dims[2] = dim2;
    varid = ncvardef (ncid, "Variable 3D", NC_SHORT, 3, dims);
    CHECK(varid, -1, "ncvardef");

    /* Write the fill value attribute */
    status = ncattput(ncid, varid, "_FillValue", NC_SHORT, 1, &fillval);
    CHECK(status, -1, "ncattput");

    /* Leave define mode */
    status = ncendef (ncid);
    CHECK(status, -1, "ncendef");

    {  /* Write data the first time */
	long rh_start[] = {0,0,0};	/* starting writing at beginning */
	long rh_edges[] = {4,1,1};	/* 4x1x1 slab */
	short data[] = {300,301,302,303};

	/* Write 4 elements starting at index 0 */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get variable information and size of the unlimited dimension */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    /* Close file */
    status = ncclose (ncid);
    CHECK(status, -1, "ncclose");

    /* Open file for reading and writing */
    ncid = ncopen(FILENAME3, NC_RDWR);
    CHECK(ncid, -1, "ncopen");

    /* Get access to the variable */
    varid = ncvarid(ncid, "Variable 3D");
    CHECK(varid, -1, "ncvarid");

    /* Get and verify variable info and size of the unlimited dimension again */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 4, "ncdiminq");

    status = ncdiminq(ncid, rh_dims[1], 0, &dimsize1);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize1, DIM1, "ncdiminq");

    status = ncdiminq(ncid, rh_dims[2], 0, &dimsize2);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize2, DIM2, "ncdiminq");

    { /* Read and verify data of the variable */
	long start[] = {0,0,0};
	long edges[] = {0,DIM1,DIM2};
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0*DIM1*DIM2);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* Verify data, should be:
		300  -3     301  -3     302  -3     303  -3
		 -3  -3      -3  -3      -3  -3      -3  -3
		 -3  -3      -3  -3      -3  -3      -3  -3
		unlimited dimension ---->
		(-3 is fill value)
	*/

	for (ii = 0; ii < edges[0]; ii++) 
	for (jj = 0; jj < DIM1; jj++)
	for (kk = 0; kk < DIM2; kk++)
	    if (outdata[ii][jj][kk] != result[ii][jj][kk])
                fprintf(stderr,"test_multidim_singlevar: Read data %d doesn't match input %d at indices %d,%d,%d\n", outdata[ii][jj][kk], result[ii][jj][kk], ii,jj,kk);
    } /* end read data */

    {  /* Append data to variable pass the end */
	long rh_start[] = {6,0,0};	/* the end is 3 */
	long rh_edges[] = {2,1,1};	/* two slabs */
	short data[] = { 400,401};

	/* Write 2 values starting at index 6 */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get and verify variable info and size of the unlimited dimension again */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 8, "ncdiminq");

    { /* Read variable after appending with gap */
	long start[] = {0,0,0};
	long edges[] = {0,DIM1,DIM2};
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0*DIM1*DIM2);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be
		300  -3     301  -3     302  -3     303  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...

		... -3  -3      -3  -3     400  -3     401  -3
		... -3  -3      -3  -3      -3  -3      -3  -3
		... -3  -3      -3  -3      -3  -3      -3  -3
		unlimited dimension ---->
		(-3 is fill value)
	*/
	for (ii = 0; ii < edges[0]; ii++) 
	for (jj = 0; jj < DIM1; jj++)
	for (kk = 0; kk < DIM2; kk++)
	    if (outdata[ii][jj][kk] != result[ii][jj][kk])
                fprintf(stderr,"test_multidim_singlevar: Read data %d doesn't match input %d at indices %d,%d,%d\n", outdata[ii][jj][kk], result[ii][jj][kk], ii,jj,kk);
    } /* end read data */

    {  /* Write variable immediately pass the end */
	long rh_start[] = {0,0,0};	/* the end is 3 */
	long rh_edges[] = {1,3,2};	/* two elements */
	short data[] = {800,801,802,803,804,805};
	rh_start[0] = dimsize;	/* starting immediatly pass dimension size */

	/* Write 3 elements to the variable starting at 'dimsize' */
	status = ncvarput(ncid, varid, rh_start, rh_edges, (void *)data);
	CHECK(status, -1, "ncvarput");
    }

    /* Get and verify variable info and size of the unlimited dimension */
    status = ncvarinq (ncid, varid, varname, &rh_type, &rh_ndims, rh_dims, &rh_natts);
    CHECK(status, -1, "ncvarinq");
    status = ncdiminq(ncid, rh_dims[0], 0, &dimsize);
    CHECK(status, -1, "ncdiminq");
    VERIFY(dimsize, 9, "ncdiminq");

    { /* read variable */
	long start[] = {0,0,0};
	long edges[] = {0,DIM1,DIM2};
	edges[0] = dimsize;
	HDmemset(outdata, 0, DIM0*DIM1*DIM2);
	status = ncvarget(ncid, varid, start, edges, outdata);
	CHECK(status, -1, "ncvarget");

	/* verify data, should be
		300  -3     301  -3     302  -3     303  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...
		 -3  -3      -3  -3      -3  -3      -3  -3 ...

		... -3  -3      -3  -3     400  -3     401  -3     800 801
		... -3  -3      -3  -3      -3  -3      -3  -3     802 803
		... -3  -3      -3  -3      -3  -3      -3  -3     804 805
		unlimited dimension ---->
		(-3 is fill value)
	*/
	for (ii = 0; ii < edges[0]; ii++) 
	for (jj = 0; jj < DIM1; jj++)
	for (kk = 0; kk < DIM2; kk++)
	    if (outdata[ii][jj][kk] != result[ii][jj][kk])
                fprintf(stderr,"test_multidim_singlevar: Read data %d doesn't match input %d at indices %d,%d,%d\n", outdata[ii][jj][kk], result[ii][jj][kk], ii,jj,kk);
    } /* end read data */

    /* Close file */
    status = ncclose(ncid);
    CHECK(status, -1, "ncclose");

    return 0;
}


/* Test driver for testing reading/writing variables with unlimited dimension 
   using nc API. */
extern int test_ncunlim()
{
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("read/write variables with unlimited dims via nc API (tncunlim.c)");

    /* Test single variable with 1 unlimited dimension */
    num_errs = num_errs + test_1dim_singlevar();

    /* Test multiple variables with 1 unlimited dimension */
    num_errs = num_errs + test_1dim_multivars();

    /* Test multiple variables with multiple dimensions */
    num_errs = num_errs + test_multidim_singlevar();

    if (num_errs == 0) PASSED();
    return num_errs;
}
