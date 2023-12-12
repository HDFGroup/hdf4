      program create_vdatas
      implicit none
C
C     Parameter declaration
C
      character*18 FILE1_NAME
      character*14 FILE2_NAME
      character*7  VDATA_NAME
      character*12 VDATA_CLASS
C
      parameter (FILE1_NAME  = 'General_Vdatas.hdf',
     +           FILE2_NAME  = 'Two_Vdatas.hdf',
     +           VDATA_NAME  = 'Vdata 1',
     +           VDATA_CLASS = 'Empty Vdatas')
      integer DFACC_CREATE
      parameter (DFACC_CREATE = 4)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsfsnam, vsfscls, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file1_id, file2_id
      integer vdata_id, vdata1_id, vdata2_id 
      integer vdata_ref
C
C**** End of variable declaration ************************************
C
C
C     Create the first HDF file.
C
      file1_id = hopen(FILE1_NAME, DFACC_CREATE, 0)
C
C     Initialize the VS interface associated with the first HDF file.
C
      status = vfstart(file1_id) 
C
C     Create a vdata in the first HDF file.
C
      vdata_ref = -1
      vdata_id = vsfatch(file1_id, vdata_ref, 'w')
C
C     Assign a name to the vdata.
C
      status = vsfsnam(vdata_id, VDATA_NAME)
C
C     Other operations on the vdata identified by vdata_id can be carried out
C     starting from this point.
C
C     Create the second HDF file.
C
      file2_id = hopen(FILE2_NAME, DFACC_CREATE, 0) 
C
C     Initialize the VS interface associated with the second HDF file.
C
      status = vfstart(file2_id) 
C
C     Create the first vdata in the second HDF file.
C
      vdata1_id = vsfatch(file2_id, vdata_ref, 'w')
C
C     Create the second vdata in the second HDF file.
C
      vdata2_id = vsfatch(file2_id, vdata_ref, 'w')
C
C     Assign a class name to these vdatas.
C
      status = vsfscls(vdata1_id, VDATA_CLASS)
      status = vsfscls(vdata2_id, VDATA_CLASS)
C
C     Other operations on the vdatas identified by vdata1_id and vdata2_id
C     can be carried out starting from this point.
C
C
C     Terminate access to the first vdata in the second HDF file.
C
      status = vsfdtch(vdata1_id)
C
C     Terminate access to the second vdata in the second HDF file.
C
      status = vsfdtch(vdata2_id)
C
C     Terminate access to the VS interface associated with the second HDF file.
C
      status = vfend(file2_id)
C
C     Close the second HDF file.
C
      status = hclose(file2_id)
C
C     Terminate access to the vdata in the first HDF file.
C
      status = vsfdtch(vdata_id)
C
C     terminate access to the VS interface associated with the first HDF file.
C
      status = vfend(file1_id)
C
C     Close the first HDF file.
C
      status = hclose(file1_id)
      end
