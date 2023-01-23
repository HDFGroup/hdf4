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
   tbv.c
   Test HDF bit-vector (bv) routines.
 */

#include "tproto.h"
#include "bitvect.h"

static void test_1(void);
static void test_2(void);
static void test_3(void);
static void test_4(void);

/* Basic creation & deletion tests */
static void
test_1(void)
{
    bv_ptr b;
    int32  size;
    intn   ret;

    MESSAGE(6, printf("Testing basic bit-vector creation & deletion\n"););

    /* Test basic default creation */
    MESSAGE(7, printf("Create a bit-vector with all defaults\n"););
    b = bv_new(-1);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, BV_DEFAULT_BITS, "bv_size");
    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");

    /* Test creating a bit-vector w/ size >> defaults */
    MESSAGE(7, printf("Create an extendable bit-vector with large # of bits\n"););
    b = bv_new(80000);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, 80000, "bv_size");
    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");
} /* end test_1 */

/* Basic set & get tests */
static void
test_2(void)
{
    bv_ptr b;
    int32  size;
    intn   ret;

    MESSAGE(6, printf("Testing basic bit-vector set & get\n"););

    /* test basic default creation */
    MESSAGE(7, printf("Create a bit-vector with all defaults\n"););
    b = bv_new(-1);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, BV_DEFAULT_BITS, "bv_size");

    /* Check setting bits */
    ret = bv_set(b, 13, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 3, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 150, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* Check getting bits */
    ret = bv_get(b, 2);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 3);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, 0);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 13);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, -1);
    VERIFY_VOID(ret, FAIL, "bv_get");

    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");

    /* Test another bit vector with a large number of bits */
    MESSAGE(7, printf("Create an extendable bit-vector with large # of bits\n"););
    b = bv_new(1000);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, 1000, "bv_size");

    /* Check setting bits */
    ret = bv_set(b, 13, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 3, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 1050, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* Check getting bits */
    ret = bv_get(b, 2);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 3);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, 0);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 13);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, 1040);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 1050);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");

    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");
} /* end test_2 */

/* Advanced set & get tests */
static void
test_3(void)
{
    bv_ptr b;
    int32  size;
    int32  bit_num;
    intn   ret;

    MESSAGE(6, printf("Testing basic bit-vector set & get\n"););

    /* Create an extendable bit-vector */
    MESSAGE(7, printf("Create an extendable bit-vector\n"););
    b = bv_new(-1);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, BV_DEFAULT_BITS, "bv_size");

    /* Check setting bits */
    ret = bv_set(b, 13, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 3, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 150, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 152, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* Check getting bits */
    ret = bv_get(b, 2);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 3);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, 0);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 13);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");
    ret = bv_get(b, 140);
    VERIFY_VOID(ret, BV_FALSE, "bv_get");
    ret = bv_get(b, 150);
    VERIFY_VOID(ret, BV_TRUE, "bv_get");

    /* Check finding 0s in the bit-vector, remembering the last one. This
     * should skip bit 3, which we set to 1 in the above code.
     */
    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 0, "bv_find_next_zero");
    MESSAGE(8, printf("First 0 found at: %" PRId32 "\n", bit_num););
    ret = bv_set(b, bit_num, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 1, "bv_find_next_zero");
    MESSAGE(8, printf("Second 0 found at: %" PRId32 "\n", bit_num););
    ret = bv_set(b, bit_num, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 2, "bv_find_next_zero");
    MESSAGE(8, printf("Third 0 found at: %" PRId32 "\n", bit_num););
    ret = bv_set(b, bit_num, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 4, "bv_find_next_zero");
    MESSAGE(8, printf("Fourth 0 found at: %" PRId32 "\n", bit_num););
    ret = bv_set(b, bit_num, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");
} /* end test_3 */

/* Pathologic set/find test */
static void
test_4(void)
{
    bv_ptr b;
    int32  size;
    int32  bit_num;
    intn   ret;

    MESSAGE(6, printf("Testing pathologic bit-vector set & get\n"););

    /* Create an extendable bit-vector */
    MESSAGE(7, printf("Create an extendable bit-vector\n"););
    b = bv_new(16);
    CHECK_VOID(b, NULL, "bv_new");
    size = bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%" PRId32 "\n", size););
    VERIFY_VOID(size, 16, "bv_size");

    /* Set all the bits to 1 */
    for (int i = 0; i < 16; i++) {
        ret = bv_set(b, i, BV_TRUE);
        CHECK_VOID(ret, FAIL, "bv_set");
    }

    /* Set two bits to 0 (order is important!), one in each byte. At this
     * point the cached first zero should be 5.
     */
    ret = bv_set(b, 13, BV_FALSE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 5, BV_FALSE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* Now set the lower bit to 1. This should invalidate the cached value. */
    ret = bv_set(b, 5, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* See if we can find the remaining 0 */
    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 13, "bv_find_next_zero");
    MESSAGE(8, printf("Remaining 0 found at: %" PRId32 "\n", bit_num););

    /* Reset the zero bits */
    ret = bv_set(b, 13, BV_FALSE);
    CHECK_VOID(ret, FAIL, "bv_set");
    ret = bv_set(b, 5, BV_FALSE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* Now set the higher bit to 1, just to make sure this order works */
    ret = bv_set(b, 13, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* See if we can find the remaining 0 */
    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 5, "bv_find_next_zero");
    MESSAGE(8, printf("Remaining 0 found at: %" PRId32 "\n", bit_num););

    /* Now set remaining bit to 1 */
    ret = bv_set(b, 5, BV_TRUE);
    CHECK_VOID(ret, FAIL, "bv_set");

    /* The next bit should extend the array */
    bit_num = bv_find_next_zero(b);
    CHECK_VOID(bit_num, FAIL, "bv_find_next_zero");
    VERIFY_VOID(bit_num, 16, "bv_find_next_zero");
    MESSAGE(8, printf("Remaining 0 found at: %" PRId32 "\n", bit_num););

    ret = bv_delete(b);
    CHECK_VOID(ret, FAIL, "bv_delete");
} /* end test_4 */

void
test_bitvect(void)
{
    test_1(); /* basic creation & deletion tests */
    test_2(); /* basic set & get testing */
    test_3(); /* advanced set & get testing */
    test_4(); /* pathological set & find testing */
}
