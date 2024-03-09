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

#include "hdf_priv.h"

/*
LOCAL ROUTINES
  None
EXPORTED ROUTINES
  HDmemfill    -- copy a chunk of memory repetitively into another chunk
  HIstrncpy    -- string copy with termination
  strdup     -- in-library replacement for non-ANSI strdup()
*/

/*--------------------------------------------------------------------------
 NAME
    HDmemfill -- copy a chunk of memory repetitively into another chunk

 USAGE
    void * HDmemfill(dest,src,item_size,num_items)
        void * dest;         OUT: pointer to the chunk of memory to be filled
                            with a pattern
        void * src;          IN: pointer to the pattern to copy
        uint32 item_size;   IN: size of the pattern to copy
        uint32 num_items;   IN: number of times to copy the pattern into the dest
                            buffer

 RETURNS
    Returns a pointer to the dest parameter

 DESCRIPTION
    Common code used to fill a chunk of memory with a pattern.  This
    routine can be used to copy a given "fill" value into an array
    of any number type.

 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    The src and dest pointers are assumed to point to valid portions of
    memory.
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
void *
HDmemfill(void *dest, const void *src, uint32 item_size, uint32 num_items)
{
    uint32 copy_size;  /* size of the buffer to copy */
    uint32 copy_items; /* number of items currently copying */
    uint32 items_left; /* number of items left to copy */
    uint8 *curr_dest;  /* ptr into the 'dest' memory where we are currently */

    /* minimal error check for 0 sized array or item size */
    if (num_items > 0 && item_size > 0) {
        memcpy(dest, src, item_size); /* copy first item */

        copy_size  = item_size;
        copy_items = 1;
        items_left = num_items - 1;
        curr_dest  = ((uint8 *)dest) + item_size;

        /* copy until we've copied at least half of the items */
        while (items_left >= copy_items) {

            memcpy(curr_dest, dest, copy_size); /* copy the current chunk */
            curr_dest += copy_size;             /* move the offset for the next chunk */
            items_left -= copy_items;           /* decrement the number of items left */

            copy_size *= 2;  /* increase the size of the chunk to copy */
            copy_items *= 2; /* increase the count of items we are copying */
        }                    /* end while */
        if (items_left > 0)  /* if there are any items left to copy */
            memcpy(curr_dest, dest, items_left * item_size);
    } /* end if */
    return dest;
} /* end HDmemfill() */

/*--------------------------------------------------------------------------
 NAME
    HIstrncpy -- string copy with termination
 USAGE
    char *HIstrncpy(char *dest,char *source,int32 len)
        char *dest;             OUT: location to place string
        char *source;           IN: location of string to copy
        int32 len;              IN: mas. length of dest. string
 RETURNS
    dest on success, NULL on failure.
 DESCRIPTION
    This function creates a string in dest that is at most 'len' characters
    long.  The 'len' characters *include* the NULL terminator which must be
    added.  So if you have the string "Foo\0" you must call this copy function
    with len == 4.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
    Sep 19, 11 - Changed last argument's type from int32 to int.  It didn't
    make sense to have to cast in most places. -BMR
--------------------------------------------------------------------------*/
char *
HIstrncpy(char *dest, const char *source, int len)
{
    char *destp;

    destp = dest;
    if (len == 0)
        return destp;
    for (; (len > 1) && (*source != '\0'); len--)
        *dest++ = *source++;
    *dest = '\0'; /* Force the last byte be '\0'   */
    return destp;
} /* end HIstrncpy() */
