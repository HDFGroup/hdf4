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
Revision 1.1  1993/09/30 19:04:43  koziol
Added basic compressing functionality for special tags.

 *
 */

/* avoid re-inclusion */
#ifndef __CRLE_H
#define __CRLE_H

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
