/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "mfhdf.h"

#include "hdftest.h"

/* all test functions to be called in main */
extern int test_unlim();
extern int test_ncunlim();
extern int test_ncvargetfill();

int 
main(int argc, char *argv[])
{
    intn  status;          /* status flag */
    int   num_errs = 0;    /* number of errors so far */

    /* Tests reading/writing datasets with unlimited dimension via HDF
       API (bugzilla 1378) -BMR, Jan 07, 2009 */
    status = test_unlim();  /* in tunlim.c */
    num_errs = num_errs + status;

    /* Tests reading/writing variables with unlimited dimension via nc
       API (bugzilla 1378) -BMR, Jan 07, 2009 */
    status = test_ncunlim();  /* in tncunlim.c */
    num_errs = num_errs + status;

    /* Tests ncvarget in filling fill-values where appropriate (HDFFR-1390)
       -BMR, Sep 05, 2013 */
    status = test_ncvargetfill();  /* in tncvargetfill.c */
    num_errs = num_errs + status;

    if (num_errs == 0)
	printf("*** HDF-nc test passes ***\n");
    else                                                          \
	printf("*** HDF-nc test fails ***\n");
    return num_errs;
}

