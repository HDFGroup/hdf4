/***************************************************************************
*
* slab1w.c writes the first 3 of 5 slabs to slab1w.hdf 
* slab2w.c writes out the last 2 of 5 slabs to slab1w.hdf  
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
* Revision 1.1  1993/01/07 20:56:12  georgev
* Added Hyperslab C and Fortran test programs.
*
*
*-------------------------------------------------------------------------*/

#include "../include/hdf.h"

int32 ret; 
int32 sdg_rank=3; 
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


float32 maxi = 123.0;
float32 mini = -1.0;
float32 fill = 1.0;

int32 size_dims[3] = {2,3,4}; /* size of slab dims */
int32 start_dims[3];			/* starting dims  */
int32 stride[3] = { 0, 0, 0};

char *sn = "slab1w.hdf";

main()

{
  int32 i, j, k;
  int32 num_err;

    printf("   Writing the first 3 of 5 slabs to slab1w.hdf \n");

	/* First set dimensions */
    ret = DFSDsetdims((int16)sdg_rank, size_dims);
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

    /* Set fill value */
    ret = DFSDsetfillvalue(&fill);
    no_err += ret;
    printf("\n   Setting fill value =%f \n", fill);

	/* 
	** write each slab in different order 
	*/

	/* Set max, min range */
    ret = DFSDsetrange(&maxi, &mini);
    no_err += ret;

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(sn, start_dims, stride, size_dims, slab1);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(sn, start_dims, stride, size_dims, slab3);
    no_err += ret;

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(sn, start_dims, stride, size_dims, slab5);
    no_err += ret;

    if (no_err == 0)
       printf("\n   >>> All tests passed for slab1w, now run slab2w <<< \n");
    else
       printf("\n   slab1w:  %d failures.  \n", no_err);

}

