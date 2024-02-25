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

#ifndef HDFTEST_H
#define HDFTEST_H

#include <float.h>
#include <math.h>
#include <stdio.h>

/* The name of the test is printed by saying TESTING("something") which will
 * result in the string `Testing something' being flushed to standard output.
 * If a test passes, fails, or is skipped then the PASSED(), H4_FAILED(), or
 * SKIPPED() macro should be called.  After H4_FAILED(), the caller
 * should print additional information to stdout indented by at least four
 * spaces.
 */
#define TESTING(WHAT)                                                                                        \
    {                                                                                                        \
        printf("Testing %-62s", WHAT);                                                                       \
        fflush(stdout);                                                                                      \
    }
#define PASSED()                                                                                             \
    do {                                                                                                     \
        puts(" PASSED");                                                                                     \
        fflush(stdout);                                                                                      \
    } while (0)
#define H4_FAILED()                                                                                          \
    do {                                                                                                     \
        puts("*FAILED*");                                                                                    \
        fflush(stdout);                                                                                      \
    } while (0)
#define H4_WARNING()                                                                                         \
    {                                                                                                        \
        puts("*WARNING*");                                                                                   \
        fflush(stdout);                                                                                      \
    }
#define SKIPPED()                                                                                            \
    {                                                                                                        \
        puts(" -SKIP-");                                                                                     \
        fflush(stdout);                                                                                      \
    }

/* Check status value and print error message */
#define CHECK(status, fail_value, name)                                                                      \
    {                                                                                                        \
        if (status == fail_value) {                                                                          \
            fprintf(stderr, "*** Routine %s FAILED at line %d ***\n", name, __LINE__);                       \
            num_errs++;                                                                                      \
        }                                                                                                    \
    }

/* Check status value and print error message, including index */
#define CHECK_IND(status, fail_value, name, index)                                                           \
    {                                                                                                        \
        if (status == fail_value) {                                                                          \
            fprintf(stderr, "*** Routine %s FAILED at line %d for SDS index %d ***\n", name, __LINE__,       \
                    index);                                                                                  \
            num_errs++;                                                                                      \
        }                                                                                                    \
    }

/* Verify that a value is as expected and, if not, print error message */
#define VERIFY(item, value, test_name)                                                                       \
    {                                                                                                        \
        if (item != value) {                                                                                 \
            fprintf(stderr, "*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", test_name,            \
                    (long)item, (int)__LINE__, __FILE__);                                                    \
            num_errs++;                                                                                      \
        }                                                                                                    \
    }

/* Verify that a value of type char* is as expected and, if not, print
   error message */
#define VERIFY_CHAR(item, value, test_name)                                                                  \
    {                                                                                                        \
        if (strcmp(item, value) != 0) {                                                                      \
            fprintf(stderr, "*** UNEXPECTED VALUE from %s is <%s> at line %4d in %s\n", test_name, item,     \
                    (int)__LINE__, __FILE__);                                                                \
            num_errs++;                                                                                      \
        }                                                                                                    \
    }

/* Used to validate that 'buffer' has been successfully allocated */
#define CHECK_ALLOC(buffer, buf_name, func_name)                                                             \
    {                                                                                                        \
        if (buffer == NULL) {                                                                                \
            fprintf(stderr, "in %s: space allocation for %s failed.  Terminated!\n", func_name, buf_name);   \
            exit(1);                                                                                         \
        }                                                                                                    \
    }

/*
 * Methods to compare the equality of floating-point values:
 *
 *    1. H4_XXX_ABS_EQUAL - check if the difference is smaller than the
 *       Epsilon value.  The Epsilon values, FLT_EPSILON, DBL_EPSILON,
 *       and LDBL_EPSILON, are defined by compiler in float.h.
 *
 *  HDF5 (from whence these macros came) also includes macros that
 *  use relative error. Those will be brought over only if needed.
 */
#define H4_FLT_ABS_EQUAL(X, Y)  (fabsf((X) - (Y)) < FLT_EPSILON)
#define H4_DBL_ABS_EQUAL(X, Y)  (fabs((X) - (Y)) < DBL_EPSILON)
#define H4_LDBL_ABS_EQUAL(X, Y) (fabsl((X) - (Y)) < LDBL_EPSILON)

/*************************** Utility Functions ***************************/

/* Just return the srcdir path */
const char *get_srcdir(void);

/* Append the test file name to the srcdir path and return the whole string */
const char *get_srcdir_filename(const char *filename);

/* Generates the correct name of the source path */
intn make_sourcepath(char *src_path, unsigned int size);

/* Calls SDcreate, SDwritedata, and SDendaccess */
int32 make_SDS(int32 sd_id, char *sds_name, int32 type, int32 rank, int32 *dim_sizes, int32 unlim_dim,
               void *written_data);

/* Calls SDcreate, SDsetcompress, SDwritedata, and SDendaccess */
int32 make_CompSDS(int32 sd_id, char *sds_name, int32 type, int32 rank, int32 *dim_sizes, void *written_data);

/* Calls SDcreate, SDsetexternalfile, SDwritedata, and SDendaccess */
int32 make_Ext3D_SDS(int32 sd_id, char *sds_name, int32 type, int32 rank, int32 *dim_sizes,
                     void *written_data, int32 offset, char *ext_file_name);

/* Calls get_SDSbyName, SDwritedata, and SDendaccess */
int32 append_Data2SDS(int32 sd_id, char *sds_name, int32 *start, int32 *edges, void *ap_data);

/* Calls SDgetdatasize then verify the size against data_size */
intn verify_datasize(int32 sds_id, int32 data_size, char *sds_name);

/* Verifies the unlimited dimension's size and the variable's data. */
int verify_info_data(int32 sds_id, int32 expected_dimsize, int16 *result);

/* Find and open an SDS by name */
int32 get_SDSbyName(int32 sd_id, const char *sds_name);

#endif /* HDFTEST_H */
