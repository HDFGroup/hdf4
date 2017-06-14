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

intn fuzzy_memcmp(const void *s1, const void *s2, int32 len, intn fuzz_factor)
{
    const uint8 *t1 = (const uint8 *) s1;
    const uint8 *t2 = (const uint8 *) s2;

    while (len > 0 && (int) ABS(*t2 - *t1) <= fuzz_factor)
      {
          t1++;
          t2++;
          len--;
      }     /* end while */
    if (len == 0)
        return (0);
    else
      {
          return ((intn) (*t1 - *t2));
      }
}   /* end fuzzy_memcmp() */

void print_mismatched(const void *s1, const void *s2, int32 size2cmp)
{
    int ii, jj, nn=0, kk;
    const uint8 *t1 = (const uint8 *) s1;
    const uint8 *t2 = (const uint8 *) s2;

    for (ii = 0; ii < size2cmp; ii++)
      {
        if (ABS(*t1 - *t2) > 0)
            fprintf(stderr, "item#%d: HDF(%d) - JPEG(%d)\n", ii, *t1, *t2);
          t1++;
          t2++;
      }
}

/* Generate the correct name for the test file, by prepending the source path
   if it exists, otherwise, assume it is the local directory */
   /* NOTE: should move all utilities into mfutil.c or something like that */
intn make_datafilename(char* basename, char* testfile, unsigned int size)
{
    char *srcdir = getenv("srcdir");
    char *tempfile = NULL;

    tempfile = (char *) HDmalloc(sizeof(char *) * (size+1));
    HDmemset(tempfile, '\0', size+1);

    /* Generate the correct name for the test file, by prepending the source path */
    if (srcdir && ((strlen(srcdir) + strlen(basename) + 1) < size))
    {
        strcpy(tempfile, srcdir);
        strcat(tempfile, "/");
    }

    /* Windows doesn't set srcdir, and generates files in a different relative
       path, so we need to special case here.  It is best to look for the
       testfile in the same path, and the Windows test script will make sure
       to put it there first.  - SJW 2007/09/19 (from tnetcdf.c) */
#ifndef _WIN32
    /* This is to get to the file when the library was built without srcdir
       option and the test is ran by ./hdftest in the test src directory
       instead of by make check.  - BMR 2007/08/09 */
    if (srcdir == NULL)
        strcpy(tempfile, "./");
#endif /* _WIN32 */

    /* Name of data file */
    strcat(tempfile, basename);

    /* Verify that file name is not NULL */
    if (tempfile == NULL || tempfile[0] == '\0')
        return FAIL;

    /* File name is generated, return it */
    HDstrcpy(testfile, tempfile);
    return SUCCEED;
}
