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
C $Id$
C
      subroutine tanf (number_failed)
C
C
C  Test program: stores annotations in a file.
C                Writes several SDSs and corresponding RISs to a file.
C                Writes labels and descriptions for all but the first 
C                   three SDSs.
C                Writes labels and descriptions for all RISs.
C
C  Input file:  none
C  Output files: tdfanF.hdf
C
C  Possible bug:  When reading in a label, we have to give it a 
C                 length that is one greater than MAXLEN_LAB. This
C                 may be due to a bug in dfan.c in DFANIgetann().
C
C                 This was addressed in BUG 1640: Added isfortran 
C                 flag to DFANIgetann to avoid termination 
C                 of the string with the null character when the routine 
C                 is call from FORTRAN, hence no longer need to add one.
C

      implicit none
      include 'fortest.inc'

      integer number_failed
      character*20 myname
      parameter (myname = 'an')

      integer ISFIRST, NOTFIRST, MAXLEN_LAB
      integer MAXLEN_DESC, ROWS, COLS, REPS

      parameter ( ISFIRST =        1, 
     *            NOTFIRST =       0, 
     *            MAXLEN_LAB =    30,
     *            MAXLEN_DESC =  500,
     *            ROWS =          10, 
     *            COLS =          10,
     *            REPS =           2 )

      integer refnum
      integer ret
      integer rank
      integer j, dimsizes(2)

      character*30 labsds, labris
      character*500 descsds, descris
      character pal(768)
      character*64 TESTFILE

      character*1 CR
      character image(ROWS, COLS), newimage(ROWS, COLS)
      real      data(ROWS, COLS)


      call ptestban('Testing', myname)
      number_failed =  0
      CR = char(10)
      TESTFILE = 'tdfanF.hdf'

C *** set up object labels and descriptions ***

      labsds = 'Object label #1: sds'
      labris = 'Object label #2: image'
      descsds = 'Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 ' 
     *          // CR // '                13 14 15 16 17 18 19 20 '
     *          // ' **END SDS DESCR**'
      descris = 'Object Descr #2: A B C D E F G H I J K L '
     *          // CR // '                M N O **END IMAGE DESCR **'

C  *** generate float array and image ***
 
      rank = 2
      dimsizes(1)=ROWS 
      dimsizes(2)=COLS

      call gen2Dfloat(ROWS, COLS, data)
      call genimage(ROWS, COLS, data, image)

      ret = dssdims(rank,dimsizes)
      call VRFY(ret, 'dssdims', number_failed)

C  ***  Write labels and descriptions ***

      call MESSAGE(VERBO_MED,
     +        '***  Writing labels & descriptions with SDS and RIS ***')

      do 100 j=1,REPS
C         write out scientific data set 
          ret = dsadata(TESTFILE, rank,dimsizes, data)
          call VRFY(ret, 'dsadata', number_failed)

C         write out annotations for 2 out of every 3 
          if (mod(j,3) .ne. 0) then 
              refnum = dslref()
              ret = daplab(TESTFILE, DFTAG_SDG, refnum, labsds)
              call VRFY(ret, 'daplab', number_failed)
              ret = dapdesc(TESTFILE, DFTAG_SDG, refnum, 
     *                                     descsds, len(descsds))
              call VRFY(ret, 'dapdesc', number_failed)
          endif

          ret = d8aimg(TESTFILE, image, COLS, ROWS, 0)
          call VRFY(ret, 'd8aimg', number_failed)
          refnum = DFR8lastref()
          ret = daplab(TESTFILE, DFTAG_RIG, refnum, labris)
          call VRFY(ret, 'daplab', number_failed)
          ret = dapdesc(TESTFILE,DFTAG_RIG,refnum, descris, 
     *                                                 len(descris))
          call VRFY(ret, 'dapdesc', number_failed)
  100 continue


C********  Read labels and descriptions *********

      call MESSAGE(VERBO_MED,
     +        '*** Reading labels and descriptions for SDS and RIS ***')

      do 200 j=1,REPS

          ret = dsgdims(TESTFILE, rank,dimsizes,3)
          call VRFY(ret, 'dsgdims', number_failed)
          refnum = dslref()

C         read in annotations for 2 out of every 3 
          if (mod(j,3) .ne. 0) then
              call an_check_lab_desc(TESTFILE, DFTAG_SDG, refnum, 
     *                                  labsds, descsds, number_failed)
          endif

          ret = d8gimg(TESTFILE, newimage, COLS, ROWS, pal)
          call VRFY(ret, 'd8gimg', number_failed)
          refnum = DFR8lastref()
          call an_check_lab_desc(TESTFILE, DFTAG_RIG, refnum, 
     *                                labris, descris, number_failed)
      
  200 continue

      if ( number_failed .eq. 0 ) then
          call MESSAGE(VERBO_HI,
     +        '***** ALL DFAN TESTS SUCCESSFUL ***** ')
      else
          print *,'***** ',number_failed,' TESTS FAILED ***** '
      endif

      return
      end


C**************************************************************
C
C  an_check_lab_desc:  read and compare label and description
C                   with expected ones
C
C**************************************************************
      subroutine an_check_lab_desc(filename, tag, ref, label, desc, 
     *                                                   number_failed)
      implicit none
      include 'fortest.inc'

      character*(*) filename, label, desc
      integer tag, ref, number_failed

      integer MAXLEN_LAB, MAXLEN_DESC
      parameter ( MAXLEN_LAB =    30,
     *            MAXLEN_DESC =  500 )

      integer  inlablen, indesclen, ret
      character*30   inlabel
      character*500 indesc

      inlablen =  dagllen(filename, tag, ref)
      call VRFY(inlablen, 'dagllen', number_failed)

      if (inlablen .ne. len(label)) then
          print *,'   >>>BAD LABEL LENGTH.'
          print *,'                        IS: ', inlablen
          print *,'                 SHOULD BE: ', len(label)
          number_failed = number_failed + 1
      endif

      ret = daglab(filename, tag, ref, inlabel, MAXLEN_LAB)
      call VRFY(ret, 'daglab', number_failed)
      if (inlabel .ne. label) then
          print *,'   >>>BAD LABEL.'
          print *,'                        IS: ', inlabel
          print *,'                 SHOULD BE: ', label
          number_failed = number_failed + 1
      endif

      indesclen = dagdlen(filename, tag, ref)
      call VRFY(indesclen, 'dagdlen', number_failed)
      if (indesclen .ne. len(desc)) then
          print *,'   >>>BAD DESCRIPTION LENGTH.' 
          print *,'                        IS: ', indesclen 
          print *,'                 SHOULD BE: ', len(desc) 
          number_failed = number_failed + 1 
      else 
          ret = dagdesc(filename, tag, ref, indesc, MAXLEN_DESC)
          call VRFY(ret, 'dagdesc', number_failed)
          if (indesc .ne. desc) then
              print *,'   >>>BAD DESCRIPTION.' 
              print *,'                        IS: ', indesc 
              print *,'                 SHOULD BE: ', desc 
              number_failed = number_failed + 1 
          endif
      endif

      return
      end

