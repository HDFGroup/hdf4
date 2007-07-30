/******************************************************************/
/*                                                                */
/*  sd_addvg.c                                                    */ 
/*                                                                */
/*  Description:                                                  */
/*     Create an HDF file, create a vgroup and an empty SDS.      */ 
/*     Then add the SDS to the vgroup.                            */
/*                                                                */
/*                                                                */
/******************************************************************/
#include "hdf.h"

main( ) 
{ 

     int32    file_id, vgroup_id, vdata_id, status; 
     int32    sid, sds_id, sref, dims[1];

     dims[0] = 10;

     /* Create and open the file and initiate the SD interface */
     sid = SDstart("addsdvg.hdf", DFACC_CREATE);

     /* Open the HDF file for the Vgroup interface */
     file_id = Hopen("addsdvg.hdf", DFACC_WRITE, 0);

     /* Create the array data set */
     sds_id = SDcreate(sid, "sd1", DFNT_INT32, 1, dims);

     /* Initialize HDF for subsequent Vgroup/Vdata access. */
     status = Vstart(file_id);

     /* Create a Vgroup. */
     vgroup_id = Vattach(file_id, -1, "w"); 

     /* Set the name and class for this Vgroup. */
     status = Vsetname(vgroup_id, "VG_Name_1");
     status = Vsetclass(vgroup_id, "VG_Class_1");

     /* Add mysd1 into the Vgroup */
     sref = SDidtoref(sds_id);
     status = Vaddtagref(vgroup_id, DFTAG_NDG, sref);

     /* Terminate access to the SD interface */
     status = SDendaccess (sds_id);
     status = SDend (sid);

     /* Terminate access to the Vgroup interface. */
     status = Vdetach(vgroup_id);
     status = Vend(file_id);

     /* Close the HDF file. */
     status = Hclose(file_id);
     
}
     
     
