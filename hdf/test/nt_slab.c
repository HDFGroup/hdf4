/***************************************************************************
*
* Slab test routines slabw(0, slab1w(), slab2w(), slab3w(), slab4w()
*
***************************************************************************/

#ifdef RCSID
static char RcsId[] = "$Id$";
#endif
/*-------------------------------------------------------------------------
 *
 * $Header$
 *
 * $Log$
 * Revision 1.2  1993/02/25 22:36:17  georgev
 * Minor changes .
 *
 * Revision 1.1  1993/02/19  17:48:40  georgev
 * Added slab tests.
 *
 *
 *------------------------------------------------------------------------*/

#include "../include/hdf.h"
#include "tutils.h"

/* External variables */
extern int num_errs;
extern int Verbocity;

/* Internal variables */

static int32 rank = 3; 

static float32 maxi = 123.0;
static float32 mini = -1.0;
static float32 fill = 1.0;


static int32 size_dims[3]  = {2, 3, 4}; /* size of slab dims */
static int32 start_dims[3] = {1, 1, 1}; /* starting dims  */
static int32 stride[3]     = {0, 0, 0};

/* luf for planes, rows and cols  */
static char *lpln = "Time"; 	
static char *upln = "Second"; 
static char *fpln = "Int32";
static char *lrow = "Line";
static char *urow = "Inch";
static char *frow = "Int16";	
static char *lcol = "Column";
static char *ucol = "Cm";
static char *fcol = "Int32";

/* scales for planes, rows, and cols */
static float32 scpln[2] = { 0.0, 100.0};	
static float32	scrow[3] = {0.0, 10.0, 20.0};
static float32	sccol[4] = {0.0, 1.0, 2.0, 3.0};

/* Slabs for slabw(), slab1w(), slab2w() */
static float32 slabw1[1][1][3] = { { {110.0, 111.0, 112.0} } }; 
static float32 slabw2[2][1][3] = { { {20.0, 21.0, 22.0} },
                           { {120.0, 121.0, 122.0} } }; 
static float32 slabw3[1][2][3] = { { {0.0, 1.0, 2.0},
                             {10.0, 11.0, 12.0} } }; 
static float32 slabw4[1][1][3] = { { {100.0, 101.0, 102.0} } }; 
static float32 slabw5[2][3][1] = { { {3.0}, {13.0}, {23.0} }, 
                           { {103.0}, {113.0}, {123.0} } }; 

/* Slabs for slab3w() */
static float32 slab1[1][1][1] = { { {0.0} } }; 
static float32 slab2[1][1][1] = { { {1.0} } }; 
static float32 slab3[1][1][1] = { { {2.0} } }; 
static float32 slab4[1][1][1] = { { {3.0} } }; 
static float32 slab5[1][1][1] = { { {10.0} } }; 
static float32 slab6[1][1][1] = { { {11.0} } }; 
static float32 slab7[1][1][1] = { { {12.0} } }; 
static float32 slab8[1][1][1] = { { {13.0} } }; 
static float32 slab9[1][1][1] = { { {20.0} } }; 
static float32 slab10[1][1][1] = { { {21.0} } }; 
static float32 slab11[1][1][1] = { { {22.0} } }; 
static float32 slab12[1][1][1] = { { {23.0} } }; 
static float32 slab13[1][1][1] = { { {100.0} } }; 
static float32 slab14[1][1][1] = { { {101.0} } }; 
static float32 slab15[1][1][1] = { { {102.0} } }; 
static float32 slab16[1][1][1] = { { {103.0} } }; 
static float32 slab17[1][1][1] = { { {110.0} } }; 
static float32 slab18[1][1][1] = { { {111.0} } }; 
static float32 slab19[1][1][1] = { { {112.0} } }; 
static float32 slab20[1][1][1] = { { {113.0} } }; 
static float32 slab21[1][1][1] = { { {120.0} } }; 
static float32 slab22[1][1][1] = { { {121.0} } }; 
static float32 slab23[1][1][1] = { { {122.0} } }; 
static float32 slab24[1][1][1] = { { {123.0} } }; 

/* data array in memory  */
static float32 fdata[2][3][4] = 
		{{{   0.0,   1.0,   2.0,   3.0},  
		  {  10.0,  11.0,  12.0,  13.0},
		  {  20.0,  21.0,  22.0,  23.0}},
		 {{ 100.0, 101.0, 102.0, 103.0},
		  { 110.0, 111.0, 112.0, 113.0},
		  { 120.0, 121.0, 122.0, 123.0}}};

/* Output files */
static char *sw  = "slabw.hdf";
static char *sw1 = "slab1w.hdf";
static char *sw3 = "slab3w.hdf";
static char *sw4 = "slab4w.hdf";

