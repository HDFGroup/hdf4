/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
 * This file tests the Special Chunking Element layer of the
 * HDF library.
 *
 * NOTE: No failure conditions are tested yet....
 *
 * -GV
 */

#include "tproto.h"
#include "hchunks.h"

#define TESTFILE_NAME "tchunks.hdf"
#define BUFSIZE 12288

/* Some static buffers */
static uint8  outbuf[BUFSIZE],  inbuf[BUFSIZE];
static uint8  outbuf_1[16] = {0,0,2,3,0,0,6,7,8,9,0,0,12,13,0,0};
static uint8  ui8data[2][3][4] =
{
    {
        {0, 1, 2, 3},
        {10, 11, 12, 13},
        {20, 21, 22, 23}},
    {
        {100, 101, 102, 103},
        {110, 111, 112, 113},
        {120, 121, 122, 123}}};

static    uint8      cptr0[4] = {0,1,4,5};
static    uint8      cptr1[4] = {2,3,6,7};
static    uint8      cptr2[4] = {8,9,12,13};
static    uint8      cptr3[4] = {10,11,14,15};

/*
 * main entry point to tests the Special Chunking layer...
 * -GV
 */
void
test_chunks()
{
    int32       fid, fid1;
    int32       aid, aid1, aid2;
    int32       fileid, length, offset, posn;
    uint16      tag, ref;
    int16       acc_mode, special;
    register int i, j, k;
    int32       x,y;
    int32       ret;
    CHUNK_DEF   chunk[1];
    CHUNK_DEF  *chkptr = NULL;
    int32       dim[5];
    int32      fill_val_len = 1;
    uint8      fill_val = 0;
    int32      nseek = 0;
    intn       errors = 0;

    /* intialize outbufer */
    for (i = 0; i < BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);

    /* allocate space for chunk dimensions */
    if ((chunk[0].pdims = (DIM_DEF *)HDmalloc(5*sizeof(DIM_DEF))) == NULL)
      {
          printf("test_chunks: error allocatin space for chunk dims\n");
          return;
      }

    /* Create file first */
    MESSAGE(5, printf("Creating a file %s\n", TESTFILE_NAME); );
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

#if 0

#endif
    /* First test simple writing of 2-D element with no ghost/partial chunks.
       Set dimension to 4x4 array with 4 chunks 
       where each chunk is 2x2.
       Write out 12 bytes to all 4 chunks with only partial writes
       to the last 2 chunks..
       Write again to last 2 chunks with whole data chunks */
    chunk[0].num_dims   = 2; /* 2-D */
    chunk[0].chunk_size = 4; /* 2x2 = 4 bytes */
    chunk[0].chunk_flag = 0; /* nothing set */

    chunk[0].pdims[0].dim_length   = 4;
    chunk[0].pdims[0].chunk_length = 2;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 4;
    chunk[0].pdims[1].chunk_length = 2;
    chunk[0].pdims[1].distrib_type = 1;

    MESSAGE(5, printf("Create a new element as a 2-D chunked element\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 2, 1, 16, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 12 bytes out */
    ret = Hwrite(aid1, 12, outbuf);
    if (ret != 12)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote first 12 bytes to 2-D chunked element to file\n"); );
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Open 2-D chunked element again for writing\n"); );

    /* Open file for writing last 2 chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start write access   tag,  ref */
    aid1 = Hstartwrite(fid, 1020, 2, 16);
    CHECK(aid1, FAIL, "Hstartwrite");

    /* Try writing to last chunk in the element */
    dim[0] = 1;
    dim[1] = 1;
    ret = HMCwriteChunk(aid1, dim, cptr3);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote to 4th chunk(4of4 chunks) in file\n"););

    /* Try writing to 2nd to last chunk in the element */
    dim[0] = 0;
    dim[1] = 1;
    ret = HMCwriteChunk(aid1, dim, cptr2);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote to 3 chunk (3of4 chunks) in file\n"););
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Open 2-D chunked element again for reading \n"); );
    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 2);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }
    /* Check values from Hinquire */
    if (ref != 2 || length != 16)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);
    VERIFY(ret, 16, "Hread");
    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying 16 bytes of data\n"); );
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Now create a new chunked 2-D element with same parameters
       before but write to 2 chunks of element using whole chunks.
       The rest of he data should contain fill values.
     */

    /* Open file for writing again */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create another new element as a 2-D chunked element\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 3, 1, 16, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* Try writing to 2 chunk in the element */
    dim[0] = 0;
    dim[1] = 1;
    ret = HMCwriteChunk(aid1, dim, cptr2);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote to 3 chunk (3of4) in file\n"); );

    /* Try writing to 1 chunk in the element */
    dim[0] = 1;
    dim[1] = 0;
    ret = HMCwriteChunk(aid1, dim, cptr1);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote to 2nd chunk (2of4 chunks) in file\n"); );

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"); );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Now reopen and read back 16 bytes */
    MESSAGE(5, printf("Open 2-D chunked element again for reading \n"); );
    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 3);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    /* Check values from Hinquire */
    if ( ref != 3 || length != 16)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);
    VERIFY(ret, 16, "Hread");
    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data, check against 'outbuf_1'  
       some of the data should be filled with fill value of 0 */
    MESSAGE(5, printf("Verifying 16 bytes of data, there will be some fill values\n"); );
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf_1[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf_1[i], inbuf[i]);
                errors++;
            }
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"); );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");


    /* Create a new element but now there will be partial chunks
       because chunk lengths are not an even multiple of dimesion lengths.
       Set dimension to 4x4 array with 4 chunks where each chunk is 3x2 = 6 bytes.
       Real data size is 16 bytes, size with chunks is 
       6 bytes x 4 chunks = 24 bytes */
    chunk[0].num_dims   = 2;
    chunk[0].chunk_size = 6; /* 3x2 = 6 bytes */

    chunk[0].pdims[0].dim_length   = 4;
    chunk[0].pdims[0].chunk_length = 3;  /* made this 3 */
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 4;
    chunk[0].pdims[1].chunk_length = 2;
    chunk[0].pdims[1].distrib_type = 1;

    /* Open file for writing odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create another new element as a 2-D chunked element\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 5, 1, 16, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 16 bytes out */
    ret = Hwrite(aid1, 16, outbuf);
    VERIFY(ret, 16, "Hwrite");
    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote to 4of4 chunks (16 bytes) in file\n"););
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"); );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Open 2-D chunked element again for reading\n"); );

    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 5);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    /* Check values from Hinquire */
    if ( ref != 5 || length != 16)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);
    VERIFY(ret, 16, "Hread");
    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying 16 bytes of data\n"); );
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
          inbuf[i] = '\0';
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"); );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Now create 3-D chunked element with no partial chunks.
       Write to only part of the chunked element. The rest
       should be filled with fill values.
       Set dimension to 4x6x8 array with 8 chunks 
       where each chunk is 2x3x4= 24 bytes , total data size 192 bytes */
    chunk[0].num_dims   = 3;
    chunk[0].chunk_size = 24; /* 2x3x4 bytes */

    chunk[0].pdims[0].dim_length   = 4;
    chunk[0].pdims[0].chunk_length = 2;  
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 6;
    chunk[0].pdims[1].chunk_length = 3;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length   = 8;
    chunk[0].pdims[2].chunk_length = 4;
    chunk[0].pdims[2].distrib_type = 1;

    /* Open file for writing last odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create another new element as a 3-D chunked element(192 bytes)\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        2,chunk[0].pdims[2].dim_length, 
                        2,chunk[0].pdims[2].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 6, 1, 192, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write only 112 bytes out */
    ret = Hwrite(aid1, 112, outbuf);
    VERIFY(ret, 112, "Hwrite");
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote 112of192 bytes to 3-D chunked element \n"); );
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Open 3-D chunked element again for reading\n"); );
    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 6);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    /* Check values from Hinquire */
    if ( ref != 6 || length != 192)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* read back in buffer  */
    ret = Hread(aid1, 112, inbuf);
    VERIFY(ret, 112, "Hread");
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying first 112 bytes data\n"); );
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
          inbuf[i] = '\0';
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Now create another 3-D chunked element with partial chunks.
       Write to part of element, rest is filled with fill value.
       Set dimension to 4x6x8 array with 8 chunks , real data 192 bytes
       where each chunk is 3x4x5= 60 bytes , 
       data size with chunks is 60 bytes x 8 chunks = 480 bytes */
    chunk[0].num_dims   = 3;
    chunk[0].chunk_size = 60; /* 3x4x5 = 60 bytes */

    chunk[0].pdims[0].dim_length   = 4;
    chunk[0].pdims[0].chunk_length = 3;  
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 6;
    chunk[0].pdims[1].chunk_length = 4;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length   = 8;
    chunk[0].pdims[2].chunk_length = 5;
    chunk[0].pdims[2].distrib_type = 1;

    /* set fill value to 1 */
    fill_val = 1;

    /* Open file for writing last odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create another new element as a 3-D chunked element(192bytes)\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        2,chunk[0].pdims[2].dim_length, 
                        2,chunk[0].pdims[2].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 7, 1, 192, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* Set max chunks to cache to 3x4 = 12 chunks */
    MESSAGE(5, printf("Set max # of chunks to cache for chunked element to 12 \n"); );
    ret = HMCsetMaxcache(aid1, 12, 0);
    VERIFY(ret, 12, "HMCsetMaxcache");
    
    /* write 112 bytes out */
    ret = Hwrite(aid1, 112, outbuf);
    VERIFY(ret, 112, "Hwrite");
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote 112of192 bytes to 3-D chunked element \n"); );

    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Open 3-D chunked element again for reading\n"); );
    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 7);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    /* Check values from Hinquire */
    if ( ref != 7 || length != 192)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* read back in buffer  */
    ret = Hread(aid1, 112, inbuf);
    VERIFY(ret, 112, "Hread");
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n"););
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
          inbuf[i] = '\0';
      }

    /* read back 20 bytes now, they should be filled with fill values */
    MESSAGE(5, printf("reading some more data\n"););
    ret = Hread(aid1, 20, inbuf);
    VERIFY(ret, 20, "Hread");
    if (ret != 20)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data, should be full of fill values\n"); );
    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != fill_val)
            {
                printf("Wrong data at %d, out %d in %d\n", i, fill_val, inbuf[i]);
                errors++;
            }
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"); );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");


    /* Create 4-D element with partial chunks.
       Write only half the data out(5,000 bytes)
       Set dimension to 10x10x10x10 array  real data 10,000 bytes .
       120 chunks whit chunks of 2x3x4x5 = 120 bytes,
       data size with chunks is 120 bytes x 120 chunks = 14,400 bytes */
    chunk[0].num_dims   = 4;
    chunk[0].chunk_size = 120;

    chunk[0].pdims[0].dim_length   = 10;
    chunk[0].pdims[0].chunk_length = 2;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 10;
    chunk[0].pdims[1].chunk_length = 3;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length   = 10;
    chunk[0].pdims[2].chunk_length = 4;
    chunk[0].pdims[2].distrib_type = 1;

    chunk[0].pdims[3].dim_length   = 10;
    chunk[0].pdims[3].chunk_length = 5;
    chunk[0].pdims[3].distrib_type = 1;

    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create another new element as a 4-D chunked element\n"););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        0,chunk[0].pdims[0].dim_length, 
                        0,chunk[0].pdims[0].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        1,chunk[0].pdims[1].dim_length, 
                        1,chunk[0].pdims[1].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        2,chunk[0].pdims[2].dim_length, 
                        2,chunk[0].pdims[2].chunk_length););
    MESSAGE(5, printf(" dim_length[%d]=%d, chunk_length[%d]=%d \n",
                        3,chunk[0].pdims[3].dim_length, 
                        3,chunk[0].pdims[3].chunk_length););

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 9, 1, 10000, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 5000 bytes out */
    ret = Hwrite(aid1, 5000, outbuf);
    VERIFY(ret, 5000, "Hwrite");
    if (ret != 5000)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote 5000of10000 bytes to 4-D chunked element \n"); );
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Open 4-D chunked element again for reading\n"); );

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 9);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }

    /* Check values from Hinquire */
    if ( ref != 9 || length != 10000)
      {
          fprintf(stderr, "ERROR: Hinquire does not return the correct values \n");
          fprintf(stderr, "       tag =%d, ref=%d, length=%d \n",tag,ref,length);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying 5000 bytes of data\n"); );

    /* read back in buffer  */
    ret = Hread(aid1, 5000, inbuf);
    VERIFY(ret, 5000, "Hread");
    if (ret != 5000)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
            {
                printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                errors++;
            }
      }

    /* verify some more data */
    MESSAGE(5, printf("seek to 7000 bytes in element and read 1000 bytes \n"); );

    /* seek past initial write  buffer  */
    ret = Hseek(aid1, 7000, 0);
    VERIFY(ret, 0, "Hseek");

    /* read back in buffer  */
    ret = Hread(aid1, 1000, inbuf);
    VERIFY(ret, 1000, "Hread");
    if (ret != 1000)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data, should be fill values */
    MESSAGE(5, printf("Verifying 1000 bytes of data, should be fill values\n"); );

    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != fill_val)
            {
                printf("Wrong data at %d, out %d in %d\n", i, fill_val, inbuf[i]);
                errors++;
            }
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the file\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

