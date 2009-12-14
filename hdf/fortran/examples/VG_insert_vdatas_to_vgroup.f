      program  add_vdatas_to_a_vgroup
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*8  VG_NAME
      character*10 VG_CLASS
      character*15 VD1_NAME
      character*8  VD1_CLASS
      character*11 VD2_NAME
      character*13 VD2_CLASS
      character*9  VD3_NAME
      character*4  VD3_CLASS
C
      parameter (FILE_NAME = 'General_Vgroups.hdf',
     +           VG_NAME   = 'Vertices',
     +           VG_CLASS  = 'Vertex Set')
      parameter (VD1_NAME  = 'X,Y Coordinates',
     +           VD2_NAME  = 'Temperature',
     +           VD3_NAME  = 'Node List')
      parameter (VD1_CLASS = 'Position',
     +           VD2_CLASS = 'Property List',
     +           VD3_CLASS = 'Mesh')
      character*2 FIELD1_VD1
      character*2 FIELD2_VD1
      character*3 FIELD_VD2
      character*4 FIELD_VD3
      character*5 FIELDNAME_LIST
      parameter (FIELD1_VD1 = 'PX',
     +           FIELD2_VD1 = 'PY',
     +           FIELD_VD2  = 'TMP',
     +           FIELD_VD3  = 'PLIST',
     +           FIELDNAME_LIST = 'PX,PY')
      integer N_RECORDS
      parameter (N_RECORDS = 30)
      
      integer  DFACC_WRITE 
      parameter (DFACC_WRITE = 2)
      integer DFNT_FLOAT32, DFNT_INT16
      parameter (DFNT_FLOAT32 = 5, DFNT_INT16 = 22)
      integer FULL_INTERLACE 
      parameter (FULL_INTERLACE = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vfatch, vfsnam, vfscls, vfinsrt, vfdtch, vfend
      integer vsfatch, vsfsnam, vsfscls, vsffdef, vsfsfld, 
     +        vsfwrt, vsfwrtc, vsfdtch

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer vgroup_id
      integer vdata1_id, vdata2_id, vdata3_id, vd_index 
      integer num_of_records
      integer i, j, k
      real    pxy(2,N_RECORDS), tmp(N_RECORDS)
      integer plist(3,N_RECORDS)
      data pxy /-1.5, 2.3, -1.5, 1.98, -2.4, .67,
     +          -3.4, 1.46, -.65, 3.1, -.62, 1.23,
     +          -.4, 3.8, -3.55, 2.3, -1.43, 2.44,
     +          .23, 1.13, -1.4, 5.43, -1.4, 5.8,
     +          -3.4, 3.85, -.55, .3, -.21, 1.22,
     +          -1.44, 1.9, -1.4, 2.8, .94, 1.78,
     +          -.4, 2.32, -.87, 1.99, -.54, 4.11,
     +          -1.5, 1.35, -1.4, 2.21, -.22, 1.8,
     +          -1.1, 4.55, -.44, .54, -1.11, 3.93,
     +          -.76, 1.9, -2.34, 1.7, -2.2, 1.21/
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for writing.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the V interface.
C
      status = vfstart(file_id)
C
C     Buffer the data for the third and second vdatas.
C
      do 20 i = 1, N_RECORDS
         do 10 j = 1, 3 
            plist(j,i) = k 
            k = k+1
10       continue
20    continue    
      do 30 i = 1, N_RECORDS
         tmp(i) = (i-1) * 10.0
30    continue
C
C     Create a vgroup and set its name and class.
C     Note that the vgroup's reference number is set to -1 for creating
C     and the access mode is 'w' for writing.
C
      vgroup_id = vfatch(file_id, -1 , 'w')
      status    = vfsnam(vgroup_id, VG_NAME)
      status    = vfscls(vgroup_id, VG_CLASS)
C
C     Create the first vdata then set its name and class. Note that the vdata's
C     reference number is set to -1 for creating and the access mode is 'w' for
C     writing.
C
      vdata1_id = vsfatch(file_id, -1, 'w')
      status = vsfsnam(vdata1_id, VD1_NAME)
      status = vsfscls(vdata1_id, VD1_CLASS)
C
C     Introduce and define the fields of the first vdata.
C
      status = vsffdef(vdata1_id, FIELD1_VD1, DFNT_FLOAT32, 1)
      status = vsffdef(vdata1_id, FIELD2_VD1, DFNT_FLOAT32, 1)
      status = vsfsfld(vdata1_id, FIELDNAME_LIST)
C
C     Write the buffered data into the first vdata.
C
      num_of_records = vsfwrt(vdata1_id, pxy, N_RECORDS,
     +                        FULL_INTERLACE)
C
C     Insert the vdata into the vgroup using its identifier.
C
      vd_index = vfinsrt(vgroup_id, vdata1_id)
C
C     Detach from the first vdata.
C
      status = vsfdtch(vdata1_id)
C
C     Create, write, and insert the second vdata to the vgroup using
C     steps similar to those used for the first vdata.
C
      vdata2_id = vsfatch(file_id, -1, 'w')
      status = vsfsnam(vdata2_id, VD2_NAME)
      status = vsfscls(vdata2_id, VD2_CLASS)
      status = vsffdef(vdata2_id, FIELD_VD2, DFNT_FLOAT32, 1)
      status = vsfsfld(vdata2_id, FIELD_VD2)
      num_of_records = vsfwrt(vdata2_id, tmp, N_RECORDS,
     +                        FULL_INTERLACE)
      vd_index = vfinsrt(vgroup_id, vdata2_id)
      status = vsfdtch(vdata2_id)
C
C     Create, write, and insert the third vdata to the vgroup using 
C     steps similar to those used for the first and second vdatas.
C
      vdata3_id = vsfatch(file_id, -1, 'w')
      status = vsfsnam(vdata3_id, VD3_NAME)
      status = vsfscls(vdata3_id, VD3_CLASS)
      status = vsffdef(vdata3_id, FIELD_VD3, DFNT_INT16, 3)
      status = vsfsfld(vdata3_id, FIELD_VD3)
      num_of_records = vsfwrtc(vdata3_id, plist, N_RECORDS,
     +                        FULL_INTERLACE)
      vd_index = vfinsrt(vgroup_id, vdata3_id)
      status = vsfdtch(vdata3_id)
 
C
C     Terminate access to the vgroup 'Vertices'.
C
      status = vfdtch(vgroup_id)
C
C     Terminate access to the V interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end
