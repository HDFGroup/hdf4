#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4  1992/05/31 17:43:29  mfolk
Put in a semicolon I accidentally deleted in last change.

 * Revision 1.3  1992/05/29  21:52:19  mfolk
 * Added some casts to make it compile without warnings on Convex.
 *
 * Revision 1.2  1992/05/28  14:24:01  chouck
 * Added casts for calls to Hinquire()
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/
/*
** TERR
**  Attempt to do something that HDF doesn't like so that we can test
**  error handling
*/

#include "hdf.h"
#define TESTFILE_LOCAL "terr.hdf"
#define TESTFILE_STUPID_PATH "/foo/bar/HDF/.empty/terr.hdf"
uint8 outbuf[4096], inbuf[4096];

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
    int16 access, special;
    int   ret, i, status;

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
    status = Hputelement(fid, (uint16) 100, (uint16) 1, 
                       (uint8 *) "test 100 1", strlen("test 100 1")+1);
    CHECK(status);

    puts("putting some data elements into a (bad) file");
    status = 
      Hputelement((int32) 23578, (uint16) 100, (uint16) 1, 
                       (uint8 *)"test 100 1", strlen("test 100 1")+1);
    CHECK(status);

    printf(" -------- \n");
    exit(0);

}
