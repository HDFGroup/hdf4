/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_EMALLOC_H
#define NCTEST_EMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

extern void *emalloc(size_t size);

extern void *erealloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_EMALLOC_H */
