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

#define CHKFILE   "chktst.hdf"    /* Chunking test file */
#define CNBITFILE "chknbit.hdf"   /* Chunking w/ NBIT compression */

/* Dimensions of slab */
static int32  edge_dims[3]  = {2, 3, 4};  /* size of slab dims */
static int32  start_dims[3] = {0, 0, 0};  /* starting dims  */
static int32  d_dims[3]     = {2, 3, 4};
static int32  cdims[3]      = {1, 2, 3};  /* chunk lengths */

/* Chunk teseting - arrays for chunk writes */
/* float32 arrays */
#ifdef NOT_USED
static float32  chunk1_f32[4] =
        {(float32) 0.0, (float32) 1.0, (float32) 2.0, (float32) 3.0};

static float32  chunk2_f32[4] =
        {(float32) 10.0, (float32) 11.0, (float32) 12.0, (float32) 13.0};

static float32  chunk3_f32[4] =
        {(float32) 20.0, (float32) 21.0, (float32) 22.0, (float32) 23.0};

static float32  chunk4_f32[4] =
        {(float32) 100.0, (float32) 101.0, (float32) 102.0, (float32) 103.0};

static float32  chunk5_f32[4] =
        {(float32) 110.0, (float32) 111.0, (float32) 112.0, (float32) 113.0};

static float32  chunk6_f32[4] =
        {(float32) 120.0, (float32) 121.0, (float32) 122.0, (float32) 123.0};

/* uint16 arrays */
static uint16  chunk1_u16[4] =        { 0, 1, 2, 3};

static uint16  chunk2_u16[4] =        { 10, 11, 12, 13};

static uint16  chunk3_u16[4] =        { 20, 21, 22, 23};

static uint16  chunk4_u16[4] =        { 100, 101, 102, 103};

static uint16  chunk5_u16[4] =        { 110, 111, 112, 113};

static uint16  chunk6_u16[4] =        { 120, 121, 122, 123};
#endif /* NOT_USED */

/* uint16 chunk arrays used in example 1 */
static uint16  chunk1_2u16[6] = {11, 21, 
                                 12, 22, 
                                 13, 23};

static uint16  chunk2_2u16[6] = {31, 41, 
                                 32, 42, 
                                 33, 43};

static uint16  chunk3_2u16[6] = {14, 24, 
                                 15, 25, 
                                 16, 26};

static uint16  chunk4_2u16[6] = {34, 44, 
                                 35, 45, 
                                 36, 46};

static uint16  chunk5_2u16[6] = {17, 27, 
                                 18, 28, 
                                 19, 29};

static uint16  chunk6_2u16[6] = {37, 47, 
                                 38, 48, 
                                 39, 49};

/* for visual layout in Example 1*/
static uint16  u16_2data[9][4] =
{ 
   {11, 21, 31, 41},
   {12, 22, 32, 42},
   {13, 23, 33, 43},
   {14, 24, 34, 44},
   {15, 25, 35, 45},
   {16, 26, 36, 46},
   {17, 27, 37, 47},
   {18, 28, 38, 48},
   {19, 29, 39, 49},
};

/* for comparison in example 1 */
static uint16 u16_2cdata[5][2] =
{
  {23, 33},
  {24, 34},
  {25, 35},
  {26, 36},
  {27, 37}
};

/* uint8 arrays */
static uint8  chunk1_u8[4] = { 0, 1, 2, 3};

static uint8  chunk2_u8[4] = { 10, 11, 12, 13};

static uint8  chunk3_u8[4] = { 20, 21, 22, 23};

static uint8  chunk4_u8[4] = { 100, 101, 102, 103};

static uint8  chunk5_u8[4] = { 110, 111, 112, 113};

static uint8  chunk6_u8[4] = { 120, 121, 122, 123};

/* data arrays layed out in memory  */
/* for comparison */
static float32  f32_data[2][3][4] =
{
    {
        {(float32) 0.0, (float32) 1.0, (float32) 2.0, (float32) 3.0},
        {(float32) 10.0, (float32) 11.0, (float32) 12.0, (float32) 13.0},
        {(float32) 20.0, (float32) 21.0, (float32) 22.0, (float32) 23.0}},
    {
        {(float32) 100.0, (float32) 101.0, (float32) 102.0, (float32) 103.0},
        {(float32) 110.0, (float32) 111.0, (float32) 112.0, (float32) 113.0},
        {(float32) 120.0, (float32) 121.0, (float32) 122.0, (float32) 123.0}}};

static uint16  u16_data[2][3][4] =
{
    {
        { 0, 1, 2, 3},
        { 10, 11, 12, 13},
        { 20, 21, 22, 23}},
    {
        { 100, 101, 102, 103},
        { 110, 111, 112, 113},
        { 120, 121, 122, 123}}};

static uint8  u8_data[2][3][4] =
{
    {
        { 0, 1, 2, 3},
        { 10, 11, 12, 13},
        { 20, 21, 22, 23}},
    {
        { 100, 101, 102, 103},
        { 110, 111, 112, 113},
        { 120, 121, 122, 123}}};

