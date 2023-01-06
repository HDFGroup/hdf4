/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
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
 * tdatasizes.c - tests the API SDgetdatasize.
 * Structure of the file:
 *    test_datasizes - test driver
 *	  test_nonspecial_SDSs - tests nonspecial SDSs
 *	  test_compressed_SDSs - tests compressed SDSs without closing file
 *	  test_empty_SDSs      - tests on empty chunked and chunked/comp SDSs
 *	  test_chunked_partial - tests on chunked and partially written SDS
 *	  test_chkcmp_SDSs     - tests chunked/compressed SDSs
 *	  test_extend_SDSs     - tests SDSs with unlimited dimensions
 *    check_datasizes - utility routine that calls SDgetdatasize and verifies 
 *		the retrieved data sizes.
 * NOTE: At this time, SDgetdatasize will not return the correct compressed
 *	size unless SDreaddata or SDendaccess is called between SDwritedata
 *	and SDgetdatasize.
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define FILE_NAME     "datasizes.hdf"	/* data file to test empty SDSs */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

/* Utility routine that selects that named SDS, then calls SDgetdatasize
 * and verifies the returned values. */
static void check_datasizes(
		int32 fid, 		/* file id */
		char* sds_name, 	/* name of the inquired SDS */
		int32 comp_size_check, 	/* expected compressed data size */
		int32 uncomp_size_check,/* expected non-compressed data size */
		int* ret_num_errs	/* current number of errors */ )
{
    int32 sds_id, sds_index;
    intn status;
    int num_errs = 0;
    char mesg[80];
    int32 uncomp_size=0, comp_size=0;

    /* Get index of dataset using its name */
    sds_index = SDnametoindex(fid, sds_name);
    sprintf(mesg, "check_datasizes: SDnametoindex (%s)", sds_name);
    CHECK(sds_index, FAIL, mesg);

    /* Select the dataset */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "check_datasizes: SDselect");

    /* The retrieved values from SDgetdatasize are verified against 
       'comp_size_check' and 'uncomp_size_check' */
    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    sprintf(mesg, "SDgetdatasize: SDS named (%s)", sds_name);
    CHECK(status, FAIL, mesg);
    VERIFY(comp_size, comp_size_check, mesg);
    VERIFY(uncomp_size, uncomp_size_check, mesg);

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "check_datasizes: SDendaccess");

    *ret_num_errs = num_errs;
}   /* check_datasizes */

/* Test non-special SDSs.  This routine creates non-special SDSs, writes
 * data to one of the SDSs, and checks the sizes returned by SDgetdatasize
 */
