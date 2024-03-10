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

/***************************************************************************
 *
 * Slab test routines slabw(), slabwf64(), slabwui16(), slabwui8(),
 *  slab1w(), slab2w(), slab3w(), slab4w()
 *
 ***************************************************************************/
/* slabwin & slabwuin do not work for all machines */

#include "testhdf.h"

/* Internal variables */
static int32 rank = 3;

static float64 maxf64  = 123.0;
static float64 minf64  = -1.0;
static float64 fillf64 = 1.0;

static float32 maxf32  = 123.0F;
static float32 minf32  = -1.0F;
static float32 fillf32 = 1.0F;

static int32 maxin  = 123;
static int32 minin  = -1;
static int32 fillin = 1;

static uint32 maxuin  = 123;
static uint32 minuin  = 2;
static uint32 filluin = 1;

static int32 maxi32  = 123;
static int32 mini32  = -1;
static int32 filli32 = 1;

static uint32 maxui32  = 123;
static uint32 minui32  = 2;
static uint32 fillui32 = 1;

static int16 maxi16  = 123;
static int16 mini16  = -1;
static int16 filli16 = 1;

static uint16 maxui16  = 123;
static uint16 minui16  = 2;
static uint16 fillui16 = 1;

static int8 maxi8  = 123;
static int8 mini8  = -1;
static int8 filli8 = 1;

static uint8 maxui8  = 123;
static uint8 minui8  = 2;
static uint8 fillui8 = 1;

/* Dimensions of slab */
static int32 size_dims[3]  = {2, 3, 4}; /* size of slab dims */
static int32 start_dims[3] = {1, 1, 1}; /* starting dims  */
static int32 stride[3]     = {0, 0, 0};
static int32 d_dims[3]     = {0, 0, 0};

/* luf for planes, rows and cols  */
static const char *lpln = "Time";
static const char *upln = "Second";
static const char *fpln = "Int32";
static const char *lrow = "Line";
static const char *urow = "Inch";
static const char *frow = "Int16";
static const char *lcol = "Column";
static const char *ucol = "Cm";
static const char *fcol = "Int32";

/* scales for planes, rows, and cols */
static float64 scplnf64[2] = {0.0, 100.0};
static float64 scrowf64[3] = {0.0, 10.0, 20.0};
static float64 sccolf64[4] = {0.0, 1.0, 2.0, 3.0};

static float32 scplnf32[2] = {0.0F, 100.0F};
static float32 scrowf32[3] = {0.0F, 10.0F, 20.0F};
static float32 sccolf32[4] = {0.0F, 1.0F, 2.0F, 3.0F};

static int32 scplnin[2] = {0, 100};
static int32 scrowin[3] = {0, 10, 20};
static int32 sccolin[4] = {0, 1, 2, 3};

static uint32 scplnuin[2] = {0, 100};
static uint32 scrowuin[3] = {0, 10, 20};
static uint32 sccoluin[4] = {0, 1, 2, 3};

static int32 scplni32[2] = {0, 100};
static int32 scrowi32[3] = {0, 10, 20};
static int32 sccoli32[4] = {0, 1, 2, 3};

static uint32 scplnui32[2] = {0, 100};
static uint32 scrowui32[3] = {0, 10, 20};
static uint32 sccolui32[4] = {0, 1, 2, 3};

static int16 scplni16[2] = {0, 100};
static int16 scrowi16[3] = {0, 10, 20};
static int16 sccoli16[4] = {0, 1, 2, 3};

static uint16 scplnui16[2] = {0, 100};
static uint16 scrowui16[3] = {0, 10, 20};
static uint16 sccolui16[4] = {0, 1, 2, 3};

static int8 scplni8[2] = {0, 100};
static int8 scrowi8[3] = {0, 10, 20};
static int8 sccoli8[4] = {0, 1, 2, 3};

static uint8 scplnui8[2] = {0, 100};
static uint8 scrowui8[3] = {0, 10, 20};
static uint8 sccolui8[4] = {0, 1, 2, 3};

