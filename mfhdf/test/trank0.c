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
 * trank0.c - tests that several APIs behave "correctly" when an SDS has
 * 		rank = 0.
 * Structure of the file:
 *    test_rank0 - test routine, called in hdftest.c
 * Description:
 *    When rank=0, use rank=1 with size=1 as for scalar data.  See issue
 *    HDFFR-1291 for decision on behavior of API functions.
 ****************************************************************************/

#include <string.h>

#include "mfhdf.h"

#include "hdftest.h"

#define FILE_NAME "trank0.hdf" /* data file to test for rank=0 */
#define X_LENGTH  0
#define RANK      0

/* Test routine for testing the several APIs with SDS having rank = 0. */
extern int
test_rank0()
{
    int32         fid, sds1_id, sds2_id, sds_id;
    int32         dim_sizes[1], array_rank, num_type, attributes;
    int32         start[1], edges[1];
    comp_coder_t  comp_type; /* to retrieve compression type into */
    comp_info     c_info;    /* compression information structure */
    int32         comp_flag; /* compression flag */
    HDF_CHUNK_DEF c_def;     /* Chunking definitions */
    int32         inbuf[1];
    int32         outbuf[1];
    intn          status;
    int32         status_32;
    intn          num_errs = 0; /* number of errors so far */

    /* Output message about test being performed */
    TESTING("several APIs with SDS having rank = 0 (trank0.c)");

    /* Create a file */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "test_rank0: SDstart");

    /**** Verify that SDsetcompress fails when dataset has rank 0 ****/

    /* Create an X_LENGTH dataset, called DataSet_1 */
    dim_sizes[0] = 1;
    sds1_id      = SDcreate(fid, "DataSet_1", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds1_id, FAIL, "test_rank0: SDcreate");

    /* Attempt to set compress but should fail */
    comp_type            = COMP_CODE_DEFLATE;
    c_info.deflate.level = 6;
    status               = SDsetcompress(sds1_id, comp_type, &c_info);
    VERIFY(status, FAIL, "test_rank0: SDsetcompress");

    /**** Verify that SDsetchunk fails when dataset has rank 0 ****/

    /* Create an X_LENGTH dataset, called DataSet_2 */
    sds2_id = SDcreate(fid, "DataSet_2", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds2_id, FAIL, "test_rank0: SDcreate");

    /* Attempt to set chunk but should fail */
    memset(&c_def, 0, sizeof(HDF_CHUNK_DEF));
    comp_flag = HDF_CHUNK;
    status    = SDsetchunk(sds2_id, c_def, comp_flag);
    VERIFY(status, FAIL, "test_rank0: SDsetchunk");

    /* Close the SDSs */
    status = SDendaccess(sds1_id);
    CHECK(status, FAIL, "test_rank0: SDendaccess");
    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "test_rank0: SDendaccess");

    /**** Verify that SDwritedata succeeds when dataset has rank 0 ****/

    /* Initialize input buffer to write, and output buffer to verify the reading */
    inbuf[0]  = 99;
    outbuf[0] = 0;

    /* Select the first dataset */
    sds_id = SDselect(fid, 0);
    CHECK(sds_id, FAIL, "test_rank0: SDselect");

    /* Define the location and size of the dataset to be written to the file */
    start[0] = 0;
    edges[0] = dim_sizes[0];

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (void *)inbuf);
    CHECK(status, FAIL, "test_rank0:SDwritedata");

    /* Close the SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_rank0: SDendaccess");

    /**** Verify that SDreaddata doesn't corrupt user's buffer, when dataset
          has rank 0; at this time, it does (buf[0][0] changed), but EP said
          to just document it for now 2/8/05 - BMR ****/

    /* Select the first dataset */
    sds_id = SDselect(fid, 0);
    CHECK(sds_id, FAIL, "test_rank0: SDselect");

    /* Read the data set */
    start[0] = 0;
    edges[0] = X_LENGTH;
    status   = SDreaddata(sds_id, start, NULL, edges, (void *)outbuf);
    CHECK(status, FAIL, "test_rank0: SDreaddata");
    VERIFY(inbuf[0], outbuf[0], "test_rank0: SDreaddata");

    /* Close the SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_rank0: SDendaccess");

    /**** Verify various functions ****/

    /* Select the second dataset */
    sds_id = SDselect(fid, 1);
    CHECK(sds_id, FAIL, "test_rank0: SDselect");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, NULL, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "test_rank0: SDgetinfo");
    VERIFY(array_rank, 0, "test_rank0: SDgetinfo");

    /* Verify that SDisrecord doesn't cause seg. fault */
    status_32 = SDisrecord(sds_id);
    VERIFY(status_32, TRUE, "test_rank0: SDisrecord");

    /* Close the SDSs */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_rank0: SDendaccess");

    /* Close the SD interface */
    status = SDend(fid);
    CHECK(status, FAIL, "test_rank0: SDend");

    if (num_errs == 0)
        PASSED();
    return num_errs;
}
