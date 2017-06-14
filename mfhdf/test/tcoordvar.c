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
 * tcoordvar.c - tests some aspects of coordinate variables
 * Structure of the file:
 *    test_coordvar - test driver
 *	  test_dim1_SDS1 - tests that data is not corrupted when the
 *		dimension of SDS #1 is named the same as that of SDS #1.
 *		(previous bug: writing to the dimension would corrupt the SDS) 
 *
 *	  test_dim1_SDS2 - tests that data is not corrupted when the
 *		dimension of SDS #1 is named the same as that of SDS #2.
 *		(previous bug: writing to the SDS #2 would corrupt the 
 *		 dimension)
 *
 *	  test_named_vars - tests that all variables of a given name are 
 *		account for.
 *		(problem: SDnametoindex only returns the first SDS of the
 *		inquired name.  Two new APIs provide number of all variables
 *		of the same name and retrieve a list of indices of the
 *		named variables.)
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

/********************************************************************
   Name: test_dim1_SDS1() - tests that data is not corrupted when a 
			dimension is named the same as its SDS.

   Description:
	In the past, naming a dimension the same as its SD and writing
	dimension scale to it will corrupt the SDS' data.  This routine 
	tests the provided fix (bugzilla 624) for this situation.

        The main contents are listed below.  Note that when a function name
	appears in the parentheses, it indicates that the associate step 
	specificly tests the changes made to that function.

	- create a one-dim SDS, named VAR1_NAME
	- name its dimension VAR1_NAME
	- get file information and verify that there is only 1 variable, 
	  dataset VAR1_NAME
	- set attribute to dimension "Variable 1" (SDsetattr)
	- set attribute to SDS "Variable 1" (SDsetattr)
	- get file information and verify that there are 2 variable, 
	  dataset VAR1_NAME and coordinate variable VAR1_NAME
	- write data to the SDS
	- close all and reopen the file
	- open dataset "Variable 1" (SDnametoindex)
	- verify that this variable is not a coordinate variable (SDiscoordvar)
	- read and verify its attribute information and values
	- get access to the dataset's first dimension
	- read and verify its attribute information and values
	- read data and verify that the data is not corrupted

   Return value:
        The number of errors occurred in this routine.

   BMR - May 18, 2007
*********************************************************************/

#define VAR1_NAME	"Variable 1"
#define ATTR1_NAME	"Attribute Dimension 1"
#define ATTR2_NAME	"Attribute SDS 1"
#define ATTR1_VAL	"This is a coord var"
#define ATTR2_VAL	"This is not a coord var"
#define ATTR1_LEN	19
#define ATTR2_LEN	23
#define FILE1		"sds1_dim1_samename.hdf"

