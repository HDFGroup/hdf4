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

/* $Id$ */

/*
   ***********************************************************************
   ** get version string from an HDF file
   ***********************************************************************
 */

#include <stdio.h>
#include "tproto.h"

void
test_vers(void)
{
    int         ret;
    uint32      lmajor, lminor, lrelease;
    char        lstring[81];

    ret = Hgetlibversion(&lmajor, &lminor, &lrelease, lstring);
    RESULT("Hgetlibversion");
    MESSAGE(5, printf("Library Version\n");
        );
    MESSAGE(5, printf("---------------\n");
        );
    MESSAGE(5, printf("Major:\t\t%u\nMinor:\t\t%u\nRelease:\t%u\nString:\t\t\"%s\"\n", (unsigned) lmajor, (unsigned) lminor, (unsigned) lrelease, lstring);
        );

}
