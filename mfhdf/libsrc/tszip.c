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

#include "mfhdf.h"
#include "hdftest.h"

#define FILE_NAME8	"SDS_8_sziped.hdf"
#define FILE_NAME16	"SDS_16_sziped.hdf"
#define FILE_NAME32	"SDS_32_sziped.hdf"
#define FILE_NAMEfl32	"SDS_fl32_sziped.hdf"
#define FILE_NAMEfl64	"SDS_fl64_sziped.hdf"
#define SDS_NAME	"SzipedData"

#define RANK		2
#define WIDTH		6
#define LENGTH		9

/* Initialize parameters for szip compression */
#define NN_OPTION_MASK 		32
#define LSB_OPTION_MASK		8
#define MSB_OPTION_MASK		16
#define RAW_OPTION_MASK		128

static intn 
test_szip_SDS8bit()
{
   /************************* Variable declaration **************************/

   int32	sd_id, sds_id;
   intn 	status;
   int32	dim_sizes[2], origin[2], array_rank, num_type, attributes;
   char 	name[MAX_NC_NAME];
   comp_info 	c_info;
   int32	pixels_per_scanline, comp_type;
   int32        start[2], edges[2];
   int8         fill_value = 0;   /* Fill value */
   int          i,j;
   int    	num_err = 0;    /* number of errors so far */
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
    CHECK(status, FAIL, "SDstart");

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
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 8;

    /* Set the compression */
    status = SDsetcompress (sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "SDsetfillvalue");

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
    CHECK(status, FAIL, "SDstart");

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
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", i, (long)in_data[j][i], (long)out_data[j][i]);
		num_err++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);

    return status;
} /* test_szip_SDS8bit */

static intn 
test_szip_SDS16bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], origin[2], array_rank, num_type, attributes;
    char	name[MAX_NC_NAME];
    comp_info	c_info;
    int32	pixels_per_scanline, comp_type;
    int32       start[2], edges[2];
    int16       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_err = 0;    /* number of errors so far */
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
    CHECK(status, FAIL, "SDstart");

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
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 16;

    /* Set the compression */
    status = SDsetcompress (sds_id, comp_type, &c_info);
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
    CHECK(status, FAIL, "SDstart");

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
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", i, (long)in_data[j][i], (long)out_data[j][i]);
		num_err++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);

    return status;
}  /* test_szip_SDS16bit */

static intn 
test_szip_SDS32bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], origin[2], array_rank, num_type, attributes;
    char	name[MAX_NC_NAME];
    comp_info	c_info;
    int32	pixels_per_scanline, comp_type;
    int32       start[2], edges[2];
    int32       fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_err = 0;    /* number of errors so far */
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
    CHECK(status, FAIL, "SDstart");

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
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 32;

    /* Set the compression */
    status = SDsetcompress (sds_id, comp_type, &c_info);
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
    CHECK(status, FAIL, "SDstart");

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
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", i, (long)in_data[j][i], (long)out_data[j][i]);
		num_err++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);

    return status;
}  /* test_szip_SDS32bit */

static intn 
test_szip_SDSfl32bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], origin[2], array_rank, num_type, attributes;
    char	name[MAX_NC_NAME];
    comp_info	c_info;
    int32	pixels_per_scanline, comp_type;
    int32       start[2], edges[2];
    float32     fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_err = 0;    /* number of errors so far */
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
    CHECK(status, FAIL, "SDstart");

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
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 32;

    /* Set the compression */
    status = SDsetcompress (sds_id, comp_type, &c_info);
    CHECK(status, FAIL, "SDsetfillvalue");

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
    CHECK(status, FAIL, "SDstart");

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
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", i, (long)in_data[j][i], (long)out_data[j][i]);
		num_err++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);

    return status;
}  /* test_szip_SDSfl32bit */

static intn 
test_szip_SDSfl64bit()
{
    /************************* Variable declaration **************************/

    int32	sd_id, sds_id;
    intn 	status;
    int32	dim_sizes[2], origin[2], array_rank, num_type, attributes;
    char	name[MAX_NC_NAME];
    comp_info	c_info;
    int32	pixels_per_scanline, comp_type;
    int32       start[2], edges[2];
    float64     fill_value = 0;   /* Fill value */
    int         i,j;
    int		num_err = 0;    /* number of errors so far */
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
    CHECK(status, FAIL, "SDstart");

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
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[1];
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
        c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 64;

    /* Set the compression */
    status = SDsetcompress (sds_id, comp_type, &c_info);
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
    CHECK(status, FAIL, "SDstart");

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
		fprintf(stderr,"Bogus val in loc [%d][%d] in compressed dset, want %ld got %ld\n", i, (long)in_data[j][i], (long)out_data[j][i]);
		num_err++;
	    }
    }

    /* Terminate access to the data set */
    status = SDendaccess (sds_id);

    /* Terminate access to the SD interface and close the file */
    status = SDend (sd_id);

    return status;
}  /* test_szip_SDSfl64bit */

/* 
 * Test drive for testing the szip compression feature with SD interface 
 */
extern int 
test_szip_compression ()
{
    int status = 0;
    status = test_szip_SDS8bit();
    status = test_szip_SDS16bit();
    status = test_szip_SDS32bit();
    status = test_szip_SDSfl32bit();
    status = test_szip_SDSfl64bit();
    return status;
}
