/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_VAL_H
#define NCTEST_VAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* fill typed value block with values of specified type */
extern void val_fill(nc_type, long, void *);

/* fill typed value block with zeros of specified type */
extern void val_fill_zero(nc_type, long, void *);

/*
 * compare two typed value blocks, return 0 if equal, 1+n otherwise,
 * where n is the index of the first differing element.
 */
extern int val_cmp(nc_type, long, void *, void *);

/* print typed value block with values of specified type */
extern void val_out(nc_type, long, void *);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_VAL_H */
