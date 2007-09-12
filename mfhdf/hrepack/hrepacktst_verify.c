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
#include "hrepacktst.h"

/*-------------------------------------------------------------------------
 * locals
 *-------------------------------------------------------------------------
 */ 

static int vg_getngrpdep( HFILEID f);
static int cmp_gr(int32 ri1_id, int32 ri2_id);

/*-------------------------------------------------------------------------
 * Function: sds_verifiy_comp
 *
 * Purpose: utility function to verify compression for SDS_NAME
 *
 * Return: 0, -1 on error
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int sds_verifiy_comp(const char *sds_name, 
                     int32 in_comp_type, 
                     int32 in_comp_info)
{
 comp_coder_t  comp_type;    /* to retrieve compression type into */
 comp_info     comp_info;    /* compression structure */ 
 int32         sd_id,
               sds_id, 
               sds_index;   

 /* get chunk and comp */
 sd_id     = SDstart (HREPACK_FILE1_OUT, DFACC_RDONLY);
 sds_index = SDnametoindex(sd_id, sds_name);
 if ((sds_id = SDselect(sd_id, sds_index))==FAIL) {
  printf("Error: Cannot open sds <%s>", sds_name);
  SDend (sd_id);
  return -1;
 }

/*-------------------------------------------------------------------------
 * retrieve and verify the compression info
 *-------------------------------------------------------------------------
 */
 
 comp_type = COMP_CODE_NONE;  /* reset variables before retrieving info */
 HDmemset(&comp_info, 0, sizeof(comp_info)) ;
 SDgetcompress(sds_id, &comp_type, &comp_info);
 if ( comp_type != in_comp_type )
 {
  printf("Error: Compression type does not match ");
  SDendaccess (sds_id);
  SDend (sd_id);
  return -1;
 }
 if (in_comp_info) {
  if ( comp_info.skphuff.skp_size != in_comp_info )
  {
   printf("Error: compresion information does not match ");
   SDendaccess (sds_id);
   SDend (sd_id);
   return -1;
  }
 }
 
 /* terminate access to the sds */
 SDendaccess (sds_id);
 
 /* terminate access to the sd interface */
 SDend (sd_id);
 
 return 0;
 
}

/*-------------------------------------------------------------------------
 * Function: sds_verifiy_comp_all
 *
 * Purpose: utility function to verify compression for all SDSs
 *
 * Return: 0, -1 on error
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int sds_verifiy_comp_all(comp_coder_t in_comp_type, 
                         int in_comp_info)
{
 comp_coder_t  comp_type;    /* to retrieve compression type into */
 comp_info     comp_info;    /* compression structure */ 
 int32         sd_id,
               sds_id, 
               sds_index,
               n_datasets,   /* number of datasets in the file */
               n_file_attrs, /* number of file attributes */
               data_type,              /* number type  */
               rrank,                  /* read rank */
               n_attrs,                /* number of attributes */
               dim_sizes[H4_MAX_VAR_DIMS];/* dimensions of an image */
 char          name[H4_MAX_GR_NAME];      /* name of dataset */
 int           info;
 intn          empty_sds;
 int           status;

 /* initialize the sd interface */
 sd_id  = SDstart (HREPACK_FILE1_OUT, DFACC_READ);
 
 /* determine the number of data sets in the file */
 if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL) {
  printf("Error: Cannot get file information");
  SDend (sd_id);
  return -1;
 }
 
 for (sds_index = 0; sds_index < n_datasets; sds_index++)
 {
  sds_id   = SDselect (sd_id, sds_index);
  
  /* skip dimension scales */
  if ( SDiscoordvar(sds_id) ) {
   SDendaccess(sds_id);
   continue;
  }

  name[0] = '\0';
  status=SDgetinfo(sds_id, name, &rrank, dim_sizes, &data_type, &n_attrs);
  if (status < 0) {
   printf("Error: can't read info for SDS <%s>",name);
   SDendaccess (sds_id);
   SDend (sd_id);
   return -1;
  }

 /*-------------------------------------------------------------------------
  * check if the input SDS is empty
  *-------------------------------------------------------------------------
  */ 
  if (SDcheckempty( sds_id, &empty_sds ) == FAIL) {
   printf( "Failed to check empty SDS <%s>\n", name);
   SDendaccess (sds_id);
   SDend (sd_id);
   return -1;
  }
 
 /*-------------------------------------------------------------------------
  * retrieve and verify the compression info
  *-------------------------------------------------------------------------
  */

  if (empty_sds==0 )
  {
  
   comp_type = COMP_CODE_NONE;  /* reset variables before retrieving info */
   HDmemset(&comp_info, 0, sizeof(comp_info)) ;
   
   status = SDgetcompress(sds_id, &comp_type, &comp_info);
   if (status < 0) {
    printf("Warning: can't read compression for SDS <%s>",name);
   } else {
    if ( comp_type != in_comp_type )
    {
     printf("Error: compression type does not match <%s>",name);
     SDendaccess (sds_id);
     SDend (sd_id);
     return -1;
    }
    if (in_comp_type) 
    {
     switch (in_comp_type)
     {
     case COMP_CODE_NONE:
      break;
     case COMP_CODE_RLE:
      break;
     case COMP_CODE_SZIP:
      break;
     case COMP_CODE_SKPHUFF:
      info  = comp_info.skphuff.skp_size;
      break;
     case COMP_CODE_DEFLATE:
      info  = comp_info.deflate.level;
      break;
     default:
      printf("Error: Unrecognized compression code %d\n",in_comp_type);
      info = -1;
      break;
     };
     
     if ( info != in_comp_info )
     {
      printf("Error: compresion information does not match for <%s>",name);
      SDendaccess (sds_id);
      SDend (sd_id);
      return -1;
     }
    }
   }

  } /* empty_sds */
  
  /* terminate access to the current dataset */
  SDendaccess (sds_id);
 }
 
 /* terminate access to the sd interface */
 SDend (sd_id);
 
 return 0;
}

