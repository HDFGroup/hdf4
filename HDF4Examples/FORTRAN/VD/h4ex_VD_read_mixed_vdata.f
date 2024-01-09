      program read_mixed_vdata 
      implicit none
C
C     Parameter declaration
C
      character*16 FILE_NAME
      character*16 VDATA_NAME
      character*4  FIELD1_NAME
      character*5  FIELD2_NAME
      character*10 FIELDNAME_LIST
      integer      N_RECORDS, N_FIELDS
      integer      BUFFER_SIZE 
C
      parameter (FILE_NAME       = 'Packed_Vdata.hdf',
     +           VDATA_NAME      = 'Mixed Data Vdata',
     +           FIELD1_NAME     = 'Temp',
     +           FIELD2_NAME     = 'Ident',
     +           FIELDNAME_LIST = 'Temp,Ident')
      parameter (N_RECORDS   = 20,
     +           N_FIELDS    = 2, 
     +           BUFFER_SIZE = (4 + 1)*N_RECORDS)               
   
      integer DFACC_READ, DFNT_FLOAT32, DFNT_CHAR8,
     +        FULL_INTERLACE, HDF_VSUNPACK  
      parameter (DFACC_READ       = 1,
     +           DFNT_FLOAT32     = 5,
     +           DFNT_CHAR8       = 4,
     +           FULL_INTERLACE   = 0,
     +           HDF_VSUNPACK     = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsffnd, vsfsfld,
     +        vsfnpak, vsfcpak, vsfread, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer   status
      integer   file_id, vdata_id
      integer   vdata_ref, num_of_records
      real      temp(N_RECORDS)
      character ident(N_RECORDS) 
      integer   i
C
C     Buffer for read packed data should be big enough to hold N_RECORDS.
C
      integer   databuf(BUFFER_SIZE/4 + 1)
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the VS interface.
C
      status = vfstart(file_id) 
C
C     Get the reference number of the vdata, whose name is specified in
C     VDATA_NAME, using vsffnd, which will be discussed in Section 4.7.3. 
C     
      vdata_ref = vsffnd(file_id, VDATA_NAME)
C
C     Attach to the vdata for reading if it is found, 
C     otherwise exit the program.
C
      if (vdata_ref .eq. 0) stop
      vdata_id = vsfatch(file_id, vdata_ref, 'r') 
C
C     Specify the fields that will be read. 
C
      status = vsfsfld(vdata_id, FIELDNAME_LIST)

C
C     Read N_RECORDS records of the vdata and store the values into the databuf.
C
      num_of_records = vsfread(vdata_id, databuf, N_RECORDS,
     +                         FULL_INTERLACE)
C
C     Unpack N_RECORDS from databuf into temp and ident arrays.
C     In Fortran, each field is unpacked using separate calls to 
C     vsfnpak or vsfcpak.
C
      status = vsfnpak(vdata_id, HDF_VSUNPACK, FIELDNAME_LIST, databuf,
     +                 BUFFER_SIZE, num_of_records, FIELD1_NAME, temp) 
      status = vsfcpak(vdata_id, HDF_VSUNPACK, FIELDNAME_LIST, databuf,
     +                 BUFFER_SIZE, num_of_records, FIELD2_NAME, ident) 
C
C     Display the read data being stored in the field databufs.
C
      write (*,*) '    Temp  Ident'
      do 10 i = 1, num_of_records
         write(*,1000) temp(i), ident(i)
10    continue 
1000  format (3x,F6.2, 4x, a)
C
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
