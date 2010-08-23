/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id: tutils.h 5335 2010-01-28 06:28:23Z bmribler $ */

#ifndef _TDATAINFO_H
#define _TDATAINFO_H

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

#ifdef DATAINFO_MASTER
typedef struct
  {
        int32 numtype;  /* number type of data */
        int32 n_values; /* number of values in an SDS, a vdata, or an RI image*/
        int32* offsets; /* offset(s) of data block(s) */
        int32* lengths; /* length(s) of data block(s) */
  }
t_hdf_datainfo_t;

/* Utility functions to allocate and deallocate hdf_datainfo_t's members*/
intn alloc_info(t_hdf_datainfo_t *info, uintn info_count);
void free_info(t_hdf_datainfo_t *info);

/* Other utility functions */
intn readnoHDF_char(const char *filename, const t_hdf_datainfo_t *info, const char *orig_buf);

#endif

#if 0
/* Use %ld to print the value because long could cover most cases. */
/* Used to make certain a return value _is_not_ a value.  If not ture, */
/* print error messages, increment num_err and return. */
#define CHECK(ret, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s returned %ld \n",where,(int)__LINE__,__FILE__,(long)ret);\
if(ret == val) {printf("*** UNEXPECTED RETURN from %s is %ld at line %4d in %s\n", where, (long)ret, (int)__LINE__,__FILE__); num_errs++; return num_errs;} \
} while(0)

#define CHECK_VOID(ret, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s returned %ld \n",where,(int)__LINE__,__FILE__,(long)ret);\
if(ret == val) {printf("*** UNEXPECTED RETURN from %s is %ld at line %4d in %s\n", where, (long)ret, (int)__LINE__,__FILE__); num_errs++; return;} \
} while(0)

/* Same as CHECK except no return but continue. */
#define CHECK_CONT(ret, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s returned %ld \n",where,(int)__LINE__,__FILE__,(long)ret);\
if(ret == val) {printf("*** UNEXPECTED RETURN from %s is %ld at line %4d in %s\n", where, (long)ret, (int)__LINE__,__FILE__); num_errs++;} \
} while(0)

/* Used to validate that 'buffer' has been successfully allocated */
#define CHECK_ALLOC(buffer, buf_name, func_name ) { \
      if (buffer == NULL) {\
         fprintf(stderr,"in %s: space allocation for %s failed.  Terminated!\n",\
                func_name, buf_name ); \
         exit(1); }  \
}

/* Used to make certain a return value _is_ a value */
#define VERIFY(x, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s had value %ld \n",where,(int)__LINE__,__FILE__,(long)x);\
if(x != val) {printf("*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", where, (long)x,(int)__LINE__,__FILE__); num_errs++; return(num_errs);} \
} while(0)

/* Same as VERIFY except return without a value. */
#define VERIFY_VOID(x, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s had value %ld \n",where,(int)__LINE__,__FILE__,(long)x);\
if(x != val) {printf("*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", where, (long)x,(int)__LINE__,__FILE__); num_errs++; return;} \
} while(0)

/* Same as VERIFY except no return but continue. */
#define VERIFY_CONT(x, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s had value %ld \n",where,(int)__LINE__,__FILE__,(long)x);\
if(x != val) {printf("*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", where, (long)x,(int)__LINE__,__FILE__); num_errs++;} \
} while(0)

/* Same as VERIFY except that the value has type char* */
#define VERIFY_CHAR(x, val, where) \
do {if (Verbosity>9) printf("   Call to HDF routine: %15s at line %4d in %s had value %ld \n",where,(int)__LINE__,__FILE__,(long)x);\
if(HDstrcmp(x, val) != 0) {printf("*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", where, (long)x,(int)__LINE__,__FILE__); num_errs++; return(num_errs);} \
} while(0)

#define RESULT(a) \
do { \
if (Verbosity>8) printf("   Call to HDF routine: %15s at line %4d in %s returned %ld \n",a,(int)__LINE__,__FILE__,(long)ret); \
if (Verbosity>9) HEprint(stdout,0); \
if(ret == FAIL) {printf("*** UNEXPECTED RETURN from %s is %ld at line %4d in %s\n", a, (long)ret,(int)__LINE__,__FILE__); num_errs++;} \
} while(0)

#define MESSAGE(v,a) {if (Verbosity>v) {a}}
#endif
#endif /* _TDATAINFO_H */
