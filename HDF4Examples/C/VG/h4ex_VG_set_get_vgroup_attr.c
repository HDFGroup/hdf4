#include <stdio.h>

#include "hdf.h"

#define FILE_NAME    "General_Vgroups2.hdf"
#define VGROUP_NAME  "SD Vgroup"
#define VGATTR_NAME  "First Attribute"
#define N_ATT_VALUES 7 /* number of values in the attribute */

int
main()
{
    /************************* Variable declaration **************************/

    intn  n_attrs; /* number of attributes of the vgroup */
    int32 file_id, vgroup_ref, vgroup_id, attr_index, i, vg_version, n_values;
    char  vg_attr[N_ATT_VALUES] = {'v', 'g', 'r', 'o', 'u', 'p', '\0'};
    char  vgattr_buf[N_ATT_VALUES], attr_name[30];

    /********************** End of variable declaration **********************/

    /*
     * Open the HDF file for writing.
     */
    if ((file_id = Hopen(FILE_NAME, DFACC_WRITE, 0)) == FAIL)
        printf("*** ERROR from Hopen\n");

    /*
     * Initialize the V interface.
     */
    if (Vstart(file_id) == FAIL)
        printf("*** ERROR from Vstart\n");

    /*
     * Get the reference number of the vgroup named VGROUP_NAME.
     */
    vgroup_ref = Vfind(file_id, VGROUP_NAME);

    /*
     * Attach to the vgroup found.
     */
    vgroup_id = Vattach(file_id, vgroup_ref, "w");

    /*
     * Get and display the version of the attached vgroup.
     */
    vg_version = Vgetversion(vgroup_id);
    switch (vg_version) {
        case VSET_NEW_VERSION:
            printf("\nVgroup %s is of the newest version, version 4\n", VGROUP_NAME);
            break;
        case VSET_VERSION:
            printf("Vgroup %s is of a version between 3.2 and 4.0r2\n", VGROUP_NAME);
            break;
        case VSET_OLD_VERSION:
            printf("Vgroup %s is of version before 3.2\n", VGROUP_NAME);
            break;
        default:
            printf("Unknown version = %d\n", vg_version);
    } /* switch */

    /*
     * Add the attribute named VGATTR_NAME to the vgroup.
     */
    if (Vsetattr(vgroup_id, VGATTR_NAME, DFNT_CHAR, N_ATT_VALUES, vg_attr) == FAIL)
        printf("*** ERROR from Vsetattr\n");

    /*
     * Get and display the number of attributes attached to this vgroup.
     */
    n_attrs = Vnattrs(vgroup_id);
    printf("\nThis vgroup has %d attribute(s)\n", n_attrs);

    /*
     * Get and display the name and the number of values of each attribute.
     * Note that the fourth and last parameters are set to NULL because the type
     * and the size of the attribute are not desired.
     */
    for (attr_index = 0; attr_index < n_attrs; attr_index++) {
        if (Vattrinfo(vgroup_id, attr_index, attr_name, NULL, &n_values, NULL) == FAIL)
            printf("*** ERROR from Vattrinfo\n");
        printf("\nAttribute #%d is named %s and has %d values: ", attr_index + 1, attr_name, n_values);

        /*
         * Get and display the attribute values.
         */
        if (Vgetattr(vgroup_id, attr_index, vgattr_buf) == FAIL)
            printf("*** ERROR from Vgetattr\n");
        for (i = 0; i < n_values; i++)
            printf("%c ", vgattr_buf[i]);
        printf("\n");
    }

    /*
     * Terminate access to the vgroup and to the V interface, and close
     * the HDF file.
     */
    if (Vdetach(vgroup_id) == FAIL)
        printf("*** ERROR from Vdetach\n");
    if (Vend(file_id) == FAIL)
        printf("*** ERROR from Vend\n");
    if (Hclose(file_id) == FAIL)
        printf("*** ERROR from Hclose\n");

    return 0;
}
