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

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/*
 FILE
       conv.c
       Test HDF Number-Type conversion routines

 REMARKS

 DESIGN

 BUGS/LIMITATIONS

 EXPORTED ROUTINES

 AUTHOR
       Quincey Koziol

 MODIFICATION HISTORY
   10/27/93 - Started coding.
*/

#include "tproto.h"
#include <time.h>

/* Substitute bogus value if CLOCKS_PER_SEC is unavailable */
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC -1
#endif

#ifdef UNICOS
#define TEST_SIZE 1000001
#else
#if defined PC | defined macintosh
#define TEST_SIZE 8000         /* so that 8*8000<=64K */
#else
#define TEST_SIZE 100000
#endif
#endif

/* define aliases for random number generation */
#define RAND rand
#define SEED(a) srand(a)

extern int num_errs;
extern int Verbocity;

void test_conv()
{
    clock_t c1,c2,c3,c4;
    uint8 *src_uint8,
	  *dst_uint8,
	  *dst2_uint8;
    uint16 *src_uint16,
	  *dst_uint16,
	  *dst2_uint16;
    uint32 *src_uint32,
	  *dst_uint32,
	  *dst2_uint32;
    int8 *src_int8,
	  *dst_int8,
	  *dst2_int8;
    int16 *src_int16,
	  *dst_int16,
	  *dst2_int16;
    int32 *src_int32,
	  *dst_int32,
	  *dst2_int32;
    float32 *src_float32,
	  *dst_float32,
	  *dst2_float32;
    float64 *src_float64,
	  *dst_float64,
	  *dst2_float64;
    intn i,r;
    int32 ret;

    SEED((int)time(NULL));   /* seed with effectively random number */

    MESSAGE(5,printf("Testing Big-Endian Number-Types\n"););
    MESSAGE(6,printf("seeding int8 array\n"););

    /* allocate arrays */
    src_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(src_int8==NULL) {
    	CHECK(src_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst_int8==NULL) {
    	CHECK(dst_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst2_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst2_int8==NULL) {
        CHECK(dst2_int8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int8[i]=(int8)RAND();

    MESSAGE(6,printf("converting int8 array\n"););
    c1=clock();
    ret=DFKconvert(src_int8,dst_int8,DFNT_INT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting int8 array\n"););
    c3=clock();
    DFKconvert(dst_int8,dst2_int8,DFNT_INT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int8,dst2_int8,TEST_SIZE*sizeof(int8))) {
        printf("Error converting int8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int8);
    HDfreespace((VOIDP)dst_int8);
    HDfreespace((VOIDP)dst2_int8);

    MESSAGE(6,printf("seeding uint8 array\n"););
    src_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(src_uint8==NULL) {
        CHECK(src_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst_uint8==NULL) {
        CHECK(dst_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst2_uint8==NULL) {
        CHECK(dst2_uint8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint8[i]=(uint8)RAND();

    MESSAGE(6,printf("converting uint8 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint8,dst_uint8,DFNT_UINT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting uint8 array\n"););
    c3=clock();
    DFKconvert(dst_uint8,dst2_uint8,DFNT_UINT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint8,dst2_uint8,TEST_SIZE*sizeof(uint8))) {
        printf("Error converting uint8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint8);
    HDfreespace((VOIDP)dst_uint8);
    HDfreespace((VOIDP)dst2_uint8);

    MESSAGE(6,printf("seeding int16 array\n"););
    src_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(src_int16==NULL) {
        CHECK(src_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst_int16==NULL) {
        CHECK(dst_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst2_int16==NULL) {
        CHECK(dst2_int16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int16[i]=(int16)RAND();

    MESSAGE(6,printf("converting int16 array\n"););
    c1=clock();
    ret=DFKconvert(src_int16,dst_int16,DFNT_INT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting int16 array\n"););
    c3=clock();
    DFKconvert(dst_int16,dst2_int16,DFNT_INT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int16,dst2_int16,TEST_SIZE*sizeof(int16))) {
        printf("Error converting int16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int16);
    HDfreespace((VOIDP)dst_int16);
    HDfreespace((VOIDP)dst2_int16);

    MESSAGE(6,printf("seeding uint16 array\n"););
    src_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(src_uint16==NULL) {
        CHECK(src_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst_uint16==NULL) {
        CHECK(dst_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst2_uint16==NULL) {
        CHECK(dst2_uint16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint16[i]=(uint16)RAND();

    MESSAGE(6,printf("converting uint16 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint16,dst_uint16,DFNT_UINT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting uint16 array\n"););
    c3=clock();
    DFKconvert(dst_uint16,dst2_uint16,DFNT_UINT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint16,dst2_uint16,TEST_SIZE*sizeof(uint16))) {
        printf("Error converting uint16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint16);
    HDfreespace((VOIDP)dst_uint16);
    HDfreespace((VOIDP)dst2_uint16);

    MESSAGE(6,printf("seeding int32 array\n"););
    src_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(src_int32==NULL) {
        CHECK(src_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst_int32==NULL) {
        CHECK(dst_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst2_int32==NULL) {
        CHECK(dst2_int32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int32[i]=(int32)RAND();

    MESSAGE(6,printf("converting int32 array\n"););
    c1=clock();
    ret=DFKconvert(src_int32,dst_int32,DFNT_INT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting int32 array\n"););
    c3=clock();
    DFKconvert(dst_int32,dst2_int32,DFNT_INT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d int32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int32,dst2_int32,TEST_SIZE*sizeof(int32))) {
        printf("Error converting int32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int32);
    HDfreespace((VOIDP)dst_int32);
    HDfreespace((VOIDP)dst2_int32);

    MESSAGE(6,printf("seeding uint32 array\n"););
    src_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(src_uint32==NULL) {
        CHECK(src_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst_uint32==NULL) {
        CHECK(dst_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst2_uint32==NULL) {
        CHECK(dst2_uint32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint32[i]=(uint32)RAND();

    MESSAGE(6,printf("converting uint32 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint32,dst_uint32,DFNT_UINT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting uint32 array\n"););
    c3=clock();
    DFKconvert(dst_uint32,dst2_uint32,DFNT_UINT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d uint32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint32,dst2_uint32,TEST_SIZE*sizeof(uint32))) {
        printf("Error converting uint32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint32);
    HDfreespace((VOIDP)dst_uint32);
    HDfreespace((VOIDP)dst2_uint32);

    MESSAGE(6,printf("seeding float32 array\n"););
    src_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(src_float32==NULL) {
        CHECK(src_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst_float32==NULL) {
        CHECK(dst_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst2_float32==NULL) {
        CHECK(dst2_float32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float32[i]=(float32)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float32[i]/=(float32)r;
      } /* end for */

    MESSAGE(6,printf("converting float32 array\n"););
    c1=clock();
    ret=DFKconvert(src_float32,dst_float32,DFNT_FLOAT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d float32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting float32 array\n"););
    c3=clock();
    DFKconvert(dst_float32,dst2_float32,DFNT_FLOAT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d float32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float32,dst2_float32,TEST_SIZE*sizeof(float32))) {
        printf("Error converting float32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float32);
    HDfreespace((VOIDP)dst_float32);
    HDfreespace((VOIDP)dst2_float32);

    MESSAGE(6,printf("seeding float64 array\n"););
    src_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(src_float64==NULL) {
        CHECK(src_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst_float64==NULL) {
        CHECK(dst_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst2_float64==NULL) {
        CHECK(dst2_float64,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float64[i]=(float64)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float64[i]/=(float64)r;
      } /* end for */

    MESSAGE(6,printf("converting float64 array\n"););
    c1=clock();
    ret=DFKconvert(src_float64,dst_float64,DFNT_FLOAT64,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d float64 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting float64 array\n"););
    c3=clock();
    DFKconvert(dst_float64,dst2_float64,DFNT_FLOAT64,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d float64 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float64,dst2_float64,TEST_SIZE*sizeof(float64))) {
        printf("Error converting float64 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float64);
    HDfreespace((VOIDP)dst_float64);
    HDfreespace((VOIDP)dst2_float64);

    MESSAGE(5,printf("Testing Little-Endian Number-Types\n"););
    MESSAGE(6,printf("seeding little-endian int8 array\n"););

    /* allocate arrays */
    src_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(src_int8==NULL) {
    	CHECK(src_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst_int8==NULL) {
    	CHECK(dst_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst2_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst2_int8==NULL) {
        CHECK(dst2_int8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int8[i]=(int8)RAND();

    MESSAGE(6,printf("converting little-endian int8 array\n"););
    c1=clock();
    ret=DFKconvert(src_int8,dst_int8,DFNT_LITEND|DFNT_INT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian int8 array\n"););
    c3=clock();
    DFKconvert(dst_int8,dst2_int8,DFNT_LITEND|DFNT_INT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int8,dst2_int8,TEST_SIZE*sizeof(int8))) {
        printf("Error converting little-endian int8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int8);
    HDfreespace((VOIDP)dst_int8);
    HDfreespace((VOIDP)dst2_int8);

    MESSAGE(6,printf("seeding little-endian uint8 array\n"););
    src_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(src_uint8==NULL) {
        CHECK(src_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst_uint8==NULL) {
        CHECK(dst_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst2_uint8==NULL) {
        CHECK(dst2_uint8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint8[i]=(uint8)RAND();

    MESSAGE(6,printf("converting little-endian uint8 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint8,dst_uint8,DFNT_LITEND|DFNT_UINT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian uint8 array\n"););
    c3=clock();
    DFKconvert(dst_uint8,dst2_uint8,DFNT_LITEND|DFNT_UINT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint8,dst2_uint8,TEST_SIZE*sizeof(uint8))) {
        printf("Error converting little-endian uint8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint8);
    HDfreespace((VOIDP)dst_uint8);
    HDfreespace((VOIDP)dst2_uint8);

    MESSAGE(6,printf("seeding little-endian int16 array\n"););
    src_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(src_int16==NULL) {
        CHECK(src_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst_int16==NULL) {
        CHECK(dst_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst2_int16==NULL) {
        CHECK(dst2_int16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int16[i]=(int16)RAND();

    MESSAGE(6,printf("converting little-endian int16 array\n"););
    c1=clock();
    ret=DFKconvert(src_int16,dst_int16,DFNT_LITEND|DFNT_INT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian int16 array\n"););
    c3=clock();
    DFKconvert(dst_int16,dst2_int16,DFNT_LITEND|DFNT_INT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int16,dst2_int16,TEST_SIZE*sizeof(int16))) {
        printf("Error converting little-endian int16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int16);
    HDfreespace((VOIDP)dst_int16);
    HDfreespace((VOIDP)dst2_int16);

    MESSAGE(6,printf("seeding little-endian uint16 array\n"););
    src_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(src_uint16==NULL) {
        CHECK(src_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst_uint16==NULL) {
        CHECK(dst_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst2_uint16==NULL) {
        CHECK(dst2_uint16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint16[i]=(uint16)RAND();

    MESSAGE(6,printf("converting little-endian uint16 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint16,dst_uint16,DFNT_LITEND|DFNT_UINT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian uint16 array\n"););
    c3=clock();
    DFKconvert(dst_uint16,dst2_uint16,DFNT_LITEND|DFNT_UINT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint16,dst2_uint16,TEST_SIZE*sizeof(uint16))) {
        printf("Error converting little-endian uint16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint16);
    HDfreespace((VOIDP)dst_uint16);
    HDfreespace((VOIDP)dst2_uint16);

    MESSAGE(6,printf("seeding little-endian int32 array\n"););
    src_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(src_int32==NULL) {
        CHECK(src_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst_int32==NULL) {
        CHECK(dst_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst2_int32==NULL) {
        CHECK(dst2_int32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int32[i]=(int32)RAND();

    MESSAGE(6,printf("converting little-endian int32 array\n"););
    c1=clock();
    ret=DFKconvert(src_int32,dst_int32,DFNT_LITEND|DFNT_INT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian int32 array\n"););
    c3=clock();
    DFKconvert(dst_int32,dst2_int32,DFNT_LITEND|DFNT_INT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian int32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int32,dst2_int32,TEST_SIZE*sizeof(int32))) {
        printf("Error converting little-endian int32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int32);
    HDfreespace((VOIDP)dst_int32);
    HDfreespace((VOIDP)dst2_int32);

    MESSAGE(6,printf("seeding little-endian uint32 array\n"););
    src_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(src_uint32==NULL) {
        CHECK(src_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst_uint32==NULL) {
        CHECK(dst_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst2_uint32==NULL) {
        CHECK(dst2_uint32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint32[i]=(uint32)RAND();

    MESSAGE(6,printf("converting little-endian uint32 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint32,dst_uint32,DFNT_LITEND|DFNT_UINT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian uint32 array\n"););
    c3=clock();
    DFKconvert(dst_uint32,dst2_uint32,DFNT_LITEND|DFNT_UINT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian uint32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint32,dst2_uint32,TEST_SIZE*sizeof(uint32))) {
        printf("Error converting little-endian uint32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint32);
    HDfreespace((VOIDP)dst_uint32);
    HDfreespace((VOIDP)dst2_uint32);

    MESSAGE(6,printf("seeding little-endian float32 array\n"););
    src_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(src_float32==NULL) {
        CHECK(src_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst_float32==NULL) {
        CHECK(dst_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst2_float32==NULL) {
        CHECK(dst2_float32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float32[i]=(float32)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float32[i]/=(float32)r;
      } /* end for */

    MESSAGE(6,printf("converting little-endian float32 array\n"););
    c1=clock();
    ret=DFKconvert(src_float32,dst_float32,DFNT_LITEND|DFNT_FLOAT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian float32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian float32 array\n"););
    c3=clock();
    DFKconvert(dst_float32,dst2_float32,DFNT_LITEND|DFNT_FLOAT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian float32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float32,dst2_float32,TEST_SIZE*sizeof(float32))) {
        printf("Error converting little-endian float32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float32);
    HDfreespace((VOIDP)dst_float32);
    HDfreespace((VOIDP)dst2_float32);

    MESSAGE(6,printf("seeding little-endian float64 array\n"););
    src_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(src_float64==NULL) {
        CHECK(src_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst_float64==NULL) {
        CHECK(dst_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst2_float64==NULL) {
        CHECK(dst2_float64,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float64[i]=(float64)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float64[i]/=(float64)r;
      } /* end for */

    MESSAGE(6,printf("converting little-endian float64 array\n"););
    c1=clock();
    ret=DFKconvert(src_float64,dst_float64,DFNT_LITEND|DFNT_FLOAT64,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian float64 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting little-endian float64 array\n"););
    c3=clock();
    DFKconvert(dst_float64,dst2_float64,DFNT_LITEND|DFNT_FLOAT64,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d little-endian float64 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float64,dst2_float64,TEST_SIZE*sizeof(float64))) {
        printf("Error converting little-endian float64 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float64);
    HDfreespace((VOIDP)dst_float64);
    HDfreespace((VOIDP)dst2_float64);

    MESSAGE(5,printf("Testing Native Number-Types\n"););
    MESSAGE(6,printf("seeding native int8 array\n"););

    /* allocate arrays */
    src_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(src_int8==NULL) {
    	CHECK(src_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst_int8==NULL) {
    	CHECK(dst_int8,NULL,HDgetspace);
    	return;
      } /* end if */
    dst2_int8=(int8 *)HDgetspace(TEST_SIZE*sizeof(int8));
    if(dst2_int8==NULL) {
        CHECK(dst2_int8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int8[i]=(int8)RAND();

    MESSAGE(6,printf("converting native int8 array\n"););
    c1=clock();
    ret=DFKconvert(src_int8,dst_int8,DFNT_NATIVE|DFNT_INT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native int8 array\n"););
    c3=clock();
    DFKconvert(dst_int8,dst2_int8,DFNT_NATIVE|DFNT_INT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int8,dst2_int8,TEST_SIZE*sizeof(int8))) {
        printf("Error converting native int8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int8);
    HDfreespace((VOIDP)dst_int8);
    HDfreespace((VOIDP)dst2_int8);

    MESSAGE(6,printf("seeding native uint8 array\n"););
    src_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(src_uint8==NULL) {
        CHECK(src_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst_uint8==NULL) {
        CHECK(dst_uint8,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint8=(uint8 *)HDgetspace(TEST_SIZE*sizeof(uint8));
    if(dst2_uint8==NULL) {
        CHECK(dst2_uint8,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint8[i]=(uint8)RAND();

    MESSAGE(6,printf("converting native uint8 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint8,dst_uint8,DFNT_NATIVE|DFNT_UINT8,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint8 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native uint8 array\n"););
    c3=clock();
    DFKconvert(dst_uint8,dst2_uint8,DFNT_NATIVE|DFNT_UINT8,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint8 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint8,dst2_uint8,TEST_SIZE*sizeof(uint8))) {
        printf("Error converting native uint8 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint8);
    HDfreespace((VOIDP)dst_uint8);
    HDfreespace((VOIDP)dst2_uint8);

    MESSAGE(6,printf("seeding native int16 array\n"););
    src_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(src_int16==NULL) {
        CHECK(src_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst_int16==NULL) {
        CHECK(dst_int16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int16=(int16 *)HDgetspace(TEST_SIZE*sizeof(int16));
    if(dst2_int16==NULL) {
        CHECK(dst2_int16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int16[i]=(int16)RAND();

    MESSAGE(6,printf("converting native int16 array\n"););
    c1=clock();
    ret=DFKconvert(src_int16,dst_int16,DFNT_NATIVE|DFNT_INT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native int16 array\n"););
    c3=clock();
    DFKconvert(dst_int16,dst2_int16,DFNT_NATIVE|DFNT_INT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int16,dst2_int16,TEST_SIZE*sizeof(int16))) {
        printf("Error converting native int16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int16);
    HDfreespace((VOIDP)dst_int16);
    HDfreespace((VOIDP)dst2_int16);

    MESSAGE(6,printf("seeding native uint16 array\n"););
    src_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(src_uint16==NULL) {
        CHECK(src_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst_uint16==NULL) {
        CHECK(dst_uint16,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint16=(uint16 *)HDgetspace(TEST_SIZE*sizeof(uint16));
    if(dst2_uint16==NULL) {
        CHECK(dst2_uint16,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint16[i]=(uint16)RAND();

    MESSAGE(6,printf("converting native uint16 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint16,dst_uint16,DFNT_NATIVE|DFNT_UINT16,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint16 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native uint16 array\n"););
    c3=clock();
    DFKconvert(dst_uint16,dst2_uint16,DFNT_NATIVE|DFNT_UINT16,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint16 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint16,dst2_uint16,TEST_SIZE*sizeof(uint16))) {
        printf("Error converting native uint16 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint16);
    HDfreespace((VOIDP)dst_uint16);
    HDfreespace((VOIDP)dst2_uint16);

    MESSAGE(6,printf("seeding native int32 array\n"););
    src_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(src_int32==NULL) {
        CHECK(src_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst_int32==NULL) {
        CHECK(dst_int32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_int32=(int32 *)HDgetspace(TEST_SIZE*sizeof(int32));
    if(dst2_int32==NULL) {
        CHECK(dst2_int32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_int32[i]=(int32)RAND();

    MESSAGE(6,printf("converting native int32 array\n"););
    c1=clock();
    ret=DFKconvert(src_int32,dst_int32,DFNT_NATIVE|DFNT_INT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native int32 array\n"););
    c3=clock();
    DFKconvert(dst_int32,dst2_int32,DFNT_NATIVE|DFNT_INT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native int32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_int32,dst2_int32,TEST_SIZE*sizeof(int32))) {
        printf("Error converting native int32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_int32);
    HDfreespace((VOIDP)dst_int32);
    HDfreespace((VOIDP)dst2_int32);

    MESSAGE(6,printf("seeding native uint32 array\n"););
    src_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(src_uint32==NULL) {
        CHECK(src_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst_uint32==NULL) {
        CHECK(dst_uint32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_uint32=(uint32 *)HDgetspace(TEST_SIZE*sizeof(uint32));
    if(dst2_uint32==NULL) {
        CHECK(dst2_uint32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++)
        src_uint32[i]=(uint32)RAND();

    MESSAGE(6,printf("converting native uint32 array\n"););
    c1=clock();
    ret=DFKconvert(src_uint32,dst_uint32,DFNT_NATIVE|DFNT_UINT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native uint32 array\n"););
    c3=clock();
    DFKconvert(dst_uint32,dst2_uint32,DFNT_NATIVE|DFNT_UINT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native uint32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_uint32,dst2_uint32,TEST_SIZE*sizeof(uint32))) {
        printf("Error converting native uint32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_uint32);
    HDfreespace((VOIDP)dst_uint32);
    HDfreespace((VOIDP)dst2_uint32);

    MESSAGE(6,printf("seeding native float32 array\n"););
    src_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(src_float32==NULL) {
        CHECK(src_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst_float32==NULL) {
        CHECK(dst_float32,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float32=(float32 *)HDgetspace(TEST_SIZE*sizeof(float32));
    if(dst2_float32==NULL) {
        CHECK(dst2_float32,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float32[i]=(float32)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float32[i]/=(float32)r;
      } /* end for */

    MESSAGE(6,printf("converting native float32 array\n"););
    c1=clock();
    ret=DFKconvert(src_float32,dst_float32,DFNT_NATIVE|DFNT_FLOAT32,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native float32 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native float32 array\n"););
    c3=clock();
    DFKconvert(dst_float32,dst2_float32,DFNT_NATIVE|DFNT_FLOAT32,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native float32 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float32,dst2_float32,TEST_SIZE*sizeof(float32))) {
        printf("Error converting native float32 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float32);
    HDfreespace((VOIDP)dst_float32);
    HDfreespace((VOIDP)dst2_float32);

    MESSAGE(6,printf("seeding native float64 array\n"););
    src_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(src_float64==NULL) {
        CHECK(src_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst_float64==NULL) {
        CHECK(dst_float64,NULL,HDgetspace);
        return;
      } /* end if */
    dst2_float64=(float64 *)HDgetspace(TEST_SIZE*sizeof(float64));
    if(dst2_float64==NULL) {
        CHECK(dst2_float64,NULL,HDgetspace);
        return;
      } /* end if */

    /* Seed arrays with random values */
    for(i=0; i<TEST_SIZE; i++) {
        src_float64[i]=(float64)RAND();
        while((r=RAND())==0) /* don't divide by zero */
            r=RAND();
        src_float64[i]/=(float64)r;
      } /* end for */

    MESSAGE(6,printf("converting native float64 array\n"););
    c1=clock();
    ret=DFKconvert(src_float64,dst_float64,DFNT_NATIVE|DFNT_FLOAT64,TEST_SIZE,DFACC_WRITE,0,0);
    c2=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native float64 values\n",(int)(c2-c1),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););

    MESSAGE(6,printf("re-converting native float64 array\n"););
    c3=clock();
    DFKconvert(dst_float64,dst2_float64,DFNT_NATIVE|DFNT_FLOAT64,TEST_SIZE,DFACC_READ,0,0);
    c4=clock();
    RESULT("DFKconvert");
    MESSAGE(6,printf("%d/%d seconds to convert %d native float64 values\n",(int)(c4-c3),(int)CLOCKS_PER_SEC,(int)TEST_SIZE););
    if(HDmemcmp(src_float64,dst2_float64,TEST_SIZE*sizeof(float64))) {
        printf("Error converting native float64 values!\n");
        HEprint(stdout,0);
        num_errs++;
      } /* end if */

    HDfreespace((VOIDP)src_float64);
    HDfreespace((VOIDP)dst_float64);
    HDfreespace((VOIDP)dst2_float64);

} /* end test_conv() */
