/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <string.h>

#include "hdf.h"

#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#include "testcdf.h" /* defines in-memory test cdf structure */
#include "add.h"     /* functions to update in-memory netcdf */
#include "error.h"
#include "tests.h"

/*
 * Test nctypelen
 *    try with bad datatype, check error
 *    check returned values for each proper datatype
 */
void
test_nctypelen()
{
    int         nerrs   = 0;
    static char pname[] = "test_nctypelen";

    fprintf(stderr, "*** Testing %s ...\t", &pname[5]);

    if (nctypelen(NC_BYTE) != sizeof(char)) {
        error("%s: nctypelen failed for NC_BYTE", pname);
        nerrs++;
    }
    if (nctypelen(NC_CHAR) != sizeof(char)) {
        error("%s: nctypelen failed for NC_CHAR", pname);
        nerrs++;
    }
    if (nctypelen(NC_SHORT) != sizeof(short)) {
        error("%s: nctypelen failed for NC_SHORT", pname);
        nerrs++;
    }
    if (nctypelen(NC_LONG) != sizeof(nclong)) {
        error("%s: nctypelen failed for NC_LONG", pname);
        nerrs++;
    }
    if (nctypelen(NC_FLOAT) != sizeof(float)) {
        error("%s: nctypelen failed for NC_FLOAT", pname);
        nerrs++;
    }
    if (nctypelen(NC_DOUBLE) != sizeof(double)) {
        error("%s: nctypelen failed for NC_DOUBLE", pname);
        nerrs++;
    }
    if (nerrs > 0)
        fprintf(stderr, "FAILED! ***\n");
    else
        fprintf(stderr, "ok ***\n");
}
