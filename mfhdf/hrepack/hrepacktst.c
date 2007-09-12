/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "hdf.h"
#include "mfhdf.h"
#include "hcomp.h"
#include "hrepack.h"
#include "hdiff.h"
#include "hrepacktst.h"

#if defined (H4_HAVE_LIBSZ)
#include "szlib.h"
#endif

char    *progname;   

/*-------------------------------------------------------------------------
 * prototypes
 *-------------------------------------------------------------------------
 */ 

static int test_files();

/*-------------------------------------------------------------------------
 * Function: main
 *
 * Purpose: test program for hrepack
 *
 * A)This program writes several HDF objects to the file HREPACK_FILE1
 *   The image data consists of realistic data read from the files DATA_FILE1
 *   (8bit image) , DATA_FILE2 (24bit image, pixel interlace) and
 *    DATA_FILE3 (24bit image, plane interlace)
 *  The objects written are
 *  1) groups
 *  2) images 
 *  3) datasets
 *  4) vdatas with attributes and field attributes
 *  5) global and local attributes
 *  6) labels and annotations
 *
 * B) Then several calls are made to hrepack, in each call the HREPACK_FILE1_OUT is 
 *  generated
 *
 * C) In each test the hdiff utility is called to compare the files 
 *  HREPACK_FILE1 and HREPACK_FILE1_OUT
 *
 * D) In each test the verifiy_comp_chunk function is called to compare 
 *  the input and output compression and chunking parameters
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 * Modifications: 
 *  July 7, 2007. Add test for hyperslab repacking in HREPACK_FILE2
 *  September 12, 2007. Add test for duplicate vgroup insertions in HREPACK_FILE3
 *
 *-------------------------------------------------------------------------
 */


int main(void)
{

    if ( generate_files() < 0 )
        return 1;

    if ( test_files() < 0 )
        return 1;


    return 0;

}


/*-------------------------------------------------------------------------
 * Function: test_files
 *
 * Purpose: call hrepack and hdiff on the generated files
 * 1) zip HREPACK_FILE1 with some compression/chunking options
 * 2) use the hdiff utility to compare the input and output file; it returns RET==0
 *    if the high-level objects have the same data
 * 3) use the API functions SD(GR)getcompress, SD(GR)getchunk to verify
 *    the compression/chunking input on the output file
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

static
int test_files(void)
{
    options_t     options;      /* for hrepack  */ 
    diff_opt_t    fspec;        /* for hdiff  */ 
    int           verbose=0;
    int32         in_chunk_lengths[H4_MAX_VAR_DIMS];
    
    /* initialize options for hdiff */
    memset(&fspec,0,sizeof(diff_opt_t));
    fspec.ga = 1;    /* compare global attributes */
    fspec.sa = 1;    /* compare SD local attributes */
    fspec.sd = 1;    /* compare SD data */
    fspec.vd = 1;    /* compare Vdata */
    fspec.gr = 1;    /* compare GR data */
    
    in_chunk_lengths[0]=10;
    in_chunk_lengths[1]=8;
    in_chunk_lengths[2]=6;
    
    
   /*-------------------------------------------------------------------------
    * test1:  
    * HUFF
    *-------------------------------------------------------------------------
    */
    
    
    TESTING("hrepack -t dset7:HUFF 1 -c dset7:10x8x6");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset7:HUFF 1",&options);
    hrepack_addchunk("dset7:10x8x6",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0 )
        goto out;
    if ( sds_verifiy_comp("dset7",COMP_CODE_SKPHUFF, 1) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset7",HDF_CHUNK|HDF_COMP,3,in_chunk_lengths) == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test2: 
    * RLE
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset4:RLE -c dset4:10x8");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset4:RLE",&options);
    hrepack_addchunk("dset4:10x8",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0 )
        goto out;
    if ( sds_verifiy_comp("dset4",COMP_CODE_RLE, 0) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
        goto out;
    PASSED();
    
   /*-------------------------------------------------------------------------
    * test3:  
    * SDS SELECTED with GZIP, chunking SELECTED
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset4:GZIP 6 -c dset4:10x8");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset4:GZIP 6",&options);
    hrepack_addchunk("dset4:10x8",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0 )
        goto out;
    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 6) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test4:  
    * SDS SELECTED with SZIP, chunking SELECTED
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset4:SZIP 8,EC -c dset4:10x8");
#if defined (H4_HAVE_LIBSZ)
    if (SZ_encoder_enabled()) 
    {
        hrepack_init (&options,verbose);
        hrepack_addcomp("dset4:SZIP 8,EC",&options);
        hrepack_addchunk("dset4:10x8",&options);
        if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
            goto out;
        hrepack_end (&options);
        PASSED();
        
        TESTING(HDIFF_TSTSTR);
        if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
            goto out;
        if ( sds_verifiy_comp("dset4",COMP_CODE_SZIP, 0) == -1) 
            goto out;
        if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
            goto out;
        PASSED();
    } else 
    {
        /* no szip encoder */
        SKIPPED();
    }
    
#else
    /* no szip at all */
    SKIPPED();