/* Slabs for slabw(), slab1w(), slab2w() */
static float32 slabw1[1][1][3] = {{{110.0F, 111.0F, 112.0F}}};
static float32 slabw2[2][1][3] = {{{20.0F, 21.0F, 22.0F}}, {{120.0F, 121.0F, 122.0F}}};
static float32 slabw3[1][2][3] = {{{0.0F, 1.0F, 2.0F}, {10.0F, 11.0F, 12.0F}}};
static float32 slabw4[1][1][3] = {{{100.0F, 101.0F, 102.0F}}};
static float32 slabw5[2][3][1] = {{{3.0F}, {13.0F}, {23.0F}}, {{103.0F}, {113.0F}, {123.0F}}};

static float64 slabw1f64[1][1][3] = {{{110.0, 111.0, 112.0}}};
static float64 slabw2f64[2][1][3] = {{{20.0, 21.0, 22.0}}, {{120.0, 121.0, 122.0}}};
static float64 slabw3f64[1][2][3] = {{{0.0, 1.0, 2.0}, {10.0, 11.0, 12.0}}};
static float64 slabw4f64[1][1][3] = {{{100.0, 101.0, 102.0}}};
static float64 slabw5f64[2][3][1] = {{{3.0}, {13.0}, {23.0}}, {{103.0}, {113.0}, {123.0}}};

