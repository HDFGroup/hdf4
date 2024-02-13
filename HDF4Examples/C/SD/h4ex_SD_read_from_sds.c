#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME "SDS.hdf"
#define X_LENGTH  5
#define Y_LENGTH  16

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id, sds_id, sds_index;
    int32 start[2], edges[2];
    int32 data[Y_LENGTH][X_LENGTH];
    int   j;

    /********************* End of variable declaration ***********************/

    /*
     * Open the file for reading and initialize the SD interface.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_READ)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Select the first data set.
     */
    sds_index = 0;
    sds_id    = SDselect(sd_id, sds_index);

    /*
     * Set elements of array start to 0, elements of array edges
     * to SDS dimensions,and use NULL for the argument stride in SDreaddata
     * to read the entire data.
     */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /*
     * Read entire data into data array.
     */
    if (SDreaddata(sds_id, start, NULL, edges, (void *)data) == FAIL)
        printf("*** ERROR from SDreaddata\n");

    /*
     * Print 10th row; the following numbers should be displayed.
     *
     *         10 1000 12 13 14
     */
    for (j = 0; j < X_LENGTH; j++)
        printf("%d ", data[9][j]);
    printf("\n");

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
