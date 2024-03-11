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
 * tsd.c    Tests SDstart for files with no write permission
 *
 * NOTE:    This test is very flaky since it depends on the user's
 *          permissions. When run as root, for example, the call to
 *          SDstart() with write permissions will pass.
 *
 ****************************************************************************/

#ifdef H4_HAVE_UNISTD_H
#include <unistd.h.>
#endif

#include "mfhdf.h"

#include "hdftest.h"
#include "hfile_priv.h"

/* Data file to test ID types */
#define FILE_NAME "sdtest.hdf"

int
test_sd()
{
    int32 fid;
    int   status;
#if defined(H4_HAVE_WIN32_API)
    int mode;
#else
    mode_t mode;
#endif

    FILE *ff;
    int   num_errs = 0; /* Number of errors so far */

    /* Output message about test being performed */
    TESTING("SDstart for file with no write permission (tsd.c)");

#ifdef __CYGWIN__
    /* Default Cygwin permissions will allow SDstart() to open read-only files
     * for writing, so we'll skip the test.
     */
    SKIPPED();
    return 0;
#endif

#if defined(H4_HAVE_UNISTD_H) && !defined(__MINGW32__)
    /* Root users on POSIX systems may have privileges that allow SDstart()
     * to open read-only files for writing, so we'll skip the test.
     */
    uid_t uid = getuid();

    if (uid == 0) {
        SKIPPED();
        return 0;
    }
#endif

    /* Delete the file just to be sure */
    remove(FILE_NAME);

    /* Create a file */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "SDstart");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

#if defined(H4_HAVE_WIN32_API)
    mode = _S_IREAD;
#else
    mode = S_IRUSR;
#endif

    status = chmod(FILE_NAME, mode);
    CHECK(status, FAIL, "chmod");

    /* Create a protected file */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    VERIFY(fid, FAIL, "second SDstart");

    ff = HI_OPEN(FILE_NAME, DFACC_READ);
    CHECK(ff, NULL, "fopen");

    if (ff != NULL) {
        HI_CLOSE(ff);
    }

#if defined(H4_HAVE_WIN32_API)
    mode = _S_IWRITE;
#else
    mode = S_IWUSR;
#endif

    status = chmod(FILE_NAME, mode);
    CHECK(status, FAIL, "chmod");

    if (num_errs == 0)
        PASSED();
    return num_errs;
} /* test_SDAPI_ids */
