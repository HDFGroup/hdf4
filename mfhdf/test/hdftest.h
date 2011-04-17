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

/*
 * The name of the test is printed by saying TESTING("something") which will
 * result in the string `Testing something' being flushed to standard output.
 * If a test passes, fails, or is skipped then the PASSED(), H4_FAILED(), or
 * SKIPPED() macro should be called.  After H4_FAILED(), the caller
 * should print additional information to stdout indented by at least four
 * spaces.
 */
#define TESTING(WHAT)   {printf("Testing %-62s",WHAT); fflush(stdout);}
#define PASSED()        {puts(" PASSED");fflush(stdout);}
#define H4_FAILED()     {puts("*FAILED*");fflush(stdout);}
#define H4_WARNING()    {puts("*WARNING*");fflush(stdout);}
#define SKIPPED()       {puts(" -SKIP-");fflush(stdout);}

/* Check status value and print error message */
#define CHECK(status, fail_value, name) {if(status == fail_value) { \
    fprintf(stderr, "*** Routine %s FAILED at line %d ***\n", name, __LINE__); num_errs++;}}

/* Check status value and print error message, including index */
#define CHECK_IND(status, fail_value, name, index) {if(status == fail_value) { \
    fprintf(stderr, "*** Routine %s FAILED at line %d for SDS index %d ***\n", name, __LINE__, index); num_errs++;}}

/* Verify that a value is as expected and, if not, print error message */
#define VERIFY(item, value, test_name) {if(item != value) { \
    fprintf(stderr, "*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", test_name, (long)item,(int)__LINE__,__FILE__); num_errs++;}}

/* Verify that a value of type char* is as expected and, if not, print 
   error message */
#define VERIFY_CHAR(item, value, test_name) {if(HDstrcmp(item, value) != 0) { \
    fprintf(stderr, "*** UNEXPECTED VALUE from %s is <%s> at line %4d in %s\n", test_name, item,(int)__LINE__,__FILE__); num_errs++;}}

/* Used to validate that 'buffer' has been successfully allocated */
#define CHECK_ALLOC(buffer, buf_name, func_name ) { \
    if (buffer == NULL) {\
        fprintf(stderr,"in %s: space allocation for %s failed.  Terminated!\n",\
                func_name, buf_name ); \
        exit(1); }  \
}


