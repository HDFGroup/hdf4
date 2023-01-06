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

#include "mfhdf.h"
#include "hdftest.h"
#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

#define FILE_NAME8	"SDS_8_sziped.hdf"
#define FILE_NAME16	"SDS_16_sziped.hdf"
#define FILE_NAME32	"SDS_32_sziped.hdf"
#define FILE_NAMEfl32	"SDS_fl32_sziped.hdf"
#define FILE_NAMEfl64	"SDS_fl64_sziped.hdf"
#define SDS_NAME	"SzipedData"

#define RANK		2
#define WIDTH		6
#define LENGTH		9

#ifdef H4_HAVE_SZIP_ENCODER /* only compile all these test functions when 
				encoder is available */
static intn test_szip_SDS8bit()
{
   /************************* Variable declaration **************************/

   int32	sd_id, sds_id;
   intn 	status;
   int32	dim_sizes[2], array_rank, num_type, attributes;
   char 	name[H4_MAX_NC_NAME];
   comp_info 	c_info;
   int32        start[2], edges[2];
   int8         fill_value = 0; /* Fill value */
   int          i,j;
   int    	num_errs = 0;   /* number of errors so far */
   comp_coder_t comp_type;      /* to retrieve compression type into */
   comp_info    cinfo;          /* compression information structure */
   int8         out_data[LENGTH][WIDTH];
   int8         in_data[LENGTH][WIDTH]={
	   			 1,1,2,2,3,4,
	   			 1,1,2,2,3,4,
				 1,1,2,2,3,4,
				 3,3,0,4,3,4,
				 3,3,0,4,3,4,
				 3,3,0,4,3,4,
				 0,0,6,6,3,4,
				 5,5,6,6,3,4,
				 0,0,6,6,3,4};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAME8, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_INT8, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialization for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Set the compression */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
       CHECK(status, FAIL, "SDsetcompress");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
    * Verify the compressed data
    */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAME8, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Retrieve compression informayion about the dataset */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&c_info, 0, sizeof(c_info)) ;

    status = SDgetcompinfo(sds_id, &comp_type, &c_info);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcompinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
} /* test_szip_SDS8bit */

static intn test_szip_SDS16bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    int16       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    int16	out_data[LENGTH][WIDTH];
    int16	in_data[LENGTH][WIDTH]={
	   			 100,100,200,200,300,400,
	   			 100,100,200,200,300,400,
				 100,100,200,200,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				   0,  0,600,600,300,400,
				 500,500,600,600,300,400,
				   0,  0,600,600,300,400};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAME16, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_INT16, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialize for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Set the compression */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
       CHECK(status, FAIL, "SDsetcompress");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
    * Verify the compressed data
    */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAME16, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_szip_SDS16bit */

static intn test_szip_SDS32bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    int32       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    int32	out_data[LENGTH][WIDTH];
    int32	in_data[LENGTH][WIDTH]={
	   			 100,100,200,200,300,400,
	   			 100,100,200,200,300,400,
				 100,100,200,200,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				   0,  0,600,600,300,400,
				 500,500,600,600,300,400,
				   0,  0,600,600,300,400};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAME32, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialize for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Set the compression */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
    CHECK(status, FAIL, "SDsetcompress");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
    * Verify the compressed data
    */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAME32, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_szip_SDS32bit */

static intn test_szip_SDSfl32bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    float32     fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    float32	out_data[LENGTH][WIDTH];
    float32	in_data[LENGTH][WIDTH]={
	   			 100.0,100.0,200.0,200.0,300.0,400.0,
	   			 100.0,100.0,200.0,200.0,300.0,400.0,
				 100.0,100.0,200.0,200.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				   0.0,  0.0,600.0,600.0,300.0,400.0,
				 500.0,500.0,600.0,600.0,300.0,400.0,
				   0.0,  0.0,600.0,600.0,300.0,400.0};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAMEfl32, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_FLOAT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialize for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Set the compression */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
    CHECK(status, FAIL, "SDsetcompress");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
    * Verify the compressed data
    */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAMEfl32, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_szip_SDSfl32bit */

