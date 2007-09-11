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
