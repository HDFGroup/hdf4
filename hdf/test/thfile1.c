#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1992/07/27 18:40:12  dilg
Changed DFACC_ALL to DFACC_RDWR in appropriate places to conform to new
handling of access modes by Hopen().

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

#include "hdf.h"
#define BIG 600
#define TESTFILE_NAME "thf"

int32 files[BIG];
int32 accs[BIG];

main()
{
  int i,ret;

  puts("Opening many files of same name");
  for (i=0; i< BIG; i++) {
    files[i] = Hopen("thf.hdf", DFACC_RDWR, 0);
    if (files[i] < 0) break;
  }
  printf("Opening stopped at %d/%d files\n", i, BIG);

  puts("Closing all files except first open");
  for (i--; i>0; i--) {
    ret = Hclose(files[i]);
    if (ret < 0)
      printf("Error closing file %d\n", i);
  }
  puts("Closed files");

  puts("Opening many files of different names");
  for (i=0; i< BIG; i++) {
    char fname[100];
    sprintf(fname, "%s%1d.hdf", TESTFILE_NAME, i);
    files[i] = Hopen(fname, DFACC_ALL, 0);
    if (files[i] < 0) break;
  }
  printf("Opening stopped at %d/%d files\n", i, BIG);

  puts("Closing all files except first open");
  for (i--; i>0; i--) {
    ret = Hclose(files[i]);
    if (ret < 0)
      printf("Error closing file %d\n", i);
  }
  puts("Closed files");

  puts("Opening write access elements");
  for (i=0; i<BIG; i++) {
    accs[i] = Hstartwrite(files[0], 100, i, 100L);
    if (accs[i] < 0) break;
  }
  printf("Opening stoped at %d element\n", i);

  puts("Closing access elements");
  for (i--; i>=0; i--) {
    ret = Hendaccess(accs[i]);
    if (ret < 0)
      printf("Error ending access %d\n", i);
  }
  puts("Ended access");
}










/*
 * Local variables:
 * compile-command: "gcc -O -g thfile1.c libhdf.a -o thfile1"
 * end:
 */
