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

#include <stdlib.h>
#include <string.h>

#include "mfhdf.h"
#include "hdftest.h"
#include "srcdir_str.h"

/* Buffer to construct path in and return pointer to */
static char srcdir_path_g[1024];

/* Buffer to construct file in and return pointer to */
static char srcdir_testpath_g[1024];

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

    memset(srcdir_testpath_g, 0, sizeof(srcdir_testpath_g));

    /* Build path to test file */
    if ((strlen(srcdir) + strlen(filename) + 1) < sizeof(srcdir_testpath_g)) {
        int n = snprintf(srcdir_testpath_g, sizeof(srcdir_testpath_g), "%s/%s", srcdir, filename);

        /* Check for truncation */
        if (n < (int)strlen(srcdir) + (int)strlen(filename) + 1)
            return NULL;
        else
            return srcdir_testpath_g;
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

    memset(srcdir_path_g, 0, sizeof(srcdir_path_g));

    /* Build path to all test files */
    if ((strlen(srcdir) + 2) < sizeof(srcdir_path_g)) {
        int n = snprintf(srcdir_path_g, sizeof(srcdir_path_g), "%s/", srcdir);

        /* Check for truncation */
        if (n < (int)strlen(srcdir) + 1)
            return NULL;
        else
            return srcdir_path_g;
    }
    else
        return NULL;
} /* end get_srcdir() */

/********************************************************************
   Name: make_sourcepath() - Generates the source path
   Description:
        Generate the path of srcdir if it exists, otherwise, assume
        it is the current directory.
   Return value:
        Returns SUCCEED if the source path is generated successfully,
        or FAIL, otherwise.
*********************************************************************/
int
make_sourcepath(char *src_path, unsigned int size)
{
    char *srcdir  = getenv("srcdir");
    char *tempdir = NULL;

    tempdir = (char *)malloc(size * sizeof(char));
    CHECK_ALLOC(tempdir, "tempdir", "make_sourcepath");
    memset(tempdir, 0, size);

    /* Generate the source path */
    if (srcdir && ((strlen(srcdir)) + 1) < size) {
        strcpy(tempdir, srcdir);
        strcat(tempdir, "/");
        strcat(tempdir, "\0");
    }

    /* No srcdir option */
    if (srcdir == NULL)
        strcpy(tempdir, "./");

    /* Verify that source path is not NULL */
    if (tempdir == NULL || tempdir[0] == '\0')
        return FAIL;

    strcpy(src_path, tempdir);
    free(tempdir);
    return SUCCEED;
}

