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


#include "hdiff.h"
#include "hdiff_list.h"
#include "hdiff_mattbl.h"

static
void print_dims( int r, int32 *d );

/*
Examples:

# Compare global attributes only
-g hdifftst1.hdf hdifftst2.hdf

# Compare SD local attributes only
-s hdifftst1.hdf hdifftst2.hdf

# Compare SD data only
-d hdifftst1.hdf hdifftst2.hdf

# Compare Vdata data only
-D hdifftst1.hdf hdifftst2.hdf

# Print statistics
-d -S hdifftst1.hdf hdifftst2.hdf

# Compare SD data on variable(s)
-d -v dset1 hdifftst1.hdf hdifftst2.hdf

# Compare vdata on variable(s) 
-D -u vdata1 hdifftst1.hdf hdifftst2.hdf

# Print difference up to count number
-d -e 2 hdifftst1.hdf hdifftst2.hdf

# Print difference when it is greater than limit
-d -t 2 hdifftst1.hdf hdifftst2.hdf

# no options
hdifftst1.hdf hdifftst2.hdf

# verbose 
-b hdifftst1.hdf hdifftst2.hdf
*/


/*-------------------------------------------------------------------------
 * Function: hdiff
 *
 * Purpose: find differences between two HDF files
 *
 * Return: number of differences found, -1 on failure
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 22, 2003
 *
 *-------------------------------------------------------------------------
 */

int hdiff(char *fname1, char *fname2, struct fspec fspec)
{
 table_t  *list1;
 table_t  *list2;
 int      nobjects1;
 int      nobjects2;
 int      nfound;
 int32    sd1_id,                 
          sd2_id;

 /* init tables */
 dtable_init(&list1);
 dtable_init(&list2);

/*-------------------------------------------------------------------------
 * get the objects in the file
 *-------------------------------------------------------------------------
 */

 if ((nobjects1=Hgetlist(fname1, list1))<=0)
  return FAIL;
 if ((nobjects2=Hgetlist(fname2, list2))<=0)
  return FAIL;

 if (fspec.verbose) {
  dtable_print(list1);
  dtable_print(list2);
 }

 nfound=match(fname1,nobjects1,list1,
              fname2,nobjects2,list2,fspec);
 
 /* free tables */
 dtable_free(list1);
 dtable_free(list2);

/*-------------------------------------------------------------------------
 * global and local attributes
 *-------------------------------------------------------------------------
 */

 if ((sd1_id = SDstart(fname1, DFACC_RDONLY))==FAIL) {
  printf("SDstart failed on <%s>", fname1);
  exit(1);
 }
 if ((sd2_id = SDstart(fname2, DFACC_RDONLY))==FAIL) {
  printf("SDstart failed on <%s>", fname2);
  exit(1);
 }
 
 if (fspec.ga == 1) 
  nfound+=gattr_diff(sd1_id, sd2_id, fspec);
 
 if (fspec.sa == 1)
  nfound+=sdattr_diff(sd1_id, sd2_id, fspec);
 
 SDend(sd1_id);
 SDend(sd2_id);

 return nfound;
}



/*-------------------------------------------------------------------------
 * Function: match
 *
 * Purpose: Find common objects; the algorithm used for this search is the 
 *  cosequential match algorithm and is described in 
 *  Folk, Michael; Zoellick, Bill. (1992). File Structures. Addison-Wesley.
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 22, 2003
 *
 * Comments:
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
int match( char *fname1, int nobjects1, table_t *list1,
           char *fname2, int nobjects2, table_t *list2, 
           struct fspec fspec )
{
 int32 file1_id,
       file2_id;
 int   cmp;
 int   more_names_exist = (nobjects1>0 && nobjects2>0) ? 1 : 0;
 int   curr1=0;
 int   curr2=0;
 int   nfound=0;
 /*build a common list */
 match_table_t *mattbl=NULL;
 unsigned infile[2]; 
 char     c1, c2;
 int      i;

