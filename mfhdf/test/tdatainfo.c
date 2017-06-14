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
 * tdatainfo.c - tests the function SDgetdatainfo.
 * Structure of the file:
 *    test_datainfo - test driver
 *  test_nonspecial_SDSs - tests nonspecial SDSs
 *  test_compressed_SDSs - tests compressed SDSs without closing file
 *  test_empty_SDSs      - tests on empty chunked and chunked/comp SDSs
 *  test_chunked_partial - tests on chunked and partially written SDS
 *  test_chkcmp_SDSs     - tests chunked/compressed SDSs
 *  test_extend_SDSs     - tests SDSs with unlimited dimensions
 * -BMR, Jul 2010
 ****************************************************************************/

#include "mfhdf.h"

#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef H4_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include <math.h>
#ifdef H4_HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#define snprintf sprintf_s
#define ssize_t int32
#endif 

#ifndef DATAINFO_TESTER
#define DATAINFO_TESTER /* to include mfdatainfo.h */
#endif

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

#include "hdftest.h"

static intn test_nonspecial_SDSs();
static intn test_compressed_SDSs();
static intn test_empty_SDSs();
static intn test_chunked_partial();
static intn test_chkcmp_SDSs();
static intn test_extend_SDSs();

#define SIMPLE_FILE     "datainfo_simple.hdf"  /* data file */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

typedef struct {
    int32 numtype; /* number type of the SDS' data */
    int32 n_values; /* number of values the SDS can hold */
    int32* offsets; /* offset(s) of data block(s) */
    int32* lengths; /* length(s) of data block(s) */
    int32* dimsizes;/* sizes of dimensions */
} t_hdf_datainfo_t;

/* alloc_info is a utility function that allocates hdf_datainfo_t's members*/
intn alloc_info(t_hdf_datainfo_t *info, uintn info_count, int32 n_dims) {
    HDmemset(info, 0, sizeof(info));
    info->offsets = (int32 *) HDmalloc(info_count * sizeof(int32));
    if (info->offsets == NULL)
        return -1;
    info->lengths = (int32 *) HDmalloc(info_count * sizeof(int32));
    if (info->lengths == NULL)
        return -1;
    info->dimsizes = (int32 *) HDmalloc(n_dims * sizeof(int32));
    if (info->dimsizes == NULL)
        return -1;
    return 0;
}

void free_info(t_hdf_datainfo_t *info) {
    if (info != NULL) {
        if (info->offsets != NULL)
            HDfree(info->offsets);
        if (info->lengths != NULL)
            HDfree(info->lengths);
        if (info->dimsizes != NULL)
            HDfree(info->dimsizes);
    }
}

/* This is used to temporarily verify results.  Will remove when finallized. */
static void print_info(char* name, uintn info_count, t_hdf_datainfo_t data_info) {
    int ii;

    fprintf(stderr, "offset/length of '%s'\n", name);
    for (ii = 0; ii < info_count; ii++)
        fprintf(stderr, "%d: %d   %d\n", ii, data_info.offsets[ii],
                data_info.lengths[ii]);
}

/* Calculates the number of values in an SDS using the dimensions and rank */
static int32 comp_n_values(int32 rank, int32 *dimsizes) {
    int ii;
    int32 n_values = 1;
    for (ii = 0; ii < rank; ii++)
        n_values = n_values * dimsizes[ii];
    return (n_values);
}

/****************************************************************************
 Name: test_nonspecial_SDSs() - tests non-special SDSs

 Description:
 This routine creates and writes data to non-special SDSs and verifies
 data and data information with SDgetdatainfo.  The tests include
 the following SDSs:
 - a 2-dim 5x8 element SDS, float32, with no data
 - a 1-dim 10-element SDS, int32, with 10 values
 - a 2-dim 5x8 element SDS, float32, with 5x8 values
 - a 1-dim 20-element SDS, char, with 20 values
 SDgetdatainfo will retrieve the number of blocks in the datasets
 and the offsets and lengths of the blocks.  Then the data will be read
 back from the file at the previously retrieved offsets/lengths, without
 the use of the HDF4 library, and will be verified against the original
 data buffers.

 BMR - Jul 2010
 ****************************************************************************/
#define SDS1_NAME  "Simple_data_1dim_int32"
#define SDS2_NAME  "Simple_data_2dims_float32"
#define SDS3_NAME  "Simple_data_1dim_char"
#define RANK1    1
#define RANK2    2
#define LENGTH1_X  10
#define LENGTH2_X  5
#define LENGTH2_Y  8
#define LENGTH3_X  21

