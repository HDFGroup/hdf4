#include <stdio.h>

#include "hdf.h"

#define FILE_NAME       "General_Vdatas.hdf"
#define SEARCHED_FIELDS "Position,Temperature"

int
main()
{
    /************************* Variable declaration **************************/

    int32 file_id, vdata_id, vdata_ref;
    int32 index = 0;    /* index of the vdata in the file - manually kept   */
    int8  found_fields; /* TRUE if the specified fields exist in the vdata  */

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file for reading.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_READ, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Set the reference number to -1 to start the search from
     * the beginning of file.
     */
    vdata_ref = -1;

    /*
     * Assume that the specified fields are not found in the current vdata.
     */
    found_fields = FALSE;

    /*
     * Use VSgetid to obtain each vdata by its reference number then
     * attach to the vdata and search for the fields.  The loop
     * terminates when the last vdata is reached or when a vdata which
     * contains the fields listed in SEARCHED_FIELDS is found.
     */
    while ((vdata_ref = VSgetid(file_id, vdata_ref)) != FAIL) {
        vdata_id = VSattach(file_id, vdata_ref, "r");
        if (VSfexist(vdata_id, SEARCHED_FIELDS) != FAIL) {
            found_fields = TRUE;
            break;
        }

        /*
         * Detach from the current vdata before continuing searching.
         */
        if (VSdetach(vdata_id) == FAIL)
            printf("*** ERROR from VSdetach\n");

        index++; /* advance the index by 1 for the next vdata */
    }

    /*
     * Print the index of the vdata containing the fields or a "not found"
     * message if no such vdata is found.  Also detach from the vdata found.
     */
    if (!found_fields)
        printf("Fields Position and Temperature were not found.\n");
    else {
        printf("Fields Position and Temperature found in the vdata at position %d\n", index);
        if (VSdetach(vdata_id) == FAIL)
            printf("*** ERROR from VSdetach\n");
    }

    /*
     * Terminate access to the VS interface and close the HDF file.
     */
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
