#include "mfhdf.h"

#define FILE_NAME     "SDSchunked.hdf"
#define SDS_NAME      "ChunkedData"
#define RANK          2
#define WIDTH  6
#define LENGTH 9

/* for SZIP stuff */
#define NN_OPTION_MASK             32
#define LSB_OPTION_MASK             8
#define MSB_OPTION_MASK             16
#define RAW_OPTION_MASK 128

int
test_szip_compression()
{
   /************************* Variable declaration **************************/

   int32         sd_id, sds_id, sds_index;
   intn          status;
   int32         flag, maxcache, new_maxcache;
   int32         dim_sizes[2], origin[2], array_rank, num_type, attributes;
   char name[MAX_NC_NAME];
   HDF_CHUNK_DEF c_def, c_def_out; /* Chunking definitions */ 
   comp_info 	 c_info;
   int32	 pixels_per_scanline, comp_type;
   int32         comp_flag, c_flags;
   int16         out_data[LENGTH][WIDTH];
   int16         in_data[LENGTH][WIDTH]={1,1,2,2,3,4,
	   			 1,1,2,2,3,4,
				 1,1,2,2,3,4,
				 3,3,0,4,3,4,
				 3,3,0,4,3,4,
				 3,3,0,4,3,4,
				 0,0,6,6,3,4,
				 5,5,6,6,3,4,
				 0,0,6,6,3,4};
   int32         start[2], edges[2];
   int16         chunk_out[3][2];
   int16         row[2] = { 5, 5 };
   int16         column[3] = { 4, 4, 4 };
   int16         fill_value = 0;   /* Fill value */
   int           i,j;

    /********************* End of variable declaration ***********************/
    /*
    * Create the file and initialize SD interface.
    */
    sd_id = SDstart (FILE_NAME, DFACC_CREATE);

    /*
    * Create 9x4 SDS.
    */
    dim_sizes[0] = LENGTH;
    dim_sizes[1] = WIDTH;
    pixels_per_scanline = dim_sizes[1];
    sds_id = SDcreate (sd_id, SDS_NAME,DFNT_INT16, RANK, dim_sizes);

    /* Define the location, pattern, and size of the data set */
    for (i = 0; i < RANK; i++) {
	start[i] = 0;
	edges[i] = dim_sizes[i];
	}

    /*
    * Fill the SDS array with the fill value.
    */
    status = SDsetfillvalue (sds_id, (VOIDP)&fill_value);

    /* Initialization for SZIP */
    comp_type = COMP_CODE_SZIP;
    c_info.szip.pixels = dim_sizes[0]*dim_sizes[1];;
    c_info.szip.pixels_per_block = 2;
    if(pixels_per_scanline >=2048)
        c_info.szip.pixels_per_scanline = 512;
    else
       c_info.szip.pixels_per_scanline = dim_sizes[1];

    c_info.szip.options_mask = NN_OPTION_MASK;
    c_info.szip.options_mask |= RAW_OPTION_MASK;
    c_info.szip.bits_per_pixel = 16;
    status = SDsetcompress(sds_id,comp_type,&c_info);
    status = SDwritedata(sds_id, start, NULL, edges, (VOIDP)in_data);

/*EIP
    printf("this is what in_data contains:\n");
    for (j=0; j<LENGTH; j++) 
    {
         for (i=0; i<WIDTH; i++) printf (" %d", in_data[j][i]);
         printf ("\n");
    }
*/


    /* 
    * Terminate access to the data set.
    */
    status = SDendaccess (sds_id);

    /*
    * Terminate access to the SD interface and close the file.
    */
    status = SDend (sd_id);

    /*
    * Reopen the file and access the first data set.
    */
    sd_id = SDstart (FILE_NAME, DFACC_READ);
    sds_index = 0;
    sds_id = SDselect (sd_id, sds_index);

    status = SDgetinfo(sds_id, name, &array_rank, dim_sizes, &num_type, &attributes);
/*EIP
    printf (" SDgetinfo returns: %i\n", status);
    printf (" name=%s, array_rank=%d\n", name, array_rank);
*/

    /*
    * Read the entire data set using SDreaddata function.
    */
    start[0] = 0;
    start[1] = 0;
    edges[0] = LENGTH;
    edges[1] = WIDTH;
    HDmemset(&out_data,  0, sizeof(out_data));
    status = SDreaddata (sds_id, start, NULL, edges, &out_data);
    /*status = SDreaddata (sds_id, start, NULL, edges, (VOIDP)out_data);*/

    /* 
    * Print out what we have read.
    */
/*EIP
    printf("this is what out_data contains: \n");
    for (j=0; j<LENGTH; j++) 
    {
         for (i=0; i<WIDTH; i++) printf (" %d", out_data[j][i]);
         printf ("\n");
    }
*/

    /* 
    * Terminate access to the data set.
    */
    status = SDendaccess (sds_id);

    /*
    * Terminate access to the SD interface and close the file.
    */
    status = SDend (sd_id);
    return 0;
}            


