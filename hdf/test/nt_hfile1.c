#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/01/27 22:04:32  briand
Converted test files to work with master test program: testhdf

 * Revision 1.3  1992/07/27  18:40:12  dilg
 * Changed DFACC_ALL to DFACC_RDWR in appropriate places to conform to new
 * handling of access modes by Hopen().
 *
 * Revision 1.2  1992/06/25  19:41:58  chouck
 * Changed output file names
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/
/*
  test opening files and access elements until limits are reached
*/

#include "tproto.h"
#define BIG 600
#define TESTFILE_NAME "thf"

extern int Verbocity;
extern int num_errs;

int32 files[BIG];
int32 accs[BIG];

test_hfile1()
{
  int i,ret;

  MESSAGE(5,puts("Opening many files of same name"););
  for (i=0; i< BIG; i++) {
    files[i] = Hopen("thf.hdf", DFACC_RDWR, 0);
    if (files[i] < 0) break;
  }
  MESSAGE(5,printf("Opening stopped at %d/%d files\n", i, BIG););

  MESSAGE(5,puts("Closing all files except first open"););
  for (i--; i>0; i--) {
    ret = Hclose(files[i]);
    if (ret < 0)
      printf("Error closing file %d\n", i);
  }
  MESSAGE(5,puts("Closed files"););

  MESSAGE(5,puts("Opening many files of different names"););
  for (i=0; i< BIG; i++) {
    char fname[100];
    sprintf(fname, "%s%1d.hdf", TESTFILE_NAME, i);
    files[i] = Hopen(fname, DFACC_ALL, 0);
    if (files[i] < 0) break;
  }
  MESSAGE(5,printf("Opening stopped at %d/%d files\n", i, BIG););

  MESSAGE(5,puts("Closing all files except first open"););
  for (i--; i>0; i--) {
    ret = Hclose(files[i]);
    if (ret < 0)
      printf("Error closing file %d\n", i);
  }
  MESSAGE(5,puts("Closed files"););

  MESSAGE(5,puts("Opening write access elements"););
  for (i=0; i<BIG; i++) {
    accs[i] = Hstartwrite(files[0], 100, i, 100L);
    if (accs[i] < 0) break;
  }
  MESSAGE(5,printf("Opening stoped at %d element\n", i););

  MESSAGE(5,puts("Closing access elements"););
  for (i--; i>=0; i--) {
    ret = Hendaccess(accs[i]);
    if (ret < 0)
      printf("Error ending access %d\n", i);
  }
  MESSAGE(5,puts("Ended access"););
}










/*
 * Local variables:
 * compile-command: "gcc -O -g thfile1.c libhdf.a -o thfile1"
 * end:
 */