#endif
    
   /*-------------------------------------------------------------------------
    * test4:  
    * SDS SELECTED with NONE, chunking SELECTED NONE
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset_chunk:NONE -c dset_chunk:NONE");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset_chunk_comp:NONE",&options);
    hrepack_addcomp("dset_chunk:NONE",&options);
    hrepack_addchunk("dset_chunk_comp:NONE",&options);
    hrepack_addchunk("dset_chunk:NONE",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_comp("dset_chunk_comp",COMP_CODE_NONE, 0) == -1) 
        goto out;
    if ( sds_verifiy_comp("dset_chunk",COMP_CODE_NONE, 0) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset_chunk_comp",HDF_NONE,0,0) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset_chunk",HDF_NONE,0,0) == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test5:
    * SDS SELECTED with all types, chunking SELECTED
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset4:GZIP 9 -t dset5:RLE -c dset4:10x8");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset4:GZIP 9",&options);
    hrepack_addcomp("dset5:RLE",&options);
    hrepack_addcomp("dset6:HUFF 2",&options);
#if defined (H4_HAVE_LIBSZ)
    if (SZ_encoder_enabled()) {
        hrepack_addcomp("dset7:SZIP 8,EC",&options);
    }
#endif
    hrepack_addchunk("dset4:10x8",&options);
    hrepack_addchunk("dset5:10x8",&options);
    hrepack_addchunk("dset6:10x8",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1) 
        goto out;
    if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1) 
        goto out;
    if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1) 
        goto out;
#if defined (H4_HAVE_LIBSZ)
    if (SZ_encoder_enabled()) {
        if ( sds_verifiy_comp("dset7",COMP_CODE_SZIP, 0) == -1) 
            goto out;
    }
#endif
    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset5",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset6",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test6: 
    * SDS SELECTED with all types, no chunking
    *-------------------------------------------------------------------------
    */
    TESTING("hrepack -t dset4:GZIP 9 -t dset5:RLE -t dset6:HUFF 2");
    hrepack_init (&options,verbose);
    hrepack_addcomp("dset4:GZIP 9",&options);
    hrepack_addcomp("dset5:RLE",&options);
    hrepack_addcomp("dset6:HUFF 2",&options);
#if defined (H4_HAVE_LIBSZ)
    if (SZ_encoder_enabled()) {
        hrepack_addcomp("dset7:SZIP 4,EC",&options);
    }
#endif
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1) 
        goto out;
    if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1) 
        goto out;
    if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1) 
        goto out;
#if defined (H4_HAVE_LIBSZ)
    if (SZ_encoder_enabled()) {
        if ( sds_verifiy_comp("dset7",COMP_CODE_SZIP, 0) == -1) 
            goto out;
    }
#endif
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test7:  
    * compressing SDS ALL, chunking SELECTED NONE
    *-------------------------------------------------------------------------
    */
    
    TESTING("hrepack -t *:GZIP 1 -c dset_chunk:NONE");
    hrepack_init (&options,verbose);
    hrepack_addcomp("*:GZIP 1",&options);
    hrepack_addchunk("dset_chunk_comp:NONE",&options);
    hrepack_addchunk("dset_chunk:NONE",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_comp_all(COMP_CODE_DEFLATE, 1) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset_chunk_comp",HDF_NONE,0,0) == -1) 
        goto out;
    if ( sds_verifiy_chunk("dset_chunk",HDF_NONE,0,0) == -1) 
        goto out;
    PASSED();
    
   /*-------------------------------------------------------------------------
    * test8:
    * no compressing, chunking ALL
    *-------------------------------------------------------------------------
    */
    
    TESTING("hrepack -c *:10x8");
    hrepack_init (&options,verbose);
    hrepack_addchunk("*:10x8",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_chunk_all(HDF_CHUNK,2,in_chunk_lengths,"dset7") == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test9: 
    * compressing SDS ALL with GZIP
    *-------------------------------------------------------------------------
    */
    
    TESTING("hrepack -t *:GZIP 1");
    hrepack_init (&options,verbose);
    hrepack_addcomp("*:GZIP 1",&options);
    if (hrepack(HREPACK_FILE1,HREPACK_FILE1_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR);
    if (hdiff(HREPACK_FILE1,HREPACK_FILE1_OUT,&fspec) > 0)
        goto out;
    if ( sds_verifiy_comp_all(COMP_CODE_DEFLATE, 1) == -1) 
        goto out;
    PASSED();
    
    
   /*-------------------------------------------------------------------------
    * test10: 
    * repack a big file using hyperslab reading/writing
    *-------------------------------------------------------------------------
    */
    
    TESTING("hyperslab repacking");
    hrepack_init (&options,verbose);
    if (hrepack(HREPACK_FILE2,HREPACK_FILE2_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR2);
    if (hdiff(HREPACK_FILE2,HREPACK_FILE2_OUT,&fspec) > 0)
        goto out;
    PASSED();

   /*-------------------------------------------------------------------------
    * test11: 
    * repack a file with vgroups
    *-------------------------------------------------------------------------
    */
    
    TESTING("vgroups dependencies");
    hrepack_init (&options,verbose);
    if (hrepack(HREPACK_FILE3,HREPACK_FILE3_OUT,&options)<0)
        goto out;
    hrepack_end (&options);
    PASSED();
    
    TESTING(HDIFF_TSTSTR3);
    if (hdiff(HREPACK_FILE3,HREPACK_FILE3_OUT,&fspec) > 0)
        goto out;
    if (vg_verifygrpdep(HREPACK_FILE3,HREPACK_FILE3_OUT) != 0 )
        goto out;
    PASSED();

   
       
    return SUCCEED;
out:
    H4_FAILED();
    return FAIL;
    
}



