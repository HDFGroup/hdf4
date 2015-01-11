      program vdata_info 
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      integer      DFACC_READ, FULL_INTERLACE
      integer      FIELD_SIZE
C
      parameter (FILE_NAME      = 'General_Vdatas.hdf',
     +           DFACC_READ     = 1,
     +           FULL_INTERLACE = 0,
     +           FIELD_SIZE     = 80)
      
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsfgid, vsfinq,
     +        vsfisat, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer      status
      integer      file_id, vdata_id, vdata_ref
      integer      n_records, interlace_mode, vdata_size
      character*64 vdata_name
      character*80 fieldname_list 
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
C     Set the reference number to -1 to start the search from the beginning
C     of the file. 
C     
      vdata_ref = -1
10    continue
C
C     Use vsfgid to obtain each vdata by its reference number then
C     attach to the vdata and get information. The loop terminates
C     when the last vdata is reached.
C
      vdata_ref = vsfgid(file_id, vdata_ref)
      if (vdata_ref .eq. -1) goto 100
C
C     Attach to the current vdata for reading.
C
      vdata_id = vsfatch(file_id, vdata_ref, 'r')
C
C     Test whether the current vdata is not a storage for an attribute,
C     then obtain and display its information.
      if (vsfisat(vdata_id) .ne. 1) then
C     Initialize buffers before getting values back.
          vdata_name = ' ' 
          fieldname_list = ' '
          n_records = -1 
          vdata_size = -1 
          status = vsfinq(vdata_id, n_records, interlace_mode,
     +                    fieldname_list, vdata_size, vdata_name)
          if (status .eq. 0) then
             write(*,*) 'Vdata: ', vdata_name
             write(*,*) 'contains ', n_records, ' records'
             if (interlace_mode .eq. 0) then
                 write(*,*) 'Interlace mode: FULL'
             else	 
                 write(*,*) 'Interlace mode: NONE'
             endif
             write(*,*) 'Fields: ', fieldname_list(1:30)
             write(*,*) 'Vdata record size in bytes :', vdata_size
             write(*,*)
          endif
      endif
C
C     Detach from the current vdata.
C
      status = vsfdtch(vdata_id)
      goto 10 
100   continue
C
C     Terminate access to the vdata and to the VS interface, and
C     close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
