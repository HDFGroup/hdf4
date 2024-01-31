      program create_onefield_vdatas
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      character*9  CLASS1_NAME
      character*9  CLASS2_NAME
      character*11 VDATA1_NAME
      character*12 VDATA2_NAME
      character*22 FIELD1_NAME
      character*21 FIELD2_NAME
      integer      N_RECORDS_1, N_RECORDS_2
      integer      ORDER_2
C
      parameter (FILE_NAME   = 'General_Vdatas.hdf',
     +           CLASS1_NAME = '5x1 Array',
     +           CLASS2_NAME = '6x4 Array',
     +           VDATA1_NAME = 'First Vdata',
     +           VDATA2_NAME = 'Second Vdata',
     +           FIELD1_NAME = 'Single-component Field',
     +           FIELD2_NAME = 'Multi-component Field')
      parameter (N_RECORDS_1 = 5,
     +           N_RECORDS_2 = 6,
     +           ORDER_2     = 4)               
   
      integer DFACC_WRITE, DFNT_CHAR8, DFNT_INT32
      parameter (DFACC_WRITE = 2,
     +           DFNT_CHAR8  = 4,
     +           DFNT_INT32  = 24)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vhfscd, vhfsdm, vfend 

C
C**** Variable declaration *******************************************
C
      integer   status
      integer   file_id
      integer   vdata1_ref, vdata2_ref 
      character vdata1_buf(N_RECORDS_1)
      integer   vdata2_buf(ORDER_2, N_RECORDS_2)
      data vdata1_buf /'V','D','A','T','A'/
      data vdata2_buf / 1,  2,  3,  4,
     +                  2,  4,  6,  8,
     +                  3,  6,  9, 12,
     +                  4,  8, 12, 16,
     +                  5, 10, 15, 20,
     +                  6, 12, 18, 24/
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
C     Create the first vdata and populate it with data from vdata1_buf array.
C     
      vdata1_ref = vhfscd(file_id, FIELD1_NAME, vdata1_buf, N_RECORDS_1,
     +                    DFNT_CHAR8, VDATA1_NAME, CLASS1_NAME)
C
C     Create the second vdata and populate it with data from vdata2_buf array.
C     
      vdata2_ref = vhfsdm(file_id, FIELD2_NAME, vdata2_buf, N_RECORDS_2,
     +                    DFNT_INT32, VDATA2_NAME, CLASS2_NAME,
     +                    ORDER_2)
C
C     Terminate access to the VS interface and close the HDF file.
C
      status = vfend(file_id)
      status = hclose(file_id)
      end
