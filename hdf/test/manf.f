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
      program manf
C
C
C  Test program: stores annotations in a file.
C                Writes several SDSs and corresponding RISs to a file.
C                Writes labels and descriptions for all 2 out of 3 SDS
C                Writes labels and descriptions for all RISs.
C
C  Input file:   none
C  Output files: manf.hdf
C

      integer afstart, afend, afcreate, affcreate
      integer afwriteann, afendaccess

      integer dssdims, dsadata, dslref, dsgdims
      integer d8aimg, DFR8lastref, d8gimg

      integer numberfailed, ISFIRST, NOTFIRST, MAXLENLAB
      integer MAXLEN_DESC, ROWS, COLS, REPS
      integer DFTAG_SDG, DFTAG_RIG

      parameter ( ISFIRST =        1, 
     *            NOTFIRST =       0, 
     *            MAXLENLAB =    30,
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
      integer fhandle, anhandle
      integer  DFACC_CREATE, DFACC_READ
      integer AN_DATA_LABEL, AN_DATA_DESC, AN_FILE_LABEL, AN_FILE_DESC

      character*30 labsds, labsds2 
      character*30 labris, labris2
      character*500 descsds, descsds2
      character*500 descris, descris2
      character*35 lab1, lab2
      character*100 desc1, desc2
      character pal(768)
      character*64 TESTFILE

      character*1 CR
      character image(ROWS, COLS), newimage(ROWS, COLS)
      real      data(ROWS, COLS)

      numberfailed =  0
      CR = char(10)
      DFACC_CREATE = 4
      DFACC_READ   = 1
      AN_DATA_LABEL = 0
      AN_DATA_DESC  = 1
      AN_FILE_LABEL = 2
      AN_FILE_DESC  = 3
      TESTFILE = 'manf.hdf'

C *** set up file labels and descriptions ***
      lab1 = 'File label #1: aaa'
      lab2 = 'File label #2: bbbbbb'
      desc1 = 'File descr #1: This is a test file annotation'
      desc2 = 'File descr #2: One more test ...'

C *** set up object labels and descriptions ***

      labsds  = 'Object label #1: sds'
      labsds2 = 'Object label #1: sds2'
      labris  = 'Object label #2: image'
      labris2 = 'Object label #2: image2'
      descsds = 'Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 ' 
     *          // CR // '                13 14 15 16 17 18 19 20 '
     *          // ' **END SDS DESCR**'
      descsds2 = 'Object Descr #1: 1  2  3  4  5  6  7  8  9 10 11 12 ' 
     *          // CR // '                13 14 15 16 17 18 19 20 '
     *          // ' **END SDS2 DESCR**'
      descris = 'Object Descr #2: A B C D E F G H I J K L '
     *          // CR // '                M N O **END IMAGE DESCR **'
      descris2 = 'Object Descr #2: A B C D E F G H I J K L '
     *          // CR // '                M N O **END IMAGE2 DESCR **'

C  *** generate float array and image ***
 
      rank = 2
      dimsizes(1)=ROWS 
      dimsizes(2)=COLS

      call gen2Dfloat(ROWS, COLS, data)
      call genimage(ROWS, COLS, data, image)

      ret = dssdims(rank,dimsizes)
      call RESULT(ret, 'dssdims')

C  *** start annotation on file ***
      fhandle = afstart(TESTFILE,DFACC_CREATE)
      ret = fhandle
      call RESULT(ret, 'afstart')

C  *** write file 2 labels/ 2 descriptions ***
      anhandle = affcreate(fhandle, AN_FILE_LABEL)
      ret = anhandle
      call RESULT(ret, 'afcreatef')
      ret = afwriteann(anhandle,lab2,len(lab2))
      call RESULT(ret, 'afwriteann')
      ret = afendaccess(anhandle)
      call RESULT(ret, 'afendaccess')

      anhandle = affcreate(fhandle, AN_FILE_LABEL)
      ret = anhandle
      call RESULT(ret, 'afcreatef')
      ret = afwriteann(anhandle,lab1,len(lab1))
      call RESULT(ret, 'afwriteann')
      ret = afendaccess(anhandle)
      call RESULT(ret, 'afendaccess')

      anhandle = affcreate(fhandle, AN_FILE_DESC)
      ret = anhandle
      call RESULT(ret, 'afcreatef')
      ret = afwriteann(anhandle,desc2,len(desc2))
      call RESULT(ret, 'afwriteann')
      ret = afendaccess(anhandle)
      call RESULT(ret, 'afendaccess')

      anhandle = affcreate(fhandle, AN_FILE_DESC)
      ret = anhandle
      call RESULT(ret, 'afcreatef')
      ret = afwriteann(anhandle,desc1,len(desc1))
      call RESULT(ret, 'afwriteann')
      ret = afendaccess(anhandle)
      call RESULT(ret, 'afendaccess')

C  ***  Write data labels and descriptions ***

      print *,'***  Writing labels & descriptions with SDS and RIS ***'

      do 100 j=1,REPS
C  ***  write out scientific data set 
         ret = dsadata(TESTFILE, rank,dimsizes, data)
         call RESULT(ret, 'dsadata')

C ****    write out annotations for 2 out of every 3 
         if (mod(j,3) .ne. 0) then 
            refnum = dslref()

C ********** Write out 2 labels for each SDS *****************
            anhandle = afcreate(fhandle,DFTAG_SDG,refnum,AN_DATA_LABEL)
            ret = anhandle
            call RESULT(ret, 'afcreate')
            ret = afwriteann(anhandle,labsds2,len(labsds2))
            call RESULT(ret, 'afwriteann')
            ret = afendaccess(anhandle)
            call RESULT(ret, 'afendaccess')

            anhandle = afcreate(fhandle,DFTAG_SDG,refnum,AN_DATA_LABEL)
            ret = anhandle
            call RESULT(ret, 'afcreate')
            ret = afwriteann(anhandle,labsds,len(labsds))
            call RESULT(ret, 'afwriteann')
            ret = afendaccess(anhandle)
            call RESULT(ret, 'afendaccess')

C *********** Write out 2 descritptions for each SDS ***********
            anhandle = afcreate(fhandle,DFTAG_SDG,refnum,AN_DATA_DESC)
            ret = anhandle
            call RESULT(ret, 'afcreate')
            ret = afwriteann(anhandle,descsds2,len(descsds2))
            call RESULT(ret, 'afwriteann')
            ret = afendaccess(anhandle)
            call RESULT(ret, 'afendaccess')

            anhandle = afcreate(fhandle,DFTAG_SDG,refnum,AN_DATA_DESC)
            ret = anhandle
            call RESULT(ret, 'afcreate')
            ret = afwriteann(anhandle,descsds,len(descsds))
            call RESULT(ret, 'afwriteann')
            ret = afendaccess(anhandle)
            call RESULT(ret, 'afendaccess')

         endif

         ret = d8aimg(TESTFILE, image, COLS, ROWS, NULL)
         call RESULT(ret, 'd8aimg')
         refnum = DFR8lastref()

C ********** Write out 2 labels for each Image *****************
          anhandle = afcreate(fhandle, DFTAG_RIG, refnum, AN_DATA_LABEL)
          ret = anhandle
          call RESULT(ret, 'afcreate')
          ret = afwriteann(anhandle,labris2,len(labris2))
          call RESULT(ret, 'afwriteann')
          ret = afendaccess(anhandle)
          call RESULT(ret, 'afendaccess')

          anhandle = afcreate(fhandle, DFTAG_RIG, refnum, AN_DATA_LABEL)
          ret = anhandle
          call RESULT(ret, 'afcreate')
          ret = afwriteann(anhandle,labris,len(labris))
          call RESULT(ret, 'afwriteann')
          ret = afendaccess(anhandle)
          call RESULT(ret, 'afendaccess')

C *********** Write out 2 descritptions for each Image ***********
          anhandle = afcreate(fhandle, DFTAG_RIG, refnum, AN_DATA_DESC)
          ret = anhandle
          call RESULT(ret, 'afcreate')
          ret = afwriteann(anhandle,descris2,len(descris2))
          call RESULT(ret, 'afwriteann')
          ret = afendaccess(anhandle)
          call RESULT(ret, 'afendaccess')

          anhandle = afcreate(fhandle, DFTAG_RIG, refnum, AN_DATA_DESC)
          ret = anhandle
          call RESULT(ret, 'afcreate')
          ret = afwriteann(anhandle,descris,len(descris))
          call RESULT(ret, 'afwriteann')
          ret = afendaccess(anhandle)
          call RESULT(ret, 'afendaccess')

  100 continue

C ******* End writing annotatons **********
      ret = afend(fhandle)
      call RESULT(ret, 'afend')

C********  Read data labels and descriptions *********
      print *, CR, CR
      print *,'*** Reading labels and descriptions for SDS and RIS ***'

      do 200 j=1,REPS
          ret = dsgdims(TESTFILE, rank,dimsizes,3)
          call RESULT(ret, 'dsgdims')
          refnum = dslref()

C ******  read in annotations for 2 out of every 3 
          if (mod(j,3) .ne. 0) then
              call check_lab_desc(TESTFILE, DFTAG_SDG, refnum, 
     *                            labsds, descsds, numberfailed)

              call check_lab_desc(TESTFILE, DFTAG_SDG, refnum, 
     *                            labsds2, descsds2, numberfailed)
          endif
C ****    read annotations for images
          ret = d8gimg(TESTFILE, newimage, COLS, ROWS, pal)
          call RESULT(ret, 'd8gimg')
          refnum = DFR8lastref()
          call check_lab_desc(TESTFILE, DFTAG_RIG, refnum, 
     *                        labris, descris, numberfailed)

          call check_lab_desc(TESTFILE, DFTAG_RIG, refnum, 
     *                        labris2, descris2, numberfailed)
      
  200 continue

C ****** Check file labels/descriptions *******
      print *, CR, CR
      print *,'*** Reading file labels and descriptions ***'

      call check_fan(TESTFILE, 0, lab1, desc1, numberfailed)
      call check_fan(TESTFILE, 1, lab2, desc2, numberfailed)

      if ( numberfailed .eq. 0 ) then
          print *, CR, CR
          print *,'***** ALL TESTS SUCCESSFUL ***** '
      else
          print *, CR, CR
          print *,'***** ',numberfailed,' TESTS FAILED ***** '
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
      subroutine check_lab_desc(fname, tag, ref, label, desc, 
     *                          num_failed)
      character*(*) fname, label, desc
      integer tag, ref, num_failed

      parameter ( MAXLENLAB =    30,
     *            MAXLEN_DESC =  500 )

      integer  inlablen, indesclen, ret

      integer affileinfo, afnumann, afannlist, afannlen, afreadann
      integer afstart, afend

      integer fileh
      integer nflabs, nfdescs, nolabs, nodescs
      integer numdlabels, numddescs
      integer annlen, j, found, fannlen
      integer dlabels(2), ddescs(2)
      character*30   inlabel, fannlabel
      character*500 indesc, fanndesc
      integer AN_DATA_LABEL, AN_DATA_DESC, AN_FILE_LABEL, AN_FILE_DESC
      integer DFACC_READ
      DFACC_READ = 1
      AN_DATA_LABEL = 0
      AN_DATA_DESC  = 1
      AN_FILE_LABEL = 2
      AN_FILE_DESC  = 3

C *****start annotation access on file *****
      fileh = afstart(fname, DFACC_READ)
      ret = fileh
      call RESULT(ret, 'afstart')

      ret = affileinfo(fileh,nflabs,nfdescs,nolabs,nodescs)
      call RESULT(ret, 'affileinfo')

      numdlabels = afnumann(fileh, AN_DATA_LABEL, tag, ref)
      call RESULT(numdlabels, 'afnumann')

      numddescs = afnumann(fileh, AN_DATA_DESC, tag, ref)
      call RESULT(numddescs, 'afnumann')

      ret = afannlist(fileh, AN_DATA_LABEL, tag, ref, dlabels)
      call RESULT(ret, 'afannlist')

      ret = afannlist(fileh, AN_DATA_DESC, tag, ref, ddescs)
      call RESULT(ret, 'afannlist')

C ***** Look for label in list ******
      found = 0
      fannlen = 0
      fannlabel = ' '
      do 300 j=1, numdlabels
         annlen = afannlen(dlabels(j))
         call RESULT(annlen, 'afannlen')

         ret = afreadann(dlabels(j), inlabel, MAXLENLAB+1)
         call RESULT(ret, 'afreadann')

         if (inlabel .eq. label) then
            found = 1
            inlablen = annlen
            fannlabel = inlabel
         endif

  300 continue

C ****** Check if we found label in list *****
      if (inlablen .ne. len(label)) then
         print *,'   >>>BAD LABEL LENGTH.'
         print *,'                        IS: ', inlablen
         print *,'                 SHOULD BE: ', len(label)
         num_failed = num_failed + 1
      endif

      if (fannlabel .ne. label) then
         print *,'   >>>BAD LABEL.'
         print *,'                        IS: ', fannlabel
         print *,'                 SHOULD BE: ', label
         num_failed = num_failed + 1
      endif

C ***** look for description in list 
      found = 0
      fannlen = 0
      fanndesc = ' '
      do 400 j=1, numddescs
         annlen = afannlen(ddescs(j))
         call RESULT(annlen, 'afannlen')

         ret = afreadann(ddescs(j), indesc, MAXLEN_DESC+1)
         call RESULT(ret, 'afreadann')

         if (indesc .eq. desc) then
            found = 1
            indesclen = annlen
            fanndesc = indesc
         endif

  400 continue

      if (indesclen .ne. len(desc)) then
          print *,'   >>>BAD DESCRIPTION LENGTH.' 
          print *,'                        IS: ', indesclen 
          print *,'                 SHOULD BE: ', len(desc) 
          num_failed = num_failed + 1 
       endif

       if (fanndesc .ne. desc) then
          print *,'   >>>BAD DESCRIPTION.' 
          print *,'                        IS: ', fanndesc 
          print *,'                 SHOULD BE: ', desc 
          num_failed = num_failed + 1 
       endif

C ****** close file *******
      ret = afend(fileh)
      call RESULT(ret, 'afend')

      return
      end


C************************************************************
C
C     SUBROUTINE check_fan
C
C************************************************************
      subroutine check_fan(fname, index, label, desc, num_failed)
      character*(*) fname, label, desc
      integer index, num_failed

      parameter ( MAXLENFLAB =    35,
     *            MAXLEN_FDESC =  100 )

      integer affileinfo, afselect, afannlen, afreadann
      integer afstart, afend

      integer ret
      integer fileh, annh
      integer nflabs, nfdescs, nolabs, nodescs
      integer fannlen
      character*35 flabel
      character*100 fdesc
      integer AN_DATA_LABEL, AN_DATA_DESC, AN_FILE_LABEL, AN_FILE_DESC
      integer DFACC_READ
      DFACC_READ = 1
      AN_DATA_LABEL = 0
      AN_DATA_DESC  = 1
      AN_FILE_LABEL = 2
      AN_FILE_DESC  = 3

C **** We check both file label/description

      fileh = afstart(fname, DFACC_READ)
      ret = fileh
      call RESULT(ret, 'afstart')

      ret = affileinfo(fileh,nflabs,nfdescs,nolabs,nodescs)
      call RESULT(ret, 'affileinfo')

C ***** Read file label **********
      annh = afselect(fileh, index, AN_FILE_LABEL)
      call RESULT(ret, 'afselect')

      fannlen = afannlen(annh)
      call RESULT(fannlen, 'afannlen')

      ret = afreadann(annh, flabel, fannlen+1)
      call RESULT(ret, 'afreadann')

      if (fannlen .ne. len(label)) then
         print *,'   >>>BAD LABEL LENGTH.'
         print *,'                        IS: ', fannlen
         print *,'                 SHOULD BE: ', len(label)
         num_failed = num_failed + 1
      endif

      if (flabel .ne. label) then
         print *,'   >>>BAD LABEL.'
         print *,'                        IS: ', flabel
         print *,'                 SHOULD BE: ', label
         num_failed = num_failed + 1
      endif

C **** Read file description *****
      annh = afselect(fileh, index, AN_FILE_DESC)
      call RESULT(ret, 'afselect')

      fannlen = afannlen(annh)
      call RESULT(fannlen, 'afannlen')

      ret = afreadann(annh, fdesc, fannlen+1)
      call RESULT(ret, 'afreadann')

      if (fannlen .ne. len(desc)) then
          print *,'   >>>BAD DESCRIPTION LENGTH.' 
          print *,'                        IS: ', fannlen 
          print *,'                 SHOULD BE: ', len(desc) 
          num_failed = num_failed + 1 
       endif

       if (fdesc .ne. desc) then
          print *,'   >>>BAD DESCRIPTION.' 
          print *,'                        IS: ', fdesc 
          print *,'                 SHOULD BE: ', desc 
          num_failed = num_failed + 1 
       endif

C ****** close file *******
      ret = afend(fileh)
      call RESULT(ret, 'afend')

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


