/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ncgen.h"
#include "genlib.h"

int lineno       = 1;
int derror_count = 0;

/*
 * For logging error conditions.
 */
void
derror(const char *fmt, ...)
{
    va_list args;

    if (lineno == 1)
        fprintf(stderr, "%s: %s: ", progname, cdlname);
    else
        fprintf(stderr, "%s: %s line %d: ", progname, cdlname, lineno);

    va_start(args, fmt);

    (void)vfprintf(stderr, fmt, args);
    va_end(args);

    (void)fputc('\n', stderr);
    (void)fflush(stderr); /* to ensure log files are current */
    derror_count++;
}

void *
emalloc(size_t size)
{
    void *p = NULL;

    if (size == 0)
        return NULL;

    p = (void *)malloc(size);
    if (p == NULL) {
        derror("out of memory\n");
        exit(3);
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
        derror("out of memory");
        exit(3);
    }

    return p;
}
