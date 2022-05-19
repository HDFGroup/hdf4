#include "hdf.h"

#define  FILE_NAME    "Two_Vgroups.hdf"

int main()
{
   /************************* Variable declaration **************************/

   int32 vgroup_ref = -1,
         vgroup1_id, vgroup2_id, file_id;

   /********************** End of variable declaration **********************/

   /*
   * Create the HDF file.
   */
   file_id = Hopen (FILE_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the V interface.
   */
   Vstart (file_id);

   /*
   * Create the first vgroup.  Note that the vgroup reference number is set
   * to -1 for creating and the access mode is "w" for writing.
   */
   vgroup1_id = Vattach (file_id, vgroup_ref, "w");

   /*
   * Create the second vgroup.
   */
   vgroup2_id = Vattach (file_id, vgroup_ref, "w");

   /*
   * Any operations on the vgroups.
   */

   /*
   * Terminate access to the first vgroup.
   */
   Vdetach (vgroup1_id);

   /*
   * Terminate access to the second vgroup.
   */
   Vdetach (vgroup2_id);

   /*
   * Terminate access to the V interface and close the HDF file.
   */
   Vend (file_id);
   Hclose (file_id);
   return 0;
}
