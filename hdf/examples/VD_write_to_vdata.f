      program write_to_vdata 
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      character*13 CLASS_NAME
      character*14 VDATA_NAME
      character*8  FIELD1_NAME
      character*4  FIELD2_NAME
      character*11 FIELD3_NAME
      character*27 FIELDNAME_LIST
      integer      N_RECORDS
      integer      ORDER_1, ORDER_2, ORDER_3
      integer      N_VALS_PER_REC
C
      parameter (FILE_NAME       = 'General_Vdatas.hdf',
     +           CLASS_NAME      = 'Particle Data',
     +           VDATA_NAME      = 'Solid Particle',
     +           FIELD1_NAME     = 'Position',
     +           FIELD2_NAME     = 'Mass',
     +           FIELD3_NAME     = 'Temperature',
     +           FIELDNAME_LIST = 'Position,Mass,Temperature')
      parameter (N_RECORDS = 10,
     +           ORDER_1   = 3,
     +           ORDER_2   = 1,
     +           ORDER_3   = 2,
     +           N_VALS_PER_REC = ORDER_1 + ORDER_2 + ORDER_3)               
   
      integer DFACC_WRITE, DFNT_FLOAT32, FULL_INTERLACE
      parameter (DFACC_WRITE    = 2,
     +           DFNT_FLOAT32   = 5,
     +           FULL_INTERLACE = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsfsnam, vsfscls, vsffdef, vsfsfld,
     +        vsfwrt, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, vdata_id
      integer vdata_ref, rec_num, num_of_records
      real    data_buf(N_VALS_PER_REC, N_RECORDS)
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
      status = vsffdef(vdata_id, FIELD1_NAME, DFNT_FLOAT32, ORDER_1)
      status = vsffdef(vdata_id, FIELD2_NAME, DFNT_FLOAT32, ORDER_2)
      status = vsffdef(vdata_id, FIELD3_NAME, DFNT_FLOAT32, ORDER_3)
C
C     Finalize the definition of the fields.
C
      status = vsfsfld(vdata_id, FIELDNAME_LIST)
C
C     Buffer the data by the record for fully interlaced mode. Note that the
C     first three elements contain the three values of the first field,
C     the forth element contains the value of the second field, and the last two
C     elements contain the two values of the third field.
C
      do 10 rec_num = 1, N_RECORDS
         data_buf(1, rec_num) = 1.0 * rec_num
         data_buf(2, rec_num) = 2.0 * rec_num
         data_buf(3, rec_num) = 3.0 * rec_num
         data_buf(4, rec_num) = 0.1 + rec_num
         data_buf(5, rec_num) = 0.0
         data_buf(6, rec_num) = 65.0
10    continue
C
C     Write the data from data_buf to the vdata with the full interlacing mode.
C
      num_of_records = vsfwrt(vdata_id, data_buf, N_RECORDS,
     +                        FULL_INTERLACE)
C
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
