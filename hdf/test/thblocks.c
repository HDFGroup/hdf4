#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.6  1992/06/22 23:04:42  chouck
Removed calls to fork()

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

#include "hdf.h"
#define TESTFILE_NAME "t.hdf"
uint8 outbuf[4096], inbuf[4096];

#define CHECK(ret, val, where) \
{if(ret == val) { fprintf(stderr, "%s failed, line %d, code %d\n", \
                          where, __LINE__, ret); \
                  HEprint(stderr, 0); exit(1);}}

int main(argc, argv)
    int argc;
    char *argv[];
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref;
    int16 access, special;
    register int ret, i;
    intn errors = 0;

    for (i=0; i<4096; i++) outbuf[i] = (char) (i % 256);

    printf("Creating a file %s\n", TESTFILE_NAME);
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("\nWrite an element and then promote to Linked Blocks\n");
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1, 
                     (uint8 *) "element 1000 1 wrong ",
                      strlen("element 1000 1 wrong ") + 1);
    CHECK(ret, FAIL, "Hputelement");

    aid1 = HLcreate(fid, 1000, 1, 10,10);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hseek(aid1, strlen("element 1000 1 "), DF_START);
    CHECK(ret, FAIL, "Hseek");

    ret = Hwrite(aid1, strlen("correct")+1, (uint8 *) "correct");
    if(ret != strlen("correct") + 1) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    printf("\nCreate a new element as a Linked Block\n");
    aid1 = HLcreate(fid, 1000, 4, 512, 2);
    CHECK(ret, FAIL, "HLcreate");

    ret = Hwrite(aid1, 2000, outbuf);
    if(ret != 2000) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");


    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = HLcreate(fid, 1000, 2, 128, 16);
    CHECK(aid1, FAIL, "HLcreate");

    ret = Hwrite(aid1, strlen("element 1000 2")+1, (uint8 *) "element 1000 2");
    if(ret != strlen("element 1000 2") + 1) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    printf("\nVerifying data\n");
    ret = Hgetelement(fid, (uint16) 1000, (uint16) 4, inbuf);
    if(ret != 2000) {
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

    ret = Hwrite(aid1, 4096, outbuf); 
    if(ret != 4096) {
      fprintf(stderr, "ERROR: Hwrite returned the wrong length: %d\n", ret);
      errors++;
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    printf("\nClosing and re-opening file %s\n", TESTFILE_NAME);
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    fid = Hopen(TESTFILE_NAME, DFACC_ALL, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("\nVerifying data\n");

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

    if(strcmp(inbuf, "element 1000 1 correct")) {
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

    printf("\nWriting to existing element\n");
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

    printf("\nOpen a second id for file %s\n", TESTFILE_NAME);
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    CHECK(fid1, FAIL, "Hopen");

    ret = Hnewref(fid1);
    CHECK(ret, FAIL, "Hnewref");

    printf("\nClosing the files\n");
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");

    if(errors) 
      fprintf(stderr, "\n\t>>> %d errors were encountered <<<\n\n");
    else
      fprintf(stderr, "\n\t>>> All tests passed <<< \n\n");

}











/*
 * Local variables:
 * compile-command: "gcc -g -o thblocks thblocks.c libhdf.a"
 * end:
 */
