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
      subroutine tsdntf (number_failed)
      implicit none
C
C
C  Program to test writing SDSs with different types of data.
C
C  Input file:  none
C  Output files:  o.hdf.1, o.hdf.2, ... o.hdf.5
C
C  **** VMS users ****
C
C  VMS has a special way of handling the passsing of character
C   strings between C and FORTRAN.  For these tests to work 
C   correctly, you must change the definition of i8 and ti8
C   to be 'byte' not 'character'  You will also need to remove
C   a couple of calls to char().  If you search on the string 
C   VMS you should be able to find all of the necessary changes.
C
      integer number_failed
      character*(*) myname
      parameter (myname = 'sdnt')

      integer dspdata, dsgdata, dsadata, dssdims, dssnt

      real*8 f64(10,10), tf64(10,10)
      real*4 f32(10,10), tf32(10,10)
      integer*2 i16(10,10), ti16(10,10)
      integer*4 i32(10,10), ti32(10,10)

C  Change these to be of type 'byte' for VMS
C      byte      i8(10,10), ti8(10,10)
      character i8(10,10), ti8(10,10)

      
      integer i, j, err, err1, err2
      integer rank
      integer dims(2)
      integer DFNT_FLOAT64, DFNT_FLOAT32, DFNT_INT8
      integer DFNT_INT16, DFNT_INT32
  
      call ptestban('Testing', myname)
      number_failed = 0
      DFNT_FLOAT64 = 6
      DFNT_FLOAT32 = 5
      DFNT_INT8 = 20
      DFNT_INT16 = 22
      DFNT_INT32 = 24
      rank = 2
      dims(1) = 10
      dims(2) = 10
  
      print *, 'Creating arrays...'
  
      do 110 i=1,10
          do 100 j=1,10
            f64(i,j) = (i * 10) + j
  	    f32(i,j) = (i * 10) + j
C  Use the following line for VMS
C            i8(i,j) =  (i * 10) + j
  	     i8(i,j) = char( (i * 10) + j )
  	    i16(i,j) = (i * 10) + j
  	    i32(i,j) = (i * 10) + j
  100     continue
  110 continue
  
      err = dssdims(rank, dims)
  
C  individual files 
      print *,'Testing arrays in individual files...'
  
      err = dssnt(DFNT_FLOAT64)
      err1 = dspdata('o1.hdf', rank, dims, f64)
      err2 = dsgdata('o1.hdf', rank, dims, tf64)
      print *,'Write: ', err1, '    Read: ', err2
      err = 0
      do 160 i=1,10
          do 150 j=1,10
  	    if (f64(i,j).ne.tf64(i,j)) err = 1
  	    tf64(i,j) = 0.0
  150     continue
  160 continue

      call err_check(err, number_failed, 'float64')

      err = dssnt(DFNT_FLOAT32)
      err1 = dspdata('o2.hdf', rank, dims, f32)
      err2 = dsgdata('o2.hdf', rank, dims, tf32)
      print *,'Write: ', err1, '    Read: ', err2
      err = 0
      do 210 i=1,10
          do 200 j=1,10
  	    if (f32(i,j).ne.tf32(i,j)) err = 1
  	    tf32(i,j) = 0.0
  200     continue
  210 continue

      call err_check(err, number_failed, 'float32')

      err = dssnt(DFNT_INT8)
      err1 = dspdata('o3.hdf', rank, dims, i8)
      err2 = dsgdata('o3.hdf', rank, dims, ti8)
      print *,'Write: ', err1, '    Read: ', err2
      err = 0
      do 310 i=1,10
          do 300 j=1,10
  	    if (i8(i,j).ne.ti8(i,j)) err = 1
