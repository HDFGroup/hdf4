#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/01/27 22:04:39  briand
Converted test files to work with master test program: testhdf

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

test_vers()
{
  int i, j, ret;
  int32 f;
  uint32 fmajor, fminor, frelease;
  uint32 lmajor, lminor, lrelease;
  char fstring[81], lstring[81], output[256];

  ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);
  RESULT("Hgetlibversion");
  MESSAGE(5,printf("Library Version\n"););
  MESSAGE(5,printf("---------------\n"););
  MESSAGE(5,printf("Major:\t\t%d\nMinor:\t\t%d\nRelease:\t%d\nString:\t\t\"%s\"\n",lmajor, lminor, lrelease, lstring););

}










