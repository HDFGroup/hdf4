/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1993, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
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

#ifndef MFH4_ERROR_H
#define MFH4_ERROR_H

#include "hdf.h"

HDFLIBAPI void nc_serror(const char *fmt, ...);
HDFLIBAPI void NCadvise(int err, const char *fmt, ...);

#endif /* MFH4_ERROR_H */