C Use the following line for VMS
C           ti8(i,j) = 0
  	    ti8(i,j) = char(0)
  300     continue
  310 continue

      call err_check(err, number_failed, 'int8')

      err = dssnt(DFNT_INT16)
      err1 = dspdata('o4.hdf', rank, dims, i16)
      err2 = dsgdata('o4.hdf', rank, dims, ti16)
      print *,'Write: ', err1, '    Read: ', err2
      err = 0
      do 410 i=1,10
          do 400 j=1,10
   	    if (i16(i,j).ne.ti16(i,j)) err = 1
  	    ti16(i,j) = 0
  400     continue
  410 continue

      call err_check(err, number_failed, 'int16')

      err = dssnt(DFNT_INT32)
      err1 = dspdata('o5.hdf', rank, dims, i32)
      err2 = dsgdata('o5.hdf', rank, dims, ti32)
      print *,'Write: ', err1, '    Read: ', err2
      err = 0
      do 510 i=1,10
          do 500 j=1,10
  	    if (i32(i,j).ne.ti32(i,j)) err = 1
  	    ti32(i,j) = 0
  500     continue
  510 continue

      call err_check(err, number_failed, 'int32')


C 
      print *, 'Writing arrays to single file.'
      print *, 'Error values: '
C
      err = dssnt(DFNT_FLOAT64)
      print *,'Add float64 ret: ',dsadata('ntf.hdf',rank,dims,f64)

      err = dssnt(DFNT_FLOAT32)
      print *,'Add float32 ret: ',dsadata('ntf.hdf',rank,dims,f32)

      err = dssnt(DFNT_INT8)
      print *, 'Add int8 ret: ', dsadata('ntf.hdf', rank, dims, i8)

      err = dssnt(DFNT_INT16)
      print *, 'Add int16 ret: ', dsadata('ntf.hdf', rank, dims, i16)

      err = dssnt(DFNT_INT32)
      print *, 'Add int32 ret: ', dsadata('ntf.hdf', rank, dims, i32)

C 
      print *, 'Reading arrays from single file... '
      print *, 'Error values: '
C
      print *, 'Get f64 ret: ', dsgdata('ntf.hdf', rank, dims, tf64)
      print *, 'Get f32 ret: ', dsgdata('ntf.hdf', rank, dims, tf32)
      print *, 'Get int8 ret: ', dsgdata('ntf.hdf', rank, dims, ti8)
      print *, 'Get int16 ret: ', dsgdata('ntf.hdf', rank, dims, ti16)
      print *, 'Get int32 ret: ', dsgdata('ntf.hdf', rank, dims, ti32)

C 
      print *, 'Checking arrays from single file...\n\n'

      err = 0
      do 910 i=1,10
         do 900 j=1,10
           if (f64(i,j) .ne. tf64(i,j)) err = 1
  900    continue
  910 continue

      call err_check(err, number_failed, 'float64')
C 

      err = 0
      do 1010 i=1,10
         do 1000 j=1,10
           if (f32(i,j) .ne. tf32(i,j)) err = 1
 1000    continue
 1010 continue

      call err_check(err, number_failed, 'float32')
C 
      err = 0
      do 1110 i=1,10
         do 1100 j=1,10
           if (i8(i,j) .ne. ti8(i,j)) err = 1
 1100    continue
 1110 continue

      call err_check(err, number_failed, 'int8')
C 
      err = 0
      do 1210 i=1,10
         do 1200 j=1,10
           if (i16(i,j) .ne. ti16(i,j)) err = 1
 1200    continue
 1210 continue

      call err_check(err, number_failed, 'int16')
C 
      err = 0
      do 1310 i=1,10
         do 1300 j=1,10
           if (i32(i,j) .ne. ti32(i,j)) err = 1
 1300    continue
 1310 continue

      call err_check(err, number_failed, 'int32')
C 
      print *
      if (number_failed .gt. 0 ) then
  	print *,'        >>> ', number_failed, ' TESTS FAILED <<<'
      else
  	print *,'        >>> ALL TESTS PASSED <<<'
      endif
      print *
      print *

      return
      end  
