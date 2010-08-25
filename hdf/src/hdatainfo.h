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

/* $Id: hproto.h 5400 2010-04-22 03:45:32Z bmribler $ */

#ifndef _H_DATAINFO
#define _H_DATAINFO

#include "H4api_adpt.h"

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

#if 0
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
#endif

/* Public functions for getting raw data information */

    HDFLIBAPI intn HDgetdatainfo
                (int32 file_id, uint16 data_tag, uint16 data_ref,
                 uintn start_block, uintn info_count,
                 int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn HDgetdatainfo_count
                (int32 file_id, uint16 data_tag, uint16 data_ref);

    HDFLIBAPI intn VSgetdatainfo
		(int32 vsid, uintn start_block, uintn info_count, int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn VSgetattdatainfo
		(int32 vsid, int32 findex, intn attrindex, int32 *offset, int32 *length);

    HDFLIBAPI intn Vgetattdatainfo
		(int32 vgid, intn attrindex, int32 *offset, int32 *length);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _H_DATAINFO */

