/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "emalloc.h"
#ifdef HDF
#include "hdf.h"
#endif

/* check return from malloc */
void *
emalloc (int size)
{
    void   *p;

    if (size < 0) {
        error ("negative arg to emalloc: %d", size);
	return 0;
    }
    if (size == 0)
      return 0;
#ifdef HDF
    p = (void *) HDmalloc((uint32)size);
#else
    p = (void *) malloc ((unsigned) size);
#endif
    if (p == 0) {
	error ("out of memory\n");
	exit (1);
    }
    return p;
}

/* check return from realloc */
void *
erealloc (void *ptr, int size)
{
    void *p;

    if (size < 0) {
        error ("negative arg to realloc");
	return 0;
    }
#ifdef HDF
    p = (void *) HDrealloc((VOIDP) ptr, (uint32) size);
#else
    p = (void *) realloc ((char *) ptr, (unsigned) size);
#endif

    if (p == 0) {
 	error ("out of memory");
	exit(1);
    }
    return p;
}

