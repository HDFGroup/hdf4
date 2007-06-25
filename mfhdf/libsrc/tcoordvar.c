/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 * tcoordvar.c - tests some aspects of coordinate variables
 * Structure of the file:
 *    test_coordvar - test driver
 *	  test_same_name_corrupted - tests ability to handle the situation, 
 *		where a dimension being named the same as an SDS.
 *		* if SDS is multi-dim, the library works correctly
 *		* if SDS is one-dim, the library cannot distinguish
 *		  between an SDS var and a coordinate var, which results in
 *		  unpredictable behaviors in subsequent accesses to the dim.
 *		  (bugzilla #328)
 *	  test_another_SDS - tests that data is not corrupted when a
 *		dimension is named the same as that of another SDS.

****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define FILE_NAME	"tcoordvars.hdf"  /* data file to test coord vars */
#define X_LENGTH	10
#define Y_LENGTH	14
#define RANK1		1
#define RANK2		2
#define SDS1_NAME	"One Dim"
#define SDS2_NAME	"Two Dims"
#define DIM_NAME1	"One Dim"
#define DIM_NAME2	"Two Dims"

static intn test_same_name_corrupted()
{
    int32 fid, dset1, dset2, dim_id1, dim_id2;
    int32 dimsize1[RANK1], dimsizes[RANK2], size, dim_type, dim_attrs;
    int32 start[RANK2], edges[RANK2];
    int32 array_data[X_LENGTH];
    int32 scale0[] = {0,1,2,3,4,5,6,7,8,9};
    char  dim_name[64];
    intn  i, j, status;
    intn  num_errs = 0;         /* number of errors so far */

    /* Create a file */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Create an X_LENGTH by Y_LENGTH dataset, called DataSet_1 */
    dimsize1[0] = X_LENGTH;
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;
    dset1 = SDcreate(fid, "One Dim", DFNT_INT32, RANK1, dimsize1);
    CHECK(dset1, FAIL, "SDcreate");

    /* Create another X_LENGTH by Y_LENGTH dataset, called DataSet_2 */
    dset2 = SDcreate(fid, "Two Dims", DFNT_FLOAT64, RANK2, dimsizes);
    CHECK(dset2, FAIL, "SDcreate");

    /* Fill the stored-data array with values. */
    for (i = 0; i < X_LENGTH; i++) {
	array_data[i] = i + 1;
    }

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK1; i++) {
	start[i] = 0;
	edges[i] = dimsize1[i];
    }

    /* Set name to the first dimensions of both datasets */
    dim_id1 = SDgetdimid (dset1, 0);
    CHECK(dim_id1, FAIL, "SDgetdimid");

    status = SDsetdimname (dim_id1, "One Dim");
    CHECK(status, FAIL, "SDsetdimname");

    dim_id2 = SDgetdimid (dset2, 0);
    CHECK(dim_id2, FAIL, "SDgetdimid");

    status = SDsetdimname (dim_id2, "Two Dims");
    CHECK(status, FAIL, "SDsetdimname");

    /* Get information of dim_id1; this dimension doesn't have any 
	attributes */
    status = SDdiminfo(dim_id1, dim_name, &size, &dim_type, &dim_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(HDstrcmp(dim_name, DIM_NAME1), 0, "SDdiminfo");
    VERIFY(dim_attrs, 0, "SDdiminfo");

    /* Set a character attribute for data set dset1 */
    status = SDsetattr(dset1, "attribute 1", DFNT_CHAR8, 15, "not a coord var");
    CHECK(status, FAIL, "SDsetattr");

    /* Now, get dimension info again */
    status = SDdiminfo(dim_id1, dim_name, &size, &dim_type, &dim_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(dim_attrs, 0, "SDdiminfo");
    /*  Note that, in hdf4.2r1 and earlier, a corruption problem occurred when 
	a dimension is named the same as an SDS, and the dimension "One Dim" 
	would be mistaken as the SDS "One Dim".  As the result, dim_attrs would
	have been 1 instead, which is incorrect */

    /* Get information of dim_id2; this dimension doesn't have any 
	attributes */
    status = SDdiminfo(dim_id2, dim_name, &size, &dim_type, &dim_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(HDstrcmp(dim_name, DIM_NAME2), 0, "SDdiminfo");
    VERIFY(dim_attrs, 0, "SDdiminfo");

    /* Set a character attribute for data set dset2 */
    status = SDsetattr(dset2, "attribute 2", DFNT_CHAR8, 22, "not a coord var either");
    CHECK(status, FAIL, "SDsetattr");

    /* After this call, a coordinate variable is made for this dimension */
    status = SDsetdimscale(dim_id2, dimsizes[0], DFNT_INT32, scale0);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Now, get dimension info again */
    status = SDdiminfo(dim_id2, dim_name, &size, &dim_type, &dim_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY(HDstrcmp(dim_name, SDS2_NAME), 0, "SDdiminfo");
    /* Although this dimension has the same name as the SDS, "Two Dims,"
	the correct coordinate variable is accessed because the SDS has
	more than one dimension, distinguishing it from the coordinate 
	variable of the same name. (bugzilla #328) */
    VERIFY(dim_attrs, 0, "SDdiminfo");  /* still no attributes */

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
}   /* test_same_name_corrupted */


/********************************************************************
   Name: test_another_SDS() - tests that data is not corrupted when a 
			dimension is named the same as that of another SDS.

   Description:
	In the past, naming a dimension the same as an SDS and writing
	dimension scale to it will corrupt the SDS' data.  This routine 
	tests the provided fix (bugzilla 624) for the situation where 
	the dimension of one SDS is named the same as that of another SDS.

        The main contents are listed below.  Note that when a function name
	appears in the parentheses, it indicates that the associate step 
	specificly tests the changes made to that function.

	- create a one-dim SDS, named "Variable 1"
	- name its dimension "Variable 2"
	- create a two-dim SDS, named "Variable 2", and write data to it
	- create another two-dim SDS, named "Variable 3", and write data 
	  to it (just for population, no significant purpose though)
	- write data to dataset "Variable 1"
	- set dimension scale to dimension "Variable 2" (SDsetdimscale)
	- close all and reopen the file
	- open dataset "Variable 1" (SDnametoindex)
	- get/verify the dimension's scales of SDS "Variable 1" (SDgetdimscale)
	- open dataset "Variable 2" (SDnametoindex)
	- read data and verify that the data is not corrupted

   Return value:
        The number of errors occurred in this routine.

   BMR - May 18, 2007
*********************************************************************/

#define DSET1 "Variable 1"
#define DSET2 "Variable 2"
#define DSET3 "Variable 3"
#define INFILE "SDSdim_samename.hdf"

static intn test_another_sds(void)
{
    char  sds_name[20];
    int32 datatype = DFNT_FLOAT32;
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 sds2_data[2][3] = {{0.1, 2.3, 4.5}, {4.5, 6.7, 8.9}};
    int32 dimsize[1], dimsize2[2];
    int32 sds_id, sds2_id, file_id, dim_id, index;
    int32 start=0, stride=1, stat;
    int32 start2[2]={0,0}, stride2[2]={1,1};
    int32 scale1 [5] = {101,102,103,104,105}, scale1_out[5];
    int32 num_type, array_rank, attributes;
    int32 n_datasets, n_file_attr;
    float32 out_data2[2][3];
    intn  datanum, ranknum, status =0, i, idx, idx1, idx2;
    char  inputfile[80];
    intn  num_errs = 0;         /* number of errors so far */

    file_id = SDstart(INFILE, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    dimsize[0] = 5;
    dimsize2[0] = 2;
    dimsize2[1] = 3;
    sds_id = SDcreate(file_id, DSET1, datatype, 1, dimsize);
    CHECK(sds_id, FAIL, "SDcreate");

    /*  Set the dimension name to be the same as the next dataset (not 
	created yet) */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    status = SDsetdimname(dim_id, DSET2);
    CHECK(status, FAIL, "SDsetdimname");

    status = SDfileinfo(file_id, &n_datasets, &n_file_attr);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 1, "SDfileinfo");

    sds2_id = SDcreate(file_id, DSET2, datatype, 2, dimsize2);
    CHECK(sds2_id, FAIL, "SDcreate");

    stat = SDwritedata(sds2_id, start2, stride2, dimsize2, sds2_data);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    sds2_id = SDcreate(file_id, DSET3, datatype, 2, dimsize2);
    CHECK(sds2_id, FAIL, "SDcreate");

    status = SDwritedata(sds2_id, start2, stride2, dimsize2, sds2_data);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDfileinfo(file_id, &n_datasets, &n_file_attr);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 3, "SDfileinfo");

    status = SDwritedata(sds_id, &start, &stride, dimsize, sds1_data);
    CHECK(status, FAIL, "SDwritedata");

    status = SDsetdimscale (dim_id, dimsize[0], DFNT_INT32, scale1);
    CHECK(status, FAIL, "SDsetdimscale");

    status = SDfileinfo(file_id, &n_datasets, &n_file_attr);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 4, "SDfileinfo");

    /* close dataset and file */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* open the file again to check its info */
    file_id = SDstart(INFILE, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* try opening DSET1 */
    index = SDnametoindex(file_id, DSET1);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    status = SDgetdimscale (dim_id, scale1_out);
    CHECK(status, FAIL, "SDgetdimscale");

    for (idx = 0; idx < dimsize[0]; idx++)
        if (scale1_out[idx] != scale1[idx])
	{
	    fprintf(stderr, "Read value (%d) differs from written (%d) at [%d]\n", scale1_out[idx], scale1[idx], idx);
	    num_errs++;
	}

    /* Open DSET2, read its data and verify that the data is not corrupted */
    index = SDnametoindex(file_id, DSET2);
    CHECK(index, FAIL, "SDnametoindex");

    sds2_id = SDselect(file_id, index);
    CHECK(sds2_id, FAIL, "SDselect");

    status = SDgetinfo(sds2_id, sds_name, &array_rank, dimsize2,  \
                                        &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(array_rank, 2, "SDfileinfo");

    status = SDreaddata (sds2_id, start2, NULL, dimsize2, &out_data2);
    CHECK(status, FAIL, "SDreaddata");

    for (idx1 = 0; idx1 < dimsize2[0]; idx1++)
	for (idx2 = 0; idx2 < dimsize2[1]; idx2++)
	{
            if (out_data2[idx1][idx2] != sds2_data[idx1][idx2])
	    {
		fprintf(stderr, "Read value (%f) differs from written (%f) at [%d][%d]\n", out_data2[idx1][idx2], sds2_data[idx1][idx2], idx1, idx2);
		num_errs++;
	    }
	}

    status = SDfileinfo(file_id, &n_datasets, &n_file_attr);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 4, "SDfileinfo");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

printf("test_another_sds........................\n");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_another_sds */

/* Test driver for testing various coordinate variable features. */
extern int
test_coordvar()
{
    intn num_errs = 0;         /* number of errors */

    /* test when a dimension being named the same as its SDS */
    num_errs = num_errs + test_same_name_corrupted();

    /* test when a dimension being named the same as an SDS */
    num_errs = num_errs + test_another_sds();

    return num_errs;
}

#endif /* HDF */
