/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1988, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static char mrcsid[] = "Id: cdftest.c,v 1.11 1994/01/10 23:07:27 chouck Exp ";

/*
 *	 Program to create a cdf, exercise all cdf functions.
 *  Creates cdf, stuff it full of numbers, closes it. Then
 *  reopens it, and checks for consistency.
 *  Leaves the file around afterwards.
 *
 *	Based on a program to test the nasa look-alike program,
 * so not the most appropriate test. See ../nctest for a
 * complete spec test.
 */

#include "hdf.h"

#define REDEF

/* #define SYNCDEBUG */
/* #define NOBUF */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#define CDFMAXSHORT 32767
#define CDFMAXLONG  2147483647
#define CDFMAXBYTE  127

#define FILENAME  "test.cdf"
#define NUM_DIMS  3
#define DONT_CARE -1

/* make these numbers big when you want to give this a real workout */
#define NUM_RECS 8
#define SIZE_1   7
#define SIZE_2   8

#define cdf_assert(ex)                                                                                       \
    {                                                                                                        \
        if (!(ex)) {                                                                                         \
            fprintf(stderr, "Assertion failed: file %s, line %d\n", __FILE__, __LINE__);                     \
            exit(EXIT_FAILURE);                                                                              \
        }                                                                                                    \
    }

struct {
    int num_dims;
    int num_vars;
    int num_attrs;
    int xtendim;
} cdesc[1];

struct {
    char    mnem[H4_MAX_NC_NAME];
    nc_type type;
    int     ndims;
    int     dims[H4_MAX_VAR_DIMS];
    int     num_attrs;
} vdesc[1];

struct {
    char    mnem[H4_MAX_NC_NAME];
    nc_type type;
    int     len;
} adesc[1];

union getret {
    char   by[8];
    short  sh[4];
    nclong lng[2];
    float  fl[2];
    double dbl;
};

static void
chkgot(nc_type type, union getret got, double check)
{
    switch (type) {
        case NC_BYTE:
            cdf_assert((char)check == got.by[0]);
            break;
        case NC_SHORT:
            cdf_assert((short)check == got.sh[0]);
            break;
        case NC_LONG:
            cdf_assert((nclong)check == got.lng[0]);
            break;
        case NC_FLOAT:
            cdf_assert((float)check == got.fl[0]);
            break;
        case NC_DOUBLE:
            cdf_assert(check == got.dbl);
            break;
    }
}

const char *fname = FILENAME;

int         num_dims_g    = NUM_DIMS;
long        sizes_g[]     = {NC_UNLIMITED, SIZE_1, SIZE_2};
const char *dim_names_g[] = {"record", "ixx", "iyy"};

static void
createtestdims(int cdfid, int num_dims, long *sizes, const char *dim_names[])
{
    while (num_dims--) {
        cdf_assert(ncdimdef(cdfid, *dim_names++, *sizes) >= 0);
        sizes++;
    }
}

static void
testdims(int cdfid, int num_dims, long *sizes, const char *dim_names[])
{
    int  ii;
    long size;
    char cp[H4_MAX_NC_NAME];
    for (ii = 0; ii < num_dims; ii++, sizes++) {
        cdf_assert(ncdiminq(cdfid, ii, cp, &size) >= 0);
        if (size != *sizes)
            fprintf(stderr, "%d: %ld != %ld\n", (int)ii, (long)size, (long)*sizes);
        cdf_assert(size == *sizes);
        cdf_assert(strcmp(cp, *dim_names++) == 0);
    }
}

const char *reqattr[] = {"UNITS", "VALIDMIN", "VALIDMAX", "SCALEMIN", "SCALEMAX", "FIELDNAM", _FillValue};
#define NUM_RATTRS 6

