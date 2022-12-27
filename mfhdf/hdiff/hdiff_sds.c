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

#include <assert.h>
#include "hdiff.h"
#include "hdiff_list.h"
#include "hdiff_mattbl.h"

#define H4TOOLS_BUFSIZE    (1024 * 1024)
#define H4TOOLS_MALLOCSIZE (1024 * 1024)



static
uint32 diff_sds_attrs(int32 sds1_id,
                      int32 nattrs1,
                      int32 sds2_id,
                      int32 nattrs2,
                      char* sds1_name,
                      diff_opt_t * opt);

/*-------------------------------------------------------------------------
 * Function: diff_sds
 *
 * Purpose: diff for SDS
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 25, 2003
 *
 *-------------------------------------------------------------------------
 */

uint32 diff_sds(int32 sd1_id,              
                int32 sd2_id,
                int32 ref1,
                int32 ref2,
                diff_opt_t * opt)
{
 int32 sds1_id=-1,             /* data set identifier */
       sds1_index,             /* index number of the data set */
       dtype1,                 /* SDS data type */
       dimsizes1[H4_MAX_VAR_DIMS],/* dimensional size of SDS */
       nattrs1,                /* number of SDS attributes */
       rank1,                  /* rank of SDS */
       sds2_id=-1,             /* data set identifier */
       sds2_index,             /* index number of the data set */
       dtype2,                 /* SDS data type */
       dimsizes2[H4_MAX_VAR_DIMS],/* dimensional size of SDS */
       nattrs2,                /* number of SDS attributes */
       rank2,                  /* rank of SDS */
       start[H4_MAX_VAR_DIMS],    /* read start */
       edges[H4_MAX_VAR_DIMS],    /* read edges */
       numtype,                /* number type */
       eltsz;                  /* element size */
 uint32 nelms;                 /* number of elements */
 size_t need;	               /* read size needed */
 char  sds1_name[H4_MAX_NC_NAME]; 
 char  sds2_name[H4_MAX_NC_NAME]; 
 int   dim_diff=0;             /* dimensions are different */
 intn  empty1_sds;
 intn  empty2_sds;
 VOIDP buf1=NULL;
 VOIDP buf2=NULL;
 uint32 max_err_cnt;
 int   i;
 VOIDP fill1=NULL;
 VOIDP fill2=NULL;
 uint32 nfound=0;
 void   *sm_buf1=NULL;                
 void   *sm_buf2=NULL;


/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */
  
 sds1_index = SDreftoindex(sd1_id,ref1);
 sds1_id    = SDselect(sd1_id,sds1_index);
 
 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 if (SDgetinfo(sds1_id,sds1_name,&rank1,dimsizes1,&dtype1,&nattrs1)==FAIL) {
   printf( "Failed to get info for SDS ref <%ld>\n",ref1);
   goto out;
  }


/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */

 sds2_index = SDreftoindex(sd2_id,ref2);
 sds2_id    = SDselect(sd2_id,sds2_index);
 
 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 if (SDgetinfo(sds2_id,sds2_name,&rank2,dimsizes2,&dtype2,&nattrs2)==FAIL) {
   printf( "Failed to get info for SDS ref <%ld>\n",ref2);
   goto out;
  }


 /* flag to compare SDSs */
 if (opt->sd == 1)
 {

/*-------------------------------------------------------------------------
 * check for input SDs
 *-------------------------------------------------------------------------
 */
 
 if (opt->nlvars > 0)   /* if specified vdata is selected */
 {
  int imatch = 0, j;
  for (j = 0; j < opt->nlvars; j++)
  {
   if (strcmp(sds1_name, opt->lvars[j]) == 0)
   {
    imatch = 1;
    break;
   }
  }
  if (imatch == 0)
  {
   goto do_nothing;
  }
 }  

/*-------------------------------------------------------------------------
 * check for different type
 *-------------------------------------------------------------------------
 */
 
 if (dtype1 != dtype2) 
 {
  printf("Comparison not supported\n");
  printf("<%s> has datatype %ld, <%s> has datatype %ld ",sds1_name,dtype1,sds2_name,dtype2);
  goto do_nothing;
 }

/*-------------------------------------------------------------------------
 * check for the same rank
 *-------------------------------------------------------------------------
 */
 
 if ( rank1 != rank2 )
 {
  printf("Comparison not supported\n");
  printf("<%s> has rank %ld, dimensions ", sds1_name, rank1);
  print_dims(rank1,dimsizes1);
  printf("\n" );
  printf("<%s> has rank %ld, dimensions ", sds2_name, rank2);
  print_dims(rank2,dimsizes2);
  goto do_nothing;
 }

/*-------------------------------------------------------------------------
 * check for different dimensions
 *-------------------------------------------------------------------------
 */
 
 for ( i=0; i<rank1; i++) 
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
  printf("<%s> has rank %ld, dimensions ", sds1_name, rank1);
  print_dims(rank1,dimsizes1);
  printf("\n" );
  printf("<%s> has rank %ld, dimensions ", sds2_name, rank2);
  print_dims(rank2,dimsizes2);
  goto do_nothing;
 }

/*-------------------------------------------------------------------------
 * check if the input SDSs are empty. if so , return 
 *-------------------------------------------------------------------------
 */ 
 if (SDcheckempty( sds1_id, &empty1_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", sds1_name);
  goto out;
 }
 if (empty1_sds==1) 
 {
     if (opt->verbose)
         printf( "Empty SDS <%s>\n", sds1_name);
     goto do_nothing;
 }

 if (SDcheckempty( sds2_id, &empty2_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", sds2_name);
  goto out;
 }
 if (empty2_sds==1 ) 
 {
     if (opt->verbose)
         printf( "Empty SDS <%s>\n", sds2_name);
     goto do_nothing;
 }


/*-------------------------------------------------------------------------
 * get size 
 *-------------------------------------------------------------------------
 */

 /* compute the number of the bytes for each value. */
 numtype = dtype1 & DFNT_MASK;
 eltsz = DFKNTsize(numtype | DFNT_NATIVE);

/*-------------------------------------------------------------------------
 * get fill values
 *-------------------------------------------------------------------------
 */

 fill1 = (VOIDP) HDmalloc(eltsz);
 fill2 = (VOIDP) HDmalloc(eltsz);
 if (fill1!=NULL && SDgetfillvalue(sds1_id,fill1)<0)
 {
  HDfree(fill1);
  fill1 = NULL;
 }
 if (fill2!=NULL && SDgetfillvalue(sds2_id,fill2)<0)
 {
  HDfree(fill2);
  fill2 = NULL;
 }


/*-------------------------------------------------------------------------
 * read
 *-------------------------------------------------------------------------
 */ 

 nelms = 1;
 for (i = 0; i < rank1; i++) 
 {
     nelms   *= dimsizes1[i];
 }


 need = (size_t)(nelms * eltsz); /* bytes needed */
 
 if ( need < H4TOOLS_MALLOCSIZE)
 {
     buf1 = (VOIDP)HDmalloc(need);
     buf2 = (VOIDP)HDmalloc(need);
 }

/*-------------------------------------------------------------------------
 * read all
 *-------------------------------------------------------------------------
 */ 

 if ( buf1!=NULL && buf2!=NULL)
 {
     
     /* select all */
     for (i = 0; i < rank1; i++) 
     {
         edges[i] = dimsizes1[i];
         start[i] = 0;
     }

  
     /* read */
     if (SDreaddata (sds1_id, start, NULL, edges, buf1) == FAIL) 
     {
         printf( "Could not read SDS <%s>\n", sds1_name);
         goto out;
     }
     /* read */
     if (SDreaddata (sds2_id, start, NULL, edges, buf2) == FAIL) 
     {
         printf( "Could not read SDS <%s>\n", sds2_name);
         goto out;
     }

    /*-------------------------------------------------------------------------
     * comparing
     *-------------------------------------------------------------------------
     */
     
     if (opt->verbose)
         printf("Comparing <%s>\n",sds1_name); 
     
     /* if the given max_err_cnt is set (i.e. not its default MAX_DIFF),
	use it, otherwise, use the total number of elements in the dataset */
     max_err_cnt = (opt->max_err_cnt != MAX_DIFF) ? opt->max_err_cnt : nelms;
     nfound=array_diff(buf1, 
         buf2, 
         nelms, 
         sds1_name,
         sds2_name,
         rank1,
         dimsizes1,
         dtype1, 
         opt->err_limit,
         opt->err_rel,
         max_err_cnt, 
         opt->statistics, 
         fill1, 
         fill2);
 }
 
 else /* possibly not enough memory, read/compare by hyperslabs */
  
 {
     size_t        p_type_nbytes = eltsz;   /*size of type */
     uint32        p_nelmts = nelms;        /*total selected elmts */
     uint32        elmtno;                  /*counter  */
     int           carry;                   /*counter carry value */

     
     /* stripmine info */
     int32         sm_size[H4_MAX_VAR_DIMS];   /*stripmine size */
     int32         sm_nbytes;               /*bytes per stripmine */
     int32         sm_nelmts;               /*elements per stripmine*/
     
     /* hyperslab info */
     int32         hs_offset[H4_MAX_VAR_DIMS]; /*starting offset */
     int32         hs_size[H4_MAX_VAR_DIMS];   /*size this pass */
     int32         hs_nelmts;               /*elements in request */
     
     /*
      * determine the strip mine size and allocate a buffer. The strip mine is
      * a hyperslab whose size is manageable.
      */
     sm_nbytes = p_type_nbytes;
     
     for (i = rank1; i > 0; --i) 
     {
         sm_size[i - 1] = MIN(dimsizes1[i - 1], H4TOOLS_BUFSIZE / sm_nbytes);
         sm_nbytes *= sm_size[i - 1];
         assert(sm_nbytes > 0);
     }
     
     sm_buf1 = HDmalloc((size_t)sm_nbytes);
     sm_buf2 = HDmalloc((size_t)sm_nbytes);
     
     sm_nelmts = sm_nbytes / p_type_nbytes;
     
     /* the stripmine loop */
            HDmemset(hs_offset, 0, sizeof hs_offset);
     
     for (elmtno = 0; elmtno < p_nelmts; elmtno += hs_nelmts) 
     {
         /* calculate the hyperslab size */
         if (rank1 > 0) 
         {
             for (i = 0, hs_nelmts = 1; i < rank1; i++) 
             {
                 hs_size[i] = MIN(dimsizes1[i] - hs_offset[i], sm_size[i]);
                 hs_nelmts *= hs_size[i];
             }
             
         } 
         else 
         {
             hs_nelmts = 1;
         } /* rank */

         
         /* read */
         if (SDreaddata (sds1_id, hs_offset, NULL, hs_size, sm_buf1) == FAIL) 
         {
             printf( "Could not read SDS <%s>\n", sds1_name);
             goto out;
         }
         /* read */
         if (SDreaddata (sds2_id, hs_offset, NULL, hs_size, sm_buf2) == FAIL) 
         {
             printf( "Could not read SDS <%s>\n", sds2_name);
             goto out;
         }
         
        /*-------------------------------------------------------------------------
         * comparing
         *-------------------------------------------------------------------------
         */
         
         if (opt->verbose)
             printf("Comparing <%s>\n",sds1_name); 
         
	/* if the given max_err_cnt is set (i.e. not its default MAX_DIFF),
	   use it, otherwise, use the total number of elements in the dataset */
	max_err_cnt = (opt->max_err_cnt != MAX_DIFF) ? opt->max_err_cnt : nelms;

        /* get array differences. in the case of hyperslab read, increment the number of differences 
           found in each hyperslab and pass the position at the beggining for printing 
         */
         nfound=array_diff(sm_buf1, 
             sm_buf2, 
             hs_nelmts, 
             sds1_name,
             sds2_name,
             rank1,
             dimsizes1,
             dtype1, 
             opt->err_limit,
             opt->err_rel,
             max_err_cnt, 
             opt->statistics, 
             fill1, 
             fill2);

         /* calculate the next hyperslab offset */
         for (i = rank1, carry = 1; i > 0 && carry; --i) 
         {
             hs_offset[i - 1] += hs_size[i - 1];
             if (hs_offset[i - 1] == dimsizes1[i - 1])
                 hs_offset[i - 1] = 0;
             else
                 carry = 0;
         } /* i */
     } /* elmtno */
     
     /* free */
     if (sm_buf1!=NULL)
     {
                HDfree(sm_buf1);
         sm_buf1=NULL;
     }
     if (sm_buf2!=NULL)
     {
                HDfree(sm_buf2);
         sm_buf2=NULL;
     }
     
 } /* hyperslab read */

  
 } /* flag to compare SDSs */
 
 /* flag to compare SDSs local attributes */
 if (opt->sa == 1)
 {
  nfound += diff_sds_attrs(sds1_id,nattrs1,sds2_id,nattrs2,sds1_name,opt);
 }
 
/*-------------------------------------------------------------------------
 * close
 *-------------------------------------------------------------------------
 */

do_nothing:

 SDendaccess(sds1_id);
 SDendaccess(sds2_id);
 if (buf1)  HDfree(buf1);
 if (buf2)  HDfree(buf2);
 if (fill1) HDfree(fill1);
 if (fill2) HDfree(fill2);

 return nfound;


out:

 opt->err_stat = 1;

 if (sds1_id!=-1)
  SDendaccess(sds1_id);
 if (sds2_id!=-1)
  SDendaccess(sds2_id);

 if (buf1)  HDfree(buf1);
 if (buf2)  HDfree(buf2);
 if (fill1) HDfree(fill1);
 if (fill2) HDfree(fill2);

 return 0;

}




/*-------------------------------------------------------------------------
 * Function: diff_sds_attrs
 *
 * Purpose: compare SDS attributes
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 2, 2003
 *
 *-------------------------------------------------------------------------
 */

static
uint32 diff_sds_attrs(int32 sds1_id,
                      int32 nattrs1,
                      int32 sds2_id,
                      int32 nattrs2,
                      char* sds1_name,
                      diff_opt_t * opt)
{
 int32 dtype1,                 /* SDS data type */
       nelms1,                 /* number of elements */
       dtype2,                 /* SDS data type */
       nelms2;                 /* number of elements */
 char  attr1_name[H4_MAX_NC_NAME];
 char  attr2_name[H4_MAX_NC_NAME];
 VOIDP attr1_buf=NULL;
 VOIDP attr2_buf=NULL;
 int   i, cmp;
 uint32 nfound=0;

 if ( nattrs1!=nattrs2) {
  printf( "Different number of atrributes\n");
  return 0;
 }

 /* loop through attributes */
 for (i = 0; i < nattrs1; i++) 
 {
  if (SDattrinfo (sds1_id, i, attr1_name, &dtype1, &nelms1) == FAIL) {
   printf( "Cannot get info for attribute number %d\n", i);
   goto out;
  }
  
  if (SDattrinfo (sds2_id, i, attr2_name, &dtype2, &nelms2) == FAIL) {
   printf( "Cannot get info for attribute number %d\n", i);
   goto out;
  }

  if (dtype1 != dtype2 || nelms1 != nelms2 || (strcmp(attr1_name,attr2_name)!=0)) {
   printf( "Different information for attribute <%d>\n", i);
   continue;
  }
  
        attr1_buf = (void *)HDmalloc((unsigned)nelms1*DFKNTsize(dtype1 | DFNT_NATIVE));
  if (!attr1_buf) {
   printf("Out of memory!");
   goto out;;
  }
        attr2_buf = (void *)HDmalloc((unsigned)nelms2*DFKNTsize(dtype2 | DFNT_NATIVE));
  if (!attr2_buf) {
   printf("Out of memory!");
   goto out;
  }
 
  if (SDreadattr(sds1_id, i, attr1_buf)==FAIL ) {
   printf( "Could not read attribute number %d\n", i);
   goto out;
  }
  if (SDreadattr(sds2_id, i, attr2_buf)==FAIL ) {
   printf( "Could not read attribute number %d\n", i);
   goto out;
  }


  cmp = HDmemcmp(attr1_buf,attr2_buf,nelms1*DFKNTsize(dtype1 | DFNT_NATIVE));
  if (cmp!=0)
  {
   printf("\n---------------------------\n");
   printf ("%s:%s = \n",sds1_name,attr1_name);
   printf("<<<<\n");
   pr_att_vals((nc_type)dtype1, nelms1, attr1_buf);
   printf (" ;\n");
   printf(">>>>\n");
   pr_att_vals((nc_type)dtype2, nelms2, attr2_buf);
   printf (" ;\n");

   nfound++;
  }

  if (attr1_buf) HDfree(attr1_buf);
  if (attr2_buf) HDfree(attr2_buf);
 
 }

 return nfound;


out:

    if (attr1_buf) HDfree(attr1_buf);
    if (attr2_buf) HDfree(attr2_buf);
    opt->err_stat = 1;
    return 0;
}



/*-------------------------------------------------------------------------
 * Function: print_dims
 *
 * Purpose: print dimensions
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: May 9, 2003
 *
 * Comments: 
 *
 *-------------------------------------------------------------------------
 */
void print_dims( int r, int32 *d )
{
 int i;
 printf("[ " );  
 for ( i=0; i<r; i++ ) 
  printf("%d ",(int)d[i]  );
 printf("] " );
}
