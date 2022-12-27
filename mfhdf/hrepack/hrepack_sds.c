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
#include "hdf.h"
#include "mfhdf.h"
#include "hrepack.h"
#include "hrepack_sds.h"
#include "hrepack_an.h"
#include "hrepack_utils.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"
#include "hrepack_dim.h"


#define H4TOOLS_BUFSIZE        (1024 * 1024)
#define H4TOOLS_MALLOCSIZE     (1024 * 1024)


void print_info( int chunk_flags,
                 HDF_CHUNK_DEF *chunk_def,      
                 int comp_type,
                 char *path,
                 char *ratio);

int get_print_info(  int chunk_flags,
                     HDF_CHUNK_DEF *chunk_def,      
                     int comp_type,
                     char *path,
                     char *sds_name,
                     int32 sd_id);

/*-------------------------------------------------------------------------
 * Function: copy_sds
 *
 * Purpose: copy an SDS from input file to output file and compress it 
 *  using options
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_sds(int32 sd_in,
             int32 sd_out,
             int32 tag,               /* tag of input SDS */
             int32 ref,               /* ref of input SDS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             list_table_t *list_tbl,
             dim_table_t *td1,
             dim_table_t *td2,
             int32 infile_id,
             int32 outfile_id)
{
    int32 sds_id,              /* data set identifier */
        sds_out=FAIL,          /* data set identifier */
        sds_index,             /* index number of the data set */
        dtype,                 /* SDS data type */
        dimsizes[H4_MAX_VAR_DIMS],/* dimensions of SDS */
        start[H4_MAX_VAR_DIMS],   /* read start */
        edges[H4_MAX_VAR_DIMS],   /* read edges */
        nattrs,                /* number of SDS attributes */
        rank,                  /* rank of SDS */
        sds_ref,               /* reference number of the output data set */
        numtype,               /* number type */
        eltsz,                 /* element size */
        nelms,                 /* number of elements */
        dim_id,                /* dimension ID */
        dim_out;               /* dimension ID */
    char             sds_name[H4_MAX_NC_NAME]; 
    char             dim_name[H4_MAX_NC_NAME];
    char             *path=NULL;
    VOIDP            buf=NULL;
    VOIDP            dim_buf=NULL;
    int              i, j;
    int              info;           /* temporary int compression information */
    comp_coder_t     comp_type;      /* compression type requested  */
    comp_coder_t     comp_type_in;   /* compression type original  */
    comp_info        c_info;         /* compression information requested  */
    comp_info        c_info_in;      /* compression information original  */
    HDF_CHUNK_DEF    chunk_def;      /* chunk definition */
    HDF_CHUNK_DEF    chunk_def_in;   /* chunk definition original */
    int32            chunk_flags;    /* chunk flags */ 
    int32            chunk_flags_in; /* chunk flags original*/ 
    int              szip_mode;      /* szip mode, EC, NN */
    intn             empty_sds;
    int              have_info=0;
    size_t           need;	         /* read size needed */
    void             *sm_buf=NULL;
    int              is_record = 0;
    
    sds_index = SDreftoindex(sd_in,ref);
    sds_id    = SDselect(sd_in,sds_index);
    
    /*obtain name,rank,dimsizes,datatype and num of attributes of sds */
    if (SDgetinfo(sds_id,sds_name,&rank,dimsizes,&dtype,&nattrs)==FAIL){
        printf( "Could not get information for SDS\n");
        SDendaccess(sds_id);
        return FAIL;
    }
    
    /* check if the given SDS is a dimension scale, return 0 for no table add */
    if ( SDiscoordvar(sds_id) ) {
        /* add SDS coordinate variable to dimension table 1 */
        dim_table_add(td1,ref,sds_name);
        SDendaccess(sds_id);
        return SUCCEED;
    }
    
    /* initialize path */
    path=get_path(path_name,sds_name);
    
    /* add object to table */
    list_table_add(list_tbl,tag,ref,path);
    
   /*-------------------------------------------------------------------------
    * check if the input SDS is empty. if so , avoid some operations (mainly read, write)
    *-------------------------------------------------------------------------
    */ 
    if (SDcheckempty( sds_id, &empty_sds ) == FAIL) {
        printf( "Failed to check empty SDS <%s>\n", path);
        goto out;
    }
    
   /*-------------------------------------------------------------------------
    * element size and number of elements
    *-------------------------------------------------------------------------
    */
    
    /* compute the number of the bytes for each value */
    numtype = dtype & DFNT_MASK;
    eltsz = DFKNTsize(numtype | DFNT_NATIVE);
    
    /* get number of elements */
    nelms=1;
    for (j = 0; j < rank; j++) {
        nelms   *= dimsizes[j];
    }
    
   /*-------------------------------------------------------------------------
    * get the original compression/chunk information from the object 
    *-------------------------------------------------------------------------
    */
    
    /* reset variables before retrieving information */
    comp_type = comp_type_in = COMP_CODE_NONE; 
    chunk_flags = chunk_flags_in = HDF_NONE;
    
    if (empty_sds==0 )
    {
        HDmemset(&c_info_in, 0, sizeof(comp_info)) ;
        if ( SDgetcompinfo(sds_id, &comp_type_in, &c_info_in) == FAIL )
        {
            printf( "Could not get compression information for SDS <%s>\n",path);
            goto out;
        }
        
        /* get chunk lengths */
        if ( SDgetchunkinfo(sds_id, &chunk_def_in, &chunk_flags_in)== FAIL )
        {
            printf( "Could not get chunking information for SDS <%s>\n",path);
            goto out;
        }
      
        
        /* retrieve the compress information if so */
        if ( (HDF_CHUNK | HDF_COMP) == chunk_flags_in )
        {
            chunk_def_in.comp.comp_type=comp_type_in;
            switch (comp_type_in)
            {
            case COMP_CODE_RLE:
                chunk_def_in.comp.comp_type              = COMP_CODE_RLE;
                break;
            case COMP_CODE_SKPHUFF:
                chunk_def_in.comp.comp_type              = COMP_CODE_SKPHUFF;
                chunk_def_in.comp.cinfo.skphuff          = c_info_in.skphuff;
                break;
            case COMP_CODE_DEFLATE:
                chunk_def_in.comp.comp_type              = COMP_CODE_DEFLATE;
                chunk_def_in.comp.cinfo.deflate          = c_info_in.deflate;
                break;
            case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
                chunk_def_in.comp.comp_type              = COMP_CODE_SZIP;
                chunk_def_in.comp.cinfo.szip             = c_info_in.szip;
#else
                printf("Error: SZIP compression is not available <%s>\n",path);
                goto out;
#endif
                break;
            default:
                printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,path);
                goto out;
            };
        }
        
       /*-------------------------------------------------------------------------
        * set the default values to the ones read from the object
        *-------------------------------------------------------------------------
        */
        
       /*-------------------------------------------------------------------------
        * compression
        *-------------------------------------------------------------------------
        */
        
        comp_type   = comp_type_in;
        
        switch (comp_type_in)
        {
        case COMP_CODE_NBIT:
            printf("Nbit compression not supported in this version <%s>\n",path);
            break;
        case COMP_CODE_NONE:
            break;
        case COMP_CODE_RLE:
            break;
        case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
            info      = c_info_in.szip.pixels_per_block;
            if (c_info_in.szip.options_mask & SZ_EC_OPTION_MASK) {
                szip_mode = EC_MODE;
            } else if (c_info_in.szip.options_mask & SZ_NN_OPTION_MASK) {
                szip_mode = NN_MODE;
            }
#else
            printf("SZIP compression not supported in this version <%s>\n",path);
#endif
            break;
        case COMP_CODE_SKPHUFF:
            info  = c_info_in.skphuff.skp_size;
            break;
        case COMP_CODE_DEFLATE:
            info  = c_info_in.deflate.level;
            break;
        default:
            printf("Error: Unrecognized compression code in %d <%s>\n",comp_type,path);
            goto out;
            break;
        };
        
       /*-------------------------------------------------------------------------
        * chunking
        *-------------------------------------------------------------------------
        */
        
        chunk_flags = chunk_flags_in;
        
        if ( (HDF_CHUNK) == chunk_flags )
        {
            for (i = 0; i < rank; i++) 
                chunk_def.chunk_lengths[i]      = chunk_def_in.chunk_lengths[i];
        }
        else if ( (HDF_CHUNK | HDF_COMP) == chunk_flags )
        {
            for (i = 0; i < rank; i++) {
                chunk_def.chunk_lengths[i]      = chunk_def_in.chunk_lengths[i];
                chunk_def.comp.chunk_lengths[i] = chunk_def_in.chunk_lengths[i];
            }
            chunk_def.comp.comp_type=comp_type_in;
            switch (comp_type_in)
            {
            case COMP_CODE_RLE:
                chunk_def.comp.comp_type              = COMP_CODE_RLE;
                break;
            case COMP_CODE_SKPHUFF:
                chunk_def.comp.comp_type              = COMP_CODE_SKPHUFF;
                chunk_def.comp.cinfo.skphuff          = c_info_in.skphuff;
                break;
            case COMP_CODE_DEFLATE:
                chunk_def.comp.comp_type              = COMP_CODE_DEFLATE;
                chunk_def.comp.cinfo.deflate          = c_info_in.deflate;
                break;
            case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
                chunk_def.comp.comp_type              = COMP_CODE_SZIP;
                chunk_def.comp.cinfo.szip             = c_info_in.szip;
#else
                printf("Error: SZIP compression not available in %d <%s>\n",comp_type_in,path);
#endif
                break;
            default:
                printf("Error: Unrecognized compression code in %d <%s>\n",comp_type_in,path);
                goto out;
            };
        }
        
        
       /*-------------------------------------------------------------------------
        * get the compression/chunk information of this object from the table
        * translate to usable information
        * this is done ONLY for the second trip inspection 
        *-------------------------------------------------------------------------
        */
        
        /* check inspection mode */
        if ( options->trip>0 ) 
        {
            have_info = 
                options_get_info(options,      /* global options */
                &chunk_flags, /* chunk flags OUT */
                &chunk_def,   /* chunk definition OUT */
                &info,        /* compression information OUT */
                &szip_mode,   /* compression information OUT */
                &comp_type,   /* compression type OUT  */
                rank,         /* rank of object IN */
                path,         /* path of object IN */
                1,            /* number of GR image planes (for SZIP), IN */
                dimsizes,     /* dimensions (for SZIP), IN */
                dtype         /* numeric type ( for SZIP), IN */
                );
            if (have_info==FAIL)
                goto out;
        } /* check inspection mode */
        
        
       /*-------------------------------------------------------------------------
        * check for maximum number of chunks treshold
        *-------------------------------------------------------------------------
        */
        if ( options->trip>0 ) 
        {
            int count=1, nchunks;
            int maxchunk=INT_MAX;
            if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
            {
                for (j = 0; j < rank; j++) {
                    count   *= chunk_def.chunk_lengths[j];
                }
                nchunks=nelms/count;
                if (nchunks>maxchunk){
                    printf("Warning: number of chunks is %d (greater than %d). Not chunking <%s>\n",
                        nchunks,maxchunk,path);
                    chunk_flags=HDF_NONE;
                }
            }
        }
        
        
       /*-------------------------------------------------------------------------
        * check for objects too small
        *-------------------------------------------------------------------------
        */
        if ( have_info==1 && options->trip>0  && nelms*eltsz<options->threshold )
        {
            /* reset to the original values . we don't want to uncompress if it was */
            chunk_flags=chunk_flags_in;
            comp_type=comp_type_in;
            if (options->verbose) {
                printf("Warning: object size smaller than %d bytes. Not compressing <%s>\n",
                    options->threshold,path);
            }
        }
        
        
 } /* empty_sds */
 
    
  /*-------------------------------------------------------------------------
   * check if the requested compression is valid
   * SDSs do not support JPEG
   *-------------------------------------------------------------------------
   */
   
   /* check inspection mode */
   if ( options->trip>0 && empty_sds==0) 
   {
       switch(comp_type)
       {
       case COMP_CODE_NONE:
       case COMP_CODE_RLE:
       case COMP_CODE_SKPHUFF:
       case COMP_CODE_DEFLATE:
       case COMP_CODE_SZIP:
       case COMP_CODE_NBIT:
           break;
       case COMP_CODE_JPEG:
           printf("Error: JPEG compression is not available for <%s>\n",path);
           goto out;
           break;
       default:
           printf("Error: Unrecognized compression code %d in <%s>\n",comp_type_in,path);
           goto out;
       }
   } /* check inspection mode */
   
  /*-------------------------------------------------------------------------
   * if we are in first trip inspection mode, exit, after printing the information
   *-------------------------------------------------------------------------
   */ 
   
   /* check inspection mode */
   if ( options->trip==0 ) 
   {
       /* close sds before get info */
       SDendaccess(sds_id);
       
       if ( options->verbose)
       {
           
           if (get_print_info(chunk_flags_in,
               &chunk_def,      
               comp_type,
               path,
               sds_name,
               sd_in)==FAIL)
               goto out;
       }          




       if (path) HDfree(path);
       return SUCCEED;
   }
   
  /*-------------------------------------------------------------------------
   * create new SDS
   * check for unlimited dimension
   * SDisrecord determines whether the data set identified by the parameter sds_id 
   * is appendable, which means that the slowest-changing dimension of the SDS array 
   * is declared unlimited when the data set is created.
   *-------------------------------------------------------------------------
   */
   /* do not use unlimited if compression is needed since combination of unlimited 
      and compression is not allowed. */
   if (SDisrecord(sds_id) && comp_type<=COMP_CODE_NONE) 
   {
       int32 dimsizes_cre[H4_MAX_VAR_DIMS];

       is_record = 1;

       for (j = 0; j < rank; j++) 
       {
           dimsizes_cre[j] = dimsizes[j];
       }
       dimsizes_cre[0] = SD_UNLIMITED;

       if ((sds_out = SDcreate(sd_out,sds_name,dtype,rank,dimsizes_cre)) == FAIL) 
       {
           printf( "Failed to create new SDS <%s>\n", path);
           goto out;
       }
       
   }
   else
   {
       
       if ((sds_out = SDcreate(sd_out,sds_name,dtype,rank,dimsizes)) == FAIL) 
       {
           printf( "Failed to create new SDS <%s>\n", path);
           goto out;
       }
   }
   
   
  /*-------------------------------------------------------------------------
   * set chunk 
   *
   * Chunked                  -> flags = HDF_CHUNK
   * Chunked and compressed   -> flags = HDF_CHUNK | HDF_COMP 
   * Non-chunked              -> flags = HDF_NONE
   *-------------------------------------------------------------------------
   */
   
   if (empty_sds==0 )
   {
       
       /* set chunk */
       if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
       {

           /* unlimited dimensions don't work with chunking */
           if ( ! is_record )
           {
               
               
               if (SDsetchunk (sds_out, chunk_def, chunk_flags)==FAIL)
               {
                   printf( "Error: Failed to set chunk dimensions for <%s>\n", path);
                   goto out;
               }
               
           }
          
           
       }
       
       /*-------------------------------------------------------------------------
       * set compression
       *
       * COMP_CODE_RLE       -> simple RLE encoding
       * COMP_CODE_SKPHUFF   -> Skipping huffman encoding
       * COMP_CODE_DEFLATE   -> gzip 'deflate' encoding
       *-------------------------------------------------------------------------
       */
       
       /* use compress without chunk-in */
       else if ( chunk_flags==HDF_NONE && comp_type>COMP_CODE_NONE)  
       {
           if ( nelms*eltsz<options->threshold )
           {
               /* reset to the original values . we don't want to uncompress if it was */
               comp_type=COMP_CODE_NONE;
               if (options->verbose) 
               {
                   printf("Warning: object size smaller than %d bytes. Not compressing <%s>\n",
                       options->threshold,path);
               } 
           } 
           else  
           {
               /* setup compression factors */
               switch(comp_type) 
               {
               case COMP_CODE_SZIP:
                   if (set_szip (info,szip_mode,&c_info)==FAIL)
                   {
                       comp_type=COMP_CODE_NONE;
                   }
                   break;
               case COMP_CODE_RLE:         
                   break;
               case COMP_CODE_SKPHUFF:     
                   c_info.skphuff.skp_size = info;
                   break;
               case COMP_CODE_DEFLATE:
                   c_info.deflate.level = info;
                   break;
               case COMP_CODE_NBIT:
                   comp_type = COMP_CODE_NONE;  /* not supported in this version */
                   break;
               default:
                   printf( "Error: Unrecognized compression code %d\n", comp_type);
                   goto out;
               }

                /*
                  HDF4 2.6 does not allow the combination of unlimited dimensions
                  and compression. See bug HDFFR-1280
                  The fix: for the case of the combination of unlimited dimensions
                  and compression, compressed will be applied but dimension is changed
                  to limited size in hrepack.
                */
               if (SDsetcompress (sds_out, comp_type, &c_info)==FAIL)
               {
                   printf( "Error: Failed to set compression for <%s>\n", path);
                   goto out;
               }
           }
       }
       
       
       need = (size_t)(nelms * eltsz); /* bytes needed */
       
       if ( need < H4TOOLS_MALLOCSIZE 
            ||
            /* for compressed datasets do one operation I/O, but allow hyperslab for chunked */
            (chunk_flags==HDF_NONE && comp_type>COMP_CODE_NONE))
       {
           buf = (VOIDP)HDmalloc(need);
       }
       
      /*-------------------------------------------------------------------------
       * read all
       *-------------------------------------------------------------------------
       */ 
       
       if ( buf!=NULL )
       {
           
           /* set edges of SDS, select all */
           for (i = 0; i < rank; i++) 
           {
               edges[i] = dimsizes[i];
               start[i] = 0;
           }
               
           
           /* read  */
           if (SDreaddata (sds_id, start, NULL, edges, buf) == FAIL) 
           {
               printf( "Could not read SDS <%s>\n", path);
               goto out;
           }
           
           /* write */
           if (SDwritedata(sds_out, start, NULL, edges, buf) == FAIL) 
           {
               printf( "Failed to write to new SDS <%s>\n", path);
               goto out;
           }


 
       }
       
       else /* possibly not enough memory, read/write by hyperslabs */
           
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
           
           for (i = rank; i > 0; --i) 
           {
               sm_size[i - 1] = MIN(dimsizes[i - 1], H4TOOLS_BUFSIZE / sm_nbytes);
               sm_nbytes *= sm_size[i - 1];
               assert(sm_nbytes > 0);
           }
           
           sm_buf = HDmalloc((size_t)sm_nbytes);
                      
           sm_nelmts = sm_nbytes / p_type_nbytes;
           
           /* the stripmine loop */
           HDmemset(hs_offset, 0, sizeof hs_offset);
           
           for (elmtno = 0; elmtno < p_nelmts; elmtno += hs_nelmts) 
           {
               /* calculate the hyperslab size */
               if (rank > 0) 
               {
                   for (i = 0, hs_nelmts = 1; i < rank; i++) 
                   {
                       hs_size[i] = MIN(dimsizes[i] - hs_offset[i], sm_size[i]);
                       hs_nelmts *= hs_size[i];
                   }
                   
               } 
               else 
               {
                   hs_nelmts = 1;
               } /* rank */

              
               
               /* read */
               if (SDreaddata (sds_id, hs_offset, NULL, hs_size, sm_buf) == FAIL) 
               {
                   printf( "Could not read SDS <%s>\n", sds_name);
                   goto out;
               }
              
               
               /* write */
               if (SDwritedata(sds_out, hs_offset, NULL, hs_size, sm_buf) == FAIL) 
               {
                   printf( "Failed to write to new SDS <%s>\n", path);
                   goto out;
               }


               
               /* calculate the next hyperslab offset */
               for (i = rank, carry = 1; i > 0 && carry; --i) 
               {
                   hs_offset[i - 1] += hs_size[i - 1];
                   if (hs_offset[i - 1] == dimsizes[i - 1])
                       hs_offset[i - 1] = 0;
                   else
                       carry = 0;
               } /* i */
           } /* elmtno */
           
           /* free */
           if (sm_buf!=NULL)
           {
            HDfree(sm_buf);
               sm_buf=NULL;
           }
           
           
       } /* hyperslab read */
 
  
   } /* empty_sds */


   
  /*-------------------------------------------------------------------------
   * copy attributes
   *-------------------------------------------------------------------------
   */ 
   
   if( copy_sds_attrs(sds_id,sds_out,nattrs,options)==FAIL) {
       goto out;
   }
   
  /*-------------------------------------------------------------------------
   * copy dimension scales
   *-------------------------------------------------------------------------
   */ 
   
   /* loop through each dimension up to rank of SDS */
   for (i = 0; i < rank; i++) 
   {
       int32 dim_size;
       
       /* get dimension handle for input dimension */
       if ((dim_id = SDgetdimid(sds_id, i)) == FAIL) {
           printf( "Failed to get dimension %d of SDS <%s>\n", i, path);
           goto out;
       }
       /* get dimension handle for output dimension */
       if ((dim_out = SDgetdimid(sds_out, i)) == FAIL) {
           printf( "Failed to get dim_id for dimension %d of SDS <%s>\n", i, path);
           goto out;
       }
       /* get dimension information for input dimension */
       if (SDdiminfo(dim_id, dim_name, &dim_size, &dtype, &nattrs) == FAIL) {
           printf( "Failed to get information for dimension %d of SDS <%s>\n", i, path);
           goto out;
       }
       
       /* add dimension name to dimension scales table */
       dim_table_add(td2,-1,dim_name);
       
       /* set output dimension name */
       if (SDsetdimname(dim_out, dim_name) == FAIL) {
           printf( "Failed to set dimension name %d of SDS <%s>\n", i, path);
           goto out;
       }
       /* copy attributes */
       if (nattrs && copy_sds_attrs(dim_id, dim_out, nattrs, options) == FAIL) {
           printf( "Failed to copy attributes for dimension %d of of SDS <%s>\n", i, path);
           goto out;
       }
       /* copy scale information over */
       if (dtype != 0) 
       {
           intn okdim;
           
           /* compute the number of the bytes for each value. */
           numtype = dtype & DFNT_MASK;
           eltsz = DFKNTsize(numtype | DFNT_NATIVE);
           
           if ((dim_buf = (VOIDP) HDmalloc(dimsizes[i] * eltsz)) == NULL) {
               printf( "Failed to alloc %ld for dimension scale\n", dimsizes[i]);
               goto out;
           }
           if ((okdim=SDgetdimscale(dim_id, dim_buf)) == FAIL) {
               printf( "Warning: Failed to get scale information for %s\n", dim_name);
           }
           if (okdim!=FAIL)
           {
               /* use dimsizes returned by SDgetinfo */
               if (SDsetdimscale(dim_out,dimsizes[i], dtype, dim_buf) == FAIL) {
                   printf( "Failed to set scale information for %s\n", dim_name);
                   goto out;
               }
           }
           HDfree(dim_buf);
       }
   }
   
   /* obtain the reference number of the new SDS using its identifier */
   if ((sds_ref = SDidtoref (sds_out)) == FAIL) {
       printf( "Failed to get new SDS reference in <%s>\n", path);
       goto out;
   }
   
  /*-------------------------------------------------------------------------
   * add SDS to group
   *-------------------------------------------------------------------------
   */ 
   
   /* add it to group, if present */
   if (vgroup_id_out_par) 
   {
       /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
       if (Vaddtagref (vgroup_id_out_par, TAG_GRP_DSET, sds_ref)==FAIL) {
           printf( "Failed to add new SDS to group <%s>\n", path);
           goto out;
       }
   }
   
  /*-------------------------------------------------------------------------
   * copy ANs
   *-------------------------------------------------------------------------
   */ 
   
   if (copy_an(infile_id,outfile_id,
       ref,tag,sds_ref,tag, 
       path,options)<0) {
       goto out;
   }
   

  /*-------------------------------------------------------------------------
   * close
   *-------------------------------------------------------------------------
   */ 
   
   
   if (SDendaccess(sds_id)== FAIL )
       printf( "Failed to close SDS <%s>\n", path);
   if (sds_out!=FAIL) {
       if (SDendaccess (sds_out)== FAIL )
           printf( "Failed to close SDS <%s>\n", path);
   }
  
   /*-------------------------------------------------------------------------
   * print compression
   *-------------------------------------------------------------------------
   */ 

   assert( options->trip==1 );
   if ( options->verbose )              
   {
       
       if (get_print_info(chunk_flags_in,
           &chunk_def,      
           comp_type,
           path,
           sds_name,
           sd_out)==FAIL)
           goto out;
   }          
   


   if (path)
    HDfree(path);
   if (buf)
    HDfree(buf);

   
   return SUCCEED;
   
   
