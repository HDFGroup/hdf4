/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "emalloc.h"

/* Check return from malloc */
void *
emalloc(size_t size)
{
    void *p = NULL;

    if (size == 0)
        return NULL;

    p = (void *)malloc(size);
    if (p == NULL) {
        error("out of memory\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

/* Check return from realloc
 *
 * NOTE: realloc(NULL, 0) behavior is implementation-defined
 */
void *
erealloc(void *ptr, size_t size)
{
    void *p = NULL;

    p = (void *)realloc(ptr, size);
    if (p == NULL) {
        error("out of memory");
        exit(EXIT_FAILURE);
    }

    return p;
}
