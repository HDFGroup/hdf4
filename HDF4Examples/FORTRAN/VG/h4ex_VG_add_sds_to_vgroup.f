      program  add_SDS_to_a_vgroup
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*7  SDS_NAME
      character*9  VG_NAME
      character*13 VG_CLASS
C
      parameter (FILE_NAME = 'General_Vgroups.hdf',
     +           SDS_NAME  = 'Test SD',
     +           VG_NAME   = 'SD Vgroup',
     +           VG_CLASS  = 'Common Vgroups')
      integer DFACC_CREATE, DFACC_WRITE 
      parameter (DFACC_CREATE = 4, DFACC_WRITE = 2)
      integer DFNT_INT32
      parameter (DFNT_INT32 = 24)
      integer DFTAG_NDG
      parameter (DFTAG_NDG = 720)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfsnam, vfscls, vfadtr, vfdtch, vfend
      integer sfstart, sfcreate, sfid2ref, sfendacc, sfend

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer vgroup_id
      integer sd_id, sds_id, sds_ref
      integer dim_sizes(1), rank
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
C     Initialize SD interface. 
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE) 
C
C     Set the rank and the size of SDS's dimension.
C
      rank = 1
      dim_sizes(1) = 10
C
C     Create the SDS.
C
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT32, rank, dim_sizes)
C
C     Create a vgroup and set its name and class.
C
      vgroup_id = vfatch(file_id, -1 , 'w')
      status    = vfsnam(vgroup_id, VG_NAME)
      status    = vfscls(vgroup_id, VG_CLASS)
C
C     Obtain the reference number of the SDS using its identifier.
C
      sds_ref = sfid2ref(sds_id)
C
C     Add the SDS to the vgroup. Note: the tag DFTAG_NDG is used
C     when adding an SDS.  Refer to HDF Reference Manual, Section III, Table 3K,
C     for the entire list of tags.
C
      status = vfadtr(vgroup_id, DFTAG_NDG, sds_ref)
C
C     Terminate access to the SDS and to the SD interface.
C
      status = sfendacc(sds_id)
      status = sfend(sd_id)
C
C     Terminate access to the vgroup.
C
      status = vfdtch(vgroup_id)
C
C     Terminate access to the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end
