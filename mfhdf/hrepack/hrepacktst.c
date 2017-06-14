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

#include "hdf.h"
#include "mfhdf.h"
#include "hcomp.h"
#include "pal_rgb.h"

#if defined (H4_HAVE_LIBSZ)
#include "szlib.h"
#endif

#define HREPACK_FILE1         "hrepacktst1.hdf"
#define HREPACK_FILE1_OUT     "hrepacktst1_out.hdf"
#define HREPACK_FILE2         "hrepacktst2.hdf"
#define HREPACK_FILE2_OUT     "hrepacktst2_out.hdf"
#define HREPACK_FILE3         "hrepacktst3.hdf"
#define HREPACK_FILE3_OUT     "hrepacktst3_out.hdf"
#define DATA_FILE1            "image8.txt"
#define DATA_FILE2            "image24pixel.txt"
#define DATA_FILE3            "image24plane.txt"
#define TAG_GRP_IMAGE DFTAG_RIG
#define TAG_GRP_DSET  DFTAG_NDG

#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout); }
#define PASSED() {puts(" PASSED");fflush(stdout);}
#define SKIPPED() {puts(" SKIPPED");fflush(stdout);}
#define H4_FAILED() {puts("*FAILED*");fflush(stdout);}

/* AN labels */
#define  FILE_LABEL_TXT "This is a file label"
#define  FILE_DESC_TXT  "This is a file description"
#define  DATA_LABEL_TXT "This is a data label"
#define  DATA_DESC_TXT  "This is a data annotation"
/* dimensions of image */
#define X_DIM_GR   60
#define Y_DIM_GR   400
/* dimensions of dataset */
#define X_DIM      20
#define Y_DIM      800
#define Z_DIM      2
/* dimensions */
#define XD1        60
#define YD1        40
/* dimensions for hyperslab sds */
#define DIM0       10
#define DIM1       10
#define ADD_ROWS ( 1024 * 1024 - 10 ) / 10 
/* Vdata */
#define  N_RECORDS        3         /* number of records the vdata contains */
#define  ORDER_1          3         /* order of first field */
#define  ORDER_2          1         /* order of second field */
#define  ORDER_3          2         /* order of third field */
#define  CLASS_NAME       "Particle"
#define  FIELD1_NAME      "Position"      /* contains x, y, z values */
#define  FIELD2_NAME      "Mass"          /* contains weight values */
#define  FIELD3_NAME      "Temperature"   /* contains min and max values */
#define  FIELDNAME_LIST   "Position,Mass,Temperature" /* No spaces b/w names */
#define  N_VALS_PER_REC   (ORDER_1 + ORDER_2 + ORDER_3)  /* number of values per record */

/*-------------------------------------------------------------------------
 * global variables for read image data, used in gr, r8 and r24 add
 *-------------------------------------------------------------------------
 */ 

static int            g_lenght_x;
static int            g_lenght_y;
static int            g_ncomps;
static unsigned char *g_image_data = NULL;   


/*-------------------------------------------------------------------------
 * read_data
 * utility function to read ASCII image data
 * the files have a header of the type
 *
 *   components
 *   n
 *   height
 *   n
 *   width
 *   n
 * 
 * followed by the image data
 *
 *-------------------------------------------------------------------------
 */

