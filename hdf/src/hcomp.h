/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                      *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

/*-----------------------------------------------------------------------------
 * File:    hcomp.h
 * Purpose: header file for compression information & structures
 * Dependencies: should be included after hdf.h
 * Invokes:
 * Contents:
 * Structure definitions: comp_info
 * Constant definitions: lots...
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef __HCOMP_H
#define __HCOMP_H

/* For determining which type of modeling is being done */
typedef enum {
    COMP_MODEL_STDIO=0      /* for Standard C I/O model */
} comp_model_t;

/* For determining which type of encoding is being done */
typedef enum {
    COMP_CODE_NONE=0,       /* don't encode at all, just store */
    COMP_CODE_RLE,          /* for simple RLE encoding */
    COMP_CODE_NBIT          /* for N-bit encoding */
} comp_coder_t;

/* Compression types available */
#define COMP_NONE       0
#define COMP_JPEG       2
#define COMP_RLE        11
#define COMP_IMCOMP     12

/* Set the following macro to the value the highest compression scheme is */
#define COMP_MAX_COMP   12

#ifndef COMPRESS_MASTER
extern uint16 compress_map[];
#else
uint16 compress_map[COMP_MAX_COMP+1]={  /* Mapping from compression types to tags */
    0,                  /* No corresponding tag for un-compressed data */
    0,                  /* (1) */
    DFTAG_JPEG,         /* COMP_JPEG -> DFTAG_JPEG (for JPEG compression) */
    0,                  /* (3) */
    0,                  /* (4) */
    0,                  /* (5) */
    0,                  /* (6) */
    0,                  /* (7) */
    0,                  /* (8) */
    0,                  /* (9) */
    0,                  /* (10) */
    DFTAG_RLE,          /* COMP_RLE -> DFTAG_RLE (for Run-length compression) */
    DFTAG_IMC           /* COMP_IMCOMP -> DFTAG_IMC (for IMCOMP compression) */
  };
#endif

typedef union tag_model_info {  /* Union to contain modeling information */
    struct {
        int32 nt;       /* number type */
        intn ndim;      /* number of dimensions */
        int32 *dims;    /* array of dimensions */
      } dim;
  } model_info;

typedef union tag_comp_info {  /* Union to contain compression information */
    struct {            /* Struct to contain information about how to compress */
                        /* or decompress a JPEG encoded 24-bit image */
        intn quality;   /* Quality factor for JPEG compression, should be from */
                        /* 0 (terrible) to 100 (very good) */
        intn force_baseline;    /* If force_baseline is set to TRUE then */
                                /* quantization tables are limited to */
                                /* 0..255 for JPEG baseline compability */
                                /* This is only an issue for quality */
                                /* settings below 24 */
      } jpeg;
    struct {            /* struct to contain information about how to compress */
                        /* or decompress a N-bit encoded dataset */
        int32 nt;       /* number type of the data to encode */
        bool sign_ext;  /* whether to sign extend or not */
        bool fill_one;  /* whether to fill with 1's or 0's */
        intn start_bit; /* offset of the start bit in the data */
        intn bit_len;   /* number of bits to store */
      } nbit;
  } comp_info;

#endif /* __HCOMP_H */

