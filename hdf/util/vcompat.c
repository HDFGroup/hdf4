#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/02/28 17:00:00  likkai
Initial revision

*/
#include <stdio.h>

main (ac,av) int ac; char**av; {

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

} /* main */

/* ------------------------------------------------------------------ */
