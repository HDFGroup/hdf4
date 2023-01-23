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

#ifndef H4_DFUFP2IM_H
#define H4_DFUFP2IM_H

#include "H4api_adpt.h"

/* Input structure */
typedef struct {
    int32    hdim;      /* horizontal dimension of input data */
    int32    vdim;      /* vertical dimension of input data */
    intn     is_pal;    /* flag to tell whether there is a palette */
    intn     is_vscale; /* flag telling whether vertical scale included  */
    intn     is_hscale; /* flag telling whether horizonatal scale included */
    intn     ct_method; /* color transform method: EXPAND or INTERP */
    float32  max;       /* max value of data */
    float32  min;       /* min value of the data */
    float32 *hscale;    /* horizontal scale */
    float32 *vscale;    /* vertical scale */
    float32 *data;      /* floating point data */
} Input;

/* Output structure */
typedef struct {
    int32  hres;        /* horizontal resolution of image */
    int32  vres;        /* vertical resolution of image */
    intn   compress;    /* compression scheme */
    char   outfile[32]; /* output file name */
    uint8 *image;       /* Image */
    uint8 *palette;     /* Palette */
} Output;

/*----------------------------------------------------------------------------*/
/*                           Function Prototypes                              */

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes for dfufp2im.c */

HDFLIBAPI int process(Input *in, Output *out);
HDFLIBAPI int generate_scale(int32 dim, float32 *scale);
HDFLIBAPI int convert_interp(Input *in, Output *out);
HDFLIBAPI int pixrep_scaled(Input *in, Output *out);
HDFLIBAPI int compute_offsets(float32 *scale, int32 dim, int32 *offsets, int32 res);
HDFLIBAPI int pixrep_simple(Input *in, Output *out);
HDFLIBAPI int DFUfptoimage(int32 hdim, int32 vdim, float32 max, float32 min, float32 *hscale, float32 *vscale,
                           float32 *data, uint8 *palette, char *outfile, int ct_method, int32 hres,
                           int32 vres, int compress);

#ifdef __cplusplus
}
#endif

#endif /* H4_DFUFP2IM_H */
