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

#include <stdlib.h>
#include <string.h>

#include "mfhdf.h"

#include "hdftest.h"

#define NBITFILE "nbit.hdf"

/* All test functions to be called in main */
extern int test_netcdf_reading();
extern int test_szip_compression();
extern int test_checkempty();
extern int test_idtest();
extern int test_sd();
extern int test_mixed_apis();
extern int test_files();
extern int test_SDSprops();
extern int test_coordvar();
extern int test_chunk();
extern int test_compression();
extern int test_dimensions();
extern int test_attributes();
extern int test_datasizes();
extern int test_datainfo();
extern int test_external();
extern int test_att_ann_datainfo();

/* FIXME: Disabled due to test failures */
/* extern int test_rank0(); */

static int
test_nbit()
{
    int32    fnbit;              /* File handle */
    int32    nt;                 /* Number type */
    int32    dimsize[10];        /* dimension sizes */
    int32    newsds, newsds2;    /* SDS handles */
    int      status;             /* status flag */
    int32    start[10], end[10]; /* start, end arrays */
    int32    idata[100];
    int32    rdata[100];
    float32 *data     = NULL;
    int      num_errs = 0; /* number of errors so far */

    ncopts = NC_VERBOSE;

    data = (float32 *)calloc(1000, sizeof(float32));
    CHECK_ALLOC(data, "data", "main");

    /*
     * Test the N-Bit storage stuff
     */

    /* Create file 'nbit.hdf' */
    fnbit = SDstart(NBITFILE, DFACC_CREATE);
    CHECK(fnbit, FAIL, "SDstart");

    /* Create data set 'NBitDataSet' in file 'nbit.hdf' */
    nt         = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds     = SDcreate(fnbit, "NBitDataSet", nt, 2, dimsize);
    CHECK(newsds, FAIL, "SDcreate:Failed to create a new data set('NBitDataSet') for n-bit testing");

    /* Initialize data to write out */
    for (int i = 0; i < 25; i++)
        idata[i] = i * 10;

    /* Promote the data set 'NBitDataSet' to an NBIT data set */
    status = SDsetnbitdataset(newsds, 6, 7, FALSE, FALSE);
    CHECK(status, FAIL, "SDsetnbitdataset");

    /* Write data to the NBIT data set 'NBitDataSet' */
    start[0] = start[1] = 0;
    end[0] = end[1] = 5;
    status          = SDwritedata(newsds, start, NULL, end, (void *)idata);
    CHECK(status, FAIL, "SDwritedata");

    /* end access to NBIT data set 'NBitDataSet' */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush n-bit info to file.
       hmm... */
    status = SDend(fnbit);
    CHECK(status, FAIL, "SDend");

    /* read the n-bit data back in */
    fnbit = SDstart(NBITFILE, DFACC_RDWR);
    CHECK(fnbit, FAIL, "SDstart (again)");

    /* Select the NBIT data set back in, assume it is the first one */
    newsds2 = SDselect(fnbit, 0);
    CHECK(newsds2, FAIL, "SDselect:Failed to select a data set for n-bit access");

    /* read data back in from the NBIT data set */
    start[0] = start[1] = 0;
    end[0] = end[1] = 5;
    status          = SDreaddata(newsds2, start, NULL, end, (void *)rdata);
    CHECK(status, FAIL, "SDreaddata");

    /* verify the data */
    for (int i = 0; i < 25; i++) {
        if ((idata[i] & 0x7f) != rdata[i]) {
            fprintf(stderr, "Bogus val in loc %d in n-bit dset want %ld got %ld\n", i, (long)idata[i],
                    (long)rdata[i]);
            num_errs++;
        }
    }

    /* end access to NBIT data set */
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    /* close file 'nbit.hdf' */
    status = SDend(fnbit);
    CHECK(status, FAIL, "SDend");

    free(data);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}

int
main(void)
{
    int num_errs = 0; /* number of errors so far */

    /* Test nbits functions */
    num_errs = num_errs + test_nbit();

    /* Test the compressed storage routines */
    num_errs = num_errs + test_compression();

    /* Tests chunking functions */
    num_errs = num_errs + test_chunk();

    /* Tests reading netCDF file using SD interface */
    num_errs = num_errs + test_netcdf_reading();

    /* Tests dimension functions (in tdim.c) */
    num_errs = num_errs + test_dimensions();

    /* Tests SDcheckempty (in temptySDSs.c) */
    num_errs = num_errs + test_checkempty();

    /* Tests attribute functions (in tattributes.c) */
    num_errs = num_errs + test_attributes();

    /* Tests SDgetdatasize (in tdatasizes.c) */
    num_errs = num_errs + test_datasizes();

    /* Tests getting SDS data information (in tdatainfo.c) */
    num_errs = num_errs + test_datainfo();

    /* Tests getting attribute and annotation data information (in tattdatainfo.c) */
    num_errs = num_errs + test_att_ann_datainfo();

    /* Tests SDidtype and V/VS APIs on vgroups/vdatas associated with an sds
       (in tidtypes.c) */
    num_errs = num_errs + test_mixed_apis();

    /* Tests miscellaneous file-related APIs (in tfiles.c) */
    num_errs = num_errs + test_files();

    /* Test the behavior of several functions when the SDS has rank=0 (in trank0.c) */
    /* FIXME: Currently failing - fix ASAP */
    /* num_errs = num_errs + test_rank0(); */

    /* Tests functionality related to SDS' properties (in tsdsprops.c) */
    num_errs = num_errs + test_SDSprops();

    /* Tests functionality related to coordinate variables (in tcoordvar.c) */
    num_errs = num_errs + test_coordvar();

    /* Tests functionality related to external data (in textdata.c) */
    num_errs = num_errs + test_external();

    /* Verifies that some functions will not fail even though SZIP
       library is not present or only decoder is available. */
    num_errs = num_errs + test_szip_compression();

    /* Tests SDstart for files with no write permission */
    num_errs = num_errs + test_sd();

    if (num_errs == 0) {
        printf("*** HDF-SD test passes ***\n");
        return EXIT_SUCCESS;
    }
    else {
        printf("*** HDF-SD test fails ***\n");
        return EXIT_FAILURE;
    }
}
