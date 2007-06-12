#include "mfhdf.h"
 

#define FILE1_NAME         "hdifftst1.hdf" 
#define FILE2_NAME         "hdifftst2.hdf" 
#define FILE3_NAME         "hdifftst3.hdf" 
#define FILE4_NAME         "hdifftst4.hdf"       
#define X_LENGTH           2
#define Y_LENGTH           3
#define RANK               2  
#define FILE_ATTR_NAME     "File_contents"
#define SDS_ATTR_NAME      "Valid_range"


#define  CLASS1_NAME       "5x1 Array"
#define  CLASS2_NAME       "6x4 Array"
#define  VDATA1_NAME       "vdata1"
#define  VDATA2_NAME       "vdata2"
#define  FIELD1_NAME       "Single-component Field"
#define  FIELD2_NAME       "Multi-component Field"
#define  N_RECORDS_1       5    /* number of records the first vdata contains  */
#define  N_RECORDS_2       2    /* number of records the second vdata contains */
#define  ORDER_2           4    /* order of the field in the second vdata      */

#define  CLASS3_NAME       "Particle Data"
#define  VDATA3_NAME       "vdata3"
#define  FIELD3_NAME1      "Position"                       
#define  FIELD3_NAME2      "Mass"                          
#define  FIELD3_NAME3      "Temperature"                    
#define  FIELDNAME3_LIST   "Position,Mass,Temperature"      /* No spaces b/w names */
#define  ORDER3_1          3                                /* order of first field */
#define  ORDER3_2          1                                /* order of second field */
#define  ORDER3_3          2                                /* order of third field */
#define  N_VALS_PER_REC   (ORDER3_1 + ORDER3_2 + ORDER3_3)  /* number of values per record */

static int do_big_file(void); 


