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


#include "hdf.h"
#include "mfhdf.h"
#include "hcomp.h"
#include "hzip.h"
#include "hdiff.h"
#include "add.h"
#include "verify.h"

#define DATA_FILE1       "image8.txt"
#define DATA_FILE2       "image24pixel.txt"
char    *progname;     


/*-------------------------------------------------------------------------
 * Function: main
 *
 * Purpose: test program for hzip
 *
 * A)This program writes several HDF objects to the file FILENAME
 *   The image data consists of realistic data read from the files DATA_FILE1
 *   (8bit image) and DATA_FILE2 (24bit image)
 *  The objects written are
 *  1) groups
 *  2) images 
 *  3) datasets
 *  4) vdatas with attributes and field attributes
 *  5) global and local attributes
 *  6) labels and annotations
 *
 * B) Then several calls are made to hzip, in each call the FILENAME_OUT is 
 *  generated
 *
 * C) In each test the hdiff utility is called to compare the files 
 *  FILENAME and FILENAME_OUT
 *
 * D) In each test the verifiy_comp_chunk function is called to compare 
 *  the input and output compression and chunking parameters
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int main(void)
{
 intn          status_n;     /* returned status_n for functions returning an intn  */
 int32         status_32,    /* returned status_n for functions returning an int32 */
               vgroup_ref=-1,/* reference number of the group */
               vgroup1_id,   /* vgroup identifier */
               vgroup2_id,   /* vgroup identifier */
               vgroup3_id,   /* vgroup identifier */
               vgroup_img_id,/* vgroup identifier */
               vg_index,     /* position of a vgroup in the vgroup  */
               file_id;      /* HDF file identifier, same for V interface */
 options_t     options;      /* for hzip  */ 
 static struct fspec fspec;  /* for hdiff  */ 
 int           verbose=0;
 int32         attr_n_values = 3;  /* number of values in the vg attribute */
 char          vg_attr[3]    = {'A', 'B', 'C'};/* vg attribute values*/
 comp_coder_t  comp_type;    /* to retrieve compression type into */
 int32         chunk_flags;  /* Chunking flag */ 
 comp_info     comp_info;    /* compression structure */ 
 int32         in_chunk_lengths[MAX_VAR_DIMS];

 /* initialize options for hdiff */
 memset(&fspec,0,sizeof(struct fspec));
 fspec.ga = 1;    /* compare global attributes */
 fspec.sa = 1;    /* compare SD local attributes */
 fspec.sd = 1;    /* compare SD data */
 fspec.vd = 1;    /* compare Vdata */
 fspec.gr = 1;    /* compare GR data */

/*-------------------------------------------------------------------------
 * create a file with SDSs, images , groups and vdatas
 *-------------------------------------------------------------------------
 */
       
 /* create a HDF file */
 file_id = Hopen (FILENAME, DFACC_CREATE, 0);
  
 /* initialize the V interface */
 status_n = Vstart (file_id);


/*-------------------------------------------------------------------------
 * start
 *-------------------------------------------------------------------------
 */ 

 /* create the first vgroup.  the vgroup reference number is set
  * to -1 for creating and the access mode is "w" for writing
  */
 vgroup1_id = Vattach (file_id, vgroup_ref, "w");
 status_32  = Vsetname (vgroup1_id, "g1");

 /* attach an attribute to the vgroup */
 status_n = Vsetattr (vgroup1_id,"Myattr",DFNT_CHAR,attr_n_values,vg_attr);
 
 /* create the second vgroup */
 vgroup2_id = Vattach (file_id, vgroup_ref, "w");
 status_32  = Vsetname (vgroup2_id, "g2");

 /* create the 3rd vgroup */
 vgroup3_id = Vattach (file_id, vgroup_ref, "w");
 status_32  = Vsetname (vgroup3_id, "g3");

 /* insert the second vgroup into the first vgroup using its identifier */
 vg_index = Vinsert (vgroup1_id, vgroup2_id);

 /* insert the 3rd vgroup into the 2nd vgroup using its identifier */
 vg_index = Vinsert (vgroup2_id, vgroup3_id);

 /* create the 4th vgroup, for images */
 vgroup_img_id = Vattach (file_id, vgroup_ref, "w");
 status_32     = Vsetname (vgroup_img_id, "images");

