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

#ifdef TEST_PC
#define FAR far
#else
#define FAR /* */
#endif

static uint8 FAR outbuf[BUF_SIZE],
    FAR inbuf[BUF_SIZE];

extern int num_errs;
extern int Verbocity;


void test_hfile()
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref;
    int16 access, special;
    int32 ret;
    int i;
    intn errors = 0;
    bool ret_bool;
#ifdef QAK
    uint8 *outbuf, *inbuf;

    outbuf=HDgetspace(BUF_SIZE);
    inbuf=HDgetspace(BUF_SIZE);
	if(!outbuf || !inbuf) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
#endif

    for (i=0; i<BUF_SIZE; i++)
        outbuf[i] = (char) (i % 256);

    MESSAGE(5,printf("Creating a file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    ret_bool = (bool)Hishdf(TESTFILE_NAME);
    CHECK(ret_bool, FALSE, "Hishdf");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5,printf("Reading / Writing to file\n"););
    ret = Hputelement(fid, (uint16) 100, 1,
                      (uint8 *) "testing 100 1", HDstrlen("testing 100 1")+1);
    CHECK(ret, FAIL, "Hputelement");

    ret = Hputelement(fid, (uint16) 100, (uint16) 4, outbuf, 2000);
    CHECK(ret, FAIL, "Hputelement");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hputelement(fid, (uint16) 103, (uint16) 2, 
                      (uint8 *) "element 103 2", HDstrlen("element 103 2")+1);
    CHECK(ret, FAIL, "Hputlement");

    ret = Hgetelement(fid, (uint16) 100, (uint16) 4, inbuf);
    if(ret != 2000) {
      fprintf(stderr, "Hgetelement returned wrong count: %d\n", ret);
      errors++;
    }

    for (i=0; i<ret; i++) {
       if (inbuf[i] != outbuf[i])
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
       inbuf[i] = '\0';
    }

    ret = Hputelement(fid, 102, 2, outbuf, BUF_SIZE);
    CHECK(ret, FAIL, "Hputlement");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    MESSAGE(5,printf("Closing and re-opening file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = Hstartread(fid, 100, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    MESSAGE(5,printf("Verifying data\n\n"););
    ret = Hread(aid1, length, inbuf);
    if(ret != 14) {
      fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", ret);
      errors++;
    }

    if(HDstrcmp((const char *) inbuf, (const char *) "testing 100 1")) {
      fprintf(stderr, "ERROR: Hread returned the wrong data\n");
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: testing 100 1\n");
      errors++;
    }

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5,printf("Testing a number of searching schemes\n"););
    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: Found a non-existant element at line %d\n", 
              __LINE__);
      errors++;
    }

    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, DFTAG_WILDCARD, 3, DF_CURRENT);
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: Found a non-existant element at line %d\n", 
              __LINE__);
      errors++;
    }

    ret = Hnextread(aid1, DFTAG_WILDCARD, 2, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    aid2 = Hstartwrite(fid, 100, 1, 4);
    if(aid2 == FAIL) {
      fprintf(stderr, "ERROR: was not allowed to startwrite on existing object\n");
      errors++;
    }

    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: was allowed to write to read access object\n");
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hendaccess(aid2);
    CHECK(ret, FAIL, "Hendaccess");
    
    MESSAGE(5,printf("Attempting to gain multiple access to file (is allowed)\n"););
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    if(fid1 == FAIL) {
      fprintf(stderr, "ERROR: Failed to have two concurrent access to file\n");
      errors++;
    }

    ret = Hnewref(fid1);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");

    ret_bool = (bool)Hishdf(TESTFILE_NAME);
    CHECK(ret_bool, FALSE, "Hishdf");

    ret_bool = (bool)Hishdf(__FILE__);
    CHECK(ret_bool, TRUE, "Hishdf");

    ret_bool = (bool)Hishdf("qqqqqqqq.qqq");  /* I sure hope it isn't there */
    CHECK(ret, TRUE, "Hishdf");

#ifdef QAK
    HDfreespace((VOIDP)outbuf);
    HDfreespace((VOIDP)inbuf);
#endif
}
