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

/*  hfile.h
 *  Header for hfile.c, routines for low level data element I/O
 */

#ifndef H4_HFILE_H
#define H4_HFILE_H

#include "hdf.h"

/* ----------------------------- Version Tags ----------------------------- */
/* Library version numbers */

#define LIBVER_MAJOR      4
#define LIBVER_MINOR      3
#define LIBVER_RELEASE    1
#define LIBVER_SUBRELEASE "1" /* For pre-releases like snap0 */
                              /* Empty string for real releases */
#define LIBVER_STRING "HDF Version 4.3 Release 1-1, February 29, 2024"
#define LIBVSTR_LEN   80 /* Length of version string */
#define LIBVER_LEN    92 /* 4+4+4+80 = 92 */
/* end of version tags */

#endif /* H4_HFILE_H */
