
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

/*-----------------------------------------------------------------------------
  * File:    cszip.h
  * Purpose: Header file for szip encoding information.
  * Dependencies: should only be included from hcompi.h
  * Invokes: none
  * Contents: Structures & definitions for szip encoding.  This header
  *              should only be included in hcomp.c and cszip.c.
  * Structure definitions:
  * Constant definitions:
  *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef __CSZIP_H
#define __CSZIP_H

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
    ** from cszip.c
  */

     extern int32 HCPcszip_stread
                 (accrec_t * rec);

     extern int32 HCPcszip_stwrite
                 (accrec_t * rec);

     extern int32 HCPcszip_seek
                 (accrec_t * access_rec, int32 offset, int origin);

     extern int32 HCPcszip_inquire
                 (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, 
uint16 *pref,
                int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                  int16 *pspecial);

     extern int32 HCPcszip_read
                 (accrec_t * access_rec, int32 length, void * data);

     extern int32 HCPcszip_write
                 (accrec_t * access_rec, int32 length, const void * data);

     extern intn HCPcszip_endaccess
                 (accrec_t * access_rec);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */


/* SZIP [en|de]coding information */
typedef struct
{
     int32       offset;    /* offset in the file */
     uint8       *buffer;   /* buffer for storing SZIP bytes */
         int32       buffer_pos;
         int32           buffer_size;
         int32 bits_per_pixel;
         int32 compression_mode;
         int32 options_mask;
         int32 pixels;
         int32 pixels_per_block;
         int32 pixels_per_scanline;
     enum
       {
           SZIP_INIT,                    /* initialize ... ***/
           SZIP_RUN,              /* buffer up to the current position is a 
run */
                   SZIP_TERM
       }
     szip_state;                  /* state of the buffer storage */
}
comp_coder_szip_info_t;

#ifndef CSZIP_MASTER
extern funclist_t cszip_funcs;   /* functions to perform szip encoding */
#else
funclist_t  cszip_funcs =
{                               /* functions to perform szip encoding */
     HCPcszip_stread,
     HCPcszip_stwrite,
     HCPcszip_seek,
     HCPcszip_inquire,
     HCPcszip_read,
     HCPcszip_write,
     HCPcszip_endaccess
};
#endif

#define ALLOW_K13_OPTION_MASK             1
#define CHIP_OPTION_MASK                          2
#define EC_OPTION_MASK                            4
#define LSB_OPTION_MASK                           8
#define MSB_OPTION_MASK                         16
#define NN_OPTION_MASK                          32
#define OVERWRITE_OPTION_MASK           64
#define RAW_OPTION_MASK                         128
#define KEEP_IMAGE_OPTION_MASK          256
#define KEEP_COMPRESSED_OPTION_MASK     512

#endif /* __CSZIP_H */
