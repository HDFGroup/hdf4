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
      subroutine mgrf (num_err)

C
C Test Program: 
C   Tests the multi-file GR interface.
C Input file: none
C Output file: tmgrf.hdf
C
C
      integer num_err
      integer getverb
      integer Verbosity

      integer hopen, hclose
      integer mgstart, mgfinfo, mgend, mgcreat, mgselct 
      integer mgn2ndx, mggiinf, mgwrimg, mgrdimg, mgendac
      integer mgid2rf, mgr2idx, mgrltil, mgrimil, mggltid
      integer mgglinf, mgwrlut, mgrdlut, mgsxfil, mgssctp
      integer mgsattr, mgatinf, mggattr, mgfndat

      integer	MFGR_INTERLACE_PIXEL, MFGR_INTERLACE_LINE,
     *      MFGR_INTERLACE_COMPONENT

      parameter(MFGR_INTERLACE_PIXEL	= 0,
     *		MFGR_INTERLACE_LINE	= 1,
     * 		MFGR_INTERLACE_COMPONENT	= 2)

      integer 	DFACC_READ, DFACC_WRITE, DFACC_CREATE, DFACC_ALL  
      integer	DFACC_RDONLY, DFACC_RDWR, DFACC_CLOBBER

      parameter(DFACC_READ       	 = 1,
     *		DFACC_WRITE      	 = 2,
     *		DFACC_CREATE     	 = 4,
     *		DFACC_ALL        	 = 7,
     *		DFACC_RDONLY		 = 1,
     *		DFACC_RDWR		 = 3,
     *		DFACC_CLOBBER		 = 4)

      integer 	DFNT_INT8,  DFNT_UINT8, 
     *		DFNT_INT16, DFNT_UINT16,
     *		DFNT_INT32, DFNT_UINT32, 
     *      DFNT_INT64, DFNT_UINT64,
     *      DFNT_INT128,DFNT_UINT128
 
      parameter(DFNT_INT8	= 20,
     *		DFNT_UINT8	= 21,
     *		DFNT_INT16	= 22,
     *		DFNT_UINT16	= 23,
     *		DFNT_INT32	= 24,
     *		DFNT_UINT32	= 25,
     *		DFNT_INT64	= 26,
     *		DFNT_UINT64	= 27,
     *		DFNT_INT128	= 28,
     *		DFNT_UINT128	= 29)
      integer il
      character*80 TESTFILE
      character*80 IMAGE1, IMAGE2
      character*80 ATNAME1, ATNAME2
      character*1 CR
      character buf(3, 2, 2), buf1(2, 3, 2), buf2(2, 2, 3)
      character in(3,2,2), in1(2, 3, 2), in2(2, 2, 3)
      character pal(3,256), in_pal(3,256), in_pal2(256,3)
      integer*4 file_id, gr_id, ri_id, pal_id, index, index2
      integer*4 n_datasets, n_attrs, ref
      integer*4 n_comp, nt
      integer*4 dims(2), start(2), stride(2)
      integer*4 attr(5), attr2(5)
      integer i, j, ret, number_failed

      num_err = 0
      TESTFILE = 'tmgrf.hdf'
      IMAGE1 = 'Image #1'
      ATNAME1 = 'Attr. #1'
      CR = char(10)
      Verbosity=getverb()
      number_failed = 0

C Initialize the arrays
C Initialize the image arrays
      do 150 i=1, 2
          do 2 j=1, 2
              buf(1, j, i) = char(i+j)
              buf(2, j, i) = char(i+j)
              buf(3, j, i) = char(i+j)
              buf1(j, 1, i) = char(i-j)
              buf1(j, 2, i) = char(i-j)
              buf1(j, 3, i) = char(i-j)
              buf2(j, i, 1) = char(2*i - j)
              buf2(j, i, 2) = char(2*i - j)
              buf2(j, i, 3) = char(2*i - j)
2       continue
150   continue
C Initialize the palette array
      do 160 i=1, 256
          do 3 j=1, 3
              pal(j,i)=char(i+j)
3       continue
160   continue
C Initialize the attribute
      do 170 i=1, 5
          attr(i)=i*21
170   continue

C Open the file
      file_id=hopen(TESTFILE, DFACC_ALL, 0)
      call VERIFY(file_id,'hopen',number_failed,Verbosity)
      gr_id=mgstart(file_id)
      call VERIFY(gr_id,'mgstart',number_failed,Verbosity)

C Create an image
      call MESSAGE(5,'Creating an image',Verbosity)
      dims(1)=2
      dims(2)=2
      ri_id = mgcreat(gr_id,IMAGE1,3,DFNT_UINT8,MFGR_INTERLACE_PIXEL,
     *          dims)
      call VERIFY(ri_id,'mgcreat',number_failed,Verbosity)

      start(1)=0
      start(2)=0
      stride(1)=1
      stride(2)=1
      call MESSAGE(5,'Writing image data',Verbosity)
      ret = mgwrimg(ri_id,start,stride,dims,buf)
      call VERIFY(ret,'mgwrimg',number_failed,Verbosity)