extern int
test_chunk()
{
    int32 fchk; /* File handles */
    int32 nt;                /* Number type */
    int32 dimsize[10];       /* dimension sizes */
    int32   newsds1, newsds2, newsds3, newsds4, newsds5, 
	    newsds6, newsds7, newsds8;   /* Chunked SDS ids */
    float32 inbuf_f32[2][3][4];  /* float32 Data array read from from file */
    uint16  inbuf_u16[2][3][4];  /* uint16 Data array read from from file */
    uint16  inbuf1_2u16[9][4];   /* Data array read for Example 1 */
    uint16  inbuf_2u16[5][2];    /* Data array read for Example 1 */
    uint8   inbuf_u8[2][3][4];   /* uint8 Data array read from from file */
    uint8   ru8_data[4];         /* chunk input buffer */
    int32   *rcdims;             /* for SDgetchunkinfo() */
    uint16  fill_u16 = 0;        /* fill value */
    HDF_CHUNK_DEF chunk_def;     /* Chunk defintion set */ 
    HDF_CHUNK_DEF chunk_def_out; /* Chunk defintion set */ 
    HDF_CHUNK_DEF rchunk_def;    /* Chunk defintion read */ 
    comp_coder_t comp_type;      /* to retrieve compression type into */
    comp_info cinfo;             /* compression information structure */
    int32   cflags;              /* chunk flags */
    int32   c_flags;             /* chunk flags to set */
    int32   c_flags_out;         /* chunk flags retrieved */
    int32   index;       /* Index of dataset in file */
    intn    status;      /* status flag */
    intn    i,j,k;       /* loop variables */
    int32   start[10], end[10]; /* start, end, stride arrays */
    int32   idata[100];
    int32   rdata[100];
    float32 max;
    int     num_errs = 0;    /* number of errors so far */

    /* Output message about test being performed */
    TESTING("create/read/write chunked datasets (tchunk.c)");

    /* Create file 'chktst.hdf' */
    fchk = SDstart(CHKFILE, DFACC_CREATE);
    CHECK(fchk, FAIL, "SDstart");

    /* 
     * Test 1. Create a 9x4 SDS of uint16 in file 1 
     *         With chunks of 3x2, will create 6 chunks.
     */
    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds1 = SDcreate(fchk, "DataSetChunked_2D_1", DFNT_UINT16, 2, d_dims);
    if(newsds1 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 1. Failed to create a new data set \n");
        num_errs++;
        goto test2;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds1, (VOIDP) &fill_u16);
    CHECK(status, FAIL, "Chunk Test 1. SDsetfillvalue");

    /* Added to verify that the problem reported in bug HDFFR-5 is gone.  The
       fix was actually done for and documented in bug HDFFR-171.  It is
       verified here that SDsetchunk works properly after SDgetchunkinfo
       being called on an empty SDS prior to SDsetchunk. -BMR, 2011/10/25 */
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds1, NULL, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 1. SDgetchunkinfo on empty SDS");
    VERIFY(c_flags_out, (HDF_NONE), "Chunk Test 1. SDgetchunkinfo on empty SDS");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks */
    cdims[0] = chunk_def.chunk_lengths[0] = 3;
    cdims[1] = chunk_def.chunk_lengths[1] = 2;
    status = SDsetchunk(newsds1, chunk_def, HDF_CHUNK);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 1. Failed to create new chunked data set\n");
        num_errs++;
        goto test2;
      }

    /* Check getting chunked/compressed flag only with SDgetchunkinfo */
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds1, NULL, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 1. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK), "Chunk Test 1. SDgetchunkinfo");

    /* Check getting compression info with SDgetchunkinfo */
    HDmemset(&chunk_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds1, &chunk_def_out, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 1. SDgetchunkinfo");
    VERIFY(chunk_def_out.chunk_lengths[0], chunk_def.chunk_lengths[0], "SDgetchunkinfo");
    VERIFY(chunk_def_out.chunk_lengths[1], chunk_def.chunk_lengths[1], "SDgetchunkinfo");

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetchunkcache(newsds1, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 1. SDsetchunkcache failed\n");
        num_errs++;
        goto test2;
      }


    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDwritedata(newsds1, start_dims, NULL, edge_dims, (VOIDP) u16_2data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 1. Failed to write u16_2data to new chunked data set\n");
        num_errs++;
        goto test2;
      }

    /* read a portion of data back in using SDreaddata*/
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDreaddata(newsds1, start_dims, NULL, edge_dims, (VOIDP) inbuf1_2u16);
    CHECK(status, FAIL, "Chunk Test 1. SDreaddata");
    for (i = 0; i < 9; i++)
      {
        for (j = 0; j < 4; j++)
          {
              if (inbuf1_2u16[i][j] != u16_2data[i][j])
                {
                    fprintf(stderr,"Chunk Test 1. inbuf1_2u16[%d][%d]=%d,",
                            i,j,inbuf1_2u16[i][j]);
                    fprintf(stderr,"u16_cdata[%d][%d]=%d,",
                            i,j,u16_2data[i][j]);
                    fprintf(stderr,"\n");
                    num_errs++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetchunkinfo(newsds1, &rchunk_def, &cflags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 1. SDgetchunkinfo failed \n");
        num_errs++;
        goto test2;
      }

    rcdims = rchunk_def.chunk_lengths;

    /* check chunk lengths and to see if SDS is chunked */
    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
      {
        fprintf(stderr, "Chunk Test 1. SDgetchunkinfo returned wrong values\n");
        num_errs++;
        goto test2;
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds1);
    CHECK(status, FAIL, "Chunk Test 1. SDendaccess");

    /* 
      Test 2. 2-D 9x4 SDS of uint16 with 3x2 chunks
                 Write data using SDwritechunk().
                 Read data using SDreaddata().
    */
  test2:

    /* create a  9x4 SDS of uint16 in file 1 */
    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds2 = SDcreate(fchk, "DataSetChunked_2D_2", DFNT_UINT16, 2, d_dims);
    if(newsds2 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2. Failed to create a new data set \n");
        num_errs++;
        goto test3;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds2, (VOIDP) &fill_u16);
    CHECK(status, FAIL, "Chunk Test 2. SDsetfillvalue");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks */
    cdims[0] = chunk_def.chunk_lengths[0] = 3;
    cdims[1] = chunk_def.chunk_lengths[1] = 2;
    status = SDsetchunk(newsds2, chunk_def, HDF_CHUNK);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2. Failed to create new chunked data set\n");
        num_errs++;
        goto test3;
      }

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetchunkcache(newsds2, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDsetchunkcache failed\n");
        num_errs++;
        goto test3;
      }

    /* Write data use SDwriteChunk */

    /* Write chunk 1 */
    start_dims[0] = 0;
    start_dims[1] = 0;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk1_2u16);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 1\n");
        num_errs++;
        goto test3;
      }

    /* Write chunk 4 */
    start_dims[0] = 1;
    start_dims[1] = 1;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk4_2u16);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 4\n");
        num_errs++;
        goto test3;
      }

    /* Write chunk 2 */
    start_dims[0] = 0;
    start_dims[1] = 1;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk2_2u16);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 2\n");
        num_errs++;
        goto test3;
      }

    /* Write chunk 5 */
    start_dims[0] = 2;
    start_dims[1] = 0;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk5_2u16);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 5\n");
        num_errs++;
        goto test3;
      }

    /* Write chunk 3 */
    start_dims[0] = 1;
    start_dims[1] = 0;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk3_2u16);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 3\n");
        num_errs++;
        goto test3;
      }

    /* Write chunk 6 */
    start_dims[0] = 2;
    start_dims[1] = 1;
    status = SDwritechunk(newsds2, start_dims, (VOIDP) chunk6_2u16);
     if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDwritechunk failed to write chunk 6\n");
        num_errs++;
        goto test3;
      }

    /* read a portion of data back in using SDreaddata
       i.e  5x2 subset of the whole array */
    start_dims[0] = 2;
    start_dims[1] = 1;
    edge_dims[0] = 5;
    edge_dims[1] = 2;
    status = SDreaddata(newsds2, start_dims, NULL, edge_dims, (VOIDP) inbuf_2u16);
    CHECK(status, FAIL, "Chunk Test 2. SDreaddata");
   /* This 5x2 array should look somethink like this
         {{23, 24, 25, 26, 27},
          {33, 34, 35, 36, 37}}    
    */
    for (i = 0; i < 5; i++)
      {
        for (j = 0; j < 2; j++)
          {
              if (inbuf_2u16[i][j] != u16_2cdata[i][j])
                {
                    fprintf(stderr,"Chunk Test 2. inbuf_2u16[%d][%d]=%d,",
                            i,j,inbuf_2u16[i][j]);
                    fprintf(stderr,"u16_2cdata[%d][%d]=%d,",
                            i,j,u16_2cdata[i][j]);
                    fprintf(stderr,"\n");
                    num_errs++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetchunkinfo(newsds2, &rchunk_def, &cflags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 2.SDgetchunkinfo failed \n");
        num_errs++;
        goto test3;
      }

    rcdims = rchunk_def.chunk_lengths;

    /* check chunk lengths and see if SDS is chunked */
    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cflags != HDF_CHUNK)
      {
        fprintf(stderr, "Chunk Test 2.SDgetchunkinfo returned wrong values\n");
        num_errs++;
        goto test3;
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "Chunk Test 2. SDendaccess");

    /* 
     * Next 3 differnet number types are tested with 3-D arrays
     */
  test3:
    /* 
     * Test 3. create a new chunked SDS of float32 in file 1 
     */
    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds3 = SDcreate(fchk, "DataSetChunked_3D_1", DFNT_FLOAT32, 3, d_dims);
    if(newsds3 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 3. Failed to create a new 3D float32 data set \n");
        num_errs++;
        goto test4;
      }

    max = 0.0;
    status = SDsetfillvalue(newsds3, (VOIDP) &max);
    CHECK(status, FAIL, "Chunk Test 3. SDsetfillvalue");

    /* Set chunking */
    cdims[0] = chunk_def.chunk_lengths[0] = 1;
    cdims[1] = chunk_def.chunk_lengths[1] = 2;
    cdims[2] = chunk_def.chunk_lengths[2] = 3;
    status = SDsetchunk(newsds3, chunk_def, HDF_CHUNK);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 3. Failed to create new chunked data set\n");
        num_errs++;
        goto test4;
      }

    /* Write data out */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds3, start_dims, NULL, edge_dims, (VOIDP) f32_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 3. Failed to write f32_data to new chunked data set\n");
        num_errs++;
        goto test4;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds3, start_dims, NULL, edge_dims, (VOIDP) inbuf_f32);
    CHECK(status, FAIL, "Chunk Test 3. SDreaddata");

    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_f32[i][j][k] != f32_data[i][j][k])
                    {
                        fprintf(stderr,"Chunk Test 3. inbuf_f32[%d][%d][%d]=%f,",
                                i,j,k,inbuf_f32[i][j][k]);
                        fprintf(stderr,"f32_data[%d][%d][%d]=%f,",
                                i,j,k,f32_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_errs++;
                    }
              }
          }
      }

    /* Close down SDS*/    
    status = SDendaccess(newsds3);
    CHECK(status, FAIL, "Chunk Test 3. SDendaccess");


    /* 
     * Test 4. Create a new chunked SDS of uint16 in file 1 
     */
  test4:

    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds4 = SDcreate(fchk, "DataSetChunked_3D_2", DFNT_UINT16, 3, d_dims);
    if(newsds4 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 4. Failed to set a new uint16 3D data set chunked\n");
        num_errs++;
        goto test5;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds4, (VOIDP) &fill_u16);
    CHECK(status, FAIL, "Chunk Test 4. SDsetfillvalue");

    /* Set chunking, chunk is 1x2x3 */
    cdims[0] = chunk_def.chunk_lengths[0] = 1;
    cdims[1] = chunk_def.chunk_lengths[1] = 2;
    cdims[2] = chunk_def.chunk_lengths[2] = 3;
    status = SDsetchunk(newsds4, chunk_def, HDF_CHUNK);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 4. Failed to create new chunked data set\n");
        num_errs++;
        goto test5;
      }

    /* Set Chunk cache */
    status = SDsetchunkcache(newsds4, 4, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 4. SDsetchunkcache failed\n");
        num_errs++;
        goto test5;
      }

    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds4, start_dims, NULL, edge_dims, (VOIDP) u16_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 4. Failed to write u16_data to new chunked data set\n");
        num_errs++;
        goto test5;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds4, start_dims, NULL, edge_dims, (VOIDP) inbuf_u16);
    CHECK(status, FAIL, "Chunk Test 4. SDreaddata");
    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_u16[i][j][k] != u16_data[i][j][k])
                    {
                        fprintf(stderr,"Chunk Test 4. inbuf_u16[%d][%d][%d]=%d,",
                                i,j,k,inbuf_u16[i][j][k]);
                        fprintf(stderr,"u16_data[%d][%d][%d]=%d,",
                                i,j,k,u16_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_errs++;
                    }
              }
          }
      }
    /* Check getting chunked flag */
    status = SDgetchunkinfo(newsds4, NULL, &cflags);
    CHECK(status, FAIL, "Chunk Test 4. SDgetchunkinfo");
    VERIFY(cflags, HDF_CHUNK, "Chunk Test 4. SDgetchunkinfo");

    /* Check chunk info */
    status = SDgetchunkinfo(newsds4, &rchunk_def, &cflags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 4. SDgetchunkinfo failed \n");
        num_errs++;
        goto test5;
      }

    rcdims = rchunk_def.chunk_lengths;

    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] || cdims[2] != rcdims[2] 
        || cflags != HDF_CHUNK)
      {
        fprintf(stderr, "Chunk Test 4. SDgetchunkinfo returned wrong values\n");
        num_errs++;
        goto test5;
      }

    /* Close down SDS*/    
    status = SDendaccess(newsds4);
    CHECK(status, FAIL, "Chunk Test 4. SDendaccess");


    /* 
     * Test 5. Create a new chunked SDS of uint8 in file 1 
     */
  test5:

    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds5 = SDcreate(fchk, "DataSetChunked_3D_3", DFNT_UINT8, 3, d_dims);
    if(newsds5 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. Failed to set a new uint8 3D data set chunked\n");
        num_errs++;
        goto test6;
      }

    /* Set chunking, chunk is 1x1x4 */
    cdims[0] = chunk_def.chunk_lengths[0] = 1;
    cdims[1] = chunk_def.chunk_lengths[1] = 1;
    cdims[2] = chunk_def.chunk_lengths[2] = 4;
    status = SDsetchunk(newsds5, chunk_def, HDF_CHUNK);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. Failed to create new chunked data set\n");
        num_errs++;
        goto test6;
      }

