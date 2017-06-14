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

/****************************************************************************
 * tcomp.c - tests various compression functionality
 * Structure of the file:
 *    test_compression - test driver
 *	  test_various_comps - creates several data sets with different
 *		compression methods.
 *
****************************************************************************/

#include "mfhdf.h"

#ifdef HDF

#include "hdftest.h"

/********************************************************************
   Name: test_various_comps() - creates several data sets with different
		compression methods.

   Description:
	This function creates and writes data to 3 data sets using three
	compression methods, deflate, skipping huffman, and szip, in that
	order.  The output file can be used to test display tools like
	hdp.

   Return value:
        The number of errors occurred in this routine.

   BMR - May 18, 2007
*********************************************************************/

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif

#define FILE_NAME      "sds_compressed.hdf"
#define SDS1_NAME      "SDSgzip"
#define SDS2_NAME      "SDSskiphuff"
#define SDS3_NAME      "SDSszip"
#define X_LENGTH       5
#define Y_LENGTH       16
#define RANK           2


static intn
test_various_comps()
{
    /************************* Variable declaration **************************/

    int32     sd_id, sds_id, sds_index;
    intn      status;
    int32     comp_type;    /* Compression flag */
    comp_info c_info;   /* Compression structure */
    int32     start[2], edges[2], dim_sizes[2];
    int32     data[Y_LENGTH][X_LENGTH];
    int32     pixels_per_scanline;
    intn      num_errs = 0;    /* number of errors in compression test so far */
    int       i, j;

    /********************* End of variable declaration ***********************/

    /* Buffer array data and define array dimensions. */
    for (j = 0; j < Y_LENGTH; j++) 
    {
	for (i = 0; i < X_LENGTH; i++)
		data[j][i] = (i + j) + 1;
    }
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

    /* Create 1st data set for GZIP compression. */ 
    sds_id = SDcreate (sd_id, SDS1_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set data set SDS1_NAME to use GZIP compression. */
    HDmemset(&c_info, 0, sizeof(c_info));
    comp_type = COMP_CODE_DEFLATE;
    c_info.deflate.level = 6;
    status = SDsetcompress (sds_id, comp_type, &c_info); 
    CHECK(status, FAIL, "SDsetcompress");

    /* Write the stored data to the 1st data set. */
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the 1st data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

    /* Create 2nd data set for Skipping Huffman compression. */
    sds_id = SDcreate (sd_id, SDS2_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set data set SDS2_NAME to use Skipping Huffman compression. */
    HDmemset(&c_info, 0, sizeof(c_info));
    comp_type = COMP_CODE_SKPHUFF;
    c_info.skphuff.skp_size = 4;
    status = SDsetcompress (sds_id, comp_type, &c_info); 
    CHECK(status, FAIL, "SDsetcompress");
 
    /* Write the stored data to the 2nd data set. */
    status = SDwritedata (sds_id, start, NULL, edges, (VOIDP)data);
    CHECK(status, FAIL, "SDwritedata");

    /* Terminate access to the 2nd data set. */
    status = SDendaccess (sds_id);
    CHECK(status, FAIL, "SDendaccess");

#ifdef H4_HAVE_SZIP_ENCODER     /* we have szip library with encoder */
    /* Create 3rd data set for SZIP compression. */
    sds_id = SDcreate (sd_id, SDS3_NAME, DFNT_INT32, RANK, dim_sizes);
    CHECK(sds_id, FAIL, "SDcreate");

    /* Set data set SDS3_NAME to use SZIP compression. */
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
#endif

    /* Terminate access to the SD interface and close the file. */
    status = SDend (sd_id);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;
}   /* test_various_comps */

#define COMPFILE1 "comptst1.hdf"
#define COMPFILE2 "comptst2.hdf"
#define COMPFILE3 "comptst3.hdf"
#define COMPFILE4 "comptst4.hdf"
#define COMPFILE5 "comptst5.hdf"
#define COMPFILE6 "comptst6.hdf"
#define COMPFILE7 "comptst7.hdf"
static int
test_compressed_data()
{
    int32 fcomp; /* File handle */
    int32 index;             /* Index of a dataset */
    int32 nt;                /* Number type */
    int32 dimsize[10];       /* dimension sizes */
    int32 newsds, newsds2; 	/* SDS handles */
    comp_coder_t comp_type;	/* to retrieve compression type into */
    comp_info cinfo;            /* compression information structure */
    int32 idata[100];
    int32 rdata[100];
    int32 fillval;
    intn  i;
    intn  num_errs = 0;    /* number of errors in compression test so far */
    intn  status;      /* status flag */
    int32 start[10], end[10]; /* start and end arrays */

    /*
     * Writing 1st compressed dataset, basic skipping huffman.
     */
    fcomp = SDstart(COMPFILE1, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet1", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    /* Set the dataset to be compressed with skipping huffman */
    cinfo.skphuff.skp_size=4;
    status = SDsetcompress(newsds,COMP_CODE_SKPHUFF,&cinfo);
    CHECK(status, FAIL, "SDcompress");

    /* Test get compression info when the data set is empty but set to be
       compressed */
    status = SDgetcompinfo(newsds, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "SDgetcompinfo");

    /* Only get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds, &comp_type);
    CHECK(status, FAIL, "SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "SDgetcomptype");

    /* Write data to the dataset */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    /* End access to the dataset */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read and verify the compressed data and information
     */
    fcomp = SDstart(COMPFILE1, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    /*
     * Retrieve and verify the compression info - bug# 307, 9/7/01 - BMR
     */
#ifndef H4_NO_DEPRECATED_SYMBOLS /* Jan 9, 2013 */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompress(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompress");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "SDgetcompress");
    VERIFY(cinfo.skphuff.skp_size, 4, "SDgetcompress");
#endif /* H4_NO_DEPRECATED_SYMBOLS */

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "SDgetcompinfo");
    VERIFY(cinfo.skphuff.skp_size, 4, "SDgetcompinfo");

    /* Only get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds2, &comp_type);
    CHECK(status, FAIL, "SDgetcomptype");
    VERIFY(comp_type, COMP_CODE_SKPHUFF, "SDgetcomptype");

    /* Read and verify the compressed data */
    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"thisone: Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    /* End access to the dataset and the file */
    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Writing 2nd compressed dataset, partially filled & skipping huffman
     */
    fcomp = SDstart(COMPFILE2, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet2", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    /* Set fill value */
    fillval=43;
    status = SDsetfillvalue(newsds,(VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    /* Set the dataset to be compressed with skipping huffman */
    cinfo.skphuff.skp_size=4;
    status = SDsetcompress(newsds,COMP_CODE_SKPHUFF,&cinfo);
    CHECK(status, FAIL, "SDsetcompress");

    /* fill the array with the standard info */
    for(i = 0; i < 25; i++)
        idata[i] = i*10;
    /* overwrite selected portions with the fill value */
    for(i = 0; i < 5; i++)
        idata[i] = fillval;
    for(i = 20; i < 25; i++)
        idata[i] = fillval;

    /* Write to the dataset */
    start[0] = 1;
    start[1] = 0;
    end[0]   = 3;
    end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) &idata[5]);
    CHECK(status, FAIL, "SDwritedata");

    /* End access to the datase */
    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read and verify the compressed data and information
     */
    fcomp = SDstart(COMPFILE2, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    /* Get the dataset by its name */
    index = SDnametoindex(fcomp, "CompDataSet2");
    CHECK(index, FAIL, "SDnametoindex");

    newsds2 = SDselect(fcomp, index);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Creating 3rd compressed dataset, compressed template & skipping huffman
     */
    fcomp = SDstart(COMPFILE3, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet3", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    fillval=56;
    status = SDsetfillvalue(newsds,(VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    cinfo.skphuff.skp_size=4;
    status = SDsetcompress(newsds,COMP_CODE_SKPHUFF,&cinfo);
    CHECK(status, FAIL, "SDsetcompress");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read the compressed data back in
     */
    fcomp = SDstart(COMPFILE3, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(fillval != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Creating 4th compressed dataset, compressed template read, then 
     * partial write & skipping huffman
     */
    fcomp = SDstart(COMPFILE4, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet4", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    fillval=67;
    status = SDsetfillvalue(newsds,(VOIDP)&fillval);
    CHECK(status, FAIL, "SDsetfillvalue");

    cinfo.skphuff.skp_size=4;
    status = SDsetcompress(newsds,COMP_CODE_SKPHUFF,&cinfo);
    CHECK(status, FAIL, "SDsetcompress");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read the compressed data back in
     */
    fcomp = SDstart(COMPFILE4, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(fillval != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Modifying first compressed dataset.
     */
    fcomp = SDstart(COMPFILE4, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }
    /* fill the array with the standard info */
    for(i = 0; i < 25; i++)
        idata[i] = i*10;
    /* overwrite selected portions with the fill value */
    for(i = 0; i < 10; i++)
        idata[i] = fillval;
    for(i = 20; i < 25; i++)
        idata[i] = fillval;

    start[0] = 2;
    start[1] = 0;
    end[0]   = 2;
    end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) &idata[10]);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    fcomp = SDstart(COMPFILE4, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Writing 5th compressed dataset, basic RLE.
     */
    fcomp = SDstart(COMPFILE5, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet5", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    status = SDsetcompress(newsds,COMP_CODE_RLE,&cinfo);
    CHECK(status, FAIL, "SDcompress");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read the compressed data back in.
     */
    fcomp = SDstart(COMPFILE5, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    /*
     * Retrieve and verify the compression info - bug# 307, 9/7/01 - BMR
     */
#ifndef H4_NO_DEPRECATED_SYMBOLS /* Jan 9, 2013 */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompress(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompress");
    VERIFY(comp_type, COMP_CODE_RLE, "SDgetcompress");
#endif /* H4_NO_DEPRECATED_SYMBOLS */

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_RLE, "SDgetcompinfo");

    /* Only get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds2, &comp_type);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_RLE, "SDgetcompinfo");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Writing 6th compressed dataset, no encoding
     */
    fcomp = SDstart(COMPFILE6, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet6", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    status = SDsetcompress(newsds,COMP_CODE_NONE,&cinfo);
    CHECK(status, FAIL, "SDcompress");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read the compressed data back in.
     */
    fcomp = SDstart(COMPFILE6, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    /*
     * Retrieve and verify the compression info - bug# 307, 9/7/01 - BMR
     */
#ifndef H4_NO_DEPRECATED_SYMBOLS /* Jan 9, 2013 */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompress(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompress");
    VERIFY(comp_type, COMP_CODE_NONE, "SDgetcompress");
#endif /* H4_NO_DEPRECATED_SYMBOLS */

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_NONE, "SDgetcompinfo");

    /* Only get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds2, &comp_type);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_NONE, "SDgetcompinfo");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Writing 7th compressed dataset, deflate encoding.
     */
    fcomp = SDstart(COMPFILE7, DFACC_CREATE);
    CHECK(fcomp, FAIL, "SDstart");

    nt = DFNT_INT32;
    dimsize[0] = 5;
    dimsize[1] = 5;
    newsds = SDcreate(fcomp, "CompDataSet7", nt, 2, dimsize);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to create a new data set for compression testing\n");
        num_errs++;
    }

    for(i = 0; i < 25; i++)
        idata[i] = i*10;

    cinfo.deflate.level=6;
    status = SDsetcompress(newsds,COMP_CODE_DEFLATE,&cinfo);
    CHECK(status, FAIL, "SDcompress");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDwritedata(newsds, start, NULL, end, (VOIDP) idata);
    CHECK(status, FAIL, "SDwritedata");

    status = SDendaccess(newsds);
    CHECK(status, FAIL, "SDendaccess");

    /* need to close to flush compressed info to file */
    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /*
     * Read the compressed data back in.
     */
    fcomp = SDstart(COMPFILE7, DFACC_RDWR);
    CHECK(fcomp, FAIL, "SDstart (again)");

    newsds2 = SDselect(fcomp, 0);
    if(newsds == FAIL) {
        fprintf(stderr, "Failed to select a data set for compressed access\n");
        num_errs++;
    }

    /*
     * Retrieve and verify the compression info - bug# 307, 9/7/01 - BMR
     */
#ifndef H4_NO_DEPRECATED_SYMBOLS /* Jan 9, 2013 */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompress(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompress");
    VERIFY(comp_type, COMP_CODE_DEFLATE, "SDgetcompress");
    VERIFY(cinfo.deflate.level, 6, "SDgetcompress");
#endif /* H4_NO_DEPRECATED_SYMBOLS */

    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    HDmemset(&cinfo, 0, sizeof(cinfo)) ;
    status = SDgetcompinfo(newsds2, &comp_type, &cinfo);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_DEFLATE, "SDgetcompinfo");
    VERIFY(cinfo.deflate.level, 6, "SDgetcompinfo");

    /* Only get the compression method and verify it */
    comp_type = COMP_CODE_INVALID;  /* reset variables before retrieving info */
    status = SDgetcomptype(newsds2, &comp_type);
    CHECK(status, FAIL, "SDgetcompinfo");
    VERIFY(comp_type, COMP_CODE_DEFLATE, "SDgetcompinfo");

    start[0] = start[1] = 0;
    end[0]   = end[1]   = 5;
    status = SDreaddata(newsds2, start, NULL, end, (VOIDP) rdata);
    CHECK(status, FAIL, "SDreaddata");

    for(i = 0; i < 25; i++)
        if(idata[i] != rdata[i]) {
            fprintf(stderr,"Bogus val in loc %d in compressed dset want %ld got %ld\n", i, (long)idata[i], (long)rdata[i]);
            num_errs++;
        }

    status = SDendaccess(newsds2);
    CHECK(status, FAIL, "SDendaccess");

    status = SDend(fcomp);
    CHECK(status, FAIL, "SDend");

    /* Return the number of errors that's been kept track of so far */
    return num_errs;

}   /* end test_compressed_data */

extern int
test_compression()
{
    intn num_errs = 0;         /* number of errors */

    /* Output message about test being performed */
    TESTING("create/read/write compressed datasets (tcomp.c)");

    /* create various data sets with different compression methods */
    num_errs = num_errs + test_various_comps();

    /* test writing and reading data sets with compression */
    num_errs = num_errs + test_compressed_data();

    if (num_errs == 0)
        PASSED();

    return num_errs;
}

#endif /* HDF */
