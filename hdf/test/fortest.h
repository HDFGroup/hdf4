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

#ifndef __FORTEST_H
#define __FORTEST_H
#include "hdf.h"

/* Verbosity Environment Variable */
#define FOR_VERB    "HDF_FOR_VERBOSITY"

#ifdef DF_CAPFNAMES
#  define ngetverb      FNAME(GETVERB)
#  define nhisystem       FNAME(HISYSTEM)
#else  /* !DF_CAPFNAMES */
#  define ngetverb      FNAME(getverb)
#  define nhisystem       FNAME(hisystem)
#endif /* DF_CAPFNAMES */

/* FORTRAN support C-stubs for FORTRAN interface tests */

extern FRETVAL(intf) ngetverb(void);
extern FRETVAL(intf) nhisystem(_fcd cmd, intf *cmdlen);

#endif /* __FORTEST_H */