out:
   if (SDendaccess(sds_id)== FAIL )
       printf( "Failed to close SDS <%s>\n", path);
   if (sds_out!=FAIL) {
       if (SDendaccess (sds_out)== FAIL )
           printf( "Failed to close SDS <%s>\n", path);
   }
   if (path)
    HDfree(path);
   if (buf)
    HDfree(buf);
   
   return FAIL;
   
}


/*-------------------------------------------------------------------------
 * Function: copy_sds_attrs
 *
 * Purpose: copy SD attributes from input file to output file 
 *   used for global, dataset and dimension attributes
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

int copy_sds_attrs(int32 id_in,
                   int32 id_out,
                   int32 nattrs,          
                   options_t *options)
{
    int32 dtype,               /* SDS data type */
        numtype,               /* number type */
        eltsz,                 /* element size */
        nelms;                 /* number of elements */
    char  attr_name[H4_MAX_NC_NAME];
    VOIDP attr_buf=NULL;
    int   i;
    
    /* loop through attributes in input SDS */
    for (i = 0; i < nattrs; i++) 
    {
        if (SDattrinfo (id_in, i, attr_name, &dtype, &nelms) == FAIL) {
            printf( "Cannot get information for attribute number %d\n", i);
            goto out;
        }
        /* compute the number of the bytes for each value. */
        numtype = dtype & DFNT_MASK;
        eltsz   = DFKNTsize(numtype | DFNT_NATIVE);
        if ((attr_buf = (VOIDP) HDmalloc(nelms * eltsz)) == NULL) {
            printf( "Error allocating %ld values of size %ld for attribute %s",
                nelms, numtype, attr_name);
            goto out;
        }
        /* read attributes from input SDS */
        if (SDreadattr(id_in, i, attr_buf) == FAIL) {
            printf( "Cannot read attribute %s\n", attr_name);
            goto out;
        }
        /* put attributes into output SDS */
        if (SDsetattr(id_out, attr_name, dtype, nelms, attr_buf) == FAIL) {
            printf( "Cannot write attribute %s\n", attr_name);
            goto out;
        }
        
        if (attr_buf)
            HDfree(attr_buf);
    }
    
    return SUCCEED;


