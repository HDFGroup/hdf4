/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * tdatainfo.c - tests the functions SDgetdatainfo and SDgetdatainfo_count.
 * Structure of the file:
 *    test_datainfo - test driver
 *	  test_nonspecial_SDSs - tests nonspecial SDSs
 *	  test_compressed_SDSs - tests compressed SDSs without closing file
 *	  test_empty_SDSs      - tests on empty chunked and chunked/comp SDSs
 *	  test_chunked_partial - tests on chunked and partially written SDS
 *	  test_chkcmp_SDSs     - tests chunked/compressed SDSs
 *	  test_extend_SDSs     - tests SDSs with unlimited dimensions
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

#define FILE_NAME     "datainfo.hdf"	/* data file */
#define X_LENGTH      10
#define Y_LENGTH      10
#define RANK          2

void alloc_info(hdf_datainfo_t *info, uintn info_count)
{
    HDmemset(info, 0, sizeof(info));
    info->offsets = (int32 *) HDmalloc(info_count * sizeof(int32));
    info->lengths = (int32 *) HDmalloc(info_count * sizeof(int32));
}

void free_info(hdf_datainfo_t *info)
{
    if (info != NULL)
    {
	if (info->offsets != NULL)
	    HDfree(info->offsets);
	if (info->lengths != NULL)
	    HDfree(info->lengths);
    }
}

/* This is used to temporarily verify results.  Will remove when finallized.*/
static void print_info(char* name, uintn info_count, hdf_datainfo_t data_info)
{
    int ii;

    fprintf(stderr, "offset/length of '%s'\n", name);
    for (ii = 0; ii < info_count; ii++)
	fprintf(stderr, "%d: %d   %d\n", ii, data_info.offsets[ii], data_info.lengths[ii]);
}

/* Test non-special SDSs.  This routine creates non-special SDSs, writes
 * data to one of the SDSs, and checks the returned value by SDgetdatainfo_count
 */
static intn test_nonspecial_SDSs(int32 fid)
{
    int32 sds_id;
    int32 dimsize[2], start[2], edges[2];
    int32 data[Y_LENGTH][X_LENGTH];
    uintn info_count = 0;
    hdf_datainfo_t data_info;
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

    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_nonspecial_SDSs: SDgetdatainfo_count");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    /* Create another 2x2 dataset called "WrittenDataset" */
    sds_id = SDcreate(fid, "WrittenDataset", DFNT_INT32, 2, dimsize);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDcreate");

    /* Define the location and size of the data to be written to the dataset */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(sds_id, FAIL, "test_nonspecial_SDSs: SDwritedata");

    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 1, "test_nonspecial_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_nonspecial_SDSs: SDgetdatainfo");
	VERIFY(info_count, 1, "test_nonspecial_SDSs: SDgetdatainfo");
    }

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_nonspecial_SDSs: SDendaccess");

    free_info(&data_info);

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_nonspecial_SDSs */

/* Test compressed SDSs.  This routine creates "Compressed-No-Data" and creates
 * and writes to "CompressedData" and "Non-CompressedData" SDSs.  It will then
 * check the returned value from SDgetdatainfo_count calls.
 */
