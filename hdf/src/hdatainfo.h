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

/* Activate raw datainfo interface - added for hmap project in 2010 */
#if defined DATAINFO_MASTER | defined DATAINFO_TESTER

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/* Public functions for getting raw data information */

    HDFLIBAPI intn ANgetdatainfo
		(int32 ann_id, int32 *offset, int32 *length);

    HDFLIBAPI intn HDgetdatainfo
		(int32 file_id, uint16 data_tag, uint16 data_ref,
		 int32 *chk_coord, uintn start_block, uintn info_count,
		 int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn HDgetdatainfo_count
		(int32 file_id, uint16 data_tag, uint16 data_ref, int32 *chk_coord);

    HDFLIBAPI intn VSgetdatainfo
		(int32 vsid, uintn start_block, uintn info_count,
		 int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn VSgetattdatainfo
		(int32 vsid, int32 findex, intn attrindex, int32 *offset, int32 *length);

    HDFLIBAPI intn Vgetattdatainfo
		(int32 vgid, intn attrindex, int32 *offset, int32 *length);

    HDFLIBAPI intn GRgetdatainfo
		(int32 riid, uintn start_block, uintn info_count,
		 int32 *offsetarray, int32 *lengtharray);

    HDFLIBAPI intn GRgetattdatainfo
		(int32 id, intn attrindex, int32 *offset, int32 *length);

    /* For temporary use by hmap writer to detect IMCOMP.  -BMR, Mar 11, 2011 */
    HDFLIBAPI intn grgetcomptype
		(int32 riid, int32 *comp_type);

#if defined c_plusplus || defined __cplusplus
}
#endif				/* c_plusplus || __cplusplus */
#endif				/* DATAINFO_MASTER || DATAINFO_TESTER */
#endif                          /* _H_DATAINFO */

