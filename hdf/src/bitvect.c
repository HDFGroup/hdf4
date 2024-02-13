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

    The data structure is optimized for finding the next zero bit an array
    and caches this location when appropriate. It is NOT similarly optimized
    for finding 1 bits as that is not a use case in the HDF4 library.
 */

#include "hdfi.h"
#include "bitvect.h"

/* Base type of the array used to store the bits */
typedef uint8 bv_base;

/* # of bits in the base type of the array used to store the bits */
#define BV_BASE_BITS 8

/* bit-vector structure used
 *
 * All values are set to be 32-bit signed integers since that's what the
 * API accepts (negative values are reserved for errors). Bit vectors
 * larger than 2Gbits would require an internal API change.
 */
typedef struct bv_struct_tag {
    int32    bits_used;  /* The actual number of bits current in use */
    int32    array_size; /* The number of bv_base elements in the bit-vector */
    int32    last_zero;  /* The last location we know had a zero bit */
    bv_base *buffer;     /* Pointer to the buffer used to store the bits */
} bv_struct;

/* Table of bits for each bit position */
/*  (This will need to be changed/expanded if another base type is used) */
static const uint8 bv_bit_value[8] = {
    1,  /* bit 0's value is 1 */
    2,  /* bit 1's value is 2 */
    4,  /* bit 2's value is 4 */
    8,  /* bit 3's value is 8 */
    16, /* bit 4's value is 16 */
    32, /* bit 5's value is 32 */
    64, /* bit 6's value is 64 */
    128 /* bit 7's value is 128 */
};

/* Table of bit-masks for each number of bits in a byte */
/*  (This will need to be changed/expanded if another base type is used) */
static const uint8 bv_bit_mask[9] = {
    0x00, /* 0 bits is a mask of 0x00 */
    0x01, /* 1 bits is a mask of 0x01 */
    0x03, /* 2 bits is a mask of 0x03 */
    0x07, /* 3 bits is a mask of 0x07 */
    0x0F, /* 4 bits is a mask of 0x0F */
    0x1F, /* 5 bits is a mask of 0x1F */
    0x3F, /* 6 bits is a mask of 0x3F */
    0x7F, /* 7 bits is a mask of 0x7F */
    0xFF  /* 8 bits is a mask of 0xFF */
};

