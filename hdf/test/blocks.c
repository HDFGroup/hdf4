#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include "tproto.h"
#define TESTFILE_NAME "t.hdf"

#define BUFSIZE 4096

#ifdef TEST_PC
#define FAR far
#else
#define FAR /* */
#endif

static uint8 FAR outbuf[BUFSIZE],
    FAR inbuf[BUFSIZE];

extern int num_errs;
extern int Verbocity;

void test_hblocks()
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref;
    int16 access, special;
    register int i;
    int32 ret;
    intn errors = 0;

    for (i=0; i<BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);

    MESSAGE(5,printf("Creating a file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5,printf("Write an element and then promote to Linked Blocks\n"););
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1,
                     (uint8 *) "element 1000 1 wrong ",
                      HDstrlen("element 1000 1 wrong ") + 1);
    CHECK(ret, FAIL, "Hputelement");

    aid1 = HLcreate(fid, 1000, 1, 10,10);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hseek(aid1, HDstrlen("element 1000 1 "), DF_START);
    CHECK(ret, FAIL, "Hseek");

    ret = Hwrite(aid1, HDstrlen("correct")+1, (uint8 *) "correct");
    if(ret != (int32)HDstrlen("correct") + 1) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Create a new element as a Linked Block\n"););
    aid1 = HLcreate(fid, 1000, 4, 512, 2);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = HLcreate(fid, 1000, 2, 128, 16);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, HDstrlen("element 1000 2")+1, (uint8 *) "element 1000 2");
    if(ret != (int32)HDstrlen("element 1000 2") + 1) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Verifying data\n"););
    ret = Hgetelement(fid, (uint16) 1000, (uint16) 4, inbuf);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR: Hgetelement returned the wrong length: %d\n", ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf[i] != outbuf[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
           errors++;
         }
       inbuf[i] = '\0';
    }

    aid1 = HLcreate(fid, 1020, 2, 128, 4);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, BUFSIZE, outbuf);
    if(ret != BUFSIZE) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing and re-opening file %s\n", TESTFILE_NAME););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    fid = Hopen(TESTFILE_NAME, DFACC_RDWR, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5,printf("Verifying data\n"););

    aid1 = Hstartread(fid, 1000, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

    ret = Hread(aid1, length, inbuf);
    if(ret != 23) {
      fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", ret);
      errors++;
    }

    if(HDstrcmp((const char *) inbuf, (const char *) "element 1000 1 correct")) {
      fprintf(stderr, "ERROR: Hread returned the wrong data\n");
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: element 1000 1 correct\n");
      errors++;
    }

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(special) {
      fprintf(stderr, "ERROR: Hinquire thinks a non-special element is special\n");
      errors++;
    }

    ret = Hnextread(aid1, DFTAG_WILDCARD, 1151, DF_CURRENT);
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: Hnextread found object with bogus ref\n");
      errors++;
    }

    ret = Hnextread(aid1, 1020, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

    MESSAGE(5,printf("Writing to existing element\n"););
    aid2 = Hstartwrite(fid, 1000, 1, 4);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if(ret != 4) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    /* let's try to write to a read element for fun */
    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: Hwrite allowed write to read access obj\n");
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hendaccess(aid2);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Open a second id for file %s\n", TESTFILE_NAME););
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    CHECK(fid1, FAIL, "Hopen");

    ret = Hnewref(fid1);
    CHECK(ret, FAIL, "Hnewref");

    MESSAGE(5,printf("Closing the files\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");


#ifdef QAK
    HDfreespace((VOIDP)outbuf);
    HDfreespace((VOIDP)inbuf);
#endif
}
