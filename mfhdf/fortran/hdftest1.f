C * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C  Copyright by The HDF Group.                                               *
C  Copyright by the Board of Trustees of the University of Illinois.         *
C  All rights reserved.                                                      *
C                                                                            *
C  This file is part of HDF.  The full HDF copyright notice, including       *
C  terms governing use, modification, and redistribution, is contained in    *
C  the COPYING file, which can be found at the root of the source code       *
C  distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
C  If you do not have access to either file, you may request a copy from     *
C  help@hdfgroup.org.                                                        *
C * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C
C This is a test driver for new SD Fortran functions
C
      program test_new_functions
      implicit none

      integer err1, err2

      err1=0
      err2=0
      call test_file(err1)
      if (err1 .ne. 0) then
         print*, 'test_file failed with ', err1, ' errors'
      endif
      if (err1 .eq. 0) then
         print*, 'test_file.................. PASSED'
      endif
      call test_vars(err2)
      if (err2 .ne. 0) then
         print*, 'test_vars failed with ', err2, ' errors'
      endif
      if (err2 .eq. 0) then
         print*, 'test_vars.................. PASSED'
         print*,' '
      endif
      end
          
C=============================================================
C
C This new subroutine tests the following SD Fortran functions
C       sfgetname
C       sfgetnamelen
C       sfgmaxopenf
C       sfgnumopenf
C       sfrmaxopenf
C
C=============================================================
      subroutine test_file(err)
      
      implicit none
      
      include 'mffunc.inc'
      
      integer      DFACC_CREATE
      parameter   (DFACC_CREATE = 4)
      integer      err,status
      integer      sd_id
      character*20 file
      character*20 file_out
      integer      flen
      integer      curr_max, sys_limit
      integer NUM_FILES_LOW
      parameter (NUM_FILES_LOW=35)
      integer fids(NUM_FILES_LOW)
      integer H4_MAX_NC_OPEN 
      parameter (H4_MAX_NC_OPEN=32)
      integer flen_out
      character*2 ichr2
      integer i, curr_opened

C--------------------End of declarations------------------------------

      file = 'test_file.hdf'

C Get the current max and system limit

      status = sfgmaxopenf(curr_max, sys_limit)
      if( status .ne. 0 ) then
         print*,'sfgmaxopenf failed'
         err = err + 1
      endif
      if(curr_max .ne. H4_MAX_NC_OPEN)THEN
         print*,'sfgmaxopenf failed: Incorrect max number of open files'
         err = err + 1
      endif
           
C Reset current max to an arbitrary number and check
      
      curr_max = sfrmaxopenf(40)
      if( status .ne. 0 ) then
         print*,'sfrmaxopenf failed'
         err = err + 1
      endif
      if( curr_max .ne. 40 ) then
         print*,'sfrmaxopenf failed: Current max number of open files'
         err = err + 1
      endif

