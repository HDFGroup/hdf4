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

/********************************************************************
   Name: make_datafilename() - Generates the correct name for the test file.
   Description:
    Generate the correct name for the test file by prepending the source
    path if it exists, otherwise, assume it is the local directory.
   Return value:
        Returns SUCCEED if the file name is generated successfully, or
    FAIL, otherwise.
*********************************************************************/
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


/********************************************************************
   Name: make_SDS() - Creates and writes a 3-D unlimited SDS.
   Description:
    Calls SDcreate, SDwritedata, and SDendaccess to create a 3-D
    unlimited SDS, then close it.
    (Note: should be modified for more different ranks.)
   Return value:
        Returns the size of the data that had been written successfully.
   BMR - Dec 1, 2015
*********************************************************************/
int32 make_SDS(int32 sd_id, char* sds_name, int32 type, int32 rank,
            int32* dim_sizes, int32 unlim_dim, VOIDP written_data)
{
    int32 sds_id;
    int32 *start, *edges;
    int32 sds_size = 0, count = 0;
    intn  status, ii;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    start = (int32*)HDmalloc(sizeof(int32) * rank);
    edges = (int32*)HDmalloc(sizeof(int32) * rank);

    /* Create the array with the name defined in SDS_NAME */
    sds_id = SDcreate (sd_id, sds_name, type, rank, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set the parameters start and edges to write */
    for (ii = 0; ii < rank; ii++)
    {
    start[ii] = 0;
    edges[ii] = dim_sizes[ii];
    }

    /* Give real size to the unlimited dimension */
    if (dim_sizes[0] == SD_UNLIMITED)
    edges[0] = unlim_dim;

    /* Write the data */
    status = SDwritedata (sds_id, start, NULL, edges, written_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    for (ii = 0; ii < rank; ii++)
    {
    if (ii == 0)
        count = edges[0];
    else
        count = count * edges[ii];
    }
    sds_size = count * DFKNTsize(type);

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    HDfree(edges);
    HDfree(start);

    return(sds_size);

} /* make_SDS */

/********************************************************************
   Name: make_CompSDS() - Creates and writes 3D compressed SDS.
   Description:
    Calls SDcreate, SDsetcompress, SDwritedata, and SDendaccess to
    create an SDS with SKPHUFF compression, then close it.
    (Note: should be modified for different ranks.)
   Return value:
        Returns the size of the data that had been written successfully.
   BMR - Dec 1, 2015
*********************************************************************/
int32 make_CompSDS(int32 sd_id, char* sds_name, int32 type, int32 rank,
            int32* dim_sizes, VOIDP written_data)
{
    int32 sds_id;
    int32 *start, *edges;
    comp_coder_t comp_type;    /* Compression flag */
    comp_info c_info;          /* Compression structure */
    int32 sds_size = 0, count = 0;
    intn  status, ii;
    intn  num_errs = 0;  /* number of errors in compression test so far */

    start = (int32*)HDmalloc(sizeof(int32) * rank);
    edges = (int32*)HDmalloc(sizeof(int32) * rank);

    /* Define dimensions of the array to be created */
     /* dim_sizes[0] = Z_LENGTH;
    dim_sizes[1] = Y_LENGTH;
    dim_sizes[2] = X_LENGTH;
 */

    /* Create the array with the name defined in SDS_NAME */
    sds_id = SDcreate (sd_id, sds_name, type, rank, dim_sizes);
    CHECK(status, FAIL, "SDcreate");

    /* Set compression for the data set */
    comp_type = COMP_CODE_SKPHUFF;
    c_info.skphuff.skp_size = 18;
    status = SDsetcompress (sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "SDsetcompress");

    /* Set the parameters start and edges to write */
    for (ii = 0; ii < rank; ii++)
    {
    start[ii] = 0;
    edges[ii] = dim_sizes[ii];
    }

    /* Write the data */
    status = SDwritedata (sds_id, start, NULL, edges, written_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Compute the uncompressed data size, just to have makeCompSDS similar
       to the other create SDS functions; we don't need to verify the data
       size because we don't move compressed data to external file */
    for (ii = 0; ii < rank; ii++)
    {
    count = count + dim_sizes[ii];
    }
    sds_size = count * DFKNTsize(type);

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    HDfree(edges);
    HDfree(start);

    return(sds_size);
} /* make_CompSDS */

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
   BMR - Dec 1, 2015
*********************************************************************/
int32 make_Ext3D_SDS(int32 sd_id, char* sds_name, int32 type, int32 rank,
            int32* dim_sizes, VOIDP written_data,
            int32 offset, char* ext_file_name)
{
    int32 sds_id;
    int32 *start, *edges;
    int32 sds_size = 0, count;
    intn  status, ii;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    start = (int32*)HDmalloc(sizeof(int32) * rank);
    edges = (int32*)HDmalloc(sizeof(int32) * rank);

    /* Set the parameters start and edges to write */
    for (ii = 0; ii < rank; ii++)
    {
    start[ii] = 0;
    edges[ii] = dim_sizes[ii];
    }

    /* Define dimensions of the array to be created. */
     /* dim_sizes[0] = Z_LENGTH;
    dim_sizes[1] = Y_LENGTH;
    dim_sizes[2] = X_LENGTH;
 */

    /* Create the array with the name defined in SDS_NAME. */
    sds_id = SDcreate (sd_id, sds_name, type, rank, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    status = SDsetexternalfile (sds_id, ext_file_name, offset);
    CHECK(status, FAIL, "SDsetexternalfile");

    /* Write the data */
    status = SDwritedata (sds_id, start, NULL, edges, written_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    count = 1;
    for (ii = 0; ii < rank; ii++)
    {
    count = count * dim_sizes[ii];
    }
    sds_size = count * DFKNTsize(type);


    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    HDfree(edges);
    HDfree(start);

    return sds_size;
} /* make_Ext3D_SDS */

/********************************************************************
   Name: get_SDSbyName() - Find and open an SDS by name.
   Description:
    Calls SDnametoindex and SDselect to open a data set by its name.
   Return value:
        Returns the SDS' identifier.
   BMR - Dec 1, 2015
*********************************************************************/
int32 get_SDSbyName(int32 sd_id, char* sds_name)
{
    int32 sds_id, sds_index;
    intn  status;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    sds_index = SDnametoindex(sd_id, sds_name);
    CHECK(sds_index, FAIL, "SDnametoindex");

    /* Select the data set */
    sds_id = SDselect (sd_id, sds_index);
    CHECK(sds_id, FAIL, "SDselect");

    /* Return the data set id */
    return(sds_id);

} /* get_SDSbyName */

/********************************************************************
   Name: append_Data2SDS() - Selects a data set by name then write data to it.
   Description:
    Uses the helper function get_SDSbyName to find and open the
    data set, then calls SDwritedata to append data, and SDendaccess
    to close it.
   Return value:
        Returns the size of the data that had been written successfully.
   BMR - Dec 1, 2015
*********************************************************************/
int32 append_Data2SDS(int32 sd_id, char* sds_name, int32* start, int32* edges, void* ap_data)
{
    int32 sds_id, sds_index;
    int32 sds_size, ntype;
    int32 comp_size=0, uncomp_size=0;
char name[80];
    intn  status;
    intn  num_errs = 0;    /* number of errors in compression test so far */


    /* Find and select the data set */
    sds_id = get_SDSbyName(sd_id, sds_name);
    CHECK(sds_id, FAIL, "get_SDSbyName");

    /* Get the current size of this dataset */
    status = SDgetinfo(sds_id, name, NULL, NULL, &ntype, NULL);
    CHECK(status, FAIL, "SDgetinfo");

    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "SDgetdatasize");

    /* Append data to it */
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP)ap_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Calculate data set's size to verify later */
    sds_size = uncomp_size + edges[0] * edges[1] * edges[2] * DFKNTsize(ntype);

    /* Terminate access to the data set and file */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Return the size of data being written */
    return(sds_size);

} /* append_Data2SDS */

/********************************************************************
   Name: verify_datasize() - Checks data size
   Description:
    Calls SDgetdatasize then verifies the data size against the
    given data_size and reports if they are different.
   Return value:
        None.
   BMR - Dec 1, 2015
*********************************************************************/
void verify_datasize(int32 sds_id, int32 data_size, char* sds_name)
{
    int32 comp_size=0, uncomp_size=0;
    char  msg[80];
    intn  status;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    /* Get the size of data set's data */
    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "SDgetdatasize");
    sprintf(msg, "%s on data set %s\n", "SDgetdatasize", sds_name);
    VERIFY(data_size, uncomp_size, msg);

} /* verify_datasize */

#endif /* HDF */
