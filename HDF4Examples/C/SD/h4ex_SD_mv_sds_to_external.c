#include <stdio.h>

#include "mfhdf.h"

#define FILE_NAME     "SDS.hdf"
#define EXT_FILE_NAME "ExternalSDS"
#define OFFSET        24

int
main()
{

    /************************* Variable declaration **************************/

    int32 sd_id, sds_id, sds_index;

    /********************* End of variable declaration ***********************/

    /*
     * Open the file and initialize the SD interface.
     */
    if ((sd_id = SDstart(FILE_NAME, DFACC_WRITE)) == FAIL)
        printf("*** ERROR from SDstart\n");

    /*
     * Select the first data set.
     */
    sds_index = 0;
    sds_id    = SDselect(sd_id, sds_index);

    /*
     * Create a file with the name EXT_FILE_NAME and move the data set
     * values into it, starting at byte location OFFSET.
     */
    if (SDsetexternalfile(sds_id, EXT_FILE_NAME, OFFSET) == FAIL)
        printf("*** ERROR from SDsetexternalfile\n");

    /*
     * Terminate access to the data set, SD interface, and file.
     */
    if (SDendaccess(sds_id) == FAIL)
        printf("*** ERROR from SDendaccess\n");
    if (SDend(sd_id) == FAIL)
        printf("*** ERROR from SDend\n");

    return 0;
}
