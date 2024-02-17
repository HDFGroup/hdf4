/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "hdf.h"

#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#include "testcdf.h" /* defines in-memory test cdf structure */
#include "add.h"     /* functions to update in-memory netcdf */
#include "val.h"
#include "error.h"
#include "tests.h"
#include "emalloc.h"

#define max(A, B) ((A) > (B) ? (A) : (B))

/*
 * For every variable in open netcdf, puts and gets three hypercubes
 * of data of the appropriate type, comparing values from get to
 * values put to check that both ncvarput and ncvarget worked.  The
 * three hypercubes are
 *    - a large hypercube from (0, 0, ...) to the far corner (diagonally
 *      opposite (0, 0, ...),
 *    - a size 1 hypercube from the far corner with edge lengths of 1
 *      in every direction, and
 *    - a hypercube starting about 1/3 of the way along the diagonal
 *      from (0,0,...) extending 1/3 of the way in every direction
 *      toward the far corner.
 */

/* cdfid - handle of netcdf open and in data mode */
int
test_varputget(int cdfid)
{
    int         nerrs   = 0;
    static char pname[] = "test_varputget";
    int         id, ie, iv;         /* loop indices */
    int         ne = 3;             /* number of test hypercubes for each var */
    struct cdfhc {                  /* a hypercube with generic values */
        long  cor[H4_MAX_VAR_DIMS]; /* netcdf coordinates for lower corner */
        long  edg[H4_MAX_VAR_DIMS]; /* netcdf edge lengths to upper corner */
        void *vals;                 /* pointer to block of values */
    } hc[3], tmp;                   /* test hypercubes */
    int nel[3];                     /* number of elements in hypercube */

    for (iv = 0; iv < test_g->nvars; iv++) { /* for each var in netcdf */
        for (ie = 0; ie < ne; ie++)
            nel[ie] = 1; /* to compute space for hypercube values */

        for (id = 0; id < test_g->vars[iv].ndims; id++) { /* set cubes */

            /* max dimension size, 5 for records */
            int dsize = test_g->dims[test_g->vars[iv].dims[id]].size;
            if (dsize == NC_UNLIMITED)
                dsize = 5;

            /* start at "lower-left" corner, do whole variable */
            hc[0].cor[id] = 0;
            hc[0].edg[id] = dsize;
            nel[0] *= hc[0].edg[id];

            /* start at "upper-right" corner, do one point */
            hc[1].cor[id] = dsize - 1;
            hc[1].edg[id] = 1;
            nel[1] *= hc[1].edg[id];

            /* start about 1/3 way along diagonal, do 1/3 in each direction */
            hc[2].cor[id] = dsize / 3;
            hc[2].edg[id] = max(dsize / 3, 1);
            nel[2] *= hc[2].edg[id];
        }
        for (ie = 0; ie < ne; ie++) { /* for each of ne points */

            /* allocate space for the cube of values */
            hc[ie].vals = emalloc(nel[ie] * nctypelen(test_g->vars[iv].type) + 8);
            tmp.vals    = emalloc(nel[ie] * nctypelen(test_g->vars[iv].type) + 8);

            /* fill allocated space with different values of right type */
            val_fill(test_g->vars[iv].type, nel[ie], hc[ie].vals);

            if (ncvarput(cdfid, iv, hc[ie].cor, hc[ie].edg, hc[ie].vals) == -1) {
                error("%s: ncvarput failed for point %d, variable %s", pname, ie, test_g->vars[iv].name);
                nerrs++;
                errvar(test_g, &test_g->vars[iv]);
                fprintf(stderr, "  corner = (");
                for (id = 0; id < test_g->vars[iv].ndims; id++)
                    fprintf(stderr, "%d%s", (int)hc[ie].cor[id],
                                  (id < test_g->vars[iv].ndims - 1) ? ", " : "");
                fprintf(stderr, ")\n");
                fprintf(stderr, "  edge = (");
                for (id = 0; id < test_g->vars[iv].ndims; id++)
                    fprintf(stderr, "%d%s", (int)hc[ie].edg[id],
                                  (id < test_g->vars[iv].ndims - 1) ? ", " : "");
                fprintf(stderr, ")\n");
            }
            else {
                add_data(test_g, iv, hc[ie].cor, hc[ie].edg); /* keep test in sync */
                if (ncvarget(cdfid, iv, hc[ie].cor, hc[ie].edg, tmp.vals) == -1) {
                    error("%s: ncvarget failed for point %d, variable %s", pname, ie, test_g->vars[iv].name);
                    nerrs++;
                }
                else {
                    if (val_cmp(test_g->vars[iv].type, nel[ie], hc[ie].vals, tmp.vals) != 0) {
                        error("%s: bad values returned from ncvarget", pname);
                        nerrs++;
                        errvar(test_g, &test_g->vars[iv]); /* describe var */
                    }
                }
            }
            free(hc[ie].vals);
            free(tmp.vals);
        }
    }
    return nerrs;
}
