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

/*
FILE
    bitvect.c

PURPOSE
    Provide an API for dynamically-sized bit-vectors or "bit-sets"

REMARKS
    These function manipulate ordered sets of "bits".

DESIGN
    Each bit-vector is stored in memory as an array of unsigned 8-bit
    integers (uint8's in HDF types), which can grow as additional bits are
    flagged in the bit-vector.

    Each bit-vector is stored with the lowest bits in location 0 in the
    array of base type (uint8s currently) and the bits in "standard" C order
    (i.e. bit 0 is the lowest bit in the byte) in each byte.  This does make
    for a slightly strange "bit-swapped" storage, but is the most efficient.
 */

#define BV_MASTER

#include "bitvect.h" /* Multi-file raster information */

/*--------------------------------------------------------------------------
 NAME
    bv_new
 PURPOSE
    Create a new bit-vector.
 USAGE
    bv_ptr bv_new(num_bits, flags)
        int32 num_bits;             IN: The initial number of bits in the vector
        uint32 flags;               IN: Flags to determine special attributes
                                        of the newly created bit-vector
 RETURNS
    Returns either a valid bv_ptr on succeed or NULL on failure.
 DESCRIPTION
    Creates a new bit-vector with a certain initial # of bits.
 COMMENTS, BUGS, ASSUMPTIONS
    * If num_bits is set to -1, then the default number of bits is used.
    * num_bits must be positive or -1.
    * Flags are only checked for BV_ERROR, which is considered an error
--------------------------------------------------------------------------*/
bv_ptr
bv_new(int32 num_bits, uint32 flags)
{
    int32  base_elements;  /* # of base elements needed to store the # of bits requested */
    size_t array_mem_size; /* size of the array in memory */
    bv_ptr b = NULL;       /* ptr to the new bit-vector */

    /* Check for invalid numbers of bits or bad flags */
    if (num_bits < -1 || num_bits == 0)
        goto error;
    if (flags == BV_ERROR)
        goto error;

    /* Check for requesting the default # of bits */
    if (num_bits == -1)
        num_bits = BV_DEFAULT_BITS;

    base_elements = ((num_bits % BV_BASE_BITS) > 0) ? (num_bits / BV_BASE_BITS) + 1
                                                           : (num_bits / BV_BASE_BITS);

    if ((b = malloc(sizeof(bv_struct))) == NULL)
        goto error;

    b->bits_used  = num_bits;
    b->array_size = ((base_elements / BV_CHUNK_SIZE) + 1) * BV_CHUNK_SIZE;
    b->flags      = flags;

    array_mem_size = sizeof(bv_base) * (size_t)(b->array_size);
    if ((b->buffer = malloc(array_mem_size)) == NULL)
        goto error;

    /* Zero the intial bits */
    memset(b->buffer, 0, array_mem_size);
    b->last_zero = 0;

    return b;
error:
    if (b != NULL)
        free(b->buffer);
    free(b);
    return NULL;
} /* bv_new() */

/*--------------------------------------------------------------------------
 NAME
    bv_delete
 PURPOSE
    Dispose of a new bit-vector.
 USAGE
    intn bv_delete(b)
        bv_ptr b;                   IN: Bit-vector to dispose of
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Disposes of a bit-vector created by bv_new.  This routine is responsible
    for completely cleaning up the bit-vector and disposing of all dynamically
    allocated space.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
intn
bv_delete(bv_ptr b)
{
    if (b == NULL || b->buffer == NULL)
        return FAIL;

    free(b->buffer);
    free(b);

    return SUCCEED;
} /* bv_delete() */

