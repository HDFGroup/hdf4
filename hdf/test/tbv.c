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

/*
   FILE
   tbv.c
   Test HDF bit-vector (bv) routines.

   REMARKS

   DESIGN

   BUGS/LIMITATIONS

   EXPORTED ROUTINES

   AUTHOR
   Quincey Koziol

   MODIFICATION HISTORY
   12/11/95 - Started coding.
 */

#include "tproto.h"
#include "bitvect.h"

static void test_1(void);
static void test_2(void);
static void test_3(void);

/* Basic creation & deletion tests */
static void test_1(void)
{
    bv_ptr b;
    int32 size;
    uint32 flags;
    intn ret;

    MESSAGE(6, printf("Testing basic bit-vector creation & deletion\n"););

    MESSAGE(7, printf("Create a bit-vector with all defaults\n"););
    b=bv_new(-1,0); /* test basic default creation */
    CHECK_VOID(b,NULL,"bv_new");
    size=bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%d\n",(int)size););
    VERIFY_VOID(size,BV_DEFAULT_BITS,"bv_size");
    flags=bv_flags(b);
    CHECK_VOID(flags,(uint32)FAIL,"bv_flags");
    MESSAGE(8, printf("Bit-vector flags=%lx\n",(unsigned long)flags););
    ret=bv_delete(b);
    CHECK_VOID(ret,FAIL,"bv_delete");

    MESSAGE(7, printf("Create an extendable bit-vector with large # of bits\n"););
    b=bv_new(80000,BV_EXTENDABLE); /* test creation */
    CHECK_VOID(b,NULL,"bv_new");
    size=bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%d\n",(int)size););
    VERIFY_VOID(size,80000,"bv_size");
    flags=bv_flags(b);
    CHECK_VOID(flags,(uint32)FAIL,"bv_flags");
    MESSAGE(8, printf("Bit-vector flags=%lx\n",(unsigned long)flags););
    ret=bv_delete(b);
    CHECK_VOID(ret,FAIL,"bv_delete");
} /* end test_1 */

/* Basic set & get tests */
static void test_2(void)
{
    bv_ptr b;
    int32 size;
    uint32 flags;
    intn ret;

    MESSAGE(6, printf("Testing basic bit-vector set & get \n"););

    MESSAGE(7, printf("Create a bit-vector with all defaults\n"););
    b=bv_new(-1,0); /* test basic default creation */
    CHECK_VOID(b,NULL,"bv_new");
    size=bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%d\n",(int)size););
    VERIFY_VOID(size,BV_DEFAULT_BITS,"bv_size");
    flags=bv_flags(b);
    CHECK_VOID(flags,(uint32)FAIL,"bv_flags");
    MESSAGE(8, printf("Bit-vector flags=%lx\n",(unsigned long)flags););
    /* Check setting bits */
    ret=bv_set(b,13,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,3,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,150,BV_TRUE);
    VERIFY_VOID(ret,FAIL,"bv_set");
    /* Check getting bits */
    ret=bv_get(b,2);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,3);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,0);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,13);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,-1);
    VERIFY_VOID(ret,FAIL,"bv_get");
    ret=bv_delete(b);
    CHECK_VOID(ret,FAIL,"bv_delete");

    MESSAGE(7, printf("Create an extendable bit-vector with large # of bits\n"););
    b=bv_new(1000,BV_EXTENDABLE); /* test creation */
    CHECK_VOID(b,NULL,"bv_new");
    size=bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%d\n",(int)size););
    VERIFY_VOID(size,1000,"bv_size");
    flags=bv_flags(b);
    CHECK_VOID(flags,(uint32)FAIL,"bv_flags");
    MESSAGE(8, printf("Bit-vector flags=%lx\n",(unsigned long)flags););
    /* Check setting bits */
    ret=bv_set(b,13,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,3,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,1050,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    /* Check getting bits */
    ret=bv_get(b,2);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,3);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,0);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,13);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,1040);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,1050);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_delete(b);
    CHECK_VOID(ret,FAIL,"bv_delete");
} /* end test_2 */

/* Advanced set & get tests */
static void test_3(void)
{
    bv_ptr b;
    int32 size;
    uint32 flags;
    intn ret;

    MESSAGE(6, printf("Testing basic bit-vector set & get \n"););

    MESSAGE(7, printf("Create an extendable bit-vector\n"););
    b=bv_new(-1,BV_EXTENDABLE); /* test creation */
    CHECK_VOID(b,NULL,"bv_new");
    size=bv_size(b);
    MESSAGE(8, printf("Bit-vector size=%d\n",(int)size););
    VERIFY_VOID(size,BV_DEFAULT_BITS,"bv_size");
    flags=bv_flags(b);
    CHECK_VOID(flags,(uint32)FAIL,"bv_flags");
    MESSAGE(8, printf("Bit-vector flags=%lx\n",(unsigned long)flags););

    /* Check setting bits */
    ret=bv_set(b,13,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,3,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");
    ret=bv_set(b,150,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");

    /* Check getting bits */
    ret=bv_get(b,2);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,3);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,0);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,13);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");
    ret=bv_get(b,140);
    VERIFY_VOID(ret,BV_FALSE,"bv_get");
    ret=bv_get(b,150);
    VERIFY_VOID(ret,BV_TRUE,"bv_get");

    size=bv_find(b,-1,BV_FALSE);
    CHECK_VOID(size,FAIL,"bv_find");
    MESSAGE(8, printf("First 0 found at: %lu\n",(unsigned long)size););
    ret=bv_set(b,size,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");

    size=bv_find(b,-1,BV_FALSE);
    CHECK_VOID(size,FAIL,"bv_find");
    MESSAGE(8, printf("Second 0 found at: %lu\n",(unsigned long)size););
    ret=bv_set(b,size,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");

    size=bv_find(b,-1,BV_FALSE);
    CHECK_VOID(size,FAIL,"bv_find");
    MESSAGE(8, printf("Third 0 found at: %lu\n",(unsigned long)size););
    ret=bv_set(b,size,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");

    size=bv_find(b,-1,BV_FALSE);
    CHECK_VOID(size,FAIL,"bv_find");
    MESSAGE(8, printf("Fourth 0 found at: %lu\n",(unsigned long)size););
    ret=bv_set(b,size,BV_TRUE);
    CHECK_VOID(ret,FAIL,"bv_set");

    ret=bv_delete(b);
    CHECK_VOID(ret,FAIL,"bv_delete");
} /* end test_3 */

void test_bitvect(void)
{
    test_1();   /* basic creation & deletion tests */
    test_2();   /* basic set & get testing */
    test_3();   /* advanced set & get testing */
}   /* end test_bitvect() */