static intn test_nonspecial_SDSs(int32 fid)
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
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate 'EmptyDataset'");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Check that this SDS is empty */
    check_datasizes(fid, "EmptyDataset", 0, 0, &num_errs);

    /* Create another 2x2 dataset called "WrittenDataset" */
    sds_id = SDcreate(fid, "WrittenDataset", DFNT_INT32, 2, dimsize);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate 'WrittenDataset'");

    /* Define the location and size of the data to be written to the dataset */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Check the size of the data of this SDS */
    check_datasizes(fid, "WrittenDataset", Y_LENGTH*X_LENGTH*SIZE_INT32, Y_LENGTH*X_LENGTH*SIZE_INT32, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_nonspecial_SDSs */

/* Test compressed SDSs.  This routine creates "Compressed-No-Data" and creates
 * and writes to "CompressedData" and "Non-CompressedData" SDSs.  It will then
 * check the sizes returned from SDgetdatasize calls.
 */
static intn test_compressed_SDSs(int32 fid)
{
    int32     sds_id, esds_id, usds_id;
    int32     start[2], edges[2], dim_sizes[2];
    comp_coder_t comp_type;    /* Compression flag */
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
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate 'CompressedData'");

    esds_id = SDcreate(fid, "Compressed-No-Data", DFNT_INT32, RANK, dim_sizes);
    CHECK(esds_id, FAIL, "test_compressed_SDSs: SDcreate 'Compressed-No-Data'");

    usds_id = SDcreate(fid, "Non-CompressedData", DFNT_INT32, RANK, dim_sizes);
    CHECK(usds_id, FAIL, "test_compressed_SDSs: SDcreate 'Non-CompressedData'");

    comp_type = COMP_CODE_DEFLATE;
    c_info.deflate.level = 6;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress 'CompressedData'");

    status = SDsetcompress(esds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress 'Compressed-No-Data'");

    /* Define the location and size of the dataset to be written to the file */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the stored data to the compressed dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata");
 
    /* Write the stored data to the dataset non-compressed dataset */
    status = SDwritedata(usds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata");
 
     /* Close the SDSs */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'CompressedData'");

    status = SDendaccess(esds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'Compressed-No-Data'");

    status = SDendaccess(usds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess 'Non-CompressedData'");

    /* Check the size of the data of this SDS - 59 is from examining previously */
    check_datasizes(fid, "CompressedData", 59, Y_LENGTH*X_LENGTH*SIZE_INT32, &num_errs);

    /* Check the size of the data of this SDS */
    check_datasizes(fid, "Compressed-No-Data", 0, 0, &num_errs);

    /* Check the size of the data of this SDS */
    check_datasizes(fid, "Non-CompressedData", Y_LENGTH*X_LENGTH*SIZE_INT32, Y_LENGTH*X_LENGTH*SIZE_INT32, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_compressed_SDSs */

/* Test with empty SDSs.  This routine creates a "Chunked Empty" SDS and a
 * "Chunked Compressed Empty" SDS then uses SDgetdatasize to verify that they
 * don't have data.
 */

#define X_LENGTH2  4
#define Y_LENGTH2  9
#define CHK_X      3
#define CHK_Y      2

static intn test_empty_SDSs(int32 fid)
{
    int32         ch_sds_id, chcp_sds_id;
    int32         dim_sizes[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         flag;
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create Y_LENGTH2 x X_LENGTH2 empty chunked and chunked/compressed SDSs */
    dim_sizes[0] = Y_LENGTH2;
    dim_sizes[1] = X_LENGTH2;
    ch_sds_id = SDcreate(fid, "Chunked Empty", DFNT_INT16, RANK, dim_sizes);
    CHECK(ch_sds_id, FAIL, "test_empty_SDSs: SDcreate 'Chunked Empty'");

    chcp_sds_id = SDcreate(fid, "Chunked Compressed Empty", DFNT_INT16, RANK, dim_sizes);
    CHECK(chcp_sds_id, FAIL, "test_empty_SDSs: SDcreate 'Chunked Compressed Empty'");

    /* Set info for chunking */
    flag = HDF_CHUNK;
    status = SDsetchunk(ch_sds_id, c_def, flag);
    CHECK(status, FAIL, "test_empty_SDSs: SDsetchunk");

    /* Set info for chunking and compression */
    flag = HDF_CHUNK | HDF_COMP;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 6;
    status = SDsetchunk(chcp_sds_id, c_def, flag);
    CHECK(status, FAIL, "test_empty_SDSs: SDsetchunk");

    /* Terminate access to the datasets */
    status = SDendaccess(ch_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess 'Chunked Empty'");
    status = SDendaccess(chcp_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess 'Chunked Compressed Empty'");

    /* Check the size of the data of 'Chunked Empty' */
    check_datasizes(fid, "Chunked Empty", 0, 0, &num_errs);

    /* Check the size of the data of 'Chunked Compressed Empty' */
    check_datasizes(fid, "Chunked Compressed Empty", 0, 0, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_empty_SDSs */

/* Test with chunked and partially written SDS.  This routine creates a 
 * "Chunked Not Empty" SDS and writes some chunks but not all to it.  It will
 * then call SDgetdatasize to verify the sizes.
 */
static intn test_chunked_partial(int32 fid)
{
    int32         sds_id, sds_index;
    int32         dim_sizes[RANK], origin[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         flag; /* Chunking flag */
    int16         fill_value = 0;   /* Fill value */
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int16 chunk1[CHK_X][CHK_Y] = { {1, 1},
                           {1, 1},
                           {1, 1} }; 

    int16 chunk3[CHK_X][CHK_Y] = { {3, 3},
                           {3, 3},
                           {3, 3} }; 

    /* Initialize chunk size */
    HDmemset(&c_def, 0, sizeof(c_def)) ;
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create Y_LENGTH2 x X_LENGTH2 SDS */
    dim_sizes[0] = X_LENGTH2;
    dim_sizes[1] = Y_LENGTH2;
    sds_id = SDcreate(fid, "Chunked Not Empty", DFNT_INT16, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDcreate 'Chunked Not Empty'");

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue(sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "test_chunked_partial: SDsetfillvalue");

    /* Set info for chunking */
    flag = HDF_CHUNK;
    status = SDsetchunk(sds_id, c_def, flag);
    CHECK(status, FAIL, "test_chunked_partial: SDsetchunk");

    /* Write partially to 'Chunked Not Empty' and check the sizes */

    /* Write the chunk with the coordinates (0,0) */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chunked_partial: SDwritechunk");

    /* Write the chunk with the coordinates (1,0) */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chunked_partial: SDwritechunk");

    /* Terminate access to the "Chunked Not Empty" dataset */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess 'Chunked Not Empty'");

    /* Check the size of the data of this SDS - only chunked, not compressed,
       so both values should be the same; there are two chunks of size
       CHK_X*CHK_Y in type int16 written */
    check_datasizes(fid, "Chunked Not Empty", CHK_X*CHK_Y*SIZE_INT16*2, CHK_X*CHK_Y*SIZE_INT16*2, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chunked_partial */

/* Test with chunked and compressed SDS.  This routine creates 
 * "ChunkedDeflateData" and "ChunkedNoDeflateData" SDSs and writes the same
 * data to both.  It will then use SDgetdatasize to verify the sizes.
 */
static intn test_chkcmp_SDSs(int32 fid)
{
    int32         sds_id, sds_index;
    int32         cmpsds_id, cmpsds_index;
    int32         flag, maxcache, new_maxcache;
    int32         dim_sizes[RANK], origin[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         fill_value = 0;   /* Fill value */
    int32	  comp_size1=0, uncomp_size1=0;
    int32	  comp_size2=0, uncomp_size2=0;
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int16 chunk1[CHK_X][CHK_Y] = { {1, 1},
                           {1, 1},
                           {1, 1} }; 

    int16 chunk3[CHK_X][CHK_Y] = { {3, 3},
                           {3, 3},
                           {3, 3} }; 

    int32 chunk2[CHK_X][CHK_Y] = { {2, 2},
                           {2, 2},
                           {2, 2} }; 

    /* Initialize chunk size */
    HDmemset(&c_def, 0, sizeof(c_def)) ;
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create Y_LENGTH2 x X_LENGTH2 SDS */
    dim_sizes[0] = Y_LENGTH2;
    dim_sizes[1] = X_LENGTH2;
    cmpsds_id = SDcreate(fid, "ChunkedDeflateData", DFNT_INT32, RANK, dim_sizes);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDcreate 'ChunkedDeflateData'");

    sds_id = SDcreate(fid, "ChunkedNoDeflateData", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDcreate 'ChunkedNoDeflateData'");

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue(cmpsds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue 'ChunkedDeflateData'");

    status = SDsetfillvalue(sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue 'ChunkedNoDeflateData'");

    /* Set info for chunking and compression */
    flag = HDF_CHUNK | HDF_COMP;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 6;
    status = SDsetchunk(cmpsds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk 'ChunkedDeflateData'");

    /* Set info for chunking and compression */
    HDmemset(&c_def, 0, sizeof(c_def)) ;
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    flag = HDF_CHUNK;
    status = SDsetchunk(sds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk 'ChunkedNoDeflateData'");

    /* Set chunk cache to hold maximum of 3 chunks */
    maxcache = 3;
    flag = 0;
    new_maxcache = SDsetchunkcache(cmpsds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache 'ChunkedDeflateData'");

    new_maxcache = SDsetchunkcache(sds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache 'ChunkedNoDeflateData'");

    /* Terminate access to the dataset before writing data to it. */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedDeflateData'");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedNoDeflateData'");

    /* Check that this SDS is still empty after the call to SDsetchunk */
    check_datasizes(fid, "ChunkedDeflateData", 0, 0, &num_errs);
    check_datasizes(fid, "ChunkedNoDeflateData", 0, 0, &num_errs);

    /* Re-select the datasets, write chunks using SDwritechunk function, then
       check their data sizes */

    /* Get index of dataset using its name */
    cmpsds_index = SDnametoindex(fid, "ChunkedDeflateData");
    CHECK(cmpsds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex 'ChunkedDeflateData'");

    sds_index = SDnametoindex(fid, "ChunkedNoDeflateData");
    CHECK(sds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex 'ChunkedNoDeflateData'");

    /* Select the datasets for access */
    cmpsds_id = SDselect(fid, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect 'ChunkedDeflateData'");
    sds_id = SDselect(fid, sds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect 'ChunkedNoDeflateData'");

    /* Write the chunk with the coordinates (0,0) */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedDeflateData'");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedNoDeflateData'");

    /* Write the chunk with the coordinates (1,0) */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedDeflateData'");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedNoDeflateData'");

    /* Write the chunk with the coordinates (0,1) */
    origin[0] = 0;
    origin[1] = 1;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedDeflateData'");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk 'ChunkedNoDeflateData'");

    /* Terminate access to the datasets */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedDeflateData'");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedNoDeflateData'");

    /* Verify the compressed and non-compressed data sizes of the datasets */

    /* Open dataset 'ChunkedDeflateData' */
    cmpsds_id = SDselect(fid, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect 'ChunkedDeflateData'");

    /* Get the data sizes */
    status = SDgetdatasize(cmpsds_id, &comp_size1, &uncomp_size1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatasize 'ChunkedDeflateData'");

    /* Open dataset 'ChunkedNoDeflateData' */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDselect 'ChunkedNoDeflateData'");

    /* Get the data sizes */
    status = SDgetdatasize(sds_id, &comp_size2, &uncomp_size2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatasize 'ChunkedNoDeflateData'");

    /* Non-compressed data sizes of the two datasets should be the same */
    VERIFY(uncomp_size1, uncomp_size2, "test_chkcmp_SDSs: non-compressed data sizes might be incorrect");

    /* In this test, compressed data size should be smaller than non-compressed
       data size */
    if (comp_size1 >= uncomp_size1)
    {
	printf("*** Routine test_chkcmp_SDSs: FAILED at line %d ***\n", __LINE__);
	printf("    In this test, compressed data size (%d) should be smaller than non-compressed data size (%d)\n", comp_size1, uncomp_size1);
	num_errs++;
    }

    /* Terminate access to the data sets. */
    status = SDendaccess(sds_id);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedNoDeflateData'");

    status = SDendaccess(cmpsds_id);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDendaccess 'ChunkedDeflateData'");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chkcmp_SDSs */

/* Test SDSs with unlimited dimensions.  This routine creates SDSs with
   unlimited dimensions, writes data to it, and checks the sizes returned 
   by SDgetdatasize
 */
static intn test_extend_SDSs(int32 fid)
{
    int32 sds_id, sds_index;
    int32 dimsize[2], start[2], edges[2];
    int32 dimsize1[1], start1[1], edges1[1];
    int32 data[Y_LENGTH][X_LENGTH];
    float fdata[Y_LENGTH];
    int32 output[Y_LENGTH][X_LENGTH];
    intn  status;
    int   i, j;
    int   num_errs = 0;		/* number of errors so far */

    /* Initialize data for the dataset */
    for (j = 0; j < Y_LENGTH; j++) {
        for (i = 0; i < X_LENGTH; i++)
            data[j][i] = (i + j) + 1;
    }

    /* Create a 2x2 dataset called "EmptyDataset" */
    dimsize[0] = SD_UNLIMITED;
    dimsize[1] = X_LENGTH;
    sds_id = SDcreate(fid, "AppendableDataset 1", DFNT_INT32, 2, dimsize);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate 'AppendableDataset 1'");

    /* Write the stored data to the dataset */
    start[0] = start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDwritedata");

    /* Check data. */
    HDmemset(&output, 0, sizeof(output));
    status = SDreaddata(sds_id, start, NULL, edges, (VOIDP)output);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDreaddata");
    /* Initialize data for the dataset */
    for (j = 0; j < Y_LENGTH; j++)
        for (i = 0; i < X_LENGTH; i++)
	    if (output[j][i] != data[j][i])
		fprintf(stderr, "Read value (%d) differs from written (%d) at [%d,%d]\n", output[j][i], data[j][i], j, i);

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Check that this SDS is empty */
    check_datasizes(fid, "AppendableDataset 1", Y_LENGTH*X_LENGTH*SIZE_INT32, Y_LENGTH*X_LENGTH*SIZE_INT32, &num_errs);

    /* Create another dataset with 1 unlimited dimension */
    sds_id = SDcreate(fid, "AppendableDataset 2", DFNT_FLOAT64, 1, dimsize);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate 'AppendableDataset 2'");

    /* Define the location and size of the data to be written to the dataset */
    start1[0] = 0;
    edges1[0] = Y_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start1, NULL, edges1, (VOIDP)fdata);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Check the size of the data of this SDS */
    check_datasizes(fid, "AppendableDataset 2", Y_LENGTH*SIZE_FLOAT64, Y_LENGTH*SIZE_FLOAT64, &num_errs);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_extend_SDSs */

/* Test driver for testing the API SDgetdatasize. */
extern int test_datasizes()
{
    int32 fid;
    intn status;
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("getting data size of special data (tdatasizes.c)");

    /* Open the file and initialize the SD interface */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "test_datasizes: SDstart");

    /* Test nonspecial SDSs */
    num_errs = num_errs + test_nonspecial_SDSs(fid);
    /* Test compressed SDSs */
    num_errs = num_errs + test_compressed_SDSs(fid);
    /* Test chunked empty SDSs */
    num_errs = num_errs + test_empty_SDSs(fid);
    /* Test chunked_partial SDSs */
    num_errs = num_errs + test_chunked_partial(fid);
    /* Test chunked SDSs */
    num_errs = num_errs + test_chkcmp_SDSs(fid);
    /* Test extendable SDSs */
    num_errs = num_errs + test_extend_SDSs(fid);

    /* Close the file */
    status = SDend(fid);
    CHECK(status, FAIL, "test_datasizes: SDend");

    if (num_errs == 0) PASSED();
    return num_errs;
}

#endif /* HDF */
