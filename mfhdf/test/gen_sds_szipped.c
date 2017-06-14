/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************
 * This program generates the data file "sds_szipped.dat" which contains
 * some szipped compressed SDSs. (currently only one dataset but more can be 
 * added for future tests.)  This data file is used by the following test
 * files:
 *	tcomp.c: to test getting compression information with only decoder
 *	tdatasizes.c: to test getting compressed data size with only decoder
 *
 * This program must be compiled with an h4cc version that was built with 
 * szip encoder enabled. (2008-10-09 - BMR)
 *
****************************************************************************/

#include "mfhdf.h"

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

#ifdef HDF

#include "hdftest.h"

#define FILE_NAME      "sds_szipped.dat"
#define SDS1_NAME      "TwoDimsInt32"
#define X_LENGTH       5
#define Y_LENGTH       16
#define RANK           2

/*
 * Generate szipped data to test reading szipped data and compression info.
 */
intn main()
{
    /************************* Variable declaration **************************/

    int32     sd_id, sds_id, sds_index;
    intn      status;
    int32     comp_type;    /* Compression flag */
    comp_info c_info;   /* Compression structure */
    int32     start[2], edges[2], dim_sizes[2];
    int32     pixels_per_scanline;
    intn      num_errs = 0;    /* number of errors in compression test so far */
    int       i, j;
    int32     data[Y_LENGTH][X_LENGTH]={
				100,100,200,200,300,
				0, 0, 0, 0, 0,
				100,100,200,200,300,
				400,300,200,100,0,
				300,300,  0,400,300,
				300,300,  0,400,300,
				300,300,  0,400,300,
				0,  0,600,600,300,
				500,500,600,600,300,
				0,  0,600,600,300,
				0,  0,600,600,300,
				0,  0,600,600,300,
				0,  0,600,600,300,
				500,500,600,600,300,
				500,500,600,600,300,
				500,500,600,600,300 };


    /********************* End of variable declaration ***********************/

    dim_sizes[0] = Y_LENGTH;
    dim_sizes[1] = X_LENGTH;

    /* Create the file and initialize the SD interface. */
    sd_id = SDstart (FILE_NAME, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Define the location and size of the data set to be written to the file. */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /* Create 3rd data set for SZIP compression. */
    sds_id = SDcreate (sd_id, SDS1_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set data set SDS1_NAME to use SZIP compression. */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
	c_info.szip.pixels_per_scanline = 512;
    else
	c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 64;

    status = SDsetcompress (sds_id, comp_type, &c_info); 
    CHECK(status, FAIL, "SDsetcompress");

    /* Write the stored data to the 3rd data set. */
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the 3rd data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* main */

#endif /* HDF */
