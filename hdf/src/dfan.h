/*
$Header$

$Log$
Revision 1.3  1993/01/19 05:54:07  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.2  1992/12/18  15:45:00  mfolk
 * Added defines for default label blocksize and default description
 * blocksize, for when annotations get converted to linked block
 * storage.
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*------------------------------------------------------------------------------
 * File:    dfan.h
 * Purpose: header file for the Annotations set
 * Invokes: df.h
 * Contents: 
 *  Structure definitions: DFANdirentry, DFANdirhead
 *  Constant definitions: DFAN_LABEL, DFAN_DESC
 * Remarks: none
 *----------------------------------------------------------------------------*/

#ifndef DFAN_H                      /* avoid re-inclusion */
#define DFAN_H

#include "hdf.h"

#define DFAN_LABEL  0
#define DFAN_DESC   1

#define DFAN_LAB_BLKSIZE   64   /* default blksize to use for labels */
#define DFAN_DESC_BLKSIZE 512   /* default blksize to use for descriptions */

#define DFAN_DEFENTRIES 16          /* no of dir entries to add at a time */

    /* This structure stores an entry in the label/desc directory */
    /* for a label/desc in the file, it gives the ref of the label/desc,
        and the tag/ref of the data item to which the label/desc relates */

typedef struct {
        uint16 annref;                  /* ref of annotation */
        uint16 datatag, dataref;        /* tag/ref of data */
    } DFANdirentry;

    /* This structure is a head node for the directory, which is organized as
        as a linked list of arrays.  DFANdirentry is the structure of an
        array element, while DFANdirhead is the list element */
typedef struct DFANdirhead {
        int32 nentries;
        struct DFANdirhead *next;
        DFANdirentry entries[1];        /* actually an arbitrary size array */
    } DFANdirhead;

/* testing...
#ifndef VMS
int32 DFANIgetannlen();
#else
int32 _DFANIgetannlen();
#endif
...tested */


#endif /* DFAN_H */