int
main(void) 
{
 int32 sd1_id;        /* SD interface identifier */
 int32 sd2_id;        /* SD interface identifier */
 int32 sds1_id;       /* SDS identifier */
 int32 sds2_id;       /* SDS identifier */
 int32 dim_sizes[2];  /* sizes of the SDS dimensions */
 int32 start[2];      /* start location to write */
 int32 edges[2];      /* number of elements to write */
 int32 n_values;
 int32 buf1a[Y_LENGTH][X_LENGTH] = {{1,1},{1,1},{5,6}};
 int32 buf1b[Y_LENGTH][X_LENGTH] = {{1,2},{3,4},{5,6}};

 /* percent test: compare divide by zero, both zero */
 int32 buf2a[Y_LENGTH][X_LENGTH] = {{100,100},{100,0},{0,100}}; 
 int32 buf2b[Y_LENGTH][X_LENGTH] = {{120,80},{0,100},{0,50}};

 /* global attributes */
 char8 bufga1[] = "Storm_track_data1"; 
 char8 bufga2[] = "Storm_track_data2"; 
 float32 bufa1[2] = {1., 1.};
 float32 bufa2[2] = {1., 2.};

 /*vdata*/
 int32 file1_id; 
 int32 file2_id; 
 int32 vdata1_id;
 int32 vdata2_id;
 char8 vdata1_buf1 [N_RECORDS_1] = {'V', 'D', 'A', 'T', 'A'};
 char8 vdata1_buf2 [N_RECORDS_1] = {'X', 'D', 'A', 'T', 'A'};
 int32 vdata2_buf1  [N_RECORDS_2][ORDER_2] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
 int32 vdata2_buf2  [N_RECORDS_2][ORDER_2] = {{1, 1, 1, 1}, {5, 6, 7, 8}};
 float32 vdata3_buf1[N_RECORDS_2][N_VALS_PER_REC]={{1,2,3,4,5,6},{7,8,9,10,11,12}}; 
 float32 vdata3_buf2[N_RECORDS_2][N_VALS_PER_REC]={{1,1,1,1,1,1},{7,8,9,10,11,12}}; 
 
/* Define the location and size of the data to be written to the data set*/
 start[0] = 0;
 start[1] = 0;
 edges[0] = Y_LENGTH;
 edges[1] = X_LENGTH;

/* Define the dimensions of the array to be created */
 dim_sizes[0] = Y_LENGTH;
 dim_sizes[1] = X_LENGTH;

/*-------------------------------------------------------------------------
 * SD data
 *-------------------------------------------------------------------------
 */

 
/* Create the files and initialize the SD interface */
 if ((sd1_id = SDstart (FILE1_NAME, DFACC_CREATE))==FAIL) 
     goto error;
 if ((sd2_id = SDstart (FILE2_NAME, DFACC_CREATE))==FAIL) 
     goto error;

/* Set a global attribute */
 n_values  = sizeof(bufga1);
 if ( SDsetattr (sd1_id, FILE_ATTR_NAME, DFNT_CHAR8, n_values, (VOIDP)bufga1)==FAIL) 
     goto error;
 if ( SDsetattr (sd2_id, FILE_ATTR_NAME, DFNT_CHAR8, n_values, (VOIDP)bufga2)==FAIL) 
     goto error;
  
/* Create the data sets */ 
 if ((sds1_id = SDcreate (sd1_id, "dset1", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;
 if ((sds2_id = SDcreate (sd2_id, "dset1", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;

/* Assign attribute */
 n_values  = 2;
 if ( SDsetattr (sds1_id, SDS_ATTR_NAME, DFNT_FLOAT32, n_values, (VOIDP)bufa1)==FAIL) 
     goto error;
 if ( SDsetattr (sds2_id, SDS_ATTR_NAME, DFNT_FLOAT32, n_values, (VOIDP)bufa2)==FAIL) 
     goto error;
 
/* Write the stored data to the data set */
 if ( SDwritedata (sds1_id, start, NULL, edges, (VOIDP)buf1a)==FAIL) 
     goto error;
 if ( SDwritedata (sds2_id, start, NULL, edges, (VOIDP)buf1b)==FAIL) 
     goto error;
 
/* Terminate access to the data set */
 if ( SDendaccess (sds1_id)==FAIL) 
     goto error;
 if ( SDendaccess (sds2_id)==FAIL) 
     goto error;

 /* Create another data set */ 
 if (( sds1_id = SDcreate (sd1_id, "dset2", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;
 if (( sds2_id = SDcreate (sd2_id, "dset2", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;
 if ( SDwritedata (sds1_id, start, NULL, edges, (VOIDP)buf1a)==FAIL) 
     goto error;
 if ( SDwritedata (sds2_id, start, NULL, edges, (VOIDP)buf1b)==FAIL) 
     goto error;
 if ( SDendaccess (sds1_id)==FAIL) 
     goto error;
 if ( SDendaccess (sds2_id)==FAIL) 
     goto error;

 /* data sets for -p test */ 
 if (( sds1_id = SDcreate (sd1_id, "dset3", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;
 if (( sds2_id = SDcreate (sd2_id, "dset3", DFNT_INT32, RANK, dim_sizes))==FAIL) 
     goto error;
 if ( SDwritedata (sds1_id, start, NULL, edges, (VOIDP)buf2a)==FAIL) 
     goto error;
 if ( SDwritedata (sds2_id, start, NULL, edges, (VOIDP)buf2b)==FAIL) 
     goto error;
 if ( SDendaccess (sds1_id)==FAIL) 
     goto error;
 if ( SDendaccess (sds2_id)==FAIL) 
     goto error;


/*-------------------------------------------------------------------------
 * end SD 
 *-------------------------------------------------------------------------
 */
 
 /* Terminate access to the SD interface and close the file */
 if ( SDend (sd1_id)==FAIL) 
     goto error;
 if ( SDend (sd2_id)==FAIL) 
     goto error;



/*-------------------------------------------------------------------------
 * VD data 
 *-------------------------------------------------------------------------
 */

/* Open the HDF file for writing */
 if ((file1_id = Hopen (FILE1_NAME, DFACC_WRITE, 0))==FAIL) 
     goto error;
 if ((file2_id = Hopen (FILE2_NAME, DFACC_WRITE, 0))==FAIL) 
     goto error;
 
/* Initialize the VS interface */
 if ( Vstart (file1_id)==FAIL) 
     goto error;
 if ( Vstart (file2_id)==FAIL) 
     goto error;

/*-------------------------------------------------------------------------
 * VD data one field
 *-------------------------------------------------------------------------
 */
 
/* Create the first vdata and populate it with data from vdata1_buf */
 if (VHstoredata (file1_id, FIELD1_NAME, (uint8 *)vdata1_buf1, 
  N_RECORDS_1, DFNT_CHAR8, VDATA1_NAME, CLASS1_NAME)==FAIL) 
     goto error; 
 if (VHstoredata (file2_id, FIELD1_NAME, (uint8 *)vdata1_buf2, 
  N_RECORDS_1, DFNT_CHAR8, VDATA1_NAME, CLASS1_NAME)==FAIL) 
     goto error; 

/*-------------------------------------------------------------------------
 * VD data one field, order 4
 *-------------------------------------------------------------------------
 */
 
/* Create the second vdata and populate it with data from vdata2_buf */
 if ( VHstoredatam (file1_id, FIELD2_NAME, (uint8 *)vdata2_buf1, 
  N_RECORDS_2, DFNT_INT32, VDATA2_NAME, CLASS2_NAME, ORDER_2)==FAIL) 
     goto error;  
 if ( VHstoredatam (file2_id, FIELD2_NAME, (uint8 *)vdata2_buf2, 
  N_RECORDS_2, DFNT_INT32, VDATA2_NAME, CLASS2_NAME, ORDER_2)==FAIL) 
     goto error;   

/*-------------------------------------------------------------------------
 * VD data several fields
 *-------------------------------------------------------------------------
 */

/* Create a new vdata */
 if ((vdata1_id = VSattach (file1_id, -1, "w"))==FAIL) 
     goto error;
 if ((vdata2_id = VSattach (file2_id, -1, "w"))==FAIL) 
     goto error;

/* Set name and class name of the vdata */
 if ( VSsetname (vdata1_id, VDATA3_NAME)==FAIL) 
     goto error;
 if ( VSsetclass (vdata1_id, CLASS3_NAME)==FAIL) 
     goto error;
 if ( VSsetname (vdata2_id, VDATA3_NAME)==FAIL) 
     goto error;
 if ( VSsetclass (vdata2_id, CLASS3_NAME)==FAIL) 
     goto error;

/* Define fields */
 if ( VSfdefine (vdata1_id, FIELD3_NAME1, DFNT_FLOAT32, ORDER3_1 )==FAIL) 
     goto error;
 if ( VSfdefine (vdata1_id, FIELD3_NAME2, DFNT_FLOAT32, ORDER3_2 )==FAIL) 
     goto error;
 if ( VSfdefine (vdata1_id, FIELD3_NAME3, DFNT_FLOAT32, ORDER3_3 )==FAIL) 
     goto error;
 if ( VSsetfields (vdata1_id, FIELDNAME3_LIST )==FAIL) 
     goto error;
 if ( VSfdefine (vdata2_id, FIELD3_NAME1, DFNT_FLOAT32, ORDER3_1 )==FAIL) 
     goto error;
 if ( VSfdefine (vdata2_id, FIELD3_NAME2, DFNT_FLOAT32, ORDER3_2 )==FAIL) 
     goto error;
 if ( VSfdefine (vdata2_id, FIELD3_NAME3, DFNT_FLOAT32, ORDER3_3 )==FAIL) 
     goto error;
 if ( VSsetfields (vdata2_id, FIELDNAME3_LIST)==FAIL) 
     goto error;

/* Write the data with full interlacing mode */
 if ( VSwrite (vdata1_id, (uint8 *)vdata3_buf1, N_RECORDS_2, FULL_INTERLACE)==FAIL) 
     goto error;
 if ( VSwrite (vdata2_id, (uint8 *)vdata3_buf2, N_RECORDS_2, FULL_INTERLACE)==FAIL) 
     goto error;

 if ( VSdetach (vdata1_id)==FAIL) 
     goto error;
 if ( VSdetach (vdata2_id)==FAIL) 
     goto error;

/*-------------------------------------------------------------------------
 * end VD data 
 *-------------------------------------------------------------------------
 */
 
/* Terminate access to the VS interface and close the HDF file */
 if ( Vend (file1_id)==FAIL) 
     goto error;
 if ( Vend (file2_id)==FAIL) 
     goto error;
 if ( Hclose (file1_id)==FAIL) 
     goto error;
 if ( Hclose (file2_id)==FAIL) 
     goto error;

/*-------------------------------------------------------------------------
 * write 2 big files for hyperslab reading
 *-------------------------------------------------------------------------
 */
 if (do_big_file()==FAIL) 
     goto error;

 
 return 0;

error:

 printf("Error...Exiting...\n");

 return 1;

}



#define DIM0     10
#define DIM1     10
#define ADD_ROWS ( 1024 * 1024 - 10 ) / 10 

/*-------------------------------------------------------------------------
 * write 2 big files for hyperslab reading
 *-------------------------------------------------------------------------
 */
static int do_big_file(void) 
{

    int32 sd1_id;        /* SD interface identifier */
    int32 sd2_id;        /* SD interface identifier */
    int32 sds1_id;       /* SDS identifier */
    int32 sds2_id;       /* SDS identifier */
    int32 dims[2];       /* sizes of the SDS dimensions */
    int32 start[2];      /* start location to write */
    int32 edges[2];      /* number of elements to write */

    int32 sds1_idx;
    int32 sds2_idx;
    int32 rank;
	uint8 array_data[DIM0][DIM1];
    uint8 append_data1[DIM1];
    uint8 append_data2[DIM1];
	intn  i, j, n;

	/* Create 2 files and initiate the SD interface. */
    if ((sd1_id = SDstart(FILE3_NAME, DFACC_CREATE))==FAIL) 
        goto error;
    if ((sd2_id = SDstart(FILE4_NAME, DFACC_CREATE))==FAIL) 
        goto error;

	/* Define the rank and dimensions of the data set to be created. */
	rank = 2;
	dims[0] = SD_UNLIMITED;
	dims[1] = DIM1;

	/* Create 2 data sets */
	if ((sds1_id = SDcreate(sd1_id, "data1", DFNT_UINT8, rank, dims))==FAIL) 
        goto error;
    if ((sds2_id = SDcreate(sd2_id, "data1", DFNT_UINT8, rank, dims))==FAIL) 
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

    if ( SDwritedata(sds1_id, start, NULL, edges, (VOIDP)array_data)==FAIL) 
        goto error;
    if ( SDwritedata(sds2_id, start, NULL, edges, (VOIDP)array_data)==FAIL) 
        goto error;

	/* terminate access to the datasets and SD interface */
    if ( SDendaccess(sds1_id)==FAIL) 
        goto error;
    if ( SDendaccess(sds2_id)==FAIL) 
        goto error;
    if ( SDend(sd1_id)==FAIL) 
        goto error;
    if ( SDend(sd2_id)==FAIL) 
        goto error;

    /* append data */
	if (( sd1_id = SDstart(FILE3_NAME, DFACC_WRITE))==FAIL) 
        goto error;
    if (( sd2_id = SDstart(FILE4_NAME, DFACC_WRITE))==FAIL) 
        goto error;
	
    if ((sds1_idx = SDnametoindex (sd1_id, "data1"))==FAIL) 
        goto error;
    if ((sds2_idx = SDnametoindex (sd2_id, "data1"))==FAIL) 
        goto error;
    
    if ((sds1_id = SDselect (sd1_id, sds1_idx))==FAIL) 
        goto error;
    if ((sds2_id = SDselect (sd2_id, sds2_idx))==FAIL) 
        goto error;
    
   	/* define the location of the append */
    for (n = 0; n < ADD_ROWS; n++)
    {
        start[0] = DIM0 + n;   /* 10 */
        start[1] = 0;
        edges[0] = 1;          /* 1 row at a time */
        edges[1] = DIM1;       /* 5 elements */

        /* store array values to be appended */
        for (i = 0; i < DIM1; i++)
            append_data1[i] = i + 1;
        for (i = 0; i < DIM1; i++)
            append_data2[i] = i + 1;

        if (n == 20 || n == ADD_ROWS / 2 || n == ADD_ROWS - 10 )
        {
            /* change a few values at 3 places evenly divided */
            for (i = 0; i < DIM1; i++)
                append_data2[i] = 100;
            
            /* append data to file */
            if ( SDwritedata (sds1_id, start, NULL, edges, (VOIDP) append_data1)==FAIL) 
                goto error;
            if ( SDwritedata (sds2_id, start, NULL, edges, (VOIDP) append_data2)==FAIL) 
                goto error;


        }
        
        
               
    }

    
    /* terminate access */
    if ( SDendaccess (sds1_id)==FAIL) 
        goto error;
    if ( SDendaccess (sds2_id)==FAIL) 
        goto error;
    if ( SDend (sd1_id)==FAIL) 
        goto error;
    if ( SDend (sd2_id)==FAIL) 
        goto error;
   

    return SUCCEED;
    
error:
    
    printf("Error...Exiting...\n");
    
    return FAIL;


}

