
/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef HDIFF_SDS_H__
#define HDIFF_SDS_H__

#include "hdf.h"
#include "mfhdf.h"
#include "dumplib.h"
#include "hdiff_table.h"
#include "hdiff.h"





#ifdef __cplusplus
extern "C" {
#endif

int diff_sds(char  *fname1, 
             char  *fname2, 
             int32 ref1,
             int32 ref2,
             struct fspec fspec);




#ifdef __cplusplus
}
#endif


#endif