static intn test_dim1_SDS1(void)
{
    char  sds_name[20];
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 out_data[5];
    int32 dimsize[1];
    int32 sds_id, file_id, dim_id, index;
    int32 start=0, stride=1;
    int32 scale1 [5] = {101,102,103,104,105}, scale1_out[5];
    int32 num_type, array_rank, count;
    int32 n_datasets, n_file_attrs, n_local_attrs, n_vars = 0;
    intn  datanum, ranknum, status =0, i, idx, idx1, idx2;
    hdf_varlist_t* var_list;
    intn  is_coord = FALSE;
    char  attr_name[H4_MAX_NC_NAME], attr_values[80];
    intn  num_errs = 0;         /* number of errors so far */

    file_id = SDstart(FILE1, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    /* Create a one-dim dataset named VAR1_NAME, of type DFNT_FLOAT32. */
    dimsize[0] = 5;
    sds_id = SDcreate(file_id, VAR1_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the dimension name to be the same as its dataset. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    status = SDsetdimname(dim_id, VAR1_NAME);
     /* status = SDsetdimname(dim_id, VAR1_NAME);
 */ 
    CHECK(status, FAIL, "SDsetdimname");

    /* Get file info and verify that there is 1 dataset in the file. */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 1, "SDfileinfo");

    /* Set an attribute to dimension VAR1_NAME. */
    status = SDsetattr(dim_id, ATTR1_NAME, DFNT_CHAR8, ATTR1_LEN, ATTR1_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Set an attribute to dataset VAR1_NAME. */
    status = SDsetattr(sds_id, ATTR2_NAME, DFNT_CHAR8, ATTR2_LEN, ATTR2_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Get file info and verify that there are 2 datasets in the file:
       1 SDS and 1 coordinate variable (because of SDsetattr dim) */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 2, "SDfileinfo");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, &start, &stride, dimsize, (VOIDP)sds1_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check its data */
    file_id = SDstart(FILE1, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Check variable type and attributes of each element in the file */

    /* Get the number of variables of name VAR1_NAME */
    status = SDgetnumvars_byname(file_id, VAR1_NAME, &n_vars);

    if (n_vars == 1)
    {
	/* Get index of dataset VAR1_NAME */
	index = SDnametoindex(file_id, VAR1_NAME);
	CHECK(index, FAIL, "SDnametoindex");
    }
    else
    {
	/* Get the list of all variables of named VAR1_NAME */
	var_list = (hdf_varlist_t *)HDmalloc(n_vars * sizeof(hdf_varlist_t));
	status = SDnametoindices(file_id, VAR1_NAME, var_list);

	/* In this case, the first variable is a dataset */
	for (idx = 0; idx < n_vars; idx++)
	{
	    if (var_list[idx].var_type == IS_SDSVAR)
	    {
		index = var_list[idx].var_index;
		VERIFY(index, 0, "SDnametoindices");
	    }
	}
    }

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Verify that this variable is a dataset. */
    is_coord = SDiscoordvar(sds_id);
    VERIFY(is_coord, FALSE, "SDiscoordvar");

    /* Read and verify the information of the SDS' first attribute. */
    status = SDattrinfo(sds_id, 0, attr_name, &num_type, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, ATTR2_LEN, "SDattrinfo");
    VERIFY(HDstrncmp(attr_name, ATTR2_NAME, 14), 0, "SDattrinfo");

    /* Read and verify the values of the SDS' first attribute. */
    status = SDreadattr(sds_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (HDstrncmp(attr_values, ATTR2_VAL, ATTR2_LEN) != 0)
    {
	fprintf(stderr, "Unmatched attribute values for SDS %s: is <%s>, should be <%s>\n", VAR1_NAME, attr_values, ATTR2_VAL);
	num_errs++;
    }

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Read and verify the information of the dimension's first attribute. */
    status = SDattrinfo(dim_id, 0, attr_name, &num_type, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, 19, "SDattrinfo");
    VERIFY(HDstrncmp(attr_name, ATTR1_NAME, 21), 0, "SDattrinfo");

    /* Read and verify the values of the dimension's first attribute. */
    status = SDreadattr(dim_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (HDstrncmp(attr_values, ATTR1_VAL, ATTR1_LEN) != 0)
    {
	fprintf(stderr, "Unmatched attribute values for dimension %s: is <%s>, should be <%s>\n", VAR1_NAME, attr_values, ATTR1_VAL);
	num_errs++;
    }

    /* Verify again that the number of datasets in the file is 2, 1 SDS and
       1 coordinate variable */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 2, "SDfileinfo");
    VERIFY(n_file_attrs, 0, "SDfileinfo");

    /* Read and verify the dataset's data */
    status = SDreaddata (sds_id, &start, NULL, dimsize, &out_data);
    CHECK(status, FAIL, "SDreaddata");

    for (idx1 = 0; idx1 < dimsize[0]; idx1++)
        if (out_data[idx1] != sds1_data[idx1])
	{
	    fprintf(stderr, "Read value (%f) differs from written (%f) at [%d]\n", out_data[idx1], sds1_data[idx1], idx1);
		num_errs++;
	}

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_dim1_SDS1 */


/********************************************************************
   Name: test_dim1_SDS2() - tests that data is not corrupted when a 
			dimension is named the same as that of another SDS.

   Description:
	In the past, naming a dimension the same as an SDS and writing
	dimension scale to it will corrupt the SDS' data.  This routine 
	tests the provided fix (bugzilla 624) for this situation.

        The main contents are listed below.  Note that when a function name
	appears in the parentheses, it indicates that the associate step 
	specificly tests the changes made to that function.

	- create a one-dim SDS, named "Variable 1"
	- name its dimension "Variable 2"
	- create a two-dim SDS, named "Variable 2", and write data to it
	- set dimension scale to dimension "Variable 2" (SDsetdimscale)
	- write data to dataset "Variable 1"
	- close all and reopen the file
	- open dataset "Variable 1" (SDnametoindex)
	- get/verify the first dimension's scales of SDS (SDgetdimscale)
	- open dataset "Variable 2" (SDnametoindex)
	- read data and verify that the data is not corrupted

   Return value:
        The number of errors occurred in this routine.

   BMR - May 18, 2007
*********************************************************************/

#define VAR1_NAME "Variable 1"
#define VAR2_NAME "Variable 2"
#define FILE2 "sds2_dim1_samename.hdf"

static intn test_dim1_SDS2(void)
{
    char  sds_name[20];
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 sds2_data[2][3] = {{0.1, 2.3, 4.5}, {4.5, 6.7, 8.9}};
    int32 dimsize[1], dimsize2[2];
    int32 sds1_id, sds2_id, file_id, dim_id, index;
    int32 start=0, stride=1, stat;
    int32 start2[2]={0,0}, stride2[2]={1,1};
    int32 scale1 [5] = {101,102,103,104,105}, scale1_out[5];
    int32 num_type, array_rank, attributes;
    int32 n_datasets, n_file_attrs, n_local_attrs;
    float32 out_data2[2][3];
    intn  datanum, ranknum, status =0, i, idx, idx1, idx2;
    intn  num_errs = 0;         /* number of errors so far */

    file_id = SDstart(FILE2, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    dimsize[0] = 5;
    dimsize2[0] = 2;
    dimsize2[1] = 3;
    sds1_id = SDcreate(file_id, VAR1_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds1_id, FAIL, "SDcreate");

    /* Set the dimension name to be the same as the next dataset (not 
       created yet) */
    dim_id = SDgetdimid(sds1_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    status = SDsetdimname(dim_id, VAR2_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Get file info and verify that there is 1 dataset in the file */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 1, "SDfileinfo");

    /* Create and write data to the second dataset VAR2_NAME */
    sds2_id = SDcreate(file_id, VAR2_NAME, DFNT_FLOAT32, 2, dimsize2);
    CHECK(sds2_id, FAIL, "SDcreate");

    stat = SDwritedata(sds2_id, start2, stride2, dimsize2, sds2_data);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Get file info and verify that there are 2 datasets in the file. */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 2, "SDfileinfo");

    /* Write values to the dimension VAR2_NAME (same name as VAR2_NAME) */
    status = SDsetdimscale (dim_id, dimsize[0], DFNT_INT32, scale1);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Get file info and verify that there are 3 datasets in the file:
       2 SDS and 1 coordinate variable */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 3, "SDfileinfo");

    /* Close dataset and file */
    status = SDendaccess(sds1_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check its data */
    file_id = SDstart(FILE2, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Verify dimension scale of the first dimension of SDS VAR1_NAME */

    /* Get access to dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds1_id = SDselect(file_id, index);
    CHECK(sds1_id, FAIL, "SDselect");

    /* Get access to its first dimension */
    dim_id = SDgetdimid(sds1_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension scale and verify the values */
    status = SDgetdimscale (dim_id, scale1_out);
    CHECK(status, FAIL, "SDgetdimscale");

    for (idx = 0; idx < dimsize[0]; idx++)
        if (scale1_out[idx] != scale1[idx])
	{
	    fprintf(stderr, "Read value (%d) differs from written (%d) at [%d]\n", scale1_out[idx], scale1[idx], idx);
	    num_errs++;
	}
    /* End verifying dimension scale */

    /* Verify dimension scale of the first dimension of SDS VAR1_NAME */

    /* Get access to dataset VAR2_NAME */
    index = SDnametoindex(file_id, VAR2_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds2_id = SDselect(file_id, index);
    CHECK(sds2_id, FAIL, "SDselect");

    /* Get dataset's info and verify them */
    status = SDgetinfo(sds2_id, sds_name, &array_rank, dimsize2,
                                        &num_type, &n_local_attrs);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(array_rank, 2, "SDfileinfo");
    VERIFY(num_type, DFNT_FLOAT32, "SDfileinfo");
    VERIFY(n_local_attrs, 0, "SDfileinfo");

    /* Read and verify the dataset's data */
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

    /* Verify again that the number of datasets in the file is 3, 2 SDSs and
       1 coordinate variable */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 3, "SDfileinfo");

    status = SDendaccess(sds1_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_dim1_SDS2 */


/********************************************************************
   Name: test_named_vars() - tests that data is not corrupted when a 
			dimension is named the same as that of another SDS.

   Description:
	In the past, SDnametoindex returns the index of the first variable
	of the inquired name although there might be more than one variable
	having such name.  After the fix of bugzilla 624, SDnametoindex was
	modified to return the first SDS variable of the given name if the 
	name is not unique.  Two new APIs, SDgetnumvars_byname and 
	SDnametoindices, will provide the number of variables of a given
	name, and a list of indices for those variables.  This test will
	concentrate on these new APIs.

        The main contents are listed below:

	- create two two-dim SDS, named "Common Name"  <- 2 "Common Name"s
	- create a one-dim SDS, named "One Dimension"  <- 1 "One Dimension"
	- name its dimension "Common Name"  <- still 2 "Common Name"s
	- set dimension scale to dimension "Common Name"  <- 3 "Common Name"s
	- create a one-dim SDS, named "One Dimension"  <- 2 "One Dimension"s
	- name its dimension "Another Name"
	- create a one-dim SDS, named "Another Name"  <- still 1 "Another Name"s
	- set attribute to dimension "Another Name"  <- 2 "Another Name"s
	- close all and reopen the file
	- open dataset "Common Name" (SDnametoindex)
	- this will have two dimensions
	- get number of variables of name "Common Name" and verify that it's 3
	- get the list of indices of "Common Name" and it should be
		[0, IS_SDSVAR], [1, IS_SDSVAR], [3, IS_CRDVAR]
	- get number of variables of name "Another Name" and verify that it's 2
	- get the list of indices of "Another Name" and it should be
		[5, IS_SDSVAR], [6, IS_CRDVAR]
	- get/verify the dimension scales of dimension "Common Name"
	- get/verify the attribute value of dimension "Another Name"

        The file contains these items:
        Index   Var Type	Name	Dimensions
          0     SDS             "Common Name"   2-dim
          1     SDS             "Common Name"   2-dim
          2     SDS             "One Dimension" 1-dim
          3     Coordinate      "Common Name"   1-dim
          4     SDS             "One Dimension" 1-dim
          5     SDS             "Another Name"  1-dim
          6     Coordinate      "Another Name"  1-dim

        If someone changes the file contents in this test, please update
        this table appropriately.

   Return value:
        The number of errors occurred in this routine.

   BMR - Dec 30, 2007
*********************************************************************/

#define COMMON_NAME "Common Name"
#define ONEDIM_NAME "One Dimension"
#define ANOTHER_NAME "Another Name"
#define FILE3 "vars_samename.hdf"

static intn test_named_vars(void)
{
    char  sds_name[20];
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 sds2_data[2][3] = {{0.1, 2.3, 4.5}, {4.5, 6.7, 8.9}};
    int32 dimsize[1], dimsize2[2];
    int32 sds_id, sds1_id, sds2_id, sds3_id, sds4_id, sds5_id;
    int32 file_id, dim_id, index;
    int32 start=0, stride=1, stat;
    int32 start2[2]={0,0}, stride2[2]={1,1};
    int32 scale1 [5] = {101,102,103,104,105}, scale1_out[5];
    int32 array_rank;
    int32 n_datasets, n_file_attrs, n_local_attrs, n_vars=0;
    float32 out_data2[2][3];
    intn  datanum, ranknum, status =0, idx, idx1, idx2;
    intn  is_coordvar=FALSE;
    hdf_varlist_t *allvars, *varlistp;
    intn  num_errs = 0;         /* number of errors so far */
    char  line[40];
    char  contents[7][40]={
          "#0 SDS        2-dim 'Common Name'",
          "#1 SDS        2-dim 'Common Name'",
          "#2 SDS        1-dim 'One Dimension'",
          "#3 Coordinate 1-dim 'Common Name'",
          "#4 SDS        1-dim 'One Dimension'",
          "#5 SDS        1-dim 'Another Name'",
          "#6 Coordinate 1-dim 'Another Name'"};

    file_id = SDstart(FILE3, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    dimsize2[0] = 2;
    dimsize2[1] = 3;

    /* Create first COMMON_NAME data set. */
    sds1_id = SDcreate(file_id, COMMON_NAME, DFNT_FLOAT32, 2, dimsize2);
    CHECK(sds1_id, FAIL, "SDcreate");
    status = SDendaccess(sds1_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Create second COMMON_NAME data set. */
    sds2_id = SDcreate(file_id, COMMON_NAME, DFNT_FLOAT32, 2, dimsize2);
    CHECK(sds2_id, FAIL, "SDcreate");
    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess");

    dimsize[0] = 5;
    sds3_id = SDcreate(file_id, ONEDIM_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds3_id, FAIL, "SDcreate");

    /* Set the dimension name to be the same as the previous 2 datasets */
    dim_id = SDgetdimid(sds3_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    status = SDsetdimname(dim_id, COMMON_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Get file info and verify that there are 3 datasets in the file */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 3, "SDfileinfo");

    /* Write values to the dimension COMMON_NAME (same name as first 2 datasets) */
    status = SDsetdimscale (dim_id, dimsize[0], DFNT_INT32, scale1);
    CHECK(status, FAIL, "SDsetdimscale");

    /* Get file info and verify that there are 4 datasets in the file */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 4, "SDfileinfo");

    dimsize[0] = 8;
    sds4_id = SDcreate(file_id, ONEDIM_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds4_id, FAIL, "SDcreate");

    /* Set the dimension name to be the same as the previous 2 datasets */
    dim_id = SDgetdimid(sds4_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    status = SDsetdimname(dim_id, ANOTHER_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    sds5_id = SDcreate(file_id, ANOTHER_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds5_id, FAIL, "SDcreate");

    /* Get file info and verify that there are 6 datasets in the file */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 6, "SDfileinfo");

    status = SDsetattr(dim_id, ATTR1_NAME, DFNT_CHAR8, ATTR1_LEN, ATTR1_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Get file info and verify that there are 7 datasets in the file */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 7, "SDfileinfo");

    /* Verify again that the number of datasets in the file is 7 */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 7, "SDfileinfo");

    /* There are 3 variables of name COMMON_NAME */
    status = SDgetnumvars_byname(file_id, COMMON_NAME, &n_vars);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_vars, 3, "SDfileinfo");

    allvars = (hdf_varlist_t *)HDmalloc(n_vars * sizeof(hdf_varlist_t));
    status = SDnametoindices(file_id, COMMON_NAME, allvars);
    CHECK(status, FAIL, "SDfileinfo");

    /* Compare file contents with predefined text to verify */
    for (idx = 0; idx < n_datasets; idx++)
    {

	sds_id = SDselect(file_id, idx);
	CHECK(sds_id, FAIL, "SDselect");
	status = SDgetinfo(sds_id, sds_name, &array_rank, NULL, NULL, NULL);
	CHECK(status, FAIL, "SDgetinfo");
	is_coordvar = SDiscoordvar(sds_id);
	if (is_coordvar)
	    sprintf(line,"#%d Coordinate %d-dim '%s'\n", idx, array_rank, 
								sds_name);
	else
	    sprintf(line,"#%d SDS        %d-dim '%s'\n", idx, array_rank, 
								sds_name);

	if (strncmp(contents[idx], line, strlen(contents[idx])) != 0)
	{
	    fprintf(stderr, "File contents are incorrect in testing variable types at variable of index %d\n", idx);
	}
    }

    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_named_vars */

/* Test driver for testing various coordinate variable features. */
extern int
test_coordvar()
{
    intn num_errs = 0;         /* number of errors */

    /* Output message about test being performed */
    TESTING("various coordinate variable features (tcoordvar.c)");

    /* test when a dimension being named the same as an SDS */
    num_errs = num_errs + test_dim1_SDS1();
    num_errs = num_errs + test_dim1_SDS2();
    num_errs = num_errs + test_named_vars();

    if (num_errs == 0)
        PASSED();

    return num_errs;
}

#endif /* HDF */
