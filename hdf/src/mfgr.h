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

/*-----------------------------------------------------------------------------
 * File:    mfgr.h
 * Purpose: header file for multi-file general raster information
 *---------------------------------------------------------------------------*/

#ifndef H4_MFGR_H
#define H4_MFGR_H

#include "hdf.h"

/* Interlace types available */
typedef int16 gr_interlace_t;
#define MFGR_INTERLACE_PIXEL     0 /* pixel interlacing scheme */
#define MFGR_INTERLACE_LINE      1 /* line interlacing scheme */
#define MFGR_INTERLACE_COMPONENT 2 /* component interlacing scheme */

#endif /* H4_MFGR_H */
