#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1993/04/19 23:04:13  koziol
General Code Cleanup to reduce/remove compilation warnings on PC

 * Revision 1.1  1993/04/15  20:00:42  koziol
 * Re-named the new tests for MS-DOS compatibility
 *
 * Revision 1.2  1993/01/27  22:41:34  briand
 * Fixed problem with compiling on RS6000.
 *
 * Revision 1.1  1993/01/27  22:04:39  briand
 * Converted test files to work with master test program: testhdf
 *
 * Revision 1.2  1992/02/10  22:00:57  chouck
 * Fixed earliest recognizable version
 *
 * Revision 1.1  1992/02/10  20:59:34  chouck
 * Initial revision
 *
*/
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

