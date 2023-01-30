/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_ADD_H
#define NCTEST_ADD_H

#ifdef __cplusplus
extern "C" {
#endif

/* add a dimension to an in-memory netcdf structure */
extern void add_dim(struct netcdf *, struct cdfdim *);

/* add a variable var to an in-memory netcdf structure */
extern void add_var(struct netcdf *, struct cdfvar *);

/* add an attribute att to an in-memory netcdf structure */
extern void add_att(struct netcdf *, int, struct cdfatt *);

/* reset in-memory netcdf structure to empty */
extern void add_reset(struct netcdf *);

/* delete an attribute att from an in-memory netcdf structure */
extern void del_att(struct netcdf *, int, struct cdfatt *);

/* keep max record written updated */
extern void add_data(struct netcdf *, int, long[], long[]);

/* display info about variable, for error messages */
extern void errvar(struct netcdf *, struct cdfvar *);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_ADD_H */
