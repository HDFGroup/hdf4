/*
$Header$

$Log$
Revision 1.1  1992/11/02 16:34:45  koziol
Initial revision

*/
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

#define COMP_NONE       0
#define COMP_RLE        1
#define COMP_IMCOMP     2
#define COMP_JPEG       3

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

