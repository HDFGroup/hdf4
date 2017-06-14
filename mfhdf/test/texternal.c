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

#define EXTTST    "exttst.hdf"        /* main file for external file test */
#define EXTFILE   "SD_external_file"    /* file to contain external data */
#define EXTFILE1  "SD_external_file 2"    /* file to contain external data */
#define EXTSDS    "ExternalDataSet"    /* data set written with external data
                    right after creation */
#define EXTSDS2   "ExternalDataSet 2"    /* data set first empty then written
                    with external data */
#define WRAPSDS   "WrapperDataSet"    /* data set pointing to external data */
#define NOEXTSDS  "NoExternalDataSet"    /* data set with data in main file */
#define EXTFILE2  "ExternalSDSexisting"    /* data set having data */
#define EXTFILE3  "ShouldNotHappen"    /* data set already is external */
#define OFFSET    24
#define NUM_SDS   4
#define SDS1      "Dataset 1"
#define SDS2      "Dataset 2"
#define SDS3      "Dataset 3"
#define SDS4      "Dataset 4"
#define RANK3     3
#define X_LENGTH  4
#define Y_LENGTH  5
#define Z_LENGTH  6
#define RANK2     2
#define DIM1      5
#define DIM2      5

void verify_data(int32 sd_id, int32 sds_ind);

/* Same set of data for every 3-dim data set.  Initialized in test_external(). */
int32 written_data[Z_LENGTH][Y_LENGTH][X_LENGTH];

/* Appended data or hyperslab */
int32 ap_data[1][Y_LENGTH][X_LENGTH];

