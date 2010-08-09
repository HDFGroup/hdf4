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
#include "h4config.h"

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

    HDFLIBAPI intn HDgetdatainfo
                (int32 file_id, uint16 data_tag, uint16 data_ref,
                 uintn start_block, uintn info_count,
		 hdf_datainfo_t* data_info);

    HDFLIBAPI intn HDgetdatainfo_count
                (int32 file_id, uint16 data_tag, uint16 data_ref,
		 uintn* info_count);

    HDFLIBAPI intn VSgetdatainfo
		(int32 vdata_id, uintn start_block, uintn info_count, int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn VSgetattdatainfo
		(int32 vkey, int32 *offsetarray, int32 *lengtharray);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _H_DATAINFO */