/*-------------------------------------------------------------------------
 * build the list
 *-------------------------------------------------------------------------
 */
 match_table_init( &mattbl );

 while ( more_names_exist )
 {
  cmp = strcmp( list1->objs[curr1].obj_name, list2->objs[curr2].obj_name );
  if ( cmp == 0 )
  {
   infile[0]=1; infile[1]=1;
   match_table_add(mattbl,infile,
    list1->objs[curr1].obj_name,
    list1->objs[curr1].tag,
    list1->objs[curr1].ref,
    list2->objs[curr2].tag,
    list2->objs[curr2].ref);

   curr1++;
   curr2++;
  }
  else if ( cmp < 0 )
  {
   infile[0]=1; infile[1]=0;
   match_table_add(mattbl,infile,
    list1->objs[curr1].obj_name,
    list1->objs[curr1].tag,
    list1->objs[curr1].ref,
    -1,
    -1);
   curr1++;
  }
  else 
  {
   infile[0]=0; infile[1]=1;
   match_table_add(mattbl,infile,
    list2->objs[curr2].obj_name,
    -1,
    -1,
    list2->objs[curr2].tag,
    list2->objs[curr2].ref);
   curr2++;
  }

  more_names_exist = (curr1<nobjects1 && curr2<nobjects2) ? 1 : 0;

 
 } /* end while */

 /* list1 did not end */
 if (curr1<nobjects1)
 {
  while ( curr1<nobjects1 )
  {
   infile[0]=1; infile[1]=0;
   match_table_add(mattbl,infile,
    list1->objs[curr1].obj_name,
    list1->objs[curr1].tag,
    list1->objs[curr1].ref,
    -1,
    -1);
   curr1++;
  }
 }

 /* list2 did not end */
 if (curr2<nobjects2)
 {
  while ( curr2<nobjects2 )
  {
   infile[0]=0; infile[1]=1;
   match_table_add(mattbl,infile,
    list2->objs[curr2].obj_name,
    -1,
    -1,
    list2->objs[curr2].tag,
    list2->objs[curr2].ref);
   curr2++;
  }
 }

/*-------------------------------------------------------------------------
 * print the list
 *-------------------------------------------------------------------------
 */

 if (fspec.verbose) {
  printf("---------------------------------------\n");
  printf("file1     file2\n");
  printf("---------------------------------------\n");
  for (i = 0; i < mattbl->nobjs; i++)
  {
   c1 = (mattbl->objs[i].flags[0]) ? 'x' : ' ';
   c2 = (mattbl->objs[i].flags[1]) ? 'x' : ' ';
   printf("%5c %6c    %-15s\n", c1, c2, mattbl->objs[i].obj_name);
  }
  printf("\n");
 }


/*-------------------------------------------------------------------------
 * open the file handles here
 *-------------------------------------------------------------------------
 */

 if ((file1_id = Hopen(fname1, DFACC_READ, 0))==FAIL)
 {
  printf("Exiting: Hopen failed on <%s>", fname1);
  exit(1);
 }
 
 if ((file2_id = Hopen(fname2, DFACC_READ, 0))==FAIL)
 {
  printf("Exiting: Hopen failed on <%s>", fname2);
  exit(1);
 }
 
 

/*-------------------------------------------------------------------------
 * do the diff for objects
 *-------------------------------------------------------------------------
 */

 for (i = 0; i < mattbl->nobjs; i++)
 {
  if ( mattbl->objs[i].flags[0] && mattbl->objs[i].flags[1] )
   nfound+=diff( fname1, 
                 fname2,
                 file1_id,
                 file2_id,
                 mattbl->objs[i].obj_name, 
                 mattbl->objs[i].obj_name, 
                 mattbl->objs[i].tag1,
                 mattbl->objs[i].ref1,
                 mattbl->objs[i].tag2,
                 mattbl->objs[i].ref2,
                 fspec );
 }

