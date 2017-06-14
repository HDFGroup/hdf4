/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* $Id$ */

#ifndef __FORTEST_H
#define __FORTEST_H

#include "H4api_adpt.h"
#include "hdf.h"

/* Verbosity Environment Variable */
#define FOR_VERB    "HDF_FOR_VERBOSITY"

#  define ngetverb      H4_F77_FUNC(getverb, GETVERB)
#  define nhisystem     H4_F77_FUNC(hisystem, HISYSTEM)
#  define nfixnamec     H4_F77_FUNC(fixnamec, FIXNAMEC) 

/* FORTRAN support C-stubs for FORTRAN interface tests */

HDFFCLIBAPI FRETVAL(intf) ngetverb(void);
HDFFCLIBAPI FRETVAL(intf) nhisystem(_fcd cmd, intf *cmdlen);
HDFFCLIBAPI FRETVAL(intf) nfixnamec(_fcd name, intf *name_len, _fcd name_out, intf *name_len_out);

#endif /* __FORTEST_H */