out:

    if (attr_buf)
        HDfree(attr_buf);

    return FAIL;


}


/*-------------------------------------------------------------------------
 * get_print_info
 *-------------------------------------------------------------------------
 */

int get_print_info(  int chunk_flags,
                     HDF_CHUNK_DEF *chunk_def,      /* chunk definition */
                     int comp_type,
                     char *path,
                     char *sds_name,
                     int32 sd_id)
                    
{
    
    int32  comp_size;
    int32  uncomp_size;
    int32  sds_idx;
    int32  sds_id;
    double a, b, r=0;
    char   comp_str[255];
    int    is_record = 0;
    
    if ((sds_idx = SDnametoindex (sd_id, sds_name))==FAIL) 
        goto out;
    if ((sds_id = SDselect (sd_id, sds_idx))==FAIL) 
        goto out;
    if (SDisrecord(sds_id))
        is_record = 1;
    if (SDgetdatasize(sds_id, &comp_size, &uncomp_size) == FAIL) 
    {
        printf( "Could not get data sizes for <%s>\n", sds_name);
        goto out;
    }   
    if (SDendaccess (sds_id)== FAIL )
        goto out;
    
    sprintf(comp_str,"\0");

    /* unlimited dimensions don't work with compression */
    if ( is_record )
    {
        comp_type = COMP_CODE_NONE;
        chunk_flags = HDF_NONE;
    }
    
    if ( comp_type > COMP_CODE_NONE )
        
    {
        
        /* compression ratio = uncompressed size /  compressed size */
        a = uncomp_size; 
        b = comp_size;
        if ( b != 0 )
            r = a / b;
       
        sprintf(comp_str,"(%.2f:1)", r);
        
    }
    
    
    print_info(chunk_flags,
        chunk_def,      
        comp_type,           
        path,
        comp_str);  

    return SUCCEED;
    
out:
    return FAIL;
    
} 

/*-------------------------------------------------------------------------
 * print_info
 *-------------------------------------------------------------------------
 */

void print_info( int chunk_flags,
                 HDF_CHUNK_DEF *chunk_def,      /* chunk definition */
                 int comp_type,
                 char *path,
                 char *ratio)
{
    int pr_comp_type=0;
    int pr_chunk_flags;
    
    pr_chunk_flags=chunk_flags;
    
    if (comp_type>0)
    {
        pr_comp_type=comp_type;
    }
    else
    {
        if (pr_chunk_flags == (HDF_CHUNK | HDF_COMP) )
        {
            pr_comp_type=chunk_def->comp.comp_type;
        }
    }
    printf(PFORMAT,
        (pr_chunk_flags>0)?"chunk":"",                 /*chunk information*/
        (pr_comp_type>0)?get_scomp(pr_comp_type):"",   /*compression information*/
        ratio,                                         /*ratio */
        path);                                         /*name*/
}

