/*****************************************************************************
* 
*			  NCSA HDF version 3.2beta
*			     February 29, 1992
*
* NCSA HDF Version 3.10r5 source code and documentation are in the public
* domain.  Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
*****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/02/29 18:58:36  sxu
add header

 * Revision 1.1  1992/02/28  17:00:00  likkai
 * Initial revision
 *
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
