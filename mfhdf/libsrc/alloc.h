/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MFH4_ALLOC_H
#define MFH4_ALLOC_H

#include <stdlib.h>

#ifdef HDF
#define Alloc(theNum, theType) (theType *)HDmalloc(sizeof(theType) * (theNum))
#define Free(s) HDfree(s)
#else
#define Alloc(theNum, theType) (theType *)malloc(sizeof(theType) * (theNum))
#define Free(s) free(s)
#endif

/* We need to define these to standard ones when HDF is not defined */
#ifndef HDF
#define HDcalloc(nelem, elsize) calloc(nelem, elsize)
#define HDmemset(dst, c, n)     memset(dst, c, n)
#define HDrealloc(p, s)         realloc(p, s)
#define HDmalloc(s)             malloc(s)
#define HDfree(s)               free(s)
#endif

#define ARRAYLEN(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* MFH4_ALLOC_H */
