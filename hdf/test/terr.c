#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/02/10 20:59:34  chouck
Initial revision

*/
/*
** TERR
**  Attempt to do something that HDF doesn't like so that we can test
**  error handling
*/

#include "hdf.h"
#define TESTFILE_LOCAL "terr.hdf"
#define TESTFILE_STUPID_PATH "/foo/bar/HDF/.empty/terr.hdf"
char outbuf[4096], inbuf[4096];

/*
** Test a status value.  If an error has occurred, add an error
**  record (code: return_code) for the current routine and return FAIL
*/
#define N_VERIFY(status, return_code, n) \
{if(status == n) {HERROR(return_code); return(FAIL);}}

#define ZERO_VERIFY(status, return_code) N_VERIFY(status, return_code, 0);
#define VERIFY(status, return_code)      N_VERIFY(status, return_code, FAIL);

/*
** Test a status value and print error dump if an error has
**  occurred
*/
#define N_CHECK(status, n) {if(status == n) HEprint(stdout, 0);}
#define CHECK(status)      N_CHECK(status, FAIL);
#define ZERO_CHECK(status) N_CHECK(status, 0);

main(argc, argv)
    int argc;
    char *argv[];
{
    int32 fid, fid1;
    int32 aid1, aid2;
    int32 fileid, length, offset, posn;
    uint16 tag, ref;
    int access, special, ret, i, status;

    printf(" -------- \n");

    printf("Creating (good) file %s\n", TESTFILE_LOCAL);
    fid = Hopen(TESTFILE_LOCAL, DFACC_CREATE, 0);
    CHECK(fid);

    printf("Creating (bad) file %s\n", TESTFILE_STUPID_PATH);
    status = Hopen(TESTFILE_STUPID_PATH, DFACC_CREATE, 0);
    CHECK(status);

    printf("Attempt to open the executable (bad)\n");
    status = Hopen(argv[0], DFACC_READ, 0);
    CHECK(status);

    printf(" -------- \n");

    puts("checking (good) newref");
    status = Hnewref(fid);
    ZERO_CHECK(status);

    puts("checking (bad) newref");
    status = Hnewref((int32)2700173);
    ZERO_CHECK(status);

    printf(" -------- \n");

    puts("putting some data elements into a (good) file");
    status = Hputelement(fid, 100, 1, "test 100 1", strlen("test 100 1")+1);
    CHECK(status);

    puts("putting some data elements into a (bad) file");
    status = 
      Hputelement((int32) 23578, 100, 1, "test 100 1", strlen("test 100 1")+1);
    CHECK(status);

    printf(" -------- \n");

exit(0);

    puts("Putting data element 100 4");
    ret = Hputelement(fid, 100, 4, outbuf, 2000);
    printf("ret from Hputelement is %d\n", ret);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("putting data element 103 2");
    ret = Hputelement(fid, 103, 2, "element 103 2", strlen("element 103 2")+1);
    printf("ret from Hputelement is %d\n", ret);

    puts("getting data element 100 4");
    ret = Hgetelement(fid, 100, 4, inbuf);
    printf("ret from Hgetelement is %d\n", ret);
    for (i=0; i<ret; i++) {
       if (inbuf[i] != outbuf[i])
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
       inbuf[i] = '\0';
    }

    puts("putting data element 102 2");
    ret = Hputelement(fid, 102, 2, outbuf, 4096);
    printf("ret from Hputelement is %d\n", ret);

    puts("Closing the file.");
    ret = Hclose(fid);
    printf("ret from Hclose is %d\n", ret);

    puts("Opening a file with DFACC_ALL");
    fid = Hopen(TESTFILE_LOCAL, DFACC_ALL, 0);
    printf("fid from Hopen is %d\n", fid);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("starting read 100 1");
    aid1 = Hstartread(fid, 100, 1);
    printf("aid1 from Hstartread is %d\n", aid1);

    puts("inquiring about access element");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    printf("ret from Hinquire is %d\n", ret);
    printf("fileid %d tag %d ref %d length %d offset %d posn %d\n\
 access %d special %d\n",
          fileid, tag, ref, length, offset, posn, access, special);

    puts("Reading entire data element");
    ret = Hread(aid1, length, inbuf);
    printf("ret from Hread is %d\n", ret);
    printf("data is :%s:\n", inbuf);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("searching nextread of 100 WILDCARD using DF_CURRENT");
    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    printf("ret from Hnextread is %d\n", ret);

    puts("inquiring about access element");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    printf("ret from Hinquire is %d\n", ret);
    printf("fileid %d tag %d ref %d length %d offset %d posn %d\n\
 access %d special %d\n",
          fileid, tag, ref, length, offset, posn, access, special);

    puts("searching nextread of 100 WILDCARD using DF_CURRENT");
    puts("expect failure");
    ret = Hnextread(aid1, 100, DFREF_WILDCARD, DF_CURRENT);
    printf("ret from Hnextread is %d\n", ret);

    puts("searching nextread of WILDCARD WILDCARD using DF_START");
    ret = Hnextread(aid1, DFTAG_WILDCARD, DFREF_WILDCARD, DF_START);
    printf("ret from Hnextread is %d\n", ret);

    puts("inquiring about access element");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    printf("ret from Hinquire is %d\n", ret);
    printf("fileid %d tag %d ref %d length %d offset %d posn %d\n\
 access %d special %d\n",
          fileid, tag, ref, length, offset, posn, access, special);

    puts("searching nextread of WILDCARD 3 using DF_CURRENT");
    puts("expect a failure");
    ret = Hnextread(aid1, DFTAG_WILDCARD, 3, DF_CURRENT);
    printf("ret from Hnextread is %d\n", ret);

    puts("searching nextread of WILDCARD 2 using DF_CURRENT");
    ret = Hnextread(aid1, DFTAG_WILDCARD, 2, DF_CURRENT);
    printf("ret from Hnextread is %d\n", ret);

    puts("inquiring about access element");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    printf("ret from Hinquire is %d\n", ret);
    printf("fileid %d tag %d ref %d length %d offset %d posn %d\n\
 access %d special %d\n",
          fileid, tag, ref, length, offset, posn, access, special);

    puts("starting write on old data element 100 1");
    aid2 = Hstartwrite(fid, 100, 1, 4);
    printf("aid from Hstartwrite is %d\n", aid1);

    puts("writing ABCD into data element");
    ret = Hwrite(aid1, 4, "ABCD");
    printf("ret from Hwrite is %d\n");

    puts("ending read access element");
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n");

    puts("ending write access element");
    ret = Hendaccess(aid2);
    printf("ret from Hendaccess is %d\n");

    puts("Opening a file.");
    fid1 = Hopen(TESTFILE_LOCAL, DFACC_READ, 0);
    printf("fid1 from Hopen is %d\n", fid1);

    puts("checking newref of fid1");
    ret = Hnewref(fid1);
    printf("newref is %d\n", ret);

    puts("Closing the file fid");
    ret = Hclose(fid);
    printf("ret from Hclose is %d\n", ret);

    puts("Closing the file fid1");
    ret = Hclose(fid1);
    printf("ret from Hclose is %d\n", ret);

}