/* Table of "first zero bit" for each byte value */
/*  (This will need to be changed/expanded if another base type is used) */
static const int8 bv_first_zero[256] = {
    0, /* "0" - bit 0 is lowest zero */ 1,   /* "1" - bit 1 is lowest zero */
    0, /* "2" - bit 0 is lowest zero */ 2,   /* "3" - bit 2 is lowest zero */
    0, /* "4" - bit 0 is lowest zero */ 1,   /* "5" - bit 1 is lowest zero */
    0, /* "6" - bit 0 is lowest zero */ 3,   /* "7" - bit 3 is lowest zero */
    0, /* "8" - bit 0 is lowest zero */ 1,   /* "9" - bit 1 is lowest zero */
    0, /* "10" - bit 0 is lowest zero */ 2,  /* "11" - bit 2 is lowest zero */
    0, /* "12" - bit 0 is lowest zero */ 1,  /* "13" - bit 1 is lowest zero */
    0, /* "14" - bit 0 is lowest zero */ 4,  /* "15" - bit 4 is lowest zero */
    0, /* "16" - bit 0 is lowest zero */ 1,  /* "17" - bit 1 is lowest zero */
    0, /* "18" - bit 0 is lowest zero */ 2,  /* "19" - bit 2 is lowest zero */
    0, /* "20" - bit 0 is lowest zero */ 1,  /* "21" - bit 1 is lowest zero */
    0, /* "22" - bit 0 is lowest zero */ 3,  /* "23" - bit 3 is lowest zero */
    0, /* "24" - bit 0 is lowest zero */ 1,  /* "25" - bit 1 is lowest zero */
    0, /* "26" - bit 0 is lowest zero */ 2,  /* "27" - bit 2 is lowest zero */
    0, /* "28" - bit 0 is lowest zero */ 1,  /* "29" - bit 1 is lowest zero */
    0, /* "30" - bit 0 is lowest zero */ 5,  /* "31" - bit 5 is lowest zero */
    0, /* "32" - bit 0 is lowest zero */ 1,  /* "33" - bit 1 is lowest zero */
    0, /* "34" - bit 0 is lowest zero */ 2,  /* "35" - bit 2 is lowest zero */
    0, /* "36" - bit 0 is lowest zero */ 1,  /* "37" - bit 1 is lowest zero */
    0, /* "38" - bit 0 is lowest zero */ 3,  /* "39" - bit 3 is lowest zero */
    0, /* "40" - bit 0 is lowest zero */ 1,  /* "41" - bit 1 is lowest zero */
    0, /* "42" - bit 0 is lowest zero */ 2,  /* "43" - bit 2 is lowest zero */
    0, /* "44" - bit 0 is lowest zero */ 1,  /* "45" - bit 1 is lowest zero */
    0, /* "46" - bit 0 is lowest zero */ 4,  /* "47" - bit 4 is lowest zero */
    0, /* "48" - bit 0 is lowest zero */ 1,  /* "49" - bit 1 is lowest zero */
    0, /* "50" - bit 0 is lowest zero */ 2,  /* "51" - bit 2 is lowest zero */
    0, /* "52" - bit 0 is lowest zero */ 1,  /* "53" - bit 1 is lowest zero */
    0, /* "54" - bit 0 is lowest zero */ 3,  /* "55" - bit 3 is lowest zero */
    0, /* "56" - bit 0 is lowest zero */ 1,  /* "57" - bit 1 is lowest zero */
    0, /* "58" - bit 0 is lowest zero */ 2,  /* "59" - bit 2 is lowest zero */
    0, /* "60" - bit 0 is lowest zero */ 1,  /* "61" - bit 1 is lowest zero */
    0, /* "62" - bit 0 is lowest zero */ 6,  /* "63" - bit 6 is lowest zero */
    0, /* "64" - bit 0 is lowest zero */ 1,  /* "65" - bit 1 is lowest zero */
    0, /* "66" - bit 0 is lowest zero */ 2,  /* "67" - bit 2 is lowest zero */
    0, /* "68" - bit 0 is lowest zero */ 1,  /* "69" - bit 1 is lowest zero */
    0, /* "70" - bit 0 is lowest zero */ 3,  /* "71" - bit 3 is lowest zero */
    0, /* "72" - bit 0 is lowest zero */ 1,  /* "73" - bit 1 is lowest zero */
    0, /* "74" - bit 0 is lowest zero */ 2,  /* "75" - bit 2 is lowest zero */
    0, /* "76" - bit 0 is lowest zero */ 1,  /* "77" - bit 1 is lowest zero */
    0, /* "78" - bit 0 is lowest zero */ 4,  /* "79" - bit 4 is lowest zero */
    0, /* "80" - bit 0 is lowest zero */ 1,  /* "81" - bit 1 is lowest zero */
    0, /* "82" - bit 0 is lowest zero */ 2,  /* "83" - bit 2 is lowest zero */
    0, /* "84" - bit 0 is lowest zero */ 1,  /* "85" - bit 1 is lowest zero */
    0, /* "86" - bit 0 is lowest zero */ 3,  /* "87" - bit 3 is lowest zero */
    0, /* "88" - bit 0 is lowest zero */ 1,  /* "89" - bit 1 is lowest zero */
    0, /* "90" - bit 0 is lowest zero */ 2,  /* "91" - bit 2 is lowest zero */
    0, /* "92" - bit 0 is lowest zero */ 1,  /* "93" - bit 1 is lowest zero */
    0, /* "94" - bit 0 is lowest zero */ 5,  /* "95" - bit 5 is lowest zero */
    0, /* "96" - bit 0 is lowest zero */ 1,  /* "97" - bit 1 is lowest zero */
    0, /* "98" - bit 0 is lowest zero */ 2,  /* "99" - bit 2 is lowest zero */
    0, /* "100" - bit 0 is lowest zero */ 1, /* "101" - bit 1 is lowest zero */
    0, /* "102" - bit 0 is lowest zero */ 3, /* "103" - bit 3 is lowest zero */
    0, /* "104" - bit 0 is lowest zero */ 1, /* "105" - bit 1 is lowest zero */
    0, /* "106" - bit 0 is lowest zero */ 2, /* "107" - bit 2 is lowest zero */
    0, /* "108" - bit 0 is lowest zero */ 1, /* "109" - bit 1 is lowest zero */
    0, /* "110" - bit 0 is lowest zero */ 4, /* "111" - bit 4 is lowest zero */
    0, /* "112" - bit 0 is lowest zero */ 1, /* "113" - bit 1 is lowest zero */
    0, /* "114" - bit 0 is lowest zero */ 2, /* "115" - bit 2 is lowest zero */
    0, /* "116" - bit 0 is lowest zero */ 1, /* "117" - bit 1 is lowest zero */
    0, /* "118" - bit 0 is lowest zero */ 3, /* "119" - bit 3 is lowest zero */
    0, /* "120" - bit 0 is lowest zero */ 1, /* "121" - bit 1 is lowest zero */
    0, /* "122" - bit 0 is lowest zero */ 2, /* "123" - bit 2 is lowest zero */
    0, /* "124" - bit 0 is lowest zero */ 1, /* "125" - bit 1 is lowest zero */
    0, /* "126" - bit 0 is lowest zero */ 7, /* "127" - bit 7 is lowest zero */
    0, /* "128" - bit 0 is lowest zero */ 1, /* "129" - bit 1 is lowest zero */
    0, /* "130" - bit 0 is lowest zero */ 2, /* "131" - bit 2 is lowest zero */
    0, /* "132" - bit 0 is lowest zero */ 1, /* "133" - bit 1 is lowest zero */
    0, /* "134" - bit 0 is lowest zero */ 3, /* "135" - bit 3 is lowest zero */
    0, /* "136" - bit 0 is lowest zero */ 1, /* "137" - bit 1 is lowest zero */
    0, /* "138" - bit 0 is lowest zero */ 2, /* "139" - bit 2 is lowest zero */
    0, /* "140" - bit 0 is lowest zero */ 1, /* "141" - bit 1 is lowest zero */
    0, /* "142" - bit 0 is lowest zero */ 4, /* "143" - bit 4 is lowest zero */
    0, /* "144" - bit 0 is lowest zero */ 1, /* "145" - bit 1 is lowest zero */
    0, /* "146" - bit 0 is lowest zero */ 2, /* "147" - bit 2 is lowest zero */
    0, /* "148" - bit 0 is lowest zero */ 1, /* "149" - bit 1 is lowest zero */
    0, /* "150" - bit 0 is lowest zero */ 3, /* "151" - bit 3 is lowest zero */
    0, /* "152" - bit 0 is lowest zero */ 1, /* "153" - bit 1 is lowest zero */
    0, /* "154" - bit 0 is lowest zero */ 2, /* "155" - bit 2 is lowest zero */
    0, /* "156" - bit 0 is lowest zero */ 1, /* "157" - bit 1 is lowest zero */
    0, /* "158" - bit 0 is lowest zero */ 5, /* "159" - bit 5 is lowest zero */
    0, /* "160" - bit 0 is lowest zero */ 1, /* "161" - bit 1 is lowest zero */
    0, /* "162" - bit 0 is lowest zero */ 2, /* "163" - bit 2 is lowest zero */
    0, /* "164" - bit 0 is lowest zero */ 1, /* "165" - bit 1 is lowest zero */
    0, /* "166" - bit 0 is lowest zero */ 3, /* "167" - bit 3 is lowest zero */
    0, /* "168" - bit 0 is lowest zero */ 1, /* "169" - bit 1 is lowest zero */
    0, /* "170" - bit 0 is lowest zero */ 2, /* "171" - bit 2 is lowest zero */
    0, /* "172" - bit 0 is lowest zero */ 1, /* "173" - bit 1 is lowest zero */
    0, /* "174" - bit 0 is lowest zero */ 4, /* "175" - bit 4 is lowest zero */
    0, /* "176" - bit 0 is lowest zero */ 1, /* "177" - bit 1 is lowest zero */
    0, /* "178" - bit 0 is lowest zero */ 2, /* "179" - bit 2 is lowest zero */
    0, /* "180" - bit 0 is lowest zero */ 1, /* "181" - bit 1 is lowest zero */
    0, /* "182" - bit 0 is lowest zero */ 3, /* "183" - bit 3 is lowest zero */
    0, /* "184" - bit 0 is lowest zero */ 1, /* "185" - bit 1 is lowest zero */
    0, /* "186" - bit 0 is lowest zero */ 2, /* "187" - bit 2 is lowest zero */
    0, /* "188" - bit 0 is lowest zero */ 1, /* "189" - bit 1 is lowest zero */
    0, /* "190" - bit 0 is lowest zero */ 6, /* "191" - bit 6 is lowest zero */
    0, /* "192" - bit 0 is lowest zero */ 1, /* "193" - bit 1 is lowest zero */
    0, /* "194" - bit 0 is lowest zero */ 2, /* "195" - bit 2 is lowest zero */
    0, /* "196" - bit 0 is lowest zero */ 1, /* "197" - bit 1 is lowest zero */
    0, /* "198" - bit 0 is lowest zero */ 3, /* "199" - bit 3 is lowest zero */
    0, /* "200" - bit 0 is lowest zero */ 1, /* "201" - bit 1 is lowest zero */
    0, /* "202" - bit 0 is lowest zero */ 2, /* "203" - bit 2 is lowest zero */
    0, /* "204" - bit 0 is lowest zero */ 1, /* "205" - bit 1 is lowest zero */
    0, /* "206" - bit 0 is lowest zero */ 4, /* "207" - bit 4 is lowest zero */
    0, /* "208" - bit 0 is lowest zero */ 1, /* "209" - bit 1 is lowest zero */
    0, /* "210" - bit 0 is lowest zero */ 2, /* "211" - bit 2 is lowest zero */
    0, /* "212" - bit 0 is lowest zero */ 1, /* "213" - bit 1 is lowest zero */
    0, /* "214" - bit 0 is lowest zero */ 3, /* "215" - bit 3 is lowest zero */
    0, /* "216" - bit 0 is lowest zero */ 1, /* "217" - bit 1 is lowest zero */
    0, /* "218" - bit 0 is lowest zero */ 2, /* "219" - bit 2 is lowest zero */
    0, /* "220" - bit 0 is lowest zero */ 1, /* "221" - bit 1 is lowest zero */
    0, /* "222" - bit 0 is lowest zero */ 5, /* "223" - bit 5 is lowest zero */
    0, /* "224" - bit 0 is lowest zero */ 1, /* "225" - bit 1 is lowest zero */
    0, /* "226" - bit 0 is lowest zero */ 2, /* "227" - bit 2 is lowest zero */
    0, /* "228" - bit 0 is lowest zero */ 1, /* "229" - bit 1 is lowest zero */
    0, /* "230" - bit 0 is lowest zero */ 3, /* "231" - bit 3 is lowest zero */
    0, /* "232" - bit 0 is lowest zero */ 1, /* "233" - bit 1 is lowest zero */
    0, /* "234" - bit 0 is lowest zero */ 2, /* "235" - bit 2 is lowest zero */
    0, /* "236" - bit 0 is lowest zero */ 1, /* "237" - bit 1 is lowest zero */
    0, /* "238" - bit 0 is lowest zero */ 4, /* "239" - bit 4 is lowest zero */
    0, /* "240" - bit 0 is lowest zero */ 1, /* "241" - bit 1 is lowest zero */
    0, /* "242" - bit 0 is lowest zero */ 2, /* "243" - bit 2 is lowest zero */
    0, /* "244" - bit 0 is lowest zero */ 1, /* "245" - bit 1 is lowest zero */
    0, /* "246" - bit 0 is lowest zero */ 3, /* "247" - bit 3 is lowest zero */
    0, /* "248" - bit 0 is lowest zero */ 1, /* "249" - bit 1 is lowest zero */
    0, /* "250" - bit 0 is lowest zero */ 2, /* "251" - bit 2 is lowest zero */
    0, /* "252" - bit 0 is lowest zero */ 1, /* "253" - bit 1 is lowest zero */
    0, /* "254" - bit 0 is lowest zero */ 8  /* "255" - bit 8 is lowest zero */
};