static
int read_data(const char* fname)
{
    int    i, n;
    int    color_planes;
    char   str[20];
    FILE   *f;
    int    w, h;
    
    f = fopen( fname, "r");
    
    if ( f == NULL )
    {
        printf( "Could not open file <%s>\n", fname );
        return -1;
    }
    
    fscanf( f, "%s", str );
    fscanf( f, "%d", &color_planes );
    fscanf( f, "%s", str );
    fscanf( f, "%d", &h); 
    fscanf( f, "%s", str );
    fscanf( f, "%d", &w); 
    
    /* globals */
    g_ncomps=color_planes;
    g_lenght_y=h;
    g_lenght_x=w;
    
    if ( g_image_data != NULL )
    {
        HDfree( g_image_data );
        g_image_data=NULL;
    }
    
    g_image_data = (unsigned char*)HDmalloc(w*h*color_planes*sizeof(unsigned char));
    
    for (i = 0; i < h*w*color_planes ; i++)
    {
        fscanf( f, "%d",&n );
        g_image_data[i] = (unsigned char)n;
    }
    fclose(f);
    
    return 1;
    
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
    uint16      name_len;
    char        *vgname;

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
        /* Get vgroup's name */
        if (Vgetnamelen(vg, &name_len)==FAIL)
        {
            printf("Error: Could not get name length for group with ref <%ld>\n", vgid);
            continue;
        }
        vgname = (char *) HDmalloc(sizeof(char) * (name_len+1));

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
static
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
 * Function: set_chunk_def
 *
 * Purpose: set chunk parameters. used by GR and SDS
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 15, 2003
 *
 *-------------------------------------------------------------------------
 */

static
void set_chunk_def( comp_coder_t comp_type, 
                    int32 *dim,
                    int32 ncomps,
                    int32 bits_per_pixel, /* for szip */
                    HDF_CHUNK_DEF *chunk_def )
{
    
    /* Define chunk's dimensions */
    chunk_def->chunk_lengths[0] = dim[0]/2;
    chunk_def->chunk_lengths[1] = dim[1]/2;
    /* To use chunking with RLE, Skipping Huffman, GZIP, SZIP compression */
    chunk_def->comp.chunk_lengths[0] = dim[0]/2;
    chunk_def->comp.chunk_lengths[1] = dim[1]/2;
    
    /*define some compression specific parameters */
    switch(comp_type)
    {
    default:
        break;
    case COMP_CODE_RLE:
        chunk_def->comp.comp_type = COMP_CODE_RLE;
        break;
        
    case COMP_CODE_SKPHUFF:
        chunk_def->comp.comp_type = COMP_CODE_SKPHUFF;
        chunk_def->comp.cinfo.skphuff.skp_size = 1;
        break;
        
    case COMP_CODE_DEFLATE:
        /* GZIP compression, set compression type, flag and deflate level*/
        chunk_def->comp.comp_type = COMP_CODE_DEFLATE;
        chunk_def->comp.cinfo.deflate.level = 6;
        break;
        
    case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
        if (SZ_encoder_enabled()) {
            chunk_def->comp.cinfo.szip.pixels_per_block = 2;
            chunk_def->comp.cinfo.szip.options_mask = SZ_EC_OPTION_MASK;
            chunk_def->comp.cinfo.szip.options_mask |= SZ_RAW_OPTION_MASK;
            chunk_def->comp.cinfo.szip.pixels = 0;
            chunk_def->comp.cinfo.szip.pixels_per_scanline = 0;
            chunk_def->comp.cinfo.szip.bits_per_pixel = 0;
        } else {
            printf("Warning: SZIP encoding not available\n");
        }
#else
        printf("Warning: SZIP compression not available\n");
#endif
        break;
    }
    
}


/*-------------------------------------------------------------------------
 * verify functions
 *-------------------------------------------------------------------------
 */

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

static 
int cmp_gr(int32 ri1_id, int32 ri2_id)
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
        HDfree(buf1);
    if (buf2)
        HDfree(buf2);
    return cmp;
    
}

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
static
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
    SDgetcompinfo(sds_id, &comp_type, &comp_info);
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
static
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
    int           is_record = 0;
    
    /* initialize the sd interface */
    sd_id  = SDstart (HREPACK_FILE1_OUT, DFACC_READ);
    
    /* determine the number of data sets in the file */
    if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL) 
    {
        printf("Error: Cannot get file information");
        SDend (sd_id);
        return -1;
    }
    
    for (sds_index = 0; sds_index < n_datasets; sds_index++)
    {
        sds_id   = SDselect (sd_id, sds_index);
        
        /* skip dimension scales */
        if ( SDiscoordvar(sds_id) ) 
        {
            SDendaccess(sds_id);
            continue;
        }
        
        name[0] = '\0';
        if ( SDgetinfo(sds_id, name, &rrank, dim_sizes, &data_type, &n_attrs) == FAIL )
        {
            printf("Error: can't read info for SDS <%s>",name);
            SDendaccess (sds_id);
            SDend (sd_id);
            return -1;
        }
        
        /*-------------------------------------------------------------------------
        * check if the input SDS is empty
        *-------------------------------------------------------------------------
        */ 
        if (SDcheckempty( sds_id, &empty_sds ) == FAIL) 
        {
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

            if (SDisrecord(sds_id))
                is_record = 1;

            /* unlimited dimensions don't work with compression */
            if ( ! is_record )
            {
                
                
                if ( SDgetcompinfo(sds_id, &comp_type, &comp_info) == FAIL )
                {
                    printf("Warning: can't read compression for SDS <%s>",name);
                } 
                else 
                {
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
                
            } /* is_record */
            
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
static
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
static
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
 * add functions
 *-------------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 * Function: add_an
 *
 * Purpose: utility function to write a AN
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 19, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_an(int32 file_id, int32 tag, int32 ref)
{
    int32 an_id,        /* AN interface identifier */
          data_label_id,  /* data label identifier */
          data_desc_id;   /* data description identifier */
    
    /* Initialize the AN interface */
    an_id = ANstart (file_id);
    
   /*-------------------------------------------------------------------------
    * data labels and annotations
    *-------------------------------------------------------------------------
    */ 
    
    /* Create the data label for the object identified by its tag and ref number */
    data_label_id = ANcreate (an_id, (uint16)tag, (uint16)ref, AN_DATA_LABEL);
    
    /* Write the annotation text to the data label */
    if (ANwriteann (data_label_id, DATA_LABEL_TXT, strlen (DATA_LABEL_TXT))==FAIL){
        printf("Error: writing data label in tag %ld ref %ld\n", tag, ref);
        return FAIL;
    }
    
    /* Create the data description for the object identified by its tag and ref number */
    data_desc_id = ANcreate (an_id, (uint16)tag, (uint16)ref, AN_DATA_DESC);
    
    /* Write the annotation text to the data description */
    if (ANwriteann (data_desc_id, DATA_DESC_TXT, strlen (DATA_DESC_TXT))==FAIL){
        printf("Error: writing data label in tag %ld ref %ld\n", tag, ref);
        return FAIL;
    }
    
    /* Terminate access to each annotation explicitly */
    if (ANendaccess (data_label_id)==FAIL||
        ANendaccess (data_desc_id)==FAIL){
        printf( "Failed to close AN\n");
        return FAIL;
    }
    
    /* Terminate access to the AN interface */
    if (ANend (an_id)==FAIL){
        printf( "Failed to close AN\n");
        return FAIL;
    }
    
    return SUCCEED;
}






/*-------------------------------------------------------------------------
 * Function: add_gr_ffile
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  GR - Multifile General Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */
static
int add_gr_ffile(const char* name_file,
                  int32 gr_id,
                  const char* gr_name,
                  int32 interlace_mode,
                  int32 file_id,
                  int32 vgroup_id)
{
    int32  ri_id,          /* raster image identifier */
           gr_ref,         /* reference number of the GR image */
           start[2],       /* start position to write for each dimension */
           edges[2],       /* number of elements to be written along each dimension */
           dim_gr[2],      /* dimension sizes of the image array */
           data_type;      /* data type of the image data */
    char   *srcdir = getenv("srcdir"); /* the source directory */
    char   data_file[512]="";          /* buffer to hold name of existing data file */
    uint8  attr_values[2]={1,2};
    int    n_values;
    
    /* compose the name of the file to open, using the srcdir, if appropriate */
    if ( srcdir )
    {
        HDstrcpy(data_file, srcdir);
        HDstrcat(data_file, "/");
    }
    HDstrcat( data_file, name_file);

    if ( read_data(data_file) > 0 )
    {
        /* set the data type, interlace mode, and dimensions of the image */
        data_type = DFNT_UINT8;
        dim_gr[0] = g_lenght_x;
        dim_gr[1] = g_lenght_y;
        
        /* create the raster image array */
        if ((ri_id = GRcreate (gr_id, gr_name, g_ncomps, data_type, interlace_mode, dim_gr))== FAIL)
        {
            printf("Error: Could not create GR <%s>\n", gr_name);
            return FAIL;
        }
        
        /* define the size of the data to be written */
        start[0] = start[1] = 0;
        edges[0] = g_lenght_x;
        edges[1] = g_lenght_y;
        
        
        /* write the data in the buffer into the image array */
        if (GRwriteimage(ri_id, start, NULL, edges, (VOIDP)g_image_data)==FAIL)
        {
            printf("Error: Could not write GR <%s>\n", gr_name);
        }
        
        /* assign an attribute to the SDS */
        n_values = 2;
        if(GRsetattr(ri_id, "Myattr", DFNT_UINT8, n_values, (VOIDP)attr_values)==FAIL)
        {
            printf("Error: Could not write attributes for GR <%s>\n", gr_name);
            return FAIL;
        }
        
        /* obtain the reference number of the GR using its identifier */
        gr_ref = GRidtoref (ri_id);
        
        /* add the GR to the vgroup. the tag DFTAG_RIG is used */
        if (vgroup_id) 
        {
            if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, gr_ref)==FAIL)
            {
                printf("Error: Could not add GR <%s> to group\n", gr_name);
                return FAIL;
            }
        }
        
        /* terminate access to the raster image */
        if (GRendaccess (ri_id)==FAIL)
        {
            printf("Error: Could not close GR <%s>\n", gr_name);
            return FAIL;
        }
        
        /* add an annotation and label to the object */
        if (add_an(file_id, DFTAG_RI, gr_ref)<0)
            return FAIL;
        
    }  /* read data */
    
    if ( g_image_data != NULL )
    {
        HDfree( g_image_data );
        g_image_data=NULL;
    }
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_gr
 *
 * Purpose: utility function to write images with the
 *  GR - Multifile General Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 18, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_gr(const char* gr_name,     /* gr name */
            int32 file_id,           /* file ID */
            int32 gr_id,             /* GR ID */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            comp_coder_t comp_type,  /* compression flag */
            comp_info *comp_info     /* compression structure */ )
{
    int32  ri_id,          /* raster image identifier */
           gr_ref,         /* reference number of the GR image */
           start[2],       /* start position to write for each dimension */
           edges[2],       /* number of elements to be written along each dimension */
           dim_gr[2],      /* dimension sizes of the image array */
           interlace_mode, /* interlace mode of the image */
           data_type,      /* data type of the image data */
           data[Y_DIM_GR][X_DIM_GR];
    int    i, j, n=0, ncomps=1;
    HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
    
    /* set the data type, interlace mode, and dimensions of the image */
    data_type = DFNT_UINT32;
    interlace_mode = MFGR_INTERLACE_PIXEL;
    dim_gr[0] = Y_DIM_GR;
    dim_gr[1] = X_DIM_GR;
    
    /* data set data initialization */
    for (j = 0; j < Y_DIM_GR; j++) {
        for (i = 0; i < X_DIM_GR; i++)
            data[j][i] = n++;
    }
    
    /*define some compression specific parameters */
    switch(comp_type)
    {
    default:
        break;
    case COMP_CODE_RLE:
        break;
        
    case COMP_CODE_SKPHUFF:
        comp_info->skphuff.skp_size = 1;
        break;
        
    case COMP_CODE_DEFLATE:
        comp_info->deflate.level = 6;
        break;
        
    case COMP_CODE_SZIP:
#ifdef H4_GR_SZIP
        /* not supported for GR */
#ifdef H4_HAVE_LIBSZ
        if (SZ_encoder_enabled()) {
            comp_info->szip.pixels_per_block = 2;
            comp_info->szip.options_mask = SZ_EC_OPTION_MASK;
            comp_info->szip.options_mask |= SZ_RAW_OPTION_MASK;
            comp_info->szip.pixels = 0;
            comp_info->szip.pixels_per_scanline = 0;
            comp_info->szip.bits_per_pixel = 0;
        } else {
            printf("Warning: SZIP encoding not available\n");
        }
#else
        printf("Warning: SZIP compression not available\n");
#endif
#endif
        printf("Warning: SZIP compression not available for GR\n");
        break;
    }
    
    /* create the raster image array */
    if ((ri_id = GRcreate (gr_id, gr_name, ncomps, data_type, interlace_mode, dim_gr))== FAIL)
    {
        printf("Error: Could not create GR <%s>\n", gr_name);
        return FAIL;
    }
    
    /* set chunk */
    if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
    {
        /* Define chunk's dimensions */
        chunk_def.chunk_lengths[0] = Y_DIM_GR/2;
        chunk_def.chunk_lengths[1] = X_DIM_GR/2;
        /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
        chunk_def.comp.chunk_lengths[0] = Y_DIM_GR/2;
        chunk_def.comp.chunk_lengths[1] = X_DIM_GR/2;
        
        /*define some compression specific parameters */
        switch(comp_type)
        {
        default:
            break;
        case COMP_CODE_RLE:
            chunk_def.comp.comp_type = COMP_CODE_RLE;
            break;
            
        case COMP_CODE_SKPHUFF:
            chunk_def.comp.comp_type = COMP_CODE_SKPHUFF;
            chunk_def.comp.cinfo.skphuff.skp_size = 1;
            break;
            
        case COMP_CODE_DEFLATE:
            /* GZIP compression, set compression type, flag and deflate level*/
            chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
            chunk_def.comp.cinfo.deflate.level = 6;
            break;
            
        case COMP_CODE_SZIP:
#ifdef H4_GR_SZIP
#ifdef H4_HAVE_LIBSZ
            if (SZ_encoder_enabled()) {
                chunk_def.comp.cinfo.szip.pixels_per_block = 2;
                chunk_def.comp.cinfo.szip.options_mask = SZ_EC_OPTION_MASK;
                chunk_def.comp.cinfo.szip.options_mask |= SZ_RAW_OPTION_MASK;
                chunk_def.comp.cinfo.szip.pixels = 0;
                chunk_def.comp.cinfo.szip.pixels_per_scanline = 0;
                chunk_def.comp.cinfo.szip.bits_per_pixel = 0;
            } else {
                printf("Warning: SZIP encoding not available\n");
            }
#else
            printf("Warning: SZIP compression not available\n");
#endif
#endif
            printf("Warning: SZIP compression not available for GR\n");
            break;
        }
        if(GRsetchunk (ri_id, chunk_def, chunk_flags)==FAIL)
        {
            printf("Error: Could not set chunk for GR <%s>\n", gr_name);
            return FAIL;
        }
    }
    
    /* use compress without chunk-in */
    else if ( (chunk_flags==HDF_NONE || chunk_flags==HDF_CHUNK) && 
        comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
    {
        if(GRsetcompress (ri_id, comp_type, comp_info)==FAIL)
        {
            printf("Error: Could not set compress for GR <%s>\n", gr_name);
            return FAIL;
        }
    }
    
    
    /* define the size of the data to be written */
    start[0] = start[1] = 0;
    edges[0] = Y_DIM_GR;
    edges[1] = X_DIM_GR;
    
    /* write the data in the buffer into the image array */
    if (GRwriteimage(ri_id, start, NULL, edges, (VOIDP)data)==FAIL)
    {
        printf("Error: Could not set write GR <%s>\n", gr_name);
        return FAIL;
    }
    
    /* obtain the reference number of the GR using its identifier */
    gr_ref = GRidtoref (ri_id);
    
    /* add the GR to the vgroup. the tag DFTAG_RIG is used */
    if (vgroup_id)
    {
        if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, gr_ref)==FAIL)
        {
            printf("Error: Could not add GR <%s> to group\n", gr_name);
            return FAIL;
        }
    }
        
    /* terminate access to the raster image */
    if (GRendaccess (ri_id)==FAIL)
    {
        printf("Error: Could not close GR <%s>\n", gr_name);
        return FAIL;
    }
    
    /* add an annotation and label to the object */
    if (add_an(file_id, DFTAG_RI, gr_ref)<0)
        return FAIL;
    
    return SUCCEED;
    
}

/*-------------------------------------------------------------------------
 * Function: add_glb_attrs
 *
 * Purpose: utility function to write global attributes
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */
static
int add_glb_attrs(const char *fname,
                   int32 file_id,
                   int32 sd_id,
                   int32 gr_id)
                   
{
    uint8 attr_values[2]={1,2};
    int   n_values=2;
    
   /*-------------------------------------------------------------------------
    * make SDS global attributes
    *-------------------------------------------------------------------------
    */ 
    /* assign an attribute to the SD */
    if (SDsetattr(sd_id, "MySDgattr", DFNT_UINT8, n_values, (VOIDP)attr_values)==FAIL){
        printf("Could not set SDS attr\n");
        return FAIL;
    }
    
   /*-------------------------------------------------------------------------
    * make GR global attributes
    *-------------------------------------------------------------------------
    */ 
    
    /* assign an attribute to the GR */
    if (GRsetattr(gr_id, "MyGRgattr", DFNT_UINT8, n_values, (VOIDP)attr_values)==FAIL){
        printf("Could not set GR attr\n");
        return FAIL;
    }
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_r8
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  DFR8 - Single-file 8-Bit Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_r8(const char* image_file,
            const char *fname,
            int32 file_id,
            int32 vgroup_id)
{
    int32  ri_ref;                     /* reference number of the GR image */
    char   *srcdir = getenv("srcdir"); /* the source directory */
    char   data_file[512]="";          /* buffer to hold name of existing data file */
    
    /* compose the name of the file to open, using the srcdir, if appropriate */
    if ( srcdir )
    {
        HDstrcpy(data_file, srcdir);
        HDstrcat(data_file, "/");
    }
    HDstrcat( data_file, image_file);

    if ( read_data(data_file) > 0 )
    {
        /* add a palette */
        if (DFR8setpalette(pal_rgb)==FAIL){
            printf( "Could not set palette for image\n");
            return FAIL;
        }
        
        /* write the image */
        if (DFR8addimage(fname, g_image_data, g_lenght_x, g_lenght_y, (uint16)0)==FAIL){
            printf( "Could not write palette for image\n");
            return FAIL;
        }
        
        /* obtain the reference number of the RIS8 */
        ri_ref = DFR8lastref();
        
        /* add the image to the vgroup. the tag DFTAG_RIG is used */
        if (vgroup_id)
        {
            if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, ri_ref)==FAIL){
                printf( "Could not add image to group\n");
                return FAIL;
            }
        }
        
        /* add an annotation and label to the object */
        if (add_an(file_id, TAG_GRP_IMAGE, ri_ref)<0)
            return FAIL;
    }
    
    if ( g_image_data != NULL )
    {
        HDfree( g_image_data );
        g_image_data=NULL;
    }
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_r24
 *
 * Purpose: utility function to read an image data file and save the image with the
 *  DF24 - Single-file 24-Bit Raster Image Interface,
 *  optionally inserting the image into the group VGROUP_ID
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */
static
int add_r24(const char* image_file,
            const char *fname,
            int32 file_id,
            intn il,
            int32 vgroup_id)
{
    int32  ri_ref;                      /* reference number of the GR image */
    char   *srcdir = getenv("srcdir");  /* the source directory */
    char   data_file[512]="";           /* buffer to hold name of existing data file */
    
    /* compose the name of the file to open, using the srcdir, if appropriate */
    if ( srcdir )
    {
        HDstrcpy(data_file, srcdir);
        HDstrcat(data_file, "/");
    }
    HDstrcat( data_file, image_file);

    if ( read_data(data_file) > 0 )
    {
        /* set interlace */
        if (DF24setil(il)==FAIL){
            printf( "Could not set interlace for image\n");
            return FAIL;
        }
        
        /* write the image */
        if (DF24addimage(fname, g_image_data, g_lenght_x, g_lenght_y)==FAIL){
            printf( "Could not write image\n");
            return FAIL;
        }
        
        /* obtain the reference number of the RIS24 */
        ri_ref = DF24lastref();
        
        /* add the image to the vgroup. the tag DFTAG_RIG is used */
        if (vgroup_id)
        {
            if (Vaddtagref (vgroup_id, TAG_GRP_IMAGE, ri_ref)==FAIL)
            {
                printf( "Could not set group for image\n");
                return FAIL;
            }
        }
        
        /* add an annotation and label to the object */
        if (add_an(file_id, TAG_GRP_IMAGE, ri_ref)<0)
            return FAIL;
    
    } /* read_data */
    
    if ( g_image_data != NULL )
    {
        HDfree( g_image_data );
        g_image_data=NULL;
    }
    
    return SUCCEED;
}



/*-------------------------------------------------------------------------
 * Function: add_sd
 *
 * Purpose: utility function to write with
 *  SD - Multifile Scientific Data Interface,
 *  optionally :
 *   1)inserting the SD into the group VGROUP_ID
 *   2)making the dataset chunked and/or compressed
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */
static
int add_sd(const char *fname,       /* file name */
            int32 file_id,           /* file ID */
            int32 sd_id,             /* SD id */
            const char* sds_name,    /* sds name */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            comp_coder_t comp_type,  /* compression flag */
            comp_info *comp_info     /* compression structure */ )

{
    int32  sds_id,       /* data set identifier */
           sds_ref,      /* reference number of the data set */
           dim_sds[2],   /* dimension of the data set */
           rank = 2,     /* rank of the data set array */
           n_values,     /* number of values of attribute */
           dim_index,    /* dimension index */
           dim_id,       /* dimension ID */
           start[2],     /* write start */
           edges[2],     /* write edges */
           fill_value=2, /* fill value */
           data[Y_DIM][X_DIM],
           bits_per_pixel=32;
    float32 sds_values[2] = {2., 10.}; /* values of the SDS attribute  */
    int16   data_X[X_DIM];             /* X dimension dimension scale */
    float64 data_Y[Y_DIM];             /* Y dimension dimension scale */
    int     i, j;
    HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
    
    /* set the size of the SDS's dimension */
    dim_sds[0] = Y_DIM;
    dim_sds[1] = X_DIM;
    
    /*define some compression specific parameters */
    switch(comp_type)
    {
    default:
        break;
    case COMP_CODE_RLE:
        break;
        
    case COMP_CODE_SKPHUFF:
        comp_info->skphuff.skp_size = 1;
        break;
        
    case COMP_CODE_DEFLATE:
        comp_info->deflate.level = 6;
        break;
        
    case COMP_CODE_SZIP:
#ifdef H4_HAVE_LIBSZ
        if (SZ_encoder_enabled()) {
            comp_info->szip.pixels_per_block = 2;
            comp_info->szip.options_mask = SZ_EC_OPTION_MASK;
            comp_info->szip.options_mask |= SZ_RAW_OPTION_MASK;
            comp_info->szip.pixels = 0;
            comp_info->szip.pixels_per_scanline = 0;
            comp_info->szip.bits_per_pixel = 0;
        } else {
            printf("Warning: SZIP encoding not available\n");
        }
#else
        printf("Warning: SZIP compression not available\n");
#endif
        break;
    }
    
    /* data set data initialization */
    for (j = 0; j < Y_DIM; j++) {
        for (i = 0; i < X_DIM; i++)
            data[j][i] = (i + j) + 1;
    }
    /* initialize dimension scales */
    for (i=0; i < X_DIM; i++) data_X[i] = i;
    for (i=0; i < Y_DIM; i++) data_Y[i] = 0.1 * i;
    
    /* create the SDS */
    if ((sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds))<0)
    {
        printf( "Could not create SDS <%s>\n",sds_name);
        return FAIL;
    }
    
    /* set chunk */
    if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
    {
        set_chunk_def(comp_type, 
            dim_sds,
            1,
            bits_per_pixel,
            &chunk_def);
        if (SDsetchunk (sds_id, chunk_def, chunk_flags)==FAIL)  {
            printf( "Failed to set chunk for SDS <%s>\n", sds_name);
            goto fail;
        }
    }
    
    /* use compress without chunk-in */
    else if ( chunk_flags==HDF_NONE && 
        comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
    {
        if(SDsetcompress (sds_id, comp_type, comp_info)==FAIL){
            printf( "Failed to set compress for SDS <%s>\n", sds_name);
            goto fail;
        } 
    }
    
    /* set a fill value */
    if (SDsetfillvalue (sds_id, (VOIDP)&fill_value)==FAIL){
        printf( "Failed to set fillvaclue for SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    /* define the location and size of the data to be written to the data set */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_DIM;
    edges[1] = X_DIM;
    
    /* write the stored data to the data set */
    if (SDwritedata (sds_id, start, NULL, edges, (VOIDP)data)==FAIL){
        printf( "Failed to set write for SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    /* assign an attribute to the SDS */
    n_values = 2;
    if (SDsetattr(sds_id,"Valid_range",DFNT_FLOAT32,n_values,(VOIDP)sds_values)==FAIL){
        printf( "Failed to set attr for SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    /*  For each dimension of the data set specified in SDS_NAME,
    *  get its dimension identifier and set dimension name
    *  and dimension scale. Note that data type of dimension scale 
    *  can be different between dimensions and can be different from 
    *  SDS data type.
    */
    for (dim_index = 0; dim_index < rank; dim_index++) 
    {
        /* select the dimension at position dim_index */
        dim_id = SDgetdimid (sds_id, dim_index);
        
        /* assign name and dimension scale to selected dimension */
        switch (dim_index)
        {
        case 0: 
            n_values = Y_DIM;
            
            if (SDsetdimname (dim_id, "Y_Axis")==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            } 
            if (SDsetdimscale (dim_id,n_values,DFNT_FLOAT64,(VOIDP)data_Y)==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            }   
            if (SDsetattr (dim_id, "info", DFNT_CHAR8, 7,"meters")==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            } 
            break;
        case 1: 
            n_values = X_DIM; 
            
            if (SDsetdimname (dim_id, "X_Axis")==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            } 
            if (SDsetdimscale (dim_id,n_values,DFNT_INT16,(VOIDP)data_X)==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            } 
            if (SDsetattr (dim_id, "info", DFNT_CHAR8, 5,"feet")==FAIL){
                printf( "Failed to set dims for SDS <%s>\n", sds_name);
                goto fail;
            } 
            break;
        default: 
            break;
        }
    }
    
    /* obtain the reference number of the SDS using its identifier */
    sds_ref = SDidtoref (sds_id);
    
    /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
    if (vgroup_id)
    {
        if (Vaddtagref (vgroup_id, TAG_GRP_DSET, sds_ref)==FAIL)
        {
            printf( "Failed to add ref for SDS <%s>\n", sds_name);
            goto fail;
        } 
    }
    
    /* add an annotation and label to the object */
    add_an(file_id, TAG_GRP_DSET, sds_ref);
    
    /* terminate access to the SDS */
    if (SDendaccess (sds_id)==FAIL){
        printf( "Failed to end SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    return SUCCEED;
        
fail:
    SDendaccess (sds_id);
    return FAIL;
}


/*-------------------------------------------------------------------------
 * Function: add_sd3d
 *
 * Purpose: utility function to write with
 *  SD - Multifile Scientific Data Interface,
 *  optionally :
 *   1)inserting the SD into the group VGROUP_ID
 *   2)making the dataset chunked and/or compressed
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_sd3d(const char *fname,       /* file name */
              int32 file_id,           /* file ID */
              int32  sd_id,            /* SD interface identifier */
              const char* sds_name,    /* sds name */
              int32 vgroup_id,         /* group ID */
              int32 chunk_flags,       /* chunk flags */
              comp_coder_t comp_type,  /* compression flag */
              comp_info *comp_info     /* compression structure */ )

{
    int32  sds_id,       /* data set identifier */
           sds_ref,      /* reference number of the data set */
           dim_sds[3],   /* dimension of the data set */
           rank = 3,     /* rank of the data set array */
           start[3],     /* write start */
           fill_value=2, /* fill value */
           data[Z_DIM][Y_DIM][X_DIM];
    int    i, j, k;
    HDF_CHUNK_DEF chunk_def;           /* Chunking definitions */ 
    
    
    /* Define chunk's dimensions */
    chunk_def.chunk_lengths[0] = Z_DIM/2;
    chunk_def.chunk_lengths[1] = Y_DIM/2;
    chunk_def.chunk_lengths[2] = X_DIM/2;
    /* To use chunking with RLE, Skipping Huffman, and GZIP compression */
    chunk_def.comp.chunk_lengths[0] = Y_DIM/2;
    chunk_def.comp.chunk_lengths[1] = Y_DIM/2;
    chunk_def.comp.chunk_lengths[2] = X_DIM/2;
    
    /* GZIP compression, set compression type, flag and deflate level*/
    chunk_def.comp.comp_type = COMP_CODE_DEFLATE;
    chunk_def.comp.cinfo.deflate.level = 6;             
    
    /* data set data initialization */
    for (k = 0; k < Z_DIM; k++){
        for (j = 0; j < Y_DIM; j++) 
            for (i = 0; i < X_DIM; i++)
                data[k][j][i] = (i + j) + 1;
    }
    
    /* set the size of the SDS's dimension */
    dim_sds[0] = Z_DIM;
    dim_sds[1] = Y_DIM;
    dim_sds[2] = X_DIM;
    
    /* create the SDS */
    if ((sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds))<0)
    {
        printf( "Could not create SDS <%s>\n",sds_name);
        return FAIL;
    }
    
    /* set chunk */
    if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
    {
        if (SDsetchunk (sds_id, chunk_def, chunk_flags)==FAIL){
            printf( "Failed to set chunk for SDS <%s>\n", sds_name);
            goto fail;
        } 
    }
    
    /* use compress without chunk-in */
    else if ( (chunk_flags==HDF_NONE) && comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
    {
        if (SDsetcompress (sds_id, comp_type, comp_info)==FAIL){
            printf( "Failed to set compress for SDS <%s>\n", sds_name);
            goto fail;
        }  
    }
    
    /* set a fill value */
    if (SDsetfillvalue (sds_id, (VOIDP)&fill_value)==FAIL)
    {
        printf( "Failed to set fill for SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    /* define the location and size of the data to be written to the data set */
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    
    /* write the stored data to the data set */
    if (SDwritedata (sds_id, start, NULL, dim_sds, (VOIDP)data)==FAIL)
    {
        printf( "Failed to write SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    /* obtain the reference number of the SDS using its identifier */
    sds_ref = SDidtoref (sds_id);
    
    /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
    if (vgroup_id)
    {
        if (Vaddtagref (vgroup_id, TAG_GRP_DSET, sds_ref)==FAIL)
        {
            printf( "Failed to set ref for SDS <%s>\n", sds_name);
            goto fail;
        } 
    }
    
    /* add an annotation and label to the object */
    add_an(file_id, TAG_GRP_DSET, sds_ref);
    
    /* terminate access to the SDS */
    if (SDendaccess (sds_id)==FAIL)
    {
        printf( "Failed to end SDS <%s>\n", sds_name);
        goto fail;
    } 
    
    return SUCCEED;
    
fail:
    SDendaccess (sds_id);
    return FAIL;
}



/*-------------------------------------------------------------------------
 * Function: add_empty_sd
 *
 * Purpose: utility function to write an empty sds
 *
 *-------------------------------------------------------------------------
 */
static
int add_empty_sd(int32 sd_id,             /* SD id */
                 const char* sds_name     /* sds name */
                )

{
    int32  sds_id,       /* data set identifier */
           dim_sds[2],   /* dimension of the data set */
           rank = 2;     /* rank of the data set array */
    
    /* set the size of the SDS's dimension */
    dim_sds[0] = Y_DIM;
    dim_sds[1] = X_DIM;
    
    /* create the SDS */
    if ((sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds))<0)
    {
        printf( "Could not create SDS <%s>\n",sds_name);
        return FAIL;
    }
    
    /* terminate access to the SDS */
    if (SDendaccess (sds_id)==FAIL)
    {
        printf( "Failed to end SDS <%s>\n", sds_name);
        return FAIL;
    } 
    
    return SUCCEED;
    
}

/*-------------------------------------------------------------------------
 * Function: add_unl_sd
 *
 * Purpose: utility function to create a sds with unlimited dimension
 *
 *-------------------------------------------------------------------------
 */
static
int add_unl_sd(int32 sd_id,             /* SD id */
               const char* sds_name,    /* sds name */
               int do_write
                )

{
    int32  sds_id,       /* data set identifier */
           dim_sds[2],   /* dimension of the data set */
           rank = 2;     /* rank of the data set array */
    
    /* set the size of the SDS's dimension */
    dim_sds[0] = SD_UNLIMITED;
    dim_sds[1] = X_DIM;
    
    /* create the SDS */
    if ((sds_id = SDcreate (sd_id, sds_name, DFNT_INT32, rank, dim_sds))<0)
    {
        printf( "Could not create SDS <%s>\n",sds_name);
        return FAIL;
    }

    if ( do_write )
    {
        
        int32 start[2],     /* write start */
            edges[2],       /* write edges */
            buf[Y_DIM][X_DIM];
        int i, j;
        
        /* data set data initialization */
        for (j = 0; j < Y_DIM; j++) 
        {
            for (i = 0; i < X_DIM; i++)
            {
                buf[j][i] = (i + j) + 1;
            }
        }
        /* define the location and size of the data to be written to the data set */
        start[0] = 0;
        start[1] = 0;
        edges[0] = Y_DIM;
        edges[1] = X_DIM;
        
        /* write the stored data to the data set */
        if (SDwritedata (sds_id, start, NULL, edges, (VOIDP)buf)==FAIL)
        {
            printf( "Failed to set write for SDS <%s>\n", sds_name);
            goto fail;
        } 
        
    }
    
    /* terminate access to the SDS */
    if (SDendaccess (sds_id)==FAIL)
    {
        printf( "Failed to end SDS <%s>\n", sds_name);
        return FAIL;
    } 
    
    return SUCCEED;

fail:

    SDendaccess (sds_id);
    return FAIL;
    
}


/*-------------------------------------------------------------------------
 * Function: add_vs
 *
 * Purpose: utility function to write with
 *  VS - Vdata Interface,
 *  optionally inserting the VS into the group VGROUP_ID
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_vs(const char* vs_name,
            int32 file_id,
            int32 vgroup_id)
{
    int32   vdata_ref,      /* reference number of the vdata */
            vdata_tag,      /* tag number of the vdata */
            vdata_id;       /* vdata id */
    int32   attr_n_values   = 3; /* number of values in the vdata attribute */
    int32   field_n_values  = 4; /* number of values in the field attribute */
    char    vd_attr[3]      = {'A', 'B', 'C'};/* vdata attribute values*/
    int32   fld_attr[4]     = {2, 4, 6, 8};   /* field attribute values*/
    float32 data_buf[N_RECORDS][N_VALS_PER_REC]; /* buffer for vdata values */
    int     i;
    
    /* Initialize the VS interface */
    Vstart (file_id);
    
    /* Create a new vdata, set to -1 to create  */
    vdata_ref = -1,    
    vdata_id = VSattach (file_id, vdata_ref, "w");
    
    /* Set name and class name of the vdata */
    if (VSsetname (vdata_id, vs_name)==FAIL){
        printf( "Could not set name for VS\n");
        return FAIL;
    }
    if (VSsetclass (vdata_id, CLASS_NAME)==FAIL){
        printf( "Could not set class for VS\n");
        return FAIL;
    }
    
    /* Introduce each field's name, data type, and order */
    VSfdefine (vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER_1 );
    VSfdefine (vdata_id, FIELD2_NAME, DFNT_FLOAT32, ORDER_2 );
    VSfdefine (vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER_3 );
    
    /* Finalize the definition of the fields */
    if (VSsetfields (vdata_id, FIELDNAME_LIST)==FAIL){
        printf( "Could not set fields for VS\n");
        return FAIL;
    }
    
    /* 
    * Buffer the data by the record for fully interlaced mode.  Note that the
    * first three elements contain the three values of the first field, the
    * fourth element contains the value of the second field, and the last two
    * elements contain the two values of the third field.
    */
    for (i = 0; i < N_RECORDS; i++)
    {
        data_buf[i][0] = (float32)1.0 * i;
        data_buf[i][1] = (float32)2.0 * i;
        data_buf[i][2] = (float32)3.0 * i;
        data_buf[i][3] = (float32)0.1 + i;
        data_buf[i][4] = 0.0;
        data_buf[i][5] = 65.0;
    }
    
    /* Write the data from data_buf to the vdata with full interlacing mode */
    if (VSwrite (vdata_id, (uint8 *)data_buf, N_RECORDS,FULL_INTERLACE)==FAIL){
        printf( "Could not write VS\n");
        return FAIL;
    }
    
    /* Attach an attribute to the vdata, i.e., indicated by the second parameter */
    if (VSsetattr (vdata_id,_HDF_VDATA,"Myattr",DFNT_CHAR,
        attr_n_values, vd_attr)==FAIL){
        printf( "Could not set attr for VS\n");
        return FAIL;
    }
    
    /* Attach an attribute to the field 0 */
    if (VSsetattr (vdata_id, 0, "Myfattr", DFNT_INT32, 
        field_n_values, fld_attr)==FAIL){
        printf( "Could not set attr for VS\n");
        return FAIL;
    }
    
    /* Obtain the tag and ref number of the vdata */
    vdata_tag = VSQuerytag (vdata_id);
    vdata_ref = VSQueryref (vdata_id);
    
    /* add the VS to the vgroup*/
    if (vgroup_id)
    {
        if (Vaddtagref (vgroup_id, vdata_tag, vdata_ref)==FAIL){
            printf( "Could not set group for VS\n");
            return FAIL;
        }
    }
    
    /* terminate access to the VSs */
    if (VSdetach (vdata_id)==FAIL){
        printf( "Could not detach VS\n");
        return FAIL;
    }
    
    /* Terminate access to the VS interface */
    if (Vend (file_id)==FAIL){
        printf( "Could not end VS\n");
        return FAIL;
    }
    
    /* add an annotation and label to the vdata */
    if (add_an(file_id, vdata_tag, vdata_ref)<0)
        return FAIL;
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_file_an
 *
 * Purpose: utility function to write a file AN
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 30, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_file_an(int32 file_id)
{
    int32 an_id,        /* AN interface identifier */
          file_label_id,/* file label identifier */
          file_desc_id, /* file description identifier */
          data_label_id,  /* data label identifier */
          data_desc_id,   /* data description identifier */
          vgroup_id;
    uint16 vgroup_tag, vgroup_ref;
    
    /* Initialize the AN interface */
    an_id = ANstart (file_id);
    
   /*-------------------------------------------------------------------------
    * file labels and annotations
    *-------------------------------------------------------------------------
    */ 
    
    /* Create the file label */
    file_label_id = ANcreatef (an_id, AN_FILE_LABEL);
    
    /* Write the annotations to the file label */
    if (ANwriteann (file_label_id,FILE_LABEL_TXT,strlen (FILE_LABEL_TXT))==FAIL)
    {
        printf( "Could not write AN\n");
        return FAIL;
    }
    
    /* Create file description */
    file_desc_id = ANcreatef (an_id, AN_FILE_DESC);
    
    /* Write the annotation to the file description */
    if (ANwriteann (file_desc_id, FILE_DESC_TXT, strlen (FILE_DESC_TXT))==FAIL)
    {
        printf( "Could not write AN\n");
        return FAIL;
    }
    
   /*-------------------------------------------------------------------------
    * data labels and annotations
    *-------------------------------------------------------------------------
    */ 
    
    /* Create a vgroup in the V interface*/
    Vstart (file_id);
    vgroup_id = Vattach (file_id, -1, "w");
    if (Vsetname (vgroup_id, "an_group")==FAIL)
    {
        printf( "Could not set name for VG\n");
        return FAIL;
    }
    
    /* Obtain the tag and ref number of the vgroup */
    vgroup_tag = (uint16) VQuerytag (vgroup_id);
    vgroup_ref = (uint16) VQueryref (vgroup_id);
    
    /* Create the data label for the vgroup identified by its tag and ref number */
    data_label_id = ANcreate (an_id, vgroup_tag, vgroup_ref, AN_DATA_LABEL);
    
    /* Write the annotation text to the data label */
    if (ANwriteann (data_label_id, DATA_LABEL_TXT, strlen (DATA_LABEL_TXT))==FAIL)
    {
        printf( "Could not write AN\n");
        return FAIL;
    }
    
    /* Create the data description for the vgroup identified by its tag and ref number */
    data_desc_id = ANcreate (an_id, (uint16)vgroup_tag, (uint16)vgroup_ref, AN_DATA_DESC);
    
    /* Write the annotation text to the data description */
    if (ANwriteann (data_desc_id, DATA_DESC_TXT, strlen (DATA_DESC_TXT))==FAIL)
    {
        printf( "Could not write AN\n");
        return FAIL;
    }
    
    /* Teminate access to the vgroup and to the V interface */
    if (Vdetach (vgroup_id)==FAIL)
    {
        printf( "Could not detach VG\n");
        return FAIL;
    }
    if (Vend (file_id)==FAIL){
        printf( "Could not end VG\n");
        return FAIL;
    }
    
    /* Terminate access to each annotation explicitly */
    if (ANendaccess (file_label_id)==FAIL||
        ANendaccess (file_desc_id)==FAIL||
        ANendaccess (data_label_id)==FAIL||
        ANendaccess (data_desc_id)==FAIL){
        printf( "Could not end AN\n");
        return FAIL;
    }
    
    /* Terminate access to the AN interface */
    if (ANend (an_id)==FAIL==FAIL){
        printf( "Could not end AN\n");
        return FAIL;
    }
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_pal
 *
 * Purpose: utility function to write a palette
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 19, 2003
 *
 *-------------------------------------------------------------------------
 */

static
int add_pal(const char* fname)
{
    uint8  pal[256*3];
    int i, n;

   /*-------------------------------------------------------------------------
    * define a palette, blue to red tones 
    *-------------------------------------------------------------------------
    */
    for ( i=0, n=0; i< 256*3; i+=3, n++)
    {
        pal[i]  =n;      /* red */
        pal[i+1]=0;      /* green */
        pal[i+2]=255-n;  /* blue */
    }
    
    if (DFPaddpal(fname,pal)==FAIL){
        printf( "Failed to write palette in <%s>\n", fname);
        return FAIL;
    }
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: add_sd_szip
 *
 * Purpose: utility function to write with SZIPed SDSs
 *  SD - Multifile Scientific Data Interface,
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: September 15, 2003
 *
 *-------------------------------------------------------------------------
 */
static
int add_sd_szip(const char *fname,       /* file name */
                 int32 file_id,           /* file ID */
                 int32 sd_id,             /* SD interface identifier */
                 const char* sds_name,    /* sds name */
                 int32 vgroup_id,         /* group ID */
                 int32 chunk_flags,       /* chunk flags */
                 int32 nt,                /* number type */
                 int32 bits_per_pixel,    /* szip parameter */
                 int32 *dim,              /* dimension of the data set */
                 void *data
                 )

{
    int32          sds_id,            /* data set identifier */
                   sds_ref,           /* reference number of the data set */
                   rank = 2;          /* rank of the data set array */
    comp_coder_t   comp_type  = COMP_CODE_NONE;    /* compression flag */
    comp_info      comp_info;         /* compression structure */
    HDF_CHUNK_DEF  chunk_def;         /* chunking definitions */ 
    int32          edges[2],          /* write edges */
                   start[2]={0,0};    /* write start */

    edges[0]=dim[0]; 
    edges[1]=dim[1];
    
#ifdef H4_HAVE_LIBSZ
    if (SZ_encoder_enabled()) {
        comp_type = COMP_CODE_SZIP;
        comp_info.szip.pixels_per_block = 2;
        comp_info.szip.options_mask = SZ_EC_OPTION_MASK;
        comp_info.szip.options_mask |= SZ_RAW_OPTION_MASK;
        comp_info.szip.pixels = 0;
        comp_info.szip.pixels_per_scanline = 0;
        comp_info.szip.bits_per_pixel = 0;
    } else {
        printf("Warning: SZIP encoding not available\n");
    }
#else
    printf("Warning: SZIP compression not available\n");
#endif
    
    /* create the SDS */
    sds_id = SDcreate (sd_id, sds_name, nt, rank, dim);
    if (sds_id < 0) {
        printf( "SDcreate failed for file <%s>\n",sds_name);
        return FAIL;
    }
    
    /* set chunk */
    if ( (chunk_flags == HDF_CHUNK) || (chunk_flags == (HDF_CHUNK | HDF_COMP)) )
    {
        set_chunk_def(comp_type, 
            dim,
            1,
            bits_per_pixel,
            &chunk_def);
        if (SDsetchunk (sds_id, chunk_def, chunk_flags)==FAIL) {
            printf( "Failed to set chunk for SDS <%s>\n",sds_name);
            goto fail;
        }
    }
    
    /* use compress without chunk-in */
    else if ( chunk_flags==HDF_NONE && 
        comp_type>COMP_CODE_NONE && comp_type<COMP_CODE_INVALID)
    {
        if (SDsetcompress (sds_id, comp_type, &comp_info)==FAIL) {
            printf( "Failed to set compress for SDS <%s>\n",sds_name);
            goto fail;
        } 
    }
    
    /* write the stored data to the data set */
    if (SDwritedata (sds_id, start, NULL, edges, (VOIDP)data)==FAIL) {
        printf( "Failed to writer SDS <%s>\n",sds_name);
        goto fail;
    }
    
    /* obtain the reference number of the SDS using its identifier */
    sds_ref = SDidtoref (sds_id);
    
    /* add the SDS to the vgroup. the tag DFTAG_NDG is used */
    if (vgroup_id) 
    {
        if (Vaddtagref (vgroup_id, TAG_GRP_DSET, sds_ref)==FAIL) {
            printf( "Failed to set ref for SDS <%s>\n",sds_name);
            goto fail;
        }
    }
    
    /* terminate access to the SDS */
    if (SDendaccess (sds_id)==FAIL) {
        printf( "Failed to end SDS <%s>\n",sds_name);
        goto fail;
    }
    
    return SUCCEED;
    
fail:
    SDendaccess (sds_id);
    return FAIL;
}




/*-------------------------------------------------------------------------
 * Function: do_file_all
 *
 * Purpose: writes all types of HDF objects 
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

static 
int do_file_all(char* fname)
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
    if ((file_id = Hopen (fname, DFACC_CREATE, (int16)0))<0)
    {
        printf("Error: Could not create file <%s>\n",fname);
        return FAIL;
    }
    
    /* initialize the SD interface */
    if ((sd_id = SDstart (fname, DFACC_RDWR))== FAIL)
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

    
    
   /*-------------------------------------------------------------------------
    * add some SDSs to the file
    * duplicates are inserted in the groups "g1", "g2", "g3" and root
    *-------------------------------------------------------------------------
    */ 

   
    /* add non chunked, non compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_NONE;
    if (add_sd(fname,file_id,sd_id,"dset1",vgroup1_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(fname,file_id,sd_id,"dset2",vgroup2_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(fname,file_id,sd_id,"dset3",vgroup3_id,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(fname,file_id,sd_id,"dset4",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(fname,file_id,sd_id,"dset5",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd(fname,file_id,sd_id,"dset6",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    if (add_sd3d(fname,file_id,sd_id,"dset7",0,chunk_flags,comp_type,NULL)<0)
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
    if (add_sd(fname,file_id,sd_id,"dset_chunk",0,chunk_flags,comp_type,NULL)<0)
        goto out;
    
    /* add a chunked-compressed sds with SDsetchunk */
    chunk_flags = HDF_CHUNK | HDF_COMP;
    comp_type   = COMP_CODE_DEFLATE;
    if (add_sd(fname,file_id,sd_id,"dset_chunk_comp",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * GZIP
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_DEFLATE;
    if (add_sd(fname,file_id,sd_id,"dset_gzip",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * add an empty sds
    *-------------------------------------------------------------------------
    */ 
    if (add_empty_sd(sd_id,"dset_empty")<0)
        goto out;

    /*-------------------------------------------------------------------------
    * add 2 SDSs with unlimited dimensions, one written
    *-------------------------------------------------------------------------
    */ 
    if (add_unl_sd(sd_id,"dset_unl", 0)<0)
        goto out;
    if (add_unl_sd(sd_id,"dset_unlw", 1)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * RLE
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_RLE;
    if (add_sd(fname,file_id,sd_id,"dset_rle",0,chunk_flags,comp_type,&comp_info)<0)
        goto out;
    
   /*-------------------------------------------------------------------------
    * HUFF
    *-------------------------------------------------------------------------
    */ 
    
    /* add some non chunked, compressed sds */
    chunk_flags = HDF_NONE;
    comp_type   = COMP_CODE_SKPHUFF;
    if (add_sd(fname,file_id,sd_id,"dset_huff",0,chunk_flags,comp_type,&comp_info)<0)
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
        if (add_sd(fname,file_id,sd_id,"dset_szip",0,chunk_flags,comp_type,&comp_info)<0)
            goto out;
        
        {
            
            int i, j;
            {
                int32 buf[YD1][XD1];
                int32 dim[2]={YD1,XD1};
                int32 bpp=32;
                for (j = 0; j < YD1; j++) {
                    for (i = 0; i < XD1; i++)
                        buf[j][i] = (int32) (i + j) + 1;
                }
                if (add_sd_szip(fname,file_id,sd_id,"dset32szip",0,HDF_NONE,DFNT_INT32,bpp,dim,buf)<0)
                    return FAIL;
            }
            
        }
    }
    
#endif
    
   /*-------------------------------------------------------------------------
    * add some RIS24 images to the file
    *-------------------------------------------------------------------------
    */
    /* Pixel Interlacing */
    if (add_r24(DATA_FILE2,fname,file_id,DFIL_PIXEL,vgroup_img_id)<0)
        goto out;
    /* Scan Plane Interlacing */
    if (add_r24(DATA_FILE3,fname,file_id,DFIL_PLANE,vgroup_img_id)<0)
        goto out;  
    
    
   /*-------------------------------------------------------------------------
    * add some RIS8 images to the file
    *-------------------------------------------------------------------------
    */ 
    if (add_r8(DATA_FILE1,fname,file_id,vgroup_img_id)<0)
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
    
    if (add_vs("vdata1",file_id,vgroup1_id)<0)
        goto out; 
    if (add_vs("vdata2",file_id,vgroup2_id)<0)
        goto out; 
    if (add_vs("vdata3",file_id,vgroup3_id)<0)
        goto out;
    if (add_vs("vdata4",file_id,0)<0)
        goto out; 
    
   /*-------------------------------------------------------------------------
    * add some global attributes to the file
    *-------------------------------------------------------------------------
    */ 
    if (add_glb_attrs(fname,file_id,sd_id,gr_id)<0)
        goto out;

    
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


    if (add_pal(fname)<0)
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
 * write a big file for hyperslab reading
 *-------------------------------------------------------------------------
 */

static 
int do_file_hyperslab(char* fname) 
{

    int32 sd_id;         /* SD interface identifier */
    int32 sds_id;        /* SDS identifier */
    int32 dims[2];       /* sizes of the SDS dimensions */
    int32 start[2];      /* start location to write */
    int32 edges[2];      /* number of elements to write */

    int32 sds_idx;
    int32 rank;
	uint8 array_data[DIM0][DIM1];
    uint8 append_data[DIM1];
	intn  i, j, n;

	/* Create a file and initiate the SD interface. */
    if ((sd_id = SDstart(fname, DFACC_CREATE))==FAIL) 
        goto error;
  
	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = SD_UNLIMITED;
	dims[1] = DIM1;

	/* Create 2 data sets */
	if ((sds_id = SDcreate(sd_id, "data1", DFNT_UINT8, rank, dims))==FAIL) 
        goto error;
  
	/* initial values */
    for (j = 0; j < DIM0; j++) 
    {
        for (i = 0; i < DIM1; i++)
            array_data[j][i] = (i + j) + 1;
    }

	/* define the location, pattern, and size of the data set */
    for (i = 0; i < rank; i++) 
    {
        start[i] = 0;
    }
	edges[0] = DIM0; /* 10 */
	edges[1] = DIM1; /* 5 */

    if ( SDwritedata(sds_id, start, NULL, edges, (VOIDP)array_data)==FAIL) 
        goto error;

	/* terminate access to the datasets and SD interface */
    if ( SDendaccess(sds_id)==FAIL) 
        goto error;
    if ( SDend(sd_id)==FAIL) 
        goto error;
   
    /* append data */
	if (( sd_id = SDstart(fname, DFACC_WRITE))==FAIL) 
        goto error;
   	
    if ((sds_idx = SDnametoindex (sd_id, "data1"))==FAIL) 
        goto error;
    if ((sds_id = SDselect (sd_id, sds_idx))==FAIL) 
        goto error;
  
    /* store array values to be appended */
    for (i = 0; i < DIM1; i++)
        append_data[i] = i + 1;
    
   	/* define the location of the append */
    for (n = 0; n < ADD_ROWS; n++)
    {
        start[0] = DIM0 + n;   /* 10 */
        start[1] = 0;
        edges[0] = 1;          /* 1 row at a time */
        edges[1] = DIM1;       /* 5 elements */
        
        /* append data to file */
        if ( SDwritedata (sds_id, start, NULL, edges, (VOIDP) append_data)==FAIL) 
            goto error;

    }

    
    /* terminate access */
    if ( SDendaccess (sds_id)==FAIL) 
        goto error;
    if ( SDend (sd_id)==FAIL) 
        goto error;
  
    return SUCCEED;
    
error:
    
    printf("Error...Exiting...\n");
    
    return FAIL;


}

/*-------------------------------------------------------------------------
 * write groups
 * a circular reference
 *
 *                g0 ----->   g1
 *                 |       |    ^
 *                 |       |    |
 *                 |       \/   |
 *                 |----->   g1.1
 *
 *-------------------------------------------------------------------------
 */

static 
int do_file_groups(char* name) 
{
     
     int32 vg0_id,    /* vgroup identifier */
           vg1_id,    /* vgroup identifier */
           vg2_id,    /* vgroup identifier */
           file1_id;  /* HDF file identifier */
     
     /* create a HDF file */
     if ((file1_id = Hopen (name, DFACC_CREATE, (int16)0))<0)
     {
         printf("Error: Could not create file <%s>\n",name);
         return FAIL;
     }
     
     /* initialize the V interface */
     if (Vstart (file1_id)==FAIL)
     {
         printf( "Could not start VG\n");
         return FAIL;
     }
     
     /* create a vgroup */
     vg0_id = Vattach (file1_id, -1, "w");
     if (Vsetname (vg0_id, "g0")==FAIL)
     {
         printf( "Could not name group\n");
         goto out;
     }
     
     /* create the second vgroup */
     vg1_id = Vattach (file1_id, -1, "w");
     if (Vsetname (vg1_id, "g1")==FAIL)
     {
         printf( "Could not name group\n");
         goto out;
     }
     
     /* create the third vgroup */
     vg2_id = Vattach (file1_id, -1, "w");
     if (Vsetname (vg2_id, "g1.1")==FAIL)
     {
         printf( "Could not name group\n");
         goto out;
     }
  
     if (Vinsert (vg0_id, vg1_id)==FAIL)
     {
         printf( "Could not insert VG\n");
         goto out;
     }
     
     if (Vinsert (vg0_id, vg2_id)==FAIL)
     {
         printf( "Could not insert VG\n");
         goto out;
     }
     
     if (Vinsert (vg1_id, vg2_id)==FAIL)
     {
         printf( "Could not insert VG\n");
         goto out;
     }
     
     if (Vinsert (vg2_id, vg1_id)==FAIL)
     {
         printf( "Could not insert VG\n");
         goto out;
     }
     
     /* terminate access to the vgroup */
     if (Vdetach (vg0_id)==FAIL)
     {
         printf( "Could not close group\n");
         goto out;
     }
     
     /* terminate access to the vgroup */
     if (Vdetach (vg1_id)==FAIL)
     {
         printf( "Could not close group\n");
         goto out;
     }
     
     /* terminate access to the vgroup */
     if (Vdetach (vg2_id)==FAIL)
     {
         printf( "Could not close group\n");
         goto out;
     }
     
     /* terminate access to the V interface */
     if (Vend (file1_id)==FAIL)
     {
         printf( "Could not end VG\n");
         goto out;
     }
     
     /* close the HDF file */
     if (Hclose (file1_id)==FAIL)
     {
         printf( "Could not close file\n");
         return FAIL;
     }
     
     return SUCCEED;
     
out:
     printf("Error...Exiting...\n");
     return FAIL;
     
}


/*-------------------------------------------------------------------------
 * Function: generate_files
 *
 * Purpose: writes several HDF objects to the files
 *  HREPACK_FILE1
 *  HREPACK_FILE2
 *  HREPACK_FILE3
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

static
int generate_files(void)
{
    TESTING("generating files");
    
    if (do_file_all(HREPACK_FILE1)<0)
        return FAIL;
    
    if (do_file_hyperslab(HREPACK_FILE2)<0)
        return FAIL;
    
    if (do_file_groups(HREPACK_FILE3)<0)
        return FAIL;

    PASSED();
    
    return SUCCEED;
    
}

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

    return 0;

}
