/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_TESTCDF_H
#define NCTEST_TESTCDF_H

/*
 * In-memory structure holding netcdf structure for comparing with
 * on-disk netcdf.  Whenever a disk netcdf is updated in the test
 * package, this structure should be updated at the same time, to keep
 * them in sync.
 */

#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "mfhdf.h"

#define ___      0              /* marker for structure place-holder */
#define BAD_TYPE NC_UNSPECIFIED /* must be distinct from valid types */

struct cdfdim { /* dimension */
    char *name;
    long  size;
};

struct cdfvar { /* variable */
    char   *name;
    nc_type type;
    int     ndims;
    int    *dims;
    int     natts;
};

struct cdfatt { /* attribute */
    int     var;
    char   *name;
    nc_type type;
    int     len;
    void   *val;
};

#define MAX_TEST_DIMS 32
#define MAX_TEST_VARS 32
#define MAX_TEST_ATTS 32

struct netcdf {
    int           ndims;               /* number of dimensions declared for netcdf */
    int           nvars;               /* number of variables declared for netcdf */
    int           natts;               /* number of attributes */
    int           ngatts;              /* number of global attributes */
    int           xdimid;              /* number of the unlimited dimension, if any */
    struct cdfdim dims[MAX_TEST_DIMS]; /* dimensions */
    struct cdfvar vars[MAX_TEST_VARS]; /* variables */
    struct cdfatt atts[MAX_TEST_ATTS]; /* attributes */
};

/* In-memory netcdf structure, kept in sync with disk netcdf */
extern struct netcdf *test_g;

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

#endif /* NCTEST_TESTCDF_H */
