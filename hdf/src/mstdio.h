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
Revision 1.1  1993/09/30 19:05:23  koziol
Added basic compressing functionality for special tags.

 *
 */

/* avoid re-inclusion */
#ifndef __MSTDIO_H
#define __MSTDIO_H

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
