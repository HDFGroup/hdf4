#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/06/01 18:57:55  chouck
Cleaned up output so its clear what is going on

 * Revision 1.3  1992/05/31  15:13:55  mfolk
 * Added several casts (uint8 * and uint16) to keep Convex from complaining.
 *
 * Revision 1.2  1992/05/28  14:24:01  chouck
 * Added casts for calls to Hinquire()
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/

#include "hdf.h"
#define TESTFILE_NAME "t.hdf"
static int shell = 0;
uint8 outbuf[4096], inbuf[4096];


#define CHECK(ret, val, where) \
{if(ret == val) { fprintf(stderr, "%s failed, line %d, code %d\n", \
                          where, __LINE__, ret); \
                  HEprint(stderr, 0); exit(1);}}

void do_shell()
{
    if (shell) {
       puts("Spawning test shell.");
       if (fork() > 0) {
           execl("/bin/csh", "csh", (char*)0);
       } else {
           wait((int*)0);
       }
       puts("Returned from shell.");
    }
}

int main(argc, argv)
    int argc;
    char *argv[];
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref;
    int16 access, special;
    int ret, i;
    intn errors = 0;

    if (argc > 1 && strcmp(argv[1], "-shell") == 0) {
       shell = 1;
    }
    for (i=0; i<4096; i++) outbuf[i] = (char) (i % 256);

    printf("Creating base file %s\n", TESTFILE_NAME);

    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("Writing object into base file\n");
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1, 
                      (uint8 *) "element 1000 1 wrong",
                      strlen("element 1000 1 wrong ") + 1);
    CHECK(ret, FAIL, "Hputelement");

    printf("Promoting above object to external element in file #1\n\n");
    aid1 = HXcreate(fid, 1000, 1, "t.hdf1");
    CHECK(aid1, FAIL, "HXcreate");

    ret = Hseek(aid1, strlen("element 1000 1") + 1, DF_START);
    CHECK(ret, FAIL, "Hseek");

    ret = Hwrite(aid1, strlen("correct")+1, (uint8 *) "correct");
    if(ret != strlen("correct") + 1) {
      fprintf(stderr, "Hwrite failed (code %d)\n", ret);
      HEprint(stderr, 0);
      exit(1);
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    printf("Creating an external element in file #2\n");
    aid1 = HXcreate(fid, 1000, 4, "t.hdf2");
    CHECK(aid1, FAIL, "HXcreate");

    printf("Writing 2000 bytes to file #2\n\n");
    ret = Hwrite(aid1, 2000, outbuf);
    CHECK(ret, FAIL, "Hwrite");

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("Creating an external element in file #3\n");
    aid1 = HXcreate(fid, 1000, 2, "t.hdf3");
    CHECK(aid1, FAIL, "HXcreate");

    printf("Writing string 'element 1000 2' to file #3\n\n");
    ret = Hwrite(aid1, strlen("element 1000 2")+1, (uint8 *) "element 1000 2");
    if(ret != strlen("element 1000 2")+1) {
      fprintf(stderr, "Hwrite failed (code %d)\n", ret);
      HEprint(stderr, 0);
      exit(1);
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    printf("Verifying data that was stored to file #2\n");
    ret = Hgetelement(fid, (uint16) 1000, (uint16) 4, inbuf);
    if(ret != 2000) {
      fprintf(stderr, "Incorrect element size returned from Hgetelement: %d\n",
              ret);
      HEprint(stderr, 0);
      exit(1);
    }

    for (i=0; i<ret; i++) {
       if (inbuf[i] != outbuf[i]) {
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
           errors ++;
         }
       inbuf[i] = '\0';
    }

    printf("\nCreating an external element in file #4\n");
    aid1 = HXcreate(fid, 1020, 2, "t.hdf4");
    CHECK(aid1, FAIL, "HXcreate");

    printf("Writing 4096 bytes to file #4\n\n");
    ret = Hwrite(aid1, 4096, outbuf);
    if(ret != 4096) {
      fprintf(stderr, "Hwrite failed (code %d)\n", ret);
      HEprint(stderr, 0);
      exit(1);
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    do_shell();

    printf("Closing and re-opening base file %s\n", TESTFILE_NAME);
    fid = Hopen(TESTFILE_NAME, DFACC_ALL, 0);
    CHECK(ret, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = Hstartread(fid, 1000, 1);
    CHECK(aid1, FAIL, "Hstartread");

    printf("\nInquiring about access element in file #1\n");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    ret = Hread(aid1, length, inbuf);
    if(ret != length) {
      fprintf(stderr, "Hread failed (code %d)\n", ret);
      HEprint(stderr, 0);
      exit(1);
    }

    if(strcmp(inbuf, "element 1000 1 correct")) {
      fprintf(stderr, "Object stored in file #1 is wrong\n");
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: element 1000 1 correct\n");
      errors++;
    }

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    printf("\nInquiring about access element in file #2\n");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    CHECK(ret, FAIL, "Hnextread");

    printf("\nInquiring about access element in file #3\n");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    ret = Hnextread(aid1, DFTAG_WILDCARD, 3, DF_CURRENT);
    if(ret != FAIL) {
      fprintf(stderr, "Hnextread call should have failed !!!\n");
      errors++;
    }

    ret = Hnextread(aid1, DFTAG_WILDCARD, 2, DF_CURRENT);
    CHECK(ret, FAIL, "Hnextread");

    printf("\nInquiring about access element in file #4\n");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    aid2 = Hstartwrite(fid, 1000, 1, 4);
    CHECK(aid2, FAIL, "Hstartwrite");

    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    if(ret != 4) {
      fprintf(stderr, "Hwrite failed (code %d)\n", ret);
      HEprint(stderr, 0);
      exit(1);
    }

    ret = Hendaccess(aid1);
    CHECK(ret, FAIL, "Hendaccess");

    ret = Hendaccess(aid2);
    CHECK(ret, FAIL, "Hendaccess");

    do_shell();

    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
    CHECK(fid1, FAIL, "Hopen");

    ret = Hnewref(fid1);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    ret = Hclose(fid1);
    CHECK(ret, FAIL, "Hclose");

    do_shell();

    if(errors) 
      fprintf(stderr, "\n\t>>> %d errors were encountered <<<\n\n");
    else
      fprintf(stderr, "\n\t>>> All tests passed <<< \n\n");

}











/*
 * Local variables:
 * compile-command: "gcc -g -o thextelt thextelt.c libhdf.a"
 * end:
 */
