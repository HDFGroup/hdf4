#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.7  1993/04/19 23:04:46  koziol
General Code Cleanup to reduce/remove compilation warnings on PC

 * Revision 1.6  1993/01/19  06:00:34  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.5  1992/07/15  21:48:48  sxu
 * No change
 *
 * Revision 1.4  1992/05/26  21:00:08  koziol
 * Folded Jason's Mac port and Linted code into the main version
 *
 * Revision 1.3  1992/04/24  15:38:25  koziol
 * PC port
 *
 * Revision 1.2  1992/02/29  18:58:36  sxu
 * add header
 *
 * Revision 1.1  1992/02/28  17:00:00  likkai
 * Initial revision
 *
*/
#include <stdio.h>
#include "vg.h"

#ifdef PROTOTYPE
main (int ac,char **av)
#else
main (ac,av)
int ac;
char**av;
#endif
{

if (ac!=2) { 
	fprintf(stderr,"%v: converts HDF vset v1.0 files to v2.0\n",av[0]);
	fprintf(stderr,"Usage: %s hdf-file\n",av[0]);
	exit(0);
	}

if ( 0 == vcheckcompat(av[1]) ) {
	vmakecompat (av[1]);
	fprintf(stderr,"file [%s] is now compatible\n", av[1]);
	}
else
	fprintf(stderr,"file [%s] already compatible with r2.0\n",av[1]);

    return(0);
} /* main */

/* ------------------------------------------------------------------ */