/*-------------------------------------------------------------------------
 * close the file handles 
 *-------------------------------------------------------------------------
 */
 
 if (Hclose(file1_id)==FAIL) {
  printf("Error: Hclose failed on <%s>", fname1);
 }
 if (Hclose(file2_id)==FAIL) {
  printf("Error: Hclose failed on <%s>", fname2);
 }

 /* free table */
 match_table_free(mattbl);
 return nfound;
}

/*-------------------------------------------------------------------------
 * Function: diff
 *
 * Purpose: switch between types and choose the diff function
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 25, 2003
 *
 *-------------------------------------------------------------------------
 */

int diff( char *fname1,
          char *fname2, 
          int32 file1_id,
          int32 file2_id,
          char *obj1_name,
          char *obj2_name,
          int32 tag1,
          int32 ref1,
          int32 tag2,
          int32 ref2,
          struct fspec fspec )
{
 int nfound=0;

 switch ( tag1 )
 {
  case DFTAG_SD:  /* Scientific Data */
  case DFTAG_SDG: /* Scientific Data Group */
  case DFTAG_NDG: /* Numeric Data Group */
   if (fspec.sd == 1)
   nfound=diff_sds(fname1,fname2,ref1,ref2,fspec);
  break;

  case DFTAG_VG: 
   break;

   
  case DFTAG_RI:  /* Raster Image */
  case DFTAG_CI:  /* Compressed Image */
  case DFTAG_RIG: /* Raster Image Group */
  case DFTAG_RI8: /* Raster-8 image */
  case DFTAG_CI8: /* RLE compressed 8-bit image */
  case DFTAG_II8: /* IMCOMP compressed 8-bit image */
   if (fspec.gr == 1)
   nfound=diff_gr(file1_id,file2_id,ref1,ref2,fspec);
   break;
   
  case DFTAG_VH: 
   if (fspec.vd == 1)
   nfound=diff_vs(file1_id,file2_id,ref1,ref2,fspec);
   break;
  
  default:
   printf("Tag <%d> and Tag <%d>: Comparison not supported for <%s> and <%s> \n", 
    tag1, tag2, obj1_name, obj2_name);
   break;
 } 
 
 return nfound;
}


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

int diff_sds(char  *fname1, 
             char  *fname2, 
             int32 ref1,
             int32 ref2,
             struct fspec specp)
{
 intn  status_n;               /* returned status_n for functions returning an intn  */
 int32 sd1_id,                 /* SD identifier */
       sds1_id,                /* data set identifier */
       sds1_index,             /* index number of the data set */
       dtype1,                 /* SDS data type */
       dimsizes1[MAX_VAR_DIMS],/* dimensional size of SDS */
       nattrs1,                /* number of SDS attributes */
       rank1,                  /* rank of SDS */
       sd2_id,                 /* SD identifier */
       sds2_id,                /* data set identifier */
       sds2_index,             /* index number of the data set */
       dtype2,                 /* SDS data type */
       dimsizes2[MAX_VAR_DIMS],/* dimensional size of SDS */
       nattrs2,                /* number of SDS attributes */
       rank2,                  /* rank of SDS */
       start[MAX_VAR_DIMS],    /* read start */
       edges[MAX_VAR_DIMS],    /* read edges */
       numtype,                /* number type */
       eltsz,                  /* element size */
       nelms;                  /* number of elements */
 char  sds1_name[MAX_NC_NAME]; 
 char  sds2_name[MAX_NC_NAME]; 
 int   nfound=0;
 int   dim_diff=0;             /* dimensions are different */
 intn  empty1_sds;
 intn  empty2_sds;
 VOIDP buf1=NULL;
 VOIDP buf2=NULL;
 unsigned int max_err_cnt;
 int    i, k;
 vnode* vlist = newvlist();  /* list for vars specified with -v option */
 int32  varid1;

/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */
	 
