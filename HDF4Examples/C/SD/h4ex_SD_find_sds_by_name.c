#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME  "SDS.hdf"
#define SDS_NAME   "SDStemplate"
#define WRONG_NAME "WrongName"
#define X_LENGTH   5
#define Y_LENGTH   16

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
     * Find index of the data set with the name specified in WRONG_NAME.
     * Error condition occurs, since the data set with that name does not exist
     * in the file.
     */
    sds_index = SDnametoindex(sd_id, WRONG_NAME);
    if (sds_index == FAIL)
        printf("Data set with the name \"WrongName\" does not exist\n");

    /*
     * Find index of the data set with the name specified in SDS_NAME and use
     * the index to select the data set.
     */
    sds_index = SDnametoindex(sd_id, SDS_NAME);
    sds_id    = SDselect(sd_id, sds_index);

    /*
     * Set elements of the array start to 0, elements of the array edges to
     * SDS dimensions, and use NULL for stride argument in SDreaddata to read
     * the entire data.
     */
    start[0] = 0;
    start[1] = 0;
    edges[0] = Y_LENGTH;
    edges[1] = X_LENGTH;

    /*
     * Read the entire data into the buffer named data.
     */
    if (SDreaddata(sds_id, start, NULL, edges, (void *)data) == FAIL)
        printf("*** ERROR from SDreaddata\n");

    /*
     * Print 10th row; the following numbers should be displayed:
     *
     *             10 1000 12 13 14
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
