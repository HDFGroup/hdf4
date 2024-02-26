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

/*------------------------------------------------------------------
 File:  dfkswap.c

 Purpose:
    Routines to support little-endian conversion to and from HDF format

 PRIVATE conversion functions:
    DFKsb2b -  Byte swapping for 16 bit integers
    DFKsb4b -  Byte swapping for 32 bit integers
    DFKsb8b -  Byte swapping for 64 bit floats

 Remarks:
    These files used to be in dfconv.c, but it got a little too huge,
    so I broke them out into separate files. - Q

 *------------------------------------------------------------------*/

/*****************************************************************************/
/*                                                                           */
/*    All the routines in this file marked as PRIVATE have been marked so    */
/*  for a reason.  *ANY* of these routines may or may nor be supported in    */
/*  the next version of HDF (4.00).  Furthurmore, the names, parameters, or   */
/*  functionality is *NOT* guaranteed to remain the same.                    */
/*    The *ONLY* guarantee possible is that DFKnumin(), and DFKnumout()      */
/*  will not change.  They are *NOT* guaranteed to be implemented in the     */
/*  next version of HDF as function pointers.  They are guaranteed to take   */
/*  the same arguments and produce the same results.                         */
/*    If your programs call any routines in this file except for             */
/*  DFKnumin(), DFKnumout, and/or DFKsetntype(), your code may not work      */
/*  with future versions of HDF and your code will *NOT* be portable.        */
/*                                                                           */
/*****************************************************************************/

#include "hdf_priv.h"
#include "hconv_priv.h"

/*****************************************************************************/
/* NUMBER CONVERSION ROUTINES FOR BYTE SWAPPING                              */
/*****************************************************************************/

