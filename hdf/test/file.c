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

/* $Id$ */

/*
   * Hopen
   ** Create a file.
   ** Open an existing file.
   *** Normally.
   *** Read-only file with DFACC_WRITE.
   ** Open non-existent file.
   *** With DFACC_READ.
   *** With DFACC_WRITE.
   ** Create an illegal file.
   ** Open the same file twice.
   *** First with DFACC_WRITE then with DFACC_READ.
   *** First with DFACC_WRITE then with DFACC_WRITE.
   *** First with DFACC_READ and then with DFACC_WRITE.
   *** First with DFACC_<any> and then with DFACC_CREATE.
   ** Open more files than there is slots.

   * Hclose
   ** Close a proper file.
   ** Close with an illegal file id.
   *** Random file id.
   *** Correct tag but bad slot.

   * Hstartread
   ** Normal.
   ** With illegal file id.
   ** With illegal tag/ref.
   ** With wildcard.
   ** Open more access elements than there is space.

 */

#include "tproto.h"
#define TESTFILE_NAME "t.hdf"
#define BUF_SIZE        4096

static uint8  outbuf[BUF_SIZE],  inbuf[BUF_SIZE];

void
test_hfile(void)
{
    int32       fid, fid1;
    int32       aid1, aid2;
    int32       fileid, length, offset, posn;
    uint16      tag, ref;
    int16       acc_mode, special;
    int32       ret;
    int         i;
    intn        errors = 0;
    intn        ret_bool;

    for (i = 0; i < BUF_SIZE; i++)
        outbuf[i] = (char) (i % 256);

    MESSAGE(5, printf("Creating a file %s\n", TESTFILE_NAME);
        );
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    ret_bool = (intn) Hishdf(TESTFILE_NAME);
    CHECK_VOID(ret_bool, FALSE, "Hishdf");

    ret = (int32)Hnewref(fid);
    CHECK_VOID(ret, FAIL, "Hnewref");

    MESSAGE(5, printf("Reading / Writing to file\n");
        );
    ret = Hputelement(fid, (uint16) 100, 1,
                  (const uint8 *) "testing 100 1", (int32)HDstrlen("testing 100 1") + 1);
    CHECK_VOID(ret, FAIL, "Hputelement");

    ret = Hputelement(fid, (uint16) 100, (uint16) 4, outbuf, 2000);
    CHECK_VOID(ret, FAIL, "Hputelement");

    ret = (int32)Hnewref(fid);
    CHECK_VOID(ret, FAIL, "Hnewref");

    ret = Hputelement(fid, (uint16) 103, (uint16) 2,
                  (const uint8 *) "element 103 2", (int32)HDstrlen("element 103 2") + 1);
    CHECK_VOID(ret, FAIL, "Hputlement");

    ret = Hgetelement(fid, (uint16) 100, (uint16) 4, inbuf);
    if (ret != 2000)
      {
          fprintf(stderr, "Line %d: Hgetelement returned wrong count: %d\n", (int)__LINE__, (int) ret);
          errors++;
      }

    for (i = 0; i < ret; i++)
      {
          if (inbuf[i] != outbuf[i])
              printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
          inbuf[i] = '\0';
      }

    ret = Hputelement(fid, 102, 2, outbuf, BUF_SIZE);
    CHECK_VOID(ret, FAIL, "Hputlement");

    ret = Hclose(fid);
    CHECK_VOID(ret, FAIL, "Hclose");

    MESSAGE(5, printf("Closing and re-opening file %s\n", TESTFILE_NAME);
        );
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK_VOID(fid, FAIL, "Hopen");

    ret = (int32)Hnewref(fid);
    CHECK_VOID(ret, FAIL, "Hnewref");

    aid1 = Hstartread(fid, 100, 1);
    CHECK_VOID(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK_VOID(ret, FAIL, "Hinquire");

    MESSAGE(5, printf("Verifying data\n\n");
        );
    ret = Hread(aid1, length, inbuf);
    if (ret != 14)
      {
          fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", (int) ret);
          errors++;
      }

    if (HDstrcmp((const char *) inbuf, (const char *) "testing 100 1"))
      {
          fprintf(stderr, "ERROR: Hread returned the wrong data\n");
          fprintf(stderr, "\t       Is: %s\n", (char *) inbuf);
          fprintf(stderr, "\tShould be: testing 100 1\n");
          errors++;
      }

    ret = (int32)Hnewref(fid);
    CHECK_VOID(ret, FAIL, "Hnewref");

    MESSAGE(5, printf("Testing a number of searching schemes\n");
        );
    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    CHECK_VOID(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK_VOID(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    if (ret != FAIL)
      {
          fprintf(stderr, "ERROR: Found a non-existant element at line %d\n",
                  __LINE__);
          errors++;
      }

    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    CHECK_VOID(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK_VOID(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, DFTAG_WILDCARD, 3, DF_CURRENT);
    if (ret != FAIL)
      {
          fprintf(stderr, "ERROR: Found a non-existant element at line %d\n",
                  __LINE__);
          errors++;
      }

    ret = Hnextread(aid1, DFTAG_WILDCARD, 2, DF_CURRENT);
    CHECK_VOID(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                   &acc_mode, &special);
    CHECK_VOID(ret, FAIL, "Hinquire");

    aid2 = Hstartwrite(fid, 100, 1, 4);
    if (aid2 == FAIL)
      {
          fprintf(stderr, "ERROR: was not allowed to startwrite on existing object\n");
          errors++;
      }

    ret = Hwrite(aid1, 4, "ABCD");
    if (ret != FAIL)
      {
          fprintf(stderr, "ERROR: was allowed to write to read access object\n");
          errors++;
      }

    ret = Hendaccess(aid1);
    CHECK_VOID(ret, FAIL, "Hendaccess");

    ret = Hendaccess(aid2);
    CHECK_VOID(ret, FAIL, "Hendaccess");

    MESSAGE(5, printf("Attempting to gain multiple access to file (is allowed)\n");
        );
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    if (fid1 == FAIL)
      {
          fprintf(stderr, "ERROR: Failed to have two concurrent access to file\n");
          errors++;
      }

    ret = (int32)Hnewref(fid1);
    CHECK_VOID(ret, FAIL, "Hnewref");

    ret = Hclose(fid);
    CHECK_VOID(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK_VOID(ret, FAIL, "Hclose");

    ret_bool = (intn) Hishdf(TESTFILE_NAME);
    CHECK_VOID(ret_bool, FALSE, "Hishdf");

    ret_bool = (intn) Hishdf(__FILE__);
    CHECK_VOID(ret_bool, TRUE, "Hishdf");

    ret_bool = (intn) Hishdf("qqqqqqqq.qqq");   /* I sure hope it isn't there */
    CHECK_VOID(ret, TRUE, "Hishdf");

}
