/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/README file for copying and redistribution conditions.
 *********************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hdf.h"

#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#include "dumplib.h"

/*
 * Print error message to stderr, don't exit
 */
void
error(const char *fmt, ...)
{
    va_list args;

    fprintf(stderr, "*** %s: ", progname);

    va_start(args, fmt);
    (void)vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    (void)fflush(stderr); /* to ensure log files are current */
}

#define LINEPIND "    " /* indent of continued lines */

static int linep;
static int max_line_len;

void
set_indent(int in)
{
    linep = in;
}

void
set_max_len(int len)
{
    max_line_len = len - 2;
}

void
lput(const char *cp)
{
    int nn = strlen(cp);

    if (nn + linep > max_line_len && nn > 2) {
        (void)fputs("\n", stdout);
        (void)fputs(LINEPIND, stdout);
        linep = strlen(LINEPIND);
    }
    (void)fputs(cp, stdout);
    linep += nn;
}

static char *formats[] = {
    "%d",    /* bytes, shorts */
    "%s",    /* char arrays as strings */
    "%ld",   /* longs */
    "%.7g ", /* floats */
    "%.15g"  /* doubles */
};

/* In case different formats specified with -d option, set them here. */
void
set_formats(char *flt, char *dbl)
{
    strcpy(formats[3], flt);
    strcpy(formats[4], dbl);
}

/* ncid - netcdf id */
/* varid - variable id */
static char *
has_c_format_att(int ncid, int varid)
{
    nc_type cfmt_type;
    int     cfmt_len;
    int     savopts;
#define C_FMT_NAME   "C_format" /* name of C format attribute */
#define MAX_CFMT_LEN 100        /* max length of C format attribute */
    static char cfmt[MAX_CFMT_LEN];

    /*
     * turn off error handling, we expect ncattinq to fail if there is no
     * C_format attribute
     */
    savopts = ncopts;
    ncopts  = 0;
    if (ncattinq(ncid, varid, "C_format", &cfmt_type, &cfmt_len) != -1) {
        ncopts = savopts; /* restore error handling */
        if (cfmt_type == NC_CHAR && cfmt_len > 0 && cfmt_len < MAX_CFMT_LEN) {
            if (ncattget(ncid, varid, "C_format", (void *)cfmt) != -1)
                return &cfmt[0];
        }
    }
    ncopts = savopts; /* restore error handling */
    return 0;
}

/*
 * Determine print format to use for each value for this variable.  Use value
 * of attribute C_format if it exists, otherwise a sensible default.
 */
/* ncid  - netcdf id */
/* varid - variable id */
/* type  - netCDF data type */
const char *
get_fmt(int ncid, int varid, nc_type type)
{
    char *c_format_att = has_c_format_att(ncid, varid);

    /* If C_format attribute exists, return it */
    if (c_format_att)
        return c_format_att;

    /* Otherwise return sensible default. */
    switch (type) {
        case NC_BYTE:
            return formats[0];
        case NC_CHAR:
            return formats[1];
        case NC_SHORT:
            return formats[0];
        case NC_LONG:
            return formats[2];
        case NC_FLOAT:
            return formats[3];
        case NC_DOUBLE:
            return formats[4];
        default:
            error("pr_vals: bad type");
            return NULL;
    }
}

static vnode *
newvnode()
{
    vnode *newvp = (vnode *)malloc(sizeof(vnode));

    if (!newvp) {
        error("out of memory!");
        exit(EXIT_FAILURE);
    }
    return newvp;
}

vnode *
newvlist()
{
    vnode *vp = newvnode();

    vp->next = 0;
    vp->id   = -1; /* bad id */

    return vp;
}

void
varadd(vnode *vlist, int varid)
{
    vnode *newvp = newvnode();

    newvp->next = vlist->next;
    newvp->id   = varid;
    vlist->next = newvp;
}

int
varmember(vnode *vlist, int varid)
{
    vnode *vp = vlist->next;

    for (; vp; vp = vp->next)
        if (vp->id == varid)
            return 1;
    return 0;
}
