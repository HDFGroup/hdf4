C * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C  Copyright by The HDF Group.                                               *
C  Copyright by the Board of Trustees of the University of Illinois.         *
C  All rights reserved.                                                      *
C                                                                            *
C  This file is part of HDF.  The full HDF copyright notice, including       *
C  terms governing use, modification, and redistribution, is contained in    *
C  the files COPYING and Copyright.html.  COPYING can be found at the root   *
C  of the source code distribution tree; Copyright.html can be found at      *
C  http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
C  access to either file, you may request a copy from help@hdfgroup.org.     *
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
            print *, 'test_file failed with ', err1, ' errors'
         endif
         if (err1 .eq. 0) then
            print *, 'test_file.................. PASSED'
         endif
         call test_vars(err2)
         if (err2 .ne. 0) then
            print *, 'test_vars failed with ', err2, ' errors'
         endif
         if (err2 .eq. 0) then
            print *, 'test_vars.................. PASSED'
            print *,' '
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
        subroutine   test_file(err)

        include 'mffunc.inc'

        integer      DFACC_CREATE
        parameter   (DFACC_CREATE = 4)
        integer      err,status
        integer      sd_id
        character*13 file
        character*13 file_out
        integer      flen


C--------------------End of declarations------------------------------

        file = 'test_file.hdf'
C
C  Initialize SD interfaces
C
        sd_id = sfstart (file, DFACC_CREATE)
            if( sd_id .le. 0 ) then
                print *, 'sfstart failed'
                err = err + 1
                goto 1000
            endif
        status = sfgetnamelen(sd_id, flen) 
            if( status .ne. 0 .or. flen .ne. 13) then
                print *, 'sfgetnamelen failed'
                err = err + 1
            endif
        flen_out = sfgetfname(sd_id, file_out)
            if( flen_out .ne. 13 ) then
                print *, 'sfgetfname failed: wrong length'
                err = err + 1
            endif
            if( file_out(1:13) .ne. file(1:13) ) then
                print *, 'sfgetfname failed: wrong name'
                err = err + 1
            endif

        
        status = sfend(sd_id)
            if( status .ne. 0 ) then
                print *, 'sfend failed'
                err = err + 1
            endif
1000    return
        end

C=============================================================
C This new subroutine tests the following SD Fortran functions
C       sfidtype
C       sfgnvars_byname
C       sfname2ind
C
C=============================================================
        subroutine   test_vars(err)

        include 'mffunc.inc'

        integer      err,status
        integer      sd_id
        character*12 file
        integer      DFACC_CREATE
        parameter   (DFACC_CREATE = 4)

C--------------------End of declarations------------------------------
        file = 'test_var.hdf'
C
C  Initialize SD interfaces
C
        sd_id = sfstart (file, DFACC_CREATE)
            if( sd_id .le. 0 ) then
                print *, 'sfstart failed'
                err = err + 1
                goto 1000
            endif
        status = sfend(sd_id)
            if( status .ne. 0 ) then
                print *, 'sfend failed'
                err = err + 1
            endif
1000    return
        end