static int32 slabw1in[1][1][3] = {{{110, 111, 112}}};
static int32 slabw2in[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static int32 slabw3in[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static int32 slabw4in[1][1][3] = {{{100, 101, 102}}};
static int32 slabw5in[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static uint32 slabw1uin[1][1][3] = {{{110, 111, 112}}};
static uint32 slabw2uin[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static uint32 slabw3uin[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static uint32 slabw4uin[1][1][3] = {{{100, 101, 102}}};
static uint32 slabw5uin[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static int32 slabw1i32[1][1][3] = {{{110, 111, 112}}};
static int32 slabw2i32[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static int32 slabw3i32[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static int32 slabw4i32[1][1][3] = {{{100, 101, 102}}};
static int32 slabw5i32[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static uint32 slabw1ui32[1][1][3] = {{{110, 111, 112}}};
static uint32 slabw2ui32[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static uint32 slabw3ui32[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static uint32 slabw4ui32[1][1][3] = {{{100, 101, 102}}};
static uint32 slabw5ui32[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static int16 slabw1i16[1][1][3] = {{{110, 111, 112}}};
static int16 slabw2i16[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static int16 slabw3i16[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static int16 slabw4i16[1][1][3] = {{{100, 101, 102}}};
static int16 slabw5i16[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static uint16 slabw1ui16[1][1][3] = {{{110, 111, 112}}};
static uint16 slabw2ui16[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static uint16 slabw3ui16[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static uint16 slabw4ui16[1][1][3] = {{{100, 101, 102}}};
static uint16 slabw5ui16[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static int8 slabw1i8[1][1][3] = {{{110, 111, 112}}};
static int8 slabw2i8[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static int8 slabw3i8[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static int8 slabw4i8[1][1][3] = {{{100, 101, 102}}};
static int8 slabw5i8[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};

static uint8 slabw1ui8[1][1][3] = {{{110, 111, 112}}};
static uint8 slabw2ui8[2][1][3] = {{{20, 21, 22}}, {{120, 121, 122}}};
static uint8 slabw3ui8[1][2][3] = {{{0, 1, 2}, {10, 11, 12}}};
static uint8 slabw4ui8[1][1][3] = {{{100, 101, 102}}};
static uint8 slabw5ui8[2][3][1] = {{{3}, {13}, {23}}, {{103}, {113}, {123}}};
/* Slabs for slab3w() */
static float32 slab1[1][1][1]  = {{{0.0F}}};
static float32 slab2[1][1][1]  = {{{1.0F}}};
static float32 slab3[1][1][1]  = {{{2.0F}}};
static float32 slab4[1][1][1]  = {{{3.0F}}};
static float32 slab5[1][1][1]  = {{{10.0F}}};
static float32 slab6[1][1][1]  = {{{11.0F}}};
static float32 slab7[1][1][1]  = {{{12.0F}}};
static float32 slab8[1][1][1]  = {{{13.0F}}};
static float32 slab9[1][1][1]  = {{{20.0F}}};
static float32 slab10[1][1][1] = {{{21.0F}}};
static float32 slab11[1][1][1] = {{{22.0F}}};
static float32 slab12[1][1][1] = {{{23.0F}}};
static float32 slab13[1][1][1] = {{{100.0F}}};
static float32 slab14[1][1][1] = {{{101.0F}}};
static float32 slab15[1][1][1] = {{{102.0F}}};
static float32 slab16[1][1][1] = {{{103.0F}}};
static float32 slab17[1][1][1] = {{{110.0F}}};
static float32 slab18[1][1][1] = {{{111.0F}}};
static float32 slab19[1][1][1] = {{{112.0F}}};
static float32 slab20[1][1][1] = {{{113.0F}}};
static float32 slab21[1][1][1] = {{{120.0F}}};
static float32 slab22[1][1][1] = {{{121.0F}}};
static float32 slab23[1][1][1] = {{{122.0F}}};
static float32 slab24[1][1][1] = {{{123.0F}}};

/* data array in memory  */
static float32 fdata[2][3][4] = {
    {{0.0F, 1.0F, 2.0F, 3.0F}, {10.0F, 11.0F, 12.0F, 13.0F}, {20.0F, 21.0F, 22.0F, 23.0F}},
    {{100.0F, 101.0F, 102.0F, 103.0F}, {110.0F, 111.0F, 112.0F, 113.0F}, {120.0F, 121.0F, 122.0F, 123.0F}}};
static float64 f64data[2][3][4] = {
    {{0.0, 1.0, 2.0, 3.0}, {10.0, 11.0, 12.0, 13.0}, {20.0, 21.0, 22.0, 23.0}},
    {{100.0, 101.0, 102.0, 103.0}, {110.0, 111.0, 112.0, 113.0}, {120.0, 121.0, 122.0, 123.0}}};
static int32  indata[2][3][4]   = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static uint32 uindata[2][3][4]  = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                  {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static int32  i32data[2][3][4]  = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                 {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static uint32 ui32data[2][3][4] = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                   {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static int16  i16data[2][3][4]  = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                 {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static uint16 ui16data[2][3][4] = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                   {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static int8   i8data[2][3][4]   = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                               {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
static uint8  ui8data[2][3][4]  = {{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}},
                                 {{100, 101, 102, 103}, {110, 111, 112, 113}, {120, 121, 122, 123}}};
/* Output files */
static const char *swf32  = "swf32.hdf";
static const char *swf64  = "swf64.hdf";
static const char *swin   = "swin.hdf";
static const char *swuin  = "swuin.hdf";
static const char *swi32  = "swi32.hdf";
static const char *swui32 = "swui32.hdf";
static const char *swi16  = "swi16.hdf";
static const char *swui16 = "swui16.hdf";
static const char *swi8   = "swi8.hdf";
static const char *swui8  = "swui8.hdf";
static const char *sw1    = "s1w.hdf";
static const char *sw3    = "s3w.hdf";
static const char *sw4    = "s4w.hdf";

static int slabwf32(void);
static int slabwf64(void);
static int slabwin(void);
static int slabwuin(void);
static int slabwi32(void);
static int slabwui32(void);
static int slabwi16(void);
static int slabwui16(void);
static int slabwi8(void);
static int slabwui8(void);
static int slab1w(void);
static int slab2w(void);
static int slab3w(void);
static int slab4w(void);

/*
 ** Write data set to slabw.hdf as 5 hyperslabs.
 */
static int
slabwf32(void)
{
    int32   i, j, k;
    int32   ret     = 0;
    int32   num_err = 0;
    float32 sdata[2][3][4]; /* Data array read from from file */
    float32 lfill = 0.0F;

    MESSAGE(10, printf("\n slabwf32:  Writing 5 slabs to slabwf32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxf32, (void *)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillf32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swf32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swf32, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (!H4_FLT_ABS_EQUAL(lfill, fillf32))
        num_err++;
    MESSAGE(10, printf("\n       fill value =: %f \n", (double)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwf32:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (!H4_FLT_ABS_EQUAL(sdata[i][j][k], fdata[i][j][k]))
                    num_err++;
                MESSAGE(10, printf("%f, ", (double)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwf32 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwf32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write float64 data set to slabw.hdf as 5 hyperslabs.
 */
static int
slabwf64(void)
{
    int32   i, j, k;
    int32   ret     = 0;
    int32   num_err = 0;
    float64 sdata[2][3][4]; /* Data array read from from file */
    float64 lfill = 0.0;

    MESSAGE(10, printf("\n slabwf64:  Writing 5 slabs to slabwf64.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_FLOAT64);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxf64, (void *)&minf64);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillf64);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swf64);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swf64, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (!H4_DBL_ABS_EQUAL(lfill, fillf64))
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %f \n", (double)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwf64:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (!H4_DBL_ABS_EQUAL(sdata[i][j][k], f64data[i][j][k]))
                    num_err++;
                MESSAGE(10, printf("%f, ", (double)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwf64 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwf64:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write int32 data set as native data type to slabwin.hdf as 5 hyperslabs.
 */
static int
slabwin(void)
{
    int32 i, j, k;
    int32 ret     = 0;
    int32 num_err = 0;
    int32 sdata[2][3][4]; /* Data array read from from file */
    int32 lfill = 0;

    MESSAGE(10, printf("\n slabwin:  Writing 5 slabs to slabwin.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_NINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxin, (void *)&minin);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillin);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swin);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swin, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillin)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %d \n", (int)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwin:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != indata[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%u, ", (unsigned)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwin <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwin:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write uint32 data set as native data type to slabwuin.hdf as 5 hyperslabs.
 */
static int
slabwuin(void)
{
    int32  i, j, k;
    int32  ret     = 0;
    int32  num_err = 0;
    uint32 sdata[2][3][4]; /* Data array read from from file */
    uint32 lfill = 0;

    MESSAGE(10, printf("\n slabwuin:  Writing 5 slabs to slabwuin.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_NUINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnuin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowuin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccoluin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxuin, (void *)&minuin);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&filluin);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swuin);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swuin, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filluin)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %u \n", (unsigned)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwin:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != uindata[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%u, ", (unsigned)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwuin <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwuin:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write int32 data set to slabwi32.hdf as 5 hyperslabs.
 */
static int
slabwi32(void)
{
    int32 i, j, k;
    int32 ret     = 0;
    int32 num_err = 0;
    int32 sdata[2][3][4]; /* Data array read from from file */
    int32 lfill = (int32)0;

    MESSAGE(10, printf("\n slabwi32:  Writing 5 slabs to slabwi32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplni32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowi32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccoli32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxi32, (void *)&mini32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&filli32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swi32, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli32)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %d \n", (int)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwi32:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != i32data[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%d, ", (int)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwi32 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwi32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write uint32 data set to slabwui32.hdf as 5 hyperslabs.
 */
static int
slabwui32(void)
{
    int32  i, j, k;
    int32  ret     = 0;
    int32  num_err = 0;
    uint32 sdata[2][3][4]; /* Data array read from from file */
    uint32 lfill = (uint32)0;

    MESSAGE(10, printf("\n slabwui32:  Writing 5 slabs to slabwui32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxui32, (void *)&minui32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillui32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swui32, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui32)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %u \n", (unsigned)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwui32:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != ui32data[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%u, ", (unsigned)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwui32 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwui32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write int16 data set to slabwi16.hdf as 5 hyperslabs.
 */
static int
slabwi16(void)
{
    int32 i, j, k;
    int32 ret     = 0;
    int32 num_err = 0;
    int16 sdata[2][3][4]; /* Data array read from from file */
    int16 lfill = (int16)0;

    MESSAGE(10, printf("\n slabwi16:  Writing 5 slabs to slabwi16.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT16);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplni16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowi16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccoli16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxi16, (void *)&mini16);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&filli16);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi16);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swi16, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli16)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %d \n", lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwi16:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != i16data[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%d, ", sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwi16 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwi16:  %d wrong values in slab.  \n", (int)num_err);)

    return (int)num_err;
}

/*
 ** Write uint16 data set to slabwui16.hdf as 5 hyperslabs.
 */
static int
slabwui16(void)
{
    int32  i, j, k;
    int32  ret     = 0;
    int32  num_err = 0;
    uint16 sdata[2][3][4]; /* Data array read from from file */
    uint16 lfill = (uint16)0;

    MESSAGE(10, printf("\n slabwui16:  Writing 5 slabs to slabwui16.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT16);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxui16, (void *)&minui16);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillui16);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui16);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swui16, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui16)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %u \n", lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwui16:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != ui16data[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%u, ", sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwui16 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwui16:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write int8 data set to slabwi8.hdf as 5 hyperslabs.
 */
static int
slabwi8(void)
{
    int32 i, j, k;
    int32 ret     = 0;
    int32 num_err = 0;
    int8  sdata[2][3][4]; /* Data array read from from file */
    int8  lfill = (int8)0;

    MESSAGE(10, printf("\n slabwi8:  Writing 5 slabs to slabwi8.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT8);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplni8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowi8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccoli8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxi8, (void *)&mini8);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&filli8);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi8);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swi8, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli8)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %d \n", (int)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwi8:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != i8data[i][j][k])
                    num_err++;
                MESSAGE(10, printf("%d, ", (int)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwi8 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwi8:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Write uint8 data set to slabw.hdf as 5 hyperslabs.
 */
static int
slabwui8(void)
{
    int32 i, j, k;
    int32 ret     = 0;
    int32 num_err = 0;
    uint8 sdata[2][3][4]; /* Data array read from from file */
    uint8 lfill = (uint8)0;

    MESSAGE(10, printf("\n slabwui8:  Writing 5 slabs to slabwui8.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT8);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write data out using slabs with
     ** each slab in different order to the file "slab.hdf"
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxui8, (void *)&minui8);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillui8);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui8);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(swui8, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui8)
        num_err += 1;
    MESSAGE(10, printf("\n       fill value =: %u \n", (unsigned)lfill););

    if (num_err != 0)
        MESSAGE(10, printf("\n      slabwui8:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n  Verifying data  \n"););
    MESSAGE(10, printf("sdata = "););
    num_err = 0;
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (sdata[i][j][k] != ui8data[i][j][k])
                    num_err += 1;
                MESSAGE(10, printf("%u, ", (unsigned)sdata[i][j][k]););
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n       >>> All tests passed for slabwui8 <<< \n");)
    else
        MESSAGE(10, printf("\n       slabwui8:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

static int
slab1w(void)
{
    int32 ret     = 0;
    int32 num_err = 0;

    MESSAGE(10, printf("\n slab1w: Writing the first 3 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* Set fill value */
    ret = DFSDsetfillvalue((void *)&fillf32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");
    MESSAGE(10, printf("\n        slab1w: Setting fill value =%f \n", (double)fillf32););

    /*
     ** write each slab in different order
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxf32, (void *)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 2;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    if (num_err == 0)
        MESSAGE(10, printf("\n        >>> All tests passed for slab1w, now run slab2w <<< \n");)
    else
        MESSAGE(10, printf("\n         slab1w:  %d failures.  \n", (int)num_err););

    return (int)num_err;
}

static int
slab2w(void)
{
    int32   i, j, k;
    int32   ret     = 0;
    int32   num_err = 0;
    float32 sdata[2][3][4]; /* Data array read from from file */
    float32 lfill = 0.0F;
    int     trank;

    MESSAGE(10, printf("\n slab2w:  Writing the last 2 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* Get dimensions */
    ret = DFSDgetdims(sw1, &trank, size_dims, 3);
    CHECK(ret, FAIL, "DFSDgetdims");

    /* Get fill value */
    ret = DFSDgetfillvalue((void *)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");

    MESSAGE(10, printf("\n       fill value =: %f \n", (double)lfill););

    /* Call Writeref() first */
    ret = DFSDwriteref(sw1, 2);
    CHECK(ret, FAIL, "DFSDwriteref");

    /*
     ** write each slab in different order
     */
    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 3;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(sw1, start_dims, size_dims, stride, (void *)sdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    if (num_err != 0)
        MESSAGE(10, printf("\n        slab2w:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n         Verifying data \n"););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (!H4_FLT_ABS_EQUAL(sdata[i][j][k], fdata[i][j][k]))
                    num_err++;
            }

    if (num_err == 0)
        MESSAGE(10, printf("\n        >>> All tests passed for slab2w <<< \n");)
    else
        MESSAGE(10, printf("\n        slab2w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

static int
slab3w(void)
{
    int32   i, j, k;
    int32   ret     = 0;
    int32   num_err = 0;
    float32 adata[2][3][4]; /* Data array read from from file */

    MESSAGE(10, printf("\n slab3w: Writing all 24 elements of data as slabs to slab3w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
     ** write each element in different order
     */

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxf32, (void *)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw3);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab20);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab21);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 3;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab22);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 3;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab23);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 3;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab24);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab6);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab7);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab9);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab10);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab17);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab18);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 2;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab19);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab11);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 3;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab12);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab13);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab14);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2;
    start_dims[1] = 1;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab15);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 2;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 3;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 4;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1;
    start_dims[1] = 2;
    start_dims[2] = 1;
    size_dims[0]  = 1;
    size_dims[1]  = 1;
    size_dims[2]  = 1;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)slab5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(sw3, start_dims, size_dims, stride, (void *)adata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    if (num_err != 0)
        MESSAGE(10, printf("\n        slab3w:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n        Verifying data  \n"););
    MESSAGE(10, printf("adata = "););
    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (!H4_FLT_ABS_EQUAL(adata[i][j][k], fdata[i][j][k]))
                    num_err++;
                MESSAGE(10, printf("%f, ", (double)adata[i][j][k]););
            }

    if (num_err == 0)
        MESSAGE(10, printf("\n        >>> All tests passed for slab3w <<< \n");)
    else
        MESSAGE(10, printf("\n       slab3w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

static int
slab4w(void)
{
    int32   i, j, k;
    int32   ret     = 0;
    int32   num_err = 0;
    float32 bdata[2][3][4]; /* Data array read from from file */

    MESSAGE(10, printf("\n slab4w: Writing data as 1 slab to slab4w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (void *)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (void *)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (void *)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* Set max, min range */
    ret = DFSDsetrange((void *)&maxf32, (void *)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw4);
    CHECK(ret, FAIL, "DFSDstartslab");

    /* write out all the data to hdf file */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    ret           = DFSDwriteslab(start_dims, stride, size_dims, (void *)fdata);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1;
    start_dims[1] = 1;
    start_dims[2] = 1;
    size_dims[0]  = 2;
    size_dims[1]  = 3;
    size_dims[2]  = 4;
    d_dims[0]     = 2;
    d_dims[1]     = 3;
    d_dims[2]     = 4;
    ret           = DFSDreadslab(sw4, start_dims, size_dims, stride, (void *)bdata, d_dims);
    CHECK(ret, FAIL, "DFSDreadslab");

    if (num_err != 0)
        MESSAGE(10, printf("\n        slab4w:  %d failures.  \n", (int)num_err););

    MESSAGE(10, printf("\n          Verifying data  \n"););

    for (i = 0; i < d_dims[0]; i++)
        for (j = 0; j < d_dims[1]; j++)
            for (k = 0; k < d_dims[2]; k++) {
                if (!H4_FLT_ABS_EQUAL(bdata[i][j][k], fdata[i][j][k]))
                    num_err++;
            }
    if (num_err == 0)
        MESSAGE(10, printf("\n          >>> All tests passed for slab4w <<< \n");)
    else
        MESSAGE(10, printf("\n          slab4w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
 ** Main slab call to all other slab functions
 */
void
test_slab(void)
{
    num_errs += slabwf32();
    num_errs += slabwf64();
    num_errs += slabwin();
    num_errs += slabwuin();
    num_errs += slabwi32();
    num_errs += slabwui32();
    num_errs += slabwi16();
    num_errs += slabwui16();
    num_errs += slabwi8();
    num_errs += slabwui8();
    num_errs += slab1w();
    num_errs += slab2w();
    num_errs += slab3w();
    num_errs += slab4w();
}
