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

/************************************************************************
 * tdfr8.c - test file for images of DFR8 API
 *************************************************************************/

#include "hdf.h"
#include "tproto.h"	/* for utility macros */

/************************************************************************
   Name: test_GRgetcomptype() - test GRgetcomptype for hmap project

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

	The function GRgetcomptype will be call on each image and the returned
	compression type will be verified against the list of compressions used.
   BMR - Mar 12, 2011
**************************************************************************/
#define FILE_NAME	"tGRgetcomptype.hdf"
#define HEIGHT		3
#define WIDTH		8
#define N_IMAGES	12
#define N_ENTRIES	256
#define N_COMPONENTS	3

void
test_GRgetcomptype()
{
    intn  ii;
    int   row, col;
    int   entry, component;
    uint8 raster[HEIGHT][WIDTH];
    uint8 palette[N_ENTRIES*N_COMPONENTS],
	  temp_pal[N_ENTRIES][N_COMPONENTS];
    int32 file_id, gr_id, ri_id;
    int32 num_images=0, num_fattrs=0;
    intn  status;
    comp_coder_t comp_type;
    comp_info compress_info;

    /* used to verify the compression type of the images */
    comp_coder_t check_comp[N_IMAGES] = {COMP_CODE_NONE, COMP_CODE_NONE,
		COMP_CODE_NONE, COMP_CODE_NONE, COMP_CODE_RLE, COMP_CODE_RLE,
		COMP_CODE_IMCOMP, COMP_CODE_IMCOMP, COMP_CODE_JPEG,
		COMP_CODE_JPEG, COMP_CODE_RLE, COMP_CODE_RLE};

    /* used to verify whether an image should be mapped */
    intn maplist[N_IMAGES] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE};

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
    for ( entry = 0; entry < N_ENTRIES; entry++ ) {
        for ( component = 0; component < N_COMPONENTS; component++ )
	{
            if (( 0 <= entry ) && ( entry <= 7 ))
	    {   /* values in first row of raster */
                temp_pal[entry][0] = 255;    /* Red */
                temp_pal[entry][1] = 0;      /* Green */
                temp_pal[entry][2] = 0;      /* Blue */
            } else if (( 10 <= entry ) && ( entry <= 17)) 
	    {   /* values in second row of raster */
                temp_pal[entry][0] = 0;       /* Red */
                temp_pal[entry][1] = 255;     /* Green */
                temp_pal[entry][2] = 0;       /* Blue */
            } else if (( 20 <= entry ) && ( entry <= 27)) 
	    {   /* values in third row of raster */
                temp_pal[entry][0] = 0;       /* Red */
                temp_pal[entry][1] = 0;       /* Green */
                temp_pal[entry][2] = 255;     /* Blue */
            } else 
	    {   /* all else (don't expect) will be black */
                temp_pal[entry][0] = 0;       /* Red */
                temp_pal[entry][1] = 0;       /* Green */
                temp_pal[entry][2] = 0;       /* Blue */
            }
        }
    }

    /* Work around to pass "palette" into DFR8setpalette w/o compiler warning.*/
    HDmemcpy(palette, temp_pal, N_ENTRIES*N_COMPONENTS);

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
        intn  is_mappedable;/* TRUE if the image is mapped-able (hmap project)*/
        intn  name_generated; /* TRUE if the image has name generated by lib */

	ri_id = GRselect(gr_id, ii);
	CHECK_VOID(ri_id, FAIL, "GRselect");

	/* Get image's compression type */
	status = GRgetcomptype(ri_id, &comp_type);
	CHECK_VOID(status, FAIL, "GRgetcomptype");

        /* Test GR2bmapped on this image (For hmap project only) */
        status = GR2bmapped(ri_id, &is_mappedable, &name_generated);
        CHECK_VOID(status, FAIL, "GR2bmapped");
        VERIFY_VOID(is_mappedable, maplist[ii], "GR2bmapped");
        VERIFY_VOID(name_generated, TRUE, "GR2bmapped");

	/* Verify compression type */
  	if (comp_type != check_comp[ii])
	VERIFY_VOID(check_comp[ii], comp_type, "GRgetcomptype");

	status = GRendaccess(ri_id);
	CHECK_VOID(status, FAIL, "GRendaccess");
    }

    /* Terminate access to the file */
    status = GRend(gr_id);
    CHECK_VOID(status, FAIL, "GRend");
    status = Hclose(file_id);
    CHECK_VOID(status, FAIL, "Hclose");

} /* test_GRgetcomptype */
