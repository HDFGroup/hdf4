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

/* $Id$ */

#ifndef DFUFP2IM_H /* avoid re-inclusion */
#define DFUFP2IM_H

/*
* definitions of structs used by routines: Input & Output
*/

struct Input {
    int32
        hdim, vdim;     /* horizontal and vertical dimensions of input data */
    int
        is_pal,         /* flag to tell whether there is a palette */
        is_vscale,      /* flags telling whether scales were included  */
        is_hscale,
        ct_method;      /* color transform method: EXPAND or INTERP */
    float32
        max, min,        /* max and min values of the data */
        *hscale,*vscale, /* horizontal and vertical scales */
        *data;           /* floating point data */
};

struct Output {
    int32
        hres,vres;  /* horizontal and vertical resolution of image */
    int compress;   /* compression scheme */
    char
        outfile[32]; /* output file name */
    uint8
        *image;     
    uint8
        *palette;   
};

/*----------------------------------------------------------------------------*/
/*                           Function Prototypes                              */

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

/* prototypes for dfufp2im.c */

#ifdef OLD_WAY
extern int duif2i_(int32 *hdim, int32 *vdim, float32 *max, float32 *min,
		   float32 hscale[], float32 vscale[], float32 data[],
           uint8 *palette, _fcd outfile, int *ct_method, int32 *hres,
           int32 *vres, int *compress, int *lenfn);
extern int DFUfptoimage(int32 hdim, int32 vdim, float32 max, float32 min,
                        float32 *hscale, float32 *vscale, float32 *data,
                        uint8 *palette, char *outfile, int ct_method,
                        int32 hres, int32 vres, int compress);
#endif
extern int process
    PROTO((struct Input *in, struct Output *out));
extern int generate_scale
    PROTO((int32 dim, float32 *scale));
extern int convert_interp
    PROTO((struct Input *in, struct Output *out));
extern int pixrep_scaled
    PROTO((struct Input *in, struct Output *out));
extern int compute_offsets
    PROTO((float32 *scale, int32 dim, int32 *offsets, int32 res));
extern int pixrep_simple
    PROTO((struct Input *in, struct Output *out));

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

#endif /* DFUFP2IM_H */
