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

/*-----------------------------------------------------------------------------
 * File:    dfi.h
 * Purpose: HDF internal header file
 * Invokes: stdio.h, sys/file.h
 * Contents:
 *  Compilation parameters
 *  Machine-dependent definitions
 *  Flexibility definitions: i/o buffering, dynamic memory, structure i/o
 *  Size parameters
 * Remarks: To port to a new system, only dfi.h and Makefile need be modified.
 *          This file is included with user programs, but users do not see it.
 *---------------------------------------------------------------------------*/

#ifndef H4_DFI_H
#define H4_DFI_H

/*--------------------------------------------------------------------------*/
/*          Compilation Parameters for Flexibility and Portability          */

/* modify this line for buffered/unbuffered i/o */
#define DF_BUFFIO

/* modify this line for dynamic/static memory allocation */
#define DF_DYNAMIC

/* modify this line if structures cannot be read/written as is */
#undef DF_STRUCTOK /* leave it this way - hdfsh expects it */

/*--------------------------------------------------------------------------*/
/*                      Flexibility parameters                              */
#ifdef DF_BUFFIO /* set all calls to do buffered I/O */
#define DF_OPEN(x, y)        fopen(x, y)
#define DF_CLOSE(x)          fclose(x)
#define DF_SEEK(x, y, z)     fseek(x, y, z)
#define DF_SKEND(x, y, z)    fseek(x, y, z)
#define DF_TELL(x)           ftell(x)
#define DF_READ(a, b, c, d)  fread(a, b, c, d)
#define DF_WRITE(a, b, c, d) fwrite(a, b, c, d)
#define DF_FLUSH(a)          fflush(a)
#define DF_OPENERR(f)        (!(f))
#define DF_RDACCESS          "rb"
#define DF_WRACCESS          "rb+"

#else /*DF_BUFFIO         unbuffered i/o */
#define DF_OPEN(x, y)        open(x, y)
#define DF_CLOSE(x)          close(x)
#define DF_SEEK(x, y, z)     lseek(x, y, z)
#define DF_SKEND(x, y, z)    lseek(x, -1 * y, z)
#define DF_TELL(x)           lseek(x, 0L, 1)
#define DF_READ(a, b, c, d)  read(d, a, b *c)
#define DF_WRITE(a, b, c, d) write(d, a, b *c)
#define DF_OPENERR(f)        ((f) == -1)
#define DF_FLUSH(a)          /* no need to flush */
#define DF_RDACCESS          O_RDONLY
#define DF_WRACCESS          O_RDWR
#endif /* DF_BUFFIO */

#ifndef FILE
#include <stdio.h>
#endif /*FILE */

#endif /* H4_DFI_H */