static intn test_szip_SDSfl64bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    float64     fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    float64	out_data[LENGTH][WIDTH];
    float64	in_data[LENGTH][WIDTH]={
	   			 100.0,100.0,200.0,200.0,300.0,400.0,
	   			 100.0,100.0,200.0,200.0,300.0,400.0,
				 100.0,100.0,200.0,200.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				 300.0,300.0,  0.0,400.0,300.0,400.0,
				   0.0,  0.0,600.0,600.0,300.0,400.0,
				 500.0,500.0,600.0,600.0,300.0,400.0,
				   0.0,  0.0,600.0,600.0,300.0,400.0};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAMEfl64, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_FLOAT64, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialization for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Set the compression */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
    CHECK(status, FAIL, "SDsetcompress");

    /* Write data to the SDS */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
     * Verify the compressed data
     */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAMEfl64, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_szip_SDSfl64bit */


#define FILE_NAME	"SDSchunkedsziped.hdf"
#define SDS_NAME_CH	"ChunkedData"
#define RANK_CH         2	/* rank of the chunked data set */
#define WIDTH_CH 	4	/* width of the chunked data set */
#define LENGTH_CH	9	/* length of the chunked data set */
#define CWIDTH		2	/* width of the chunk */
#define CLENGTH		3	/* length of the chunk */