/*-------------------------------------------------------------------------
 * Function: sds_verifiy_chunk
 *
 * Purpose: utility function to verify chunking for  SDS_NAME
 *
 * Return: 0, -1 on error
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int sds_verifiy_chunk(const char *sds_name, 
                      int32 in_chunk_flags, 
                      int rank, 
                      int32 *in_chunk_lengths)
{
 HDF_CHUNK_DEF chunk_def;    /* chunk defintion read */ 
 int32         chunk_flags;  /* chunking flag */ 
 int32         sd_id,
               sds_id, 
               sds_index;   
 int           i;

 /* get chunk and comp */
 sd_id     = SDstart (HREPACK_FILE1_OUT, DFACC_RDONLY);
 sds_index = SDnametoindex(sd_id, sds_name);
 if ((sds_id = SDselect(sd_id, sds_index))==FAIL) {
  printf("Error: cannot open sds <%s>", sds_name);
  SDend (sd_id);
  return -1;
 }
 SDgetchunkinfo (sds_id, &chunk_def, &chunk_flags);

/*-------------------------------------------------------------------------
 * retrieve and verify the chunk info
 *-------------------------------------------------------------------------
 */
 if ( chunk_flags != (in_chunk_flags) )
 {
  printf("Error: chunk flags do not match");
  SDendaccess (sds_id);
  SDend (sd_id);
  return -1;
 }
 for (i = 0; i < rank; i++)
 {
  if (chunk_def.chunk_lengths[i] != in_chunk_lengths[i] )
  {
   printf("Error: chunk lengths do not match ");
   SDendaccess (sds_id);
   SDend (sd_id);
   return -1;
  }
 }

 /* terminate access to the sds */
 SDendaccess (sds_id);
 
 /* terminate access to the sd interface */
 SDend (sd_id);
 
 return 0;
 
}

/*-------------------------------------------------------------------------
 * Function: sds_verifiy_chunk_all
 *
 * Purpose: utility function to verify chunking for all SDSs
 *
 * Return: 0, -1 on error
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int sds_verifiy_chunk_all(int32 in_chunk_flags, 
                          int rank, 
                          int32 *in_chunk_lengths,
                          const char *sds_exclude)
{
 HDF_CHUNK_DEF chunk_def;    /* chunk defintion read */ 
 int32         chunk_flags;  /* chunking flag */ 
 int32         sd_id,
               sds_id, 
               sds_index,
               n_datasets,   /* number of datasets in the file */
               n_file_attrs, /* number of file attributes */
               data_type,              /* number type  */
               rrank,                  /* read rank */
               n_attrs,                /* number of attributes */
               dim_sizes[H4_MAX_VAR_DIMS];/* dimensions of an image */
 char          name[H4_MAX_GR_NAME];      /* name of dataset */
 int           i;

 /* initialize the sd interface */
 sd_id  = SDstart (HREPACK_FILE1_OUT, DFACC_READ);
 
 /* determine the number of data sets in the file */
 if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL) {
  printf("Error: cannot get file information");
  SDend (sd_id);
  return -1;
 }
 
 for (sds_index = 0; sds_index < n_datasets; sds_index++)
 {
  sds_id   = SDselect (sd_id, sds_index);
  
  /* skip dimension scales */
  if ( SDiscoordvar(sds_id) ) {
   SDendaccess(sds_id);
   continue;
  }

  SDgetinfo(sds_id, name, &rrank, dim_sizes, &data_type, &n_attrs);
  SDgetchunkinfo (sds_id, &chunk_def, &chunk_flags);

  /* do not compare this one */
  if (strcmp(name,sds_exclude)==0)
  {
   SDendaccess(sds_id);
   SDend (sd_id);
   return 0;
  }
  
 /*-------------------------------------------------------------------------
  * retrieve and verify the chunk info
  *-------------------------------------------------------------------------
  */
  if ( chunk_flags != (in_chunk_flags) )
  {
   printf("Error: chunk flags do not match");
   SDendaccess (sds_id);
   SDend (sd_id);
   return -1;
  }
  for (i = 0; i < rank; i++)
  {
   if (chunk_def.chunk_lengths[i] != in_chunk_lengths[i] )
   {
    printf("Error: chunk lengths do not match ");
    SDendaccess (sds_id);
    SDend (sd_id);
    return -1;
   }
  }
  
  /* terminate access to the current dataset */
  SDendaccess (sds_id);
 }
 
 /* terminate access to the sd interface */
 SDend (sd_id);
 
 return 0;
 
}