struct tcdfvar {
    const char *mnem;
    nc_type     type;
    const char *fieldnam;
    double      validmin;
    double      validmax;
    double      scalemin;
    double      scalemax;
    const char *units;
    int         ndims;
    int         dims[NUM_DIMS];
} testvars_g[] = {
#define Byte_id 0
    {"Byte",
     NC_BYTE,
     "Byte sized integer variable",
     -CDFMAXBYTE,
     CDFMAXBYTE,
     -CDFMAXBYTE,
     CDFMAXBYTE,
     "ones",
     2,
     {0, 1, DONT_CARE}},
#define Short_id 1
    {"Short",
     NC_SHORT,
     "Short variable",
     -CDFMAXSHORT,
     CDFMAXSHORT,
     -CDFMAXSHORT,
     CDFMAXSHORT,
     "ones",
     2,
     {0, 2, DONT_CARE}},
#define Long_id 2
    {"Long",
     NC_LONG,
     "Long Integer variable",
     -CDFMAXLONG,
     CDFMAXLONG,
     -CDFMAXLONG,
     CDFMAXLONG,
     "ones",
     2,
     {1, 2, DONT_CARE}},
#define Float_id 3
    {"Float",
     NC_FLOAT,
     "Single Precision Floating Point variable",
     -CDFMAXLONG,
     CDFMAXLONG,
     -CDFMAXLONG,
     CDFMAXLONG,
     "flots",
     3,
     {0, 1, 2}},
#define Double_id 4
    {"Double",
     NC_DOUBLE,
     "Double Precision Floating Point variable",
     -CDFMAXLONG,
     CDFMAXLONG,
     -CDFMAXLONG,
     CDFMAXLONG,
     "dflots",
     3,
     {0, 1, 2}},
};
#define NUM_TESTVARS 5

static void
createtestvars(int id, struct tcdfvar *testvars, int count)
{
    int             ii;
    struct tcdfvar *vp = testvars;

    for (ii = 0; ii < count; ii++, vp++) {
        cdf_assert(ncvardef(id, vp->mnem, vp->type, vp->ndims, vp->dims) == ii);

        cdf_assert(ncattput(id, ii, reqattr[0], NC_CHAR, strlen(vp->units), vp->units) == 0);
        cdf_assert(ncattput(id, ii, reqattr[1], NC_DOUBLE, 1, (ncvoid *)&(vp->validmin)) == 1);
        cdf_assert(ncattput(id, ii, reqattr[2], NC_DOUBLE, 1, (ncvoid *)&(vp->validmax)) == 2);
        cdf_assert(ncattput(id, ii, reqattr[3], NC_DOUBLE, 1, (ncvoid *)&(vp->scalemin)) == 3);
        cdf_assert(ncattput(id, ii, reqattr[4], NC_DOUBLE, 1, (ncvoid *)&(vp->scalemax)) == 4);
        cdf_assert(ncattput(id, ii, reqattr[5], NC_CHAR, strlen(vp->fieldnam), vp->fieldnam) == 5);
    }
}

static void
parray(const char *label, unsigned count, const long array[])
{
    fprintf(stdout, "%s", label);
    fputc('\t', stdout);
    for (; count > 0; count--, array++)
        fprintf(stdout, " %ld", *array);
}

static void
fill_seq(int id)
{
    long  vindices[NUM_DIMS];
    long *cc, *mm;
    float val;
    int   ii = 0;

    sizes_g[0] = NUM_RECS;
    /* zero the indices */

    cc = vindices;
    while (cc <= &vindices[num_dims_g - 1])
        *cc++ = 0;

    /* ripple counter */
    cc = vindices;
    mm = sizes_g;
    while (*vindices < *sizes_g) {
        while (*cc < *mm) {
            if (mm == &sizes_g[num_dims_g - 1]) {
                val = ii;
#ifdef VDEBUG
                parray("indices", NUM_DIMS, vindices);
                printf("\t val %f\n", val);
#endif
                cdf_assert(ncvarput1(id, Float_id, vindices, (ncvoid *)&val) != -1);
                (*cc)++;
                ii++;
                continue;
            }
            cc++;
            mm++;
        }
        if (cc == vindices)
            break;
        *cc = 0;
        cc--;
        mm--;
        (*cc)++;
    }
}

static void
check_fill_seq(int id)
{
    long         vindices[NUM_DIMS];
    long        *cc, *mm;
    union getret got;
    int          ii = 0;
    float        val;

    sizes_g[0] = NUM_RECS;
    cc         = vindices;
    while (cc <= &vindices[num_dims_g - 1])
        *cc++ = 0;

    /* ripple counter */
    cc = vindices;
    mm = sizes_g;
    while (*vindices < *sizes_g) {
        while (*cc < *mm) {
            if (mm == &sizes_g[num_dims_g - 1]) {
                if (ncvarget1(id, Float_id, vindices, (ncvoid *)&got) == -1)
                    goto bad_ret;
                val = ii;
                if (val != got.fl[0]) {
                    parray("indices", (unsigned)NUM_DIMS, vindices);
                    printf("\t%f != %f\n", (double)val, (double)got.fl[0]);
                }
                (*cc)++;
                ii++;
                continue;
            }
            cc++;
            mm++;
        }
        if (cc == vindices)
            break;
        *cc = 0;
        cc--;
        mm--;
        (*cc)++;
    }
    return;
bad_ret:
    printf("couldn't get a var in check_fill_seq() %d\n", (int)ii);
    return;
}

