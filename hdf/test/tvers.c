#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/02/10 22:00:57  chouck
Fixed earliest recognizable version

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
#include "hdf.h"

int main(argc, argv)
     int argc;
     char *argv[];
{
  int i, j, ret;
  int32 f;
  uint32 fmajor, fminor, frelease;
  uint32 lmajor, lminor, lrelease;
  char fstring[81], lstring[81], output[256];

  ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);
  printf("Library Version\n");
  printf("---------------\n");
  printf("Major:\t\t%d\nMinor:\t\t%d\nRelease:\t%d\nString:\t\t\"%s\"\n\n\n",
	 lmajor, lminor, lrelease, lstring);

  for (i=1; i<argc; i++) {
    f = Hopen(argv[i], DFACC_READ, 0);
    if (f < 0) {
      printf("%s: not a hdf-file\n", argv[i]);
    } else {
      ret = Hgetfileversion(f, &fmajor, &fminor, &frelease, fstring);
      sprintf(output, "File Version (%s)\n", argv[i]);
      printf(output);
      for(j = strlen(output) - 1; j; j--)
	printf("-");	
      if ((ret < 0) || ((fmajor == 0) && (fminor == 0))){
	printf("\nEarlier than 3.2\n\n");
      } else {
	printf("\nMajor:\t\t%d\nMinor:\t\t%d\nRelease:\t%d\nString:\t\t\"%s\"\n\n",
	       fmajor, fminor, frelease, fstring);
      }
      Hclose(f);
    }
  }

  exit(0);
}