/********************************************************************
   Name: test_setexternal() - tests basic funtionalities in storing
            data in an external file

   Description:
    This function tests three scenarios:
    - Data written in main file then moved to external file and modified
    - Data in external file is pointed to by a "wrapper" data set
    - Empty data set is written with data in the external file

    The main contents include:
    - Data written in main file then moved and modified
    + create and write the entire data set in the main file
    + move the data to the external file with SDsetexternalfile
    + modify this external data
    - Data in external file pointed to by a "wrapper" data set
    + create a data set in the main file, i.e., the wrapper data set
    + have the wrapper pointed to part of the external data that
        belongs to the external data set above
    + read the wrapper's data and verify
    - Empty data set is written with data in the external file
    + create a data set in the main file and close it
    + re-open the data set
    + promote it to external data set, i.e., SDsetexternalfile
    + write data to the data set

   Return value:
    The number of errors occurred in this routine.

   BMR - Jan 16, 2009
*********************************************************************/
static int test_setexternal()
{
    int32 sd_id, sds_id;
    int32 start[2], edges[2], dimsizes[2], nt, offset;
    int32 idata[DIM1 * DIM2];
    int ii;
    intn status;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    /* Create an HDF file */
    sd_id = SDstart(EXTTST, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create a data set in the HDF file */
    nt = DFNT_INT32 | DFNT_NATIVE;
    dimsizes[0] = DIM1;
    dimsizes[1] = DIM2;
    sds_id = SDcreate(sd_id, EXTSDS, nt, RANK2, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate: Failed to create a new data set 'ExternalDataSet' for external promotion");

    /* Initialize data to write out */
    for(ii = 0; ii < dimsizes[0] * dimsizes[1]; ii++)
        idata[ii] = ii;

    /* Write data to the entire data set */
    start[0] = start[1] = 0;
    edges[0]   = dimsizes[0];
    edges[1]   = dimsizes[1];

    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    /* Promote the data set to an external data set by storing its data in
       the external file EXTFILE */
    status = SDsetexternalfile(sds_id, EXTFILE, 0);
    CHECK(status, FAIL, "SDsetexternalfile");

    for(ii = 0; ii < 3*dimsizes[1]; ii++)
        idata[ii] = ii * 10;

    /* Write data to part of the newly promoted data set which now contains
       data in the external file */
    start[0] = start[1] = 0;
    edges[0]  = 3;
    edges[1]  = dimsizes[1];
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    /* End access to the data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Need to close to flush external info to the HDF file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Open the HDF file again */
    sd_id = SDstart(EXTTST, DFACC_RDWR);
    CHECK(sd_id, FAIL, "SDstart (again)");

    /* Create a data set in the HDF file */
    dimsizes[0] = 3;
    dimsizes[1] = 3;
    sds_id = SDcreate(sd_id, WRAPSDS, nt, 2, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a new data set('WrapperDataSet') for external wrapping");

    /* Promote the regular data set to a "wrapper" one by making it point to
       the actual data in the external file 'extfile.hdf'.
       Note that only a subset of the existing data (which belongs to the
       previous data set, EXTSDS) is pointed to by the "wrapper"
       data set.  The subset is specified by dimsizes array */
    offset = DFKNTsize(nt) * 2;
    status = SDsetexternalfile(sds_id, EXTFILE, offset);
    CHECK(status, FAIL, "SDsetexternalfile");

    /* End access to the data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Need to close to flush external info to the HDF, or main, file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Open the HDF file again */
    sd_id = SDstart(EXTTST, DFACC_RDWR);
    CHECK(sd_id, FAIL, "SDstart (again)");

    sds_id = get_SDSbyName(sd_id, WRAPSDS);

    /* Read and verify data via the "wrapper" data set */
    {
    int32 odata[9];

    /* Read data back from this "wrapper" data set */
    start[0] = start[1] = 0;
    edges[0]  = 3;
    edges[1]  = 3;
    status = SDreaddata(sds_id, start, NULL, edges, (VOIDP) odata);
    CHECK(status, FAIL, "SDreaddata");

    /* Verify data read back in */
    for(ii = 0; ii < edges[0]*edges[1]; ii++)
    {
        if(odata[ii] != (ii + 2) * 10)
        {
        fprintf(stderr, "Bogus val in loc %d in wrapper dset want %d  got %ld\n",
        ii, (ii + 2) * 10, (long)odata[ii]);
        num_errs++;
        }
    }
    }

    /* End access to the wrapper data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Create an empty data set then write external data to it */

    /* Create data set EXTSDS2 */
    nt = DFNT_INT32 | DFNT_NATIVE;
    dimsizes[0] = X_LENGTH;
    dimsizes[1] = Y_LENGTH;
    sds_id = SDcreate(sd_id, EXTSDS2, nt, 2, dimsizes);
    CHECK(sds_id, FAIL, "SDcreate: Failed to create a new data set for testing writing external data to an empty data set");

    /* Close data sets */
    status = SDendaccess(sds_id);

    /* Re-open the named data set, id is checked by callee */
    sds_id = get_SDSbyName(sd_id, EXTSDS2);

    /* Make this data set to have external data in a new file */
    status = SDsetexternalfile(sds_id, EXTFILE1, 0);

    /* initialize data to write out */
    for(ii = 0; ii < dimsizes[0]*dimsizes[1]; ii++)
        idata[ii] = ii;

    /* Write data to all of data set EXTSDS2 in the file EXTFILE1 */
    start[0] = start[1] = 0;
    edges[0] = dimsizes[0];
    edges[1] = dimsizes[1];
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    /* Close data sets */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Close HDF file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_setexternal() */

/********************************************************************
   Name: test_getexternal() - tests getting external file info on
    various data sets

   Description:
    The main contents include:
    - Create and write a data set in the main file only, i.e., non-external
    data set
    - Get and verify external file info for external data set and
    wrapper data set
    - Verifying that there is no external file info from the non-external
    data set

   Return value:
    The number of errors occurred in this routine.

   BMR - Jan 16, 2009
*********************************************************************/
static int test_getexternal()
{
    int32 sd_id, sds_id, noextsds;
    intn  name_len=0;
    char *extfile_name;
    int32 offset=0, length=0;
    int32 start[2], edges[2], dimsizes[2], nt;
    int32 idata[DIM1*DIM2];
    int   ii;
    intn  num_errs = 0;    /* number of errors in compression test so far */
    intn  status = SUCCEED;

    /* Open file 'exttst.hdf' again */
    sd_id = SDstart(EXTTST, DFACC_RDWR);
    CHECK(sd_id, FAIL, "SDstart (again)");

    /* Create and write a data set in the main file */

    /* Create data set NOEXTSDS */
    nt = DFNT_INT32 | DFNT_NATIVE;
    dimsizes[0] = DIM1;
    dimsizes[1] = DIM2;
    noextsds = SDcreate(sd_id, NOEXTSDS, nt, 2, dimsizes);
    CHECK(noextsds, FAIL, "SDcreate: Failed to create a new data set 'NoExternalDataSet' for testing SDSgetexternalfile on a non-external element");

    /* initialize data to write out */
    for(ii = 0; ii < 25; ii++)
        idata[ii] = ii;

    /* Write data to all of data set NOEXTSDS in main file */
    start[0] = start[1] = 0;
    edges[0] = edges[1] = DIM1;
    status = SDwritedata(noextsds, start, NULL, edges, (VOIDP) idata);
    CHECK(status, FAIL, NOEXTSDS);
    CHECK(status, FAIL, "SDwritedata");

    /* Close data sets */
    status = SDendaccess(noextsds);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Test getting external info on an external data set; should return the
     * external file information successfully.
     */

    /* Get access to the data set named EXTSDS */
    sds_id = get_SDSbyName(sd_id, EXTSDS);

    /* Call SDgetexternalfile the first time passing in 0 for external
    file name length to get the actual length - SDgetexternalfile is
    deprecated as of 4.2.7 because it missed the length argument */
    { /* deprecated */
        name_len = SDgetexternalfile(sds_id, 0, NULL, NULL);
        VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalfile");

        extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
        CHECK_ALLOC(extfile_name, "extfile_name", "SDgetexternalfile");
        HDmemset(extfile_name, '\0', name_len+1);

        /* Call SDgetexternalfile again and get the external file info */
        name_len = SDgetexternalfile(sds_id, name_len+1, extfile_name, &offset);
        VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalfile");
        VERIFY_CHAR(EXTFILE, extfile_name, "SDgetexternalfile");
    }

    /* Call SDgetexternalinfo the first time passing in 0 for external
    file name length to get the actual length */
    name_len = SDgetexternalinfo(sds_id, 0, NULL, NULL, NULL);
    VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalinfo");

    /* Test passing in NULL pointer for external file name buffer, should
    fail gracefully */
    {
    char *null_buffer=NULL;
    intn ret_code=0;
    ret_code = SDgetexternalinfo(sds_id, name_len+1, null_buffer, &offset, &length);
    VERIFY(ret_code, FAIL, "SDgetexternalinfo");
    }

    /* Prepare buffer for external file name */
    extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(extfile_name, "extfile_name", "SDgetexternalinfo");
    HDmemset(extfile_name, '\0', name_len+1);

    /* Call SDgetexternalinfo again and get the external file info */
    name_len = SDgetexternalinfo(sds_id, name_len+1, extfile_name, &offset, &length);
    VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalinfo");
    VERIFY_CHAR(EXTFILE, extfile_name, "SDgetexternalinfo");

    /* Test passing in smaller buffer for external file name than actual;
    name should be truncated */
    {
        char *short_name = (char *) HDmalloc(sizeof(char *) * (name_len));
        HDmemset(short_name, '\0', name_len);
        HDstrncpy(short_name, EXTFILE, name_len-2);
        HDmemset(extfile_name, '\0', name_len);

        /* Call SDgetexternalinfo again with smaller buffer size and verify
           that SDgetexternalinfo reads the name truncated to the given
           buffer size*/
        name_len = SDgetexternalinfo(sds_id, name_len-2, extfile_name, &offset, &length);
        VERIFY(name_len, (intn)HDstrlen(extfile_name), "SDgetexternalinfo");
        VERIFY_CHAR(short_name, extfile_name, "SDgetexternalinfo");
        HDfree(short_name);
    }

    /* Close the data set */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /*
     * Test getting external info on a wrapper data set; should return the
     * external file information successfully.
     */

    /* Get access to the data set named WRAPSDS */
    sds_id = get_SDSbyName(sd_id, WRAPSDS);

    /* Call SDgetexternalinfo the first time passing in 0 for external
    file name length to get the actual length */
    name_len = SDgetexternalinfo(sds_id, 0, NULL, NULL, NULL);
    VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalinfo");

    /* Test passing in NULL pointer for external file name buffer, should
    fail gracefully */
    {
    char *null_buffer=NULL;
    intn ret_code=0;
    ret_code = SDgetexternalinfo(sds_id, name_len+1, null_buffer, &offset, &length);
    VERIFY(ret_code, FAIL, "SDgetexternalinfo");
    }

    extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(extfile_name, "extfile_name", "SDgetexternalinfo");
    HDmemset(extfile_name, '\0', name_len+1);

    /* Call SDgetexternalinfo again and get the external file info */
    name_len = SDgetexternalinfo(sds_id, name_len+1, extfile_name, &offset, &length);
    VERIFY(name_len, (intn)HDstrlen(EXTFILE), "SDgetexternalinfo");
    VERIFY_CHAR(EXTFILE, extfile_name, "SDgetexternalinfo");

    /*
     * Test getting external info on a non-external data set; should return
     * no external file information
     */

    /* Get access to the data set named NOEXTSDS */
    noextsds = get_SDSbyName(sd_id, NOEXTSDS);

    /* Call SDgetexternalinfo on the SDS that doesn't have external
    element, should return 0 for length of external file name */
    name_len = SDgetexternalinfo(noextsds, 0, NULL, NULL, NULL);
    VERIFY(name_len, 0, "SDgetexternalinfo");

    status = SDendaccess(noextsds);
    CHECK(status, FAIL, "SDendaccess");

    /* Close file 'exttst.hdf' */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_getexternal() */

/********************************************************************
   Name: test_mult_setexternal() - tests setting external multiple times

   Description:
    The main contents include:
    - Create and write an external data set to FILE_NAME, then close
    it and the file.
    - Re-open the file
    - Call SDsetexternal on the data set, which is already an external SDS.
    This is to make sure HDFFR-1516 is fixed.
    - Re-open the file and verify data of each data set.

   Return value:
    The number of errors occurred in this routine.

   BMR - Jan 16, 2009
*********************************************************************/
int test_mult_setexternal()
{
    int32 sd_id, sds1_id, sds2_id, sds3_id, sds4_id;
    int32 ap_start[3], ap_edges[3], dim_sizes[3];
    int32 sds1_size=0, sds2_size=0, sds3_size=0;
    char *extfile_name;
    intn  name_len = 0;
    intn  status = SUCCEED;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart (EXTTST, DFACC_CREATE);
    CHECK(status, FAIL, "SDstart");

    dim_sizes[0] = Z_LENGTH;
    dim_sizes[1] = Y_LENGTH;
    dim_sizes[2] = X_LENGTH;

    /* Create data set SDS1 and write data to the external file; the returned
       value is the size of the data had been written for this sds */
    sds1_size = make_Ext3D_SDS(sd_id, SDS1, DFNT_INT32, 3, dim_sizes, (VOIDP)written_data, OFFSET, EXTFILE2);

    /* Close the file to flush */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file */
    sd_id = SDstart (EXTTST, DFACC_RDWR);
    CHECK(status, FAIL, "SDstart");

    /* Move data from an external data set, SDS1, into the external file again.
       This simulates the situation of the example being run more than once,
       causing failure in daily test.  This action should have no effect now.
       (HDFFR-1521)-BMR */

    /* Select the named data set, id is checked by callee */
    sds1_id = get_SDSbyName(sd_id, SDS1);

    /* Try to move it to the external file again; should neither fail, nor have
       any effect.  External file name should still be EXTFILE2 */
    status = SDsetexternalfile (sds1_id, EXTFILE3, OFFSET);
    if (status < 0)
        fprintf(stderr, "SDsetexternalfile still fail when called more than once on an SDS\n");

    /* Verify that external file still is EXTFILE2, and not EXTFILE3 */

    /* Call SDgetexternalinfo the first time passing in 0 for external
    file name length to get the actual length */
    name_len = SDgetexternalinfo(sds1_id, 0, NULL, NULL, NULL);
    if (name_len <= 0)
        fprintf(stderr, "SDsetexternalfile should return length greater than 0\n");

    /* Prepare buffer for external file name */
    extfile_name = (char *) HDmalloc(sizeof(char *) * (name_len+1));
    CHECK_ALLOC(extfile_name, "extfile_name", "SDgetexternalinfo");
    HDmemset(extfile_name, '\0', name_len+1);

    /* Call SDgetexternalinfo again and get the external file info */
    name_len = SDgetexternalinfo(sds1_id, name_len+1, extfile_name, NULL, NULL);
    VERIFY(name_len, (intn)HDstrlen(EXTFILE2), "SDgetexternalinfo");
    VERIFY_CHAR(EXTFILE2, extfile_name, "SDgetexternalinfo");

    /* Close the data set and the file */
    status = SDendaccess(sds1_id);
    CHECK(status, FAIL, "SDendaccess SDS1");
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to verify written data */
    sd_id = SDstart (EXTTST, DFACC_RDWR);
    CHECK(status, FAIL, "SDstart");

    /* Read data of the data set and verify against the original */
    verify_data(sd_id, 0);

    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_mult_setexternal() */

/********************************************************************
   Name: test_special_combos() - tests combining other specialness with
                external data feature.
   Description:
    The main contents include:
    - Open the file from test_mult_setexternal
    - Create and write two unlimited-dimension data sets, SDS2 and SDS3
    - Append data to the first unlimited-dimension data set, SDS2, to
    create linked-block element.
    - Move data of the first unlimited-dim data set, SDS2, to the external
    file to test the combination of SPECIAL_LINKED and SPECIAL_EXT.
    - Move data of SDS3 to the external file to test unlimited and
    SPECIAL_EXT without linked-block element.
    - Make a compressed data set, SDS4, in the main file.
    - Attempt to move data of SDS4 to the external file should fail, because
    SPECIAL_COMP and SPECIAL_EXT combo is not supported.
    - Close everything.
    - Re-open the file and verify data of each data set in the file.

   Return value:
    The number of errors occurred in this routine.

   BMR - Jan 16, 2009
*********************************************************************/
int test_special_combos()
{
    int32 sd_id, sds1_id, sds2_id, sds3_id, sds4_id;
    int32 sds_size;
    int32 num_sds = 0, num_attrs = 0;
    int32 ap_start[3], ap_edges[3], dim_sizes[3];
    int32 sds1_size=0, sds2_size=0, sds3_size=0, sds4_size=0;
    intn  status = 0;
    int   ii, jj, kk;
    intn  num_errs = 0;    /* number of errors in compression test so far */

    /* Create the file and initialize the SD interface */
    sd_id = SDstart (EXTTST, DFACC_CREATE);
    CHECK(status, FAIL, "SDstart");

    dim_sizes[0] = SD_UNLIMITED;
    dim_sizes[1] = Y_LENGTH;
    dim_sizes[2] = X_LENGTH;

    /* Create and write two unlimited-dimension data sets, SDS2 and SDS3,
       in the main file.  Z_LENGTH is passed for unlimited dimension. */
    sds2_size = make_SDS(sd_id, SDS2, DFNT_INT32, 3, dim_sizes, Z_LENGTH, (VOIDP)written_data);
    sds3_size = make_SDS(sd_id, SDS3, DFNT_INT32, 3, dim_sizes, Z_LENGTH, (VOIDP)written_data);

    /* Close the file to flush */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file */
    sd_id = SDstart (EXTTST, DFACC_RDWR);
    CHECK(status, FAIL, "SDstart");

    /* Start appending data at the end of the unlimited dimension */
    ap_start[0] = Z_LENGTH;
    ap_start[1] = ap_start[2] = 0;
    ap_edges[0] = 1;
    ap_edges[1] = Y_LENGTH;
    ap_edges[2] = X_LENGTH;

    /* Data initialization, a hyperslab 1xY_LENGTHxX_LENGTH */
    for (kk = 0; kk < ap_edges[0]; kk++)
        for (jj = 0; jj < ap_edges[1]; jj++)
            for (ii = 0; ii < ap_edges[2]; ii++)
                ap_data[kk][jj][ii] = (ii + 1) + (jj + 1) + (kk + 1);

    /* Append data to the unlimited-dimension data set SDS2.  This should */
    /* produce a linked-block element, because SDS3 had been written */
    sds2_size = append_Data2SDS(sd_id, SDS2, ap_start, ap_edges, (VOIDP)ap_data);
    CHECK(status, FAIL, "append_Data2SDS");

    /* Select the named data set, id is checked by callee */
    sds2_id = get_SDSbyName(sd_id, SDS2);

    /* Now, move SDS2's data to the external file, then check its size
       This tests the combo: SPECIAL_LINKED and SPECIAL_EXT.  1600 is just a
       random number that is more than enough to go pass the existing data.
       This action verifies the fix in HDFFR-1516.  SDsetexternalfile does not
       fail.  Note that there is no test for the case where calling
       SDsetexternalfile on an external SDS failed, because SDsetexternalfile is
       modified to have no effect if it is called more than once on an SDS, see
       HDFFR-1521. */
    status = SDsetexternalfile (sds2_id, EXTFILE2, 1600);
    CHECK(status, FAIL, "SDsetexternalfile");

    /* Verify data size */
    verify_datasize(sds2_id, sds2_size, SDS2);

    /* Move data of an existing contiguous data set to the external file */

    /* Select the named data set, id is checked by callee */
    sds3_id = get_SDSbyName(sd_id, SDS3);

    /* Move SDS3's data to the external file, then check its size.  This also */
    /* tests moving an existing unlimited-dimension data set to external file */
    status = SDsetexternalfile(sds3_id, EXTFILE2, 2500);  /* random spot */
    CHECK(status, FAIL, "SDsetexternalfile");
    verify_datasize(sds3_id, sds3_size, SDS3);

    /* Attempt to move a compressed data set to an external file; should fail */

    /* Create and write to a compressed data set */
    sds4_size = make_SDS(sd_id, SDS4, DFNT_INT32, 3, dim_sizes, Z_LENGTH, (VOIDP)written_data);

    /* Select the named data set, id is checked by callee */
    sds4_id = get_SDSbyName(sd_id, SDS4);

    /* Try to move SDS4's data to the external file, should fail because HDF4
       doesn't support compressed and external data together */
    status = SDsetexternalfile(sds4_id, EXTFILE2, 4000); /* random spot */
    CHECK(status, FAIL, "SDsetexternalfile");

    /* Check the data length of each data set */
    verify_datasize(sds2_id, sds2_size, SDS2);
    verify_datasize(sds3_id, sds3_size, SDS3);
    verify_datasize(sds4_id, sds4_size, SDS4);

    status = SDendaccess(sds2_id);
    CHECK(status, FAIL, "SDendaccess SDS2");
    status = SDendaccess(sds3_id);
    CHECK(status, FAIL, "SDendaccess SDS3");
    status = SDendaccess(sds4_id);
    CHECK(status, FAIL, "SDendaccess SDS4");

    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Re-open the file to verify written data */
    sd_id = SDstart (EXTTST, DFACC_RDWR);
    CHECK(status, FAIL, "SDstart");

    status = SDfileinfo(sd_id, &num_sds, &num_attrs);
    CHECK(status, FAIL, "SDfileinfo");

    /* Read data of each data sets and verify against the original */
    for (ii = 0; ii < num_sds; ii++)
    {
    verify_data(sd_id, ii);
    }

    /* Close the file */
    status = SDend(sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_special_combos() */

/* Test driver for testing external file functions */
 /* extern int test_external()
 */
int test_external()
{
    int ii, jj, kk;
    intn num_errs = 0;         /* number of errors */

    /* Data initialization */
    for (kk = 0; kk < Z_LENGTH; kk++)
        for (jj = 0; jj < Y_LENGTH; jj++)
            for (ii = 0; ii < X_LENGTH; ii++)
                written_data[kk][jj][ii] = (ii + 1) + (jj + 1) + (kk + 1);

    /* Output message about test being performed */
    TESTING("external file functions (texternal.c)");

    /* Test SDsetexternalfile basic functionality (modified from hdftest.c) */
    num_errs = num_errs + test_setexternal();

    /* Test SDgetexternalfile basic functionality */
    num_errs = num_errs + test_getexternal();

    /* Test that calling SDsetexternalfile repeatedly will not fail (HDFFR-1516)
       -BMR, 10/29/15 */
    num_errs = num_errs + test_mult_setexternal();

    /* Test multiple specialness */
    num_errs = num_errs + test_special_combos();

    if (num_errs == 0) PASSED();
    return num_errs;
}

/*********************** Local utility functions ***********************/

/********************************************************************
   Name: verify_data() - Verifies the written data, given the SDS' index.
   Description:
    Calls SDselect, SDgetinfo, and SDreaddata to verify the sds_ind'th
    data against the original buffer.
   Return value:
        None.
   BMR - Dec 1, 2015
*********************************************************************/
void verify_data(int32 sd_id, int32 sds_ind)
{
    int32 sds_id;
    int32 *ptr;
    char name[80];
    int32 data_size, rank1;
    int32 start[3], edges[3], dims[3];
    intn  status;
    int32 *outdata = NULL, num_elems;
    int32 outd[140];
    intn  num_errs = 0;    /* number of errors in compression test so far */

    /* Select the data set. */
    sds_id = SDselect (sd_id, sds_ind);
    CHECK(sds_id, FAIL, "SDselect");

    /* Set the parameters start and edges to read */
    edges[1] = Y_LENGTH;
    edges[2] = X_LENGTH;
    start[0] = start[1] = start[2] = 0;

    /* Get the name of the data set */
    status = SDgetinfo(sds_id, name, &rank1, dims, NULL, NULL);
    CHECK(status, FAIL, "SDgetinfo");

    /* The data set SDS2 has appended data so the written data is different
    from the rest of the data sets in the file */
    if (!HDstrncmp(name, SDS2, HDstrlen(SDS2)))
    {
    /* Buffer for first written data + appended data */
    int32 data_wappended[Z_LENGTH+1][Y_LENGTH][X_LENGTH];

    /* Number of elements in first written data + appended data */
    num_elems = Z_LENGTH*Y_LENGTH*X_LENGTH + 1*Y_LENGTH*X_LENGTH;

    /* Copy buffer of first written data to data_wappended */
    HDmemcpy(data_wappended, written_data, (Z_LENGTH*Y_LENGTH*X_LENGTH)*sizeof(int));

    /* Forward to the end of first written data */
    ptr = &data_wappended[Z_LENGTH][0][0];

    /* Copy appended data to data_wappended */
    HDmemcpy(ptr, ap_data, (1*Y_LENGTH*X_LENGTH)*sizeof(int));

    /* Back to the beginning of data_wappended */
    ptr = &data_wappended[0][0][0];

    /* Size of data written including appended data */
    data_size = ((Z_LENGTH+1) * Y_LENGTH*X_LENGTH)*sizeof(int);
    edges[0] = Z_LENGTH + 1;

    } /* with appended data */

    /* Everyone else */
    else
    {
    /* Point to written data buffer */
        ptr = &written_data[0][0][0];

    /* Number of elements */
    num_elems = Z_LENGTH*Y_LENGTH*X_LENGTH;

    /* Size of data written */
    data_size = num_elems * sizeof(int);
    edges[0] = Z_LENGTH;
    }

    /* Allocate buffer for reading, after establishing the data size */
    outdata = (int32 *) HDmalloc(data_size);

    /* Read the entire sds and verify that the data is as the original buffer */
    status = SDreaddata(sds_id, start, NULL, edges, (VOIDP) outdata);
    CHECK(status, FAIL, "SDreaddata");

    /* Verify that data is correct comparing against the written data */
    {
    int ii;
    int32* out;
    out = &outdata[0];

    for (ii = 0; ii < num_elems; ii++, ptr++, out++)
    if (*ptr != *out)
    {
        fprintf(stderr, "Data read (%d) is different than written (%d) for SDS #%d, name = %s\n", *out, *ptr, sds_ind, name);
    }
    }

    /* Terminate access to the data set, SD interface, and file. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

} /* verify_data */
#endif /* HDF */

