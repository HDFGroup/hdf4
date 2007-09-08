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

/* Check status value and print error message */
#define CHECK(status, fail_value, name) {if(status == fail_value) { \
    printf("*** Routine %s FAILED at line %d ***\n", name, __LINE__); num_errs++;}}

/* Verify that a value is as expected and, if not, print error message */
#define VERIFY(item, value, test_name) {if(item != value) { \
    printf("*** UNEXPECTED VALUE from %s is %ld at line %4d in %s\n", test_name, (long)item,(int)__LINE__,__FILE__); num_errs++;}}

/* Verify that a value of type char* is as expected and, if not, print 
   error message */
#define VERIFY_CHAR(item, value, test_name) {if(HDstrcmp(item, value) != 0) { \
    printf("*** UNEXPECTED VALUE from %s is <%s> at line %4d in %s\n", test_name, item,(int)__LINE__,__FILE__); num_errs++;}}

