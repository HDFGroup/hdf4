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
Revision 1.3  1993/10/04 20:02:36  koziol
Updated error reporting in H-Layer routines, and added more error codes and
compression stuff.

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

/* RLE coding information */
typedef struct {
    uint8 buffer[128];      /* buffer for storing RLE bytes as they are sent */
    intn  buf_pos;          /* offset into the buffer */
    uint8 last_byte,        /* the last byte stored in the buffer */
        second_byte;        /* the second to last byte stored in the buffer */
    enum {RUN=0,            /* buffer up to the current position is a run */
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