static intn test_nonspecial_SDSs() {
    int32 sd_id, sds_id;
    int32 dimsizes[2], starts[2], edges[2], rank = 0;
    int32 data1[LENGTH1_X];
    float data2[LENGTH2_X][LENGTH2_Y];
    char data3[LENGTH3_X], outdata3[LENGTH3_X];
    int32 offset1, length1, offset2, length2, offset3, length3;
    t_hdf_datainfo_t sds1_info, sds2_info, sds3_info;
    uintn info_count = 0;
    intn status;
    int ii, jj;
    intn num_errs = 0; /* number of errors so far */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(SIMPLE_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_nonspecial_SDSs: SDstart");

    /***************************************************************
     Create and write non-special SDSs
     ***************************************************************/

    /* Create a 2x2 dataset called "EmptyDataset" */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate(sd_id, "EmptyDataset", DFNT_FLOAT32, 2, dimsizes);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Open that first dataset and verify that number of data block is 0 */
    sds_id = SDselect(sd_id, 0);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDselect");
    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");
    VERIFY(info_count, 0, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /*
     * Create a 1-dim 10-element SDS, type int32, then write 10 values
     * to it
     */
    dimsizes[0] = LENGTH1_X;
    sds_id = SDcreate(sd_id, SDS1_NAME, DFNT_INT32, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate");

    for (ii = 0; ii < LENGTH1_X; ii++)
        data1[ii] = 1000 * ii;

    starts[0] = 0;
    edges[0] = LENGTH1_X;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data1);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDwritedata");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* 
     * Create a 2-dim 5x8 element SDS, type float32, then write 5x8 values
     * to it
     */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate(sd_id, SDS2_NAME, DFNT_FLOAT32, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate");

    for (ii = 0; ii < LENGTH2_X; ii++)
        for (jj = 0; jj < LENGTH2_Y; jj++)
            data2[ii][jj] = 500.50 * (ii + jj);

    starts[0] = 0;
    starts[1] = 0;
    edges[0] = LENGTH2_X;
    edges[1] = LENGTH2_Y;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data2);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDwritedata");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* 
     * Create a 1-dim 20-element SDS, type char, then write 20 values
     * to it
     */
    dimsizes[0] = LENGTH3_X;
    sds_id = SDcreate(sd_id, SDS3_NAME, DFNT_CHAR, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate");

    strcpy(data3, "The data of 3rd SDS.");

    starts[0] = 0;
    edges[0] = LENGTH3_X;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data3);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDwritedata");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /***********************************************************************
     Read data info for later accessing data without the use of HDF4 library
     ***********************************************************************/

    /* Open the second dataset, verify that number of data block is 1, then
     retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 1);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDselect SDS index 1");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 1");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds1_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, &rank, sds1_info.dimsizes, &(sds1_info.numtype), NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 1");

    /* Record number of values the SDS can have */
    sds1_info.n_values = comp_n_values(rank, sds1_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds1_info.offsets, sds1_info.lengths);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Open the third dataset, verify that number of data block is 1, then
     retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 2);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDselect SDS index 2");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 1");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds2_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds2_info.dimsizes, &(sds2_info.numtype), NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 2");

    /* Record number of values the SDS can have */
    sds2_info.n_values = comp_n_values(rank, sds2_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds2_info.offsets, sds2_info.lengths);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Close SDS index 2 */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Open the last dataset, verify that number of data block is 1, then
     retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 3);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDselect SDS index 3");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 1");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds3_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds3_info.dimsizes, &(sds3_info.numtype), NULL);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetinfo SDS index 3");

    /* Record number of values the SDS can have */
    sds3_info.n_values = comp_n_values(rank, sds3_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds3_info.offsets, sds3_info.lengths);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");

    /* Close SDS index 3 */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    status = SDend(sd_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDend");

    /******************************************************************
     Read data using previously obtained data info without HDF4 library
     ******************************************************************/

    /* Open file and read in data without using SD API */
    {
        int fd; /* for open */
        int32 ret32; /* for DFKconvert */
        int ret; /* for fabs */
        ssize_t readlen = 0; /* for read */
        int32 *readibuf, *readibuf_swapped;
        float *readfbuf, *readfbuf_swapped;
        char *readcbuf, *readcbuf_swapped;
        char readfbuf_str[12], data2_str[12]; /* for comparing readfbuf values */
        uint32 n_values;
        int ii, jj, kk;

        /* Open the file for reading without SD API */
        fd = open(SIMPLE_FILE, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "test_nonspecial_SDSs: unable to open file %s", SIMPLE_FILE);
            num_errs++;
            return num_errs;
        }

        /* Forward to the position of the data of SDS at index 1 */
        if (lseek(fd, (off_t) sds1_info.offsets[0], SEEK_SET) == -1) {
            fprintf(stderr, "test_nonspecial_SDSs: unable to seek offset %d\n",
                    (int) sds1_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP) readibuf, (size_t) sds1_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds1_info.numtype,
                           (uint32) sds1_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        if (ret32 > 0) {
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds1_info.n_values; ii++) {
                if (readibuf_swapped[ii] != data1[ii])
                    fprintf(stderr, "At value# %d: written = %d read = %d\n",
                            ii, data1[ii], readibuf_swapped[ii]);
            }
        }
        HDfree (readibuf_swapped);
        HDfree (readibuf);

        /* Forward to the position of the data of SDS at index 2 */
        if (lseek(fd, (off_t) sds2_info.offsets[0], SEEK_SET) == -1) {
            fprintf(stderr, "test_nonspecial_SDSs: unable to seek offset %d\n",
                    (int) sds2_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readfbuf = (float32 *) HDmalloc(sds2_info.n_values * sizeof(float32));
        readfbuf_swapped = (float32 *) HDmalloc(sds2_info.n_values * sizeof(float32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP) readfbuf, (size_t) sds2_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readfbuf, readfbuf_swapped, sds2_info.numtype,
                           (uint32) sds2_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        /* Compare data read without SD API against the original buffer */
        kk = 0;
        for (jj = 0; jj < sds2_info.dimsizes[0]; jj++)
            for (ii = 0; ii < sds2_info.dimsizes[1]; ii++) {
                /* Flag if the two numbers are not close enough */
                if (fabs(readfbuf_swapped[kk] - data2[jj][ii]) > 0.00001)
                    fprintf(stderr, "At value# %d: written = %f read = %f\n",
                            ii, data2[jj][ii], readfbuf_swapped[kk]);
                if (kk < sds2_info.n_values)
                    kk++;
            }
        HDfree (readfbuf_swapped);
        HDfree (readfbuf);

        /* Forward to the position of the data of SDS at index 3 */
        if (lseek(fd, (off_t) sds3_info.offsets[0], SEEK_SET) == -1) {
            fprintf(stderr, "test_nonspecial_SDSs: unable to seek offset %d\n",
                    (int) sds3_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP) readibuf, (size_t) sds3_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds3_info.numtype,
                           (uint32) sds3_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        if (ret32 > 0) {
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds3_info.n_values; ii++) {
                if (readibuf_swapped[ii] != data3[ii])
                    fprintf(stderr, "At value# %d: written = %d read = %d\n",
                            ii, data3[ii], readibuf_swapped[ii]);
            }
        }
        HDfree (readibuf_swapped);
        HDfree (readibuf);

        if (close(fd) == -1) {
            fprintf(stderr, "test_nonspecial_SDSs: unable to close file %s", SIMPLE_FILE);
            num_errs++;
            return num_errs;
        }
    }
    free_info(&sds1_info);
    free_info(&sds2_info);
    free_info(&sds3_info);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_nonspecial_SDSs */

/****************************************************************************
 Name: test_compressed_SDSs() - tests compressed SDSs

 Description:
 This routine creates and writes data to compressed SDSs and verifies
 data and data information with SDgetdatainfo.  The tests include
 the following SDSs:
 - a 2-dim 5x8 element SDS, float32, NBIT compression, no data
 - a 1-dim 10-element SDS, int32, Deflate compression, with 10 values
 - a 2-dim 5x8 element SDS, float32, SZIP compression, with 5x8 values
 - a 1-dim 20-element SDS, char, Skipping Huffman compression,
 with 20 values

 SDgetdatainfo will retrieve the number of blocks in the datasets
 and the offsets and lengths of the blocks.  Then the data will be read
 back from the file at the previously retrieved offsets/lengths, without
 the use of the HDF4 library, and will be verified against the original
 data buffers.

 BMR - Jul 2010
 ****************************************************************************/
#define  COMP_FILE  "datainfo_cmp.hdf"  /* data file */
static intn test_compressed_SDSs()
{
    int32 sd_id, sds_id, esds_id, usds_id;
    int32 starts[2], edges[2], dimsizes[2], rank = 0;
    comp_coder_t comp_type; /* Compression flag */
    comp_info c_info; /* Compression structure */
    int8 data[Y_LENGTH][X_LENGTH];
    int32 data1[LENGTH1_X];
    float data2[LENGTH2_X][LENGTH2_Y];
    char data3[LENGTH3_X];
    t_hdf_datainfo_t sds1_info, sds2_info, sds3_info;
    int32 offset = 0, length = 0;
    int32 pixels_per_scanline;
    uintn info_count = 0;
    intn status;
    int ii, jj;
    int num_errs = 0; /* number of errors so far */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(COMP_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_compressed_SDSs: SDstart");

    /***************************************************************
     Create and write compressed SDSs
     ***************************************************************/

    /* Create data set 'NBit-No-Data' */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate(sd_id, "NBit-No-Data", DFNT_INT32, 2, dimsizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'NBit-No-Data'");

    /* Promote the data set 'NBit-No-Data' to an NBIT data set */
    status = SDsetnbitdataset(sds_id, 6, 7, FALSE, FALSE);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetnbitdataset");

    /* End access to 'NBit-No-Data' */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    /*
     * Create a 1-dim 10-element SDS, type int32, set Deflate compression,
     * then write 10 values to it
     */
    dimsizes[0] = LENGTH1_X;
    sds_id = SDcreate(sd_id, "Deflate-Data", DFNT_INT32, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'Deflate-Data'");

    comp_type = COMP_CODE_DEFLATE;
    HDmemset(&c_info, 0, sizeof(c_info));
    c_info.deflate.level = 6;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress 'Deflate-Data'");

    for (ii = 0; ii < LENGTH1_X; ii++)
        data1[ii] = 1000 * ii;

    starts[0] = 0;
    edges[0] = LENGTH1_X;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data1);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata 'Deflate-Data'");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'Deflate-Data'");

    for (ii = 0; ii < LENGTH2_X; ii++)
        for (jj = 0; jj < LENGTH2_Y; jj++)
            data2[ii][jj] = 500.50 * (ii + jj);

#ifdef H4_HAVE_SZIP_ENCODER
    /* 
     * Create a 2-dim 5x8 element SDS, type float32, set SZIP compression,
     * then write 5x8 values to it
     */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate(sd_id, "SZIP-Data", DFNT_FLOAT32, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'SZIP-Data'");

    comp_type = COMP_CODE_SZIP;
    HDmemset(&c_info, 0, sizeof(c_info));
    pixels_per_scanline = dimsizes[1];
    c_info.szip.pixels = dimsizes[0] * dimsizes[1];
    ;
    c_info.szip.pixels_per_block = 2;
    if (pixels_per_scanline >= 2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dimsizes[1];

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 64;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress 'SZIP-Data'");

    starts[0] = starts[1] = 0;
    edges[0] = LENGTH2_X;
    edges[1] = LENGTH2_Y;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data2);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata 'SZIP-Data'");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'SZIP-Data'");
#else /* SZIP lib not available */
    /* 
     * Create a 2-dim 5x8 element SDS, type float32, set SZIP compression,
     * then write 5x8 values to it
     */
    dimsizes[0] = LENGTH2_X;
    dimsizes[1] = LENGTH2_Y;
    sds_id = SDcreate (sd_id, "NBIT-Data", DFNT_FLOAT32, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'NBIT-Data'");

    /* Promote the data set 'NBit-No-Data' to an NBIT data set */
    status = SDsetnbitdataset(sds_id, 6, 7, FALSE, FALSE);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetnbitdataset");

    /* Initialize data to write out */
    /* for(ii = 0; ii < LENGTH2_X*LENGTH2_Y; ii++)
     idata[ii] = ii*10;
     */

    /* Write data to the NBIT data set 'NBitDataSet' */
    starts[0] = starts[1] = 0;
    edges[0] = LENGTH2_X;
    edges[1] = LENGTH2_Y;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP)data2);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'SZIP-Data'");
#endif /* SZIP lib available */

    /* 
     * Create a 1-dim 20-element SDS, type char, set Skipping Huffman
     * compression, then write 20 values to it
     */
    dimsizes[0] = LENGTH3_X;
    sds_id = SDcreate(sd_id, "SKPHUFF-Data", DFNT_CHAR, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'SKPHUFF-Data'");

    HDmemset(&c_info, 0, sizeof(c_info));
    comp_type = COMP_CODE_SKPHUFF;
    c_info.skphuff.skp_size = 4;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress 'SKPHUFF-Data'");

    strcpy(data3, "The data of 3rd SDS.");

    starts[0] = 0;
    edges[0] = LENGTH3_X;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data3);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata 'SKPHUFF-Data'");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'SKPHUFF-Data'");

    status = SDend(sd_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDend COMP_FILE");

    /***********************************************************************
     Read data info for later accessing data without the use of HDF4 library
     ***********************************************************************/

    /* Open the file to get data info of the SDSs */
    sd_id = SDstart(COMP_FILE, DFACC_RDONLY);
    CHECK(sd_id, FAIL, "test_compressed_SDSs: SDstart COMP_FILE");

    /* Open the first dataset, verify that number of data block is 0, then
     verify its compression type is NBIT */
    sds_id = SDselect(sd_id, 0);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDselect SDS index 0");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_compressed_SDSs: SDgetdatainfo");
    VERIFY(info_count, 0, "test_compressed_SDSs: SDgetdatainfo");

    comp_type = COMP_CODE_INVALID; /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_NBIT, "test_compressed_SDSs: SDgetcomptype");

    /* Open the second dataset, verify that number of data block is 1, verify
     its comp type is Deflate, then retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 1);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDselect SDS index 1");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_compressed_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");

    comp_type = COMP_CODE_INVALID; /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_DEFLATE, "test_compressed_SDSs: SDgetcomptype");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 1");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds1_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds1_info.dimsizes, &(sds1_info.numtype), NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 1");

    /* Record number of values the SDS can have */
    sds1_info.n_values = comp_n_values(rank, sds1_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds1_info.offsets, sds1_info.lengths);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    /* Open the third dataset, verify that number of data block is 1, verify its
     comp type is SZIP or NBIT, then retrieve and record the offset/length */
    sds_id = SDselect(sd_id, 2);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDselect SDS index 2");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_compressed_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");

    comp_type = COMP_CODE_INVALID; /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetcomptype");
#ifdef H4_HAVE_SZIP_ENCODER
    VERIFY(comp_type, COMP_CODE_SZIP, "test_compressed_SDSs: SDgetcomptype");
#else
    VERIFY(comp_type, COMP_CODE_NBIT, "test_compressed_SDSs: SDgetcomptype");
#endif

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 2");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds2_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds2_info.dimsizes, &(sds2_info.numtype), NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 2");

    /* Record number of values the SDS can have */
    sds2_info.n_values = comp_n_values(rank, sds2_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds2_info.offsets, sds2_info.lengths);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");

    /* Close SDS index 2 */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    /* Open the third dataset, verify that number of data block is 1, verify
     its comp type is Skipping Huffman, then retrieve and record the
     offset/length */
    sds_id = SDselect(sd_id, 3);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDselect SDS index 3");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_compressed_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");

    comp_type = COMP_CODE_INVALID; /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "test_compressed_SDSs: SDgetcomptype");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 3");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds3_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds3_info.dimsizes, &(sds3_info.numtype), NULL);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetinfo SDS index 3");

    /* Record number of values the SDS can have */
    sds3_info.n_values = comp_n_values(rank, sds3_info.dimsizes);

    /* Retrieve the offset and length of the data block */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds3_info.offsets, sds3_info.lengths);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");

    /* Close SDS index 3 */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDend");

    /******************************************************************
     Read data using previously obtained data info without HDF4 library
     ******************************************************************/

