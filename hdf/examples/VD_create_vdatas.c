#include "hdf.h"

/* Used to make certain a return value _is_not_ a value.  If not ture, */
/* print error messages, increment num_err and return. */
#define CHECK(ret, val, where) \
do {if(ret == val) {printf("*** ERROR from %s is %ld at line %4d in %s\n", where, (long)ret, (int)__LINE__,__FILE__);} \
} while(0)

#define  FILE1_NAME     "General_Vdatas.hdf"
#define  FILE2_NAME     "Two_Vdatas.hdf"
#define  VDATA_NAME     "Vdata 1"
#define  VDATA_CLASS    "Empty Vdatas"

int main( )
{
   /************************* Variable declaration **************************/

   intn  status_n;      /* returned status for functions returning an intn  */
   int32 status_32,     /* returned status for functions returning an int32 */
         file1_id, file2_id,
         vdata_id, vdata1_id, vdata2_id,
         vdata_ref = -1;     /* ref number of a vdata, set to -1 to create  */

   /********************** End of variable declaration **********************/

   /*
   * Create the first HDF file.
   */
   file1_id = Hopen (FILE1_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the VS interface associated with the first HDF file.
   */
   status_n = Vstart (file1_id);
   CHECK(status_n, FAIL, "Vstart");

   /*
   * Create a vdata in the first HDF file.
   */
   vdata_id = VSattach (file1_id, vdata_ref, "w");

   /*
   * Assign a name to the vdata.
   */
   status_32 = VSsetname (vdata_id, VDATA_NAME);
   CHECK(status_32, FAIL, "VSsetname");

   /*
   * Other operations on the vdata identified by vdata_id can be carried
   * out starting from this point.
   */

   /*
   * Create the second HDF file.
   */
   file2_id = Hopen (FILE2_NAME, DFACC_CREATE, 0);

   /*
   * Initialize the VS interface associated with the second HDF file.
   */
   status_n = Vstart (file2_id);
   CHECK(status_n, FAIL, "Vstart");

   /*
   * Create the first vdata in the second HDF file.
   */
   vdata1_id = VSattach (file2_id, vdata_ref, "w");

   /*
   * Create the second vdata in the second HDF file.
   */
   vdata2_id = VSattach (file2_id, vdata_ref, "w");

   /*
   * Assign a class name to these vdatas.
   */
   status_32 = VSsetclass (vdata1_id, VDATA_CLASS);
   CHECK(status_32, FAIL, "VSsetclass");
   status_32 = VSsetclass (vdata2_id, VDATA_CLASS);
   CHECK(status_32, FAIL, "VSsetclass");

   /*
   * Other operations on the vdatas identified by vdata1_id and vdata2_id
   * can be carried out starting from this point.
   */

   /*
   * Terminate access to the first vdata in the second HDF file.
   */
   status_32 = VSdetach (vdata1_id);
   CHECK(status_32, FAIL, "VSdetach");

   /*
   * Terminate access to the second vdata in the second HDF file.
   */
   status_32 = VSdetach (vdata2_id);
   CHECK(status_32, FAIL, "VSdetach");

   /*
   * From this point on, any operations on the vdatas identified by vdata1_id
   and vdata2_id are invalid but not on the vdata identified by vdata_id.
   */

   /*
   * Terminate access to the VS interface associated with the second HDF file.
   */
   status_n = Vend (file2_id);
   CHECK(status_n, FAIL, "Vend");

   /*
   * Close the second HDF file.
   */
   status_n = Hclose (file2_id);
   CHECK(status_n, FAIL, "Hclose");

   /*
   * Terminate access to the vdata in the first HDF file.
   */
   status_32 = VSdetach (vdata_id);
   CHECK(status_32, FAIL, "VSdetach");

   /*
   * Terminate access to the VS interface associated with the first HDF file.
   */
   status_n = Vend (file1_id);
   CHECK(status_n, FAIL, "Vend");

   /*
   * Close the first HDF file.
   */
   status_n = Hclose (file1_id);
   CHECK(status_n, FAIL, "Hclose");

   return 0;
}