/*-------------------------------------------------------------------------
 * Function: cmp_grs
 *
 * Purpose: compare all GR images in 2 files, assumed to be identical
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 03, 2003
 *
 *-------------------------------------------------------------------------
 */

void cmp_grs(char* file1_name,char* file2_name)
{
 int32 file1_id,          /* file identifier */
       file2_id,          /* file identifier */
       gr1_id,            /* GR interface identifier */
       gr2_id,            /* GR interface identifier */
       ri1_id,            /* raster image identifier */
       ri2_id,            /* raster image identifier */
       n_rimages,         /* number of raster images in the file */
       n_file_attrs,      /* number of file attributes */
       ri_index;          /* index of a image */

 /* open the files for read  */
 file1_id = Hopen (file1_name,DFACC_READ,(int16)0);
 file2_id = Hopen (file2_name,DFACC_READ,(int16)0);
 
 /* initialize the GR interface */
 gr1_id = GRstart (file1_id);
 gr2_id = GRstart (file2_id);
 
 /* determine the contents of the file */
 if (GRfileinfo (gr1_id, &n_rimages, &n_file_attrs)==FAIL){
  printf("Error: Cannot get GR info\n");
  goto out;
 }
  
 for (ri_index = 0; ri_index < n_rimages; ri_index++)
 {
  ri1_id = GRselect (gr1_id, ri_index);
  ri2_id = GRselect (gr2_id, ri_index);
  
  /* compare GR  */
  cmp_gr(ri1_id,ri2_id);

  /* terminate access to the current raster image */
  GRendaccess (ri1_id);
  GRendaccess (ri2_id);
 }
 
out:
 /* terminate access to the GR interface */
 GRend (gr2_id);
 GRend (gr2_id);
 /* close the HDF files */
 if (Hclose (file1_id)== FAIL )
  printf( "Failed to close file <%s>\n", file1_name);
 if (Hclose (file2_id)== FAIL )
  printf( "Failed to close file <%s>\n", file2_name);


}


/*-------------------------------------------------------------------------
 * Function: cmp_gr
 *
 * Purpose: compare 2 GR images 
 *
 * Return: same as memcmp
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 03, 2003
 *
 *-------------------------------------------------------------------------
 */

static int cmp_gr(int32 ri1_id, int32 ri2_id)
{
 int32         dimsizes[2],   /* dimensions of an image */
               n_comps,       /* number of components an image contains */
               interlace_mode1,/* interlace mode of an image */ 
               dtype,         /* number type of an image */
               n_attrs;       /* number of attributes belong to an image */
 int32         interlace_mode2;        
 char          gr_name[H4_MAX_GR_NAME]; 
 int           j, rank=2;
 int32         start[2],       /* read start */
               edges[2],       /* read edges */
               numtype,        /* number type */
               eltsz,          /* element size */
               nelms,          /* number of elements */
               data_size;
 VOIDP         buf1=NULL, buf2=NULL;
 int           cmp=-1;

 GRgetiminfo(ri1_id,gr_name,&n_comps,&dtype,&interlace_mode1,dimsizes,&n_attrs);
 GRgetiminfo(ri2_id,gr_name,&n_comps,&dtype,&interlace_mode2,dimsizes,&n_attrs);

 printf( "Comparing GR <%s>: ", gr_name);

 
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
  printf("Warning: different interlace mode: <%ld> and <%ld>", 
   interlace_mode1,interlace_mode2);
  interlace_mode1=interlace_mode2;
 }

