#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/05/31 15:13:55  mfolk
Added several casts (uint8 * and uint16) to keep Convex from complaining.

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

    if (argc > 1 && strcmp(argv[1], "-shell") == 0) {
       shell = 1;
    }
    for (i=0; i<4096; i++) outbuf[i] = (char) (i % 256);

    printf("Creating a file %s\n", TESTFILE_NAME);
    fid = Hopen(TESTFILE_NAME, DFACC_CREATE, 0);
    printf("fid from Hopen is %d\n", fid);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("putting some data elements into the file.");
    puts("Regular Data element 1000 1");
    ret = Hputelement(fid, (uint16) 1000, (uint16) 1, 
                      (uint8 *) "element 1000 1 wrong",
                      strlen("element 1000 1 wrong") + 1);
    printf("ret from Hputelement is %d\n", ret);
    aid1 = HXcreate(fid, 1000, 1, "t.hdf1");
    printf("aid1 from HXcreate is %d\n", aid1);
    ret = Hseek(aid1, strlen("element 1000 1"), DF_START);
    printf("ret from Hseek is %d\n", ret);
    ret = Hwrite(aid1, strlen("correct")+1, (uint8 *) "correct");
    printf("ret from Hwrite is %d\n", ret);
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n", ret);

    puts("Putting data element 1000 4");
    aid1 = HXcreate(fid, 1000, 4, "t.hdf2");
    printf("aid1 from Hcreate is %d\n", aid1);
    ret = Hwrite(aid1, 2000, outbuf);
    printf("ret from Hwrite is %d\n", ret);
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n", ret);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("putting data element 1000 2");
    aid1 = HXcreate(fid, 1000, 2, "t.hdf3");
    printf("aid1 from HXcreate is %d\n", aid1);
    ret = Hwrite(aid1, strlen("element 1000 2")+1, (uint8 *) "element 1000 2");
    printf("ret from Hwrite is %d\n", ret);
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n", ret);

    puts("getting data element 1000 4");
    ret = Hgetelement(fid, (uint16) 1000, (uint16) 4, inbuf);
    printf("ret from Hgetelement is %d\n", ret);
    for (i=0; i<ret; i++) {
       if (inbuf[i] != outbuf[i])
           printf("Wrong data at %d, out %d in %d\n", i, outbuf[i], inbuf[i]);
       inbuf[i] = '\0';
    }

    puts("putting data element 1020 2");
    aid1 = HXcreate(fid, 1020, 2, "t.hdf4");
    printf("aid1 from HXcreate is %d\n", aid1);
    ret = Hwrite(aid1, 4096, outbuf);
    printf("ret from Hwrite is %d\n", ret);
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n", ret);

    puts("Closing the file.");
    ret = Hclose(fid);
    printf("ret from Hclose is %d\n", ret);

    do_shell();

    puts("Opening a file with DFACC_ALL");
    fid = Hopen(TESTFILE_NAME, DFACC_ALL, 0);
    printf("fid from Hopen is %d\n", fid);

    puts("checking newref");
    ret = Hnewref(fid);
    printf("newref is %d\n", ret);

    puts("starting read 1000 1");
    aid1 = Hstartread(fid, 1000, 1);
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

    puts("searching nextread of 1000 WILDCARD using DF_CURRENT");
    ret = Hnextread(aid1, 1000, DFREF_WILDCARD, DF_CURRENT);
    printf("ret from Hnextread is %d\n", ret);

    puts("inquiring about access element");
    ret = Hinquire(aid1, &fileid, &tag, &ref, &length, &offset, &posn,
                  &access, &special);
    printf("ret from Hinquire is %d\n", ret);
    printf("fileid %d tag %d ref %d length %d offset %d posn %d\n\
 access %d special %d\n",
          fileid, tag, ref, length, offset, posn, access, special);

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

    puts("starting write on old data element 1000 1");
    aid2 = Hstartwrite(fid, 1000, 1, 4);
    printf("aid from Hstartwrite is %d\n", aid1);

    puts("writing ABCD into data element");
    ret = Hwrite(aid2, 4, (uint8 *) "ABCD");
    printf("ret from Hwrite is %d\n", ret);

    puts("ending read access element");
    ret = Hendaccess(aid1);
    printf("ret from Hendaccess is %d\n", ret);

    puts("ending write access element");
    ret = Hendaccess(aid2);
    printf("ret from Hendaccess is %d\n", ret);

    do_shell();

    puts("Opening a file.");
    fid1 = Hopen(TESTFILE_NAME, DFACC_READ, 0);
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

    do_shell();

}











/*
 * Local variables:
 * compile-command: "gcc -g -o thextelt thextelt.c libhdf.a"
 * end:
 */
