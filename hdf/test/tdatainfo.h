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

/* $Id: tutils.h 5335 2010-01-28 06:28:23Z bmribler $ */

#ifndef _TDATAINFO_H
#define _TDATAINFO_H

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
intn readnoHDF_char(const char *filename, const int32 offset, const int32 length, const char *orig_buf);

#endif /* _TDATAINFO_H */
