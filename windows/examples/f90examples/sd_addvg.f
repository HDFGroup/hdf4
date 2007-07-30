      PROGRAM ADD_SD_TO_VGROUP

      implicit none

      integer file_id, vgroup_id, sref
      integer*4  sd_id, sds_id, istat 
      integer dims(1)
      integer hopen, vfatch, vfsnam, vfscls
      integer vfadtr, vfdtch
      integer hclose, vfstart, vfend
      integer sfstart, sfcreate, sfid2ref
      integer sfendacc, sfend
      integer DFACC_CREATE, DFACC_WRITE, DFNT_INT32, DFTAG_NDG
      parameter (DFACC_CREATE = 4, DFACC_WRITE = 2, DFNT_INT32 = 24,
     +           DFTAG_NDG =720)

      dims(1)=10

C     Create and open the file and initiate the SD interface.
      sd_id = sfstart('addsdvg.hdf', DFACC_CREATE)

C     Open the HDF file with Read/Write access.
      file_id = hopen('addsdvg.hdf', DFACC_WRITE, 0)

C     Create the SDS 
      sds_id = sfcreate(sd_id, 'sd1', DFNT_INT32, 1, dims)

C     Initialize HDF for subsequent Vgroup/Vdata access. 
      istat = vfstart(file_id)
      vgroup_id = vfatch(file_id, -1, 'w') 

C     Set the name and class for this vgroup. 
      istat = vfsnam(vgroup_id, 'VG_Name_1')
      istat = vfscls(vgroup_id, 'VG_Class_1')

C     Get the SDS reference number
      sref = sfid2ref (sds_id) 

C     Insert the SDS into the vgroup. 
      istat = vfadtr(vgroup_id, DFTAG_NDG, sref)

C     Terminate access to the array.
      istat = sfendacc(sds_id)
 
C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)

C     Terminate access to the V interface. 
      istat = vfdtch(vgroup_id)
      istat = vfend(file_id)

C     Close the HDF file. 
      istat = hclose(file_id)

      end