/*-------------------------------------------------------------------------
 * add some SDS to the file
 * duplicates are inserted in the groups "g1", "g2", "g3" and root
 *-------------------------------------------------------------------------
 */ 

 /* add non chunked, non compressed sds */
 chunk_flags = HDF_NONE;
 comp_type   = COMP_CODE_NONE;
 add_sd(FILENAME,"dset1",vgroup1_id,chunk_flags,comp_type,NULL);
 add_sd(FILENAME,"dset2",vgroup2_id,chunk_flags,comp_type,NULL);
 add_sd(FILENAME,"dset3",vgroup3_id,chunk_flags,comp_type,NULL);
 add_sd(FILENAME,"dset4",0,chunk_flags,comp_type,NULL);
 add_sd(FILENAME,"dset5",0,chunk_flags,comp_type,NULL);
 add_sd(FILENAME,"dset6",0,chunk_flags,comp_type,NULL);
 add_sd3d(FILENAME,"dset7",0,chunk_flags,comp_type,NULL);


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
 add_sd(FILENAME,"dset_chunk",0,chunk_flags,comp_type,NULL);

 /* add a chunked-compressed sds with SDsetchunk */
 chunk_flags = HDF_CHUNK | HDF_COMP;
 comp_type   = COMP_CODE_NONE;
 add_sd(FILENAME,"dset_chunk_comp",0,chunk_flags,comp_type,NULL);

/* add a non chunked, compressed sds */
 chunk_flags = HDF_NONE;
 comp_type   = COMP_CODE_DEFLATE;
 comp_info.deflate.level = 6;
 add_sd(FILENAME,"dset_comp",0,chunk_flags,comp_type,&comp_info);

 
/*-------------------------------------------------------------------------
 * add some RIS24 images to the file
 *-------------------------------------------------------------------------
 */ 

 add_r24(FILENAME,DATA_FILE2,vgroup_img_id);
 add_r24(FILENAME,DATA_FILE2,0);

/*-------------------------------------------------------------------------
 * add some GR images to the file
 * duplicates are inserted in the groups "g1", "g2", "g3" and root
 *-------------------------------------------------------------------------
 */ 

 add_gr(DATA_FILE1,"gr1",file_id,vgroup1_id);
 add_gr(DATA_FILE2,"gr2",file_id,vgroup2_id);
 add_gr(DATA_FILE1,"gr3",file_id,vgroup3_id);
 add_gr(DATA_FILE2,"gr4",file_id,0);

/*-------------------------------------------------------------------------
 * add some RIS8 images to the file
 *-------------------------------------------------------------------------
 */ 
 add_r8(FILENAME,DATA_FILE1,vgroup_img_id);
 add_r8(FILENAME,DATA_FILE1,0);


/*-------------------------------------------------------------------------
 * add some VS to the file
 * duplicates are inserted in the groups "g1", "g2", "g3" and root
 *-------------------------------------------------------------------------
 */ 

 add_vs("vdata1",file_id,vgroup1_id);
 add_vs("vdata2",file_id,vgroup2_id);
 add_vs("vdata3",file_id,vgroup3_id);
 add_vs("vdata4",file_id,0);

/*-------------------------------------------------------------------------
 * add some global attributes to the file
 *-------------------------------------------------------------------------
 */ 
 add_glb_attrs(FILENAME,file_id);

/*-------------------------------------------------------------------------
 * add annotations to the file
 *-------------------------------------------------------------------------
 */ 
 add_an(file_id);

/*-------------------------------------------------------------------------
 * end
 *-------------------------------------------------------------------------
 */

 /* terminate access to the vgroups */
 status_32 = Vdetach (vgroup1_id);
 status_32 = Vdetach (vgroup2_id);
 status_32 = Vdetach (vgroup3_id);
 status_32 = Vdetach (vgroup_img_id);
 
 /* terminate access to the V interface */
 status_n = Vend (file_id);

 /* close the HDF file */
 status_n = Hclose (file_id);


/*-------------------------------------------------------------------------
 * TESTS:
 * 1) zip FILENAME with some compression/chunking options
 * 2) use the hdiff utility to compare the input and output file; it returns RET==0
 *    if the high-level objects have the same data
 * 3) use the API functions SD(GR)getcompress, SD(GR)getchunk to verify
 *    the compression/chunking input on the otput file
 *-------------------------------------------------------------------------
 */

#if defined (HZIPTST_DEBUG)
 verbose        =1;
 fspec.verbose  =1;
