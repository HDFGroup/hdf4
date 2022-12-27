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

/****************************************************************************
 * tattributes.c - tests attribute features
 *	(currently only SDsetattr with count = 0)
 * Structure of the file:
 *    test_attributes - test driver
 *	  test_count - tests that SDsetattr fails when the parameter
 *		"count" is set to 0.  (HDFFD-989 and 227: SDsetattr didn't
 *		fail but, eventually, SDend did)
 *
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

/********************************************************************
   Name: test_count() - tests that SDsetattr fails when the parameter
			"count" is passed into SDsetattr as 0 and the
			requested attribute is not created.

   Description:
	In the past, when the parameter "count" was set to 0, SDsetattr
	didn't fail but, eventually, SDend failed when the vdata storing
	the attribute was being created.  With the knowledge that existing
	applications would have failed eventually at SDend if such calls
	existed, and if SDend failure is not detected, the file will be
	corrupted, we decided to change so that SDsetattr now fails when
	"count" is 0.

        The main contents of the test are listed below.




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

#define FILE_SATTR	"tattributes.hdf"
#define VAR1_NAME	"Variable 1"
#define DIM1_NAME	"Dimension 1"
#define ATTR1_NAME	"Attribute Dimension 1"
#define ATTR2_NAME	"Attribute SDS 1"
#define ATTR3_NAME	"Attribute Zero"
#define ATTR1_VAL       "This is a good attr"
#define ATTR2_VAL       "This is a another attr"
#define ATTR1_LEN	20
#define ATTR2_LEN	23
#define ATTR3_LEN	5
#define ATTR_ZERO	"ZERO"
#define ATTR_LEN_ZERO	0

static intn test_count(void)
{
    char  sds_name[20], dim_name[20];
    float32 sds1_data[] = {0.1, 2.3, 4.5, 6.7, 8.9};
    float32 out_data[5];
    int32 dimsize[1], size;
    int32 sds_id, file_id, dim_id, index;
    int32 start=0, stride=1;
    int32 attr_data [5] = {101,102,103,104,105}, scale1_out[5];
    int32 ntype, rank, count;
    int32 n_datasets=0, n_file_attrs=0, nattrs=0;
    intn  status =0;
    hdf_varlist_t* var_list;
    intn  is_coord = FALSE;
    char  attr_name[H4_MAX_NC_NAME], attr_values[80];
    intn  num_errs = 0;         /* number of errors so far */

    file_id = SDstart(FILE_SATTR, DFACC_CREATE);
    CHECK(file_id, FAIL, "SDstart");

    /* Create a one-dim dataset named VAR1_NAME, of type DFNT_FLOAT32. */
    dimsize[0] = 5;
    sds_id = SDcreate(file_id, VAR1_NAME, DFNT_FLOAT32, 1, dimsize);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the dimension name. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");
    status = SDsetdimname(dim_id, DIM1_NAME);
    CHECK(status, FAIL, "SDsetdimname");

    /* Attempt to set a string attr to dim passing in count=0, should fail. */
    status = SDsetattr(dim_id, ATTR_ZERO, DFNT_CHAR8, ATTR_LEN_ZERO, ATTR1_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* To verify that this attribute is not created, SDend must be called and
       the file is opened again. */

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension information to verify there are no attrs set */
    status = SDdiminfo (dim_id, dim_name, &size, &ntype, &nattrs );
    VERIFY(nattrs, 0, "SDdiminfo");

    /* Set an attribute to dimension DIM1_NAME. */
    status = SDsetattr(dim_id, ATTR1_NAME, DFNT_CHAR8, ATTR1_LEN, ATTR1_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Attempt to set a string attr to SDS passing in count=0, should fail. */
    status = SDsetattr(sds_id, ATTR_ZERO, DFNT_CHAR8, ATTR_LEN_ZERO, ATTR2_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* Attempt to set a float attr to SDS passing in count=0, should fail. */
    status = SDsetattr(sds_id, ATTR_ZERO, DFNT_FLOAT32, ATTR_LEN_ZERO, ATTR2_VAL);
    VERIFY(status, FAIL, "SDsetattr");

    /* Close the file and reopen it to verify the number of attributes for SDS
       and for the dimension. */

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get access to the SDS' first dimension. */
    dim_id = SDgetdimid(sds_id, 0);
    CHECK(dim_id, FAIL, "SDgetdimid");

    /* Get dimension information to verify there is one attribute set */
    status = SDdiminfo (dim_id, dim_name, &size, &ntype, &nattrs );
    VERIFY(nattrs, 1, "SDdiminfo");

    /* Get SDS info to verify there is no SDS attribute yet. */
    status = SDgetinfo(sds_id, sds_name, &rank, dimsize, &ntype, &nattrs);
    VERIFY(nattrs, 0, "SDsetattr");

    /* Set an attribute to dataset VAR1_NAME. */
    status = SDsetattr(sds_id, ATTR2_NAME, DFNT_CHAR8, ATTR2_LEN, ATTR2_VAL);
    CHECK(status, FAIL, "SDsetattr");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, &start, &stride, dimsize, (VOIDP)sds1_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Open the file again to check attributes */
    file_id = SDstart(FILE_SATTR, DFACC_RDWR);
    CHECK(file_id, FAIL, "SDstart");

    /* Get index of dataset VAR1_NAME */
    index = SDnametoindex(file_id, VAR1_NAME);
    CHECK(index, FAIL, "SDnametoindex");

    sds_id = SDselect(file_id, index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get dataset information to verify that there is one attribute set. */
    status = SDgetinfo(sds_id, sds_name, &rank, dimsize, &ntype, &nattrs);
    VERIFY(nattrs, 1, "SDsetattr");

    /* Read and verify the information of the SDS' first attribute. */
    status = SDattrinfo(sds_id, 0, attr_name, &ntype, &count);
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

    /* Get dimension information to verify number of attrs */
    status = SDdiminfo (dim_id, dim_name, &size, &ntype, &nattrs );
    VERIFY(nattrs, 1, "SDattrinfo");

    /* Read and verify the information of the dimension's first attribute. */
    status = SDattrinfo(dim_id, 0, attr_name, &ntype, &count);
    CHECK(status, FAIL, "SDattrinfo");
    VERIFY(count, ATTR1_LEN, "SDattrinfo");
    VERIFY(HDstrncmp(attr_name, ATTR1_NAME, ATTR1_LEN), 0, "SDattrinfo");

    /* Read and verify the values of the dimension's first attribute. */
    status = SDreadattr(dim_id, 0, attr_values);
    CHECK(status, FAIL, "SDreadattr");

    if (HDstrncmp(attr_values, ATTR1_VAL, ATTR1_LEN) != 0)
    {
	fprintf(stderr, "Unmatched attribute values for dimension %s: is <%s>, should be <%s>\n", VAR1_NAME, attr_values, ATTR1_VAL);
	num_errs++;
    }

    /* Close dataset and file. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(file_id); 
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_count */


/* Test driver for testing SD attributes. */
extern int
test_attributes()
{
    intn num_errs = 0;         /* number of errors */

    /* Output message about test being performed */
    TESTING("various setting attribute features (tattributes.c)");

    /* test when count is passed into SDsetattr as 0 */
    num_errs = num_errs + test_count();

    if (num_errs == 0)
        PASSED();

    return num_errs;
}

#endif /* HDF */
