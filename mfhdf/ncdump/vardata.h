/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCDUMP_VARDATA_H
#define NCDUMP_VARDATA_H

extern char *progname; /* for error messages */

#ifdef __cplusplus
extern "C" {
#endif

/* Output the data for a single variable, in CDL syntax. */
extern int vardata(struct ncvar *, /* variable */
                   long[],         /* variable dimension sizes */
                   int,            /* netcdf id */
                   int,            /* variable id */
                   struct fspec *  /* formatting specs */
);

#ifdef __cplusplus
}
#endif

#endif /* NCDUMP_VARDATA_H */
