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
 * main entry point to tests
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

    MESSAGE(5, printf("Creating a file %s\n", TESTFILE_NAME);
        );
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    MESSAGE(5, printf("Create a new element as a chunked element\n");
        );
#if 0

#endif

    /* Set dimension to 4x4 array with 4 chunks 
       where each chunk is 2x2 */
    chunk[0].num_dims = 2;
    chunk[0].chunk_size = 4;
    chunk[0].chunk_flag = 0; /* nothing set */

    chunk[0].pdims[0].dim_length = 4;
    chunk[0].pdims[0].chunk_length = 2;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 4;
    chunk[0].pdims[1].chunk_length = 2;
    chunk[0].pdims[1].distrib_type = 1;

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

    MESSAGE(5, printf("Wrote first 2of4 chunks to file\n");
        );
    /* end access */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

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

    MESSAGE(5, printf("Wrote 4of4 chunks to file\n");
        );

    /* Try writing to 2nd to last chunk in the element */
    dim[0] = 0;
    dim[1] = 1;
    ret = HMCwriteChunk(aid1, dim, cptr2);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote 3of4 chunks to file\n");
        );
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

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);

    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
        );

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

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");



    /* Open file for writing again */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 3, 1, 16, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* Try writing to 2 chunk in the element */
    dim[0] = 0;
    dim[1] = 1;
    ret = HMCwriteChunk(aid1, dim, cptr2);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote 3of4 chunks to file\n");
        );

    /* Try writing to 1 chunk in the element */
    dim[0] = 1;
    dim[1] = 0;
    ret = HMCwriteChunk(aid1, dim, cptr1);
    CHECK(ret, FAIL, "HMCwriteChunk");

    MESSAGE(5, printf("Wrote 2of4 chunks to file\n");
        );

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Now reopen and read back 16 bytes */
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

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);

    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
        );

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

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");


    /* Set dimension to 4x4 array with 4 chunks 
       where each chunk is 3x2 = 6 bytes */
    chunk[0].num_dims = 2;
    chunk[0].chunk_size = 6;

    chunk[0].pdims[0].dim_length = 4;
    chunk[0].pdims[0].chunk_length = 3;  /* made this 3 */
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 4;
    chunk[0].pdims[1].chunk_length = 2;
    chunk[0].pdims[1].distrib_type = 1;

    /* Open file for writing last odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 5, 1, 16, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 16 bytes out */
    ret = Hwrite(aid1, 16, outbuf);
    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote first 4of4 chunks to file\n");
        );
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
    aid1 = Hstartread(fid, 1020, 5);
    CHECK(aid1, FAIL, "Hstartread");

    /* read back in buffer  */
    ret = Hread(aid1, 16, inbuf);

    if (ret != 16)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
        );

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

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Set dimension to 4x6x8 array with 8 chunks 
       where each chunk is 2x3x4= 24 bytes , total 192 bytes */
    chunk[0].num_dims = 3;
    chunk[0].chunk_size = 24; /* 2x3x4 */

    chunk[0].pdims[0].dim_length = 4;
    chunk[0].pdims[0].chunk_length = 2;  
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 6;
    chunk[0].pdims[1].chunk_length = 3;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length = 8;
    chunk[0].pdims[2].chunk_length = 4;
    chunk[0].pdims[2].distrib_type = 1;

    /* Open file for writing last odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 6, 1, 192, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 112 bytes out */
    ret = Hwrite(aid1, 112, outbuf);
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote first 4of4 chunks to file\n");
        );
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
    aid1 = Hstartread(fid, 1020, 6);
    CHECK(aid1, FAIL, "Hstartread");

    /* read back in buffer  */
    ret = Hread(aid1, 112, inbuf);

    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
        );

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

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Set dimension to 4x6x8 array with 8 chunks 
       where each chunk is 3x4x5= 60 bytes , 
       chunk total 480 bytes , real data 192 bytes */
    chunk[0].num_dims = 3;
    chunk[0].chunk_size = 60; /* 3x4x5 */

    chunk[0].pdims[0].dim_length = 4;
    chunk[0].pdims[0].chunk_length = 3;  
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 6;
    chunk[0].pdims[1].chunk_length = 4;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length = 8;
    chunk[0].pdims[2].chunk_length = 5;
    chunk[0].pdims[2].distrib_type = 1;

    /* set fill value to 1 */
    fill_val = 1;

    /* Open file for writing last odd size chunks now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 7, 1, 192, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 112 bytes out */
    ret = Hwrite(aid1, 112, outbuf);
    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    MESSAGE(5, printf("Wrote first 4of4 chunks to file\n");
        );
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
    aid1 = Hstartread(fid, 1020, 7);
    CHECK(aid1, FAIL, "Hstartread");

    /* read back in buffer  */
    ret = Hread(aid1, 112, inbuf);

    if (ret != 112)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
        );

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
    ret = Hread(aid1, 20, inbuf);

    if (ret != 20)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    /* verify the data */
    MESSAGE(5, printf("Verifying data, should be full of fill values\n");
        );

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

    MESSAGE(5, printf("Closing the files\n");
        );
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");


    /* Set dimension to 10x10x10x10 array with 120 chunks 
     whith chunk of 2x3x4x5 = 120 bytes, 10,000 bytes */
    chunk[0].num_dims = 4;
    chunk[0].chunk_size = 120;

    chunk[0].pdims[0].dim_length = 10;
    chunk[0].pdims[0].chunk_length = 2;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 10;
    chunk[0].pdims[1].chunk_length = 3;
    chunk[0].pdims[1].distrib_type = 1;

    chunk[0].pdims[2].dim_length = 10;
    chunk[0].pdims[2].chunk_length = 4;
    chunk[0].pdims[2].distrib_type = 1;

    chunk[0].pdims[3].dim_length = 10;
    chunk[0].pdims[3].chunk_length = 5;
    chunk[0].pdims[3].distrib_type = 1;

    /* Open file for reading now */
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Create element     tag, ref,  nlevels, data_len, chunk array */
    aid1 = HMCcreate(fid, 1020, 9, 1, 10000, fill_val_len, &fill_val, (CHUNK_DEF *)chunk);
    CHECK(aid1, FAIL, "HMCcreate");

    /* write 10000 bytes out */
    ret = Hwrite(aid1, 10000, outbuf);
    if (ret != 10000)
      {
          fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", (int) ret);
          errors++;
      }

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

    /* verify the data */
    MESSAGE(5, printf("Verifying data\n");
            );

    /* read back in buffer  */
    ret = Hread(aid1, 10000, inbuf);
    
    if (ret != 10000)
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

    /* end access and close file */
    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Closing the files\n");
        );
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
    chunk[0].num_dims = 2;
    chunk[0].chunk_size = 57600;

    chunk[0].pdims[0].dim_length = 12000;
    chunk[0].pdims[0].chunk_length = 240;
    chunk[0].pdims[0].distrib_type = 1;

    chunk[0].pdims[1].dim_length = 12000;
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
