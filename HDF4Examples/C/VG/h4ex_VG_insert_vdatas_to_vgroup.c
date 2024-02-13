#include <stdio.h>

#include "hdf.h"

#define FILE_NAME      "General_Vgroups3.hdf"
#define N_RECORDS      30 /* number of records in the vdatas */
#define ORDER          3  /* order of field FIELD_VD2 */
#define VG_NAME        "Vertices"
#define VG_CLASS       "Vertex Set"
#define VD1_NAME       "X,Y Coordinates" /* first vdata to hold X,Y...*/
#define VD1_CLASS      "Position"        /*...values of the vertices */
#define VD2_NAME       "Temperature"     /* second vdata to hold the...*/
#define VD2_CLASS      "Property List"   /*...temperature field */
#define VD3_NAME       "Node List"       /* third vdata to hold...*/
#define VD3_CLASS      "Mesh"            /*...the list of nodes */
#define FIELD1_VD1     "PX"              /* first field of first vdata - X values */
#define FIELD2_VD1     "PY"              /* second field of first vdata - Y values */
#define FIELD_VD2      "TMP"             /* field of third vdata */
#define FIELD_VD3      "PLIST"           /* field of second vdata */
#define FIELDNAME_LIST "PX,PY"           /* field name list for first vdata */
/* Note that the second and third vdatas can use the field names as
   the field name lists unless more fields are added to a vdata.
   Then a field name list is needed for that vdata */

int
main()
{
    /************************* Variable declaration **************************/

    int32   file_id, vgroup_id;
    int32   vdata1_id, vdata2_id, vdata3_id;
    int8    i, j, k = 0;
    float32 pxy[N_RECORDS][2] = /* buffer for data of the first vdata */
        {{-1.5, 2.3},  {-1.5, 1.98}, {-2.4, .67},   {-3.4, 1.46}, {-.65, 3.1},  {-.62, 1.23},
         {-.4, 3.8},   {-3.55, 2.3}, {-1.43, 2.44}, {.23, 1.13},  {-1.4, 5.43}, {-1.4, 5.8},
         {-3.4, 3.85}, {-.55, .3},   {-.21, 1.22},  {-1.44, 1.9}, {-1.4, 2.8},  {.94, 1.78},
         {-.4, 2.32},  {-.87, 1.99}, {-.54, 4.11},  {-1.5, 1.35}, {-1.4, 2.21}, {-.22, 1.8},
         {-1.1, 4.55}, {-.44, .54},  {-1.11, 3.93}, {-.76, 1.9},  {-2.34, 1.7}, {-2.2, 1.21}};
    float32 tmp[N_RECORDS];      /* buffer for data of the second vdata */
    int16   plist[N_RECORDS][3]; /* buffer for data of the third vdata */

    /********************** End of variable declaration ***********************/

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
     * Buffer the data for the second and third vdatas.
     */
    for (i = 0; i < N_RECORDS; i++)
        for (j = 0; j < ORDER; j++)
            plist[i][j] = ++k;

    for (i = 0; i < N_RECORDS; i++)
        tmp[i] = i * 10.0;

    /*
     * Create the vgroup then set its name and class.  Note that the vgroup's
     * reference number is set to -1 for creating and the access mode is "w" for
     * writing.
     */
    vgroup_id = Vattach(file_id, -1, "w");
    if (Vsetname(vgroup_id, VG_NAME) == FAIL)
        printf("*** ERROR from Vsetname\n");
    if (Vsetclass(vgroup_id, VG_CLASS) == FAIL)
        printf("*** ERROR from Vsetclass\n");

    /*
     * Create the first vdata then set its name and class. Note that the vdata's
     * reference number is set to -1 for creating and the access mode is "w" for
     * writing.
     */
    vdata1_id = VSattach(file_id, -1, "w");
    if (VSsetname(vdata1_id, VD1_NAME) == FAIL)
        printf("*** ERROR from VSsetname\n");
    if (VSsetclass(vdata1_id, VD1_CLASS) == FAIL)
        printf("*** ERROR from VSsetclass\n");

    /*
     * Introduce and define the fields of the first vdata.
     */
    if (VSfdefine(vdata1_id, FIELD1_VD1, DFNT_FLOAT32, 1) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSfdefine(vdata1_id, FIELD2_VD1, DFNT_FLOAT32, 1) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSsetfields(vdata1_id, FIELDNAME_LIST) == FAIL)
        printf("*** ERROR from VSsetfields\n");

    /*
     * Write the buffered data into the first vdata with full interlace mode.
     */
    VSwrite(vdata1_id, (uint8 *)pxy, N_RECORDS, FULL_INTERLACE);

    /*
     * Insert the vdata into the vgroup using its identifier.
     */
    Vinsert(vgroup_id, vdata1_id);

    /*
     * Detach from the first vdata.
     */
    if (VSdetach(vdata1_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
     * Create, write, and insert the second vdata to the vgroup using
     * steps similar to those used for the first vdata.
     */
    vdata2_id = VSattach(file_id, -1, "w");
    if (VSsetname(vdata2_id, VD2_NAME) == FAIL)
        printf("*** ERROR from VSsetname\n");
    if (VSsetclass(vdata2_id, VD2_CLASS) == FAIL)
        printf("*** ERROR from VSsetclass\n");
    if (VSfdefine(vdata2_id, FIELD_VD2, DFNT_FLOAT32, 1) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSsetfields(vdata2_id, FIELD_VD2) == FAIL)
        printf("*** ERROR from VSsetfields\n");
    VSwrite(vdata2_id, (uint8 *)tmp, N_RECORDS, FULL_INTERLACE);
    Vinsert(vgroup_id, vdata2_id);
    if (VSdetach(vdata2_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
     * Create, write, and insert the third vdata to the vgroup using
     * steps similar to those used for the first and second vdatas.
     */
    vdata3_id = VSattach(file_id, -1, "w");
    if (VSsetname(vdata3_id, VD3_NAME) == FAIL)
        printf("*** ERROR from VSsetname\n");
    if (VSsetclass(vdata3_id, VD3_CLASS) == FAIL)
        printf("*** ERROR from VSsetclass\n");
    if (VSfdefine(vdata3_id, FIELD_VD3, DFNT_INT16, 3) == FAIL)
        printf("*** ERROR from VSfdefine\n");
    if (VSsetfields(vdata3_id, FIELD_VD3) == FAIL)
        printf("*** ERROR from VSsetfields\n");
    VSwrite(vdata3_id, (uint8 *)plist, N_RECORDS, FULL_INTERLACE);
    Vinsert(vgroup_id, vdata3_id);
    if (VSdetach(vdata3_id) == FAIL)
        printf("*** ERROR from VSdetach\n");

    /*
     * Terminate access to the vgroup "Vertices".
     */
    if (Vdetach(vgroup_id) == FAIL)
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
