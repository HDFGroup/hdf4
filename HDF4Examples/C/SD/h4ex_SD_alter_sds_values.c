#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME "SDS.hdf"

int
main()
{
    /************************* Variable declaration **************************/

    int32 sd_id, sds_id, sds_index;
    int32 start[2], edges[2];
    int32 new_data[2];

    /********************* End of variable declaration ***********************/
    /*
     * Open the file and initialize the SD interface with write access.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_WRITE)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Select the first data set.
     */
    sds_index = 0;
    sds_id    = SDselect(sd_id, sds_index);

    /*
     * Set up the start and edge parameters to write new element values
     * into 10th row, 2nd column place, and 11th row, 2nd column place.
     */
    start[0] = 9; /* starting at 10th row   */
    start[1] = 1; /* starting at 2nd column */
    edges[0] = 2; /* rows 10th and 11th     */
    edges[1] = 1; /* column 2nd only        */

    /*
     * Initialize buffer with the new values to be written.
     */
    new_data[0] = new_data[1] = 1000;

    /*
     * Write the new values.
     */
    if (SDwritedata(sds_id, start, NULL, edges, (void *)new_data) == FAIL)
        printf("*** ERROR from SDwritedata\n");

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