static intn test_compressed_SDSs(int32 fid)
{
    int32     sds_id, esds_id, usds_id;
    int32     start[2], edges[2], dim_sizes[2];
    int32     comp_type;    /* Compression flag */
    comp_info c_info;	   /* Compression structure */
    int8     data[Y_LENGTH][X_LENGTH];
    uintn info_count = 0;
    hdf_datainfo_t data_info;
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
    sds_id = SDcreate(fid, "CompressedData", DFNT_INT8, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDcreate");

    esds_id = SDcreate(fid, "Compressed-No-Data", DFNT_INT32, RANK, dim_sizes);
    CHECK(esds_id, FAIL, "test_compressed_SDSs: SDcreate");

    usds_id = SDcreate(fid, "Non-CompressedData", DFNT_INT8, RANK, dim_sizes);
    CHECK(usds_id, FAIL, "test_compressed_SDSs: SDcreate");

    comp_type = COMP_CODE_DEFLATE;
    c_info.deflate.level = 6;
    status = SDsetcompress(sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress");

    status = SDsetcompress(esds_id, comp_type, &c_info);
    CHECK(status, FAIL, "test_compressed_SDSs: SDsetcompress");

    /* Define the location and size of the dataset to be written to the file */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    status = SDgetdatainfo_count(esds_id, &info_count);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_compressed_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(esds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");
	VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");
    }

    /* Write the stored data to the compressed dataset */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata");
 
    /* Write the stored data to the dataset non-compressed dataset */
    status = SDwritedata(usds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "test_compressed_SDSs: SDwritedata");
 
    status = SDgetdatainfo_count(usds_id, &info_count);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(usds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");
	VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");
    }

     /* Close the SDSs */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    status = SDendaccess(esds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    status = SDendaccess(usds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    sds_id = SDselect(fid, SDnametoindex(fid, "CompressedData"));
    CHECK(sds_id, FAIL, "test_compressed_SDSs: SDselect");
    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo_count");
    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_compressed_SDSs: SDgetdatainfo");
	VERIFY(info_count, 1, "test_compressed_SDSs: SDgetdatainfo");
    }

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_compressed_SDSs: SDendaccess");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_compressed_SDSs */

/* Test with empty SDSs.  This routine creates a "Chunked Empty" SDS and a
 * "Chunked Compressed Empty" SDS then uses SDgetdatainfo_count to get the
 * number of blocks for data, which should be 0.
 */
#define X_LENGTH2  4
#define Y_LENGTH2  9
#define CHK_X      3
#define CHK_Y      2

static intn test_empty_SDSs(int32 fid)
{
    int32         ch_sds_id, chcmp_sds_id, sds_index;
    int32         dim_sizes[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         flag;
    uintn info_count = 0;
    hdf_datainfo_t data_info;
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create Y_LENGTH2 x X_LENGTH2 empty chunked and chunked/compressed SDSs */
    dim_sizes[0] = Y_LENGTH2;
    dim_sizes[1] = X_LENGTH2;
    ch_sds_id = SDcreate(fid, "Chunked Empty", DFNT_INT16, RANK, dim_sizes);
    CHECK(ch_sds_id, FAIL, "test_empty_SDSs: SDcreate");

    chcmp_sds_id = SDcreate(fid, "Chunked Compressed Empty", DFNT_INT16, RANK, dim_sizes);
    CHECK(chcmp_sds_id, FAIL, "test_empty_SDSs: SDcreate");

    /* Set info for chunking */
    flag = HDF_CHUNK;
    status = SDsetchunk(ch_sds_id, c_def, flag);
    CHECK(status, FAIL, "test_empty_SDSs: SDsetchunk");

    /* Set info for chunking and compression */
    flag = HDF_CHUNK | HDF_COMP;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 6;
    status = SDsetchunk(chcmp_sds_id, c_def, flag);
    CHECK(status, FAIL, "test_empty_SDSs: SDsetchunk");

    /* Terminate access to the datasets */
    status = SDendaccess(ch_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess");
    status = SDendaccess(chcmp_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess");

    sds_index = SDnametoindex(fid, "Chunked Empty");
    CHECK(sds_index, FAIL, "test_empty_SDSs: SDnametoindex");

    ch_sds_id = SDselect(fid, sds_index);
    CHECK(ch_sds_id, FAIL, "test_empty_SDSs: SDselect");

    status = SDgetdatainfo_count(ch_sds_id, &info_count);
    CHECK(status, FAIL, "test_empty_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_empty_SDSs: SDgetdatainfo_count");

    status = SDendaccess(ch_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess");

    sds_index = SDnametoindex(fid, "Chunked Compressed Empty");
    CHECK(sds_index, FAIL, "test_empty_SDSs: SDnametoindex");

    chcmp_sds_id = SDselect(fid, sds_index);
    CHECK(ch_sds_id, FAIL, "test_empty_SDSs: SDselect");

    status = SDgetdatainfo_count(chcmp_sds_id, &info_count);
    CHECK(status, FAIL, "test_empty_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_empty_SDSs: SDgetdatainfo_count");

    status = SDendaccess(chcmp_sds_id);
    CHECK(status, FAIL, "test_empty_SDSs: SDendaccess");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_empty_SDSs */

/* Test with chunked and partially written SDS.  This routine creates a 
 * "Chunked Not Empty" SDS and writes some chunks but not all to it.  It will
 * then call SDgetdatainfo to verify the number of data blocks.
 */
static intn test_chunked_partial(int32 fid)
{
    int32         sds_id, sds_index;
    int32         dim_sizes[RANK], origin[RANK];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    int32         flag; /* Chunking flag */
    int32         fill_value = 0;   /* Fill value */
    uintn info_count = 0;
    hdf_datainfo_t data_info;
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int32 chunk1[CHK_X][CHK_Y] = { {4, 4},
                           {4, 4},
                           {4, 4} }; 

    int32 chunk3[CHK_X][CHK_Y] = { {3, 3},
                           {3, 3},
                           {3, 3} }; 

    /* Initialize chunk size */
    HDmemset(&c_def, 0, sizeof(c_def)) ;
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    /* Create Y_LENGTH2 x X_LENGTH2 SDS */
    dim_sizes[0] = Y_LENGTH2;
    dim_sizes[1] = X_LENGTH2;
    sds_id = SDcreate(fid, "Chunked Not Empty", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "test_chunked_partial: SDcreate");

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
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess");

    sds_id = SDselect(fid, SDnametoindex(fid, "Chunked Not Empty"));
    CHECK(sds_id, FAIL, "test_chunked_partial: SDselect");

    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_chunked_partial: SDgetdatainfo_count");
    VERIFY(info_count, 2, "test_chunked_partial: SDgetdatainfo_count");
    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_chunked_partial: SDgetdatainfo");
	VERIFY(info_count, 2, "test_chunked_partial: SDgetdatainfo");
    }

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chunked_partial: SDendaccess");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chunked_partial */

/* Test with chunked and compressed SDS.  This routine creates 
 * "ChunkedDeflateData" and "ChunkedNoDeflateData" SDSs and writes the same
 * data to both.  It will then use SDgetdatainfo to verify the number of
 * data blocks.
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
    uintn info_count = 0;
    hdf_datainfo_t data_info;
    intn          status;
    int           num_errs = 0;   /* number of errors so far */

    /* Declare chunks data type and initialize some of them. */
    int16 chunk1[CHK_X][CHK_Y] = {
                           {1, 1},
                           {1, 1},
                           {1, 1} }; 

    int16 chunk3[CHK_X][CHK_Y] = {
                           {3, 3},
                           {3, 3},
                           {3, 3} }; 

    int32 chunk2[CHK_X][CHK_Y] = {
                           {2, 2},
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
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDcreate");

    sds_id = SDcreate(fid, "ChunkedNoDeflateData", DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDcreate");

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue(cmpsds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue");

    status = SDsetfillvalue(sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetfillvalue");

    /* Set info for chunking and compression */
    flag = HDF_CHUNK | HDF_COMP;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 6;
    status = SDsetchunk(cmpsds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk");

    /* Set info for chunking and compression */
    HDmemset(&c_def, 0, sizeof(c_def)) ;
    c_def.chunk_lengths[0] = CHK_X;
    c_def.chunk_lengths[1] = CHK_Y;

    flag = HDF_CHUNK;
    status = SDsetchunk(sds_id, c_def, flag);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDsetchunk");

    /* Set chunk cache to hold maximum of 3 chunks */
    maxcache = 3;
    flag = 0;
    new_maxcache = SDsetchunkcache(cmpsds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache");

    new_maxcache = SDsetchunkcache(sds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "test_chkcmp_SDSs: SDsetchunkcache");

    /* Terminate access to the dataset before writing data to it. */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    /* Re-select the datasets, write chunks using SDwritechunk function, then
       check their data sizes */

    /* Get index of dataset using its name */
    cmpsds_index = SDnametoindex(fid, "ChunkedDeflateData");
    CHECK(cmpsds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex");

    sds_index = SDnametoindex(fid, "ChunkedNoDeflateData");
    CHECK(sds_index, FAIL, "test_chkcmp_SDSs: SDnametoindex");

    /* Select the datasets for access */
    cmpsds_id = SDselect(fid, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect");
    sds_id = SDselect(fid, sds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    status = SDgetdatainfo_count(cmpsds_id, &info_count);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_chkcmp_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
	VERIFY(info_count, 0, "test_chkcmp_SDSs: SDgetdatainfo");
    }

    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 0, "test_chkcmp_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
	VERIFY(info_count, 0, "test_chkcmp_SDSs: SDgetdatainfo");
    }

    /* Write the chunk with the coordinates (0,0) */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (1,0) */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Write the chunk with the coordinates (0,1) */
    origin[0] = 0;
    origin[1] = 1;
    status = SDwritechunk(cmpsds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");
    status = SDwritechunk(sds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDwritechunk");

    /* Terminate access to the datasets */
    status = SDendaccess(cmpsds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDendaccess");

    /* Verify the compressed and non-compressed data sizes of the datasets */

    /* Open dataset 'ChunkedDeflateData' */
    cmpsds_id = SDselect(fid, cmpsds_index);
    CHECK(cmpsds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    /* Open dataset 'ChunkedNoDeflateData' */
    sds_id = SDselect(fid, sds_index);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDselect");

    status = SDgetdatainfo_count(sds_id, &info_count);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 3, "test_chkcmp_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
	VERIFY(info_count, 3, "test_chkcmp_SDSs: SDgetdatainfo");
    }

    status = SDendaccess(sds_id);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDendaccess");

    status = SDgetdatainfo_count(cmpsds_id, &info_count);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo_count");
    VERIFY(info_count, 3, "test_chkcmp_SDSs: SDgetdatainfo_count");

    if (info_count > 0)
    {
	alloc_info(&data_info, info_count);
	status = SDgetdatainfo(sds_id, info_count, 0, &data_info);
	CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatainfo");
	VERIFY(info_count, 3, "test_chkcmp_SDSs: SDgetdatainfo");
    }
    status = SDendaccess(cmpsds_id);
    CHECK(sds_id, FAIL, "test_chkcmp_SDSs: SDendaccess");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_chkcmp_SDSs */

/* Test SDSs with unlimited dimensions.  This routine creates SDSs with
   unlimited dimensions, writes data to it, and checks the sizes returned 
   by SDgetdatainfo.
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

    /* Create a 2x2 dataset called "AppendableDataset 1" */
    dimsize[0] = SD_UNLIMITED;
    dimsize[1] = X_LENGTH;
    sds_id = SDcreate(fid, "AppendableDataset 1", DFNT_INT32, 2, dimsize);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate");

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

    /* Create another dataset with 1 unlimited dimension */
    sds_id = SDcreate(fid, "AppendableDataset 2", DFNT_FLOAT64, 1, dimsize);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDcreate");

    /* Define the location and size of the data to be written to the dataset */
    start1[0] = 0;
    edges1[0] = Y_LENGTH;

    /* Write the stored data to the dataset */
    status = SDwritedata(sds_id, start1, NULL, edges1, (VOIDP)fdata);
    CHECK(sds_id, FAIL, "test_extend_SDSs: SDwritedata");

    /* Close this SDS */
    status = SDendaccess(sds_id);
    CHECK(status, FAIL, "test_extend_SDSs: SDendaccess");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_extend_SDSs */

/* Test driver for testing the API SDgetdatainfo. */
extern int test_datainfo()
{
    int32 fid;
    intn status;
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("getting location info of data (tdatainfo.c)");

    /* Open the file and initialize the SD interface */
    fid = SDstart(FILE_NAME, DFACC_CREATE);
    CHECK(fid, FAIL, "test_datainfo: SDstart");

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
    CHECK(status, FAIL, "test_datainfo: SDend");

    if (num_errs == 0) PASSED();
    return num_errs;
}

#endif /* HDF */
