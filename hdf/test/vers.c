#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*
***********************************************************************
** get version string from an HDF file
***********************************************************************
*/

#include <stdio.h>
#include "tproto.h"

extern int Verbocity;
extern int num_errs;

void test_vers()
{
  int ret;
  uint32 lmajor, lminor, lrelease;
  char lstring[81];

  ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);
  RESULT("Hgetlibversion");
  MESSAGE(5,printf("Library Version\n"););
  MESSAGE(5,printf("---------------\n"););
  MESSAGE(5,printf("Major:\t\t%d\nMinor:\t\t%d\nRelease:\t%d\nString:\t\t\"%s\"\n",lmajor, lminor, lrelease, lstring););

}

