/*-----------------------------------------------------------------------------
 * File:    hcomp.h
 * Purpose: header file for compression information & structures
 * Invokes:
 * Contents: 
 * Structure definitions: comp_info
 * Constant definitions: lots...
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef _HCOMP_H
#define _HCOMP_H

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
  } comp_info;

#endif /* _HCOMP_H */

