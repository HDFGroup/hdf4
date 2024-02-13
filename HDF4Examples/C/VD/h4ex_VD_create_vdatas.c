#include <stdio.h>

#include "hdf.h"

#define FILE1_NAME  "h4ex_VD_create_vdatas.hdf"
#define FILE2_NAME  "Two_Vdatas.hdf"
#define VDATA_NAME  "Vdata 1"
#define VDATA_CLASS "Empty Vdatas"

int
main()
{
    /************************* Variable declaration **************************/

    int32 file1_id, file2_id, vdata_id, vdata1_id, vdata2_id,
        vdata_ref = -1; /* ref number of a vdata, set to -1 to create  */

    /********************** End of variable declaration **********************/

    /*
     * Create the first HDF file.
     */
    if ((file1_id = Hopen(FILE1_NAME, DFACC_CREATE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface associated with the first HDF file.
     */
    if (Vstart(file1_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Create a vdata in the first HDF file.
     */
    vdata_id = VSattach(file1_id, vdata_ref, "w");

    /*
     * Assign a name to the vdata.
     */
    if (VSsetname(vdata_id, VDATA_NAME) == FAIL)
        printf("*** ERROR from VSsetname\n");

    /*
     * Other operations on the vdata identified by vdata_id can be carried
     * out starting from this point.
     */

    /*
     * Create the second HDF file.
     */
    if ((file2_id = Hopen(FILE2_NAME, DFACC_CREATE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the VS interface associated with the second HDF file.
     */
    if (Vstart(file2_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Create the first vdata in the second HDF file.
     */
    vdata1_id = VSattach(file2_id, vdata_ref, "w");

    /*
     * Create the second vdata in the second HDF file.
     */
    vdata2_id = VSattach(file2_id, vdata_ref, "w");

    /*
     * Assign a class name to these vdatas.
     */
    if (VSsetclass(vdata1_id, VDATA_CLASS) == FAIL)
        printf("*** ERROR from VSsetclass\n");
    if (VSsetclass(vdata2_id, VDATA_CLASS) == FAIL)
        printf("*** ERROR from VSsetclass\n");

    /*
     * Other operations on the vdatas identified by vdata1_id and vdata2_id
     * can be carried out starting from this point.
     */

    /*
     * Terminate access to the first vdata in the second HDF file.
     */
    if (VSdetach(vdata1_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
     * Terminate access to the second vdata in the second HDF file.
     */
    if (VSdetach(vdata2_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
    * From this point on, any operations on the vdatas identified by vdata1_id
    and vdata2_id are invalid but not on the vdata identified by vdata_id.
    */

    /*
     * Terminate access to the VS interface associated with the second HDF file.
     */
    if (Vend(file2_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file2_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    /*
     * Terminate access to the vdata in the first HDF file.
     */
    if (VSdetach(vdata_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
     * Terminate access to the VS interface associated with the first HDF file.
     */
    if (Vend(file1_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file1_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
