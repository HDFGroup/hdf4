#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/06/01 19:23:47  chouck
Cleaned up output

 * Revision 1.3  1992/05/31  15:23:30  mfolk
 * Added uint8 * and uint16 casts to make Convex stop complaining.
 *
 * Revision 1.2  1992/05/28  14:24:01  chouck
 * Added casts for calls to Hinquire()
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/
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

    printf("Creating a file %s\n\n", TESTFILE_NAME);
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("Reading / Writing to file\n");
    ret = Hputelement(fid, (uint16) 100, 1, 
                      (uint8 *) "testing 100 1", strlen("testing 100 1")+1);
    CHECK(ret, FAIL, "Hputelement");

    ret = Hputelement(fid, (uint16) 100, (uint16) 4, outbuf, 2000);
    CHECK(ret, FAIL, "Hputelement");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    ret = Hputelement(fid, (uint16) 103, (uint16) 2, 
                      (uint8 *) "element 103 2", strlen("element 103 2")+1);
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

    ret = Hputelement(fid, 102, 2, outbuf, 4096);
    CHECK(ret, FAIL, "Hputlement");

    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    do_shell();

    printf("\nClosing and re-opening file %s\n\n", TESTFILE_NAME);
    fid = Hopen(TESTFILE_NAME, DFACC_ALL, 0);
    CHECK(fid, FAIL, "Hopen");

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    aid1 = Hstartread(fid, 100, 1);
    CHECK(aid1, FAIL, "Hstartread");

    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    CHECK(ret, FAIL, "Hinquire");

    printf("Verifying data\n\n");
    ret = Hread(aid1, length, inbuf);
    if(ret != 14) {
      fprintf(stderr, "ERROR: Hread returned the wrong length: %d\n", ret);
      errors++;
    }

    if(strcmp(inbuf, "testing 100 1")) {
      fprintf(stderr, "ERROR: Hread returned the wrong data\n");
      fprintf(stderr, "\t       Is: %s\n", inbuf);
      fprintf(stderr, "\tShould be: testing 100 1\n");
      errors++;
    }

    ret = Hnewref(fid);
    CHECK(ret, FAIL, "Hnewref");

    printf("Testing a number of searching schemes\n\n");
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
    
    do_shell();


    printf("Attempting to gain multiple access to file (is allowed)\n");
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

    do_shell();
  
    if(errors) 
      fprintf(stderr, "\n\t>>> %d errors were encountered <<<\n\n");
    else
      fprintf(stderr, "\n\t>>> All tests passed <<< \n\n");

}
