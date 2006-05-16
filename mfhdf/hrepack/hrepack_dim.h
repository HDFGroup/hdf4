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


#ifndef REPACK_DIM_H_
#define REPACK_DIM_H_

#include "hrepack.h"


#ifdef __cplusplus
extern "C" {
#endif


void match_dim(int32 sd_in,
               int32 sd_out,
               table_t *td1,
               table_t *td2,
               options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_DIM_H_ */