C Try to create more files than the default max (currently, 32) and
C all should succeed

      do i = 1, NUM_FILES_LOW
         write(ichr2,'(I2.2)') i
         fids(i) = sfstart ('test_file'//ichr2//'.hdf', DFACC_CREATE)
         if( fids(i) .le. 0 ) then
            print*, 'sfstart failed'
            err = err + 1
         endif
      enddo

C verify that NUM_FILES_LOW files are opened
      status = sfgnumopenf(curr_opened)
      if( curr_opened .ne. NUM_FILES_LOW ) then
         print*, 'sfgnumopenf failed: Incorrect number of open files'
         err = err + 1
      endif

C Close all the files
      
      do i = 1, NUM_FILES_LOW
         status = sfend(fids(i))
         if( status .ne. 0 ) then
            print*, 'sfend failed'
            err = err + 1
         endif
      enddo
C
C  Initialize SD interfaces
C
      sd_id = sfstart (file, DFACC_CREATE)
      if( sd_id .le. 0 ) then
         print*, 'sfstart failed'
         err = err + 1
         goto 1000
      endif
      status = sfgetnamelen(sd_id, flen) 
      if( status .ne. 0 .or. flen .ne. 13) then
         print*, 'sfgetnamelen failed'
         err = err + 1
      endif
      flen_out = sfgetfname(sd_id, file_out)
      if( flen_out .ne. 13 ) then
         print*, 'sfgetfname failed: wrong length'
         err = err + 1
      endif
      if( file_out(1:13) .ne. file(1:13) ) then
         print*, 'sfgetfname failed: wrong name'
         err = err + 1
      endif
      status = sfend(sd_id)
      if( status .ne. 0 ) then
         print*, 'sfend failed'
         err = err + 1
      endif
 1000 return
      end

C=============================================================
C This new subroutine tests the following SD Fortran functions
C       sfidtype
C       sfgnvars_byname
C       sfn2indices
C
C=============================================================
      subroutine test_vars(err)
      
      implicit none
      
      include 'mffunc.inc'
      
      integer      dset1, dset2, dim_id
      integer      err,status
      integer      sd_id, id_type
      character*12 file
      integer      DFACC_CREATE
      parameter   (DFACC_CREATE = 4)
      integer      DFACC_READ
      parameter   (DFACC_READ = 1)
      integer      RANK
      parameter   (RANK=2)
      integer      X_LENGTH,Y_LENGTH
      parameter   (X_LENGTH=5)
      parameter   (Y_LENGTH=5)
      integer      dimsize(RANK)
      character*9  VAR1_NAME
      integer      var_list(3), type_list(3)
      character*21   ATTR1_NAME
      character*15   ATTR2_NAME
      character*19   ATTR1_VAL
      character*23   ATTR2_VAL
      integer ATTR1_LEN
      integer ATTR2_LEN
      real sds1_data(X_LENGTH,Y_LENGTH)
      integer start(2)
      integer stride(2)
C
C  Data type parameters
C
C 
      integer   DFNT_CHAR,
     .             DFNT_INT16,
     .             DFNT_INT32,
     .             DFNT_FLOAT32,
     .             DFNT_FLOAT64
      parameter (DFNT_CHAR   = 4,
     .             DFNT_INT16   = 22,
     .             DFNT_INT32   = 24,
     .             DFNT_FLOAT32 = 5,
     .             DFNT_FLOAT64 = 6)
      integer i,j
      integer n_vars


C--------------------End of declarations------------------------------
      file = 'test_var.hdf'
      VAR1_NAME = "DataSet_1"
      ATTR1_NAME = "Attribute Dimension 1"
      ATTR2_NAME = "Attribute SDS 1"
      ATTR1_VAL = "This is a coord var"
      ATTR2_VAL = "This is not a coord var"
      ATTR1_LEN = 21
      ATTR2_LEN = 15
      
      start(1) = 0
      start(2) = 0
      stride(1) = 1
      stride(2) = 1
      
      do i = 1, X_LENGTH
         do j = 1, Y_LENGTH
            sds1_data(j,i) = j*i
         enddo
      enddo
C     
C  Initialize SD interfaces
C
      sd_id = sfstart (file, DFACC_CREATE)
      if( sd_id .le. 0 ) then
         print*, 'sfstart failed'
         err = err + 1
         goto 1000
      endif

C   Create an X_LENGTH by Y_LENGTH dataset, called DataSet_1

      dimsize(1) = X_LENGTH
      dimsize(2) = Y_LENGTH
      
      dset1 = sfcreate(sd_id,VAR1_NAME,DFNT_FLOAT64,RANK,dimsize)
      if(dset1.eq.-1) then
         print*, 'sfcreate returned bad ID', dset1
         err = err + 1
      endif

C Set the dimension name to be the same as its dataset.
      dim_id = sfdimid(dset1, 0)
      if(dim_id.eq.-1)then
         print*,'sfgetdimid returned error code', status
         err = err + 1
      endif

      status = sfsdmname(dim_id, VAR1_NAME)
      if(status.eq.-1)then
         print*,'sfsdmname returned error code', status
         err = err + 1
      endif

C Set an attribute to dimension VAR1_NAME.
      status=sfsnatt(dim_id,ATTR1_NAME,DFNT_CHAR,ATTR1_LEN,ATTR1_VAL)
      if(status.eq.-1)then
         print*,'sfsnatt returned error code', status
         err = err + 1
      endif

C Set an attribute to dataset VAR1_NAME.
      status=sfsnatt(sd_id,ATTR2_NAME,DFNT_CHAR,ATTR2_LEN,ATTR2_VAL)
      if(status.eq.-1)then
         print*,'sfsnatt returned error code', status
         err = err + 1
      endif

C Create another X_LENGTH by Y_LENGTH dataset, called DataSet_2

      dset2 = sfcreate(sd_id,VAR1_NAME,DFNT_FLOAT64,RANK,dimsize)
      if(dset2.eq.-1) then
         print*, 'sfcreate returned bad ID', dset2
         err = err + 1
      endif

C Test SDidtype on the second dataset */
      status = sfidtype(dset2,id_type)
      if(status.eq.-1)then
         print*,  'sfidtype returned error code', status
         err = err + 1
      else if(id_type.ne.1) then
         print*, 'sfidtype returned bad object type', id_type
         err = err + 1
      endif

C Write data to the SDS

      status = sfwdata(dset1, start, stride, dimsize, sds1_data)
      if( status .ne. 0 ) then
         print*, 'sfwdata failed for', 1, ' -th dataset'
         err = err + 1
      endif

C Close the datasets          

      status = sfendacc(dset1)
      if(status.eq.-1)then
         print*,  'sfendacc returned error code', status
         err = err + 1
      end if

      status = sfendacc(dset2)
      if(status.eq.-1)then
         print*,  'sfendacc returned error code', status
         err = err + 1
      end if

C Close the file

      status = sfend(sd_id)
      if( status .ne. 0 ) then
         print*, 'sfend failed ', status
         err = err + 1
      endif
      
C Re-open the file to test SDidtype more */
      sd_id = sfstart (file, DFACC_READ)
      if( sd_id .le. 0 ) then
         print*, 'sfstart failed'
         err = err + 1
         goto 1000
      endif

C Check variable type and attributes of each element in the file

C Get the number of variables of name VAR1_NAME
      status = sfgnvars_byname(sd_id, VAR1_NAME, n_vars)
      if( status .eq. -1)then
         print*, 'sfgnvars_byname failed'
         err = err + 1
      endif
      
      if(n_vars.eq. 1)then
C        Get index of dataset VAR1_NAME
         status = sfn2index(sd_id, VAR1_NAME)
         if( status .lt. 0)then
            print*, 'sfn2index failed'
            err = err + 1
         endif
      else
C        Get the list of all variables of named VAR1_NAME
         status=sfn2indices(sd_id,VAR1_NAME,var_list,type_list,
     +                      n_vars)
         if( status .lt. 0)then
            print*, 'sfn2indices failed'
            err = err + 1
         endif
C        In this case, the first and third variable is a dataset
         do i = 1, n_vars
            if (type_list(i) .eq. 0)then
               if( var_list(i) .ne. i-1)then
                  print*, 'sfn2indices failed in indices'
                  err = err + 1
               endif
            endif
         enddo
      endif

C Test SDidtype on the SD id
      status = sfidtype(sd_id,id_type)
      if(status.eq.-1)then
         print*,  'sfidtype returned error code', status
         err = err + 1
      else if(id_type.ne.0) then
         print*, 'sfidtype returned bad object type', id_type
         err = err + 1
      endif
C Get access to the first dataset and test SDidtype on the SDS id

      dset1 = sfselect(sd_id, 0);
      if( dset1 .eq. -1 ) then
         print*, 'sfselect failed for', 0, ' -th dataset'
         err = err + 1
      endif
      
      status = sfidtype(dset1,id_type)
      if(status.eq.-1)then
         print*,  'sfidtype returned error code', status
         err = err + 1
      else if(id_type.ne.1) then
         print*, 'sfidtype returned bad object type', id_type
         err = err + 1
      endif

C Get access to the second dataset and test SDidtype on the SDS id
        
      dset2 = sfselect(sd_id, 1);
      if( dset2 .eq. -1 ) then
         print*, 'sfselect failed for', 1, ' -th dataset'
         err = err + 1
      endif
      
      status = sfidtype(dset2,id_type)
      if(status.eq.-1)then
         print*,'sfidtype returned error code', status
         err = err + 1
      else if(id_type.ne.1) then
         print*,'sfidtype returned bad object type', id_type
         err = err + 1
      endif

C Get dimension handle for first dimension of DataSet_1 and test
C SDidtype on the dimension id

      dim_id = sfdimid(dset1, 0)
      if(dim_id.eq.-1)then
         print*,'sfgetdimid returned error code', status
         err = err + 1
      endif
      
      status = sfidtype(dim_id,id_type)
      if(status.eq.-1)then
         print*,'sfidtype returned error code', status
         err = err + 1
      else if(id_type.ne.2) then
         print*,'sfidtype returned bad object type', id_type
         err = err + 1
      endif

C Close the datasets
      status = sfendacc(dset1)
      if(status.eq.-1)then
         print*,  'sfendacc returned error code', status
         err = err + 1
      end if

      status = sfendacc(dset2)
      if(status.eq.-1)then
         print*,  'sfendacc returned error code', status
         err = err + 1
      end if

C Close the file
      status = sfend(sd_id)
      if( status .ne. 0 ) then
         print*, 'sfend failed'
         err = err + 1
      endif
 1000 return
      end

