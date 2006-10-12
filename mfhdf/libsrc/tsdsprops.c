/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group (THG).                                         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file and in the print documentation copyright notice.         *
 * COPYING can be found at the root of the source code distribution tree;    *
 * the copyright notice in printed HDF documentation can be found on the     *
 * back of the title page.  If you do not have access to either version,     *
 * you may request a copy from help@hdfgroup.org.                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tsdsprops.c - tests functionality regarding an SDS' properties, such
 *		 name, type, size,...
 * Structure of the file:
 *    test_SDSprops - test driver
 *	  test_SDSnames    - tests that an SDS name can be more than 
 *		previously hardcoded at 64 characters; now length is variable.
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

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
    int32	 fid, dset1, dset2, dim_id;
    int32	 rank, dtype, nattrs, dimsizes[RANK];
    char*	 ds_name;
    uint16	 name_len;
    intn	 status;
    intn	 num_errs = 0;         /* number of errors so far */

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

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of, so far */
    return num_errs;
}   /* test_SDSnames */

/* Test driver for testing the API SDidtype. */
extern int
test_SDSprops()
{
    intn num_errs = 0;         /* number of errors */

    num_errs = num_errs + test_SDSnames();

    return num_errs;
}

#endif /* HDF */
