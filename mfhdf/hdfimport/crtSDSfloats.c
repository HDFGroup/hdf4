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

#include "mfhdf.h"

#define FILE2_NAME     "SDSfloat2.hdf" 
#define FILE3_NAME     "SDSfloat3.hdf" 
#define SDS1_NAME      "SDStemplate" 
#define SDS2_NAME      "SDStemplate again" 
#define X_LENGTH      6
#define Y_LENGTH      9
#define Z_LENGTH      2
#define RANK2          2  /* Number of dimensions of the SDS */
#define RANK3          3  /* Number of dimensions of the SDS */

main() 
{
    /************************* Variable declaration ************************/

    int32   sd2_id, sd3_id, sds1_id, sds2_id;
    int32   dim2_sizes[2];	/* sizes of the 2-dim SDS dimensions */
    int32   dim3_sizes[3];	/* sizes of the 3-dim SDS dimensions */
    int32   start2[2], start3[3];	/* start arrays for both SDSs */
    intn    status;
    int32   dim_id, dim_index,	/* dimension id and index */
	    size, data_type, 	/* dimension's size and data type */
	    nattrs;		/* number of attributes */
    char    dim_name[20];
    float32 in2_data[X_LENGTH][Y_LENGTH]={ /* input for 2-dim SDS */
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                   0.0,  0.0,600.0,600.0,300.0,400.0,
                                 500.0,500.0,600.0,600.0,300.0,400.0,
                                   0.0,  0.0,600.0,600.0,300.0,400.0};
    float32 in3_data[X_LENGTH][Y_LENGTH][Z_LENGTH]={ /* input for 3-dim SDS */
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 100.0,100.0,200.0,200.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                 300.0,300.0,  0.0,400.0,300.0,400.0,
                                   0.0,  0.0,600.0,600.0,300.0,400.0,
                                 500.0,500.0,600.0,600.0,300.0,400.0,
                                   0.0,  0.0,600.0,600.0,300.0,400.0,
                                 1.0,1.0,2.0,2.0,3.0,4.0,
                                 1.0,1.0,2.0,2.0,3.0,4.0,
                                 1.0,1.0,2.0,2.0,3.0,4.0,
                                 3.0,3.0,  0.0,4.0,3.0,4.0,
                                 3.0,3.0,  0.0,4.0,3.0,4.0,
                                 3.0,3.0,  0.0,4.0,3.0,4.0,
                                   0.0,  0.0,6.0,6.0,3.0,4.0,
                                 5.0,5.0,6.0,6.0,3.0,4.0,
                                   0.0,  0.0,6.0,6.0,3.0,4.0,
				};

    /********************* End of variable declaration ***********************/

    /*
    * Create the files and initialize the SD interface.
    */
    sd2_id = SDstart (FILE2_NAME, DFACC_CREATE);
    sd3_id = SDstart (FILE3_NAME, DFACC_CREATE);
 
    /*
    * Define the dimensions/origins of the two SDSs to be created.
    */
    dim2_sizes[0] = Y_LENGTH;
    dim2_sizes[1] = X_LENGTH;
    dim3_sizes[0] = Z_LENGTH;
    dim3_sizes[1] = Y_LENGTH;
    dim3_sizes[2] = X_LENGTH;

    start2[0] = start2[1] = 0;
    start3[0] = start3[1] = start3[2] = 0;

    /*
    * Create two data sets, SDS1_NAME and SDS2_NAME, with type DFNT_FLOAT32
    * in the two files, FILE2_NAME and FILE3_NAME.
    */
    sds1_id = SDcreate (sd2_id, SDS1_NAME, DFNT_FLOAT32, RANK2, dim2_sizes);
    sds2_id = SDcreate (sd3_id, SDS2_NAME, DFNT_FLOAT32, RANK3, dim3_sizes);

    /* Write data to the SDSs */
    status = SDwritedata(sds1_id, start2, NULL, dim2_sizes, (VOIDP)in2_data);
    status = SDwritedata(sds2_id, start3, NULL, dim3_sizes, (VOIDP)in3_data);

    /*
    * Terminate access to the data sets.
    */
    status = SDendaccess (sds1_id);
    status = SDendaccess (sds2_id);

    /*
    * Terminate access to the SD interface and close the files.
    */
    status = SDend (sd2_id);
    status = SDend (sd3_id);
}
