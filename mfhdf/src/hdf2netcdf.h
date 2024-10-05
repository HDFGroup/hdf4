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

#ifndef MFH4_HDF_2_NETCDF_H
#define MFH4_HDF_2_NETCDF_H

#include "hdf.h"

/* Prepend 'sd_' to all NetCDF function names to avoid name clash when the
 * HDF and NetCDF headers are included in the same application.
 */
#define HNAME(x) sd_##x 

/* Variables */
#define ncerr       HNAME(ncerr)
#define ncopts      HNAME(ncopts)

/* API calls */
#define nccreate    HNAME(nccreate)         /* used in src - mfsd.c */
#define ncopen      HNAME(ncopen)           /* used in src - mfsd.c */
#define ncendef     HNAME(ncendef)
#define ncclose     HNAME(ncclose)          /* used in src - mfsd.c */
#define ncabort     HNAME(ncabort)          /* used in src - file.c */
#define ncdimdef    HNAME(ncdimdef)
#define ncdiminq    HNAME(ncdiminq)

/* The ncvar* calls are used in a bunch of tests - can these be removed? */
#define ncvardef    HNAME(ncvardef)
#define ncvarid     HNAME(ncvarid)
#define ncvarinq    HNAME(ncvarinq)
#define ncvarput1   HNAME(ncvarput1)
#define ncvarget1   HNAME(ncvarget1)
#define ncvarput    HNAME(ncvarput)
#define ncvarget    HNAME(ncvarget)         /* used in src - array.c */
#define ncvarputs   HNAME(ncvarputs)
#define ncvargets   HNAME(ncvargets)
#define ncvarputg   HNAME(ncvarputg)
#define ncvargetg   HNAME(ncvargetg)

#define ncattput    HNAME(ncattput)
#define nctypelen   HNAME(nctypelen)        /* used in src - array.c, nssdc.c, putget.c */
#define ncsetfill   HNAME(ncsetfill)        /* used in src - mfsd.c */

#endif /* MFH4_HDF_2_NETCDF_H */