/*-------------------------------------------------------------------------
 * check for data size before printing
 *-------------------------------------------------------------------------
 */

 /* compute the number of the bytes for each value. */
 numtype = dtype & DFNT_MASK;
 eltsz = DFKNTsize(numtype | DFNT_NATIVE);

 /* set edges of GR */
 nelms=1;
 for (j = 0; j < rank; j++) {
  nelms   *= dimsizes[j];
  edges[j] = dimsizes[j];
  start[j] = 0;
 }

 data_size = dimsizes[0]*dimsizes[1]*n_comps*eltsz;

/*-------------------------------------------------------------------------
 * read gr 1
 *-------------------------------------------------------------------------
 */

 /* alloc */
 if ((buf1 = (VOIDP) HDmalloc(data_size)) == NULL) {
  printf( "Failed to allocate %ld elements of size %ld\n", nelms, eltsz);
  goto out;
 }

 
 /* read data */
 if (GRreadimage (ri1_id, start, NULL, edges, buf1) == FAIL) {
  printf( "Could not read GR\n");
  goto out;
 }

/*-------------------------------------------------------------------------
 * read gr 2
 *-------------------------------------------------------------------------
 */

 /* alloc */
 if ((buf2 = (VOIDP) HDmalloc(data_size)) == NULL) {
  printf( "Failed to allocate %ld elements of size %ld\n", nelms, eltsz);
  goto out;
 }
 
 /* read data */
 if (GRreadimage (ri2_id, start, NULL, edges, buf2) == FAIL) {
  printf( "Could not read GR\n");
  goto out;
 }
 
 cmp = HDmemcmp(buf1,buf2,data_size);
 if (cmp!=0)
  printf("Differences found\n");
 else
  printf("\n");

out:
 /* terminate access to the GRs */
 GRendaccess(ri1_id);
 GRendaccess(ri2_id);
 if (buf1)
  free(buf1);
 if (buf2)
  free(buf2);
 return cmp;
 
}



/*-------------------------------------------------------------------------
 * Function: vg_verifygrpdep
 *
 * Purpose: utility function to verify number of vgroups dependencies in 
 *  2 files NAME1 and NAME2
 *
 * Return: 0, group dependencies are the same in both files
 *         1, they are not the same
 *        -1, error
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: September 10, 2007
 *
 *-------------------------------------------------------------------------
 */

int vg_verifygrpdep( char* name1, char* name2 )
{
    HFILEID f1;
    HFILEID f2;
    int32   nlnk1;
    int32   nlnk2;
    
    
    if ((f1 = Hopen(name1, DFACC_READ, 0)) == FAIL)
    {
        printf("\nFile (%s) failed to open.\n", name1);
        return -1;
    }
    if ((f2 = Hopen(name2, DFACC_READ, 0)) == FAIL)
    {
        printf("\nFile (%s) failed to open.\n", name2);
        return -1;
    }

    nlnk1 = vg_getngrpdep( f1 );
    nlnk2 = vg_getngrpdep( f2 );
    
    Hclose(f1);
    Hclose(f2);

    return (nlnk1 == nlnk2) ? 0: 1;
    
}


/*-------------------------------------------------------------------------
 * Function: vg_getngrpdep
 *
 * Purpose: utility function to get number of vgroups dependencies in 
 *  file HFILEID f
 *
 * Return: number of vgroups dependencies in file
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: September 10, 2007
 *
 *-------------------------------------------------------------------------
 */

static
int vg_getngrpdep( HFILEID f)
{
    int32       vg, vgt;
    int32       vgotag, vgoref;
    int32       vgid = -1;
    int32       vsid = -1;
    int32       vsno = 0;
    int32       vstag;
    int32       i, nvg, n, ne, nlnk;
    char        vgname[VGNAMELENMAX];

    Vstart(f);
    
    nvg = 0;
    nlnk = 0;
    while ((vgid = Vgetid(f, vgid)) != -1)
    {
        vg = Vattach(f, vgid, "r");
        if (vg == FAIL)
        {
            printf("cannot open vg id=%d\n", (int) vgid);
        }
        Vinquire(vg, &n, vgname);
        vgotag = VQuerytag(vg);
        vgoref = VQueryref(vg);
       
        for (i = 0; i < Vntagrefs(vg); i++)
        {
            Vgettagref(vg, i, &vstag, &vsid);
            
            if (vstag == DFTAG_VG)
            {
                vgt = Vattach(f, vsid, "r");
                
                if (vgt == FAIL)
                {
                    printf("cannot open vg id=%d\n", (int) vsid);
                    continue;
                }
                
                Vinquire(vgt, &ne, vgname);
                
                vgotag = VQuerytag(vgt);
                vgoref = VQueryref(vgt);
                
                Vdetach(vgt);

                nlnk++;
                
            } /* if */
            
        }   /* for */
        
        Vdetach(vg);
        nvg++;
        
    }  /* while */
    

    Vend(f);

    return nlnk;
    
}
