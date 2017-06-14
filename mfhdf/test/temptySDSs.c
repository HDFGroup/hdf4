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
 * temptySDS.c - tests the API SDcheckempty.
 * Structure of the file:
 *    test_checkempty - test driver
 *	  test_nonspecial_SDSs - tests nonspecial SDSs without closing file
 *	  test_compressed_SDSs - tests compressed SDSs without closing file
 *	  test_chunked_SDSs    - tests chunked SDSs without closing file
 *	  test_unlimited_SDSs  - tests unlimited dim SDSs without closing file
 *	  test_with_existing_file - tests all SDSs in file after closing the
 *	  			 file and reopening it
 *    check_empty_SDS - utility routine that calls SDcheckempty and verifies
 *		values
 *    check_getchunkinfo - utility routine that calls SDgetchunkinfo and
 *		verifies that function did not fail on empty SDS and the file
 *		is opened as read-only. (bug HDFFR-171, -BMR 2011/10/23)
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define FILE_NAME     "emptySDSs.hdf"	/* data file to test empty SDSs */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

/* Utility routine that selects that named SDS, then calls SDcheckempty
 * and verifies the returned values. */
static void check_empty_SDS(
		int32 fid, 		/* file id */
		char* sds_name, 	/* name of the inquired SDS */
		int32 verify_value, 	/* expected value of 'emptySDS' from SDcheckempty */
		int* ret_num_errs	/* current number of errors */ )
{
    int32 sds_id, sds_index, status_32;
    intn status, emptySDS;
    int num_errs = 0;
    char mesg[80];

    /* Get index of dataset using its name */
    sds_index = SDnametoindex(fid, sds_name);
    sprintf(mesg, "In check_empty_SDS: SDnametoindex (%s)", sds_name);
    CHECK(sds_index, FAIL, mesg);

    /* Select the dataset */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "In check_empty_SDS: SDselect");

    /* The returned value from SDcheckempty is CHECKed for FAIL and
       'emptySDS' is verified to be verify_value, which can be TRUE or FALSE. */
    status_32 = SDcheckempty( sds_id, &emptySDS);
    CHECK(status_32, FAIL, "SDcheckempty");
    VERIFY(emptySDS, verify_value, "SDcheckempty");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In check_empty_SDS: SDendaccess");

    *ret_num_errs = num_errs;
}   /* check_empty_SDS */

/* Utility routine that selects that named SDS, then calls SDgetchunkinfo and
 * verify the returned values.  The routine also verifies that SDgetchunkinfo
 * did not fail when the file is opened as read-only (bug HDFFR-171) */
static void check_getchunkinfo(
		int32 fid, 		/* file id */
		char* sds_name, 	/* name of the inquired SDS */
		int32 verify_value, 	/* expected value of 'flags' from SDgetchunkinfo */
		int* ret_num_errs	/* current number of errors */ )
{
    int32 sds_id, sds_index;
    HDF_CHUNK_DEF c_def_out; /* Chunking definitions */
    int32 comp_flag, c_flags;
    intn status;
    int num_errs = 0;
    char mesg[80];

    /* Get index of dataset using its name */
    sds_index = SDnametoindex(fid, sds_name);
    sprintf(mesg, "In check_getchunkinfo: SDnametoindex (%s)", sds_name);
    CHECK(sds_index, FAIL, mesg);

    /* Select the dataset */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "In check_getchunkinfo: SDselect");

    /* The returned value from SDgetchunkinfo is CHECKed for FAIL and
       'c_flags' is VERIFied to be verify_value, which can be HDF_NONE,
       HDF_CHUNK, or HDF_CHUNK | HDF_COMP. */
    status = SDgetchunkinfo (sds_id, &c_def_out, &c_flags);
    CHECK(status, FAIL, "SDgetchunkinfo");
    VERIFY(c_flags, verify_value, "SDgetchunkinfo");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In check_getchunkinfo: SDendaccess");

    *ret_num_errs = num_errs;
}   /* check_getchunkinfo */