/*--------------------------------------------------------------------------
 NAME
    bv_set
 PURPOSE
    Set a bit in a bit-vector
 USAGE
    intn bv_set(b,bit_num,value)
        bv_ptr b;                   IN: Bit-vector to use
        int32 bit_num;              IN: bit to set
        bv_bool value;              IN: bit value to set the bit to
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Sets a bit in a bit-vector to a bit value.  Also extends the bit-vector
    if the bit to set is beyond the end of the current bit-vector and the
    bit-vector is marked as extendable.
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
intn
bv_set(bv_ptr b, int32 bit_num, bv_bool value)
{
    int32 base_elem; /* the base array index of the bit */
    int32 bit_elem;  /* the bit index of the bit to set */

    if (b == NULL || bit_num < 0)
        return FAIL;

    base_elem = bit_num / BV_BASE_BITS;
    bit_elem  = bit_num % BV_BASE_BITS;

    /* Check if the bit is beyond the end of the current bit-vector */
    if (bit_num >= b->bits_used) {
        /* OK to extend? */
        if (b->flags & BV_EXTENDABLE) {
            if (base_elem < b->array_size) {
                /* just use more bits in the currently allocated block */
                b->bits_used = bit_num + 1;
            }
            else {
                /* allocate more space for bits */
                bv_base *old_buf = b->buffer; /* ptr to the old buffer */
                int32    num_chunks;          /* number of chunks to grab */
                size_t   new_size;
                size_t   extra_size;

                num_chunks =
                    ((((bit_num / BV_BASE_BITS) + 1) - b->array_size) / BV_CHUNK_SIZE) + 1;
                new_size = (size_t)(b->array_size + num_chunks * BV_CHUNK_SIZE);
                if ((b->buffer = realloc(b->buffer, new_size)) == NULL) {
                    b->buffer = old_buf;
                    /* Could not allocate a larger bit buffer */
                    return FAIL;
                }

                /* Zero the bits, for the new bits */
                extra_size = (size_t)(num_chunks * BV_CHUNK_SIZE);
                memset(&b->buffer[b->array_size], 0, extra_size);

                b->array_size += num_chunks * BV_CHUNK_SIZE;
                b->bits_used = bit_num + 1;
            }
        }
        else {
            /* Can't extend */
            return FAIL;
        }
    }

    if (value == BV_FALSE) {
        b->buffer[base_elem] &= ~bv_bit_value[bit_elem];
        if (base_elem < b->last_zero)
            b->last_zero = base_elem;
    }
    else
        b->buffer[base_elem] |= bv_bit_value[bit_elem];

    return SUCCEED;
} /* bv_set() */

/*--------------------------------------------------------------------------
 NAME
    bv_get
 PURPOSE
    Get a bit from a bit-vector
 USAGE
    intn bv_get(b,bit_num)
        bv_ptr b;                   IN: Bit-vector to use
        int32 bit_num;              IN: bit to set
 RETURNS
    Returns either BV_TRUE/BV_FALSE on success, or FAIL on error
 DESCRIPTION
    Gets a bit from a bit-vector.
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
intn
bv_get(bv_ptr b, int32 bit_num)
{
    int32 base_elem; /* the base array index of the bit */
    int32 bit_elem;  /* the bit index of the bit to set */
    intn  ret_value; /* the variable to store the return value */

    if (b == NULL || b->buffer == NULL || bit_num < 0)
        return (FAIL);

    /* Check for asking for a bit off of the end of the vector */
    if (bit_num >= b->bits_used)
        return BV_FALSE;

    base_elem = bit_num / BV_BASE_BITS;
    bit_elem  = bit_num % BV_BASE_BITS;

    ret_value = b->buffer[base_elem] & bv_bit_value[bit_elem];
    ret_value >>= bit_elem;

    return ret_value;
} /* bv_get() */

/*--------------------------------------------------------------------------
 NAME
    bv_clear
 PURPOSE
    Clears a bit-vector to a given boolean value
 USAGE
    intn bv_clear(b,value)
        bv_ptr b;                   IN: Bit-vector to use
        bv_bool value;              IN: bit value to set the bit to
 RETURNS
    Returns SUCCEED/FAIL
 DESCRIPTION
    Sets an entire bit vector to a given boolean value
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
intn
bv_clear(bv_ptr b, bv_bool value)
{
    size_t array_mem_size;

    if (b == NULL || b->buffer == NULL)
        return FAIL;

    array_mem_size = sizeof(bv_base) * (size_t)(b->array_size);

    if (value == BV_TRUE) {
        memset(b->buffer, 255, array_mem_size);
        b->last_zero = -1;
    }
    else {
        memset(b->buffer, 0, array_mem_size);
        b->last_zero = 0;
    }

    return SUCCEED;
} /* bv_clear() */

/*--------------------------------------------------------------------------
 NAME
    bv_size
 PURPOSE
    Report the number of bits in the bit-vector
 USAGE
    int32 bv_size(b)
        bv_ptr b;                   IN: Bit-vector to use
 RETURNS
    Returns number of bits in use on success, FAIL on error
 DESCRIPTION
    Report the number of bits currently in-use for a bit-vector.
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
int32
bv_size(bv_ptr b)
{
    if (b == NULL)
        return FAIL;

    return b->bits_used;
} /* bv_size() */

