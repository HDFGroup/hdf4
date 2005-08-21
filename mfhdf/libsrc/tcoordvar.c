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
 *	  test_same_name - tests ability to handle the situation, where a 
 *		dimension being named the same as an SDS.
 *		* if SDS is multi-dim, the library works correctly
 *		* if SDS is one-dim, the library cannot distinguish
 *		  between an SDS var and a coordinate var, which results in
 *		  unpredictable behaviors in subsequent accesses to the dim.
 *		  (bugzilla #328)
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

static intn
test_same_name()
{
    int32     fid, dset1, dset2, dim_id1, dim_id2;
    int32     dimsize1[RANK1], dimsizes[RANK2], size, dim_type, dim_attrs;
    int32 start[RANK2], edges[RANK2];
    int32 array_data[X_LENGTH];
    int32 scale0 [] = {0,1,2,3,4,5,6,7,8,9};
    char  dim_name[64];
    intn      i, j, status;
    intn      num_errs = 0;         /* number of errors so far */

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

    status = SDsetdimname (dim_id1,  DIM_NAME1);
    CHECK(status, FAIL, "SDsetdimname");

    dim_id2 = SDgetdimid (dset2, 0);
    CHECK(dim_id2, FAIL, "SDgetdimid");

    status = SDsetdimname (dim_id2,  DIM_NAME2);
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
    VERIFY(HDstrcmp(dim_name, SDS1_NAME), 0, "SDdiminfo");
    VERIFY(dim_attrs, 1, "SDdiminfo");

    /* This dimension doesn't have an attribute, but dim_attrs > 0
	because the SDS variable was mistaken as the coordinate variable
	for dimension named "One dim" as the name of the SDS.  This is
	to show that giving a dimension the same name as a one-dim SDS
	will yield unpredictable results - bugzilla #328.  No further
	accesses should be done on this dimension. */
    if (dim_attrs > 0)
    {
	char attr_buf[16];
	status = SDreadattr(dim_id1, 0, &attr_buf);
	CHECK(dim_id1, FAIL, "SDreadattr");
	/* this is the attribute of the SDS dset1 above */
	VERIFY(HDstrncmp(attr_buf, "not a coord var", 15), 0, "SDreadattr");
    }

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
}   /* test_SDAPI_ids */

/* Test driver for testing various coordinate variable features. */
extern int
test_coordvar()
{
    intn num_errs = 0;         /* number of errors */

    /* test when a dimension being named the same as an SDS */
    num_errs = num_errs + test_same_name();

    return num_errs;
}

#endif /* HDF */