#if 0

#endif

#if 0

    /* The rest of the tests here are commented out
       They are some extra high order tests to replicate
       some test done on EOS-DEM data  -GV.....*/

    /* Set dimension to 12000x12000 array with 2,500 chunks 
     whith chunk of 240x240 = 57,600 bytes,  bytes */
    chunk[0].num_dims   = 2;
    chunk[0].chunk_size = 57600;

    chunk[0].pdims[0].dim_length   = 12000;
    chunk[0].pdims[0].chunk_length = 240;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length   = 12000;
    chunk[0].pdims[1].chunk_length = 240;
    chunk[0].pdims[1].distrib_type = 1;

#if 0
    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");
#endif

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 10, 1, 144000000, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    for (j = 0; j < 12000; j++)
      {
          printf("%d,",j);
          /* write 12000 bytes out */
          ret = Hwrite(aid1, 12000, outbuf);
          if (ret != 12000)
            {
                fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
                errors++;
            }
      }
    printf("\n");

    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");


    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* start read access   tag,  ref */
    aid1 = Hstartread(fid, 1020, 10);
    CHECK(aid1, FAIL, "Hstartread");

    /* inquire about element */
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);

    CHECK(ret, FAIL, "Hinquire");
    if (!special)
      {
          fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
                  __LINE__);
          errors++;
      }
