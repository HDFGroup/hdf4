/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
  test opening files and access elements until limits are reached
*/

#include "tproto.h"
#define BIG 600
#define TESTFILE_NAME "thf"

static int32 FAR files[BIG];
static int32 FAR accs[BIG];

void test_hfile1()
{
    int i;
    int32 ret;
#ifdef QAK
    int32 *files, *accs;

    files=HDgetspace(BIG*sizeof(int32));
    accs=HDgetspace(BIG*sizeof(int32));
	if(!files || !accs) {
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
#endif


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
    accs[i] = Hstartwrite(files[0], (uint16)100, (uint16)i, 100L);
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

    ret = Hclose(files[0]);
#ifdef QAK
    HDfreespace((VOIDP)files);
    HDfreespace((VOIDP)accs);
#endif
}
