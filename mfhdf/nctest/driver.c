/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Id$
 *********************************************************************/

#include "h4config.h"
#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#include "tests.h"

/* #define MDEBUG 1 */

  /*
   * Test driver for netCDF implementation.  This program performs
   * tests against the netCDF specification for all user-level
   * functions in an implementation of the netCDF library.  Must be
   * invoked from a directory in which the invoker has write
   * permission.
   */

#include <stdio.h>
#if defined TEST_PC || defined TEST_WIN
FILE *dbg_file;
#endif

#include <stdlib.h>
#include <string.h> /* to remove warnings, HDFFR-1434 */

#ifdef PROTOTYPE
int main(int argc, char *argv[])
#else
int main(argc, argv)
int argc;
char *argv[];
#endif
{
    static char testfile[] = "test.nc";
    static char unlim_testfile_name[] = "test_unlim.nc";
    int status = 0;

#if defined TEST_PC || defined TEST_WIN
    dbg_file=fopen("test.dbg","w+");
#endif

#ifdef MDEBUG
    malloc_debug(2);
#endif /* MDEBUG */

    ncopts &= ~NC_FATAL;	/* make errors nonfatal */
    ncopts &= ~NC_VERBOSE;	/* turn off error messages */
    ncopts |= NC_VERBOSE;	/* turn  error messages on--AKC */
    ncopts &= ~NC_VERBOSE;	/* turn off error messages */

    test_nccreate(testfile);

    test_ncopen(testfile);

    test_ncredef(testfile);

    test_ncendef(testfile);

    test_ncclose(testfile);

    test_ncinquire(testfile);
#if (!defined _MSC_VER) || (_MSC_VER < 1400)
    test_ncsync(testfile);
#endif
    test_ncabort(testfile);

    test_ncdimdef(testfile);

    test_ncdimid(testfile);

    test_ncdiminq(testfile);

    test_ncdimrename(testfile);

    test_ncvardef(testfile);

    test_ncvarid(testfile);

    test_ncvarinq(testfile);

    test_ncvarput1(testfile);

    test_ncvarget1(testfile);

    test_ncvarput(testfile);

    test_ncvarget(testfile);

    test_ncvarget_unlim(unlim_testfile_name);

    test_ncvarputg(testfile);

    test_ncvargetg(testfile);

    test_ncrecinq(testfile);

    test_ncrecput(testfile);

    test_ncrecget(testfile);

    test_ncvarrename(testfile);

    test_ncattput(testfile);

    test_ncattinq(testfile);

    test_ncattget(testfile);

    test_ncattcopy(testfile, "test2.nc");

    test_ncattname(testfile);

    test_ncattrename(testfile);

    test_ncattdel(testfile);

    test_nctypelen();

#if defined TEST_PC || defined TEST_WIN
    fclose(dbg_file);
#endif
#define EXIT_SUCCESS 0
    return EXIT_SUCCESS;
}

