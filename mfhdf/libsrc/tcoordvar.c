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
 *	  test_dim1_SDS1 - tests that data is not corrupted when the
 *		dimension of SDS #1 is named the same as that of SDS #1.
 *		(previous bug: writing to the dimension would corrupt the SDS) 
 *
 *	  test_dim1_SDS2 - tests that data is not corrupted when the
 *		dimension of SDS #1 is named the same as that of SDS #2.
 *		(previous bug: writing to the SDS #2 would corrupt the 
 *		 dimension)

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

	- create a one-dim SDS, named "Variable 1"
	- name its dimension "Variable 1"
	- set attribute to dimension "Variable 1" (SDsetattr)
	- close all and reopen the file
	- open dataset "Variable 1" (SDnametoindex)
	- read data and verify that the data is not corrupted
	- verify that this variable is not a coordinate variable (SDiscoordvar)
	- get/verify the dimension's scales of dimension "Variable 1"
	- verify that this variable is a coordinate variable (SDiscoordvar)

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
    int32 datatype = DFNT_FLOAT32;
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 sds2_data[2][3] = {{0.1, 2.3, 4.5}, {4.5, 6.7, 8.9}};
    int32 dimsize[1], dimsize2[2];
    int32 sds_id, file_id, dim_id, index;
    int32 start=0, stride=1, stat;
    int32 start2[2]={0,0}, stride2[2]={1,1};
    int32 scale1 [5] = {101,102,103,104,105}, scale1_out[5];
    int32 num_type, array_rank, count;
    int32 n_datasets, n_file_attrs, n_local_attrs;
    intn  datanum, ranknum, status =0, i, idx, idx1, idx2;
    char  attr_name[MAX_NC_NAME], attr_values[80];
    intn  num_errs = 0;         /* number of errors so far */

    file_id = SDstart(FILE1, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    dimsize[0] = 5;
    dimsize2[0] = 2;
    dimsize2[1] = 3;
    sds_id = SDcreate(file_id, VAR1_NAME, datatype, 1, dimsize);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the dimension name to be the same as its dataset */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    status = SDsetdimname(dim_id, VAR1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Get file info and verify that there are 2 datasets in the file. */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 1, "SDfileinfo");

    /* Set an attribute to dimension VAR1_NAME */
    status = SDsetattr(dim_id, ATTR1_NAME, DFNT_CHAR8, ATTR1_LEN, ATTR1_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Set an attribute to dataset VAR1_NAME */
    status = SDsetattr(sds_id, ATTR2_NAME, DFNT_CHAR8, ATTR2_LEN, ATTR2_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Get file info and verify that there are 2 datasets in the file:
       1 SDS and 1 coordinate variable */
    status = SDfileinfo(file_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "SDfileinfo");
    VERIFY(n_datasets, 2, "SDfileinfo");

    /* Close dataset and file */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check its data */
    file_id = SDstart(FILE1, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Check variable type and attributes of each element in the file */

    /* Get access to dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    status = SDattrinfo(sds_id, 0, attr_name, &num_type, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, 23, "SDattrinfo");
    VERIFY(HDstrncmp(attr_name, ATTR2_NAME, 14), 0, "SDattrinfo");

    /* Read and verify the values of the SDS' attribute */
    status = SDreadattr(sds_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (HDstrncmp(attr_values, ATTR2_VAL, ATTR2_LEN) != 0)
    {
	fprintf(stderr, "Unmatched attribute values for SDS %s: is <%s>, should be <%s>\n", VAR1_NAME, attr_values, ATTR2_VAL);
	num_errs++;
    }

    /* Get access to its first dimension */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    status = SDattrinfo(dim_id, 0, attr_name, &num_type, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, 19, "SDattrinfo");
    VERIFY(HDstrncmp(attr_name, ATTR1_NAME, 21), 0, "SDattrinfo");

    /* Read and verify the values of the SDS' attribute */
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
	- get/verify the dimension's scales of SDS "Variable 1" (SDgetdimscale)
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
    int32 datatype = DFNT_FLOAT32;
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
    sds1_id = SDcreate(file_id, VAR1_NAME, datatype, 1, dimsize);
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
    sds2_id = SDcreate(file_id, VAR2_NAME, datatype, 2, dimsize2);
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
    VERIFY(num_type, datatype, "SDfileinfo");
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

/* Test driver for testing various coordinate variable features. */
extern int
test_coordvar()
{
    intn num_errs = 0;         /* number of errors */

    /* test when a dimension being named the same as an SDS */
    num_errs = num_errs + test_dim1_SDS1();
    num_errs = num_errs + test_dim1_SDS2();

    return num_errs;
}

#endif /* HDF */