C Store a palette with the image
      call MESSAGE(5,'Writing palette data',Verbosity)
      pal_id = mggltid(ri_id, 0)
      call VERIFY(pal_id,'mggltid',number_failed,Verbosity)
      ret = mgwrlut(pal_id,3,DFNT_UINT8,MFGR_INTERLACE_PIXEL,256,pal)
      call VERIFY(ret,'mgwrlut',number_failed,Verbosity)

C Store an attribute with the image
      call MESSAGE(5,'Writing attribute data',Verbosity)
      ret = mgsattr(ri_id,ATNAME1,DFNT_INT32,5,attr)
      call VERIFY(ret,'mgsattr',number_failed,Verbosity)

C End access to the image
      ret = mgendac(ri_id)
      call VERIFY(ret,'mgendac',number_failed,Verbosity)

C End access to the GR interface
      ret = mgend(gr_id)
      call VERIFY(ret,'mgend',number_failed,Verbosity)

C Close the file
      ret = hclose(file_id)
      call VERIFY(ret,'hclose',number_failed,Verbosity)

C Re-open the file
      file_id=hopen(TESTFILE, DFACC_ALL, 0)
      call VERIFY(file_id,'hopen',number_failed,Verbosity)
      gr_id=mgstart(file_id)
      call VERIFY(gr_id,'mgstart',number_failed,Verbosity)

C Get info about the file
      call MESSAGE(5,'Getting GR file information',Verbosity)
      ret = mgfinfo(gr_id,n_datasets,n_attrs)
      call VERIFY(ret,'mgfinfo',number_failed,Verbosity)

C Select an image
      call MESSAGE(5,'Selecting an image',Verbosity)
      index = mgn2ndx(gr_id, IMAGE1)
      call VERIFY(index,'mgn2ndx',number_failed,Verbosity)
      ri_id = mgselct(gr_id,index)
      call VERIFY(ri_id,'mgselct',number_failed,Verbosity)

C Get info about the image
      call MESSAGE(5,'Getting image information',Verbosity)
      ret = mggiinf(ri_id,IMAGE2,n_comp,nt,il,dims,n_attr)
      call VERIFY(ret,'mggiinf',number_failed,Verbosity)
      ref = mgid2rf(ri_id)
      call VERIFY(ref,'mgid2rf',number_failed,Verbosity)
      index2 = mgr2idx(gr_id,ref)
      call VERIFY(index2,'mgr2idx',number_failed,Verbosity)

C Check image reading
      start(1)=0
      start(2)=0
      stride(1)=1
      stride(2)=1
      call MESSAGE(5,'Reading image data',Verbosity)
      ret = mgrdimg(ri_id,start,stride,dims,in)
      call VERIFY(ret,'mgrdimg',number_failed,Verbosity)
      ret = mgrimil(ri_id,MFGR_INTERLACE_LINE)
      call VERIFY(ret,'mgrimil',number_failed,Verbosity)
      ret = mgrdimg(ri_id,start,stride,dims,in1)
      call VERIFY(ret,'mgrdimg',number_failed,Verbosity)
      ret = mgrimil(ri_id,MFGR_INTERLACE_COMPONENT)
      call VERIFY(ret,'mgrimil',number_failed,Verbosity)
      ret = mgrdimg(ri_id,start,stride,dims,in2)
      call VERIFY(ret,'mgrdimg',number_failed,Verbosity)

C Check palette reading
      pal_id = mggltid(ri_id, 0)
      call VERIFY(pal_id,'mggltid',number_failed,Verbosity)
      call MESSAGE(5,'Reading palette data',Verbosity)
      ret = mgglinf(pal_id,n_comp,nt,il,i)
      call VERIFY(ret,'mgglinf',number_failed,Verbosity)
      ret = mgrdlut(pal_id,in_pal)
      call VERIFY(ret,'mgrdlut',number_failed,Verbosity)
      ret = mgrltil(pal_id,MFGR_INTERLACE_COMPONENT)
      call VERIFY(ret,'mgrltil',number_failed,Verbosity)
      ret = mgrdlut(pal_id,in_pal2)
      call VERIFY(ret,'mgrdlut',number_failed,Verbosity)

C Check attribute reading
      index = mgfndat(ri_id,ATNAME1)
      call VERIFY(index,'mgfndat',number_failed,Verbosity)
      call MESSAGE(5,'Reading attribute data',Verbosity)
      ret = mgatinf(ri_id,index,ATNAME2,nt,i)
      call VERIFY(ret,'mgatinf',number_failed,Verbosity)
      ret = mggattr(ri_id,index,attr2)
      call VERIFY(ret,'mggattr',number_failed,Verbosity)

C End access to the image
      ret = mgendac(ri_id)
      call VERIFY(ret,'mgendac',number_failed,Verbosity)

C End access to the GR interface
      ret = mgend(gr_id)
      call VERIFY(ret,'mgend',number_failed,Verbosity)

C Close the file
      ret = hclose(file_id)
      call VERIFY(ret,'hclose',number_failed,Verbosity)

      if (Verbosity .gt. 6) then
      print *, CR, CR
      if (number_failed .eq. 0) then 
          print *, '****** ALL TESTS SUCCESSFUL ******'
      else
          print *, '****** ', number_failed, ' TESTS FAILED  ******'
      endif
      endif

      return 
      end