 sd1_id     = SDstart(fname1, DFACC_RDONLY);
 sds1_index = SDreftoindex(sd1_id,ref1);
 sds1_id    = SDselect(sd1_id,sds1_index);
 
 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 if (SDgetinfo(sds1_id,sds1_name,&rank1,dimsizes1,&dtype1,&nattrs1)==FAIL) {
   printf( "Failed to get info for SDS ref <%d>\n",ref1);
   SDendaccess(sds1_id);
   SDend(sd1_id);
   return FAIL;
  }


/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */

 sd2_id     = SDstart(fname2, DFACC_RDONLY);
 sds2_index = SDreftoindex(sd2_id,ref2);
 sds2_id    = SDselect(sd2_id,sds2_index);
 
 /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
 if (SDgetinfo(sds2_id,sds2_name,&rank2,dimsizes2,&dtype2,&nattrs2)==FAIL) {
   printf( "Failed to get info for SDS ref <%d>\n",ref2);
   SDendaccess(sds2_id);
   SDend(sd2_id);
   return FAIL;
  }

/*-------------------------------------------------------------------------
 * check for input SDs
 *-------------------------------------------------------------------------
 */

/*
 * If any vars were specified with -v option, get list of associated
 * variable ids
 */
 for (k=0; k < specp.nlvars; k++) 
 {
  varid1 = SDnametoindex(sd1_id, specp.lvars[k]);
  varadd(vlist, varid1);
 }
 
 /* if var list specified, test for membership */
 if (specp.nlvars > 0 && ! varmember(vlist, sds1_index))
  goto out;




/*-------------------------------------------------------------------------
 * check for different type
 *-------------------------------------------------------------------------
 */
 
 if (dtype1 != dtype2) 
 {
  printf("Comparison not supported\n");
  printf("<%s> has datatype %d, <%s> has datatype %d ",sds1_name,dtype1,sds2_name,dtype2);
  goto out;
 }

/*-------------------------------------------------------------------------
 * check for the same rank
 *-------------------------------------------------------------------------
 */
 