#endif

#if 0
    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
            );

    for (j = 0; j < 12000; j++)
      {

          if ((j % 1000) == 0)
            {
                printf("read %d,",j);
                /* read back in buffer  */
                ret = Hread(aid1, 12000, inbuf);

                if (ret != 12000)
                  {
                      fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
                      errors++;
                  }

                for (i = 0; i < ret; i++)
                  {
                      if (inbuf[i] != outbuf[i])
                        {
                            printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
                            errors++;
                        }
                  }
            }
          else
            {
                /* seek to next buffer  */
                ret = Hseek(aid1, 12000, 1);
                VERIFY(ret, 0, "Hseek");
            }
      }
#endif

#if 0

    /* Mimic read pattern of EOS-DEM test  behaviour */
    x = 1000;
    y = 1000;
    nseek = (12000 * y) + x;
    printf("seek to initial byte postion = %d \n",nseek);
    /* seek to next buffer  */
    ret = Hseek(aid1, nseek, 0);
    VERIFY(ret, 0, "Hseek");

    for (j = 0; j < 11; j++)
      {
          printf("read 2000x2000 square at x=%d,y=%d\n",x,y);

          for (k = 0; k < 2000; k++)
            {
                /* read back in buffer  */
                ret = Hread(aid1, 2000, inbuf);

                if (ret != 2000)
                  {
                      fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
                      errors++;
                  }

                /* seek to next buffer  */
                ret = Hseek(aid1, 10000, 1);
                VERIFY(ret, 0, "Hseek");
            }

          x = x + 100;
          y = y + 1000;
          nseek = (12000 * y) + x;
          printf("seek to next byte postion = %d\n",nseek);
          /* seek to next buffer  */
          ret = Hseek(aid1, nseek, 0);
          VERIFY(ret, 0, "Hseek");
      }

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");
#endif

    if (chunk[0].pdims != NULL)
        HDfree(chunk[0].pdims);

    num_errs += errors;     /* increment global error count */
}
