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


#include "hdiff.h"
#include "hdiff_list.h"
#include "hdiff_mattbl.h"


/*-------------------------------------------------------------------------
 * Function: diff_gr
 *
 * Purpose: diff for GR
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 27, 2003
 *
 *-------------------------------------------------------------------------
 */

uint32 diff_gr( int32 gr1_id,              
                int32 gr2_id,
                int32 ref1,              
                int32 ref2,
                diff_opt_t *opt)  
{
 int32 ri1_id=-1,              /* data set identifier */
       ri1_index,              /* index number of the data set */
       dtype1,                 /* GR data type */
       dimsizes1[H4_MAX_VAR_DIMS],/* dimensional size */
       nattrs1,                /* number of attributes */
       ncomps1,                /* number of components */
       interlace_mode1,        /* interlace mode of an image */ 
       ri2_id=-1,              /* data set identifier */
       ri2_index,              /* index number of the data set */
       dtype2,                 /* GR data type */
       dimsizes2[H4_MAX_VAR_DIMS],/* dimensional size */
       nattrs2,                /* number of attributes */
       ncomps2,                /* number of components */
       interlace_mode2,        /* interlace mode of an image */ 
       start[H4_MAX_VAR_DIMS],    /* read start */
       edges[H4_MAX_VAR_DIMS],    /* read edges */
       numtype,                /* number type */
       eltsz,                  /* element size */
       data_size;
 uint32 nelms;                 /* number of elements */ 
 char  gr1_name[H4_MAX_NC_NAME]; 
 char  gr2_name[H4_MAX_NC_NAME]; 
 int   dim_diff=0;             /* dimensions are different */
 VOIDP buf1=NULL;
 VOIDP buf2=NULL;
 uint32 max_err_cnt;
 int    i, cmp;
 uint32 nfound=0;
 int    compare = 1;



/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */
 
 ri1_index = GRreftoindex(gr1_id,(uint16)ref1);
 ri1_id    = GRselect(gr1_id,ri1_index);
   
 /*obtain name,rank,dimsizes,datatype and num of attributes of gr */
 if (GRgetiminfo(ri1_id,gr1_name,&ncomps1,&dtype1,&interlace_mode1,dimsizes1,&nattrs1)==FAIL) {
   printf( "Failed to get info for SDS ref <%ld>\n",ref1);
   goto out;
  }


/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */
 
 ri2_index = GRreftoindex(gr2_id,(uint16)ref2);
 ri2_id    = GRselect(gr2_id,ri2_index);
   
 /*obtain name,rank,dimsizes,datatype and num of attributes of gr */
 if (GRgetiminfo(ri2_id,gr2_name,&ncomps2,&dtype2,&interlace_mode2,dimsizes2,&nattrs2)==FAIL) {
   printf( "Failed to get info for SDS ref <%ld>\n",ref2);
   goto out;
  }

 
 if (opt->verbose)
     printf("Comparing <%s>\n",gr1_name); 


/*-------------------------------------------------------------------------
 * check for different type
 *-------------------------------------------------------------------------
 */
 
 if (dtype1 != dtype2) 
 {
  printf("Comparison not supported\n");
  printf("<%s> has datatype %ld, <%s> has datatype %ld ",gr1_name,dtype1,gr2_name,dtype2);
  compare = 0;
 }

/*-------------------------------------------------------------------------
 * check for the same rank
 *-------------------------------------------------------------------------
 */
 
 if ( ncomps1 != ncomps2 )
 {
  printf("Comparison not supported\n");
  printf("<%s> has %ld components\n", gr1_name, ncomps1);
  printf("\n" );
  printf("<%s> has %ld components\n", gr2_name, ncomps2);
  compare = 0;
 }

/*-------------------------------------------------------------------------
 * check for different dimensions
 *-------------------------------------------------------------------------
 */
 
 for ( i=0; i<2; i++) 
 {
  if ( dimsizes1[i] != dimsizes2[i] )
   dim_diff=1;
 }

/*-------------------------------------------------------------------------
 * dimensions
 *-------------------------------------------------------------------------
 */

 if (dim_diff==1)
 {
  printf("Comparison not supported\n");
  printf("<%s> has dimensions ", gr1_name);
  print_dims(2,dimsizes1);
  printf("\n" );
  printf("<%s> has dimensions ", gr2_name);
  print_dims(2,dimsizes2);
  compare = 0;
 }

/*-------------------------------------------------------------------------
 * match interlace 
 * NOTE: GR images are always stored as pixel_interlace (0) on disk
 *       that does not happen with images saved with the 
 *       DF24 - Single-file 24-Bit Raster Image Interface,
 *       where the interlace mode on disk can be 0, 1 or 2
 *-------------------------------------------------------------------------
 */
 if ( interlace_mode1 != interlace_mode2 )
 {
  if (opt->verbose)
  printf("Warning: different interlace mode: <%ld> and <%ld>\n", 
   interlace_mode1,interlace_mode2);
  interlace_mode1=interlace_mode2;
 }


/*-------------------------------------------------------------------------
 * get size 
 *-------------------------------------------------------------------------
 */

 /* compute the number of the bytes for each value. */
 numtype = dtype1 & DFNT_MASK;
 eltsz = DFKNTsize(numtype | DFNT_NATIVE);

 /* set edges of SDS */
 nelms=1;
 for (i = 0; i < 2; i++) {
  nelms   *= dimsizes1[i];
  edges[i] = dimsizes1[i];
  start[i] = 0;
 }

 data_size = dimsizes1[0]*dimsizes1[1]*ncomps1*eltsz;

 if (compare)
 {
     
    /*-------------------------------------------------------------------------
     * read image 1
     *-------------------------------------------------------------------------
     */
     
     /* alloc */
     if ((buf1 = (VOIDP) HDmalloc(data_size)) == NULL) {
         printf( "Failed to allocate %lu elements of size %ld\n", nelms, eltsz);
         goto out;
     }
     
     /* set the interlace for reading  */
     if ( GRreqimageil(ri1_id, interlace_mode1) == FAIL ){
         printf( "Could not set interlace for GR <%s>\n", gr1_name);
         goto out;
     }
     
     /* read data */
     if (GRreadimage (ri1_id, start, NULL, edges, buf1) == FAIL) {
         printf( "Could not read GR <%s>\n", gr1_name);
         goto out;
     }
     
    /*-------------------------------------------------------------------------
     * read image 2
     *-------------------------------------------------------------------------
     */
     
     /* alloc */
     if ((buf2 = (VOIDP) HDmalloc(data_size)) == NULL) {
         printf( "Failed to allocate %lu elements of size %ld\n", nelms, eltsz);
         goto out;
     }
     
     /* set the interlace for reading  */
     if ( GRreqimageil(ri2_id, interlace_mode2) == FAIL ){
         printf( "Could not set interlace for GR <%s>\n", gr2_name);
         goto out;
     }
     
     /* read data */
     if (GRreadimage (ri2_id, start, NULL, edges, buf2) == FAIL) {
         printf( "Could not read GR <%s>\n", gr2_name);
         goto out;
     }
     
     
    /*-------------------------------------------------------------------------
     * comparing
     *-------------------------------------------------------------------------
     */
     
         
     cmp = HDmemcmp(buf1,buf2,data_size);
     if (cmp!=0)
     {
     /* if the given max_err_cnt is set (i.e. not its default MAX_DIFF),
	use it, otherwise, use the total number of elements in the dataset */
         max_err_cnt = (opt->max_err_cnt != MAX_DIFF) ? opt->max_err_cnt : nelms;
         nfound=array_diff(
             buf1, 
             buf2, 
             nelms, 
             gr1_name,
             gr2_name,
             2,
             dimsizes1,
             dtype1, 
             opt->err_limit, 
             opt->err_rel,
             max_err_cnt, 
             opt->statistics, 
             0, 
             0);
     }
     
 } /* compare */
  
/*-------------------------------------------------------------------------
 * close
 *-------------------------------------------------------------------------
 */


 GRendaccess(ri1_id);
 GRendaccess(ri2_id);
 if (buf1) HDfree(buf1);
 if (buf2) HDfree(buf2);


 return nfound;



out:

 opt->err_stat = 1;

 if (ri1_id!=-1)
    GRendaccess(ri1_id);
 if (ri2_id!=-1)
    GRendaccess(ri2_id);
 if (buf1) HDfree(buf1);
 if (buf2) HDfree(buf2);

 return 0;
}