/********************************************************************
   Name: make_SDS() - Creates and writes a 3-D unlimited SDS.
   Description:
    Calls SDcreate, SDwritedata, and SDendaccess to create a 3-D
    unlimited SDS, then close it.
    (Note: should be modified for more different ranks.)
   Return value:
        Returns the size of the data that had been written successfully,
        of FAIL.

*********************************************************************/
int32
make_SDS(int32 sd_id, const char *sds_name, int32 type, int32 rank, int32 *dim_sizes, int32 unlim_dim,
         void *written_data)
{
    int32  sds_id;
    int32 *start, *edges;
    int32  sds_size = 0, count = 0;
    int    status, ii;
    int    num_errs = 0; /* number of errors in compression test so far */

    start = (int32 *)malloc(sizeof(int32) * (size_t)rank);
    CHECK_ALLOC(start, "start", "make_SDS");
    edges = (int32 *)malloc(sizeof(int32) * (size_t)rank);
    CHECK_ALLOC(edges, "edges", "make_SDS");

    /* Create the array with the name defined in SDS_NAME */
    sds_id = SDcreate(sd_id, sds_name, type, rank, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the parameters start and edges to write */
    for (ii = 0; ii < rank; ii++) {
        start[ii] = 0;
        edges[ii] = dim_sizes[ii];
    }

    /* Give real size to the unlimited dimension */
    if (dim_sizes[0] == SD_UNLIMITED)
        edges[0] = unlim_dim;

    /* Write the data */
    status = SDwritedata(sds_id, start, NULL, edges, written_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    for (ii = 0; ii < rank; ii++) {
        if (ii == 0)
            count = edges[0];
        else
            count = count * edges[ii];
    }
    sds_size = count * DFKNTsize(type);

    /* Terminate access to the data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    free(edges);
    free(start);

    /* Return the size of data being written if no error */
    if (num_errs == 0)
        return (sds_size);
    else
        return FAIL;
} /* make_SDS */

/********************************************************************
   Name: make_Ext3D_SDS() - Creates and writes a 3D SDS with external data.
   Description:
    Calls SDcreate, SDsetexternalfile, SDwritedata, and SDendaccess
    to create a 3-D SDS with external data storage, then close it.
    (Note: should be modified for different ranks.)
   Return value:
        Returns the size of the data that had been written successfully.
   Return value:
        None.

*********************************************************************/
int32
make_Ext3D_SDS(int32 sd_id, char *sds_name, int32 type, int32 rank, int32 *dim_sizes, void *written_data,
               int32 offset, char *ext_file_name)
{
    int32  sds_id;
    int32 *start, *edges;
    int32  sds_size = 0, count;
    int    status   = 0, ii;
    int    num_errs = 0; /* number of errors in compression test so far */

    start = (int32 *)malloc(sizeof(int32) * (size_t)rank);
    CHECK_ALLOC(start, "start", "make_Ext3D_SDS");
    edges = (int32 *)malloc(sizeof(int32) * (size_t)rank);
    CHECK_ALLOC(edges, "edges", "make_Ext3D_SDS");

    /* Set the parameters start and edges to write */
    for (ii = 0; ii < rank; ii++) {
        start[ii] = 0;
        edges[ii] = dim_sizes[ii];
    }

    /* Create the array with the name defined in SDS_NAME. */
    sds_id = SDcreate(sd_id, sds_name, type, rank, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    status = SDsetexternalfile(sds_id, ext_file_name, offset);
    CHECK(status, FAIL, "SDsetexternalfile");

    /* Write the data */
    status = SDwritedata(sds_id, start, NULL, edges, written_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    count = 1;
    for (ii = 0; ii < rank; ii++) {
        count = count * dim_sizes[ii];
    }
    sds_size = count * DFKNTsize(type);

    /* Terminate access to the data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    free(edges);
    free(start);

    /* Return the size of data being written if no error */
    if (num_errs == 0)
        return (sds_size);
    else
        return FAIL;
} /* make_Ext3D_SDS */

/********************************************************************
   Name: get_SDSbyName() - Find and open an SDS by name.
   Description:
    Calls SDnametoindex and SDselect to open a data set by its name.
   Return value:
        Returns the SDS' identifier if successful, or FAIL, otherwise.

*********************************************************************/
int32
get_SDSbyName(int32 sd_id, const char *sds_name)
{
    int32 sds_id, sds_index;
    int   num_errs = 0; /* number of errors in compression test so far */

    sds_index = SDnametoindex(sd_id, sds_name);
    CHECK(sds_index, FAIL, "SDnametoindex");

    /* Select the data set */
    sds_id = SDselect(sd_id, sds_index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Return the data set id or FAIL */
    return (sds_id);

} /* get_SDSbyName */

/********************************************************************
   Name: append_Data2SDS() - Selects a data set by name then write data to it.
   Description:
    Uses the helper function get_SDSbyName to find and open the
    data set, then calls SDwritedata to append data, and SDendaccess
    to close it.
   Return value:
        Returns the size of the data that had been written successfully,
        or FAIL.

*********************************************************************/
int32
append_Data2SDS(int32 sd_id, char *sds_name, int32 *start, int32 *edges, void *ap_data)
{
    int32 sds_id;
    int32 sds_size, ntype;
    int32 comp_size = 0, uncomp_size = 0;
    char  name[80];
    int   status   = 0;
    int   num_errs = 0; /* number of errors in compression test so far */

    /* Find and select the data set */
    sds_id = get_SDSbyName(sd_id, sds_name);
    CHECK(sds_id, FAIL, "get_SDSbyName");

    /* Get the current size of this dataset */
    status = SDgetinfo(sds_id, name, NULL, NULL, &ntype, NULL);
    CHECK(status, FAIL, "SDgetinfo");

    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "SDgetdatasize");

    /* Append data to it */
    status = SDwritedata(sds_id, start, NULL, edges, (void *)ap_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    sds_size = uncomp_size + edges[0] * edges[1] * edges[2] * DFKNTsize(ntype);

    /* Terminate access to the data set and file */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Return the size of data being written if no error */
    if (num_errs == 0)
        return (sds_size);
    else
        return FAIL;

} /* append_Data2SDS */

/********************************************************************
   Name: verify_datasize() - Checks data size
   Description:
    Calls SDgetdatasize then verifies the data size against the
    given data_size and reports if they are different.
   Return value:
        SUCCEED or FAIL

*********************************************************************/
int
verify_datasize(int32 sds_id, int32 data_size, const char *sds_name)
{
    int32 comp_size = 0, uncomp_size = 0;
    char  msg[80];
    int   status   = 0;
    int   num_errs = 0; /* number of errors in compression test so far */

    /* Get the size of data set's data */
    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "SDgetdatasize");
    sprintf(msg, "%s on data set %s\n", "SDgetdatasize", sds_name);
    VERIFY(data_size, uncomp_size, msg);

    if (num_errs == 0)
        return SUCCEED;
    else
        return FAIL;
} /* verify_datasize */
