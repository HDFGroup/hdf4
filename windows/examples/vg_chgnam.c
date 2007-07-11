
#include "hdf.h"

main( ) 
{

     int32      file_id, vg_id, vg_ref;
     intn       status;

     /* Open the HDF file. */
     file_id = Hopen("sd.hdf", DFACC_WRITE, 0);
     printf ("Hopen returns: %i\n", file_id);

     /* Initialize HDF Vgroup/Vdata interface. */
     status = Vstart(file_id);
     printf ("Vstart returns: %i\n", status);

     vg_ref = Vfind(file_id, "data1");
     printf ("Vfind returns: %i\n", vg_ref);
     
     if (vg_ref > 0)
     {
        vg_id = Vattach (file_id, vg_ref, "w");
        printf ("Vattach returns: %i\n", vg_id);

        status = Vsetname(vg_id, "SDS 1");
        printf ("Vsetname returns: %i\n", status);

        /* Terminate access to the Vdata. */
        status = Vdetach(vg_id);
        printf ("Vdetach returns: %i\n", status);

     }

     /* Terminate access to the Vset interface and close the file. */
     status = Vend(file_id);
     printf ("Vend returns: %i\n", status);
     status = Hclose(file_id);
     printf ("Hclose returns: %i\n", status);

}