 if ( rank1 != rank2 )
 {
  printf("Comparison not supported\n");
  printf("<%s> has rank %d, dimensions ", sds1_name, rank1);
  print_dims(rank1,dimsizes1);
  printf("\n" );
  printf("<%s> has rank %d, dimensions ", sds2_name, rank2);
  print_dims(rank2,dimsizes2);
  goto out;
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
  printf("<%s> has rank %d, dimensions ", sds1_name, rank1);
  print_dims(rank1,dimsizes1);
  printf("\n" );
  printf("<%s> has rank %d, dimensions ", sds2_name, rank2);
  print_dims(rank2,dimsizes2);
  goto out;
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
 for (i = 0; i < rank1; i++) {
  nelms   *= dimsizes1[i];
  edges[i] = dimsizes1[i];
  start[i] = 0;
 }

/*-------------------------------------------------------------------------
 * check if the input SDSs are empty. if so , do not read its data 
 *-------------------------------------------------------------------------
 */ 
 if (SDcheckempty( sds1_id, &empty1_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", sds1_name);
  goto out;
 }
 if (empty1_sds==1 ) {
  printf( "Empty SDS <%s>\n", sds1_name);
  goto out;
 }
 if (SDcheckempty( sds2_id, &empty2_sds ) == FAIL) {
  printf( "Failed to check empty SDS <%s>\n", sds2_name);
  goto out;
 }
 if (empty2_sds==1 ) {
  printf( "Empty SDS <%s>\n", sds2_name);
  goto out;
 }

/*-------------------------------------------------------------------------
 * Read
 *-------------------------------------------------------------------------
 */
 
 /* alloc */
 if ((buf1 = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
  printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  goto out;
 }
 /* read data */
 if (SDreaddata (sds1_id, start, NULL, edges, buf1) == FAIL) {
  printf( "Could not read SDS <%s>\n", sds1_name);
  goto out;
 }
 /* alloc */
 if ((buf2 = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
  printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  goto out;
 }
 /* read data */
 if (SDreaddata (sds2_id, start, NULL, edges, buf2) == FAIL) {
  printf( "Could not read SDS <%s>\n", sds2_name);
  goto out;
 }


/*-------------------------------------------------------------------------
 * Comparing
 *-------------------------------------------------------------------------
 */

 if (specp.verbose)
 printf("Comparing <%s>\n",sds1_name); 

 /* 
  If max_err_cnt is set (i.e. not its default -1), use it otherwise set it
  to tot_err_cnt so it doesn't trip  
 */
 max_err_cnt = (specp.max_err_cnt >= 0) ? specp.max_err_cnt : nelms;
 nfound=array_diff(buf1, buf2, nelms, dtype1, specp.err_limit, 
   max_err_cnt, specp.statistics, 0, 0);
  


 
/*-------------------------------------------------------------------------
 * close
 *-------------------------------------------------------------------------
 */

out:
 status_n = SDendaccess(sds1_id);
 status_n = SDend(sd1_id);
 status_n = SDendaccess(sds2_id);
 status_n = SDend(sd2_id);
 if (buf1) free(buf1);
 if (buf2) free(buf2);

 return nfound;
}



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

int diff_gr( int32 file1_id,
             int32 file2_id,
             int32 ref1,              
             int32 ref2,
             struct fspec specp)  
{
 intn  status_n;               /* returned status_n for functions returning an intn  */
 int32 gr1_id,                 /* GR identifier */
       ri1_id,                 /* data set identifier */
       ri1_index,              /* index number of the data set */
       dtype1,                 /* GR data type */
       dimsizes1[MAX_VAR_DIMS],/* dimensional size */
       nattrs1,                /* number of attributes */
       ncomps1,                /* number of components */
       interlace_mode1,        /* interlace mode of an image */ 
       gr2_id,                 /* GR identifier */
       ri2_id,                 /* data set identifier */
       ri2_index,              /* index number of the data set */
       dtype2,                 /* GR data type */
       dimsizes2[MAX_VAR_DIMS],/* dimensional size */
       nattrs2,                /* number of attributes */
       ncomps2,                /* number of components */
       interlace_mode2,        /* interlace mode of an image */ 
       start[MAX_VAR_DIMS],    /* read start */
       edges[MAX_VAR_DIMS],    /* read edges */
       numtype,                /* number type */
       eltsz,                  /* element size */
       nelms,                  /* number of elements */
       data_size;
 char  gr1_name[MAX_NC_NAME]; 
 char  gr2_name[MAX_NC_NAME]; 
 int   nfound=0;
 int   dim_diff=0;             /* dimensions are different */
 VOIDP buf1=NULL;
 VOIDP buf2=NULL;
 unsigned int max_err_cnt;
 int    i;


/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */
	
 gr1_id    = GRstart(file1_id);
 ri1_index = GRreftoindex(gr1_id,(uint16)ref1);
 ri1_id    = GRselect(gr1_id,ri1_index);
   
 /*obtain name,rank,dimsizes,datatype and num of attributes of gr */
 if (GRgetiminfo(ri1_id,gr1_name,&ncomps1,&dtype1,&interlace_mode1,dimsizes1,&nattrs1)==FAIL) {
   printf( "Failed to get info for SDS ref <%d>\n",ref1);
   GRendaccess(ri1_id);
   GRend(gr1_id);
   return FAIL;
  }


/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */
	
 gr2_id    = GRstart(file2_id);
 ri2_index = GRreftoindex(gr2_id,(uint16)ref2);
 ri2_id    = GRselect(gr2_id,ri2_index);
   
 /*obtain name,rank,dimsizes,datatype and num of attributes of gr */
 if (GRgetiminfo(ri2_id,gr2_name,&ncomps2,&dtype2,&interlace_mode2,dimsizes2,&nattrs2)==FAIL) {
   printf( "Failed to get info for SDS ref <%d>\n",ref2);
   GRendaccess(ri2_id);
   GRend(gr2_id);
   return FAIL;
  }


/*-------------------------------------------------------------------------
 * check for different type
 *-------------------------------------------------------------------------
 */
 
 if (dtype1 != dtype2) 
 {
  printf("Comparison not supported\n");
  printf("<%s> has datatype %d, <%s> has datatype %d ",gr1_name,dtype1,gr2_name,dtype2);
  goto out;
 }

/*-------------------------------------------------------------------------
 * check for the same rank
 *-------------------------------------------------------------------------
 */
 
 if ( ncomps1 != ncomps2 )
 {
  printf("Comparison not supported\n");
  printf("<%s> has %d components\n", gr1_name, ncomps1);
  printf("\n" );
  printf("<%s> has %d components\n", gr2_name, ncomps2);
  goto out;
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
  goto out;
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

/*-------------------------------------------------------------------------
 * Read
 *-------------------------------------------------------------------------
 */
 
 /* alloc */
 if ((buf1 = (VOIDP) HDmalloc(data_size)) == NULL) {
  printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  goto out;
 }
 
 /* set the interlace for reading by component interlacing scheme */
 if ( GRreqimageil(ri1_id, interlace_mode1) == FAIL ){
  printf( "Could not set interlace for GR <%s>\n", gr1_name);
  goto out;
 }
 
 /* read data */
 if (GRreadimage (ri1_id, start, NULL, edges, buf1) == FAIL) {
  printf( "Could not read GR <%s>\n", gr1_name);
  goto out;
 }

 /* alloc */
 if ((buf2 = (VOIDP) HDmalloc(data_size)) == NULL) {
  printf( "Failed to allocate %d elements of size %d\n", nelms, eltsz);
  goto out;
 }
 
 /* set the interlace for reading by component interlacing scheme */
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
 * Comparing
 *-------------------------------------------------------------------------
 */

 if (specp.verbose)
 printf("Comparing <%s>\n",gr1_name); 

 /* 
  If max_err_cnt is set (i.e. not its default -1), use it otherwise set it
  to tot_err_cnt so it doesn't trip  
 */
 max_err_cnt = (specp.max_err_cnt >= 0) ? specp.max_err_cnt : nelms;
 nfound=array_diff(buf1, buf2, nelms, dtype1, specp.err_limit, 
   max_err_cnt, specp.statistics, 0, 0);
  
/*-------------------------------------------------------------------------
 * close
 *-------------------------------------------------------------------------
 */

out:
 status_n = GRendaccess(ri1_id);
 status_n = GRend(gr1_id);
 status_n = GRendaccess(ri2_id);
 status_n = GRend(gr2_id);
 if (buf1) free(buf1);
 if (buf2) free(buf2);

 return nfound;
}




/*-------------------------------------------------------------------------
 * Function: diff_vs
 *
 * Purpose: diff for VS 
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 26, 2003
 *
 *-------------------------------------------------------------------------
 */

int diff_vs( int32 file1_id,
             int32 file2_id,
             int32 ref1,              
             int32 ref2,
             struct fspec specp)          
{
 int32 vdata1_id,             /* vdata identifier */
       n_records1,            /* number of records */
       n_fields1,             /* number of fields */
       vdata1_size, 
       interlace1_mode,
       vdata2_id,             /* vdata identifier */
       n_records2,            /* number of records */
       n_fields2,             /* number of fields */
       vdata2_size, 
       interlace2_mode;
 char  vdata1_name [VSNAMELENMAX];
 char  vdata1_class[VSNAMELENMAX];
 char  fieldname1_list[VSFIELDMAX*FIELDNAMELENMAX];
 char  vdata2_name [VSNAMELENMAX];
 char  vdata2_class[VSNAMELENMAX];
 char  fieldname2_list[VSFIELDMAX*FIELDNAMELENMAX];
 int   ret=0;


 
/*-------------------------------------------------------------------------
 * object 1
 *-------------------------------------------------------------------------
 */

 if (Vstart (file1_id)==FAIL) {
  printf("Error: Could not start VS interface in VS ref %d\n", ref1);
  return FAIL;
 }

 if ((vdata1_id  = VSattach (file1_id, ref1, "r")) == FAIL ){
  printf( "Failed to attach VS ref %d\n", ref1);
  return FAIL;
 }
 if (VSgetname  (vdata1_id, vdata1_name) == FAIL ){
  printf( "Failed to name for VS ref %d\n", ref1);
  ret=-1;
  goto out;
 }
 if (VSgetclass (vdata1_id, vdata1_class) == FAIL ){
  printf( "Failed to name for VS ref %d\n", ref1);
  ret=-1;
  goto out;
 }
 
 if (VSinquire(vdata1_id, &n_records1, &interlace1_mode, fieldname1_list, 
  &vdata1_size, vdata1_name) == FAIL) {
  printf( "Failed to get info for VS ref %d\n", ref1);
  ret=-1;
  goto out;
 }
 
 if ((n_fields1 = VFnfields(vdata1_id)) == FAIL ){
  printf( "Failed getting fields forVS ref %d\n", ref1);
  ret=-1;
  goto out;
 }
 

/*-------------------------------------------------------------------------
 * object 2
 *-------------------------------------------------------------------------
 */

 if (Vstart (file2_id)==FAIL) {
  printf("Error: Could not start VS interface in VS ref %d\n", ref1);
  return FAIL;
 }

 if ((vdata2_id  = VSattach (file2_id, ref2, "r")) == FAIL ){
  printf( "Failed to attach VS ref %d\n", ref2);
  ret=-2;
  goto out;
 }
 if (VSgetname  (vdata2_id, vdata2_name) == FAIL ){
  printf( "Failed to name for VS ref %d\n", ref2);
  ret=-2;
  goto out;
 }
 if (VSgetclass (vdata2_id, vdata2_class) == FAIL ){
  printf( "Failed to name for VS ref %d\n", ref2);
  ret=-2;
  goto out;
 }
 
 if (VSinquire(vdata2_id, &n_records2, &interlace2_mode, fieldname2_list, 
  &vdata2_size, vdata2_name) == FAIL) {
  printf( "Failed to get info for VS ref %d\n", ref2);
  ret=-2;
  goto out;
 }
 
 if ((n_fields2 = VFnfields(vdata2_id)) == FAIL ){
  printf( "Failed getting fields forVS ref %d\n", ref2);
  ret=-2;
  goto out;
 }

/*-------------------------------------------------------------------------
 * check for input VSs
 *-------------------------------------------------------------------------
 */
 
 if (specp.nuvars > 0)   /* if specified vdata is selected */
 {
  int imatch = 0, j;
  for (j = 0; j < specp.nuvars; j++)
  {
   if (strcmp(vdata1_name, specp.uvars[j]) == 0)
   {
    imatch = 1;
    break;
   }
  }
  if (imatch == 0)
  {
   goto out;
  }
 }   


/*-------------------------------------------------------------------------
 * Comparing
 *-------------------------------------------------------------------------
 */

 if (specp.verbose)
 printf("Comparing <%s>\n",vdata1_name);  

 ret=vdata_cmp(vdata1_id,vdata2_id,vdata1_name,vdata1_class,specp.max_err_cnt);

out:
 /* terminate access to the VSs */
 if (VSdetach (vdata1_id)==FAIL) {
  printf( "Failed to dettach VS ref %d\n", ref1);
 }
 if (VSdetach (vdata2_id)==FAIL) {
  printf( "Failed to dettach VS ref %d\n", ref2);
 }
 
 return ret;
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
static
void print_dims( int r, int32 *d )
{
 int i;
 printf("[ " );  
 for ( i=0; i<r; i++ ) 
  printf("%d ",(int)d[i]  );
 printf("] " );
}