/*--------------------------------------------------------------------------
 NAME
    bv_flags
 PURPOSE
    Returns the flags for a bit-vector
 USAGE
    uint32 bv_size(b)
        bv_ptr b;                   IN: Bit-vector to use
 RETURNS
    Returns bit-vector flags.
    Returns BV_ERROR on errors.
 DESCRIPTION
    Returns the current flags for the bit-vector.
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
uint32
bv_flags(bv_ptr b)
{
    if (b == NULL)
        return BV_ERROR;

    return b->flags;
} /* bv_flags() */

/*--------------------------------------------------------------------------
 NAME
    bv_find
 PURPOSE
    Find the next bit of a given value
 USAGE
    int32 bv_find(b,last_find,value)
        bv_ptr b;                   IN: Bit-vector to use
        int32 last_find;            IN: bit offset of last bit found
        bv_bool value;              IN: boolean value to look for
 RETURNS
    Returns offset of next bit on success, FAIL on error
 DESCRIPTION
    Find the next highest bit of a given bit value.
 GLOBAL VARIABLES
 COMMENTS, BUGS, ASSUMPTIONS
    "last_find" capability not currently implemented for '0' searches - QAK
 EXAMPLES
 REVISION LOG
--------------------------------------------------------------------------*/
int32
bv_find(bv_ptr b, int32 last_find, bv_bool value)
{
    int32   old_bits_used;  /* the last number of bits used */
    int32   bytes_used;     /* number of full bytes used */
    int32   first_byte = 0; /* The first byte to begin searching at */
    bv_base slush_bits;     /* extra bits which don't fit into a byte */
    int32   i;              /* local counting variable */

    if (b == NULL || b->buffer == NULL)
        return FAIL;

    bytes_used = b->bits_used / BV_BASE_BITS;
    if (value == BV_TRUE) {   /* looking for first '1' in the bit-vector */
        if (last_find >= 0) { /* if the last bit found option is used, look for more bits in that same byte */
            intn bit_off;

            first_byte = last_find / BV_BASE_BITS;
            bit_off    = (intn)((last_find - (first_byte * BV_BASE_BITS)) + 1);
            slush_bits = b->buffer[first_byte] & (~bv_bit_mask[bit_off]);
            if (slush_bits != 0)
                return (first_byte * BV_BASE_BITS) + bv_first_zero[(~slush_bits)];
            first_byte++;
        }

        for (i = first_byte; i < bytes_used; i++) {
            if (b->buffer[i] != 0)
                return (i * BV_BASE_BITS) + bv_first_zero[~b->buffer[i]];
        }

        /* Any extra bits left over? */
        if ((bytes_used * BV_BASE_BITS) < b->bits_used) {
            slush_bits = (bv_base)(b->buffer[i] & bv_bit_mask[b->bits_used - (bytes_used * BV_BASE_BITS)]);
            if (slush_bits != 0)
                return (i * BV_BASE_BITS) + bv_first_zero[~slush_bits];
        }
    }
    else {
        /* looking for first '0' in the bit-vector */
        bv_base *tmp_buf;

        if (b->last_zero >= 0)
            i = b->last_zero;
        else
            i = 0;
        tmp_buf = &b->buffer[i];
        while (i < bytes_used && *tmp_buf == 255) {
            i++;
            tmp_buf++;
        }
        if (i < bytes_used) {
            b->last_zero = i;
            return (i * BV_BASE_BITS) + bv_first_zero[*tmp_buf];
        }

        /* Any extra bits left over? */
        if ((bytes_used * BV_BASE_BITS) < b->bits_used) {
            slush_bits = (bv_base)(b->buffer[i] & bv_bit_mask[b->bits_used - (bytes_used * BV_BASE_BITS)]);
            if (slush_bits != 255) {
                b->last_zero = i;
                return (i * BV_BASE_BITS) + bv_first_zero[slush_bits];
            }
        }
    }

    /* Beyond the current end of the bit-vector, extend the bit-vector */
    old_bits_used = b->bits_used;
    if (bv_set(b, b->bits_used, BV_FALSE) == FAIL)
        return FAIL;

    return old_bits_used;
} /* bv_find() */
