C****************************************************************************
C* NCSA HDF                                                                 *
C* Software Development Group                                               *
C* National Center for Supercomputing Applications                          *
C* University of Illinois at Urbana-Champaign                               *
C* 605 E. Springfield, Champaign IL 61820                                   *
C*                                                                          *
C* For conditions of distribution and use, see the accompanying             *
C* hdf/COPYING file.                                                        *
C*                                                                          *
C****************************************************************************
C
C $Id$
C
C
      program tdfanF
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

      integer daplab, dapdesc  
      integer dssdims, dsadata, dslref, dsgdims
      integer d8aimg, DFR8lastref, d8gimg

      integer number_failed, ISFIRST, NOTFIRST, MAXLEN_LAB
      integer MAXLEN_DESC, ROWS, COLS, REPS
      integer DFTAG_SDG, DFTAG_RIG

      parameter ( ISFIRST =        1, 
     *            NOTFIRST =       0, 
     *            MAXLEN_LAB =    30,
     *            MAXLEN_DESC =  500, 
     *            DFTAG_SDG   =  700,
     *            DFTAG_RIG   =  306,
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
      call RESULT(ret, 'dssdims')

C  ***  Write labels and descriptions ***

      print *,'***  Writing labels & descriptions with SDS and RIS ***'

      do 100 j=1,REPS
C         write out scientific data set 
          ret = dsadata(TESTFILE, rank,dimsizes, data)
          call RESULT(ret, 'dsadata')

C         write out annotations for 2 out of every 3 
          if (mod(j,3) .ne. 0) then 
              refnum = dslref()
              ret = daplab(TESTFILE, DFTAG_SDG, refnum, labsds)
              call RESULT(ret, 'daplab')
              ret = dapdesc(TESTFILE, DFTAG_SDG, refnum, 
     *                                     descsds, len(descsds))
              call RESULT(ret, 'dapdesc')
          endif

          ret = d8aimg(TESTFILE, image, COLS, ROWS, NULL)
          call RESULT(ret, 'd8aimg')
          refnum = DFR8lastref()
          ret = daplab(TESTFILE, DFTAG_RIG, refnum, labris)
          call RESULT(ret, 'daplab')
          ret = dapdesc(TESTFILE,DFTAG_RIG,refnum, descris, 
     *                                                 len(descris))
          call RESULT(ret, 'dapdesc')
  100 continue


C********  Read labels and descriptions *********

      print *, CR, CR
      print *,'*** Reading labels and descriptions for SDS and RIS ***'

      do 200 j=1,REPS

          ret = dsgdims(TESTFILE, rank,dimsizes,3)
          call RESULT(ret, 'dsgdims')
          refnum = dslref()

C         read in annotations for 2 out of every 3 
          if (mod(j,3) .ne. 0) then
              call check_lab_desc(TESTFILE, DFTAG_SDG, refnum, 
     *                                  labsds, descsds, number_failed)
          endif

          ret = d8gimg(TESTFILE, newimage, COLS, ROWS, pal)
          call RESULT(ret, 'd8gimg')
          refnum = DFR8lastref()
          call check_lab_desc(TESTFILE, DFTAG_RIG, refnum, 
     *                                labris, descris, number_failed)
      
  200 continue

      if ( number_failed .eq. 0 ) then
          print *, CR, CR
          print *,'***** ALL TESTS SUCCESSFUL ***** '
      else
          print *, CR, CR
          print *,'***** ',number_failed,' TESTS FAILED ***** '
      endif

      stop
      end


C***************************************************************
C
C  gen2Dfloat:  generate 2-D data array 
C
C***************************************************************
      subroutine gen2Dfloat(height, width, data)
      integer   height, width
      real data(height,width)

      integer i, j

C     store one value per row, increasing by one for each row 
      do 110 i=1, height
          do 100 j=1, width
             data(height, width) = float(i)
  100     continue
  110 continue
      return
      end


C***************************************************************
C
C  genimage:  generate image from 2-D float array
C
C***************************************************************
      subroutine genimage(height, width, data, image)
      integer   height, width
      real      data(height, width)
      character image(height, width)

      integer i
      real   max, min, multiplier

      max = data(1,1)
      min = data(1,1)
      do 110 i=1, height
          do 100 j=1, width
             if (max .gt. data(i,j)) max = data(i,j)
             if (min .lt. data(i,j)) min = data(i,j)
  100     continue
  110 continue

C     store one value per row, increasing by one for each row 
      multiplier = 255.0 /(max-min)
      do 210 i=1, height
          do 200 j=1, width
             image(i,j) = char( int((data(i,j)-min) * multiplier) )
  200     continue
  210 continue
      return 
      end


C**************************************************************
C
C  check_lab_desc:  read and compare label and description
C                   with expected ones
C
C**************************************************************
      subroutine check_lab_desc(filename, tag, ref, label, desc, 
     *                                                   num_failed)
      character*(*) filename, label, desc
      integer tag, ref, num_failed

      parameter ( MAXLEN_LAB =    30,
     *            MAXLEN_DESC =  500 )

      integer daglab, dagllen, dagdlen, dagdesc
      integer  inlablen, indesclen, ret
      character*30   inlabel
      character*500 indesc

      inlablen =  dagllen(filename, tag, ref)
      call RESULT(inlablen, 'dagllen')

      if (inlablen .ne. len(label)) then
          print *,'   >>>BAD LABEL LENGTH.'
          print *,'                        IS: ', inlablen
          print *,'                 SHOULD BE: ', len(label)
          num_failed = num_failed + 1
      endif

      ret = daglab(filename, tag, ref, inlabel, MAXLEN_LAB+1)
      call RESULT(ret, 'daglab')
      if (inlabel .ne. label) then
          print *,'   >>>BAD LABEL.'
          print *,'                        IS: ', inlabel
          print *,'                 SHOULD BE: ', label
          num_failed = num_failed + 1
      endif

      indesclen = dagdlen(filename, tag, ref)
      call RESULT(indesclen, 'dagdlen')
      if (indesclen .ne. len(desc)) then
          print *,'   >>>BAD DESCRIPTION LENGTH.' 
          print *,'                        IS: ', indesclen 
          print *,'                 SHOULD BE: ', len(desc) 
          num_failed = num_failed + 1 
      else 
          ret = dagdesc(filename, tag, ref, indesc, MAXLEN_DESC)
          call RESULT(ret, 'dagdesc')
          if (indesc .ne. desc) then
              print *,'   >>>BAD DESCRIPTION.' 
              print *,'                        IS: ', indesc 
              print *,'                 SHOULD BE: ', desc 
              num_failed = num_failed + 1 
          endif
      endif

      return
      end

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE RESULT
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        subroutine RESULT(errval, routine)
        integer errval
        character*(*)  routine

        integer FAIL

        FAIL = -1

        if (errval.eq.FAIL) then
            num_fail = num_fail + 1
            print *,'   >>> ', routine, ' FAILED: ret = ', 
     *                                                  errval, ' <<<'
        else
            print *, routine, ' SUCCESSFUL'
        endif

        return
        end


