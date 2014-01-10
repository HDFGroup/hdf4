/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */
#include <stdio.h>
#include "hdf.h"

int
main(int argc, char *argv[])
{
    if (argc != 2)
      {
          fprintf(stderr, "%s: converts HDF vset v1.0 files to v2.0\n", argv[0]);
          fprintf(stderr, "Usage: %s hdf-file\n", argv[0]);
          exit(0);
      }

    if (0 == vcheckcompat(argv[1]))
      {
          vmakecompat(argv[1]);
          fprintf(stderr, "file [%s] is now compatible\n", argv[1]);
      }
    else
        fprintf(stderr, "file [%s] already compatible with r2.0\n", argv[1]);

    return (0);
}   /* main */

/* ------------------------------------------------------------------ */