static intn test_szip_chunk()
{
   /************************* Variable declaration **************************/

   int32         sd_id, sds_id, sds_index;
   intn          status;
   int32         flag, maxcache, new_maxcache;
   int32         dim_sizes[2], origin[2];
   HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
   HDF_CHUNK_DEF c_def_out; /* Chunking definitions */ 
   int32         c_flags, c_flags_out;
   int32         all_data[LENGTH_CH][WIDTH_CH];
   int32         start[2], edges[2];
   int32	 comp_size=0, uncomp_size=0;
   int32         chunk_out[CLENGTH][CWIDTH];
   int32         row[CWIDTH] = { 5, 5 };
   int32         column[CLENGTH] = { 4, 4, 4 };
   int32         fill_value = 0;   /* Fill value */
   comp_coder_t  comp_type;        /* to retrieve compression type into */
   comp_info     cinfo;            /* compression information structure */
   int    	 num_errs = 0;     /* number of errors so far */
   int           i,j;

   /*
   * Define all chunks.  Note that chunks 4 & 5 are not used to write,
   * only to verify the read data.  The 'row' and 'column' are used
   * to write in the place of these chunks.
   */
          int32 chunk1[CLENGTH][CWIDTH] = { 1, 1,
                                            1, 1,
                                            1, 1 }; 

          int32 chunk2[CLENGTH][CWIDTH] = { 2, 2,
                                            2, 2,
                                            2, 2 }; 

          int32 chunk3[CLENGTH][CWIDTH] = { 3, 3,
                                            3, 3,
                                            3, 3 }; 

          int32 chunk4[CLENGTH][CWIDTH] = { 0, 4,
                                            0, 4,
                                            0, 4 }; 

          int32 chunk5[CLENGTH][CWIDTH] = { 0, 0,
                                            5, 5,
                                            0, 0 }; 

          int32 chunk6[CLENGTH][CWIDTH] = { 6, 6,
                                            6, 6,
                                            6, 6 };


    /* Initialize chunk lengths. */
    c_def.comp.chunk_lengths[0] = CLENGTH;
    c_def.comp.chunk_lengths[1] = CWIDTH;

    /* Create the file and initialize SD interface. */
    sd_id = SDstart (FILE_NAME, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create LENGTH_CHxWIDTH_CH SDS. */
    dim_sizes[0] = LENGTH_CH;
    dim_sizes[1] = WIDTH_CH;
    sds_id = SDcreate (sd_id, SDS_NAME_CH,DFNT_INT32, RANK_CH, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for chunking/szip compression testing");

    /* Fill the SDS array with the fill value. */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Set parameters for Chunking/SZIP */
    c_def.comp.comp_type = COMP_CODE_SZIP;
    c_def.comp.cinfo.szip.pixels_per_block = 2;

    c_def.comp.cinfo.szip.options_mask = SZ_EC_OPTION_MASK;
    c_def.comp.cinfo.szip.options_mask |= SZ_MSB_OPTION_MASK;
    c_def.comp.cinfo.szip.bits_per_pixel = 0;
    c_def.comp.cinfo.szip.pixels = 0;
    c_def.comp.cinfo.szip.pixels_per_scanline = 0;
    c_flags = HDF_CHUNK | HDF_COMP;
    status = SDsetchunk (sds_id, c_def, c_flags);
       CHECK(status, FAIL, "SDsetchunk");

    /* Set chunk cache to hold maximum of 3 chunks. */
    maxcache = 3;
    flag = 0;
    new_maxcache = SDsetchunkcache (sds_id, maxcache, flag);
    CHECK(new_maxcache, FAIL, "SDsetchunkcache");

    HDmemset(&c_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(sds_id, &c_def_out, &c_flags_out);
    CHECK(status, FAIL, "SDgetchunkinfo");
    VERIFY(c_flags_out, c_flags, "SDgetchunkinfo");
    VERIFY(c_def_out.comp.comp_type, COMP_CODE_SZIP, "SDgetchunkinfo");

    /* 
     * Write chunks using SDwritechunk function.  Chunks can be written 
     * in any order. 
     */

    /* Write the chunk with the coordinates (0,0). */
    origin[0] = 0;
    origin[1] = 0;
    status = SDwritechunk (sds_id, origin, (VOIDP) chunk1);
    CHECK(status, FAIL, "SDwritechunk");

    /* Write the chunk with the coordinates (1,0). */
    origin[0] = 1;
    origin[1] = 0;
    status = SDwritechunk (sds_id, origin, (VOIDP) chunk3);
    CHECK(status, FAIL, "SDwritechunk");

    /* Write the chunk with the coordinates (0,1). */
    origin[0] = 0;
    origin[1] = 1;
    status = SDwritechunk (sds_id, origin, (VOIDP) chunk2);
    CHECK(status, FAIL, "SDwritechunk");

    /* Write chunk with the coordinates (1,2) using SDwritedata function. */
    start[0] = 6;
    start[1] = 2;
    edges[0] = 3;
    edges[1] = 2;
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP) chunk6); 
    CHECK(status, FAIL, "SDwritedata");

    /* Fill second column in the chunk with the coordinates (1,1) using 
     * SDwritedata function. */
    start[0] = 3;
    start[1] = 3;
    edges[0] = 3;
    edges[1] = 1;
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP) column); 
    CHECK(status, FAIL, "SDwritedata");

    /* Fill second row in the chunk with the coordinates (0,2) using 
     * SDwritedata function. */
    start[0] = 7;
    start[1] = 0;
    edges[0] = 1;
    edges[1] = 2;
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP) row); 
    CHECK(status, FAIL, "SDwritedata");
           
    /* Terminate access to the data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
     * Verify the compressed data
     */

    /* Reopen the file and access the first data set. */
    sd_id = SDstart (FILE_NAME, DFACC_READ);
    sds_index = 0;
    sds_id = SDselect (sd_id, sds_index);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for chunking/szip compression testing");

    /* Retrieve compression information about the dataset */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;

    status = SDgetcompinfo(sds_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcompinfo");

    /* Retrieve compression method alone from the dataset */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcomptype");

    /* Read the entire data set using SDreaddata function. */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH_CH;
    edges[1] = WIDTH_CH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)all_data);
    CHECK(status, FAIL, "SDreaddata");

    /* 
    * This is how the entire array should look like:
    *
    *          1 1 2 2
    *          1 1 2 2
    *          1 1 2 2
    *          3 3 0 4
    *          3 3 0 4
    *          3 3 0 4
    *          0 0 6 6
    *          5 5 6 6
    *          0 0 6 6
    */

    /* Read chunk #4 with the coordinates (1,1) and verify it. */
    origin[0] = 1;
    origin[1] = 1;    	
    status = SDreadchunk (sds_id, origin, chunk_out);
    CHECK(status, FAIL, "SDreadchunk");

    for (j=0; j<CLENGTH; j++) 
    {
	for (i=0; i<CWIDTH; i++) 
	{
	    if (chunk_out[j][i] != chunk4[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in chunk #4, want %ld got %ld\n", j, i, chunk4[j][i], chunk_out[j][i]);
		num_errs++;
	    }
	}
    }

    /* 
    * Read chunk #5 with the coordinates (2,0) and verify it.
    */
    origin[0] = 2;
    origin[1] = 0;    	
    status = SDreadchunk (sds_id, origin, chunk_out);
    CHECK(status, FAIL, "SDreadchunk");

    for (j=0; j<CLENGTH; j++) 
    {
	for (i=0; i<CWIDTH; i++) 
	    if (chunk_out[j][i] != chunk5[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in chunk #5, want %ld got %ld\n", j, i, chunk5[j][i], chunk_out[j][i]);
		num_errs++;
	    }
    }

    /* Get the data sizes */
    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "test_chkcmp_SDSs: SDgetdatasize");

    /* Terminate access to the data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_szip_chunk */ 


#define FILE_NAME_3D	"SDSchunkedsziped3d.hdf"
#define SDS_NAME_CH3D	"3D Chunked Szipped"
#define SDS_NAME_CH0	"3D Chunked Szipped Empty"
#define RANK_CH3        3	/* rank of the chunked data set */
#define SDS_DIM1 	4	/* first dimension of the chunked data set */
#define SDS_DIM0	9	/* second dimension of the chunked data set */
#define SDS_DIM2	11	/* third dimension of the chunked data set */
#define CHK_DIM0	9	/* first dimension of the chunk */
#define CHK_DIM1	2	/* second dimension of the chunk */
#define CHK_DIM2	2	/* third dimension of the chunk */

int16         all_data[SDS_DIM0][SDS_DIM1][SDS_DIM2];
int16         out_data[SDS_DIM0][SDS_DIM1][SDS_DIM2];

static intn test_szip_chunk_3d()
{
    /************************* Variable declaration **************************/

    int32         sd_id, sds_id0, sds_id, sds_index;
    intn          status;
    int32         dim_sizes[3];
    HDF_CHUNK_DEF c_def; /* Chunking definitions */ 
    HDF_CHUNK_DEF c_def_out; /* Chunking definitions */ 
    int32         c_flags, c_flags_out;
    int32         start[3], edges[3];
    int16         fill_value = 0;   /* Fill value */
    comp_coder_t  comp_type;        /* to retrieve compression type into */
    comp_info     cinfo;            /* compression information structure */
    int    	  num_errs = 0;     /* number of errors so far */
    int           i,j,k;

    for (i = 0; i < SDS_DIM0; i++) {
	for (j = 0; j < SDS_DIM1; j++) {
	    for (k = 0; k < SDS_DIM2; k++) {
		out_data[i][j][k] = i*100+j*10+k;
    }}}

    /* Initialize chunk lengths. */
    c_def.comp.chunk_lengths[0] = CHK_DIM0;
    c_def.comp.chunk_lengths[1] = CHK_DIM1;
    c_def.comp.chunk_lengths[2] = CHK_DIM2;

    /* Create the file and initialize SD interface. */
    sd_id = SDstart (FILE_NAME_3D, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create SDS_DIM0xSDS_DIM1 SDS. */
    dim_sizes[0] = SDS_DIM0;
    dim_sizes[1] = SDS_DIM1;
    dim_sizes[2] = SDS_DIM2;
    sds_id = SDcreate (sd_id, SDS_NAME_CH3D, DFNT_INT16, RANK_CH3, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for chunking/szip compression testing");

    /* Create a similar SDS and will make it chunked, but will not 
       write data to it */
    sds_id0 = SDcreate (sd_id, SDS_NAME_CH0, DFNT_INT16, RANK_CH3, dim_sizes);
    CHECK(sds_id0, FAIL, "SDcreate:Failed to create a data set for chunking/szip compression testing");

    /* Fill the SDS array with the fill value. */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Set parameters for Chunking/SZIP */
    c_def.comp.comp_type = COMP_CODE_SZIP;
    c_def.comp.cinfo.szip.pixels_per_block = 2;

    c_def.comp.cinfo.szip.options_mask = SZ_EC_OPTION_MASK;
    c_def.comp.cinfo.szip.options_mask |= SZ_MSB_OPTION_MASK;
    c_def.comp.cinfo.szip.bits_per_pixel = 2;
    c_def.comp.cinfo.szip.pixels = 16;
    c_def.comp.cinfo.szip.pixels_per_scanline = 2;
    c_flags = HDF_CHUNK | HDF_COMP;
    status = SDsetchunk (sds_id0, c_def, c_flags);
    status = SDsetchunk (sds_id, c_def, c_flags);
    CHECK(status, FAIL, "SDsetchunk");

    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    edges[0] = SDS_DIM0;
    edges[1] = SDS_DIM1;
    edges[2] = SDS_DIM2;
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP) out_data); 
    CHECK(status, FAIL, "SDwritedata");

    HDmemset(&c_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(sds_id0, &c_def_out, &c_flags_out);
    CHECK(status, FAIL, "SDgetchunkinfo");
    VERIFY(c_flags_out, c_flags, "SDgetchunkinfo");
    VERIFY(c_def_out.comp.comp_type, COMP_CODE_SZIP, "SDgetchunkinfo");

    HDmemset(&c_def_out, 0, sizeof(HDF_CHUNK_DEF));
    c_flags_out = 0;
    status = SDgetchunkinfo(sds_id, &c_def_out, &c_flags_out);
    CHECK(status, FAIL, "SDgetchunkinfo");
    VERIFY(c_flags_out, c_flags, "SDgetchunkinfo");
    VERIFY(c_def_out.comp.comp_type, COMP_CODE_SZIP, "SDgetchunkinfo");

    /* Terminate access to the data sets. */
    status = SDendaccess (sds_id0);
    CHECK(status, FAIL, "SDendaccess");

    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
     * Verify the compressed data
     */

    /* Reopen the file and access the first data set. */
    sd_id = SDstart (FILE_NAME_3D, DFACC_READ);
    sds_index = 0;
    sds_id = SDselect (sd_id, sds_index);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for chunking/szip compression testing");

    /* Retrieve compression information about the dataset */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;

    status = SDgetcompinfo(sds_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcompinfo");

    /* Retrieve compression method alone from the dataset */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcomptype");

    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    edges[0] = SDS_DIM0;
    edges[1] = SDS_DIM1;
    edges[2] = SDS_DIM2;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)all_data);
    CHECK(status, FAIL, "SDreaddata");

    for (i = 0; i < SDS_DIM0; i++) {
	for (j = 0; j < SDS_DIM1; j++) {
	    for (k = 0; k < SDS_DIM2; k++) {
	    if (out_data[i][j][k] != all_data[i][j][k])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d][%d] want %ld got %ld\n", i, j,k, out_data[i][j][k], all_data[i][j][k]);
		num_errs++;
	    }
	    }
	}
    }

    /* Terminate access to the data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_szip_chunk_3D */ 

