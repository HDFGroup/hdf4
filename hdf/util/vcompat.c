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

#include <stdio.h>
#include "vg.h"

#ifdef PROTOTYPE
int main (int ac,char **av)
#else
int main (ac,av)
int ac;
char**av;
#endif
{

if (ac!=2) { 
	fprintf(stderr,"%s: converts HDF vset v1.0 files to v2.0\n",av[0]);
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