/************************************************************/
/* DFKsb2b()                                                */
/* -->Byte swapping for 2 byte data items                   */
/************************************************************/
int
DFKsb2b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride)
{
    int    fast_processing = 0; /* Default is not fast processing */
    int    in_place        = 0; /* Inplace must be detected */
    uint32 i;
    uint8  buf[2]; /* Inplace processing buffer */
    uint8 *source = (uint8 *)s;
    uint8 *dest   = (uint8 *)d;

    HEclear();

    if (num_elm == 0) { /* No elements is an error. */
        HERROR(DFE_BADCONV);
        return FAIL;
    }

    /* Determine if faster array processing is appropriate */
    if (source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    /* Determine if the conversion should be inplace */
    if (source == dest)
        in_place = 1;

    if (fast_processing) {
        if (!in_place) {
            for (i = 0; i < num_elm; i++) {
                dest[0] = source[1];
                dest[1] = source[0];
                dest += 2;
                source += 2;
            }
            return 0;
        }
        else {
            for (i = 0; i < num_elm; i++) {
                buf[0]  = source[1];
                buf[1]  = source[0];
                dest[0] = buf[0];
                dest[1] = buf[1];
                dest += 2;
                source += 2;
            }
            return 0;
        }
    }

    /* Generic stride processing */
    if (!in_place)
        for (i = 0; i < num_elm; i++) {
            dest[0] = source[1];
            dest[1] = source[0];
            dest += dest_stride;
            source += source_stride;
        }
    else
        for (i = 0; i < num_elm; i++) {
            buf[0]  = source[1];
            buf[1]  = source[0];
            dest[0] = buf[0];
            dest[1] = buf[1];
            dest += dest_stride;
            source += source_stride;
        }
    return 0;
}

/************************************************************/
/* DFKsb4b()                                                */
/* -->Byte swapping for 4 byte data items                   */
/************************************************************/
int
DFKsb4b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride)
{
    int    fast_processing = 0; /* Default is not fast processing */
    int    in_place        = 0; /* Inplace must be detected */
    uint32 i;
    uint8  buf[4]; /* Inplace processing buffer */
    uint8 *source = (uint8 *)s;
    uint8 *dest   = (uint8 *)d;

    HEclear();

    if (num_elm == 0) { /* No elements is an error. */
        HERROR(DFE_BADCONV);
        return FAIL;
    }

    /* Determine if faster array processing is appropriate */
    if (source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    /* Determine if the conversion should be inplace */
    if (source == dest)
        in_place = 1;

    if (fast_processing) {
        if (!in_place) {
            for (i = 0; i < num_elm; i++) {
                dest[0] = source[3];
                dest[1] = source[2];
                dest[2] = source[1];
                dest[3] = source[0];
                dest += 4;
                source += 4;
            }
            return 0;
        }
        else {
            for (i = 0; i < num_elm; i++) {
                buf[0]  = source[3];
                buf[1]  = source[2];
                buf[2]  = source[1];
                buf[3]  = source[0];
                dest[0] = buf[0];
                dest[1] = buf[1];
                dest[2] = buf[2];
                dest[3] = buf[3];
                dest += 4;
                source += 4;
            }
            return 0;
        }
    }

    /* Generic stride processing */
    if (!in_place)
        for (i = 0; i < num_elm; i++) {
            dest[0] = source[3];
            dest[1] = source[2];
            dest[2] = source[1];
            dest[3] = source[0];
            dest += dest_stride;
            source += source_stride;
        }
    else
        for (i = 0; i < num_elm; i++) {
            buf[0]  = source[3];
            buf[1]  = source[2];
            buf[2]  = source[1];
            buf[3]  = source[0];
            dest[0] = buf[0];
            dest[1] = buf[1];
            dest[2] = buf[2];
            dest[3] = buf[3];
            dest += dest_stride;
            source += source_stride;
        }
    return 0;
}

/************************************************************/
/* DFKsb8b()                                                */
/* -->Byte swapping for 8 byte data items                   */
/************************************************************/
int
DFKsb8b(void *s, void *d, uint32 num_elm, uint32 source_stride, uint32 dest_stride)
{
    int    fast_processing = 0; /* Default is not fast processing */
    int    in_place        = 0; /* Inplace must be detected */
    uint32 i;
    uint8  buf[8]; /* Inplace processing buffer */
    uint8 *source = (uint8 *)s;
    uint8 *dest   = (uint8 *)d;

    HEclear();

    if (num_elm == 0) { /* No elements is an error. */
        HERROR(DFE_BADCONV);
        return FAIL;
    }

    /* Determine if faster array processing is appropriate */
    if (source_stride == 0 && dest_stride == 0)
        fast_processing = 1;

    /* Determine if the conversion should be inplace */
    if (source == dest)
        in_place = 1;

    if (fast_processing) {
        if (!in_place) {
            for (i = 0; i < num_elm; i++) {
                dest[0] = source[7];
                dest[1] = source[6];
                dest[2] = source[5];
                dest[3] = source[4];
                dest[4] = source[3];
                dest[5] = source[2];
                dest[6] = source[1];
                dest[7] = source[0];
                dest += 8;
                source += 8;
            }
            return 0;
        }
        else {
            for (i = 0; i < num_elm; i++) {
                buf[0]  = source[7];
                buf[1]  = source[6];
                buf[2]  = source[5];
                buf[3]  = source[4];
                buf[4]  = source[3];
                buf[5]  = source[2];
                buf[6]  = source[1];
                buf[7]  = source[0];
                dest[0] = buf[0];
                dest[1] = buf[1];
                dest[2] = buf[2];
                dest[3] = buf[3];
                dest[4] = buf[4];
                dest[5] = buf[5];
                dest[6] = buf[6];
                dest[7] = buf[7];
                dest += 8;
                source += 8;
            }
            return 0;
        }
    }

    /* Generic stride processing */
    if (!in_place)
        for (i = 0; i < num_elm; i++) {
            dest[0] = source[7];
            dest[1] = source[6];
            dest[2] = source[5];
            dest[3] = source[4];
            dest[4] = source[3];
            dest[5] = source[2];
            dest[6] = source[1];
            dest[7] = source[0];
            dest += dest_stride;
            source += source_stride;
        }
    else
        for (i = 0; i < num_elm; i++) {
            buf[0]  = source[7];
            buf[1]  = source[6];
            buf[2]  = source[5];
            buf[3]  = source[4];
            buf[4]  = source[3];
            buf[5]  = source[2];
            buf[6]  = source[1];
            buf[7]  = source[0];
            dest[0] = buf[0];
            dest[1] = buf[1];
            dest[2] = buf[2];
            dest[3] = buf[3];
            dest[4] = buf[4];
            dest[5] = buf[5];
            dest[6] = buf[6];
            dest[7] = buf[7];
            dest += dest_stride;
            source += source_stride;
        }

    return 0;
}
