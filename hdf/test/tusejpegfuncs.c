/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The test functions in this file are derived from an example included in   *
 * the JPEG package by The Independent JPEG Group                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include "jpeglib.h"
#include "tproto.h"

#define ABS(x)  ((int)(x)<0 ? (-x) : x)

/************************************************************************
   Name: comp_using_jpeglib() - compresses a buffer using JPEG functions

   Description:
        This routine uses functions from the JPEG library directly to
	compress the provided image buffer and writes the compressed image
	to the specified file.
   Return value:
        The number of errors occurred in this routine.
   Apr 11, 2011 -BMR
*************************************************************************/
intn comp_using_jpeglib(
	char *filename,		/* file to write compressed data in */
	long *file_offset,	/* end offset of previous data and indicating where
				   to start writing data in this round */
	int im_height,		/* image's height */
	int im_width,		/* image's width */
	int im_ncomps,		/* image's number of components */
	int quality,		/* JPEG quality value */
	uint8 *written_buffer)	/* data to be compressed */
{
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    /* JPEG object for JPEG compression parameters and pointers to working space
       (which is allocated as needed by the JPEG library). */ 
    struct jpeg_compress_struct cinfo;

    /* This struct represents a JPEG error handler.  It is declared separately
     * because applications often want to supply a specialized error handler
     * (see the second half of this file for an example).  But here we just
     * take the easy way out and use the standard error handler, which will
     * print a message on stderr and call exit() if compression fails.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct jpeg_error_mgr jerr;
  
    /* Initialize JPEG compression object */
  
    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error(&jerr);
  
    /* Initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);
  
    /* Open the output file to write binary data */
    if ((outfile = fopen(filename, "ab")) == NULL)
    {
      fprintf(stderr, "can't open %s\n", filename);
      exit(1);
    }

    /* Forward to the position to write data */
    if (fseek(outfile, (off_t)*file_offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "can't seek offset %d\n", (int)*file_offset);
        exit(1);
    }

    /* Specify output file */
    jpeg_stdio_dest(&cinfo, outfile);
  
    /* Set parameters for compression */
  
    /* Supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width = im_width; 	/* image width and height, in pixels */
    cinfo.image_height = im_height;
    cinfo.input_components = im_ncomps;/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  
    /* Set default compression parameters.  At least cinfo.in_color_space must
       be set before calling jpeg_set_defaults, since the defaults depend on the
       source color space */
    jpeg_set_defaults(&cinfo);

    /* Set quality (quantization table) scaling */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
     * From JPEG: Pass TRUE unless you are very sure of what you're doing. */
    jpeg_start_compress(&cinfo, TRUE);

    row_stride = im_width * im_ncomps;/* JSAMPLEs per row in written_buffer */

    /* While there are more scan line in the buffer */
    while (cinfo.next_scanline < cinfo.image_height)
    {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
	row_pointer[0] = &written_buffer[cinfo.next_scanline * row_stride];
	(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Finish compression */
    jpeg_finish_compress(&cinfo);

    /* Get the current file offset to return */
    *file_offset = ftell(outfile);

    /* Close the file */
    fclose(outfile);

    /* Release JPEG compression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    return 0;
} /* comp_using_jpeglib */

/***************************************************************************
   Name: decomp_using_jpeglib() - decompresses a buffer using JPEG functions

   Description:
        This routine uses functions from the JPEG library directly to
	decompress the data read from the specified file and store the
	uncompressed data in the provided buffer.
   Return value:
        The number of errors occurred in this routine.
   Apr 11, 2011 -BMR
****************************************************************************/
intn decomp_using_jpeglib(
	char *filename,		/* file to read compressed data from */
	long file_offset,	/* offset in the file to start reading */
	int im_height,		/* image's height */
	int im_width,		/* image's width */
	int im_ncomps,		/* image's number of components */
	uint8 *read_buffer)	/* buffer to store decompressed data */
{
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo; /* JPEG compression info */
    struct jpeg_error_mgr jerr_pub; /* JPEG error handler */
    FILE * infile;	/* source file */
    JSAMPARRAY buffer;	/* Output row buffer */
    int row_stride;	/* physical row width in output buffer */
    uint8 *local_buf = NULL, *ptr = NULL;

    /* Open the output file to write binary data */
    if ((infile = fopen(filename, "rb")) == NULL)
    {
	fprintf(stderr, "can't open %s\n", filename);
	exit(1);
    }

    /* Forward to the specified position to write data */
    if (fseek(infile, (off_t)file_offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "can't seek offset %d\n", (int)file_offset);
        exit(1);
    }

    /* Allocate local buffer to hold read values until all reading is done
       before copying into caller's buffer */
    local_buf = HDmalloc(im_height * im_width * im_ncomps * sizeof(uint8));
    CHECK_ALLOC(local_buf, "local_buf", "decomp_using_jpeglib" );

    /* Set up the JPEG error routines */
    cinfo.err = jpeg_std_error(&jerr_pub);

    /* Initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Specify input file */
    jpeg_stdio_src(&cinfo, infile);

    /* Read file parameters */
    (void) jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
     *   (a) suspension is not possible with the stdio data source, and
     *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.doc for more info.
     */

    /* Start decompressor */
    (void) jpeg_start_decompress(&cinfo);
    /* the return value is ignored here since suspension is not possible
     * with the stdio data source (from JPEG example) */

    /* Number of values per row */
    row_stride = cinfo.output_width * cinfo.output_components;

    /* Make a one-row-high array to read a row of values,  JSAMPLEs per row */
    buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    ptr = local_buf; /* index into the buffer */

    /* Read each scanline until all are read */
    while (cinfo.output_scanline < cinfo.output_height)
    {
	/* jpeg_read_scanlines expects an array of pointers to scanlines. */
	(void) jpeg_read_scanlines(&cinfo, buffer, 1);

	/* Saved read line to the local buffer */
	memcpy(ptr, buffer[0], row_stride); 
	ptr = ptr + row_stride;
    }

    /* Copying values from local buffer to caller's buffer after success */
    memcpy(read_buffer, local_buf, im_height*im_width*im_ncomps);
    HDfree(local_buf);

    /* Finish decompression */
    (void) jpeg_finish_decompress(&cinfo);

    /* Release JPEG decompression object */
    jpeg_destroy_decompress(&cinfo);

    /* Close the file */
    fclose(infile);

    return 0;
} /* decomp_using_jpeglib */
