/*-----------------------------------------------------------------------------
 * File:    hcompi.h
 * Purpose: Internal library header file for compression information
 * Dependencies: should be included after hdf.h
 * Invokes:
 * Contents:
 * Structure definitions: 
 * Constant definitions: 
 *---------------------------------------------------------------------------*/
#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1993/10/01 20:00:57  koziol
Put "extern C" block around function prototypes for C++ compatibility.

 * Revision 1.1  1993/09/30  19:16:25  koziol
 * Initial revision of the internal compression header
 *
 *
 */

/* avoid re-inclusion */
#ifndef __HCOMPI_H
#define __HCOMPI_H

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

/*
** from crle.c
*/

extern int32 HCPcrle_stread
    PROTO((accrec_t *rec));

extern int32 HCPcrle_stwrite
    PROTO((accrec_t *rec));

extern int32 HCPcrle_seek
    PROTO((accrec_t *access_rec, int32 offset, int origin));

extern int32 HCPcrle_inquire
    PROTO((accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
            int32 *plength, int32 *poffset,int32 *pposn, int16 *paccess,
            int16 *pspecial));

extern int32 HCPcrle_read
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPcrle_write
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPcrle_endaccess
    PROTO((accrec_t *access_rec));

/*
** from mstdio.c
*/

extern int32 HCPmstdio_stread
    PROTO((accrec_t *rec));

extern int32 HCPmstdio_stwrite
    PROTO((accrec_t *rec));

extern int32 HCPmstdio_seek
    PROTO((accrec_t *access_rec, int32 offset, int origin));

extern int32 HCPmstdio_inquire
    PROTO((accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
            int32 *plength, int32 *poffset,int32 *pposn, int16 *paccess,
            int16 *pspecial));

extern int32 HCPmstdio_read
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPmstdio_write
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPmstdio_endaccess
    PROTO((accrec_t *access_rec));

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

#endif /* __HCOMPI_H */

