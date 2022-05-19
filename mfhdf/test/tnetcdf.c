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

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define NETCDF_READ_TEST

#ifdef NETCDF_READ_TEST

/* Get the srcdir string to locate the input files */
#define srcdir getenv("srcdir")

/********************************************************************
   Name: test_read_dim() - tests reading a dimension scale in a netCDF file.

   Description:
        The fix to detect a dimension scale/coordinate variable in HDF file
    caused incorrect result when attempting to retrieve the dimension
    scale's number type from a netCDF file (bugzilla #1644.)
    This test verifies that the number type of dimension scale is
    retrieved correctly after the fix.

   Return value:
        The number of errors occurred in this routine.

   BMR - Sep 23, 2009
*********************************************************************/

/* Note: This file was generated by the netCDF ncgen tool from a CDL
   file.  The CDL file was first generated by the netCDF ncdump tool
   from the netCDF file slice.0000.nc in bugzilla 1644 report; then
   it was edited to change the dimensions to much smaller values. */
#define NC_FILE  "smallslice.0000.nc"

static intn test_read_dim()
{
    int32 fid, sds_id, status, dim1_id;
    int32 dim_sizes[H4_MAX_VAR_DIMS];        /* read dimensions */
    int32 array_rank, num_type, attributes;
    char  dim_name[H4_MAX_NC_NAME], name[H4_MAX_NC_NAME];
    int32 size, dim_data_type, dim_num_attrs;
    char  testfile[512] = "";
    intn  num_errs = 0;    /* number of errors so far */

    /* Generate the correct name for the test file, by prepending the source path */
    if (srcdir && ((strlen(srcdir) + strlen(NC_FILE) + 1) < sizeof(testfile)))
    {
        strcpy(testfile, srcdir);
        strcat(testfile, "/");
    }

    /* Windows doesn't set srcdir, and generates files in a different relative
       path, so we need to special case here.  It is best to look for the
       testfile in the same path, and the Windows test script will make sure
       to put it there first.  - SJW 2007/09/19 */
#if !defined _WIN32
    /* This is to get to the file when the library was built without
       srcdir option and the test is ran by ./hdftest in the src directory
       hdf4/mfhdf/libsrc instead of by make check.  - BMR 2007/08/09 */
    if (srcdir == NULL)
    strcpy(testfile, "./");
#endif /* _WIN32 */

    strcat(testfile, NC_FILE);

    /* Create the file defined by BASIC_FILE and initiate the SD interface. */
    fid = SDstart(testfile, DFACC_READ);
    CHECK(fid, FAIL, "SDstart");

    if (fid != FAIL) /* solution to stop hundreds of errors */
    {

    /* Access first dataset to see what it is */
    sds_id = SDselect(fid, 0);
    CHECK(sds_id, FAIL, "SDselect");

    /* Get info of the dataset and verify them: it is a LENGTH0 x LENGTH1
       array of type DFNT_INT16 and is named by DS0_NAME */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Get the first dimension id */
    dim1_id = SDgetdimid(sds_id, 0);
    CHECK(dim1_id, FAIL, "SDgetdimid");

    /* Read the dimension information */
    status = SDdiminfo(dim1_id, dim_name, &size, &dim_data_type, &dim_num_attrs);
    CHECK(status, FAIL, "SDdiminfo");
    VERIFY_CHAR(dim_name, "LON", "SDdiminfo");
    VERIFY(dim_data_type, 6, "SDdiminfo");

    /* Terminate access to the array. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "SDend");

    } /* SDstart failed */

    /* Return the number of errors that's been kept track of so far */
    return num_errs;

} /* test_read_dim */

static int16  netcdf_u16[2][3] = {{1, 2, 3},
                                   {4, 5, 6}};

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
    intn  num_errs = 0;    /* number of errors so far */
    const char *basename = "test1.nc";
    char  testfile[512] = "";

    /* Output message about test being performed */
    TESTING("reading of netCDF file using the SDxxx interface (tnetcdf.c)");

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
#if !defined _WIN32
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

    if (sd_id != FAIL) /* temporary solution to stop hundreds of errors */
    {

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

    } /* SDstart failed */

    /* Test reading dimension scale - bugzilla 1644 */
    num_errs = num_errs + test_read_dim();

    if (num_errs == 0) PASSED();
    return num_errs;
} /* test_netcdf_reading() */
#endif /* NETCDF_READ_TEST */


#endif /* HDF */
