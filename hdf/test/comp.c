#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

#include "tproto.h"
#define TESTFILE_NAME "tcomp.hdf"

#define BUFSIZE 4096

#ifdef TEST_PC
#define FAR far
#else
#define FAR /* */
#endif

static uint8 FAR outbuf[BUFSIZE],
    FAR inbuf[BUFSIZE];

static uint8 FAR outbuf2[BUFSIZE],
    FAR inbuf2[BUFSIZE];

extern int num_errs;
extern int Verbocity;

void test_comp()
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref, ref1, ref2, ref3;
    int16 access, special;
    register int i,j;
    int32 ret;
    intn errors = 0;

    for (i=0; i<BUFSIZE; i++)
        outbuf[i] = (char) (i % 256);
    for(i=0; i<BUFSIZE; i++)
	for(j=0; j<130 && i<BUFSIZE; j++,i++)
	    outbuf[i]=i;

    MESSAGE(5,printf("Creating a file %s\n", TESTFILE_NAME););
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

#ifdef QAK
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
#endif

    ref1 = Hnewref(fid);
    CHECK(ref1, 0, "Hnewref");

    MESSAGE(5,printf("Create a new element as a compressed element\n"););
    aid1 = HCcreate(fid, 1000, ref1, COMP_MODEL_STDIO, COMP_CODE_RLE);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR(%d): Hwrite returned the wrong length: %d\n", __LINE__,ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ref2 = Hnewref(fid);
    CHECK(ref2, 0, "Hnewref");

    aid1 = HCcreate(fid, 1000, ref2, COMP_MODEL_STDIO, COMP_CODE_RLE);
    CHECK(aid1, FAIL, "HCcreate");

    ret = Hwrite(aid1, BUFSIZE/2, outbuf2);
    if(ret != BUFSIZE/2) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      HEprint(stdout,0);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Verifying data\n"););
    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref1, inbuf);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
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

    ret = Hgetelement(fid, (uint16) 1000, (uint16) ref2, inbuf2);
    if(ret != BUFSIZE/2) {
      HEprint(stderr,0);
      fprintf(stderr, "ERROR: Hgetelement returned the wrong length: %d\n", ret);
      errors++;
    }

    for (i=0; i < ret; i++) {
       if (inbuf2[i] != outbuf2[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf2[i], inbuf2[i]);
           errors++;
         }
       inbuf2[i] = '\0';
    }

    ref3 = Hnewref(fid);
    CHECK(ref3, 0, "Hnewref");

    aid1 = HCcreate(fid, 1020, ref3, COMP_MODEL_STDIO, COMP_CODE_RLE);
    CHECK(aid1, FAIL, "HCcreate");

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

    MESSAGE(5,printf("Verifying data\n"););

#ifdef QAK
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

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");
#else

    ret = Hstartread(fid, 1000, ref1);
    CHECK(ret, FAIL, "Hstartread");
#endif

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");
    if(!special) {
      fprintf(stderr, "ERROR: Hinquire does not think element is special line %d\n",
              __LINE__);
      errors++;
    }

#ifdef QAK
    MESSAGE(5,printf("Writing to existing element\n"););
    aid2 = Hstartwrite(fid, 1000, 1, ref1);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if(ret != 4) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }
#endif

    /* let's try to write to a read element for fun */
    ret = Hwrite(aid1, 4, (uint8 *) "ABCD");
    if(ret != FAIL) {
      fprintf(stderr, "ERROR: Hwrite allowed write to read access obj\n");
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    MESSAGE(5,printf("Closing the files\n"););
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");
}

