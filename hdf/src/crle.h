/*-----------------------------------------------------------------------------
 * File:    crle.h
 * Purpose: Header file for run-length encoding information.
 * Dependencies: should be included after hdf.h
 * Invokes:
 * Contents: Structures & definitions for run-length encoding.  This header
 *              should only be included in hcomp.c and crle.c.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/
#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.4.2.1  1993/10/10 22:10:03  koziol
Moved Tag descriptions into a header file.  Updated compression routines.

 * Revision 1.4  1993/10/06  20:27:22  koziol
 * More compression fixed, and folded Doug's suggested change into VSappendable.
 *
 * Revision 1.3  1993/10/04  20:02:36  koziol
 * Updated error reporting in H-Layer routines, and added more error codes and
 * compression stuff.
 *
 * Revision 1.2  1993/10/01  20:00:28  koziol
 * Put "extern C" block around function prototypes for C++ compatibility.
 *
 * Revision 1.1  1993/09/30  19:04:43  koziol
 * Added basic compressing functionality for special tags.
 *
 *
 */

/* avoid re-inclusion */
#ifndef __CRLE_H
#define __CRLE_H

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

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

/* size of the RLE buffer */
#define RLE_BUF_SIZE    128
/* NIL code for run bytes */
#define RLE_NIL         (-1)
/* minimum length of run */
#define RLE_MIN_RUN     3
/* maximum length of run */
#define RLE_MAX_RUN     (RLE_BUF_SIZE+RLE_MIN_RUN-1)
/* minimum length of mix */
#define RLE_MIN_MIX     1

/* RLE [en|de]coding information */
typedef struct {
    uint32 offset;          /* offset in the file */
    uint8 buffer[RLE_BUF_SIZE];     /* buffer for storing RLE bytes */
    intn buf_length;        /* number of bytes in buffer */
    intn buf_pos;           /* offset into the buffer */
    intn last_byte,         /* the last byte stored in the buffer */
        second_byte;        /* the second to last byte stored in the buffer */
    enum {INIT,             /* initial state, need to read a byte to determine
                                next state */
        RUN,                /* buffer up to the current position is a run */
        MIX}                /* buffer up to the current position is a mix */
	    rle_state;          /* state of the buffer storage */
 } comp_coder_rle_info_t;

#ifndef CRLE_MASTER
extern funclist_t crle_funcs;     /* functions to perform run-length encoding */
#else
funclist_t crle_funcs={    /* functions to perform run-length encoding */
    HCPcrle_stread,
    HCPcrle_stwrite,
    HCPcrle_seek,
    HCPcrle_inquire,
    HCPcrle_read,
    HCPcrle_write,
    HCPcrle_endaccess
  };
#endif

#endif /* __CRLE_H */

