/***************************************************************************
*
* The program writes the 24 elements of the data set individually
* as slabs to slab3w.hdf
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
 * Revision 1.5  1993/03/24 22:32:02  briand
 * fixed path for hdf.h file in slab tests. Fixed function pointer in
 * testhdf
 *
 * Revision 1.4  1993/02/01  23:23:21  georgev
 * Changed hyperslab test files to reflect new interface
 *
 * Revision 1.3  1993/01/19  05:58:25  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.2  1993/01/07  21:38:40  georgev
 * Minor cosmetic changes.
 *
 * Revision 1.1  1993/01/07  20:56:18  georgev
 * Added Hyperslab C and Fortran test programs.
 *
 *
 *------------------------------------------------------------------------*/

#include "hdf.h"

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

float32 slab1[1][1][1] = { { {0.0} } }; 
float32 slab2[1][1][1] = { { {1.0} } }; 
float32 slab3[1][1][1] = { { {2.0} } }; 
float32 slab4[1][1][1] = { { {3.0} } }; 
float32 slab5[1][1][1] = { { {10.0} } }; 
float32 slab6[1][1][1] = { { {11.0} } }; 
float32 slab7[1][1][1] = { { {12.0} } }; 
float32 slab8[1][1][1] = { { {13.0} } }; 
float32 slab9[1][1][1] = { { {20.0} } }; 
float32 slab10[1][1][1] = { { {21.0} } }; 
float32 slab11[1][1][1] = { { {22.0} } }; 
float32 slab12[1][1][1] = { { {23.0} } }; 
float32 slab13[1][1][1] = { { {100.0} } }; 
float32 slab14[1][1][1] = { { {101.0} } }; 
float32 slab15[1][1][1] = { { {102.0} } }; 
float32 slab16[1][1][1] = { { {103.0} } }; 
float32 slab17[1][1][1] = { { {110.0} } }; 
float32 slab18[1][1][1] = { { {111.0} } }; 
float32 slab19[1][1][1] = { { {112.0} } }; 
float32 slab20[1][1][1] = { { {113.0} } }; 
float32 slab21[1][1][1] = { { {120.0} } }; 
float32 slab22[1][1][1] = { { {121.0} } }; 
float32 slab23[1][1][1] = { { {122.0} } }; 
float32 slab24[1][1][1] = { { {123.0} } }; 


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

char *fn = "slice.hdf";
char *sn = "slab3w.hdf";

main()

{
  int32 i, j, k;
  int32 num_err;

    printf("\n  Writing all 24 elements of data as slabs to slab3w.hdf \n");

	/* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    no_err += ret;

	/* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    no_err += ret;

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    no_err += ret;

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    no_err += ret;

	/* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], scpln);
    no_err += ret;

    ret = DFSDsetdimscale(2, size_dims[1], scrow);
    no_err += ret;

    ret = DFSDsetdimscale(3, size_dims[2], sccol);
    no_err += ret;

	/* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    no_err += ret;

	/* 
	** write each element in different order 
	*/

	/* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    no_err += ret;

    ret = DFSDstartslab(sn);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab20);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab21);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab22);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab23);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab24);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab6);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab7);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab8);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab9);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab10);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab16);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab17);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab18);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab19);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab11);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab12);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab13);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab14);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab15);
    no_err += ret;
    
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab1);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab2);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab3);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab4);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, slab5);
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
      printf("\n   slab3w:  %d failures.  \n", no_err);

    printf("\n  Verifying data  \n");
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
       printf("\n   slab3w:  %d wrong values in slab.  \n", num_err);


    exit(0);

}

