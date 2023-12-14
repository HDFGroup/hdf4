      program read_from_vdata 
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      character*14 VDATA_NAME
      character*20 FIELDNAME_LIST
      integer      N_RECORDS, RECORD_INDEX
      integer      ORDER_1, ORDER_2
      integer      N_VALS_PER_REC
C
      parameter (FILE_NAME       = 'General_Vdatas.hdf',
     +           VDATA_NAME      = 'Solid Particle',
     +           FIELDNAME_LIST = 'Position,Temperature')
      parameter (N_RECORDS  = 5,
     +           RECORD_INDEX = 3,
     +           ORDER_1    = 3,
     +           ORDER_2    = 2,
     +           N_VALS_PER_REC = ORDER_1 + ORDER_2 )               
   
      integer DFACC_READ, FULL_INTERLACE
      parameter (DFACC_READ     = 1,
     +           FULL_INTERLACE = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsffnd, vsfatch, vsfsfld, vsfrd, vsfseek,
     +        vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, vdata_id
      integer vdata_ref, rec_num, num_of_records, rec_pos
      real    databuf(N_VALS_PER_REC, N_RECORDS)
      integer i
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
C     Place the current point to the position specified in RECORD_INDEX.
C
      rec_pos = vsfseek(vdata_id, RECORD_INDEX) 
C
C     Read the next N_RECORDS from the vdata and store the data in the buffer 
C     databuf with fully interlace mode. 
C
      num_of_records = vsfrd(vdata_id, databuf, N_RECORDS,
     +                        FULL_INTERLACE)
C
C     Display the read data as many records as the number of records returned
C     by vsfrd.
C
      write(*,*) '  Particle Position     Temperature Range'
      write(*,*)
      do 10 rec_num = 1, num_of_records
         write(*,1000) (databuf(i, rec_num), i = 1, N_VALS_PER_REC)
10    continue 
1000  format(1x,3(f6.2), 8x,2(f6.2))
C
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