/* 
 * At this time, the use of SZIP compression with unlimited dimension SDSs
 * is unavailable.  This test program is to verify that the feature is 
 * correctly disabled.
 */
#define FILE_NAME_UNLIM	"SDSunlimitedsziped.hdf"
#define SDS_NAME_UNLIM	"UnlimitedData"
static intn test_szip_unlimited()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    int32       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    int32	out_data[LENGTH][WIDTH];
    int32	in_data[LENGTH][WIDTH]={
	   			 100,100,200,200,300,400,
	   			 100,100,200,200,300,400,
				 100,100,200,200,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				 300,300,  0,400,300,400,
				   0,  0,600,600,300,400,
				 500,500,600,600,300,400,
				   0,  0,600,600,300,400};

    /********************* End of variable declaration ***********************/

    /* Create the file and initialize SD interface */
    sd_id = SDstart (FILE_NAME_UNLIM, DFACC_CREATE);
    CHECK(sd_id, FAIL, "SDstart");

    /* Create the SDS */
    dim_sizes[0] = SD_UNLIMITED;
    dim_sizes[1] = WIDTH;
    sds_id = SDcreate (sd_id, SDS_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate:Failed to create a data set for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	}
    edges[0] = LENGTH;
    edges[1] = WIDTH;

    /* Fill the SDS array with the fill value */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Initialize for SZIP */
    c_info.szip.pixels_per_block = 2;

    c_info.szip.options_mask = SZ_EC_OPTION_MASK;
    c_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 0;
    c_info.szip.pixels = 0;
    c_info.szip.pixels_per_scanline = 0;

    /* Attempting to set SZIP compression will fail because SZIP is 
     * not available with unlimited dimension yet */
    status = SDsetcompress (sds_id, COMP_CODE_SZIP, &c_info);
    VERIFY(status, FAIL, "SDsetcompress");

    /* Write data to the SDS; data will be uncompressed */
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file to 
       flush the compressed info to the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /*
    * Verify the written data
    */

    /* Reopen the file and select the first SDS */
    sd_id = SDstart (FILE_NAME_UNLIM, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for testing szip compression with unlimited dimension");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<LENGTH; j++) 
    {
        for (i=0; i<WIDTH; i++)
	    if (out_data[j][i] != in_data[j][i])
	    {
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_szip_unlimited */

#endif /* only compile all the above test functions when encoder is available */


/********************************************************************
   Name: test_getszipinfo() - verifies that some functions will not 
		fail even though SZIP library is not present or only 
		decoder is available.

   Description:
	In the past, some SD APIs will fail if szlib was not present.  These
	functions have been revised so that they are no longer depending on 
	SZIP library.  New APIs are designed to function correctly as well.
	The functions below are being tested here:
	- SDgetcomptype
	- SDgetdatasize

	This test function opens an existing file containing datasets with 
	SZIP compression and verifies that the above APIs will work with or 
	without SZIP library.

	The input file, sds_szipped.dat, is generated by the program
	mfhdf/libsrc/gen_sds_szipped.c

   Return value:
        The number of errors occurred in this routine.

   BMR - Oct 10, 2008
*********************************************************************/

#define SZIP_FILE	"sds_szipped.dat" /* generated by gen_sds_szipped.c */
#define SDS1_NAME	"TwoDimsInt32"
#define RANK		2
#define SZ_WIDTH	5
#define SZ_LENGTH	16

static intn test_getszipinfo()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id, sds_index;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    comp_coder_t comp_type;
    int32	comp_size, uncomp_size, orig_size;
    int16       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    char	testfile[512] = "";
    int32      *out_data = NULL;
    char *basename = "sds_szipped.dat";

    /********************* End of variable declaration ***********************/

    /* Make the name for the test file */
    make_datafilename(basename, testfile, sizeof(testfile));

    /* Open the file and select dataset SDS1_NAME */
    sd_id = SDstart (testfile, DFACC_RDONLY);
    CHECK(sd_id, FAIL, "SDstart");

    sds_index = SDnametoindex(sd_id, SDS1_NAME);
    CHECK(sds_index, FAIL, "SDnametoindex");

    sds_id = SDselect (sd_id, sds_index);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve and verify information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");
    VERIFY(array_rank, RANK, "SDgetinfo");
    VERIFY(dim_sizes[0], SZ_LENGTH, "SDgetinfo");
    VERIFY(dim_sizes[1], SZ_WIDTH, "SDgetinfo");
    VERIFY(num_type, DFNT_INT32, "SDgetinfo");
    VERIFY(attributes, 0, "SDgetinfo");

    /* Get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(sds_id, &comp_type);
    CHECK(status, FAIL, "SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SZIP, "SDgetcomptype");

    /* Get the compressed data size and non-compressed data size */
    status = SDgetdatasize(sds_id, &comp_size, &uncomp_size);
    CHECK(status, FAIL, "SDgetdatasize");

    /* Compute the uncompressed/original size of the data for comparision */
    orig_size = SZ_WIDTH * SZ_LENGTH * SIZE_INT32;
    VERIFY(uncomp_size, orig_size, "SDgetdatasize");

    /* In this test, compressed data size should be smaller than non-compressed
       data size */
    if (comp_size >= uncomp_size)
    {
        printf("*** Routine test_getszipinfo: FAILED at line %d ***\n", __LINE__);
        printf("    In this test, compressed data size (%d) should be smaller than non-compressed data size (%d)\n", comp_size, uncomp_size);
        num_errs++;
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_getszipinfo */


/********************************************************************
   Name: test_getszipdata() - verifies that SZIP compressed data can be read
		when either SZIP library encoder or only decoder is present

   Description:
	This test function opens the existing file "sds_szipped.dat" that 
	contains a dataset with SZIP compression and verifies that the SZIP 
	compressed data can be read with or without the encoder as long as
	the szlib is available.

	The input file, sds_szipped.dat, is generated by the program
	mfhdf/libsrc/gen_sds_szipped.c

   Return value:
        The number of errors occurred in this routine.

   BMR - Oct 10, 2008
*********************************************************************/

#ifdef H4_HAVE_LIBSZ	/* needed to read data, either decoder or encoder */
static intn test_getszipdata()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], array_rank, num_type, attributes;
    char	name[H4_MAX_NC_NAME];
    comp_info	c_info;
    int32       start[2], edges[2];
    int16       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_errs = 0;    /* number of errors so far */
    int32       out_data[SZ_LENGTH][SZ_WIDTH];
    char	testfile[512] = "";
    char       *basename = "sds_szipped.dat";

    /* data to compare against read data from sds_szipped.dat */
    int32	in_data[SZ_LENGTH][SZ_WIDTH]={
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

    /* Make the name for the test file */
    make_datafilename(basename, testfile, sizeof(testfile));

    /* Open the file */
    sd_id = SDstart (testfile, DFACC_READ);
    CHECK(sd_id, FAIL, "SDstart");

    /* Get the first SDS */
    sds_id = SDselect (sd_id, 0);
    CHECK(sds_id, FAIL, "SDselect:Failed to select a data set for szip compression testing");

    /* Retrieve information of the data set */
    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
    CHECK(status, FAIL, "SDgetinfo");

    /* Prepare for reading */
    start[0] = 0;
    start[1] = 0;
    edges[0] = dim_sizes[0];
    edges[1] = dim_sizes[1];

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the data set */
    status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "SDreaddata");

    /* Compare read data against input data */
    for (j=0; j<SZ_LENGTH; j++) 
    {
        for (i=0; i<SZ_WIDTH; i++)
	     if (out_data[j][i] != in_data[j][i])
	    {
		 fprintf(stderr,"This one: Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", j, i, (long)in_data[j][i], (long)out_data[j][i]);
		num_errs++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}  /* test_getszipdata */
#endif /* ifdef H4_HAVE_LIBSZ */

/* 
 * Test drive for testing the szip compression feature with SD interface 
 */
extern int 
test_szip_compression ()
{
    int num_errs = 0;

    /* Output message about test being performed */
    TESTING("szip compression for datasets (tszip.c)");

#ifdef H4_HAVE_SZIP_ENCODER
    /* Test creating and writing SZIP compressed data only when szlib encoder
	is available */
    num_errs = num_errs + test_szip_SDS8bit();
    num_errs = num_errs + test_szip_SDS16bit();
    num_errs = num_errs + test_szip_SDS32bit();
    num_errs = num_errs + test_szip_SDSfl32bit();
    num_errs = num_errs + test_szip_SDSfl64bit();
    num_errs = num_errs + test_szip_chunk();
    num_errs = num_errs + test_szip_unlimited();
    num_errs = num_errs + test_szip_chunk_3d();
#else
    /* printf("Test creating and writing SZIP compressed data \tSKIPPED\n"); */
#endif

#ifdef H4_HAVE_LIBSZ
    /* Test reading szipped data when szlib is available, either with encoder
	or only decoder */
    num_errs = num_errs + test_getszipdata();
#else
    /* printf("Test reading SZIP compressed data \tSKIPPED\n"); */
#endif

    /* Test reading szip compression information and should pass with or 
	without SZIP library */
    num_errs = num_errs + test_getszipinfo();

    if (num_errs == 0) PASSED();
    return num_errs;
}
