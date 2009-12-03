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

#include "hdf.h"
#include "tutils.h"

intn fuzzy_memcmp(const void *s1, const void *s2, int32 len, intn fuzz_factor)
{
    const uint8 *t1 = (const uint8 *) s1;
    const uint8 *t2 = (const uint8 *) s2;

    while (len > 0 && (int) ABS(*t2 - *t1) <= fuzz_factor)
      {
          t1++;
          t2++;
          len--;
      }     /* end while */
    if (len == 0)
        return (0);
    else
      {
          return ((intn) (*t1 - *t2));
      }
}   /* end fuzzy_memcmp() */

