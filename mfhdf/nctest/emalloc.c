/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Header$
 *********************************************************************/

/*LINTLIBRARY*/
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "emalloc.h"

void *
emalloc (size)			/* check return from malloc */
int size;
{
    void   *p;

    if (size < 0) {
        error ("negative arg to emalloc: %d", size);
	return 0;
    }
    if (size == 0)
      return 0;
    p = (void *) malloc ((unsigned) size);
    if (p == 0) {
	error ("out of memory\n");
	exit (1);
    }
    return p;
}

void *
erealloc (ptr,size)		/* check return from realloc */
     void *ptr;
     int size;
{
    void *p;

    if (size < 0) {
        error ("negative arg to realloc");
	return 0;
    }
    p = (void *) realloc ((char *) ptr, (unsigned) size);

    if (p == 0) {
 	error ("out of memory");
	exit(1);
    }
    return p;
}
