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
#include "test_hrepack_add.h"
#include "test_hrepack_verify.h"
#if defined (H4_HAVE_LIBSZ)
#include "szlib.h"
#endif

#define DATA_FILE1       "image8.txt"
#define DATA_FILE2       "image24pixel.txt"
#define DATA_FILE3       "image24plane.txt"
char    *progname;   

#define HDIFF_TSTSTR     "hdiff hrepacktest.hdf hrepacktest_out.hdf"
#define HDIFF_TSTSTR2    "hdiff hrepacktest2.hdf hrepacktest2_out.hdf"

/*-------------------------------------------------------------------------
 * prototypes
 *-------------------------------------------------------------------------
 */ 

static int generate_files();
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
 * Modifications: July 7, 2007
 *  Add test for hyperslab repacking in HREPACK_FILE2
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
 * Function: generate_files
 *
 * Purpose: writes several HDF objects to the files 
 *  HREPACK_FILE1
 *  HREPACK_FILE2
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

static
int generate_files(void)
{
    int32         vgroup1_id,   /* vgroup identifier */
                  vgroup2_id,   /* vgroup identifier */
                  vgroup3_id,   /* vgroup identifier */
                  vgroup_img_id,/* vgroup identifier */
                  file_id,      /* HDF file identifier, same for V interface */
                  gr_id,        /* GR interface identifier */
                  sd_id;        /* SD interface identifier */
    int32         attr_n_values = 3;  /* number of values in the vg attribute */
    char          vg_attr[3]    = {'A', 'B', 'C'};/* vg attribute values*/
    comp_coder_t  comp_type;    /* to retrieve compression type into */
    int32         chunk_flags;  /* Chunking flag */ 
    comp_info     comp_info;    /* compression structure */ 

   /*-------------------------------------------------------------------------
    * create a file with SDSs, images , groups and vdatas
    *-------------------------------------------------------------------------
    */

    
    /* create a HDF file */
    if ((file_id = Hopen (HREPACK_FILE1, DFACC_CREATE, (int16)0))<0)
    {
        printf("Error: Could not create file <%s>\n",HREPACK_FILE1);
        return FAIL;
    }
    
    /* initialize the SD interface */
    if ((sd_id = SDstart (HREPACK_FILE1, DFACC_RDWR))== FAIL)
    {
        printf("Error: Could not start SD interface\n");
        return FAIL;
    }
    
    /* initialize the GR interface */
    if ((gr_id = GRstart (file_id))== FAIL)
    {
        printf("Error: Could not start GR interface\n");
        return FAIL;
    }
    
    /* initialize the V interface */
    if (Vstart (file_id)==FAIL)
    {
        printf( "Could not start VG\n");
        return FAIL;
    }
    
    
   /*-------------------------------------------------------------------------
    * create groups
    *-------------------------------------------------------------------------
    */ 

    TESTING("generating groups");
    
    vgroup1_id = Vattach (file_id, -1, "w");
    if (Vsetname (vgroup1_id, "g1")==FAIL)
    {
        printf( "Could not name group\n");
        goto out;
    }
    
    /* attach an attribute to the vgroup */
    if (Vsetattr (vgroup1_id,"Myattr",DFNT_CHAR,attr_n_values,vg_attr)==FAIL)
    {
        printf( "Could set group attributes\n");
        goto out;
    }
    
    /* create the second vgroup */
    vgroup2_id = Vattach (file_id, -1, "w");
    if (Vsetname (vgroup2_id, "g2")==FAIL)
    {
        printf( "Could not name group\n");
        goto out;
    }
    
    /* create the 3rd vgroup */
    vgroup3_id = Vattach (file_id, -1, "w");
    if (Vsetname (vgroup3_id, "g3")==FAIL)
    {
        printf( "Could not name group\n");
        goto out;
    }
    
    /* insert the second vgroup into the first vgroup using its identifier */
    if (Vinsert (vgroup1_id, vgroup2_id)==FAIL)
    {
        printf( "Could not insert VG\n");
        goto out;
    }
    
    /* insert the 3rd vgroup into the 2nd vgroup using its identifier */
    if (Vinsert (vgroup2_id, vgroup3_id)==FAIL)
    {
        printf( "Could not insert VG\n");
        goto out;
    }
    
    /* create the 4th vgroup, for images */
    vgroup_img_id = Vattach (file_id, -1, "w");
    if (Vsetname (vgroup_img_id, "images")==FAIL)
    {
        printf( "Could not name group\n");
        goto out;
    }

    PASSED();
    
   /*-------------------------------------------------------------------------
    * add some SDSs to the file
    * duplicates are inserted in the groups "g1", "g2", "g3" and root
    *-------------------------------------------------------------------------
    */ 

    TESTING("generating SDSs");

    
    /* add non chunked, non compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_NONE;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset1",vgroup1_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset2",vgroup2_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset3",vgroup3_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset4",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset5",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset6",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd3d(HREPACK_FILE1,file_id,sd_id,"dset7",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    
    
   /*-------------------------------------------------------------------------
    * add some chunked/compressd SDS to the file
    * Chunked                  -> flags = HDF_CHUNK
    * Chunked and compressed   -> flags = HDF_CHUNK | HDF_COMP 
    * Non-chunked              -> flags = HDF_NONE
    *-------------------------------------------------------------------------
    */ 
    
    /* add a chunked, non compressed sds */
    chunk_flags = HDF_CHUNK;
    comp_type   = COMP_CODE_NONE;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_chunk",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    
    /* add a chunked-compressed sds with SDsetchunk */
    chunk_flags = HDF_CHUNK | HDF_COMP;
    comp_type   = COMP_CODE_DEFLATE;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_chunk_comp",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * GZIP
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_DEFLATE;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_gzip",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * add an empty sds
    *-------------------------------------------------------------------------
    */ 
    if (add_empty_sd(sd_id,"dset_empty")<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * RLE
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_RLE;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_rle",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * HUFF
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_SKPHUFF;
    if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_huff",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
#if defined (H4_HAVE_LIBSZ)
   /*-------------------------------------------------------------------------
    * SZIP
    *-------------------------------------------------------------------------
    */ 
    if (SZ_encoder_enabled()) 
    {
        chunk_flags = HDF_NONE;
        comp_type   = COMP_CODE_SZIP;
        if (add_sd(HREPACK_FILE1,file_id,sd_id,"dset_szip",0,chunk_flags,comp_type,&comp_info)<0)
            goto out;
        
        if (add_sd_szip_all(HREPACK_FILE1,file_id,sd_id,0)<0)
            goto out;
    }
    
#endif

    PASSED();

    TESTING("generating images");

    
   /*-------------------------------------------------------------------------
    * add some RIS24 images to the file
    *-------------------------------------------------------------------------
    */
    /* Pixel Interlacing */
    if (add_r24(DATA_FILE2,HREPACK_FILE1,file_id,DFIL_PIXEL,vgroup_img_id)<0)
        goto out;
    /* Scan Plane Interlacing */
    if (add_r24(DATA_FILE3,HREPACK_FILE1,file_id,DFIL_PLANE,vgroup_img_id)<0)
        goto out;  
    
    
   /*-------------------------------------------------------------------------
    * add some RIS8 images to the file
    *-------------------------------------------------------------------------
    */ 
    if (add_r8(DATA_FILE1,HREPACK_FILE1,file_id,vgroup_img_id)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * add some GR images to the file with compression/chunking
    *-------------------------------------------------------------------------
    */ 
    
   /*-------------------------------------------------------------------------
    * no compression
    *-------------------------------------------------------------------------
    */ 
    
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_NONE;
    if (add_gr("gr_none",file_id,gr_id,0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
    
   /*-------------------------------------------------------------------------
    * GZIP
    *-------------------------------------------------------------------------
    */ 
    
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_DEFLATE;
    if (add_gr("gr_gzip",file_id,gr_id,0,chunk_flags,comp_type,&comp_info)<0)
        goto out; 
    
#if defined (H4_GR_SZIP)
   /* not supported for GR */
   /*-------------------------------------------------------------------------
    * SZIP
    *-------------------------------------------------------------------------
    */ 
    
#if defined (H4_HAVE_LIBSZ)
    
    if (SZ_encoder_enabled()) 
    {
        chunk_flags = HDF_NONE;
        comp_type   = COMP_CODE_SZIP;
        if (add_gr("gr_szip",file_id,gr_id,0,chunk_flags,comp_type,&comp_info)<0)
            goto out; 
    }
    
#endif
#endif
    
   /*-------------------------------------------------------------------------
    * add some GR realistic images to the file
    * realistic data is read from ASCII files
    *-------------------------------------------------------------------------
    */ 
    
    if (add_gr_ffile(DATA_FILE1,gr_id,"gr_8bit",0,file_id,0)<0)
        goto out;  
    if (add_gr_ffile(DATA_FILE2,gr_id,"gr_24bit",0,file_id,0)<0)
        goto out;  
    
   /*-------------------------------------------------------------------------
    * add some VS to the file
    * duplicates are inserted in the groups "g1", "g2", "g3" and root
    *-------------------------------------------------------------------------
    */ 

    PASSED();

    TESTING("generating Vdatas");

    
    if (add_vs("vdata1",file_id,vgroup1_id)<0)
        goto out; 
    if (add_vs("vdata2",file_id,vgroup2_id)<0)
        goto out; 
    if (add_vs("vdata3",file_id,vgroup3_id)<0)
        goto out;
    if (add_vs("vdata4",file_id,0)<0)
        goto out; 

    PASSED();

    TESTING("generating global attributes");

    
   /*-------------------------------------------------------------------------
    * add some global attributes to the file
    *-------------------------------------------------------------------------
    */ 
    if (add_glb_attrs(HREPACK_FILE1,file_id,sd_id,gr_id)<0)
        goto out;


    PASSED();

    TESTING("generating annotations");

    
   /*-------------------------------------------------------------------------
    * add annotations to the file
    *-------------------------------------------------------------------------
    */ 
    if (add_file_an(file_id)<0)
        goto out; 
    
   /*-------------------------------------------------------------------------
    * add a palette to the file
    *-------------------------------------------------------------------------
    */ 

    PASSED();


    TESTING("generating palettes");
    

    if (add_pal(HREPACK_FILE1)<0)
        goto out;

    PASSED();
    
   /*-------------------------------------------------------------------------
    * generate a big file for hyperslab reading
    *-------------------------------------------------------------------------
    */ 

    TESTING("generating big file");

    if (do_big_file(HREPACK_FILE2)<0)
        goto out;

    
   /*-------------------------------------------------------------------------
    * close
    *-------------------------------------------------------------------------
    */
    
    /* terminate access to the vgroups */
    if (Vdetach (vgroup1_id)==FAIL ||
        Vdetach (vgroup2_id)==FAIL ||
        Vdetach (vgroup3_id)==FAIL ||
        Vdetach (vgroup_img_id)==FAIL)
    {
        printf( "Could not close group\n");
        goto out;
    }
    
    /* terminate access to the V interface */
    if (Vend (file_id)==FAIL)
    {
        printf( "Could not end VG\n");
        goto out;
    }
    
    /* terminate access to the GR interface */
    if (GRend (gr_id)==FAIL)
    {
        printf("Error: Could not close GR interface\n");
        goto out;
    }
    /* terminate access to the SD interface */
    if (SDend (sd_id)==FAIL)
    {
        printf("Error: Could not close SD interface\n");
        goto out;
    }
    /* close the HDF file */
    if (Hclose (file_id)==FAIL)
    {
        printf( "Could not close file\n");
        return FAIL;
    }

    PASSED();

    

    return SUCCEED;

out:

    /* close interfaces */
    Vend (file_id);
    GRend (gr_id);
    SDend (sd_id);
    Hclose (file_id);
    return FAIL;
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
       
    return SUCCEED;
out:
    H4_FAILED();
    return FAIL;
    
}



