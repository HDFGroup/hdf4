      program write_mixed_vdata 
      implicit none
C
C     Parameter declaration
C
      character*16 FILE_NAME
      character*18 CLASS_NAME
      character*16 VDATA_NAME
      character*4  FIELD1_NAME
      character*6  FIELD2_NAME
      character*5  FIELD3_NAME
      character*5  FIELD4_NAME
      character*23 FIELDNAME_LIST
      integer      N_RECORDS, N_FIELDS, ORDER
      integer      BUF_SIZE 
C
      parameter (FILE_NAME       = 'Packed_Vdata.hdf',
     +           CLASS_NAME      = 'General Data Class',
     +           VDATA_NAME      = 'Mixed Data Vdata',
     +           FIELD1_NAME     = 'Temp',
     +           FIELD2_NAME     = 'Height',
     +           FIELD3_NAME     = 'Speed',
     +           FIELD4_NAME     = 'Ident',
     +           FIELDNAME_LIST = 'Temp,Height,Speed,Ident')
      parameter (N_RECORDS = 20,
     +           N_FIELDS  = 4, 
     +           ORDER     = 1,
     +           BUF_SIZE = (4 + 2 + 4 + 1)*N_RECORDS)               
   
      integer DFACC_WRITE, DFNT_FLOAT32, DFNT_INT16, DFNT_CHAR8,
     +        FULL_INTERLACE, HDF_VSPACK  
      parameter (DFACC_WRITE    = 2,
     +           DFNT_FLOAT32   = 5,
     +           DFNT_INT16     = 22,
     +           DFNT_CHAR8     = 4,
     +           FULL_INTERLACE = 0,
     +           HDF_VSPACK     = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsfsnam, vsfscls, vsffdef, vsfsfld,
     +        vsfnpak, vsfcpak, vsfwrit, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer   status
      integer   file_id, vdata_id
      integer   vdata_ref, num_of_records
      real      temp(N_RECORDS)
      integer*2 height(N_RECORDS)
      real      speed(N_RECORDS)
      character ident(N_RECORDS) 
      integer   i
C
C     Buffer for packed data should be big enough to hold N_RECORDS.
C
      integer   databuf(BUF_SIZE/4 + 1)
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for writing.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the VS interface.
C
      status = vfstart(file_id) 
C
C     Create a new vdata.
C     
      vdata_ref = -1
      vdata_id = vsfatch(file_id, vdata_ref, 'w') 
C
C     Set name and class name of the vdata.
C
      status = vsfsnam(vdata_id, VDATA_NAME)
      status = vsfscls(vdata_id, CLASS_NAME)
C
C     Introduce each field's name, data type, and order. This is the
C     first part in defining a field.
C
      status = vsffdef(vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER)
      status = vsffdef(vdata_id, FIELD2_NAME, DFNT_INT16, ORDER)
      status = vsffdef(vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER)
      status = vsffdef(vdata_id, FIELD4_NAME, DFNT_CHAR8, ORDER)
C
C     Finalize the definition of the fields.
C
      status = vsfsfld(vdata_id, FIELDNAME_LIST)
C
C     Enter data values into the field databufs by the records.
C
      do 10 i = 1, N_RECORDS
         temp(i)   = 1.11 * i
         height(i) = i - 1
         speed(i)  = 1.11 * i
         ident(i)  = char(64+i)
10    continue
C
C     Pack N_RECORDS of data into databuf. In Fortran, each field is packed 
C     using separate calls to vsfnpak or vsfcpak.
C
      status = vsfnpak(vdata_id, HDF_VSPACK, ' ', databuf, BUF_SIZE,
     +                 N_RECORDS, FIELD1_NAME, temp) 
      status = vsfnpak(vdata_id, HDF_VSPACK, ' ', databuf, BUF_SIZE,
     +                 N_RECORDS, FIELD2_NAME, height) 
      status = vsfnpak(vdata_id, HDF_VSPACK, ' ', databuf, BUF_SIZE,
     +                 N_RECORDS, FIELD3_NAME, speed) 
      status = vsfcpak(vdata_id, HDF_VSPACK, ' ', databuf, BUF_SIZE,
     +                 N_RECORDS, FIELD4_NAME, ident) 
C
C     Write all the records of the packed data to the vdata.
C
      num_of_records = vsfwrit(vdata_id, databuf, N_RECORDS,
     +                         FULL_INTERLACE)
C
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
