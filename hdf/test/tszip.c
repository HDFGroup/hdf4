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

#include <hdf.h>
#include "tutils.h"

#define  FILE_NAME8     "RI_8_sziped.hdf"
#define  FILE_NAME16    "RI_16_sziped.hdf"
#define  FILE_NAME32    "RI_32_sziped.hdf"
#define  FILE_NAMEfl32  "RI_fl32_sziped.hdf"
#define  FILE_NAMEfl64  "RI_fl64_sziped.hdf"
#define  WIDTH 		10    /* number of columns in the image */
#define  LENGTH		6     /* number of rows in the image */
#define  N_COMPS	3     /* number of components in the image */
#define  IMAGE_NAME 	"Sziped_Image"

/* Initialize parameters for szip compression */
#define NN_OPTION_MASK          32
#define LSB_OPTION_MASK         8
#define MSB_OPTION_MASK         16
#define RAW_OPTION_MASK 	128

/* 
 * Sub-tests for test_mgr_szip():
 *  test_szip_RI8bit()
 *  test_szip_RI16bit()
 *  test_szip_RI32bit()
 *  test_szip_RIfl32bit()
 *  test_szip_RIfl64bit()
 */

/* 
 * Write/Read szip compressed image with 8-bit integer data
 */
static void 
test_szip_RI8bit()
{
   /************************* Variable declaration **************************/

    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,       	  /* raster image identifier */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          index,
	  pixels_per_scanline;
    int32 start[2],
          edges[2];
    comp_info cinfo;    /* Compression parameters - union */
 
    comp_coder_t comp_type;
    int8 out_data[LENGTH][WIDTH][N_COMPS];
    int8 in_data[LENGTH][WIDTH][N_COMPS]    = {
		10, 11, 12, 13, 14, 15, 40, 41, 42, 43, 44, 45,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
		 0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 50, 51, 52, 
		53, 54, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0, 30, 31, 32, 33, 34, 
		35, 60, 61, 62, 63, 64, 65,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70, 71, 72, 
		73, 74, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0, 80, 81, 82, 83, 84, 85,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0, 90, 91, 92, 93, 94, 95};


    /********************** End of variable declaration **********************/

    /* Create and open the file for sziped data */
    file_id = Hopen (FILE_NAME8, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize the GR interface */
    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_INT8;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = WIDTH;
    dim_sizes[1] = LENGTH;

    /* Create the raster image array */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate:Failed to create a raster image for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;

    /* Initializate for SZIP */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[0]*N_COMPS;
    cinfo.szip.pixels = dim_sizes[0]*dim_sizes[1]*N_COMPS;
    cinfo.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
          cinfo.szip.pixels_per_scanline = 512;
    else
          cinfo.szip.pixels_per_scanline = pixels_per_scanline;

    cinfo.szip.options_mask = NN_OPTION_MASK;
    cinfo.szip.options_mask |= MSB_OPTION_MASK;
    cinfo.szip.options_mask |= RAW_OPTION_MASK;
    cinfo.szip.bits_per_pixel = 8;

    /* Set the compression */
    status = GRsetcompress(ri_id, comp_type, &cinfo);
    CHECK(status, FAIL, "GRsetcompress");

    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "GRwriteimage");

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the file to
       flush the compressed info to the file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

    /*
     * Verify the compressed data
     */

    /* Reopen the file */

    file_id = Hopen (FILE_NAME8, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Find the index of the specified image */
    index = GRnametoindex(gr_id, IMAGE_NAME);
    CHECK(index, FAIL, "GRnametoindex");
   
    /* Select the image */
    ri_id = GRselect(gr_id, index);
    CHECK(ri_id, FAIL, "GRselect");

    /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo,  0, sizeof(cinfo)) ;

    status = GRgetcompress(ri_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompress");
    VERIFY(comp_type, COMP_CODE_SZIP, "GRgetcompress");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the whole image */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "GRreadimage");

    /* Compare read data against input data */
    if (0!= HDmemcmp(out_data, in_data, sizeof(in_data)))
        printf("Error in reading the whole image \n" );

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the HDF file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

}  /* end of test_szip_RI8bit */

/* 
 * Write/Read szip compressed image with 16-bit integer data
 */
