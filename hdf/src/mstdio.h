/*-----------------------------------------------------------------------------
 * File:    mstdio.h
 * Purpose: Header file for stdio-like modeling information.
 * Dependencies: should be included after hdf.h
 * Invokes:
 * Contents: Structures & definitions for stdio modeling.  This header
 *              should only be included in hcomp.c and mstdio.c.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/
#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.3  1993/10/06 20:27:55  koziol
More compression fixed, and folded Doug's suggested change into VSappendable.

 * Revision 1.2  1993/10/01  20:01:23  koziol
 * Put "extern C" block around function prototypes for C++ compatibility.
 *
 * Revision 1.1  1993/09/30  19:05:23  koziol
 * Added basic compressing functionality for special tags.
 *
 *
 */

/* avoid re-inclusion */
#ifndef __MSTDIO_H
#define __MSTDIO_H

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

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

/* model information about stdio model */
typedef struct {
    uint32 pos;
 } comp_model_stdio_info_t;

#ifndef MSTDIO_MASTER
extern funclist_t mstdio_funcs;     /* functions to perform run-length encoding */
#else
funclist_t mstdio_funcs={    /* functions to perform run-length encoding */
    HCPmstdio_stread,
    HCPmstdio_stwrite,
    HCPmstdio_seek,
    HCPmstdio_inquire,
    HCPmstdio_read,
    HCPmstdio_write,
    HCPmstdio_endaccess
  };
#endif

#endif /* __MSTDIO_H */
