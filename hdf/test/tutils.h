/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

#ifndef _TUTILS_H
#define _TUTILS_H

/* Define these for use in all the tests */
#ifndef TESTMASTER
extern
#endif
int         num_errs
#ifdef TESTMASTER
= 0
#endif
,           Verbosity
#ifdef TESTMASTER
= 0
#endif
           ;

#ifdef TEST_PC
#define FAR far
#else
#define FAR	/* */
#endif /* TEST_PC */

/* Used to make certain a return value _is_not_ a value */
#define CHECK(ret, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s returned %d \n",where,__LINE__,__FILE__,(int)ret);\
if(ret == val) {printf("*** UNEXPECTED RETURN from %s is %d at line %4d in %s\n", where, (int)ret, __LINE__,__FILE__); num_errs++;} \
} while(0)

/* Used to make certain a return value _is_ a value */
#define VERIFY(x, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s had value %d \n",where,__LINE__,__FILE__,(int)x);\
if(x != val) {printf("*** UNEXPECTED VALUE from %s is %d at line %4d in %s\n", where, (int)x,__LINE__,__FILE__); num_errs++;} \
} while(0)

#define RESULT(a) \
do { \
if (Verbosity>8) printf("   Call to HDF routine: %15s at line %4d in %s returned %d \n",a,__LINE__,__FILE__,(int)ret); \
if (Verbosity>9) HEprint(stdout,0); \
if(ret == FAIL) {printf("*** UNEXPECTED RETURN from %s is %d at line %4d in %s\n", a, (int)ret,__LINE__,__FILE__); num_errs++;} \
} while(0)

#define MESSAGE(v,a) {if (Verbosity>v) {a}}

#endif /* _TUTILS_H */
