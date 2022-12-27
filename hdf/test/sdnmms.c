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

#include "tproto.h"

static double  f64[10][10],  tf64[10][10];
static double  f64scale[10],  tf64scale[10];
static double f64max = (double) 40.0, f64min = (double) 0.0;
static double tf64max, tf64min;

static float  f32[10][10],  tf32[10][10];
static float  f32scale[10],  tf32scale[10];
static float f32max = (float) 40.0, f32min = (float) 0.0;
static float tf32max, tf32min;

static int8  i8[10][10],  ti8[10][10];
static int8  i8scale[10],  ti8scale[10];
static int8 i8max = 127, i8min = -128;
static int8 ti8max, ti8min;

static uint8  ui8[10][10],  tui8[10][10];
static uint8  ui8scale[10],  tui8scale[10];
static uint8 ui8max = 255, ui8min = 0;
static uint8 tui8max, tui8min;

static int16  i16[10][10],  ti16[10][10];
static int16  i16scale[10],  ti16scale[10];
static int16 i16max = 1200, i16min = -1200;
static int16 ti16max, ti16min;

static uint16  ui16[10][10],  tui16[10][10];
static uint16  ui16scale[10],  tui16scale[10];
static uint16 ui16max = 20000, ui16min = 0;
static uint16 tui16max, tui16min;

static int32  i32[10][10],  ti32[10][10];
static int32  i32scale[10],  ti32scale[10];
static int32 i32max = 99999999, i32min = -999999999;
static int32 ti32max, ti32min;

static uint32  ui32[10][10],  tui32[10][10];
static uint32  ui32scale[10],  tui32scale[10];
static uint32 ui32max = 999999999, ui32min = 0;
static uint32 tui32max, tui32min;

