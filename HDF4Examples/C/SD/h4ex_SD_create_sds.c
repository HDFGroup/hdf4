#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME "SDS.hdf"
#define SDS_NAME  "SDStemplate"
#define X_LENGTH  5
#define Y_LENGTH  16
#define RANK      2 /* Number of dimensions of the SDS */

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id, sds_id; /* SD interface and data set identifiers */
    int32 dim_sizes[2];  /* sizes of the SDS dimensions */

    /********************* End of variable declaration ***********************/

    /*
     * Create the file and initialize the SD interface.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_CREATE)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Define the dimensions of the array to be created.
     */
    dim_sizes[0] = Y_LENGTH;
    dim_sizes[1] = X_LENGTH;

    /*
     * Create the data set with the name defined in SDS_NAME. Note that
     * DFNT_INT32 indicates that the SDS data is of type int32. Refer to
     * Table 2F, "Standard HDF Data Types and Flags," for definitions of
     * other types.
     */
    sds_id = SDcreate(sd_id, SDS_NAME, DFNT_INT32, RANK, dim_sizes);

    /*
     * Terminate access to the data set.
     */
    if (SDendaccess(sds_id) == FAIL)
        printf("*** ERROR from SDendaccess\n");

    /*
     * Terminate access to the SD interface and close the file.
     */
    if (SDend(sd_id) == FAIL)
        printf("*** ERROR from SDend\n");

    return 0;
}