#if 0
    /* Open file and read in data without using SD API */
    {
        int fd; /* for open */
        off_t ret; /* for lseek */
        int32 ret32; /* for DFKconvert */
        ssize_t readlen=0; /* for read */
        int32 *readibuf, *readibuf_swapped;
        float *readfbuf, *readfbuf_swapped;
        char *readcbuf, *readcbuf_swapped;
        uint32 n_values;
        int ii, jj, kk;

        /* Open the file for reading without SD API */
        fd = open(SIMPLE_FILE, O_RDONLY);
        if (fd == -1)
        {
            fprintf(stderr, "test_compressed_SDSs: unable to open file %s", SIMPLE_FILE);
            num_errs++;
            return num_errs;
        }

        /* Forward to the position of the data of SDS at index 1 */
        if (lseek(fd, (off_t)sds1_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_compressed_SDSs: unable to seek offset %d\n",
                    (int)sds1_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds1_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readibuf, (size_t)sds1_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds1_info.numtype,
                (uint32)sds1_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        if (ret32 > 0)
        {
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds1_info.n_values; ii++)
            {
                if (readibuf_swapped[ii] != data1[ii])
                fprintf(stderr, "At value# %d: written = %d read = %d\n",
                        ii, data1[ii], readibuf_swapped[ii]);
            }
        }

        /* Forward to the position of the data of SDS at index 2 */
        if (lseek(fd, (off_t)sds2_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_compressed_SDSs: unable to seek offset %d\n",
                    (int)sds2_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds2_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds2_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readibuf, (size_t)sds2_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds2_info.numtype,
                (uint32)sds2_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        if (ret32 > 0)
        {
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds2_info.n_values; ii++)
            {
                if (readibuf_swapped[ii] != data2[ii])
                fprintf(stderr, "At value# %d: written = %d read = %d\n",
                        ii, data2[ii], readibuf_swapped[ii]);
            }
        }

        /* Forward to the position of the data of SDS at index 3 */
        if (lseek(fd, (off_t)sds3_info.offsets[0], SEEK_SET) == -1)
        {
            fprintf(stderr, "test_compressed_SDSs: unable to seek offset %d\n",
                    (int)sds3_info.offsets[0]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        readibuf_swapped = (int32 *) HDmalloc(sds3_info.n_values * sizeof(int32));
        /* Read in this block of data */
        readlen = read(fd, (VOIDP)readibuf, (size_t)sds3_info.lengths[0]);
        CHECK(readlen, FAIL, "DFKconvert");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds3_info.numtype,
                (uint32)sds3_info.n_values, DFACC_WRITE, 0, 0);
        CHECK(ret32, FAIL, "DFKconvert");

        if (ret32 > 0)
        {
            /* Compare data read without SD API against the original buffer */
            for (ii = 0; ii < sds3_info.n_values; ii++)
            {
                if (readibuf_swapped[ii] != data3[ii])
                fprintf(stderr, "At value# %d: written = %d read = %d\n",
                        ii, data3[ii], readibuf_swapped[ii]);
            }
        }

        if (close(fd) == -1)
        {
            fprintf(stderr, "test_compressed_SDSs: unable to close file %s", SIMPLE_FILE);
            num_errs++;
            return num_errs;
        }
    }
#endif
    free_info(&sds1_info);
    free_info(&sds2_info);
    free_info(&sds3_info);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_compressed_SDSs */

/*
 * Test with empty SDSs.  This routine creates a "Chunked Empty" SDS and a
 * "Chunked Compressed Empty" SDS then uses SDgetdatainfo to get the
 * number of blocks for data, which should be 0.
 */
/****************************************************************************
 Name: test_empty_SDSs() - tests special but empty SDSs

 Description:
 This routine creates special SDSs but does not write data to any of
 the SDSs, then uses SDgetdatainfo to verify that the number of data
 blocks of each SDS is 0.

 BMR - Jul 2010
 ****************************************************************************/
#define X_LENGTH2  4
#define Y_LENGTH2  9
#define CHK_X      3
#define CHK_Y      2
#define NUM_SDS    3
#define  NODATA_FILE  "datainfo_nodata.hdf"  /* data file */

static intn test_empty_SDSs()
{
    int32 sd_id, sds_id, sds_index;
    int32 dimsizes[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */
    int32 flag;
    uintn info_count = 0;
    comp_coder_t comp_type; /* Compression flag */
    comp_info c_info; /* Compression structure */
    int ii, jj;
    intn status;
    int num_errs = 0; /* number of errors so far */

    /* Use the same file as in test_compressed_SDSs */
    sd_id = SDstart(NODATA_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_empty_SDSs: SDstart");

    /*
     * Create compressed, chunked, chunked/compressed, and expandible SDSs
     * without writing data to any of them
     */
    dimsizes[0] = Y_LENGTH2;
    dimsizes[1] = X_LENGTH2;

    /* Contiguous-No-Data */
    sds_id = SDcreate(sd_id, "Contiguous-No-Data", DFNT_INT16, RANK, dimsizes);
    CHECK(sds_id, FAIL, "test_empty_SDSs: SDcreate 'Contiguous-No-Data'");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess 'Contiguous-No-Data'");

    /* Compressed-No-Data */
    sds_id = SDcreate(sd_id, "Compressed-No-Data", DFNT_INT16, RANK, dimsizes);
    CHECK(sds_id, FAIL, "test_empty_SDSs: SDcreate 'Compressed-No-Data'");

    HDmemset(&c_info, 0, sizeof(c_info));
    comp_type = COMP_CODE_SKPHUFF;
    c_info.skphuff.skp_size = 4;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_empty_SDSs: SDsetcompress 'Compressed-No-Data'");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess 'Compressed-No-Data'");

    /* Extend-No-Data */
    dimsizes[0] = SD_UNLIMITED;
    sds_id = SDcreate(sd_id, "Extend-No-Data", DFNT_INT16, RANK, dimsizes);
    CHECK(sds_id, FAIL, "test_empty_SDSs: SDcreate 'Extend-No-Data'");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess 'Extend-No-Data'");

    /* Verify that the number of data block is 0 for all data sets */
    for (ii = 0; ii < NUM_SDS; ii++)
    {
  sds_id = SDselect(sd_id, ii);
  CHECK_IND(sds_id, FAIL, "test_empty_SDSs: SDselect", ii);

  info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
  CHECK_IND(info_count, FAIL, "test_empty_SDSs: SDgetdatainfo", ii);
  VERIFY(info_count, 0, "test_empty_SDSs: SDgetdatainfo");

  status = SDendaccess(sds_id);
  CHECK_IND(status, FAIL, "test_empty_SDSs: SDendaccess", ii);
    }

    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDend");

    /* Reopen the file and check again to make sure that it is still correct
     after flushing the metadata */
    sd_id = SDstart(NODATA_FILE, DFACC_READ);
    CHECK(sd_id, FAIL, "test_empty_SDSs: SDstart");

    /* Verify that the number of data block is 0 for all data sets */
    for (ii = 0; ii < NUM_SDS; ii++) {
        sds_id = SDselect(sd_id, ii);
        CHECK_IND(sds_id, FAIL, "test_empty_SDSs: SDselect", ii);

        info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
        CHECK_IND(info_count, FAIL, "test_empty_SDSs: SDgetdatainfo", ii);
        VERIFY(info_count, 0, "test_empty_SDSs: SDgetdatainfo");

        status = SDendaccess(sds_id);
        CHECK_IND(status, FAIL, "test_empty_SDSs: SDendaccess", ii);
    }
    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_empty_SDSs */

/****************************************************************************
 Name: test_chunked_partial() - tests writing partially to chunked SDSs

 Description:
 This routine creates and writes data to compressed SDSs and verifies
 data and data information with SDgetdatainfo.  The tests include
 the following SDSs:
 - a 2-dim 5x8 element SDS, float32, NBIT compression, no data
 - a 1-dim 10-element SDS, int32, Deflate compression, with 10 values
 - a 2-dim 5x8 element SDS, float32, SZIP compression, with 5x8 values
 - a 1-dim 20-element SDS, char, Skipping Huffman compression,
 with 20 values

 SDgetdatainfo will retrieve the number of blocks in the datasets
 and the offsets and lengths of the blocks.  Then the data will be read
 back from the file at the previously retrieved offsets/lengths, without
 the use of the HDF4 library, and will be verified against the original
 data buffers.

 BMR - Jul 2010
 ****************************************************************************/
#define  CHK_FILE  "datainfo_chk.hdf"  /* data file */
static intn test_chunked_partial()
{
    int32 sd_id, sds_id, sds_index;
    int32 dimsizes[RANK], origin[RANK], starts[RANK], rank = 0, edges[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */
    int32 flag; /* Chunking flag */
    int32 fill_value = -2; /* Fill value */
    uintn info_count = 0;
    t_hdf_datainfo_t sds_info;
    int32 *offarray = NULL, *lenarray = NULL;
    int32 data[Y_LENGTH][X_LENGTH];
    int fd; /* for open */
    int ii, jj, chk_num;
    int num_errs = 0; /* number of errors so far */
    intn status;

    /* Declare chunks data type and initialize some of them. */
    int32 chunk1[CHK_X][CHK_Y] = { { 4, 4 }, { 4, 4 }, { 4, 4 } };

    int32 chunk3[CHK_X][CHK_Y] = { { 3, 3 }, { 3, 3 }, { 3, 3 } };

    int32 chunk_1dim[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(CHK_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_chunked_partial: SDstart");

    /* Initialize chunk size */
    HDmemset(&c_def, 0, sizeof(c_def));
    c_def.chunk_lengths[0] = 10;

    /*
     -  Create a 1-dim 100-element SDS with chunk size 10, write 2 chunks, first and last, close it
     -  Create a 2-dim (5x5)-element non-special SDS, write all the data, close it
     -  Call SDgetdatainfo on the first dataset to locate the data written (what about fill values?)
     -  Open first SDS then write 1 more chunk after the first chunk
     -  Call SDgetdatainfo (investigate!)
     */
    /* Create a one-dim chunked SDS to be written partially */
    dimsizes[0] = 100;
    sds_id = SDcreate(sd_id, "Chunked-Partial-Data", DFNT_INT32, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDcreate");

    /* Fill the SDS array with fill value */
    /*  status = SDsetfillvalue(sds_id, (VOIDP)&fill_value);
     CHECK(status, FAIL, "test_chunked_partial: SDsetfillvalue");
     */

    /* Set info for chunking */
    status = SDsetchunk(sds_id, c_def, HDF_CHUNK);
    CHECK(status, FAIL, "test_chunked_partial: SDsetchunk");

    /* Write partially to 'Chunked-Partial-Data' and check the sizes */

    /* Write the chunk with the coordinates (0) */
    origin[0] = 0;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk_1dim);
    CHECK(status, FAIL, "test_chunked_partial: SDwritechunk");

    /* Write the chunk with the coordinates (4) */
    origin[0] = 4;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk_1dim);
    CHECK(status, FAIL, "test_chunked_partial: SDwritechunk");

    /* Terminate access to 'Chunked-Partial-Data' */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess");

    /* Create a 2x2 dataset called 'Non-Special-Data 1' */
    dimsizes[0] = Y_LENGTH;
    dimsizes[1] = X_LENGTH;
    sds_id = SDcreate(sd_id, "Non-Special-Data 1", DFNT_INT32, 2, dimsizes);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDcreate");

    /* Initialize data for the dataset */
    for (jj = 0; jj < Y_LENGTH; jj++) {
        for (ii = 0; ii < X_LENGTH; ii++)
            data[jj][ii] = (ii + jj) + 1;
    }

    /* Write the stored data to the dataset */
    starts[0] = starts[1] = 0;
    edges[0] = dimsizes[0];
    edges[1] = dimsizes[1];
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data);
    CHECK(status, FAIL, "test_chunked_partial: SDwritedata");

    /* Get access to the chunked SDS that was written partially earlier */
    sds_index = SDnametoindex(sd_id, "Chunked-Partial-Data");
    CHECK(sds_index, FAIL, "test_chunked_partial: SDnametoindex");
    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDselect 'Chunked-Partial-Data'");

    /* Verify that only two chunks had been written */
#if 0 /* need to figure out how to test this feature */
    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_chunked_partial: SDgetdatainfo");
    VERIFY(info_count, 2, "test_chunked_partial: SDgetdatainfo");
#endif

    /* Write another chunk at the coordinate (6) */
    origin[0] = 6;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk_1dim);
    CHECK(status, FAIL, "test_chunked_partial: SDwritechunk");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess");

    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDselect 'Chunked-Partial-Data'");

    /* Verify new number of chunks written */
#if 0 /* need to figure out how to test this feature */
    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_chunked_partial: SDgetdatainfo");
    VERIFY(info_count, 3, "test_chunked_partial: SDgetdatainfo");
#endif

    /* Retrieve the offset and length of the chunks */

    /* Get SDS' rank to know how much to allocate space for sds_info */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_chunked_partial: SDgetinfo");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds_info.dimsizes, &(sds_info.numtype), NULL);
    CHECK(status, FAIL, "test_chunked_partial: SDgetinfo");

    /* Record number of values the SDS can have */
    sds_info.n_values = 1 * 10; /* chunk has 1 dim of size 10 */

#if 0 /* need to figure out how to test this feature */
    status = SDgetdatainfo(sds_id, NULL, 0, info_count, sds_info.offsets, sds_info.lengths);
    CHECK(status, FAIL, "test_chunked_partial: SDgetdatainfo");
#endif

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess");
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_chunked_partial: SDend");

    /* Open file and read in data without using SD API */
    fd = open(CHK_FILE, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "test_chunked_partial: unable to open file %s", CHK_FILE);
        num_errs++;
        return num_errs;
    }

    /* Read each chunk and compare values */
    for (chk_num = 0; chk_num < info_count; chk_num++) {
        off_t ret; /* for lseek */
        int32 ret32; /* for DFKconvert */
        ssize_t readlen = 0; /* for read */
        int32 *readibuf, *readibuf_swapped;
        float *readfbuf, *readfbuf_swapped;
        char *readcbuf, *readcbuf_swapped;
        uint32 n_values;
        int ii, jj, kk;

        /* Forward to the position of the data of the SDS */
        if (lseek(fd, (off_t) sds_info.offsets[chk_num], SEEK_SET) == -1) {
            fprintf(stderr, "test_chunked_partial: unable to seek offset %d\n",
                    (int) sds_info.offsets[chk_num]);
            num_errs++;
            return num_errs;
        }

        /* Allocate buffers for SDS' data */
        readibuf = (int32 *) HDmalloc(sds_info.lengths[chk_num]);
        readibuf_swapped = (int32 *) HDmalloc(sds_info.lengths[chk_num]);
        /* readibuf = (int32 *) HDmalloc(sds_info.n_values * sizeof(int32));
         readibuf_swapped = (int32 *) HDmalloc(sds_info.n_values * sizeof(int32));
         */
        /* Read in this block of data */
        readlen = read(fd, (VOIDP) readibuf, (size_t) sds_info.lengths[chk_num]);
        CHECK(readlen, FAIL, "test_chunked_partial: read");

        ret32 = DFKconvert(readibuf, readibuf_swapped, sds_info.numtype, 10, DFACC_WRITE, 0, 0);
        /* (uint32)sds_info.n_values, DFACC_WRITE, 0, 0);
         */
        CHECK(ret32, FAIL, "test_chunked_partial: DFKconvert");

        /* Compare data read without SD API against the original buffer */
        for (ii = 0; ii < sds_info.n_values; ii++) {
            if (readibuf_swapped[ii] != chunk_1dim[ii])
                fprintf(stderr, "At value# %d: written = %d read = %d\n", ii,
                        chunk_1dim[ii], readibuf_swapped[ii]);
        }
        HDfree (readibuf_swapped);
        HDfree (readibuf);
    }
    free_info(&sds_info);

    if (close(fd) == -1) {
        fprintf(stderr, "test_chunked_partial: unable to close file %s", CHK_FILE);
        num_errs++;
        return num_errs;
    }
    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chunked_partial */

/*
 * Test with chunked and compressed SDS.  This routine creates 
 * "Chunked-Deflate-Data" and "Chunked-NoDeflate-Data" SDSs and writes the same
 * data to both.  It will then use SDgetdatainfo to verify the number of
 * data blocks.
 */
#define CHKCMP_FILE     "datainfo_chkcmp.hdf"  /* data file */
static intn test_chkcmp_SDSs()
{
    int32 sd_id, sds_id, sds_index;
    int32 cmpsds_id, cmpsds_index;
    int32 flag, maxcache, new_maxcache;
    int32 dimsizes[RANK], origin[RANK], rank = 0;
    HDF_CHUNK_DEF c_def; /* Chunking definitions */
    t_hdf_datainfo_t sds_info, cmpsds_info;
    int32 fill_value = 0; /* Fill value */
    int32 comp_size1 = 0, uncomp_size1 = 0;
    int32 comp_size2 = 0, uncomp_size2 = 0;
    int32 chk_coord[2];
    uintn info_count = 0;
    intn status;
    int num_errs = 0; /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int16 chunk1[CHK_X][CHK_Y] = { { 1, 1 }, { 1, 1 }, { 1, 1 } };

    int16 chunk3[CHK_X][CHK_Y] = { { 3, 3 }, { 3, 3 }, { 3, 3 } };

    int32 chunk2[CHK_X][CHK_Y] = { { 2, 2 }, { 2, 2 }, { 2, 2 } };

    /* Initialize chunk size */
    HDmemset(&c_def, 0, sizeof(c_def));
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(CHKCMP_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_chkcmp_SDSs: SDstart");

    /* Create Y_LENGTH2 x X_LENGTH2 SDS */
    dimsizes[0] = Y_LENGTH2;
    dimsizes[1] = X_LENGTH2;
    cmpsds_id = SDcreate(sd_id, "Chunked-Deflate-Data", DFNT_INT32, RANK, dimsizes);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDcreate");

    sds_id = SDcreate(sd_id, "Chunked-NoDeflate-Data", DFNT_INT32, RANK, dimsizes);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDcreate");

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue(cmpsds_id, (VOIDP) &fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue");

    status = SDsetfillvalue(sds_id, (VOIDP) &fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue");

    /* Set info for chunking and compression */
    HDmemset(&c_def, 0, sizeof(c_def));
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    flag = HDF_CHUNK | HDF_COMP;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 6;
    status = SDsetchunk(cmpsds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk");

    HDmemset(&c_def, 0, sizeof(c_def));
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    flag = HDF_CHUNK;
    status = SDsetchunk(sds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk");

    /* Set chunk cache to hold maximum of 3 chunks
     maxcache = 1;
     new_maxcache = SDsetchunkcache(cmpsds_id, maxcache, 0);
     CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache");

     new_maxcache = SDsetchunkcache(sds_id, maxcache, 0);
     CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache");
     */
    /* Terminate access to the dataset before writing data to it. */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    /* Re-select the datasets, write chunks using SDwritechunk function, then
     check their data sizes */

    /* Get index of dataset using its name */
    cmpsds_index = SDnametoindex(sd_id, "Chunked-Deflate-Data");
    CHECK(cmpsds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex");
    sds_index = SDnametoindex(sd_id, "Chunked-NoDeflate-Data");
    CHECK(sds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex");

    /* Select the datasets for access */
    cmpsds_id = SDselect(sd_id, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect");
    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    /* Write the chunk with the coordinates (0,0) */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (1,0) */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (0,1) */
    origin[0] = 0;
    origin[1] = 1;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Terminate access to the datasets */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    status = SDend(sd_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDend");

    /* Retrieve the offset and length of the chunks of both SDSs */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(CHKCMP_FILE, DFACC_RDWR);
    CHECK(sd_id, FAIL, "test_chkcmp_SDSs: SDstart");

    /* Open dataset 'Chunked-Deflate-Data' */
    cmpsds_id = SDselect(sd_id, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    /* Open dataset 'Chunked-NoDeflate-Data' */
    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    /*
     * "Chunked-NoDeflate-Data"
     */
    chk_coord[0] = chk_coord[1] = 0;
    info_count = SDgetdatainfo(sds_id, chk_coord, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_chkcmp_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetinfo SDS index 3");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds_info.dimsizes, &(sds_info.numtype), NULL);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetinfo");

    /* Record number of values the SDS can have */
    sds_info.n_values = comp_n_values(rank, sds_info.dimsizes);

    status = SDgetdatainfo(sds_id, chk_coord, 0, info_count, sds_info.offsets, sds_info.lengths);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");

    free_info(&sds_info);

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    /*
     * "Chunked-Deflate-Data"
     */
    chk_coord[0] = 0;
    chk_coord[1] = 1;
    info_count = SDgetdatainfo(cmpsds_id, chk_coord, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
    VERIFY(info_count, 1, "test_chkcmp_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(cmpsds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetinfo");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&cmpsds_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(cmpsds_id, NULL, NULL, cmpsds_info.dimsizes, &(cmpsds_info.numtype), NULL);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetinfo");

    /* Record number of values the SDS can have */
    cmpsds_info.n_values = comp_n_values(rank, cmpsds_info.dimsizes);

    status = SDgetdatainfo(cmpsds_id, chk_coord, 0, info_count, cmpsds_info.offsets, cmpsds_info.lengths);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");

    free_info(&cmpsds_info);

    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    status = SDend(sd_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chkcmp_SDSs */

/*
 * Test SDSs with unlimited dimensions.  This routine creates SDSs with
 * unlimited dimensions, writes data to them, and use SDgetdatainfo to
 * verify the number of data blocks.
 */
#define EXTEND_FILE     "datainfo_extend.hdf"  /* data file */
#define BLOCK_SIZE	400
static intn test_extend_SDSs()
{
    int32 sd_id, sds_id, sds_index;
    int32 dimsizes[2], starts[2], edges[2], rank = 0;
    int32 dimsize1[1], start1[1], edges1[1];
    int32 data1[Y_LENGTH][X_LENGTH];
    int32 data2[Y_LENGTH][X_LENGTH];
    int32 data3[Y_LENGTH][X_LENGTH];
    float fdata[Y_LENGTH];
    int32 output[Y_LENGTH * 3][X_LENGTH];
    uintn info_count = 0;
    t_hdf_datainfo_t sds_info;
    int32 *offarray = NULL, *lenarray = NULL;
    int32  block_size = 0;
    intn status;
    int i, j, kk;
    int num_errs = 0; /* number of errors so far */

    /* Initialize data for the dataset */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
            data1[j][i] = (i + j) + 1;

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(EXTEND_FILE, DFACC_CREATE);
    CHECK(sd_id, FAIL, "test_extend_SDSs: SDstart");

    /* Create a 2x2 dataset called "Extend-Data 1" */
    dimsizes[0] = SD_UNLIMITED;
    dimsizes[1] = X_LENGTH;
    sds_id = SDcreate(sd_id, "Extend-Data 1", DFNT_INT32, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate");

    status = SDsetblocksize(sds_id, BLOCK_SIZE); /* to force linked blocks */
    CHECK(status, FAIL, "test_extend_SDSs: SDsetblocksize");

    /* Write the first batch of data to the dataset */
    starts[0] = starts[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data1);
    CHECK(status, FAIL, "test_extend_SDSs: SDwritedata");

    /* Get the block size of "Extend-Data 1" right after writing data */
    status = SDgetblocksize(sds_id, &block_size);
    CHECK(status, FAIL, "test_extend_SDSs: SDgetblocksize");
    VERIFY(block_size, BLOCK_SIZE, "SDgetblocksize");

    /* Check data. */
    HDmemset(&output, 0, sizeof(output));
    status = SDreaddata(sds_id, starts, NULL, edges, (VOIDP) output);
    CHECK(status, FAIL, "test_extend_SDSs: SDreaddata");

    /* Verify first batch of data in the unlimited dimension SDS */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
            if (output[j][i] != data1[j][i])
                fprintf(stderr, "Read value (%d) differs from written (%d) at [%d,%d]\n",
                        output[j][i], data1[j][i], j, i);

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Create another dataset with 1 unlimited dimension */
    sds_id = SDcreate(sd_id, "Extend-Data 2", DFNT_FLOAT64, RANK1, dimsizes);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate");

    /* Define the location and size of the data to be written to the dataset */
    start1[0] = 0;
    edges1[0] = Y_LENGTH;

    /* Write the stored data to 'Extend-Data 2' */
    status = SDwritedata(sds_id, start1, NULL, edges1, (VOIDP) fdata);
    CHECK(status, FAIL, "test_extend_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Select the dataset "Extend-Data 1", then write more data to it */
    sds_index = SDnametoindex(sd_id, "Extend-Data 1");
    CHECK(sds_index, FAIL, "test_extend_SDSs: SDnametoindex");

    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDselect");

    /* Get the block size of "Extend-Data 1" */
    status = SDgetblocksize(sds_id, &block_size);
    CHECK(status, FAIL, "test_extend_SDSs: SDgetblocksize");
    VERIFY(block_size, BLOCK_SIZE, "SDgetblocksize");

    /* Initialize second batch of data for the extendable dataset */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
            data2[j][i] = (i + j) + 10;

    /* Append the stored data to dataset "Extend-Data 1" */
    starts[0] = Y_LENGTH;
    starts[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data2);
    CHECK(status, FAIL, "test_extend_SDSs: SDwritedata");

    /* Initialize third batch of data for the extendable dataset */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
            data3[j][i] = (i + j) + 100;

    starts[0] = Y_LENGTH + Y_LENGTH;
    starts[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;
    status = SDwritedata(sds_id, starts, NULL, edges, (VOIDP) data3);
    CHECK(status, FAIL, "test_extend_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Open "Extend-Data 1" again and check data. */
    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDselect");

    starts[0] = 0;
    starts[1] = 0;
    edges[0] = Y_LENGTH + Y_LENGTH + Y_LENGTH;
    edges[1] = X_LENGTH;
    HDmemset(&output, 0, sizeof(output));
    status = SDreaddata(sds_id, starts, NULL, edges, (VOIDP) output);
    CHECK(status, FAIL, "test_extend_SDSs: SDreaddata");

    /* Check data against first batch */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
            if (output[j][i] != data1[j][i])
                fprintf(stderr,
       "Read value (%d) differs from written (%d) at [%d,%d]\n",
       output[j][i], data1[j][i], j, i);

    /* Check against second batch */
    kk = Y_LENGTH;
    for (j = 0; j < Y_LENGTH; j++, kk++)
        for (i = 0; i < X_LENGTH; i++)
            if (output[kk][i] != data2[j][i])
                fprintf(stderr, "Read value (%d) differs from written (%d) at [%d,%d]\n",
                        output[kk][i], data2[j][i], kk, i);

    /* Check against third batch */
    kk = Y_LENGTH+Y_LENGTH;
    for (j = 0; j < Y_LENGTH; j++, kk++)
        for (i = 0; i < X_LENGTH; i++)
            if (output[kk][i] != data3[j][i])
                fprintf(stderr, "Read value (%d) differs from written (%d) at [%d,%d]\n",
                        output[kk][i], data3[j][i], kk, i);

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Select the dataset "Extend-Data 1", then get offsets and lengths of
     its data blocks */
    sds_index = SDnametoindex(sd_id, "Extend-Data 1");
    CHECK(sds_index, FAIL, "test_extend_SDSs: SDnametoindex");

    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDselect");

    info_count = SDgetdatainfo(sds_id, NULL, 0, 0, NULL, NULL);
    CHECK(info_count, FAIL, "test_extend_SDSs: SDgetdatainfo");
    VERIFY(info_count, 3, "test_extend_SDSs: SDgetdatainfo");

    /* Get SDS' rank */
    status = SDgetinfo(sds_id, NULL, &rank, NULL, NULL, NULL);
    CHECK(status, FAIL, "test_extend_SDSs: SDgetinfo");

    /* Allocate space to record the SDS' data info for later use */
    alloc_info(&sds_info, info_count, rank);

    /* Get SDS' information */
    status = SDgetinfo(sds_id, NULL, NULL, sds_info.dimsizes, &(sds_info.numtype), NULL);
    CHECK(status, FAIL, "test_extend_SDSs: SDgetinfo");

    /* Record number of values the SDS can have */
    sds_info.n_values = comp_n_values(rank, sds_info.dimsizes);

    /* Get offsets and lengths of the data */
    info_count = SDgetdatainfo(sds_id, NULL, 0, info_count, sds_info.offsets, sds_info.lengths);

    { /* Verify the offsets and lengths returned by SDgetdatainfo */
      /* NOTE: if "datainfo_extend.hdf" is changed, the following
         initialization of the offsets must be updated accordingly. -BMR */
      int32 check_offsets[3] = {2776, 3962, 4362};
      int32 check_lengths[3] = {400, 400, 400};
      VERIFY(info_count, 3, "test_extend_SDSs: SDgetdatainfo");
      for (kk = 0; kk < info_count; kk++)
      {
  if (sds_info.offsets[kk] != check_offsets[kk])
      fprintf(stderr,
    "test_extend_SDSs: incorrect offset %d for block #%d\n",
    sds_info.offsets[kk], kk);
  if (sds_info.lengths[kk] != check_lengths[kk])
      fprintf(stderr,
    "test_extend_SDSs: incorrect length %d for block #%d\n",
    sds_info.lengths[kk], kk);
      } } /* done verifying offsets and lengths */

    /* Release memory */
    free_info(&sds_info);

    /* Close this SDS and the SD interface */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDend");

    /*
     * Reopen the file and verify that "Extend-Data 1" has the correct
     * block size that was set earlier.
     */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart(EXTEND_FILE, DFACC_RDONLY);
    CHECK(sd_id, FAIL, "test_extend_SDSs: SDstart");

    /* Select the dataset "Extend-Data 1" */
    sds_index = SDnametoindex(sd_id, "Extend-Data 1");
    CHECK(sds_index, FAIL, "test_extend_SDSs: SDnametoindex");

    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDselect");

    /* Verify that the block size is still as set earlier */
    status = SDgetblocksize(sds_id, &block_size);
    CHECK(status, FAIL, "test_extend_SDSs: SDgetblocksize");
    VERIFY(block_size, BLOCK_SIZE, "SDgetblocksize");

    /* Close this SDS and the SD interface */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");
    status = SDend(sd_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_extend_SDSs */

/* Test driver for testing the public function SDgetdatainfo. */
extern int test_datainfo() {
    intn status;
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("getting location info of data (tdatainfo.c)");

    /* Test nonspecial SDSs */
    num_errs = num_errs + test_nonspecial_SDSs();

    /* Test compressed SDSs */
    num_errs = num_errs + test_compressed_SDSs();

    /* Test chunked empty SDSs */
    num_errs = num_errs + test_empty_SDSs();

    /* Test chunked_partial SDSs */
    num_errs = num_errs + test_chunked_partial();

    /* Test chunked SDSs */
    num_errs = num_errs + test_chkcmp_SDSs();

    /* Test extendable SDSs */
    num_errs = num_errs + test_extend_SDSs();

    if (num_errs == 0)
        PASSED();
    return num_errs;
}