void
test_sdnmms(void)
{
    int         i, j, err, err1, err2, ret;
    intn        rank;
    int32       dims[2];

    rank = 2;
    dims[0] = 10;
    dims[1] = 10;

    MESSAGE(5, printf("Creating arrays...\n");
        );

    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
            {

                f64[i][j] = (double)((i * 40) + j);   /* range: 0 ~ 4-billion */

                f32[i][j] = (float)((i * 40) + j);   /* range: 0 ~ 4-billion */

                i8[i][j] = (int8) ((i * 10) + j);   /* range: 0 ~ 100 */
                ui8[i][j] = (uint8) ((i * 20) + j);     /* range: 0 ~ 200 */

                i16[i][j] = (int16) ((i * 3000) + j);   /* range: 0 ~ 30000 */
                ui16[i][j] = (uint16) ((i * 6000) + j);     /* range: 0 ~ 60000 */

                i32[i][j] = (int32) ((i * 20) + j);     /* range: 0 ~ 2-billion */
                ui32[i][j] = (uint32) ((i * 40) + j);   /* range: 0 ~ 4-billion */
            }

          f64scale[i] = (double)((i * 40) + j);   /* range: 0 ~ 4-billion */

          f32scale[i] = (float) ((i * 40) + j);   /* range: 0 ~ 4-billion */

          i8scale[i] = (int8) ((i * 10) + j);   /* range: 0 ~ 100 */
          ui8scale[i] = (uint8) ((i * 20) + j);     /* range: 0 ~ 200 */

          i16scale[i] = (int16) ((i * 3000) + j);   /* range: 0 ~ 30000 */
          ui16scale[i] = (uint16) ((i * 6000) + j);     /* range: 0 ~ 60000 */

          i32scale[i] = (int32) ((i * 20) + j);     /* range: 0 ~ 2-billion */
          ui32scale[i] = (uint32) ((i * 40) + j);   /* range: 0 ~ 4-billion */
      }

    ret = DFSDsetdims(rank, dims);
    RESULT("DFSDsetdims");

    MESSAGE(5, printf("Writing arrays to single file...\n");
        );

    ret = DFSDsetNT(DFNT_NFLOAT64);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) f64scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &f64max, (VOIDP) &f64min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) f64);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NFLOAT32);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) f32scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &f32max, (VOIDP) &f32min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) f32);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NINT8);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) i8scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &i8max, (VOIDP) &i8min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) i8);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NUINT8);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) ui8scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &ui8max, (VOIDP) &ui8min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) ui8);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NINT16);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) i16scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &i16max, (VOIDP) &i16min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) i16);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NUINT16);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) ui16scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &ui16max, (VOIDP) &ui16min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) ui16);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NINT32);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) i32scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &i32max, (VOIDP) &i32min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) i32);
    RESULT("DFSDadddata");

    ret = DFSDsetNT(DFNT_NUINT32);
    RESULT("DFSDsetNT");
    ret = DFSDsetdimscale(1, (int32) 10, (VOIDP) ui32scale);
    RESULT("DFSDsetdimscale");
    ret = DFSDsetrange((VOIDP) &ui32max, (VOIDP) &ui32min);
    RESULT("DFSDsetrange");
    ret = DFSDadddata("nntcheck.hdf", rank, dims, (VOIDP) ui32);
    RESULT("DFSDadddata");

    MESSAGE(5, printf("Reading arrays from single file...\n");
        );

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) tf64);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) tf64scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &tf64max, (VOIDP) &tf64min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) tf32);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) tf32scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &tf32max, (VOIDP) &tf32min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) ti8);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) ti8scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &ti8max, (VOIDP) &ti8min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) tui8);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) tui8scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &tui8max, (VOIDP) &tui8min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) ti16);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) ti16scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &ti16max, (VOIDP) &ti16min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) tui16);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) tui16scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &tui16max, (VOIDP) &tui16min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) ti32);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) ti32scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &ti32max, (VOIDP) &ti32min);
    RESULT("DFSDgetrange");

    ret = DFSDgetdata("nntcheck.hdf", rank, dims, (VOIDP) tui32);
    RESULT("DFSDgetdata");
    ret = DFSDgetdimscale(1, (int32) 10, (VOIDP) tui32scale);
    RESULT("DFSDgetdimscale");
    ret = DFSDgetrange((VOIDP) &tui32max, (VOIDP) &tui32min);
    RESULT("DFSDgetrange");

    MESSAGE(5, printf("Checking arrays from single file...\n\n");
        );

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (f64[i][j] != tf64[i][j])
                  err = 1;
          if (f64scale[i] != tf64scale[i])
              err2 = 1;
      }
    if ((f64max != tf64max) || (f64min != tf64min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for float64 array.\n");
            else
            printf("Test passed for float64 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for float64 scales.\n");
            else
            printf("Test passed for float64 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for float64 max/min.\n");
            else
            printf("Test passed for float64 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (f32[i][j] != tf32[i][j])
                  err = 1;
          if (f32scale[i] != tf32scale[i])
              err2 = 1;
      }
    if ((f32max != tf32max) || (f32min != tf32min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for float array.\n");
            else
            printf("Test passed for float array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for float scales.\n");
            else
            printf("Test passed for float scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for float max/min.\n");
            else
            printf("Test passed for float max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (i8[i][j] != ti8[i][j])
                  err = 1;
          if (i8scale[i] != ti8scale[i])
              err2 = 1;
      }
    if ((i8max != ti8max) || (i8min != ti8min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for int8 array.\n");
            else
            printf("Test passed for int8 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for int8 scales.\n");
            else
            printf("Test passed for int8 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for int8 max/min.\n");
            else
            printf("Test passed for int8 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (ui8[i][j] != tui8[i][j])
                  err = 1;
          if (ui8scale[i] != tui8scale[i])
              err2 = 1;
      }
    if ((ui8max != tui8max) || (ui8min != tui8min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for uint8 array.\n");
            else
            printf("Test passed for uint8 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for uint8 scales.\n");
            else
            printf("Test passed for uint8 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for uint8 max/min.\n");
            else
            printf("Test passed for uint8 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (i16[i][j] != ti16[i][j])
                  err = 1;
          if (i16scale[i] != ti16scale[i])
              err2 = 1;
      }
    if ((i16max != ti16max) || (i16min != ti16min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for int16 array.\n");
            else
            printf("Test passed for int16 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for int16 scales.\n");
            else
            printf("Test passed for int16 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for int16 max/min.\n");
            else
            printf("Test passed for int16 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (ui16[i][j] != tui16[i][j])
                  err = 1;
          if (ui16scale[i] != tui16scale[i])
              err2 = 1;
      }
    if ((ui16max != tui16max) || (ui16min != tui16min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for uint16 array.\n");
            else
            printf("Test passed for uint16 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for uint16 scales.\n");
            else
            printf("Test passed for uint16 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for uint16 max/min.\n");
            else
            printf("Test passed for uint16 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (i32[i][j] != ti32[i][j])
                  err = 1;
          if (i32scale[i] != ti32scale[i])
              err2 = 1;
      }
    if ((i32max != ti32max) || (i32min != ti32min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for int32 array.\n");
            else
            printf("Test passed for int32 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for int32 scales.\n");
            else
            printf("Test passed for int32 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for int32 max/min.\n");
            else
            printf("Test passed for int32 max/min.\n");) ;

    err = 0;
    err1 = 0;
    err2 = 0;
    for (i = 0; i < 10; i++)
      {
          for (j = 0; j < 10; j++)
              if (ui32[i][j] != tui32[i][j])
                  err = 1;
          if (ui32scale[i] != tui32scale[i])
              err2 = 1;
      }
    if ((ui32max != tui32max) || (ui32min != tui32min))
        err1 = 1;

    num_errs += err + err1 + err2;
    MESSAGE(5, if (err == 1)
            printf(">>> Test failed for uint32 array.\n");
            else
            printf("Test passed for uint32 array.\n");) ;
    MESSAGE(5, if (err2 == 1)
            printf(">>> Test failed for uint32 scales.\n");
            else
            printf("Test passed for uint32 scales.\n");) ;
    MESSAGE(5, if (err1 == 1)
            printf(">>> Test failed for uint32 max/min.\n");
            else
            printf("Test passed for uint32 max/min.\n");) ;

}
