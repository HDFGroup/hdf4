      program  vgroup_attribute
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*9  VGROUP_NAME
      character*15 VGATTR_NAME
C
      parameter (FILE_NAME    = 'General_Vgroups.hdf',
     +           VGROUP_NAME  = 'SD Vgroup',
     +           VGATTR_NAME  = 'First Attribute')
      integer VSET_NEW_VERSION, VSET_VERSION, VSET_OLD_VERSION
      parameter (VSET_NEW_VERSION = 4,
     +           VSET_VERSION     = 3,
     +           VSET_OLD_VERSION = 2)  
      integer DFACC_WRITE 
      parameter (DFACC_WRITE = 2)
      integer DFNT_CHAR
      parameter (DFNT_CHAR = 4)
      integer N_ATT_VALUES
      parameter (N_ATT_VALUES = 6)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfgver, vfscatt, vfnatts, vfainfo,
     +        vfind, vfgcatt, vfdtch, vfend
C
C**** Variable declaration *******************************************
C
      integer status, n_attrs
      integer file_id
      integer vgroup_id, vgroup_ref, vg_version
      integer attr_index, i
      integer data_type, n_values, size 
      character vg_attr(N_ATT_VALUES)
      character vgattr_buf(N_ATT_VALUES), attr_name(30)
      data vg_attr /'v','g','r','o','u','p'/
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading/writing.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Get the reference number of the vgroup named VGROUP_NAME.
C
      vgroup_ref = vfind(file_id, VGROUP_NAME)
C
C     Attach to the vgroup found.
C
      vgroup_id = vfatch(file_id, vgroup_ref , 'w')
C
C     Get and display the version of the attached vgroup.
C
      vg_version = vfgver(vgroup_id)
      if (vg_version .eq. VSET_NEW_VERSION) write(*,*)
     +   VGROUP_NAME, ' is of the newest version, version 4'
      if (vg_version .eq. VSET_VERSION) write(*,*)
     +   VGROUP_NAME, ' is of a version between 3.2 and 4.0r2'
      if(vg_version .eq. VSET_OLD_VERSION) write(*,*)
     +   VGROUP_NAME, ' is of version before 3.2'
      if ((vg_version .ne. VSET_NEW_VERSION) .and.
     +    (vg_version .ne. VSET_VERSION)     .and.
     +    (vg_version .ne. VSET_OLD_VERSION)) write(*,*)   
     +    'Unknown version'
C
C     Add the attribute named VGATTR_NAME to the vgroup.
C
      status = vfscatt(vgroup_id, VGATTR_NAME, DFNT_CHAR, N_ATT_VALUES,
     +                 vg_attr)
C
C     Get and display the number of attributes attached to this group.
C
      n_attrs = vfnatts(vgroup_id)
      write(*,*) 'This group has', n_attrs, ' attributes'
C
C     Get and display the name and the number of values of each attribute.
C
      do 10 attr_index=1, n_attrs
         attr_name = ' '
         status = vfainfo(vgroup_id, attr_index-1, attr_name, data_type,
     +                    n_values, size)
      write(*,*) 'Attribute #', attr_index-1, ' is named ', attr_name
      write(*,*) 'and has', n_values, ' values: '
C
C     Get and display the attribute values.
C
      status = vfgcatt(vgroup_id, attr_index-1, vgattr_buf)
      write(*,*) (vgattr_buf(i), i=1,n_values)
10    continue
C
C     Terminate access to the vgroup.
C
      status = vfdtch(vgroup_id)
C
C     Terminate accessto the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end
