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
Revision 1.3.2.1  1993/10/10 22:10:15  koziol
Moved Tag descriptions into a header file.  Updated compression routines.

 * Revision 1.3  1993/10/06  20:27:32  koziol
 * More compression fixed, and folded Doug's suggested change into VSappendable.
 *
 * Revision 1.2  1993/10/01  20:00:57  koziol
 * Put "extern C" block around function prototypes for C++ compatibility.
 *
 * Revision 1.1  1993/09/30  19:16:25  koziol
 * Initial revision of the internal compression header
 *
 *
 */

/* avoid re-inclusion */
#ifndef __HCOMPI_H
#define __HCOMPI_H

/* Modeling information */

/* structure for storing modeling information */
/* only allow modeling and master compression routines access */

#include "mstdio.h"         /* stdio modeling header */

typedef struct comp_model_info_tag {
    comp_model_t model_type;  /* model this stream is using */
    union {     /* union of all the different types of model information */
        comp_model_stdio_info_t stdio_info;     /* stdio model info */
    } model_info;
    funclist_t model_funcs;     /* functions to perform modeling */
 } comp_model_info_t;


/* Coding information */

/* structure for storing modeling information */
/* only allow encoding and master compression routines access */

#include "crle.h"           /* run-length encoding header */

typedef struct comp_coder_info_tag {
    comp_coder_t coder_type;    /* coding scheme this stream is using */
    union {     /* union of all the different types of coding information */
        comp_coder_rle_info_t rle_info;      /* RLE coding info */
    } coder_info;
    funclist_t coder_funcs;     /* functions to perform encoding */
 } comp_coder_info_t;

/* structure for storing a state */
typedef struct comp_stateinfo_tag {
    uint32 d_offset;            /* the offset of the state in the dataset */
    uint32 c_offset;            /* offset of the state in the compressed data */
    comp_model_info_t minfo;    /* modeling information */
    comp_coder_info_t cinfo;    /* coding information */
 } comp_stateinfo_t;

/* structure for storing state caching information */
typedef struct comp_state_cache_tag {
    intn num_states;            /* the number of states cached */
    comp_stateinfo_t **comp_state;   /* pointer to an array of pointers to
                                    compression states */
 } comp_state_cache_t;

/* compinfo_t -- compressed element information structure */
typedef struct compinfo_tag {
    intn attached;              /* number of access records attached
                                  to this information structure */
    int32 length;               /* the actual length of the data elt */
    uint16 comp_ref;            /* compressed info ref. number */
    int32 aid;                  /* AID of the compressed info */
    comp_model_info_t minfo;    /* modeling information */
    comp_coder_info_t cinfo;    /* coding information */
    bool caching;               /* whether caching is turned on */
    comp_state_cache_t sinfo;   /* state information for caching */
} compinfo_t;

#endif /* __HCOMPI_H */