/* 
** Write data set to slabw.hdf as 5 hyperslabs.
*/
int
slabw()
{
  int32 i, j, k;
  int32 ret = 0; 
  int32 no_err = 0;
  int32 num_err = 0;
  static float32 sdata[2][3][4]; /* Data array read from from file */

    MESSAGE(10,printf("\n slabw:  Writing 5 slabs to slabw.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], scpln);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], scrow);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], sccol);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue(&fill);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(sw);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sw, start_dims, size_dims, sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (no_err != 0)
        MESSAGE(10,printf("\n      slabw:  %d failures.  \n", no_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != fdata[i][j][k])
               num_err += 1;
            MESSAGE(10,printf("%f, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabw <<< \n");)
    else
       MESSAGE(10,printf("\n       slabw:  %d wrong values in slab.  \n", num_err););

    return num_err;
}

int
slab1w()
{
  int32 i, j, k;
  int32 ret = 0; 
  int32 no_err = 0;

    MESSAGE(10,printf("\n slab1w: Writing the first 3 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], scpln);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], scrow);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], sccol);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* Set fill value */
    ret = DFSDsetfillvalue(&fill);
    CHECK(ret, FAIL, "DFSDsetfillvalue");
    MESSAGE(10,printf("\n        slab1w: Setting fill value =%f \n", fill););

    /* 
    ** write each slab in different order 
    */

    /* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    if (no_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab1w, now run slab2w <<< \n");)
    else
      MESSAGE(10,printf("\n         slab1w:  %d failures.  \n", no_err););

    return no_err;
}

int
slab2w()
{
  int32 i, j, k;
  int32 ret = 0; 
  int32 no_err = 0;
  int32 num_err = 0;
  static float32 sdata[2][3][4]; /* Data array read from from file */
  int32 trank;

    MESSAGE(10,printf("\n slab2w:  Writing the last 2 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* Get dimensions */
    ret = DFSDgetdims(sw1, &trank, size_dims, 3);
    CHECK(ret, FAIL, "DFSDgetdims");

    /* Get fill value */
    ret = DFSDgetfillvalue((float32 *)&fill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");

    MESSAGE(10,printf("\n       fill value =: %f \n", fill););

    /* Call Writeref() first */
    ret = DFSDwriteref(sw1, 2);
    CHECK(ret, FAIL, "DFSDwriteref");

    /* 
    ** write each slab in different order 
    */
    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0]  = 2; size_dims[1]  = 3; size_dims[2]  = 4;
    stride[0]     = 2; stride[1]     = 3; stride[2]     = 4;
    ret = DFSDgetslice(sw1, start_dims, size_dims, sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (no_err != 0)
      MESSAGE(10,printf("\n        slab2w:  %d failures.  \n", no_err););

    MESSAGE(10,printf("\n         Verifying data \n"););
    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != fdata[i][j][k])
               num_err += 1;
          }

    if (num_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab2w <<< \n");)
    else
       MESSAGE(10,printf("\n        slab2w:  %d wrong values in slab.  \n", num_err););

    return num_err;
}

int
slab3w()

{
  int32 i, j, k;
  int32 ret = 0; 
  int32 no_err = 0;
  int32 num_err = 0;
  static float32 adata[2][3][4]; /* Data array read from from file */

    MESSAGE(10,printf("\n slab3w: Writing all 24 elements of data as slabs to slab3w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], scpln);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], scrow);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], sccol);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write each element in different order 
    */

    /* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw3);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab20);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab21);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab22);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab23);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab24);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab6);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab7);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab9);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab10);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab17);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab18);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab19);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab11);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab12);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab13);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab14);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab15);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sw3, start_dims, size_dims, adata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (no_err != 0)
      MESSAGE(10,printf("\n        slab3w:  %d failures.  \n", no_err););

    MESSAGE(10,printf("\n        Verifying data  \n"););
    MESSAGE(10,printf("adata = "););
    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (adata[i][j][k] != fdata[i][j][k])
               num_err += 1;
            MESSAGE(10,printf("%f, ",adata[i][j][k]););
          }

    if (num_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab3w <<< \n");)
    else
       MESSAGE(10,printf("\n       slab3w:  %d wrong values in slab.  \n", num_err););

    return num_err;
}

int
slab4w()
{
  int32 i, j, k;
  int32 ret = 0; 
  int32 no_err = 0;
  int32 num_err = 0;
  static float32 bdata[2][3][4]; /* Data array read from from file */

    MESSAGE(10,printf("\n slab4w: Writing data as 1 slab to slab4w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], scpln);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], scrow);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], sccol);
    CHECK(ret, FAIL, "DFSDsetdimscale");

	/* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw4);
    CHECK(ret, FAIL, "DFSDstartslab");

    /* write out all the data to hdf file */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    ret = DFSDwriteslab(start_dims, stride, size_dims, fdata);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sw4, start_dims, size_dims, bdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (no_err != 0)
        MESSAGE(10,printf("\n        slab4w:  %d failures.  \n", no_err););

   MESSAGE(10,printf("\n          Verifying data  \n"););

    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (bdata[i][j][k] != fdata[i][j][k])
               num_err += 1;
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n          >>> All tests passed for slab4w <<< \n");)
    else
       MESSAGE(10,printf("\n          slab4w:  %d wrong values in slab.  \n", num_err););

    return (num_err + no_err);
}

/*
** Main slab call to all other slab functions
*/
void
test_slab()
{
    num_errs += slabw();
    num_errs += slab1w();
    num_errs += slab2w();
    num_errs += slab3w();
    num_errs += slab4w(); 
}
