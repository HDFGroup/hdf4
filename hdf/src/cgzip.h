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
 * File:    cgzip.h
 * Purpose: Header file for gzip encoding information.
 * Dependencies: should only be included from hcompi.h
 * Invokes: none
 * Contents: Structures & definitions for gzip encoding.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/

/* avoid re-inclusion */
#ifndef __CGZIP_H
#define __CGZIP_H

#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
   ** from cgzip.c
 */

    extern int32 HCPcgzip_stread
                (accrec_t * rec);

    extern int32 HCPcgzip_stwrite
                (accrec_t * rec);

    extern int32 HCPcgzip_seek
                (accrec_t * access_rec, int32 offset, int origin);

    extern int32 HCPcgzip_inquire
                (accrec_t * access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
               int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                 int16 *pspecial);

    extern int32 HCPcgzip_read
                (accrec_t * access_rec, int32 length, VOIDP data);

    extern int32 HCPcgzip_write
                (accrec_t * access_rec, int32 length, const VOIDP data);

    extern intn HCPcgzip_endaccess
                (accrec_t * access_rec);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

/* gzip [en|de]coding information */
typedef struct
{
    intn        skip_size;      /* number of bytes in each element */
    intn        skip_pos;       /* current byte to read or write */
    int32       offset;         /* offset in the de-compressed array */
}
comp_coder_gzip_info_t;

#ifndef CGZIP_MASTER
extern funclist_t cgzip_funcs;   /* functions to perform gzip encoding */
#else
funclist_t  cgzip_funcs =
{                               /* functions to perform gzip encoding */
    HCPcgzip_stread,
    HCPcgzip_stwrite,
    HCPcgzip_seek,
    HCPcgzip_inquire,
    HCPcgzip_read,
    HCPcgzip_write,
    HCPcgzip_endaccess
};
#endif

#endif /* __CGZIP_H */