/* Test non-special SDSs.  This routine creates non-special SDSs, writes
 * data to one of the SDSs, and checks for emptiness on each without closing
 * the file. */
static intn 
test_nonspecial_SDSs(int32 fid)
{
    int32 sds_id;
    int32 dimsize[2], start[2], edges[2];
    int32 data[Y_LENGTH][X_LENGTH];
    intn  status;
    int   i, j;
    int   num_errs = 0;		/* number of errors so far */

    /* Initialize data for the dataset */
    for (j = 0; j < Y_LENGTH; j++) {
        for (i = 0; i < X_LENGTH; i++)
            data[j][i] = (i + j) + 1;
    }

    /* Create a 2x2 dataset called "EmptyDataset" */
    dimsize[0] = Y_LENGTH;
    dimsize[1] = X_LENGTH;
    sds_id = SDcreate(fid, "EmptyDataset", DFNT_FLOAT32, 2, dimsize);
    CHECK(sds_id, FAIL, "In test_nonspecial_SDSs: SDcreate 'EmptyDataset'");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_nonspecial_SDSs: SDendaccess");

    /* Check that this SDS is empty */
    check_empty_SDS(fid, "EmptyDataset", TRUE, &num_errs);

    /* Create another 2x2 dataset called "WrittenDataset" */
    sds_id = SDcreate(fid, "WrittenDataset", DFNT_INT32, 2, dimsize);
    CHECK(sds_id, FAIL, "In test_nonspecial_SDSs: SDcreate 'WrittenDataset'");

    /* Define the location and size of the data to be written to the dataset */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(sds_id, FAIL, "In test_nonspecial_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_nonspecial_SDSs: SDendaccess");

    /* Check that this SDS is NOT empty */
    check_empty_SDS(fid, "WrittenDataset", FALSE, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_nonspecial_SDSs */

/* Test compressed SDSs.  This routine creates compressed SDSs, writes
 * data to one of the SDSs, and checks for emptiness on each without closing
 * the file. */
static intn
test_compressed_SDSs(int32 fid)
{
    int32     sds_id, esds_id;
    int32     start[2], edges[2], dim_sizes[2];
    int32     comp_type;    /* Compression flag */
    comp_info c_info;	   /* Compression structure */
    int32     data[Y_LENGTH][X_LENGTH];
    intn      status;
    int       i, j;
    int   num_errs = 0;   /* number of errors so far */

    /* Buffer array data and define array dimensions */
    for (j = 0; j < Y_LENGTH; j++)
    {
        for (i = 0; i < X_LENGTH; i++)
                data[j][i] = (i + j) + 1;
    }
    dim_sizes[0] = Y_LENGTH;
    dim_sizes[1] = X_LENGTH;

    /* Create datasets, one to be written with data, the other not */
    sds_id = SDcreate(fid, "CompressedData", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "In test_compressed_SDSs: SDcreate 'CompressedData'");

    esds_id = SDcreate(fid, "Compressed-No-Data", DFNT_INT32, RANK, dim_sizes);
    CHECK(esds_id, FAIL, "In test_compressed_SDSs: SDcreate 'Compressed-No-Data'");

    comp_type = COMP_CODE_DEFLATE;
    c_info.deflate.level = 6;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "In test_compressed_SDSs: SDsetcompress 'CompressedData'");

    status = SDsetcompress(esds_id, comp_type, &c_info);
    CHECK(status, FAIL, "In test_compressed_SDSs: SDsetcompress 'Compressed-No-Data'");

    /* Define the location and size of the dataset to be written to the file */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "In test_compressed_SDSs: SDwritedata");
 
     /* Close the SDSs */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_compressed_SDSs: SDendaccess 'CompressedData'");

    status = SDendaccess(esds_id);
    CHECK(status, FAIL, "In test_compressed_SDSs: SDendaccess 'Compressed-No-Data'");

    /* Check that this SDS is NOT empty */
    check_empty_SDS(fid, "CompressedData", FALSE, &num_errs);

    /* Check that this SDS is empty */
    check_empty_SDS(fid, "Compressed-No-Data", TRUE, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_compressed_SDSs */

/* Test chunked SDSs.  This routine creates chunked SDSs, writes data
 * to one of the SDSs, and checks for emptiness on each without closing
 * the file. */

#define X_CHUNKED_LENGTH      4
#define Y_CHUNKED_LENGTH      9

static intn
test_chunked_SDSs(int32 fid)
{
    int32         sds_id, esds_id, sds_index;
    int32         flag, maxcache, new_maxcache;
    int32         dim_sizes[RANK], origin[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         comp_flag;
    int16         fill_value = 0;   /* Fill value */
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int16 chunk1[3][2] = { 1, 1,
                           1, 1,
                           1, 1 }; 

    int16 chunk2[3][2] = { 2, 2,
                           2, 2,
                           2, 2 }; 

    int16 chunk3[3][2] = { 3, 3,
                           3, 3,
                           3, 3 }; 

    int16 chunk6[3][2] = { 6, 6,
                           6, 6,
                           6, 6 };


    c_def.chunk_lengths[0] = 3;
    c_def.chunk_lengths[1] = 2;

    /* Create Y_CHUNKED_LENGTH x X_CHUNKED_LENGTH SDS */
    dim_sizes[0] = Y_CHUNKED_LENGTH;
    dim_sizes[1] = X_CHUNKED_LENGTH;
    esds_id = SDcreate(fid, "Chunked-No-Data", DFNT_INT16, RANK, dim_sizes);
    CHECK(esds_id, FAIL, "In test_chunked_SDSs: SDcreate 'Chunked-No-Data'");

    /* Set info for chunking */
    comp_flag = HDF_CHUNK;
    status = SDsetchunk(esds_id, c_def, comp_flag);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDsetchunk");

    /* Terminate access to the "Chunked-No-Data" dataset */
    status = SDendaccess(esds_id);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDendaccess 'Chunked-No-Data'");

    /* Check that this SDS is empty */
    check_empty_SDS(fid, "Chunked-No-Data", TRUE, &num_errs);

    /* Create another Y_CHUNKED_LENGTH x X_CHUNKED_LENGTH SDS but this 
       one will be written with chunks */
    sds_id = SDcreate(fid, "ChunkedData", DFNT_INT16, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "In test_chunked_SDSs: SDcreate 'ChunkedData'");

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue(sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDsetfillvalue");

    /* Set info for chunking */
    comp_flag = HDF_CHUNK;
    status = SDsetchunk(sds_id, c_def, comp_flag);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDsetchunk");

    /* Set chunk cache to hold maximum of 3 chunks */
    maxcache = 3;
    flag = 0;
    new_maxcache = SDsetchunkcache(sds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "In test_chunked_SDSs: SDsetchunkcache");

    /* Terminate access to the dataset then check if it's empty - and it 
       should be, before writing data to it. */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDendaccess");

    /* Check that this SDS is still empty after the call to SDsetchunk */
    check_empty_SDS(fid, "ChunkedData", TRUE, &num_errs);

    /* Re-select the "ChunkedData" SDS, then write chunks using SDwritechunk 
       function */

    /* Get index of dataset using its name */
    sds_index = SDnametoindex(fid, "ChunkedData");
    CHECK(sds_index, FAIL, "In test_chunked_SDSs: SDnametoindex");

    /* Select the dataset */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "In test_chunked_SDSs: SDselect");

    /* Write the chunk with the coordinates (0,0) */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (1,0) */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (0,1) */
    origin[0] = 0;
    origin[1] = 1;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDwritechunk");

    /* Terminate access to the dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_chunked_SDSs: SDendaccess");

    /* Check that this SDS is NOT empty */
    check_empty_SDS(fid, "ChunkedData", FALSE, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chunked_SDSs */

/* Test unlimited dimension SDSs.  This routine creates unlimited 
 * dimension SDSs, writes data to one of the SDSs, and checks for 
 * emptiness on each without closing the file. */
static intn
test_unlimited_SDSs(int32 fid)
{
    int32 sds_id, esds_id, sds_index;
    int32 dim_sizes[2];
    int32 data[Y_LENGTH][X_LENGTH], append_data[X_LENGTH];
    int32 start[2], edges[2];
    intn  status;
    int   i, j;
    int   num_errs = 0;   /* number of errors so far */

    /* Data initialization */
    for (j = 0; j < Y_LENGTH; j++) 
    {
        for (i = 0; i < X_LENGTH; i++)
           data[j][i] = (i + 1) + (j + 1);
    }

    /* Define dimensions of the array. Make the first dimension 
       appendable by defining its length to be unlimited */
    dim_sizes[0] = SD_UNLIMITED;
    dim_sizes[1] = X_LENGTH;

    /* Create the array datasets */
    esds_id = SDcreate(fid, "Appendable-No-Data", DFNT_INT32, RANK, dim_sizes);
    CHECK(esds_id, FAIL, "In test_unlimited_SDSs: SDcreate 'Appendable-No-Data'");

    sds_id = SDcreate(fid, "AppendableData", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "In test_unlimited_SDSs: SDcreate 'AppendableData'");

    /* Terminate access to the dataset "Appendable-No-Data" */
    status = SDendaccess(esds_id);

    /* Define the location and the size of the data to be written 
       to the second dataset  */
    start[0] = start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the data */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "In test_unlimited_SDSs: SDwritedata");

    /* Terminate access to the unlimited dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_unlimited_SDSs: SDendaccess");

    /* Check that this SDS is NOT empty */
    check_empty_SDS(fid, "AppendableData", FALSE, &num_errs);

    /* Store the array values to be appended to the dataset */
    for (i = 0; i < X_LENGTH; i++)
        append_data[i] = 1000 + i;

    /* Get index of the dataset using its name */
    sds_index = SDnametoindex(fid, "AppendableData");
    CHECK(sds_index, FAIL, "In test_unlimited_SDSs: SDnametoindex");

    /* Select the "AppendableData" dataset */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "In test_unlimited_SDSs: SDselect 'AppendableData'");

    /* Check if selected SDS is unlimited. If it is not, then terminate 
       access to the SDS */
    if (SDisrecord(sds_id)) 
    {
        /* Define the location of the append to start at the first column 
         * of the 11th row of the dataset and to stop at the end of the
         * eleventh row.  */
        start[0] = Y_LENGTH;
        start[1] = 0;
        edges[0] = 1;
        edges[1] = X_LENGTH;

        /* Append data to the dataset */
        status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)append_data);
        CHECK(status, FAIL, "In test_unlimited_SDSs: SDwritedata");
    }

    /* Terminate access to the dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "In test_unlimited_SDSs: SDendaccess");

    /* Check that this SDS is NOT empty */
    check_empty_SDS(fid, "AppendableData", FALSE, &num_errs);

    /* Check that this SDS is empty */
    check_empty_SDS(fid, "Appendable-No-Data", TRUE, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_unlimited_SDSs */

/* This function checks which of the SDSs in the file are empty/written,
 * just as in the previous individual tests.  The difference is these
 * SDSs are read back in after the file is closed and reopened. */
static intn 
test_with_existing_file()
{
    int32 fid;
    intn status;
    int   num_errs = 0;		/* number of errors so far */

    /* Open the file and initialize the SD interface */
    fid = SDstart(FILE_NAME, DFACC_READ);
    CHECK(fid, FAIL, "In test_with_existing_file: SDstart");

    /* Check that SDS named "EmptyDataset" is empty */
    check_empty_SDS(fid, "EmptyDataset", TRUE, &num_errs);

    /* Check that SDS named "AppendableData" is NOT empty */
    check_empty_SDS(fid, "AppendableData", FALSE, &num_errs);

    /* Check that SDS named "Appendable-No-Data" is empty */
    check_empty_SDS(fid, "Appendable-No-Data", TRUE, &num_errs);

    /* Check that SDS named "CompressedData" is NOT empty */
    check_empty_SDS(fid, "CompressedData", FALSE, &num_errs);

    /* Check that SDS named "Compressed-No-Data" is empty */
    check_empty_SDS(fid, "Compressed-No-Data", TRUE, &num_errs);

    /* Check that SDS named "ChunkedData" is NOT empty */
    check_empty_SDS(fid, "ChunkedData", FALSE, &num_errs);

    /* Check that SDS named "Chunked-No-Data" is empty */
    check_empty_SDS(fid, "Chunked-No-Data", TRUE, &num_errs);

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "In test_with_existing_file: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_with_existing_file */

/* This function verifies that the bug HDFFR-171 is fixed.  It opens
   the file as read-only, then calls SDgetchunkinfo on some empty SDS's.
   SDgetchunkinfo should not fail and should return HDF_NONE for the flag.
   -BMR 2011/10/20. */
static intn 
test_getchunkinfo()
{
    int32 fid;
    intn status;
    int num_errs = 0;		/* number of errors so far */

    /* Open the file and initialize the SD interface */
    fid = SDstart(FILE_NAME, DFACC_RDONLY);
    CHECK(fid, FAIL, "In check_getchunkinfo: SDstart");

    /* Check that SDS named "EmptyDataset" is empty */
    check_getchunkinfo(fid, "EmptyDataset", HDF_NONE, &num_errs);

    /* Check that SDS named "AppendableData" is NOT empty */
    check_getchunkinfo(fid, "AppendableData", HDF_NONE, &num_errs);

    /* Check that SDS named "Appendable-No-Data" is empty */
    check_getchunkinfo(fid, "Appendable-No-Data", HDF_NONE, &num_errs);

    /* Check that SDS named "CompressedData" is NOT empty */
    check_getchunkinfo(fid, "CompressedData", HDF_NONE, &num_errs);

    /* Check that SDS named "Compressed-No-Data" is empty */
    check_getchunkinfo(fid, "Compressed-No-Data", HDF_NONE, &num_errs);

    /* Check that SDS named "ChunkedData" is NOT empty */
    check_getchunkinfo(fid, "ChunkedData", HDF_CHUNK, &num_errs);

    /* Check that SDS named "Chunked-No-Data" is empty */
    check_getchunkinfo(fid, "Chunked-No-Data", HDF_CHUNK, &num_errs);

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "In check_getchunkinfo: SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_getchunkinfo */

/* Test drive for testing the API SDcheckempty. */
extern int
test_checkempty()
{
    int32 fid;
    intn status;
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("functions involving empty data sets (temptySDSs.c)");

    /* Open the file and initialize the SD interface */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "In test_checkempty: SDstart");

    num_errs = num_errs + test_nonspecial_SDSs(fid);
    num_errs = num_errs + test_compressed_SDSs(fid);
    num_errs = num_errs + test_chunked_SDSs(fid);
    num_errs = num_errs + test_unlimited_SDSs(fid);

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "In test_checkempty: SDend");

    /* This function will reopen the file and check the SDSs in it */
    num_errs = num_errs + test_with_existing_file();

    /* This function will reopen the file as read-only to test SDgetchunkinfo
       on empty SDS's */
    num_errs = num_errs + test_getchunkinfo();

    if (num_errs == 0) PASSED();
    return num_errs;
}

#endif /* HDF */