static void 
test_szip_RI16bit()
{
   /************************* Variable declaration **************************/

    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,       	  /* raster image identifier */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          index,
	  pixels_per_scanline;
    int32 start[2],
          edges[2];
    comp_info cinfo;    /* Compression parameters - union */

    comp_coder_t comp_type;
    int16 out_data[LENGTH][WIDTH][N_COMPS];
    int16 in_data[LENGTH][WIDTH][N_COMPS]    = {
		10, 11, 12, 13, 14, 15, 40, 41, 42, 43, 44, 45,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
		 0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 50, 51, 52, 
		53, 54, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0, 30, 31, 32, 33, 34, 
		35, 60, 61, 62, 63, 64, 65,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70, 71, 72, 
		73, 74, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0, 80, 81, 82, 83, 84, 85,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0, 90, 91, 92, 93, 94, 95};


    /********************** End of variable declaration **********************/

    /* Create and open the file for sziped data */
    file_id = Hopen (FILE_NAME16, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize the GR interface */
    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_INT16;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = WIDTH;
    dim_sizes[1] = LENGTH;

    /* Create the raster image array */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate:Failed to create a raster image for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;

    /* Initializate for SZIP */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[0]*N_COMPS;
    cinfo.szip.pixels = dim_sizes[0]*dim_sizes[1]*N_COMPS;
    cinfo.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
         cinfo.szip.pixels_per_scanline = 512;
    else
         cinfo.szip.pixels_per_scanline = pixels_per_scanline;

    cinfo.szip.options_mask = NN_OPTION_MASK;
    cinfo.szip.options_mask |= MSB_OPTION_MASK;
    cinfo.szip.options_mask |= RAW_OPTION_MASK;
    cinfo.szip.bits_per_pixel = 16;
 
    /* Set the compression */
    status = GRsetcompress(ri_id, comp_type, &cinfo);
    CHECK(status, FAIL, "GRsetcompress");

    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

   /* Terminate access to the GR interface and close the file to
      flush the compressed info to the file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

    /*
     * Verify the compressed data
     */

    /* Reopen the file */
    file_id = Hopen (FILE_NAME16, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Find the index of the specified image */
    index = GRnametoindex(gr_id, IMAGE_NAME);
    CHECK(index, FAIL, "GRnametoindex");
   
    /* Select the image */
    ri_id = GRselect(gr_id, index);
    CHECK(ri_id, FAIL, "GRselect");

    /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo,  0, sizeof(cinfo)) ;

    status = GRgetcompress(ri_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompress");
    VERIFY(comp_type, COMP_CODE_SZIP, "GRgetcompress");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the whole image */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "GRreadimage");

    /* Compare read data against input data */
    if (0!= HDmemcmp(out_data, in_data, sizeof(in_data)))
        printf("Error in reading the whole image \n" );

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the HDF file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

}  /* end of test_szip_RI16bit */

/* 
 * Write/Read szip compressed image with 32-bit integer data
 */
static void 
test_szip_RI32bit()
{
   /************************* Variable declaration **************************/

    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,       	  /* raster image identifier */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          index,
	  pixels_per_scanline;
    int32 start[2],
          edges[2];
    comp_info cinfo;    /* Compression parameters - union */

    comp_coder_t comp_type;
    int32 out_data[LENGTH][WIDTH][N_COMPS];
    int32 in_data[LENGTH][WIDTH][N_COMPS]    = {
		10, 11, 12, 13, 14, 15, 40, 41, 42, 43, 44, 45,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
		 0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 50, 51, 52, 
		53, 54, 55,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0, 30, 31, 32, 33, 34, 
		35, 60, 61, 62, 63, 64, 65,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70, 71, 72, 
		73, 74, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0, 80, 81, 82, 83, 84, 85,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
		 0,  0,  0,  0,  0, 90, 91, 92, 93, 94, 95};


   /********************** End of variable declaration **********************/

    /* Create and open the file for sziped data */
    file_id = Hopen (FILE_NAME32, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize the GR interface */
    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_INT32;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = WIDTH;
    dim_sizes[1] = LENGTH;
 
    /* Create the raster image array */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate:Failed to create a raster image for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;

    /* Initializate for SZIP */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[0]*N_COMPS;
    cinfo.szip.pixels = dim_sizes[0]*dim_sizes[1]*N_COMPS;
    cinfo.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
         cinfo.szip.pixels_per_scanline = 512;
    else
         cinfo.szip.pixels_per_scanline = pixels_per_scanline;

    cinfo.szip.options_mask = NN_OPTION_MASK;
    cinfo.szip.options_mask |= MSB_OPTION_MASK;
    cinfo.szip.options_mask |= RAW_OPTION_MASK;
    cinfo.szip.bits_per_pixel = 32;

    /* Set the compression */
    status = GRsetcompress(ri_id, comp_type, &cinfo);
    CHECK(status, FAIL, "GRsetcompress");

    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "GRwriteimage");

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

   /* Terminate access to the GR interface and close the file to
      flush the compressed info to the file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

    /*
     * Verify the compressed data
     */

    /* Reopen the file */

    file_id = Hopen (FILE_NAME32, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Find the index of the specified image */
    index = GRnametoindex(gr_id, IMAGE_NAME);
    CHECK(index, FAIL, "GRnametoindex");
   
    /* Select the image */
    ri_id = GRselect(gr_id, index);
    CHECK(ri_id, FAIL, "GRselect");

    /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo,  0, sizeof(cinfo)) ;

    status = GRgetcompress(ri_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompress");
    VERIFY(comp_type, COMP_CODE_SZIP, "GRgetcompress");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the whole image */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "GRreadimage");

    /* Compare read data against input data */
    if (0!= HDmemcmp(out_data, in_data, sizeof(in_data)))
        printf("Error in reading the whole image \n" );

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the HDF file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

}  /* end of test_szip_RI32bit */

/* 
 * Write/Read szip compressed image with 32-bit floating point data
 */
static void 
test_szip_RIfl32bit()
{
   /************************* Variable declaration **************************/

    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,       	  /* raster image identifier */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          index,
	  pixels_per_scanline;
    int32 start[2],
          edges[2];
    comp_info cinfo;    /* Compression parameters - union */

    comp_coder_t comp_type;
    float32 out_data[LENGTH][WIDTH][N_COMPS];
    float32 in_data[LENGTH][WIDTH][N_COMPS]    = {
		10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,  0.0,
                 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 
		 0.0,  0.0,  0.0,  0.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 50.0, 51.0, 52.0, 
		53.0, 54.0, 55.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 30.0, 31.0, 32.0, 33.0, 34.0, 
		35.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 70.0, 71.0, 72.0, 
		73.0, 74.0, 75.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0};


   /********************** End of variable declaration **********************/

    /* Create and open the file for sziped data */
    file_id = Hopen (FILE_NAMEfl32, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize the GR interface */
    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_FLOAT32;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = WIDTH;
    dim_sizes[1] = LENGTH;

    /* Create the raster image array */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate:Failed to create a raster image for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;

    /* Initializate for SZIP */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[0]*N_COMPS;
    cinfo.szip.pixels = dim_sizes[0]*dim_sizes[1]*N_COMPS;
    cinfo.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
         cinfo.szip.pixels_per_scanline = 512;
    else
         cinfo.szip.pixels_per_scanline = pixels_per_scanline;

    cinfo.szip.options_mask = NN_OPTION_MASK;
    cinfo.szip.options_mask |= MSB_OPTION_MASK;
    cinfo.szip.options_mask |= RAW_OPTION_MASK;
    cinfo.szip.bits_per_pixel = 32;
 
    /* Set the compression */
    status = GRsetcompress(ri_id, comp_type, &cinfo);
    CHECK(status, FAIL, "GRsetcompress");

    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "GRwriteimage");

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

   /* Terminate access to the GR interface and close the file to
      flush the compressed info to the file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

    /*
     * Verify the compressed data
     */

    /* Reopen the file */
    file_id = Hopen (FILE_NAMEfl32, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Find the index of the specified image */
    index = GRnametoindex(gr_id, IMAGE_NAME);
    CHECK(index, FAIL, "GRnametoindex");
   
    /* Select the image */
    ri_id = GRselect(gr_id, index);
    CHECK(ri_id, FAIL, "GRselect");

    /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo,  0, sizeof(cinfo)) ;

    status = GRgetcompress(ri_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompress");
    VERIFY(comp_type, COMP_CODE_SZIP, "GRgetcompress");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the whole image */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "GRreadimage");

    /* Compare read data against input data */
    if (0!= HDmemcmp(out_data, in_data, sizeof(in_data)))
        printf("Error in reading the whole image \n" );

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the HDF file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

}  /* end of test_szip_RIfl32bit */

/* 
 * Write/Read szip compressed image with 64-bit floating point data
 */
static void 
test_szip_RIfl64bit()
{
   /************************* Variable declaration **************************/

    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,       	  /* raster image identifier */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          index,
	  pixels_per_scanline;
    int32 start[2],
          edges[2];
    comp_info cinfo;    /* Compression parameters - union */

    comp_coder_t comp_type;
    float64 out_data[LENGTH][WIDTH][N_COMPS];
    float64 in_data[LENGTH][WIDTH][N_COMPS]    = {
		10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0,  0.0,
                 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 
		 0.0,  0.0,  0.0,  0.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 50.0, 51.0, 52.0, 
		53.0, 54.0, 55.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 30.0, 31.0, 32.0, 33.0, 34.0, 
		35.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, 70.0, 71.0, 72.0, 
		73.0, 74.0, 75.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  
		 0.0,  0.0,  0.0,  0.0,  0.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0};


    /********************** End of variable declaration **********************/

    /* Create and open the file for sziped data */
    file_id = Hopen (FILE_NAMEfl64, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    /* Initialize the GR interface */
    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_FLOAT64;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = WIDTH;
    dim_sizes[1] = LENGTH;
 
    /* Create the raster image array */
    ri_id = GRcreate (gr_id, IMAGE_NAME, N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate:Failed to create a raster image for szip compression testing");

    /* Define the location, pattern, and size of the data set */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
 
    /* Initializate for SZIP */
    comp_type = COMP_CODE_SZIP;
    pixels_per_scanline = dim_sizes[0]*N_COMPS;
    cinfo.szip.pixels = dim_sizes[0]*dim_sizes[1]*N_COMPS;
    cinfo.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
          cinfo.szip.pixels_per_scanline = 512;
    else
          cinfo.szip.pixels_per_scanline = pixels_per_scanline;
 
    cinfo.szip.options_mask = NN_OPTION_MASK;
    cinfo.szip.options_mask |= MSB_OPTION_MASK;
    cinfo.szip.options_mask |= RAW_OPTION_MASK;
    cinfo.szip.bits_per_pixel = 64;

    /* Set the compression */
    status = GRsetcompress(ri_id, comp_type, &cinfo);
    CHECK(status, FAIL, "GRsetcompress");

    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)in_data);
    CHECK(status, FAIL, "GRwriteimage");

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the file to
      flush the compressed info to the file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

    /*
     * Verify the compressed data
     */

    /* Reopen the file */
    file_id = Hopen (FILE_NAMEfl64, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart (file_id);
    CHECK(gr_id, FAIL, "GRstart");

    /* Find the index of the specified image */
    index = GRnametoindex(gr_id, IMAGE_NAME);
    CHECK(index, FAIL, "GRnametoindex");
   
    /* Select the image */
    ri_id = GRselect(gr_id, index);
    CHECK(ri_id, FAIL, "GRselect");

    /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo,  0, sizeof(cinfo)) ;

    status = GRgetcompress(ri_id, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompress");
    VERIFY(comp_type, COMP_CODE_SZIP, "GRgetcompress");

    /* Wipe out the output buffer */
    HDmemset(&out_data, 0, sizeof(out_data));

    /* Read the whole image */
    start[0] = start[1] = 0;
    edges[0] = WIDTH;
    edges[1] = LENGTH;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)out_data);
    CHECK(status, FAIL, "GRreadimage");

    /* Compare read data against input data */
    if (0!= HDmemcmp(out_data, in_data, sizeof(in_data)))
        printf("Error in reading the whole image \n" );

    /* Terminate access to the raster image */
    status = GRendaccess (ri_id);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access to the GR interface and close the HDF file */
    status = GRend (gr_id);
    CHECK(status, FAIL, "GRend");
    status = Hclose (file_id);
    CHECK(status, FAIL, "Hclose");

}  /* end of test_szip_RIfl64bit */

/****************************************************************
 * **
 * **  test_mgr_szip(): SZIP Compression tests
 * **
 * **  IX. Compressed image tests
 * **      A. Read/Write szip compressed image with 8-bit integer data type
 * **      B. Read/Write szip compressed image with 16-bit integer data type
 * **      C. Read/Write szip compressed image with 32-bit integer data type
 * **      D. Read/Write szip compressed image with 32-bit floating point data type
 * **      E. Read/Write szip compressed image with 64-bit floating point data type
 * **
 * ****************************************************************/
extern void
test_mgr_szip()
{
    /* Output message about test being performed */
    MESSAGE(6, printf("Testing GR szip compression WRITE/READ\n"););

    test_szip_RI8bit();
    test_szip_RI16bit();
    test_szip_RI32bit();
    test_szip_RIfl32bit();
    test_szip_RIfl64bit();
} 

