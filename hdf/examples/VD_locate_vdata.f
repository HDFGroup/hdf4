      program locate_vdata 
      implicit none
C
C     Parameter declaration
C
      character*18 FILE_NAME
      character*20 SEARCHED_FIELDS 
C
      parameter (FILE_NAME       = 'General_Vdatas.hdf',
     +           SEARCHED_FIELDS = 'Position,Temperature')
      integer DFACC_READ
      parameter (DFACC_READ = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer vfstart, vsfatch, vsfgid, vsfex, vsfdtch, vfend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, vdata_id, vdata_ref
      integer index 
      logical found_fields 
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
      index = 0
C
C     Set the reference number to -1 to start the search from the beginning
C     of the file. 
C     
      vdata_ref = -1
C
C     Assume that the specified fields are not found in the current vdata.
C
      found_fields = .FALSE.
10    continue
C
C     Use vsfgid to obtain each vdata by its reference number then
C     attach to the vdata and search for the fields. The loop terminates
C     when the last vdata is reached or when a vdata which contains the
C     fields listed in SEARCHED_FIELDS is found.
C
      vdata_ref = vsfgid(file_id, vdata_ref)
      if (vdata_ref .eq. -1) goto 100
      vdata_id = vsfatch(file_id, vdata_ref, 'r')
      status = vsfex(vdata_id, SEARCHED_FIELDS)
      if (status .ne. -1) then
          found_fields = .TRUE.
          goto 100
      endif
      status = vsfdtch(vdata_id)
      index = index + 1 
      goto 10
100   continue
C
C     Print the index of the vdata containing the fields or a 'not found'
C     message if no such vdata is found. Also detach from the vdata found.
C
      if(.NOT.found_fields) then
         write(*,*) 'Fields Positions and Temperature were not found'
      else
         write(*,*)
     +   'Fields Positions and Temperature were found in the vdata',
     +   ' at position ', index
C
C        Terminate access to the vdata
C
         status = vsfdtch(vdata_id)
      endif  
C
C     Terminate access to the VS interface and close the HDF file.
C
      status = vsfdtch(vdata_id)
      status = vfend(file_id)
      status = hclose(file_id)
      end
