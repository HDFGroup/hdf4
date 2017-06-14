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

#include "hdf.h"
#include <stdio.h>
#include "tutils.h"

#define GZIPFILE    "gr_gzip.hdf"
#define JPEGFILE    "gr_jpeg.hdf"

/****************************************************************************
 * tmgrcomp.c - tests the compression functions for the multi-file GR interface
 * Structure of the file:
 *    test_mgr_compress - test driver
 *	  test_mgr_compress_a - tests with GZIP compressed image
 *	  test_mgr_compress_b - tests with 8-bit JPEG compressed image
 *	  test_mgr_compress_c - tests with 24-bit JPEG compressed image
 *	  test_get_compress - tests getting comp info with compressed image
 *	  test_mgr_chunk_compress - tests getting comp info with chunked
 *				and compressed image
 * Modification:
 *	Nov 23, 2009: Moved out from mgr.c. - BMR
*****************************************************************************/

/* Create/Write/Read GZIP compressed image */
static int test_mgr_compress_a()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int   num_errs = 0;    /* number of errors so far */

    MESSAGE(8, printf("Operate on gzip compressed images\n"););

    /* Open up the existing datafile and get the image information from it */
    fid = Hopen(GZIPFILE, DFACC_ALL, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Initialize the GR interface */
    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        comp_coder_t comp_type;            /* Compression method */
        comp_info cinfo;            /* Compression parameters */
        int32 dims[2]={10, 10};    /* dimensions for the empty image */
        uint8 image[10][10]; /* space for the image data */
        uint8 image0[10][10]; /* space for the image data */
        int32 start[2];     /* start of image data to grab */
        int32 stride[2];    /* stride of image data to grab */
        int32 count[2];     /* Size of image data to operate on */
        intn i, j;       /* indices */

        /* Initialize data we are going to write out */
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                image0[i][j] = (uint8) (i + j);

        /* Get the first image in this file */
        riid = GRcreate(grid, "image1", 1, DFNT_UINT8, MFGR_INTERLACE_PIXEL, dims);
        CHECK(riid, FAIL, "GRcreate");

        /* Set the compression method for the image */
        comp_type=COMP_CODE_DEFLATE;
        cinfo.deflate.level=7;
        ret = GRsetcompress(riid, comp_type, &cinfo);
        CHECK(ret, FAIL, "GRsetcompress");

        /* Write the whole image out */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRwriteimage(riid, start, stride, dims, image0);
        CHECK(ret, FAIL, "GRreadimage");

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Check that the image made it out correctly */

        /* Get the first image in this file */
        riid = GRselect(grid, 0);
        CHECK(riid, FAIL, "GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRreadimage(riid, start, stride, dims, image);
        CHECK(ret, FAIL, "GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image, image0, 10*10)!=0) {
            MESSAGE(3, printf("Error reading data for gzip compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Check for compressing image in the middle of writing data */

        /* Get the first image in this file */
        riid = GRcreate(grid, "image2", 1, DFNT_UINT8, MFGR_INTERLACE_PIXEL, dims);
        CHECK(riid, FAIL, "GRcreate");

        /* Write half of the image out */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        count[0]=10; count[1]=5;
        ret = GRwriteimage(riid, start, stride, count, image0);
        CHECK(ret, FAIL, "GRwriteimage");

        /* Set the compression method for the image */
        comp_type=COMP_CODE_DEFLATE;
        cinfo.deflate.level=7;
        ret = GRsetcompress(riid, comp_type, &cinfo);
        CHECK(ret, FAIL, "GRsetcompress");

        /* Write the second half of the image out */
        start[0]=0; start[1]=5;
        stride[0]=stride[1]=1;
        count[0]=10; count[1]=5;
        ret = GRwriteimage(riid, start, stride, count, &image0[5][0]);
        CHECK(ret, FAIL, "GRwriteimage");

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Check that the image made it out correctly */
        HDmemset(image, 0, 10*10);

        /* Get the second image in this file */
        riid = GRselect(grid, 1);
        CHECK(riid, FAIL, "GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRreadimage(riid, start, stride, dims, image);
        CHECK(ret, FAIL, "GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image, image0, 10*10)!=0) {
            MESSAGE(3, printf("Error reading 2nd data for gzip compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");
    }
    
    /* Shut down the GR interface */
    ret = GRend(grid);
    CHECK(ret, FAIL, "GRend");

    /* Close the file */
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
} /* end test_mgr_compress_a() */

/* Create/Write/Read 8-bit JPEG compressed image */
static int test_mgr_compress_b()
{
    int32 fid;              /* HDF file ID */
    int32 grid;             /* GRID for the interface */
    int32 ret;              /* generic return value */
    int   num_errs = 0;    /* number of errors so far */

    MESSAGE(8, printf("Operate on 8-bit JPEG compressed images\n"););

    /* Open up the existing datafile and get the image information from it */
    fid = Hopen(JPEGFILE, DFACC_ALL, 0);
    CHECK(fid, FAIL, "Hopen");

    /* Initialize the GR interface */
    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    {
        int32 riid;     /* RI ID for the new image */
        comp_coder_t comp_type;            /* Compression method */
        comp_info cinfo;            /* Compression parameters */
        int32 dims[2]={10, 10};    /* dimensions for the empty image */
        int32 dims_out[2];          /* Buffer for retrieving dimensions */
        uint8 image[10][10]; /* space for the image data */
        uint8 image0[10][10]; /* space for the image data */
        int32 start[2];     /* start of image data to grab */
        int32 stride[2];    /* stride of image data to grab */
        int32 n_images, n_file_attrs;   /* File information variables */
        int32 interlace_mode, n_comps, n_attrs, datatype; /* Image information */
        char name[30];      /* Buffer for retrieving image name */
        uint8 attr;         /* Attribute data */
#ifdef NOT_IMPLEMENTED
        int32 count[2];     /* Size of image data to operate on */
#endif /* NOT_IMPLEMENTED */
        intn i, j;       /* indices */

        /* Initialize data we are going to write out */
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                image0[i][j] = (uint8) (i + j);

        /* Get the first image in this file */
        riid = GRcreate(grid, "image1", 1, DFNT_UINT8, MFGR_INTERLACE_PIXEL, dims);
        CHECK(riid, FAIL, "GRcreate");

        attr = 100;
        ret = GRsetattr(riid, "JPEG_quality", DFNT_UINT8, 1, &attr);
        CHECK(ret, FAIL, "GRsetattr");

        /* Set the compression method for the image */
        comp_type=COMP_CODE_JPEG;
        cinfo.jpeg.quality=100;
        cinfo.jpeg.force_baseline=1;
        ret = GRsetcompress(riid, comp_type, &cinfo);
        CHECK(ret, FAIL, "GRsetcompress");

        /* Write the whole image out */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRwriteimage(riid, start, stride, dims, image0);
        CHECK(ret, FAIL, "GRwriteimage");

        /* Close the image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Close the interface */
        ret = GRend(grid);
        CHECK(ret, FAIL, "GRend");

        /* Check that the image made it out correctly */

        /* Re-open the interface */
        grid = GRstart(fid);
        CHECK(grid, FAIL, "GRstart");

        /* Get the file information */
        ret = GRfileinfo(grid, &n_images, &n_file_attrs);
        CHECK(ret, FAIL, "GRfileinfo");
        if (n_images != 1) {
            MESSAGE(3, printf("Wrong number of images found!\n"););
            num_errs++;
        }

        /* Check the name of the image in the file */
        ret = GRnametoindex(grid, "image1");
        CHECK(ret, FAIL, "GRnametoindex");

        /* Get the first image in this file */
        riid = GRselect(grid, 0);
        CHECK(riid, FAIL, "GRselect");

        /* Get the attribute information */
        ret = GRgetiminfo(riid, name, &n_comps, &datatype, &interlace_mode, dims_out, &n_attrs);
        CHECK(ret, FAIL, "GRgetiminfo");
        if (n_attrs != 1) {
            MESSAGE(3, printf("Wrong number of attributes!\n"););
            num_errs++;
        }

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRreadimage(riid, start, stride, dims, image);
        CHECK(ret, FAIL, "GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image, image0, 10*10)!=0) {
            MESSAGE(3, printf("Error reading data for gzip compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

#ifdef NOT_IMPLEMENTED
        /* Check for compressing image in the middle of writing data */

        /* Get the first image in this file */
        riid = GRcreate(grid, "image2", 1, DFNT_UINT8, MFGR_INTERLACE_PIXEL, dims);
        CHECK(riid, FAIL, "GRcreate");

        /* Write half of the image out */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        count[0]=10; count[1]=5;
        ret = GRwriteimage(riid, start, stride, count, image0);
        CHECK(ret, FAIL, "GRwriteimage");

        /* Set the compression method for the image */
        comp_type=COMP_CODE_JPEG;
        cinfo.jpeg.quality=100;
        cinfo.jpeg.force_baseline=1;
        ret = GRsetcompress(riid, comp_type, &cinfo);
        CHECK(ret, FAIL, "GRsetcompress");

        /* Write the second half of the image out */
        start[0]=0; start[1]=5;
        stride[0]=stride[1]=1;
        count[0]=10; count[1]=5;
        ret = GRwriteimage(riid, start, stride, count, &image0[5][0]);
        CHECK(ret, FAIL, "GRwriteimage");

        /* Close the empty image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");

        /* Check that the image made it out correctly */
        HDmemset(image, 0, 10*10);

        /* Get the second image in this file */
        riid = GRselect(grid, 1);
        CHECK(riid, FAIL, "GRselect");

        /* Read the whole image in */
        start[0]=start[1]=0;
        stride[0]=stride[1]=1;
        ret = GRreadimage(riid, start, stride, dims, image);
        CHECK(ret, FAIL, "GRreadimage");

        /* Verify correct image contents */
        if(HDmemcmp(image, image0, 10*10)!=0) {
            MESSAGE(3, printf("Error reading 2nd data for gzip compressed image\n"););
            num_errs++;
        } /* end if */

        /* Close the image */
        ret = GRendaccess(riid);
        CHECK(ret, FAIL, "GRendaccess");
#endif /* NOT_IMPLEMENTED */
    }
    
    /* Shut down the GR interface */
    ret = GRend(grid);
    CHECK(ret, FAIL, "GRend");

    /* Close the file */
    ret = Hclose(fid);
    CHECK(ret, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
} /* end test_mgr_compress_b() */

/* Create/Write/Read 24-bit JPEG compressed image */
static int test_mgr_compress_c()
{
    intn  status;         /* status for functions returning an intn */
    int32 file_id,        /* HDF file identifier */
          gr_id,          /* GR interface identifier */
          ri_id,          /* raster image identifier */
          start[2],       /* start position to write for each dimension */
          edges[2],       /* number of elements to be written along each dimension */
          dim_sizes[2],   /* dimension sizes of the image array */
          interlace_mode, /* interlace mode of the image */
          data_type,      /* data type of the image data */
          i, j;
    uint8 image_buf[128][128][3];
    uint8 read_buf[128][128][3];
    comp_info  c_info;
    char gname[60];
    int32 n_comps, dt, im, dims[2], na;
    int   num_errs = 0;    /* number of errors so far */
 
    MESSAGE(8, printf("Operate on 24-bit JPEG compressed images\n"););

    /* Create and open the file and initialize GR interface */
    file_id = Hopen(JPEGFILE, DFACC_CREATE, 0);
    CHECK(file_id, FAIL, "Hopen");

    gr_id = GRstart(file_id);
    CHECK(gr_id, FAIL, "GRstart");
 
    /* Set data type, interlace mode, and dimensions of image */
    data_type = DFNT_UINT8;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_sizes[0] = 128;
    dim_sizes[1] = 128;
 
    /* Create the raster image array */
    ri_id = GRcreate(gr_id, "24-bit JPEG", 3, data_type, interlace_mode, dim_sizes);
    CHECK(ri_id, FAIL, "GRcreate");
 
    /* Set JPEG compression */
    c_info.jpeg.quality=75;
    c_info.jpeg.force_baseline=1;
    status = GRsetcompress(ri_id, COMP_CODE_JPEG, &c_info);
    CHECK(status, FAIL, "GRsetcompress");
 
    /* Fill the image data buffer with values */
    for (i = 0; i < 128; i++)
       for (j = 0; j < 128; j++)
       {
          image_buf[i][j][0] = (i+j) + 1;    
          image_buf[i][j][1] = (i+j) + 1;    
          image_buf[i][j][2] = (i+j) + 1;    
       }
 
    /* Write data in the buffer into the image array */
    start[0] = start[1] = 0;
    edges[0] = 128;
    edges[1] = 128;
    status = GRwriteimage(ri_id, start, NULL, edges, (VOIDP)image_buf);
    CHECK(status, FAIL, "GRwriteimage");
 
    /* Terminate access to raster image and to GR interface */
    status = GRendaccess(ri_id);
    CHECK(status, FAIL, "GRendaccess");

    status = GRend(gr_id);
    CHECK(status, FAIL, "GRend");
 
    /* Start access to the GR interface and get access to the first RI */
    gr_id = GRstart(file_id);
    CHECK(gr_id, FAIL, "GRstart");
 
    ri_id = GRselect(gr_id, 0);
    CHECK(ri_id, FAIL, "GRselect");
 
    /* Get and verify information about this image */
    status = GRgetiminfo(ri_id, gname, &n_comps, &dt, &im, dims, &na);
    CHECK(status, FAIL, "GRreadimage");
    VERIFY(n_comps, 3, "GRgetiminfo");
    VERIFY(dt, DFNT_UINT8, "GRgetiminfo");
    VERIFY(dim_sizes[0], dims[0], "GRgetiminfo");
    VERIFY(dim_sizes[1], dims[1], "GRgetiminfo");
    VERIFY(na, 0, "GRgetiminfo");
 
    /* Read back data */
    start[0] = start[1] = 0;
    edges[0] = 128;
    edges[1] = 128;
    status = GRreadimage(ri_id, start, NULL, edges, (VOIDP)read_buf);
    CHECK(status, FAIL, "GRreadimage");
 
    /* Verify correct image contents */
    if(fuzzy_memcmp(image_buf, read_buf, 128*128*3, JPEG_FUZZ)!=0) {
        MESSAGE(3, printf("tmgrcomp: Error reading data for 24-bit JPEG compressed image\n"););
        num_errs++;
    } /* end if */

    /* Terminate access to raster image and to GR */
    /* interface and, close the HDF file.         */
    status = GRendaccess(ri_id);
    CHECK(status, FAIL, "GRendaccess");

    status = GRend(gr_id);
    CHECK(status, FAIL, "GRend");

    status = Hclose(file_id);
    CHECK(status, FAIL, "Hclose");
 
    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
} /* end test_mgr_compress_c() */

/*--------------------------------------------------------------------------
    The following 2 routines are added when bug# 307 was fixed:

    - test_get_compress: tests the new functionality, getting compression
                information of compressed image data.  The test
        + creates a file and four compressed images written to the file, 
          then closes the file.
        + re-opens the file, then reads and verifies each image's
          compression information
        The four images are created using the following compression
        methods in that order: RLE, Skipping Huffman, Deflate, and JPEG.
        For simplicity, all four images use the same data sample.

    - make_comp_image: is a helper that test_get_compress uses to create
                several compressed images.

 -BMR (Sept 7, 01)
--------------------------------------------------------------------------*/

#define	COMPFILE	"gr_comp.hdf"
#define	RLE_IMAGE	"Image with RLE Compression"
#define	DEFLATE_IMAGE	"Image with Deflate Compression"
#define	SKPHUFF_IMAGE	"Image with Skphuff Compression"
#define	JPEG_IMAGE	"Image with JPEG Compression"
#define	DEFLATE_LEVEL		7  /* arbitrary */
#define	SKPHUFF_SKIPSIZE	28  /* arbitrary */

static intn make_comp_image( 
		int32 grid, 
		char* img_name, 
		comp_coder_t comp_type,    /* Compression method */
		comp_info* cinfo, 
		char* message)    /* Compression parameters */
{
    int32 riid;         /* RI ID of the working image */
    int32 dims[2]={10, 10};	/* dimensions for the empty image */
    uint8 image_data[10][10];	/* space for the image data */
    int32 start[2];		/* start of image data to grab */
    int32 stride[2];	/* stride of image data to grab */
    intn i, j;		/* indices */
    intn ret_value;        /* generic return value */

    /* Initialize data we are going to write out */
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            image_data[i][j] = (uint8) (i + j + 10);

    /* Create the image */
    riid = GRcreate(grid, img_name, 1, DFNT_UINT8, MFGR_INTERLACE_PIXEL, dims);
    if (riid == FAIL)
    {
	strcpy(message, "make_comp_image::GRcreate");
	return FAIL;
    }

    /* Set the compression as provided */
    ret_value = GRsetcompress(riid, comp_type, cinfo);
    if (ret_value == FAIL)
    {
	strcpy(message, "make_comp_image::GRsetcompress");
	return FAIL;
    }

    /* Write the image out */
    start[0] = start[1] = 0;
    stride[0] = stride[1] = 1;
    ret_value = GRwriteimage(riid, start, stride, dims, image_data);
    if (ret_value == FAIL)
    {
	strcpy(message, "make_comp_image::GRwriteimage");
	return FAIL;
    }

    /* Close the image */
    ret_value = GRendaccess(riid);
    if (ret_value == FAIL)
    {
	strcpy(message, "make_comp_image::GRendaccess");
	return FAIL;
    }

    return SUCCEED;
}

static int test_get_compress()
{
    int32 fid;          /* HDF file ID */
    int32 grid;         /* GRID for the interface */
    int32 riid;     	/* RI ID of the working image */
    comp_coder_t comp_type;    /* Compression method */
    comp_info cinfo;    /* Compression parameters - union */
    char  err_func[80];	/* name of the functions where failure occurs */
    intn  status;       /* generic return value */
    int   num_errs = 0; /* number of errors so far */

/* D - Retrieve compression information of compressed images */
    MESSAGE(8, printf("Verify the compression information of compressed images\n"););

    /*
     * Create a new file and several images with different compression
     * schemes then close the images and the file
     */

    /* Create an hdf file, and initiate the GR interface */
    fid = Hopen(COMPFILE, DFACC_CREATE, (int16)0);
    CHECK(fid, FAIL, "Hopen");

    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    /* Create and write 4 images, with RLE, deflate, skipping huffman, 
       and JPEG compression methods. */

    /* No compression info for the RLE image */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;

    /* Create and write the first compressed image in this file */
    status = make_comp_image(grid, RLE_IMAGE, COMP_CODE_RLE, &cinfo, err_func);
    CHECK(status, FAIL, err_func);

    /* Set the compression info for the second image with skipping 
       huffman method */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    cinfo.skphuff.skp_size = SKPHUFF_SKIPSIZE;

    /* Create and write the second compressed image in this file */
    status = make_comp_image(grid, SKPHUFF_IMAGE, COMP_CODE_SKPHUFF, &cinfo, err_func);
    CHECK(status, FAIL, err_func);

    /* Set the compression info for the third image with deflate method */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    cinfo.deflate.level = DEFLATE_LEVEL;

    /* Create and write the third compressed image in this file */
    status = make_comp_image(grid, DEFLATE_IMAGE, COMP_CODE_DEFLATE, &cinfo, err_func);
    CHECK(status, FAIL, err_func);

    /* Set the compression method for the fourth image */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    cinfo.jpeg.quality = 100;	/* won't be able to retrieved anyway */
    cinfo.jpeg.force_baseline = 1;

    /* Create and write the fourth compressed image in this file */
    status = make_comp_image(grid, JPEG_IMAGE, COMP_CODE_JPEG, &cinfo, err_func);
    CHECK(status, FAIL, err_func);

    /* Terminate access to the GR interface and close the file */
    status = GRend(grid);
    CHECK(status, FAIL, "GRend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /*
     * Re-open the file COMPFILE, and retrieve the compression information
     * of its two images 
     */
    fid = Hopen(COMPFILE, DFACC_READ, 0);
    CHECK(fid, FAIL, "Hopen");

    grid = GRstart(fid);
    CHECK(grid, FAIL, "GRstart");

    /* get access to the first image */
    riid = GRselect(grid, 0);
    CHECK(riid, FAIL, "GRselect");

    /* First image uses RLE compression method, so no info will be
       retrieved */
    status = GRgetcompinfo(riid, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompinfo");
    VERIFY(comp_type, COMP_CODE_RLE, "GRgetcompinfo");

    /* end access to the first image */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");

    /* get the compression info of the second image, and then check 
     * the values against the values set earlier, which are:
     *		comp_type = COMP_CODE_SKPHUFF 
     *		skp_size = SKPHUFF_SKIPSIZE
     */

    /* get access to the second image */
    riid = GRselect(grid, 1);
    CHECK(riid, FAIL, "GRselect");

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = GRgetcompinfo(riid, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "GRgetcompinfo");
    VERIFY(cinfo.skphuff.skp_size, SKPHUFF_SKIPSIZE, "GRgetcompinfo");

    /* end access to the second image */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");

    /* get the compression info of the third image, and then check 
       the values against the values set earlier, which are:
		comp_type = COMP_CODE_DEFLATE 
		level = DEFLATE_LEVEL
    */

    /* get access to the third image */
    riid = GRselect(grid, 2);
    CHECK(riid, FAIL, "GRselect");

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = GRgetcompinfo(riid, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompinfo");
    VERIFY(comp_type, COMP_CODE_DEFLATE, "GRgetcompinfo");
    VERIFY(cinfo.deflate.level, DEFLATE_LEVEL, "GRgetcompinfo");

    /* Terminate access to the third image */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");

    /* get access to the fourth image */
    riid = GRselect(grid, 3);
    CHECK(riid, FAIL, "GRselect");

    /* get the compression info of the second image, but only check 
       the compression type value against that being set earlier 
       ('quality' and 'force_baseline' are currently not retrievable) */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = GRgetcompinfo(riid, &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompinfo");
    VERIFY(comp_type, COMP_CODE_JPEG, "GRgetcompinfo");
    VERIFY(cinfo.jpeg.quality, 0, "GRgetcompinfo");
    VERIFY(cinfo.jpeg.force_baseline, 0, "GRgetcompinfo");

    /* Terminate access to the third image */
    status = GRendaccess(riid);
    CHECK(status, FAIL, "GRendaccess");

    /* Terminate access and close the file */
    status = GRend(grid);
    CHECK(status, FAIL, "GRend");
    status = Hclose(fid);
    CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
} /* end test_get_compress */

/*--------------------------------------------------------------------------
    The test routine test_mgr_chunk_compress is added when bug# 307 was 
    fixed.

    test_mgr_chunk_compress tests the new functionality, getting 
    compression information of compressed chunked image data.  It 
        + creates the file CHKCOMPFILE and adds four compressed chunked 
	  images to it, then closes the file.
        + re-opens the file, then reads and verifies each chunked image's
          compression information
        The first image is only chunked and not compressed.
        The last three chunked images are compressed using the following
        methods in that order: RLE, Skipping Huffman, Deflate.
        For simplicity, all four images use the same data sample.
    Note: At this time JPEG is not working correctly for chunked images, 
    but when it is, its tests should be added to this routines (and to 
    test_mgr_chunkwr_pixelone as well) appropriately, i.e. another image 
    should be added to the image list.

 -BMR (Oct 7, 01)
--------------------------------------------------------------------------*/

static int test_mgr_chunk_compress()
{
#define  CHKCOMPFILE  "gr_chunkcomp.hdf"
#define  X_LENGTH     10    /* number of columns in the image */
#define  Y_LENGTH     6     /* number of rows in the image */
#define  N_COMPS      3     /* number of components in the image */
#define  N_IMAGES     4     /* number of images tested used - 5 comp. methods */

   /************************* Variable declaration **************************/

   intn  status;         /* status for functions returning an intn */
   int32 file_id,        /* HDF file identifier */
         gr_id,          /* GR interface identifier */
         ri_id[N_IMAGES],       /* raster image identifier */
         origin[2],      /* start position to write for each dimension */
         dim_sizes[2],   /* dimension sizes of the image array */
         interlace_mode, /* interlace mode of the image */
         data_type,      /* data type of the image data */
         comp_flag,      /* compression flag */
         index, 
         img_num;
   int32 start[2], 
         stride[2], 
         edge[2];
   comp_info cinfo;    /* Compression parameters - union */
    int   num_errs = 0;    /* number of errors so far */

   comp_coder_t comp_type;
   int16 data_out[3*Y_LENGTH*X_LENGTH];
   char *image_name[] = { "Image_NO", "Image_RL", "Image_Sk", "Image_DF"};
   HDF_CHUNK_DEF chunk_def[N_IMAGES];
   int16 chunk_buf[18];

   int16 chunk00[] = {        110, 111, 112, 120, 121, 122, 
                              130, 131, 132, 140, 141, 142, 
                              150, 151, 152, 160, 161, 162 };
 
   int16 chunk01[] = {    210, 211, 212, 220, 221, 222, 
                          230, 231, 232, 240, 241, 242, 
                          250, 251, 252, 260, 261, 262};
 
   int16 chunk14[] = {    1010, 1011, 1012, 1020, 1021, 1022, 
                          1030, 1031, 1032, 1040, 1041, 1042, 
                          1050, 1051, 1052, 1060, 1061, 1062};

   int16 data[]    = {
                110, 111, 112, 120, 121, 122, 210, 211, 212, 220, 221, 222,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		130, 131, 132, 140, 141, 142, 230, 231, 232, 240, 241, 242,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		150, 151, 152, 160, 161, 162, 250, 251, 252, 260, 261, 262,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 
                1010, 1011, 1012, 1020, 1021, 1022, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1030, 1031, 1032, 1040, 1041, 
                1042, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                0, 1050, 1051, 1052, 1060, 1061, 1062 }; 


   /********************** End of variable declaration **********************/

/* E - Retrieve compression information of chunked compressed images */
    MESSAGE(8, printf("Verify the compression information of compressed images\n"););

   /* Create and open the file. */
   file_id = Hopen(CHKCOMPFILE, DFACC_CREATE, 0);
   CHECK(file_id, FAIL, "Hopen");

   /* Initialize the GR interface. */
   gr_id = GRstart(file_id);
   CHECK(gr_id, FAIL, "GRstart");

   /* Set the data type, interlace mode, and dimensions of the image. */
   data_type = DFNT_INT16;
   interlace_mode = MFGR_INTERLACE_PIXEL;
   dim_sizes[0] = Y_LENGTH;
   dim_sizes[1] = X_LENGTH;

   for (img_num = 0; img_num < N_IMAGES; img_num++ ) {  

   /* Create the raster image array. */
   ri_id[img_num] = GRcreate(gr_id, image_name[img_num], N_COMPS, data_type, 
                     interlace_mode, dim_sizes);
   CHECK(ri_id[img_num], FAIL, "GRcreate");

   /* Create chunked image array. */
   switch (img_num) {
	case 0: 
		comp_flag = HDF_CHUNK;
		chunk_def[img_num].chunk_lengths[0] = 3;
		chunk_def[img_num].chunk_lengths[1] = 2;
		break;
	case 1 :
		comp_flag = HDF_CHUNK | HDF_COMP;
		chunk_def[img_num].comp.chunk_lengths[0] = 3;
		chunk_def[img_num].comp.chunk_lengths[1] = 2;
		chunk_def[img_num].comp.comp_type = COMP_CODE_RLE;
		break;
	case 2 :
		comp_flag = HDF_CHUNK | HDF_COMP;
		chunk_def[img_num].comp.chunk_lengths[0] = 3;
		chunk_def[img_num].comp.chunk_lengths[1] = 2;
		chunk_def[img_num].comp.comp_type = COMP_CODE_SKPHUFF;
		chunk_def[img_num].comp.cinfo.skphuff.skp_size = 2;
		break;
	case 3 :
		comp_flag = HDF_CHUNK | HDF_COMP;
		chunk_def[img_num].comp.chunk_lengths[0] = 3;
		chunk_def[img_num].comp.chunk_lengths[1] = 2;
		chunk_def[img_num].comp.comp_type = COMP_CODE_DEFLATE;
		chunk_def[img_num].comp.cinfo.deflate.level = 6;
		break;
#ifdef NOT_WORKING
	/* JPEG compression for chunked images is not working correctly 
	   yet.  Add test here when it is */
	case 4 :
		comp_flag = HDF_CHUNK | HDF_COMP;
		chunk_def[img_num].comp.chunk_lengths[0] = 3;
		chunk_def[img_num].comp.chunk_lengths[1] = 2;
		chunk_def[img_num].comp.comp_type = COMP_CODE_JPEG;
		chunk_def[img_num].comp.cinfo.jpeg.quality = 5;
		chunk_def[img_num].comp.cinfo.jpeg.force_baseline = 8;
		break;
#endif
	default:
		printf("Error\n");
		break;

   } /* end switch */
    
   status = GRsetchunk(ri_id[img_num], chunk_def[img_num], comp_flag);
   CHECK(status, FAIL, "GRsetchunk");

   /* Write first data chunk ( 0, 0 ). */
   origin[0] = origin[1] = 0;
   status = GRwritechunk(ri_id[img_num], origin, (VOIDP)chunk00);
   CHECK(status, FAIL, "GRwritechunk");

   /* Write second data chunk ( 0, 1 ). */
   origin[0] = 0; origin[1] = 1;
   status = GRwritechunk(ri_id[img_num], origin, (VOIDP)chunk01);
   CHECK(status, FAIL, "GRwritechunk");

   /* Write third data chunk ( 1, 4 ). */
   origin[0] = 1; origin[1] = 4;
   status = GRwritechunk(ri_id[img_num], origin, (VOIDP)chunk14);
   CHECK(status, FAIL, "GRwritechunk");

   /* Read third chunk back. */
   origin[0] = 1; origin[1] = 4;
   status = GRreadchunk(ri_id[img_num], origin, (VOIDP)chunk_buf);
   CHECK(status, FAIL, "GRreadchunk");

   /* Terminate access to the GR interface and close the HDF file. */
   status = GRendaccess(ri_id[img_num]);
   CHECK(status, FAIL, "GRendaccess");
 }  /* end for */

   status = GRend(gr_id);
   CHECK(status, FAIL, "GRend");
   status = Hclose(file_id);
   CHECK(status, FAIL, "Hclose");

    /* Open the file. */

    file_id = Hopen(CHKCOMPFILE, DFACC_WRITE, 0); 
    CHECK(file_id, FAIL, "Hopen");

   /* Initialize the GR interface. */
   gr_id = GRstart(file_id);
   CHECK(gr_id, FAIL, "GRstart");

   for (img_num = 0; img_num < N_IMAGES; img_num++ ) {  

   /* Find the index of the specified image. */
   index = GRnametoindex(gr_id, image_name[img_num]);
   CHECK(index, FAIL, "GRnametoindex");
   
   /* Select the image. */
   ri_id[img_num] = GRselect(gr_id, index);
   CHECK(ri_id[img_num], FAIL, "GRselect");

   /* Get and verify the image's compression information */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;

    status = GRgetcompinfo(ri_id[img_num], &comp_type, &cinfo);
    CHECK(status, FAIL, "GRgetcompinfo");
    switch (img_num) {
	case 0: 
	    VERIFY(comp_type, COMP_CODE_NONE, "GRgetcompinfo");
	    break;
	case 1 :
	    VERIFY(comp_type, COMP_CODE_RLE, "GRgetcompinfo");
	    break;
	case 2 :
	    VERIFY(comp_type, COMP_CODE_SKPHUFF, "GRgetcompinfo");
	    VERIFY(cinfo.skphuff.skp_size, 
		   chunk_def[img_num].comp.cinfo.skphuff.skp_size, "GRgetcompinfo");
	    break;
	case 3 :
	    VERIFY(comp_type, COMP_CODE_DEFLATE, "GRgetcompinfo");
	    VERIFY(cinfo.deflate.level, 
		   chunk_def[img_num].comp.cinfo.deflate.level, "GRgetcompinfo");
	    break;
#ifdef NOT_WORKING
	/* JPEG is not working correctly yet.  Add test here when it is */
	case 4 :  /* only return comp type for JPEG */
	    VERIFY(comp_type, COMP_CODE_JPEG, "GRgetcompinfo");
	    break;
#endif
	default:
	    printf("Error\n");
	    break;
   } /* end switch */

   /* Read third chunk back. */
   origin[0] = 1; origin[1] = 4;
   status = GRreadchunk(ri_id[img_num], origin, (VOIDP)chunk_buf);
   CHECK(status, FAIL, "GRreadchunk");
   if (0 != HDmemcmp(chunk_buf, chunk14 , sizeof(chunk14)))
      {
            MESSAGE(3, printf("Error in reading chunk at line %d\n", __LINE__););
            MESSAGE(3, printf("Image #%d\n", (int)img_num););
            num_errs++;
      } /* end if */

   /* Read the whole image. */
   start[0] = start[1] = 0;
   stride[0] = stride[1] = 1;
   edge[0] = Y_LENGTH;
   edge[1] = X_LENGTH;
   status = GRreadimage(ri_id[img_num], start, stride, edge, (VOIDP)data_out);
   CHECK(status, FAIL, "GRreadimage");
   if (0!= HDmemcmp(data_out, data, sizeof(data)))
      {
            MESSAGE(3, printf("%d: Error reading data for the whole image\n", __LINE__););
            MESSAGE(3, printf("%d: Compression method\n", (int)img_num););
            num_errs++;
      } /* end if */

   status = GRendaccess(ri_id[img_num]);
   CHECK(status, FAIL, "GRendaccess");

   } /* end for */    

   /* Terminate access to the GR interface and close the HDF file. */
   status = GRend(gr_id);
   CHECK(status, FAIL, "GRend");
   status = Hclose(file_id);
   CHECK(status, FAIL, "Hclose");

    /* Return the number of errors that's been kept track of so far */
    return(num_errs);
}  /* end of test_mgr_chunk_compress */


/****************************************************************
**
**  test_mgr_compress(): Multi-file Raster Compression tests
** 
**  IX. Compressed image tests
**      A. Create/Read/Write gzip compressed Image
**      B. Create/Read/Write 8-bit JPEG compressed Image
**      C. Create/Read/Write 24-bit JPEG compressed Image
**      D. Retrieve various compression information of compressed Image
**	E. Retrieve various compression info. of compressed, chunked images
** 
****************************************************************/
extern void test_mgr_compress()
{
    int num_errs = 0;

    /* Output message about test being performed */
    MESSAGE(5, printf("Testing Multi-file Raster Compression Functions\n"););

    /* Test compression functions on GZIP compressed image */
    num_errs = num_errs + test_mgr_compress_a();

    /* Test compression functions on 8-bit JPEG compressed image */
    num_errs = num_errs + test_mgr_compress_b();

    /* Test compression functions on 24-bit JPEG compressed image */
    num_errs = num_errs + test_mgr_compress_c();

    /* Test retrieving various compression information of compressed image */
    num_errs = num_errs + test_get_compress();

    /* Test retrieving various compression information of chunked and
       compressed image */
    num_errs = num_errs + test_mgr_chunk_compress();

    if (num_errs != 0) { H4_FAILED(); }
}   /* end test_mgr_compress() */
