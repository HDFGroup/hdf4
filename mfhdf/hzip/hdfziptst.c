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
#include "hzip.h"
#include "hdiff.h"
#include "add.h"



#define FILENAME         "hziptst1.hdf"
#define FILENAME_OUT     "hziptst1_out.hdf"
#define DATA_FILE1        "image8.txt"
#define DATA_FILE2        "image24pixel.txt"
char    *progname;		     	/* for error messages */



int main(void)
{
	
	intn   status_n;     /* returned status_n for functions returning an intn  */
 int32  status_32,    /* returned status_n for functions returning an int32 */
								vgroup_ref=-1,/* reference number of the group */
        vgroup1_id,   /* vgroup identifier */
								vgroup2_id,   /* vgroup identifier */
								vgroup3_id,   /* vgroup identifier */
								vgroup_img_id,/* vgroup identifier */
								vg_index,     /* position of a vgroup in the vgroup  */
        file_id,      /* HDF file identifier, same for V interface */
	       chunk_flags,  /* Chunking flag */ 
	       comp_type;    /* compression flag */
	comp_info     comp_info;    /* compression structure */ 
	options_t 	   options;      /* for hzip  */ 
	static struct fspec fspec;  /* for hdiff  */ 
	int           ret;

	/* initialize options for hdiff */
	memset(&fspec,0,sizeof(struct fspec));

	/* initialize options, -t table and -c table  */
	hzip_init (&options);

/*-------------------------------------------------------------------------
 * shut up the hzip verbose mode
 *-------------------------------------------------------------------------
 */
		options.verbose=0;

/*-------------------------------------------------------------------------
 * create FILENAME with SDSs, images , groups and vdatas
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
 * check; we call the hdiff utility to compare the input and output file
 *-------------------------------------------------------------------------
 */

	TESTING("copy all objects with no compression / no chunking");
	
	/* with no options table this just copies all high-level objects*/
 hzip(FILENAME,FILENAME_OUT,&options);

	ret=hdiff(FILENAME,FILENAME_OUT,fspec);
	if (ret==0) PASSED() else H5_FAILED();

	TESTING("compress selected objects / no chunking");
	
	/* compress -t "dset4:GZIP 6" */
	hzip_addcomp("dset4:GIP 6",&options);
 hzip(FILENAME,FILENAME_OUT,&options);

	ret=hdiff(FILENAME,FILENAME_OUT,fspec);
	if (ret==0) PASSED() else H5_FAILED();

	/* free tables */
 hzip_end(&options);
	
	return 0;
}

