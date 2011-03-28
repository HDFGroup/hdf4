/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/************************************************************************
 * tdfr8.c - test file for images of DFR8 API
 *************************************************************************/

/* This is only for testing and should be done in configuration later */
#ifdef DATAINFO_MASTER
   #ifndef DATAINFO_TESTER
   #define DATAINFO_TESTER /* to include hmap project's features */
   #endif
#endif

#ifdef DATAINFO_TESTER
#include "hdf.h"
#include "tproto.h"	/* for utility macros */

/************************************************************************
   Name: test_grgetcomptype() - test grgetcomptype for hmap project

   Description:
	This routine uses DFR8 API to create the following images:
	+ Name image0 with no compression and no palette
	+ Name image1 with no compression and no palette
	+ Name image2 with no compression and with palette
	+ Name image3 with no compression and with palette
	+ Name image4 with RLE compression and with palette
	+ Name image5 with RLE compression and with palette
	+ Name image6 with IMCOMP compression and with palette
	+ Name image7 with IMCOMP compression and with palette
	* Name image8 with JPEG compression and with palette
	+ Name image9 with JPEG compression and with palette
	+ Name image10 with RLE compression and with palette
	+ Name image11 with RLE compression and with palette
	All have same data in rasters and in palette (when used)
	Note: This part of the test was adopted from Ruth's test program.  

	The function grgetcomptype will be call on each image and the returned
	compression type will be verified against the list of compressions used.
   BMR - Mar 12, 2011
**************************************************************************/
#define FILE_NAME	"tgrgetcomptype.hdf"
#define HEIGHT		3
#define WIDTH		8
#define NUM_IMAGES	12

void
test_grgetcomptype()
{
    int   row, col;
    int   entry, component;
    comp_info compress_info;
    intn  status;
    uint8 raster[HEIGHT][WIDTH];
    uint8 palette[256][3];
    uint8 image[10][10];
    int32 file_id, gr_id, ri_id;
    int32 comp_type;
    int32 num_images=0, num_fattrs=0;
    intn  ii;


    /* used to verify the compression type of the images */
    int32 check_comp[NUM_IMAGES] = {COMP_CODE_NONE, COMP_CODE_NONE,
		COMP_CODE_NONE, COMP_CODE_NONE, COMP_CODE_RLE, COMP_CODE_RLE,
		COMP_IMCOMP, COMP_IMCOMP, COMP_CODE_JPEG, COMP_CODE_JPEG,
		COMP_CODE_RLE, COMP_CODE_RLE};

    /* Initialize the raster image array */
    for ( row = 0; row < HEIGHT; row++ ) {
        for ( col = 0; col < WIDTH; col++ ) {
            raster[row][col] = row*10 + col;
        }
    }
        
    /* 
     * Initialize the palette array (256 entries of RGB components)
     * We want our test case to be easy to see, so only turn
     * on colors for values we have in raster image.
     * Red for first row; Green for second row; Blue for third row
     * -RA
     */
    for ( entry = 0; entry < 256; entry++ ) {
        for ( component = 0; component < 3; component++ )
	{
            if (( 0 <= entry ) && ( entry <= 7 ))
	    {   /* values in first row of raster */
                palette[entry][0] = 255;    /* Red */
                palette[entry][1] = 0;      /* Green */
                palette[entry][2] = 0;      /* Blue */
            } else if (( 10 <= entry ) && ( entry <= 17)) 
	    {   /* values in second row of raster */
                palette[entry][0] = 0;       /* Red */
                palette[entry][1] = 255;     /* Green */
                palette[entry][2] = 0;       /* Blue */
            } else if (( 20 <= entry ) && ( entry <= 27)) 
	    {   /* values in third row of raster */
                palette[entry][0] = 0;       /* Red */
                palette[entry][1] = 0;       /* Green */
                palette[entry][2] = 255;     /* Blue */
            } else 
	    {   /* all else (don't expect) will be black */
                palette[entry][0] = 0;       /* Red */
                palette[entry][1] = 0;       /* Green */
                palette[entry][2] = 0;       /* Blue */
            }
        }
    }

    /* Write image0 to the HDF4 file with no compression and no palette
     * Note that the order of args 3 and 4 is width then height.  */
    status = DFR8putimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_NONE );
    CHECK_VOID(status, FAIL, "DFR8putimage");

    /* Write image1 to the HDF4 file with no compression and no palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_NONE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Set the palette */
    status = DFR8setpalette( palette );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image2 with no compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_NONE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image3 with no compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_NONE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image4 with RLE compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_RLE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image5 with RLE compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_RLE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image6 with IMCOMP compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_IMCOMP );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image7 with IMCOMP compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_IMCOMP );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image8 with JPEG compression and with palette */
    compress_info.jpeg.quality = 60;
    compress_info.jpeg.force_baseline = 1;
    status = DFR8setcompress( COMP_JPEG, &compress_info );
    CHECK_VOID(status, FAIL, "DFR8setcompress");

    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_JPEG );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image9 with JPEG compression and with palette */
    compress_info.jpeg.quality = 60;
    compress_info.jpeg.force_baseline = 1;
    status = DFR8setcompress( COMP_JPEG, &compress_info );
    CHECK_VOID(status, FAIL, "DFR8setcompress");

    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_JPEG );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image10 with RLE compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_RLE );
    CHECK_VOID(status, FAIL, "DFR8addimage");

    /* Write image11 with RLE compression and with palette */
    status = DFR8addimage( FILE_NAME, raster, WIDTH, HEIGHT, COMP_RLE );
    CHECK_VOID(status, FAIL, "DFR8addimage");


    /* Open the file in GR interface and verify the compression type of each
	image in the file */

    /* Open the file and initialize the GR interface */
    file_id = Hopen(FILE_NAME, DFACC_READ, 0);
    CHECK_VOID(file_id, FAIL, "Hopen");
    gr_id = GRstart(file_id);
    CHECK_VOID(gr_id, FAIL, "GRstart");

    /* Get the number of images in the file */
    status = GRfileinfo(gr_id, &num_images, &num_fattrs);
    CHECK_VOID(status, FAIL, "GRfileinfo");

    /* Verify compression type of each image */
    for (ii = 0; ii < num_images; ii++)
    {
	ri_id = GRselect(gr_id, ii);
	CHECK_VOID(ri_id, FAIL, "GRselect");

	/* Get image's compression type */
	status = grgetcomptype(ri_id, &comp_type);
	CHECK_VOID(status, FAIL, "grgetcomptype");

	/* Verify compression type */
  	if (comp_type != check_comp[ii])
	VERIFY_VOID(check_comp[ii], comp_type, "grgetcomptype");

	status = GRendaccess(ri_id);
	CHECK_VOID(status, FAIL, "GRendaccess");
    }

    /* Terminate access to the file */
    status = GRend(gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose(file_id);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_grgetcomptype */
#endif /* DATAINFO_TESTER */
