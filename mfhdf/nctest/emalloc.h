/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_EMALLOC_H
#define NCTEST_EMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

extern void *emalloc(int size);

extern void *erealloc(void *ptr, int size);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_EMALLOC_H */