#if 0
    /* Write data using SDwritedata() */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDwritedata(newsds5, start_dims, NULL, edge_dims, (VOIDP) wu8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. Failed to write wu8_data to new chunked data set\n");
        num_errs++;
        goto test6;
      }
#endif

    /* Write data use SDwriteChunk */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk1_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 1\n");
        num_errs++;
        goto test6;
      }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk4_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 4\n");
        num_errs++;
        goto test6;
      }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk2_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 2\n");
        num_errs++;
        goto test6;
      }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk5_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 5\n");
        num_errs++;
        goto test6;
      }

    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk3_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 3\n");
        num_errs++;
        goto test6;
      }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwritechunk(newsds5, start_dims, (VOIDP) chunk6_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDwritechunk failed to write chunk 6\n");
        num_errs++;
        goto test6;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds5, start_dims, NULL, edge_dims, (VOIDP) inbuf_u8);
    CHECK(status, FAIL, "Chunk Test 5. SDreaddata");
    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_u8[i][j][k] != u8_data[i][j][k])
                    {
                        fprintf(stderr,"Chunk Test 5. inbuf_u8[%d][%d][%d]=%d,",
                                i,j,k,inbuf_u8[i][j][k]);
                        fprintf(stderr,"u8_data[%d][%d][%d]=%d,",
                                i,j,k,u8_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_errs++;
                    }
              }
          }
      }

    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 1\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk1_u8[i])
            {
                printf("Chunk Test 5. chunk1_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk1_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 2\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk2_u8[i])
            {
                printf("Chunk Test 5. chunk2_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk2_u8[i], ru8_data[i]);
                num_errs++;
            }
       }
    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 3\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk3_u8[i])
            {
                printf("Chunk Test 5. chunk3_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk3_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 4\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk4_u8[i])
            {
                printf("Chunk Test 5. chunk4_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk4_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 5\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk5_u8[i])
            {
                printf("Chunk Test 5. chunk5_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk5_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadchunk(newsds5, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 5. SDreadchunk failed to read chunk 6\n");
        num_errs++;
        goto test6;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk6_u8[i])
            {
                printf("Chunk Test 5. chunk6_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk6_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    /* Close down SDS*/    
    status = SDendaccess(newsds5);
    CHECK(status, FAIL, "Chunk Test 5. SDendaccess");


    /* ---------------------------------------------------------------
     *  Chunking with Compression 
     ----------------------------------------------------------------*/


    /* 
     * Test 6. Create a new chunked SDS of uint8 in file 1 
     *         Compress using Skipping Huffman. Write using SDwriteChunk
     *         Read back in using SDreaddata and SDreadChunk. 
     *	       Retrieve and verify the compression information.
     *         Use Skipping Huffman compression
     *         Note: a template is created first then the SDS 
     *               is re-slected for writing/reading.
     */
  test6:

    d_dims[0] = 2;
    d_dims[1] = 3;
    d_dims[2] = 4;
    newsds6 = SDcreate(fchk, "DataSetChunked_3D_SKIP_HUF_2", DFNT_UINT8, 3, d_dims);
    if(newsds6 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. Failed to set a new uint8 3D data set chunked\n");
        num_errs++;
        goto test7;
      }

    /* Set chunking, chunk is 1x1x4 */
    cdims[0] = chunk_def.comp.chunk_lengths[0] = 1;
    cdims[1] = chunk_def.comp.chunk_lengths[1] = 1;
    cdims[2] = chunk_def.comp.chunk_lengths[2] = 4;
#if 0
    chunk_def.comp.comp_type = COMP_CODE_RLE;
#endif
    chunk_def.comp.comp_type = COMP_CODE_SKPHUFF; /* Skipping Huffman */
    chunk_def.comp.cinfo.skphuff.skp_size = sizeof(uint16);
    status = SDsetchunk(newsds6, chunk_def, HDF_CHUNK | HDF_COMP);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. Failed to create new chunked, Skipping Huffman compressed data set\n");
        num_errs++;
        goto test7;
      }

    /* Close down SDS ie. template creation*/    
    status = SDendaccess(newsds6);
    CHECK(status, FAIL, "Chunk Test 6. SDendaccess");

    newsds6 = FAIL;

    /* Select same SDS again, first get index */
    if ((index = SDnametoindex(fchk,"DataSetChunked_3D_SKIP_HUF_2")) == FAIL)
      {
          fprintf(stderr, "Chunk Test 6. SDnametoindex  Failed for  Skipping Huffman compressed data set\n");
          num_errs++;
          goto test7;
      }

    if ((newsds6 = SDselect(fchk,index)) == FAIL)
      {
          fprintf(stderr, "Chunk Test 6. SDselect Failed to re-select new chunked, Skipping Huffman compressed data set\n");
          num_errs++;
          goto test7;
      }

    /*
     * Retrieve and verify the compression type
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds6, &comp_type);
    CHECK(status, FAIL, "Chunk Test 6. SDgetcomptype");
    VERIFY(comp_type, chunk_def.comp.comp_type, "Chunk Test 6. SDgetcomptype");

    /*
     * Retrieve and verify the compression info - bug# 307, 10/10/01 - BMR
     * Updated: SDgetcompress replaced by SDgetcompinfo - bugzilla# 130,
     *		4/17/05 -BMR
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds6, &comp_type, &cinfo);
    CHECK(status, FAIL, "Chunk Test 6. SDgetcompinfo");
    VERIFY(comp_type, chunk_def.comp.comp_type, "Chunk Test 6. SDgetcompinfo");
    VERIFY(cinfo.skphuff.skp_size, chunk_def.comp.cinfo.skphuff.skp_size, "Chunk Test 6. SDgetcompinfo");

    /* Write data use SDwriteChunk */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk1_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 1\n");
        num_errs++;
        goto test7;
      }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk4_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 4\n");
        num_errs++;
        goto test7;
      }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk2_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 2\n");
        num_errs++;
        goto test7;
      }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk5_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 5\n");
        num_errs++;
        goto test7;
      }

    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk3_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 3\n");
        num_errs++;
        goto test7;
      }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDwritechunk(newsds6, start_dims, (VOIDP) chunk6_u8);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDwritechunk failed to write chunk 6\n");
        num_errs++;
        goto test7;
      }

    /* read data back in */
    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    edge_dims[0] = 2;
    edge_dims[1] = 3;
    edge_dims[2] = 4;
    status = SDreaddata(newsds6, start_dims, NULL, edge_dims, (VOIDP) inbuf_u8);
    CHECK(status, FAIL, "Chunk Test 6. SDreaddata");
    for (i = 0; i < d_dims[0]; i++)
      {
        for (j = 0; j < d_dims[1]; j++)
          {
            for (k = 0; k < d_dims[2]; k++)
              {
                  if (inbuf_u8[i][j][k] != u8_data[i][j][k])
                    {
                        fprintf(stderr,"Chunk Test 6. inbuf_u8[%d][%d][%d]=%d,",
                                i,j,k,inbuf_u8[i][j][k]);
                        fprintf(stderr,"u8_data[%d][%d][%d]=%d,",
                                i,j,k,u8_data[i][j][k]);
                        fprintf(stderr,"\n");
                        num_errs++;
                    }
              }
          }
      }

    start_dims[0] = 0;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDreadchunk failed to read chunk 1\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk1_u8[i])
            {
                printf("Chunk Test 6. chunk1_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk1_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 0;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDreadchunk failed to read chunk 2\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk2_u8[i])
            {
                printf("Chunk Test 6. chunk2_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk2_u8[i], ru8_data[i]);
                num_errs++;
            }
       }
    start_dims[0] = 0;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDreadchunk failed to read chunk 3\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk3_u8[i])
            {
                printf("Chunk Test 6. chunk3_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk3_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 0;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "SDreadchunk failed to read chunk 4\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk4_u8[i])
            {
                printf("Chunk Test 6. chunk4_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk4_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDreadchunk failed to read chunk 5\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk5_u8[i])
            {
                printf("Chunk Test 6. chunk5_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk5_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 0;
    status = SDreadchunk(newsds6, start_dims, (VOIDP) ru8_data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 6. SDreadchunk failed to read chunk 6\n");
        num_errs++;
        goto test7;
      }

    /* Verify chunk */
    for (i = 0; i < 4; i++)
       {
          if (ru8_data[i] != chunk6_u8[i])
            {
                printf("Chunk Test 6. chunk6_u8: Wrong data at %d, out %d in %d\n", 
                 i, chunk6_u8[i], ru8_data[i]);
                num_errs++;
            }
       }

    /* Check getting chunked/compressed flag only with SDgetchunkinfo */
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds6, NULL, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 6. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_COMP), "Chunk Test 6. SDgetchunkinfo");

    /* Check getting compression info with SDgetchunkinfo */
    HDmemset(&chunk_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds6, &chunk_def_out, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 6. SDgetchunkinfo");
    VERIFY(chunk_def_out.comp.comp_type, comp_type, "Chunk Test 6. SDgetchunkinfo");
    VERIFY(chunk_def_out.comp.cinfo.skphuff.skp_size, chunk_def.comp.cinfo.skphuff.skp_size, "Chunk Test 6. chunkinfo_new");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_COMP), "Chunk Test 6. SDgetchunkinfo");

    /* Close down SDS*/    
    status = SDendaccess(newsds6);
    CHECK(status, FAIL, "Chunk Test 6. SDendaccess");

    newsds6 = FAIL;

    /* Select same SDS again, first get index */
    if ((index = SDnametoindex(fchk,"DataSetChunked_3D_SKIP_HUF_2")) == FAIL)
      {
          fprintf(stderr, "Chunk Test 6. SDnametoindex  Failed for  Skipping Huffman compressed data set\n");
          num_errs++;
          goto test7;
      }

    if ((newsds6 = SDselect(fchk,index)) == FAIL)
      {
          fprintf(stderr, "Chunk Test 6. SDselect Failed to re-select new chunked, Skipping Huffman compressed data set\n");
          num_errs++;
          goto test7;
      }

    /*
     * Retrieve and verify the compression type
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds6, &comp_type);
    CHECK(status, FAIL, "Chunk Test 6. SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "Chunk Test 6. SDgetcomptype");

    /* Close down SDS*/    
    status = SDendaccess(newsds6);
    CHECK(status, FAIL, "Chunk Test 6. SDendaccess");


    /* 
     * Test 7. Create a  9x4 SDS of uint16 in file 1 
     *         Write using SDwritedata, read back in using SDreaddata
     *         Use GZIP compression.
     */
  test7:

    d_dims[0] = 9;
    d_dims[1] = 4;
    newsds7 = SDcreate(fchk, "DataSetChunked_2D_GZIP_1", DFNT_UINT16, 2, d_dims);
    if(newsds7 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 7. Failed to create a new 2D uint16 data set \n");
        num_errs++;
        goto test8;
      }

    /* set fill value */
    fill_u16 = 0;
    status = SDsetfillvalue(newsds7, (VOIDP) &fill_u16);
    CHECK(status, FAIL, "Chunk Test 7. SDsetfillvalue");

    /* Create chunked SDS 
       chunk is 3x2 which will create 6 chunks.
       Use GZIP compression */
    cdims[0] = chunk_def.comp.chunk_lengths[0] = 3;
    cdims[1] = chunk_def.comp.chunk_lengths[1] = 2;
#if 0
    chunk_def.comp.comp_type = COMP_CODE_RLE;   /* RLE */

    chunk_def.comp.comp_type = COMP_CODE_SKPHUFF; /* Skipping Huffman */
    chunk_def.comp.cinfo.skphuff.skp_size = sizeof(uint16);
#endif
    chunk_def.comp.comp_type = COMP_CODE_DEFLATE; /* GZIP */
    chunk_def.comp.cinfo.deflate.level = 6;

    status = SDsetchunk(newsds7, chunk_def, HDF_CHUNK | HDF_COMP);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 7. Failed to create new chunked, GZIP Compressed data set\n");
        num_errs++;
        goto test8;
      }

    /* Set Chunk cache to hold 2 chunks */
    status = SDsetchunkcache(newsds7, 2, 0);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 7. SDsetchunkcache failed\n");
        num_errs++;
        goto test8;
      }

    /* Check getting chunked/compressed flag only with SDgetchunkinfo */
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds7, NULL, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 7. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_COMP), "Chunk Test 7. SDgetchunkinfo");

    HDmemset(&chunk_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds7, &chunk_def_out, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 7. SDgetchunkinfo");
    VERIFY(chunk_def_out.comp.cinfo.deflate.level, chunk_def.comp.cinfo.deflate.level, "Chunk Test 7. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_COMP), "Chunk Test 7. SDgetchunkinfo");

    /* Write data */
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDwritedata(newsds7, start_dims, NULL, edge_dims, (VOIDP) u16_2data);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 7. Failed to write u16_2data to new chunked data set\n");
        num_errs++;
        goto test8;
      }

    /* read a portion of data back in using SDreaddata*/
    start_dims[0] = 0;
    start_dims[1] = 0;
    edge_dims[0] = 9;
    edge_dims[1] = 4;
    status = SDreaddata(newsds7, start_dims, NULL, edge_dims, (VOIDP) inbuf1_2u16);
    CHECK(status, FAIL, "Chunk Test 7. SDreaddata");
    for (i = 0; i < 9; i++)
      {
        for (j = 0; j < 4; j++)
          {
              if (inbuf1_2u16[i][j] != u16_2data[i][j])
                {
                    fprintf(stderr,"Chunk Test 7. inbuf1_2u16[%d][%d]=%d,",
                            i,j,inbuf1_2u16[i][j]);
                    fprintf(stderr,"u16_cdata[%d][%d]=%d,",
                            i,j,u16_2data[i][j]);
                    fprintf(stderr,"\n");
                    num_errs++;
                }
          }
      }
    /* Get chunk lengths */
    status = SDgetchunkinfo(newsds7, &rchunk_def, &cflags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 7. SDgetchunkinfo failed \n");
        num_errs++;
        goto test8;
      }

    rcdims = rchunk_def.comp.chunk_lengths;

    /* check chunk lengths and see if SDS is compressed and chunked */
    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] 
        || cflags != (HDF_CHUNK | HDF_COMP))
      {
        fprintf(stderr, "Chunk Test 7. SDgetchunkinfo returned wrong values\n");
        num_errs++;
        goto test8;
      }

    /* Close down this SDS*/    
    status = SDendaccess(newsds7);
    CHECK(status, FAIL, "Chunk Test 7. SDendaccess");

    /*
     * Added to test getting compression information for chunked SDS - 
     * bug# 307, 10/10/01 - BMR
     */
    /* Select same SDS again, first get index */
    if ((index = SDnametoindex(fchk,"DataSetChunked_2D_GZIP_1")) == FAIL)
      {
          fprintf(stderr, "Chunk Test 7. SDnametoindex  Failed for GZIP compressed data set\n");
          num_errs++;
          goto test8;
      }

    if ((newsds7 = SDselect(fchk,index)) == FAIL)
      {
          fprintf(stderr, "Chunk Test 7. SDselect Failed to re-select new chunked, GZIP compressed data set\n");
          num_errs++;
          goto test8;
      }

    /*
     * Retrieve and verify the compression info - bug# 307, 10/10/01 - BMR
     * Updated: SDgetcompress replaced by SDgetcompinfo - bugzilla# 130,
     *		4/17/05 -BMR
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds7, &comp_type, &cinfo);
    CHECK(status, FAIL, "Chunk Test 6. SDgetcompinfo");
    VERIFY(comp_type, chunk_def.comp.comp_type, "Chunk Test 6. SDgetcompinfo");
    VERIFY(cinfo.deflate.level, chunk_def.comp.cinfo.deflate.level, "Chunk Test 6. SDgetcompinfo");

    /*
     * Retrieve and verify the compression type
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds7, &comp_type);
    CHECK(status, FAIL, "Chunk Test 7. SDgetcomptype");
    VERIFY(comp_type, chunk_def.comp.comp_type, "Chunk Test 7. SDgetcomptype");

    /* Close down SDS*/    
    status = SDendaccess(newsds7);
    CHECK(status, FAIL, "Chunk Test 7. SDendaccess");


    /* Close down file 'chktst.hdf' */
    status = SDend(fchk);
    CHECK(status, FAIL, "SDend");

    /* ---------------------------------------------------------------
     *  Chunking with NBIT Compression 
     ----------------------------------------------------------------*/

    /*
     * Chunking with NBIT
     */
  test8:

    /* Create file */
    fchk = SDstart(CNBITFILE, DFACC_CREATE);
    CHECK(fchk, FAIL, "Chunk Test 8. SDstart");

    /* Create dataset */
    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds8 = SDcreate(fchk, "Chunked_NBitDataSet", nt, 2, dimsize);
    if(newsds8 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 8. SDcreate Failed to create a new chunked, nbit data set \n");
        num_errs++;
        goto done;
      }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;
    /* Create chunked SDS with NBIT compression.
       chunk is 2x2 which will create 9 chunks.*/
    cdims[0] = chunk_def.nbit.chunk_lengths[0] = 2;
    cdims[1] = chunk_def.nbit.chunk_lengths[1] = 2;
    chunk_def.nbit.start_bit = 6;
    chunk_def.nbit.bit_len   = 7;
    chunk_def.nbit.sign_ext  = FALSE;
    chunk_def.nbit.fill_one  = FALSE;
    c_flags = HDF_CHUNK | HDF_NBIT;
    status = SDsetchunk(newsds8, chunk_def, c_flags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 8. SDsetchunk Failed to create new chunked, NBIT data set\n");
        num_errs++;
        goto done;
      }

    /* write out the data */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds8, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "Chunk Test 8. SDwritedata");

    /* Check getting chunked/compressed flag only with SDgetchunkinfo */
    c_flags_out = 0;
    status = SDgetchunkinfo(newsds8, NULL, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 8. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_NBIT), "Chunk Test 8. SDgetchunkinfo");

    /* Check getting compression info with SDgetchunkinfo */
    c_flags_out = 0;
    HDmemset(&chunk_def_out, 0, sizeof(HDF_CHUNK_DEF));
    status = SDgetchunkinfo(newsds8, &chunk_def_out, &c_flags_out);
    CHECK(status, FAIL, "Chunk Test 8. SDgetchunkinfo");
    VERIFY(c_flags_out, (HDF_CHUNK | HDF_NBIT), "Chunk Test 8. SDgetchunkinfo");
    VERIFY(chunk_def_out.nbit.start_bit, chunk_def.nbit.start_bit, "Chunk Test 8. SDgetchunkinfo");
    VERIFY(chunk_def_out.nbit.bit_len, chunk_def.nbit.bit_len, "Chunk Test 8. SDgetchunkinfo");
    VERIFY(chunk_def_out.nbit.sign_ext, chunk_def.nbit.sign_ext, "Chunk Test 8. SDgetchunkinfo");
    VERIFY(chunk_def_out.nbit.fill_one, chunk_def.nbit.fill_one, "Chunk Test 8. SDgetchunkinfo");

    /* end access to SDS */
    status = SDendaccess(newsds8);
    CHECK(status, FAIL, "Chunk Test 8. SDendaccess");

    /* need to close to flush n-bit info to file */
    status = SDend(fchk);
    CHECK(status, FAIL, "Chunk Test 8. SDend");

    /* open file again */
    fchk = SDstart(CNBITFILE, DFACC_RDWR);
    CHECK(fchk, FAIL, "Chunk Test 8. SDstart (again)");

    /* Select SDS */
    newsds8 = SDselect(fchk, 0);
    if(newsds8 == FAIL) 
      {
        fprintf(stderr, "Chunk Test 8. Failed to select a data set for n-bit access\n");
        num_errs++;
        goto done;
      }

    /* read the n-bit data back in */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds8, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "Chunk Test 8. SDreaddata");

    /* Verify the data */
    for(i = 0; i < 25; i++)
      {
        if((idata[i]&0x7f) != rdata[i]) 
          {
            fprintf(stderr,"Chunk Test 8. Bogus val in loc %d in n-bit dset want %ld got %ld\n",
		    i, (long)idata[i], (long)rdata[i]);
            num_errs++;
          }
      }

    /* Get chunk lengths */
    status = SDgetchunkinfo(newsds8, &rchunk_def, &cflags);
    if(status == FAIL) 
      {
        fprintf(stderr, "Chunk Test 8. SDgetchunkinfo failed \n");
        num_errs++;
        goto done;
      }

    rcdims = rchunk_def.nbit.chunk_lengths;

    /* check chunk lengths and see if SDS is nbit-compressed and chunked */
    if (cdims[0] != rcdims[0] || cdims[1] != rcdims[1] 
        || cflags != (HDF_CHUNK | HDF_NBIT))
      {
        fprintf(stderr, "Chunk Test 8. SDgetchunkinfo returned wrong values\n");
        fprintf(stderr, "Chunk Test 8. cflags =%d \n",(int)cflags);
        fprintf(stderr, "Chunk Test 8. cdims[%d] =%d \n", 0, (int)cdims[0]);
        fprintf(stderr, "Chunk Test 8. cdims[%d] =%d \n", 1, (int)cdims[1]);
        fprintf(stderr, "Chunk Test 8. rcdims[%d] =%d \n", 0, (int)rcdims[0]);
        fprintf(stderr, "Chunk Test 8. rcdims[%d] =%d \n", 1, (int)cdims[1]);
        num_errs++;
        goto done;
      }
    /*
     * Retrieve and verify the compression type
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds8, &comp_type);
    CHECK(status, FAIL, "Chunk Test 8. SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_NBIT, "Chunk Test 8. SDgetcomptype");

    /*
     * Retrieve and verify the compression info - bug# 307, 10/10/01 - BMR
     * Updated: SDgetcompress replaced by SDgetcompinfo - bugzilla# 130,
     *		4/17/05 -BMR
     */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds8, &comp_type, &cinfo);
    CHECK(status, FAIL, "Chunk Test 8. SDgetcompinfo");

    /* Note: the struct nbit in the union HDF_CHUNK_DEF seems like an extra
	thing since comp_info also has nbit, but the HDF_CHUNK_DEF.nbit was
	used to set the compression info so it's also used here to verify */ 
    VERIFY(comp_type, COMP_CODE_NBIT, "Chunk Test 8. SDgetcompinfo");
    VERIFY(cinfo.nbit.sign_ext, chunk_def.nbit.sign_ext, "Chunk Test 8. SDgetcompinfo");
    VERIFY(cinfo.nbit.fill_one, chunk_def.nbit.fill_one, "Chunk Test 8. SDgetcompinfo");
    VERIFY(cinfo.nbit.start_bit, chunk_def.nbit.start_bit, "Chunk Test 8. SDgetcompinfo");
    VERIFY(cinfo.nbit.bit_len, chunk_def.nbit.bit_len, "Chunk Test 8. SDgetcompinfo");
    /* end of test for bug# 307 and bugzilla# 130 */

    /* end access to SDS */
    status = SDendaccess(newsds8);
    CHECK(status, FAIL, "Chunk Test 8. SDendaccess");

    /* close file */
    status = SDend(fchk);
    CHECK(status, FAIL, "Chunk Test 8. SDend");

    if (num_errs == 0)
        PASSED();

  done:
    return num_errs;
} /* test_chunk() */
#endif /* HDF */
