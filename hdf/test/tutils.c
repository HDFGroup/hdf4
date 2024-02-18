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

#include "hdf.h"
#include "tutils.h"
#include "srcdir_str.h"

/* Buffer to construct path in and return pointer to */
static char srcdir_path[1024];

/* Buffer to construct file in and return pointer to */
static char srcdir_testpath[1024];

/*-------------------------------------------------------------------------
 * Function:    get_srcdir_filename
 *
 * Purpose:     Append the test file name to the srcdir path and return the whole string
 *
 * Return:      The string or NULL (errors or not enough space)
 *
 *-------------------------------------------------------------------------
 */
const char *
get_srcdir_filename(const char *filename)
{
    const char *srcdir = get_srcdir();

    /* Check for error */
    if (NULL == srcdir)
        return NULL;

    /* Build path to test file */
    if ((strlen(srcdir) + strlen(filename) + 1) < sizeof(srcdir_testpath)) {
        snprintf(srcdir_testpath, sizeof(srcdir_testpath), "%s/%s", srcdir, filename);
        return srcdir_testpath;
    }

    /* If not enough space, just return NULL */
    return NULL;
} /* end get_srcdir_filename() */

/*-------------------------------------------------------------------------
 * Function:    get_srcdir
 *
 * Purpose:     Just return the srcdir path
 *
 * Return:      The string
 *
 *-------------------------------------------------------------------------
 */
const char *
get_srcdir(void)
{
    const char *srcdir = getenv("srcdir");

    /* Check for using the srcdir from configure time */
    if (NULL == srcdir)
        srcdir = config_srcdir;

    /* Build path to all test files */
    if ((strlen(srcdir) + 2) < sizeof(srcdir_path)) {
        snprintf(srcdir_path, sizeof(srcdir_path), "%s/", srcdir);
        return (srcdir_path);
    }
    else
        return NULL;
} /* end get_srcdir() */

intn
fuzzy_memcmp(const void *s1, const void *s2, int32 len, intn fuzz_factor)
{
    const uint8 *t1 = (const uint8 *)s1;
    const uint8 *t2 = (const uint8 *)s2;

    while (len > 0 && (int)ABS(*t2 - *t1) <= fuzz_factor) {
        t1++;
        t2++;
        len--;
    } /* end while */
    if (len == 0)
        return 0;
    else {
        return (intn)(*t1 - *t2);
    }
} /* end fuzzy_memcmp() */

void
print_mismatched(const void *s1, const void *s2, int32 size2cmp)
{
    int          ii;
    const uint8 *t1 = (const uint8 *)s1;
    const uint8 *t2 = (const uint8 *)s2;

    for (ii = 0; ii < size2cmp; ii++) {
        if (ABS(*t1 - *t2) > 0)
            fprintf(stderr, "item#%d: HDF(%d) - JPEG(%d)\n", ii, *t1, *t2);
        t1++;
        t2++;
    }
}
