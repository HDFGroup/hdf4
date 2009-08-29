/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "mfhdf.h"

#ifdef macintosh
    #include <LowMem.h>
#endif

#ifdef HDF

#include "hdftest.h"

#define NETCDF_READ_TEST

#ifdef NETCDF_READ_TEST
static int16  netcdf_u16[2][3] = {{1, 2, 3}, 
                                   {4, 5, 6}};

char    testfile[512] = "";

/* Tests reading of netCDF file 'test1.nc' using the SDxxx inteface.
   Note not all features of reading SDS from netCDF files are tested here.
   Hopefully more tests will be added over time as needed/required. */
extern intn
test_netcdf_reading()
{
    int32 sd_id;
    int32 sds_id;
    int32 n_datasets;
    int32 n_file_attrs;
    int32 index;
    int32 rank;
    int32 num_type;
    int32 attributes;
    int32 dim_sizes[H4_MAX_VAR_DIMS];
    int32 start[H4_MAX_VAR_DIMS];
    int32 edges[H4_MAX_VAR_DIMS];
    int16 array_data[2][3];
    char  name[H4_MAX_NC_NAME];
    int32 status;
    intn  i, j;
    int   num_errs = 0;    /* number of errors so far */
    const char *basename = "test1.nc";
    char  *srcdir = getenv("srcdir");

    /* Output message about test being performed */
    TESTING("reading of netCDF file using the SDxxx inteface (tnetcdf.c)");

    /* Generate the correct name for the test file, by prepending the source path */
    if (srcdir && ((strlen(srcdir) + strlen(basename) + 1) < sizeof(testfile))) 
    {
        strcpy(testfile, srcdir);
        strcat(testfile, "/");
    }

    /* Windows doesn't set srcdir, and generates files in a different relative
       path, so we need to special case here.  It is best to look for the
       testfile in the same path, and the Windows test script will make sure
       to put it there first.  - SJW 2007/09/19 */
#ifndef _WIN32
    /* This is to get to the file 'test1.nc' when the library was built without
       srcdir option and the test is ran by ./hdftest in the src directory
       hdf4/mfhdf/libsrc instead of by make check.  - BMR 2007/08/09 */
    if (srcdir == NULL)
	strcpy(testfile, "./");
#endif /* _WIN32 */

    strcat(testfile, basename);

    /* Open the file 'test1.nc' and initialize the SD interface. */
    sd_id = SDstart(testfile, DFACC_RDONLY);
    CHECK(sd_id, FAIL, "netCDF Read Test 1. SDstart failed on file test1.nc");

    /* Determine the contents of the file. */
    status = SDfileinfo(sd_id, &n_datasets, &n_file_attrs);
    CHECK(status, FAIL, "netCDF Read Test 1. SDfileinfo failed on file test1.nc");

    /* There should be 8 datasets in the file and 1 file level attribute */
    if (n_datasets != 8 )
      {
          fprintf(stderr,"netCDF Read Test 1: SDfileinfo returned wrong number of datasets in file test1.nc \n");
          num_errs++;
      }

    if (n_file_attrs != 1 )
      {
          fprintf(stderr,"netCDF Read Test 1: SDfileinfo returned wrong number of file attributes in file test1.nc \n");
          num_errs++;
      }

    /* Access and find the 2-dim dataset of data-type shorts(DFNT_INT16). 
       in the file while querying every data set in the file. 
       There should only be one dataset that matches and is named 'order'.*/
    for (index = 0; index < n_datasets; index++) 
      {
          sds_id = SDselect(sd_id, index);
          CHECK(sds_id, FAIL, "netCDF Read Test 1. SDselect failed for dataset in  file test1.nc");

          status = SDgetinfo(sds_id, name, &rank, dim_sizes, &num_type, &attributes);
          CHECK(status, FAIL, "netCDF Read Test 1. SDgetinfo failed for dataset in  file test1.nc");

          /* look for the dataset 'order' based on rank and number type */
          if (rank == 2 && num_type == (int32)DFNT_INT16)
            { /* should only be one of these */
                start[0] =  start[1] = 0; 
                edges [0] = dim_sizes[0];
                edges [1] = dim_sizes[1];
                status = SDreaddata (sds_id, start, NULL, edges, (VOIDP) array_data);
                CHECK(status, FAIL, "netCDF Read Test 1. SDreaddata failed for dataset in  file test1.nc");

                /* check the data against our buffer 'netcdf_u16[][]' */
                for (j = 0; j < dim_sizes[0]; j++ )
                  {
                      for (i = 0; i < dim_sizes[1]; i++)
                        {
                            if (array_data[j][i] != netcdf_u16[j][i])
                              {
                                  fprintf(stderr,"netCDF Read Test 1: bogus val read: wanted netcdf[%d][%d]=%d, read array[%d][%d]=%d \n",
                                          j,i,netcdf_u16[j][i], j,i, array_data[j][i] );
                              }
                        } /* end for inner */
                  } /* end for outer */
            }

          /* end access to this SDS */
          status = SDendaccess(sds_id);
          CHECK(status, FAIL, "netCDF Read Test 1. SDendaccess failed for dataset in  file test1.nc");
      } /* end querying every dataset in file */

    /* Terminate access to the SD interface and close the file. */
    status = SDend(sd_id);
    CHECK(status, FAIL, "netCDF Read Test 1. SDend failed for file test1.nc");

    if (num_errs == 0) PASSED();
    return num_errs;
} /* test_netcdf_reading() */
#endif /* NETCDF_READ_TEST */
#endif /* ifdef HDF */
