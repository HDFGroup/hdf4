#include <stdio.h>

#include "hdf.h"

#define FILE_NAME "h4ex_VG_create_vgroup.hdf"

int
main()
{
    /************************* Variable declaration **************************/

    int32 vgroup_ref = -1;
    int32 vgroup1_id, vgroup2_id, file_id;

    /********************** End of variable declaration **********************/

    /*
     * Create the HDF file.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_CREATE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the V interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Create the first vgroup.  Note that the vgroup reference number is set
     * to -1 for creating and the access mode is "w" for writing.
     */
    vgroup1_id = Vattach(file_id, vgroup_ref, "w");

    /*
     * Create the second vgroup.
     */
    vgroup2_id = Vattach(file_id, vgroup_ref, "w");

    /*
     * Any operations on the vgroups.
     */

    /*
     * Terminate access to the first vgroup.
     */
    if (Vdetach(vgroup1_id) == FAIL)
        printf("*** ERROR from Vdetach\n");

    /*
     * Terminate access to the second vgroup.
     */
    if (Vdetach(vgroup2_id) == FAIL)
        printf("*** ERROR from Vdetach\n");

    /*
     * Terminate access to the V interface and close the HDF file.
     */
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