/*--------------------------------------------------------------------------
 NAME
    bv_new
 PURPOSE
    Create a new bit-vector.
 USAGE
    bv_ptr bv_new(num_bits)
        int32 num_bits;             IN: The initial number of bits in the vector
 RETURNS
    Returns either a valid bv_ptr on succeed or NULL on failure.
 DESCRIPTION
    Creates a new bit-vector with a certain initial # of bits.
 COMMENTS, BUGS, ASSUMPTIONS
    * If num_bits is set to -1, then the default number of bits is used.
    * num_bits must be positive or -1.
--------------------------------------------------------------------------*/
bv_ptr
bv_new(int32 num_bits)
{
    int32  base_elements;  /* # of base elements needed to store the # of bits requested */
    size_t array_mem_size; /* size of the array in memory */
    bv_ptr b = NULL;       /* ptr to the new bit-vector */

    /* Check for invalid numbers of bits */
    if (num_bits < -1 || num_bits == 0)
        goto error;

    /* Check for requesting the default # of bits */
    if (num_bits == -1)
        num_bits = BV_DEFAULT_BITS;

    base_elements =
        ((num_bits % BV_BASE_BITS) > 0) ? (num_bits / BV_BASE_BITS) + 1 : (num_bits / BV_BASE_BITS);

    if ((b = malloc(sizeof(bv_struct))) == NULL)
        goto error;

    b->bits_used  = num_bits;
    b->array_size = ((base_elements / BV_CHUNK_SIZE) + 1) * BV_CHUNK_SIZE;

    array_mem_size = sizeof(bv_base) * (size_t)(b->array_size);
    if ((b->buffer = malloc(array_mem_size)) == NULL)
        goto error;

    /* Zero the initial bits */
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
 COMMENTS, BUGS, ASSUMPTIONS
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

            num_chunks = ((((bit_num / BV_BASE_BITS) + 1) - b->array_size) / BV_CHUNK_SIZE) + 1;
            new_size   = (size_t)(b->array_size + num_chunks * BV_CHUNK_SIZE);
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
        return FAIL;

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
    bv_find_next_zero
 PURPOSE
    Find the next zero bit
 USAGE
    int32 bv_find_next_zero(b)
        bv_ptr b;                   IN: Bit-vector to use
 RETURNS
    Returns offset of next zero bit on success, FAIL on error
 DESCRIPTION
    Find the next highest zero bit.
 COMMENTS, BUGS, ASSUMPTIONS
--------------------------------------------------------------------------*/
int32
bv_find_next_zero(bv_ptr b)
{
    int32    old_bits_used; /* the last number of bits used */
    int32    bytes_used;    /* number of full bytes used */
    bv_base  slush_bits;    /* extra bits which don't fit into a byte */
    int32    i;             /* local counting variable */
    bv_base *tmp_buf;

    if (b == NULL || b->buffer == NULL)
        return FAIL;

    bytes_used = b->bits_used / BV_BASE_BITS;

    /* looking for first '0' in the bit-vector */

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

    /* Beyond the current end of the bit-vector, extend the bit-vector */
    old_bits_used = b->bits_used;
    if (bv_set(b, b->bits_used, BV_FALSE) == FAIL)
        return FAIL;

    return old_bits_used;
} /* bv_find() */