long indices[][3] = {
    {0, 1, 3}, {0, 3, 0}, {1, 2, 3}, {3, 2, 1}, {2, 1, 3}, {1, 0, 0}, {0, 0, 0},
};

char   chs[]                           = {'A', 'B', ((char)0xff)};
long   s_start[]                       = {0, 1};
long   s_edges[]                       = {NUM_RECS, SIZE_1 - 1};
char   sentence[NUM_RECS * SIZE_1 - 1] = "The red death had long devastated the country.";
short  shs[]                           = {97, 99};
nclong birthday                        = 82555;
float  e                               = 2.7182818284590452354F;
double pinot                           = 3.25;
double zed                             = 0.0;

int
main(void)
{
    int ret;
    int id;
    char new[256];
#ifdef SYNCDEBUG
    char *str = "one";
#endif
    int             ii;
    long            iilong;
    struct tcdfvar *tvp = testvars_g;
    union getret    got;

    ncopts = NC_VERBOSE; /* errors non fatal */

    id = nccreate(fname, NC_NOCLOBBER);
    if (id == -1) {
        fprintf(stderr, "trying again\n");
        id = nccreate(fname, NC_CLOBBER);
    }
    if (id == -1)
        exit(errno);

#ifdef NOBUF
    cdf_assert(ncnobuf(id) != 1);
#endif /* NOBUF */

    cdf_assert(ncattput(id, NC_GLOBAL, "TITLE", NC_CHAR, 12, "another name") != -1);
    cdf_assert(ncattget(id, NC_GLOBAL, "TITLE", (ncvoid *)new) != -1);
    /*	printf("title 1 \"%s\"\n", new) ; */
    cdf_assert(ncattput(id, NC_GLOBAL, "TITLE", NC_CHAR, strlen(fname), fname) != -1);
    cdf_assert(ncattget(id, NC_GLOBAL, "TITLE", (ncvoid *)new) != -1);
    new[strlen(fname)] = 0;
    /*	printf("title 2 \"%s\"\n", new) ; */
    cdf_assert(strcmp(fname, new) == 0);
    cdf_assert(ncattput(id, NC_GLOBAL, "RCSID", NC_CHAR, strlen(mrcsid), (ncvoid *)mrcsid) != -1);

    createtestdims(id, NUM_DIMS, sizes_g, dim_names_g);
    testdims(id, NUM_DIMS, sizes_g, dim_names_g);

    createtestvars(id, testvars_g, NUM_TESTVARS);

    {
        long   lfill = -1;
        double dfill = -9999;
        cdf_assert(ncattput(id, Long_id, _FillValue, NC_LONG, 1, (ncvoid *)&lfill) != -1);
        cdf_assert(ncattput(id, Double_id, _FillValue, NC_DOUBLE, 1, (ncvoid *)&dfill) != -1);
    }

#ifdef REDEF
    cdf_assert(ncendef(id) != -1);
    cdf_assert(ncvarput1(id, Long_id, indices[3], (ncvoid *)&birthday) != -1);
    fill_seq(id);
    cdf_assert(ncredef(id) != -1);
#endif

    cdf_assert(ncdimrename(id, 1, "IXX") >= 0);
    cdf_assert(ncdiminq(id, 1, new, &iilong) >= 0);
    printf("dimrename: %s\n", new);
    cdf_assert(ncdimrename(id, 1, dim_names_g[1]) >= 0);

#ifdef ATTRX
    cdf_assert(ncattrename(id, 1, "UNITS", "units") != -1);
    cdf_assert(ncattdel(id, 4, "FIELDNAM") != -1);
    cdf_assert(ncattdel(id, 2, "SCALEMIN") != -1);
    cdf_assert(ncattdel(id, 2, "SCALEMAX") != -1);
#endif /* ATTRX */

    cdf_assert(ncendef(id) != -1);

#ifndef REDEF
    fill_seq(id);
    cdf_assert(ncvarput1(id, Long_id, indices[3], (char *)&birthday) != -1);
#endif

    cdf_assert(ncvarput(id, Byte_id, s_start, s_edges, (ncvoid *)sentence) != -1);

    cdf_assert(ncvarput1(id, Byte_id, indices[6], (ncvoid *)(chs + 1)) != -1);
    cdf_assert(ncvarput1(id, Byte_id, indices[5], (ncvoid *)chs) != -1);
    cdf_assert(ncvarput1(id, Short_id, indices[4], (ncvoid *)shs) != -1);
    cdf_assert(ncvarput1(id, Float_id, indices[2], (ncvoid *)&e) != -1);
    cdf_assert(ncvarput1(id, Double_id, indices[1], (ncvoid *)&zed) != -1);
    cdf_assert(ncvarput1(id, Double_id, indices[0], (ncvoid *)&pinot) != -1);

#ifdef SYNCDEBUG
    printf("Hit Return to sync\n");
    gets(str);
    ncsync(id, 0);
    printf("Sync done. Hit Return to continue\n");
    gets(str);
#endif /* SYNCDEBUG */

    ret = ncclose(id);
    printf("ncclose ret = %d\n\n", (int)ret);

    /*
     *	read it
     */
    id = ncopen(fname, NC_NOWRITE);
    if (id == -1) {
        printf("Could not open %s\n", fname);
        exit(1);
    }
    printf("reopen id = %d for filename %s\n", (int)id, fname);

#ifdef NOBUF
    cdf_assert(ncnobuf(id) != 1);
#endif /* NOBUF */

    /*	NC	*/
    printf("NC ");
    cdf_assert(
        ncinquire(id, &(cdesc->num_dims), &(cdesc->num_vars), &(cdesc->num_attrs), &(cdesc->xtendim)) == id);
    if (cdesc->num_dims != num_dims_g) {
        printf(" num_dims  : %d != %d\n", cdesc->num_dims, num_dims_g);
        exit(1);
    }
    cdf_assert(cdesc->num_vars == NUM_TESTVARS);
    printf("done\n");

    /*	GATTR	*/
    printf("GATTR ");
    cdf_assert(cdesc->num_attrs == 2);

    cdf_assert(ncattname(id, NC_GLOBAL, 0, adesc->mnem) == 0);
    cdf_assert(strcmp("TITLE", adesc->mnem) == 0);
    cdf_assert(ncattinq(id, NC_GLOBAL, adesc->mnem, &(adesc->type), &(adesc->len)) != -1);
    cdf_assert(adesc->type == NC_CHAR);
    cdf_assert(adesc->len == strlen(fname));
    cdf_assert(ncattget(id, NC_GLOBAL, "TITLE", (ncvoid *)new) != -1);
    new[adesc->len] = 0;
    cdf_assert(strcmp(fname, new) == 0);
    /*	printf("Global %s %s\n", adesc->mnem, new) ; */

    cdf_assert(ncattname(id, NC_GLOBAL, 1, adesc->mnem) == 1);
    cdf_assert(strcmp("RCSID", adesc->mnem) == 0);
    cdf_assert(ncattinq(id, NC_GLOBAL, adesc->mnem, &(adesc->type), &(adesc->len)) != -1);
    cdf_assert(adesc->type == NC_CHAR);
    cdf_assert(adesc->len == strlen(mrcsid));
    cdf_assert(ncattget(id, NC_GLOBAL, "RCSID", (ncvoid *)new) != -1);
    new[adesc->len] = 0;
    cdf_assert(strcmp(mrcsid, new) == 0);
    /*	printf("Global %s %s\n", adesc->mnem, new) ; */

    /*	VAR	*/
    printf("VAR ");
    cdf_assert(cdesc->num_vars == NUM_TESTVARS);

    for (ii = 0; ii < cdesc->num_vars; ii++, tvp++) {
        int jj;
        cdf_assert(ncvarinq(id, ii, vdesc->mnem, &(vdesc->type), &(vdesc->ndims), vdesc->dims,
                            &(vdesc->num_attrs)) == ii);
        if (strcmp(tvp->mnem, vdesc->mnem) != 0) {
            printf("attr %d mnem mismatch %s, %s\n", (int)ii, tvp->mnem, vdesc->mnem);
            continue;
        }
        if (tvp->type != vdesc->type) {
            printf("attr %d type mismatch %d, %d\n", (int)ii, tvp->type, (int)vdesc->type);
            continue;
        }
        for (jj = 0; jj < vdesc->ndims; jj++) {
            if (tvp->dims[jj] != vdesc->dims[jj]) {
                printf("inconsistent dim[%d] for variable %d: %d != %d\n", (int)jj, (int)ii,
                       (int)tvp->dims[jj], (int)vdesc->dims[jj]);
                continue;
            }
        }

        /* VATTR */
        printf("VATTR\n");
        for (jj = 0; jj < vdesc->num_attrs; jj++) {
            cdf_assert(ncattname(id, ii, jj, adesc->mnem) == jj);
            if (strcmp(adesc->mnem, reqattr[jj]) != 0) {
                printf("var %d attr %d mismatch %s != %s\n", (int)ii, (int)jj, adesc->mnem, reqattr[jj]);
                break;
            }
        }

        if (ncattinq(id, ii, reqattr[0], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_CHAR);
            cdf_assert(adesc->len == strlen(tvp->units));
            cdf_assert(ncattget(id, ii, reqattr[0], (ncvoid *)new) != -1);
            new[adesc->len] = 0;
            cdf_assert(strcmp(tvp->units, new) == 0);
        }

        if (ncattinq(id, ii, reqattr[1], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_DOUBLE);
            cdf_assert(adesc->len == 1);
            cdf_assert(ncattget(id, ii, reqattr[1], (ncvoid *)&got) != -1);
            chkgot(adesc->type, got, tvp->validmin);
        }

        if (ncattinq(id, ii, reqattr[2], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_DOUBLE);
            cdf_assert(adesc->len == 1);
            cdf_assert(ncattget(id, ii, reqattr[2], (ncvoid *)&got) != -1);
            chkgot(adesc->type, got, tvp->validmax);
        }

        if (ncattinq(id, ii, reqattr[3], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_DOUBLE);
            cdf_assert(adesc->len == 1);
            cdf_assert(ncattget(id, ii, reqattr[3], (ncvoid *)&got) != -1);
            chkgot(adesc->type, got, tvp->scalemin);
        }

        if (ncattinq(id, ii, reqattr[4], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_DOUBLE);
            cdf_assert(adesc->len == 1);
            cdf_assert(ncattget(id, ii, reqattr[4], (ncvoid *)&got) != -1);
            chkgot(adesc->type, got, tvp->scalemax);
        }

        if (ncattinq(id, ii, reqattr[5], &(adesc->type), &(adesc->len)) != -1) {
            cdf_assert(adesc->type == NC_CHAR);
            cdf_assert(adesc->len == strlen(tvp->fieldnam));
            cdf_assert(ncattget(id, ii, reqattr[5], (ncvoid *)new) != -1);
            new[adesc->len] = 0;
            cdf_assert(strcmp(tvp->fieldnam, new) == 0);
        }
    }

    printf("fill_seq ");
    check_fill_seq(id);
    printf("Done\n");

    cdf_assert(ncvarget1(id, Double_id, indices[0], (ncvoid *)&got) != -1);
    printf("got val = %f\n", got.dbl);

    cdf_assert(ncvarget1(id, Double_id, indices[1], (ncvoid *)&got) != -1);
    printf("got val = %f\n", got.dbl);

    cdf_assert(ncvarget1(id, Float_id, indices[2], (ncvoid *)&got) != -1);
    printf("got val = %f\n", (double)got.fl[0]);

    cdf_assert(ncvarget1(id, Long_id, indices[3], (ncvoid *)&got) != -1);
    printf("got val = %ld\n", (long)got.lng[0]);

    cdf_assert(ncvarget1(id, Short_id, indices[4], (ncvoid *)&got) != -1);
    printf("got val = %d\n", (int)got.sh[0]);

    cdf_assert(ncvarget1(id, Byte_id, indices[5], (ncvoid *)&got) != -1);
    printf("got val = %c (0x%02x) \n", got.by[0], got.by[0]);

    cdf_assert(ncvarget1(id, Byte_id, indices[6], (ncvoid *)&got) != -1);
    printf("got val = %c (0x%02x) \n", got.by[0], got.by[0]);

    /* memset(new,0,256) ; */
    {
        char *cp = new;
        for (; cp < &new[sizeof(new) - 1]; *cp++ = 0)
            ;
    }
    cdf_assert(ncvarget(id, Byte_id, s_start, s_edges, (ncvoid *)new) != -1);
    printf("got val = \"%s\"\n", new);

    ret = ncclose(id);
    printf("re ncclose ret = %d\n", (int)ret);

    return EXIT_SUCCESS;
}
