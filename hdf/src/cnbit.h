/*-----------------------------------------------------------------------------
 * File:    cnbit.h
 * Purpose: Header file for N-bit encoding information.
 * Dependencies: should only be included from hcompi.h
 * Invokes: none
 * Contents: Structures & definitions for N-bit encoding.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/
#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1993/10/19 17:36:32  koziol
Tons of changes and updates.  Fixed the 0 length problem in Hstartwrite(),
made all the 'void' returning routines in the Vsets return a succeed or
fail.  Added N-Bit files (which aren't working yet...)

 *
 */

/* avoid re-inclusion */
#ifndef __CNBIT_H
#define __CNBIT_H

#if defined c_plusplus || defined __cplusplus
extern "C" {
#endif /* c_plusplus || __cplusplus */

/*
** from cnbit.c
*/

extern int32 HCPcnbit_stread
    PROTO((accrec_t *rec));

extern int32 HCPcnbit_stwrite
    PROTO((accrec_t *rec));

extern int32 HCPcnbit_seek
    PROTO((accrec_t *access_rec, int32 offset, int origin));

extern int32 HCPcnbit_inquire
    PROTO((accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
            int32 *plength, int32 *poffset,int32 *pposn, int16 *paccess,
            int16 *pspecial));

extern int32 HCPcnbit_read
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPcnbit_write
    PROTO((accrec_t *access_rec, int32 length, VOIDP data));

extern int32 HCPcnbit_endaccess
    PROTO((accrec_t *access_rec));

#if defined c_plusplus || defined __cplusplus
}
#endif /* c_plusplus || __cplusplus */

/* size of the N-bit buffer */
#define NBIT_BUF_SIZE   (MAX_NT_SIZE)
/* size of the N-bit mask buffer (same as buffer size for now) */
#define NBIT_MASK_SIZE  NBIT_BUF_SIZE

typedef struct {        /* structure to hold bit vector info */
    intn offset,        /* offset of the bit information */
        length;         /* number of bits in the information */
  } nbit_mask_info_t;

/* N-bit [en|de]coding information */
typedef struct {
    int32 nt;               /* number type of data we are encoding */
    intn nt_size;           /* size of the number-type in the file */
    bool fill_one;          /* whether to fill with 1's or not (0's) */
    bool sign_ext;          /* whether to sign extend or not */
    intn mask_off,          /* offset of the bit to start masking with */
        mask_len;           /* number of bits to mask */
    uint32 offset;          /* offset in the file */
    uint8 buffer[NBIT_BUF_SIZE];     /* buffer for storing N-bit data */
    intn buf_length;        /* number of bytes in buffer */
    intn buf_pos;           /* offset into the buffer */
    uint8 mask_buf[NBIT_MASK_SIZE]; /* buffer to hold the bitmask */
    nbit_mask_info_t mask_info[NBIT_MASK_SIZE]; /* information about the mask */
    enum {NBIT_INIT,        /* initial state, need to read a byte to determine
                                next state */
        NBIT_COMPOSE}       /* composing a data chunk */
	    nbit_state;         /* state of the buffer storage */
 } comp_coder_nbit_info_t;

#ifndef CNBIT_MASTER
extern funclist_t cnbit_funcs;     /* functions to perform N-bit encoding */
#else
funclist_t cnbit_funcs={    /* functions to perform N-bit encoding */
    HCPcnbit_stread,
    HCPcnbit_stwrite,
    HCPcnbit_seek,
    HCPcnbit_inquire,
    HCPcnbit_read,
    HCPcnbit_write,
    HCPcnbit_endaccess
  };
#endif

#endif /* __CNBIT_H */

