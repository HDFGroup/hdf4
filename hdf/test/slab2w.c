/************************************************************************
*
* slab2w.c writes the last 2 of 5 slabs to slab1w.hdf
* slab1w.c writes out the first 3 of 5 slabs to slab1w.hdf
*
*************************************************************************/

#ifdef RCSID
static char RcsId[] = "$Id$";
#endif
/*-------------------------------------------------------------------------
 *
 * $Header$
 *
 * $Log$
 * Revision 1.4  1993/02/01 23:23:17  georgev
 * Changed hyperslab test files to reflect new interface
 *
 * Revision 1.3  1993/01/19  05:58:23  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.2  1993/01/07  21:38:38  georgev
 * Minor cosmetic changes.
 *
 * Revision 1.1  1993/01/07  20:56:15  georgev
 * Added Hyperslab C and Fortran test programs.
 *
 *
 *------------------------------------------------------------------------*/

#include "../include/hdf.h"

int32 ret; 
int32 rank=3; 
int32 no_err=0;

int32 np=2;			/* dimension sizes  */
int32 nr=3;
int32 nc=4;

char *lpln = "Time", 	/* luf for planes, rows and cols  */
     *upln = "Second", 
     *fpln = "Int32",
     *lrow = "Line",
     *urow = "Inch",
     *frow = "Int16",	
     *lcol = "Column",
     *ucol = "Cm",
     *fcol = "Int32";

float32 scpln[2] = { 0.0, 100.0},	/* scales for planes, rows, and cols */
		scrow[3] = {0.0, 10.0, 20.0},
		sccol[4] = {0.0, 1.0, 2.0, 3.0};

float32 slab1[1][1][3] = { { {110.0, 111.0, 112.0} } }; 
float32 slab2[2][1][3] = { { {20.0, 21.0, 22.0} },
                           { {120.0, 121.0, 122.0} } }; 
float32 slab3[1][2][3] = { { {0.0, 1.0, 2.0},
                             {10.0, 11.0, 12.0} } }; 
float32 slab4[1][1][3] = { { {100.0, 101.0, 102.0} } }; 
float32 slab5[2][3][1] = { { {3.0}, {13.0}, {23.0} }, 
                           { {103.0}, {113.0}, {123.0} } }; 

float32 sdata[2][3][4];
float32 fdata[2][3][4] =
            {{{   0.0,   1.0,   2.0,   3.0},  /* data array in memory  */
              {  10.0,  11.0,  12.0,  13.0},
              {  20.0,  21.0,  22.0,  23.0}},
             {{ 100.0, 101.0, 102.0, 103.0},
              { 110.0, 111.0, 112.0, 113.0},
              { 120.0, 121.0, 122.0, 123.0}}};


float32 maxi = 123.0;
float32 mini = -1.0;

int32 size_dims[3] = {2,3,4}; /* size of slab dims */
int32 start_dims[3];			/* starting dims  */
int32 stride[3] = { 0, 0, 0};

char *sn = "slab1w.hdf";
float32 fill_value;

main()

{
  int32 i, j, k;
  int32 num_err;

    printf("   Writing the last 2 of 5 slabs to slab1w.hdf \n");

    /* Get dimensions */
    ret = DFSDgetdims(sn, &rank, size_dims, 3);
    no_err += ret;

    /* Get fill value */
    ret = DFSDgetfillvalue((float32 *)&fill_value);
    no_err += ret;

    printf("\n  fill value =: %f \n", fill_value);

    /* Call Writeref() first */
    DFSDwriteref(sn, 2);

    /* 
    ** write each slab in different order 
    */
    ret = DFSDstartslab(sn);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab2);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab4);
    no_err += ret;

    ret = DFSDendslab();
    no_err += ret;

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sn, start_dims, size_dims, sdata, stride);
    no_err += ret;

    if (no_err != 0)
      printf("\n   slab2w:  %d failures.  \n", no_err);

    printf("\n  Verifying data \n");
    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != fdata[i][j][k])
               num_err += 1;
          }

    if (num_err == 0)
       printf("\n   >>> All tests passed <<< \n");
    else
       printf("\n   slab2w:  %d wrong values in slab.  \n", num_err);

    exit(0);

}