#endif

 in_chunk_lengths[0]=in_chunk_lengths[1]=in_chunk_lengths[2]=10;


/*-------------------------------------------------------------------------
 * test1:  
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with HUFF, chunking SELECTED");
 hzip_init (&options,verbose);
 hzip_addcomp("dset7:HUFF 1",&options);
 hzip_addchunk("dset7:10x10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp("dset7",COMP_CODE_SKPHUFF, 1) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset7",HDF_CHUNK|HDF_COMP,3,in_chunk_lengths) == -1) 
  goto out;
 PASSED();


/*-------------------------------------------------------------------------
 * test2:  
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with RLE, chunking SELECTED");
 hzip_init (&options,verbose);
 hzip_addcomp("dset4:RLE",&options);
 hzip_addchunk("dset4:10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp("dset4",COMP_CODE_RLE, 0) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
  goto out;
 PASSED();

/*-------------------------------------------------------------------------
 * test3:  
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with GZIP, chunking SELECTED");
 hzip_init (&options,verbose);
 hzip_addcomp("dset4:GZIP 6",&options);
 hzip_addchunk("dset4:10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 6) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
  goto out;
 PASSED();


/*-------------------------------------------------------------------------
 * test4:  
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with NONE, chunking SELECTED NONE");
 hzip_init (&options,verbose);
 hzip_addcomp("dset_chunk_comp:NONE",&options);
 hzip_addcomp("dset_chunk:NONE",&options);
 hzip_addchunk("dset_chunk_comp:NONE",&options);
 hzip_addchunk("dset_chunk:NONE",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
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
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with all types, chunking SELECTED");
 hzip_init (&options,verbose);
 hzip_addcomp("dset4:GZIP 9",&options);
 hzip_addcomp("dset5:RLE",&options);
 hzip_addcomp("dset6:HUFF 2",&options);
 hzip_addchunk("dset4:10x10",&options);
 hzip_addchunk("dset5:10x10",&options);
 hzip_addchunk("dset6:10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1) 
  goto out;
 if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1) 
  goto out;
 if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset5",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
  goto out;
 if ( sds_verifiy_chunk("dset6",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1) 
  goto out;
 PASSED();

/*-------------------------------------------------------------------------
 * test6:  
 *-------------------------------------------------------------------------
 */
 TESTING("compressing SDS SELECTED with all types, no chunking");
 hzip_init (&options,verbose);
 hzip_addcomp("dset4:GZIP 9",&options);
 hzip_addcomp("dset5:RLE",&options);
 hzip_addcomp("dset6:HUFF 2",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1) 
  goto out;
 if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1) 
  goto out;
 if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1) 
  goto out;
 PASSED();

/*-------------------------------------------------------------------------
 * test7:  
 *-------------------------------------------------------------------------
 */

 TESTING("compressing SDS ALL, chunking SELECTED NONE");
 hzip_init (&options,verbose);
 hzip_addcomp("*:GZIP 1",&options);
 hzip_addchunk("dset_chunk_comp:NONE",&options);
 hzip_addchunk("dset_chunk:NONE",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
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
 *-------------------------------------------------------------------------
 */

 TESTING("no compressing, chunking ALL");
 hzip_init (&options,verbose);
 hzip_addchunk("*:10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_chunk_all(HDF_CHUNK,2,in_chunk_lengths,"dset7") == -1) 
  goto out;
 PASSED();

/*-------------------------------------------------------------------------
 * test9:  
 *-------------------------------------------------------------------------
 */

 verbose        =1;
 fspec.verbose  =1;

 TESTING("compressing SDS ALL with GZIP, chunking ALL");
 printf("\n");
 hzip_init (&options,verbose);
 hzip_addcomp("*:GZIP 1",&options);
 hzip_addchunk("*:10x10",&options);
 hzip(FILENAME,FILENAME_OUT,&options);
 hzip_end (&options);
 if (hdiff(FILENAME,FILENAME_OUT,fspec) == 1)
  goto out;
 if ( sds_verifiy_comp_all(COMP_CODE_DEFLATE, 1) == -1) 
  goto out;
 if ( sds_verifiy_chunk_all(HDF_CHUNK|HDF_COMP,2,in_chunk_lengths,"dset7") == -1) 
  goto out;
 PASSED();
 
/*-------------------------------------------------------------------------
 * all tests PASSED
 *-------------------------------------------------------------------------
 */
 
 return 0;
out:
 H4_FAILED();
 return 1;
}

