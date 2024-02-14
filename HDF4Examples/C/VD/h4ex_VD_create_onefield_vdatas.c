#include <stdio.h>

#include "hdf.h"

#define FILE_NAME   "h4ex_VD_create_onefield_vdatas.hdf"
#define CLASS1_NAME "5x1 Array"
#define CLASS2_NAME "6x4 Array"
#define VDATA1_NAME "First Vdata"
#define VDATA2_NAME "Second Vdata"
#define FIELD1_NAME "Single-component Field"
#define FIELD2_NAME "Multi-component Field"
#define N_RECORDS_1 5 /* number of records the first vdata contains  */
#define N_RECORDS_2 6 /* number of records the second vdata contains */
#define ORDER_2     4 /* order of the field in the second vdata      */
                      /* Note that the order of the field in the first vdata is 1 */

int
main()
{
    /************************* Variable declaration **************************/

    int32 file_id;

    /*
     * Define an array to buffer the data of the first vdata.
     */
    char8 vdata1_buf[N_RECORDS_1] = {'V', 'D', 'A', 'T', 'A'};

    /*
     * Define an array to buffer the data of the second vdata.
     */
    int32 vdata2_buf[N_RECORDS_2][ORDER_2] = {{1, 2, 3, 4},   {2, 4, 6, 8},    {3, 6, 9, 12},
                                              {4, 8, 12, 16}, {5, 10, 15, 20}, {6, 12, 18, 24}};

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file for writing.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_WRITE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Create the first vdata and populate it with data from the vdata1_buf
     * array. Note that the buffer vdata1_buf is cast to (uint8 *) for the
     * benefit of generic data type.
     */
    VHstoredata(file_id, FIELD1_NAME, (uint8 *)vdata1_buf, N_RECORDS_1, DFNT_CHAR8, VDATA1_NAME, CLASS1_NAME);

    /*
     * Create the second vdata and populate it with data from the vdata2_buf
     * array.
     */
    VHstoredatam(file_id, FIELD2_NAME, (uint8 *)vdata2_buf, N_RECORDS_2, DFNT_INT32, VDATA2_NAME, CLASS2_NAME,
                 ORDER_2);

    /*
     * Terminate access to the VS interface and close the HDF file.
     */
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
