#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1993/08/28 00:12:42  georgev
Added VOIDP casts to HDfreespace().

 * Revision 1.1  1993/08/16  21:47:51  koziol
 * Wrapped in changes for final, working version on the PC.
 *
 * Revision 1.2  1993/04/19  23:03:43  koziol
 * General Code Cleanup to reduce/remove compilation warnings on PC
 *
 * Revision 1.1  1993/04/15  20:00:11  koziol
 * Re-named the new tests for MS-DOS compatibility
 *
 * Revision 1.3  1993/02/16  20:51:13  chouck
 * Went back to using -ansi so needed to fix a few casting problems
 *
 * Revision 1.2  1993/01/27  22:41:24  briand
 * Fixed problem with compiling on RS6000.
 *
 * Revision 1.1  1993/01/27  22:04:28  briand
 * Converted test files to work with master test program: testhdf
 *
 * Revision 1.7  1992/07/27  18:40:12  dilg
 * Changed DFACC_ALL to DFACC_RDWR in appropriate places to conform to new
 * handling of access modes by Hopen().
 *
 * Revision 1.6  1992/06/22  23:04:42  chouck
 * Removed calls to fork()
 *
 * Revision 1.5  1992/06/02  15:40:41  chouck
 * Cleaned up output
 *
 * Revision 1.4  1992/05/31  19:08:09  mfolk
 * Added uint8 * cast in line 91 for Convex.
 *
 * Revision 1.3  1992/05/29  22:30:15  mfolk
 * Added some casts to avoid warnings when compiling on Convex.
 *
 * Revision 1.2  1992/05/14  15:25:31  chouck
 * Fixed calls to Hinquire()
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/

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
#ifdef QAK
    uint8 *outbuf, *inbuf;

    outbuf=HDgetspace(BUFSIZE);
    inbuf=HDgetspace(BUFSIZE);
	if(!outbuf || !inbuf) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
#endif

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
    CHECK(ret, FAIL, "HLcreate");

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
