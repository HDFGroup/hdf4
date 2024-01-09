      program  create_vgroup
      implicit none
C
C     Parameter declaration
C
      character*15 FILE_NAME
C
      parameter (FILE_NAME = 'Two_Vgroups.hdf')
      integer DFACC_CREATE
      parameter (DFACC_CREATE = 4)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfdtch, vfend

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer vgroup1_id, vgroup2_id, vgroup_ref
C
C**** End of variable declaration ************************************
C
C
C     Create the HDF file.
C
      file_id = hopen(FILE_NAME, DFACC_CREATE, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Create the first vgroup. Note that the vgroup reference number is set
C     to -1 for creating and the access mode is 'w' for writing.
C
      vgroup_ref = -1
      vgroup1_id = vfatch(file_id, vgroup_ref, 'w')
C
C     Create the second vgroup.
C
      vgroup2_id = vfatch(file_id, vgroup_ref, 'w')
C
C     Any operations on the vgroups.
C
C     ..............................
C
C     Terminate access to the first vgroup.
C
      status = vfdtch(vgroup1_id)
C
C     Terminate access to the second vgroup.
C
      status = vfdtch(vgroup2_id)
C
C     Terminate access to the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end
