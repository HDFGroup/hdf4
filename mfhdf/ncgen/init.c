/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Id$
 *********************************************************************/

#include <stdio.h>
#include "ncgen.h"
#include "genlib.h"

extern int netcdf_flag;
extern int c_flag;
extern int fortran_flag;

struct dims dims[H4_MAX_NC_DIMS];		/* table of netcdf dimensions */

int ncid;			/* handle for netCDF */
int ndims;			/* number of dimensions declared for netcdf */
int nvars;			/* number of variables declared for netcdf */
int natts;			/* number of attributes */
int nvdims;			/* number of dimensions for variables */
int dimnum;			/* dimension number index for variables */
int varnum;			/* variable number index for attributes */
int valnum;			/* value number index for attributes */
int rec_dim;			/* number of the unlimited dimension, if any */
long var_len;			/* variable length (product of dimensions) */
int var_size;			/* size of each element of variable */
long netcdf_record_number;	/* current record number for variables */

struct vars vars[H4_MAX_NC_VARS];	/* should be a malloc'ed list, not an array */

struct atts atts[H4_MAX_NC_ATTRS];	/* should be a malloc'ed list, not an array */

extern void clearout();

void
init_netcdf() {			/* initialize global counts, flags */
    clearout();			/* reset symbol table to empty */
    ndims = 0;
    nvars = 0;
    rec_dim = -1;		/* means no unlimited dimension (yet) */
}
