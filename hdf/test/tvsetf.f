C**********************************************************************
C* NCSA HDF
C*
C* Software Development Group
C*
C* National Center for Supercomputing Applications
C*
C* University of Illinois at Urbana-Champaign
C*
C* 605 E. Springfield, Champaign IL 61820
C*
C*
C*
C* For conditions of distribution and use, see the accompanying
C*
C* hdf/COPYING file.
C*
C*
C*
C*********************************************************************
C      test vset Fortran data types
C
      subroutine tvsetf (number_failed)
C Test Program:
C   Tests the vdata and vgroup interface
C Input file: none
C Output file: tvsetf1.hdf

      implicit none
      include 'fortest.inc'

      integer number_failed
      character*20 myname
      parameter (myname = 'vsetf')

      integer hopen,  vfatch,  vfgid,   vfgnxt, vfinsrt
      integer vfgttrs,vfadtr,  hclose,  vsfatch
      integer vsffdef,vsffnd,  vsfgid
      integer vsfinq, vsfsfld
      integer vfisvs, vsfcpak, vsfnpak, vsfrd
      integer vsfrdc, vsfwrt,  vsfwrtc, vhfscd, vhfscdm

      integer*4 fid1, vgid1, vgid2, vsid1, vsid2
      integer*4 vgref1, vsref1, vsref2, vref
      integer vsize, found
      integer DFACC_CREATE, DFNT_CHAR
      integer DFNT_INT16, DFNT_INT32, DFNT_FLOAT32
      integer DFNT_FLOAT64, FULL_INTERLACE, DFACC_RDWR
      integer DFTAG_VH, HDF_VSPACK, HDF_VSUNPACK
      integer ret,ntrs,i,il,nelts
      integer*4 dbuf(320),idbuf(320),ddata4(10),iddata4(10)
      integer buf(1), ibuf(1)
      equivalence (buf(1), dbuf(1)), (ibuf(1), idbuf(1))
      integer*2 ddata1(10), iddata1(10)
      integer*2 ddata2(10),iddata2(10)
      integer bufd2(10),ibufd2(10)
      equivalence (bufd2(1), ddata2(1)), (ibufd2(1), iddata2(1))
      integer*4 tags(10), refs(10)
      real      fdata(10), ifdata(10)
      integer buff(10),ibuff(10)
      equivalence (buff(1), fdata(1)), (ibuff(1), ifdata(1))
      double precision gdata1(10)
      double precision igdata1(10)
      integer bufg1(10),ibufg1(10)
      equivalence (bufg1(1), gdata1(1)), (ibufg1(1), igdata1(1))
      double precision geps
      real   feps

      character*11 fn1
      character*64 vname
      character*9  fields1
      character*10 fields2
      character*31 fields3
      character*15 cdata, icdata
      parameter (DFACC_CREATE = 4,
     +           DFACC_RDWR = 3,
     +           DFNT_CHAR = 4,
     +           DFNT_FLOAT32 = 5,
     +           DFNT_FLOAT64 = 6,
     +           DFNT_INT16 = 22,
     +           DFNT_INT32 = 24,
     +           FULL_INTERLACE = 0,
     +           DFTAG_VH = 1962,
     +           HDF_VSPACK = 0,
     +           HDF_VSUNPACK = 1,
     +           feps = 1.0E-5,
     +           geps = 1.0D-9
     +          )

      DATA ddata1/10,11,12,13,14,15,16,17,18,19/
      DATA ddata2/20,21,22,23,24,25,26,27,28,29/
      DATA ddata4/40,41,42,43,44,45,46,47,48,49/
      DATA fdata/5.0,5.1,5.2,5.3,5.4,5.5,5.6,5.7,5.8,5.9/
      DATA gdata1/6.0,6.1,6.2,6.3,6.4,6.5,6.6,6.7,6.8,6.9/

      call ptestban('Testing', myname)
      cdata = 'abcdebcdefcdefg'
      fn1 = 'tvsetf1.hdf'

C Open the file
      fid1 = hopen(fn1, DFACC_CREATE, 0)
      call VERIFY(fid1,'hopen',number_failed)
      call vfstart(fid1)
C Create a vgroup
      call MESSAGE(5,'Creating a vgroup')
      vgid1 = vfatch(fid1, -1, 'w')
      call VERIFY(vgid1, 'vfatch', number_failed)
      call vfsnam(vgid1, 'Top Vgroup')
      call vfscls(vgid1, 'Test Object')

C Add a vgroup to it
      call MESSAGE(5,'Add a vgroup to the Top vgroup')
      vgid2 = vfatch(fid1, -1, 'w')
      call VERIFY(vgid2, 'vfatch', number_failed)
      ret = vfinsrt(vgid1, vgid2)
      call VERIFY(ret, 'vfinsrt', number_failed)

      call vfdtch(vgid1)
      call vfdtch(vgid2)

      call vfend(fid1)
      ret = hclose(fid1)
      call VERIFY(ret,'hclose',number_failed)

C      add a vdatas in vgroup1
      call MESSAGE(5,'Creating a char vdata')
      fid1 =  hopen(fn1, DFACC_RDWR, 0)
      call vfstart(fid1)
      vgref1 = vfgid(fid1, -1)
      call VERIFY(vgref1,'vfgid',number_failed)
      vgid1 = vfatch(fid1, vgref1, 'w')
      call VERIFY(vgid1,'vfatch',number_failed)
C      create a single field (char) vdata
      vsid1 = vsfatch(fid1, -1, 'w')
      call VERIFY(vsid1,'vsfatch',number_failed)
      ret = vsffdef(vsid1, 'char type', DFNT_CHAR, 5)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsfsfld(vsid1, 'char type')
      call VERIFY(ret,'vsfsfld',number_failed)
C      Use vsfwrtc to write the values
      ret = vsfwrtc(vsid1, cdata, 3, FULL_INTERLACE)
      call VERIFY(ret,'vsfwrtc',number_failed)
      call vsfdtch(vsid1)
C      create a single field int16 vdata
      call MESSAGE(5,'Creating an int16 vdata')
      vsid1 = vsfatch(fid1, -1, 'w')
      call VERIFY(vsid1,'vsfatch',number_failed)
      ret = vsffdef(vsid1, 'int16 type', DFNT_INT16, 2)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsfsfld(vsid1, 'int16 type')
      call VERIFY(ret,'vsfsfld',number_failed)
C      Use vsfwrt to write the values
      ret = vsfwrt(vsid1, ddata1, 5, FULL_INTERLACE)
      call VERIFY(ret,'vsfwrt',number_failed)
      call vsfdtch(vsid1)
C      use vhfscd and vhfscdm to create char vdata
      call MESSAGE(5,'Using vhfscd/vsfscdm to create vdatas')
      ret = vhfscd(fid1,'char1',cdata,5,DFNT_CHAR,'c1','c')
      call VERIFY(ret,'vhfscd',number_failed)
      ret = vhfscdm(fid1,'char3',cdata,5,DFNT_CHAR,'c3','s',3)
      call VERIFY(ret,'vhfscdm',number_failed)

C      create a five-field vdata, 2*int32,2*double,2*float32,
C             2*int16 and 3*char type 
      call MESSAGE(5,'Creating a five_field vdata')
      vsid2 = vsfatch(fid1, -1, 'w')
      call VERIFY(vsid2,'vsfatch',number_failed)
      call vsfsnam(vsid2, 'mixed type')
      call vsfscls(vsid2, 'test NT')
      ret = vsffdef(vsid2, 'int32', DFNT_INT32, 2)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsffdef(vsid2, 'double', DFNT_FLOAT64, 2)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsffdef(vsid2, 'float32', DFNT_FLOAT32, 2)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsffdef(vsid2, 'int16', DFNT_INT16, 2)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsffdef(vsid2, 'char', DFNT_CHAR, 3)
      call VERIFY(ret,'vsffdef',number_failed)
      ret = vsfsfld(vsid2, 'int32,double,float32,int16,char')
      call VERIFY(ret,'vsfsfld',number_failed)
C     pack the fields into data buf
      ret = vsfnpak(vsid2, HDF_VSPACK,
     +             'int32,double,float32,int16,char', dbuf,
     +             320*4,5,'int32',ddata4)
      call VERIFY(ret,'vsfnpak',number_failed)
      ret = vsfnpak(vsid2, HDF_VSPACK,
     +             ' ', dbuf, 320*4, 5,'double',bufg1)
c     +             ' ', dbuf, 320*4, 5,'double',gdata1)
      call VERIFY(ret,'vsfnpak',number_failed)
      ret = vsfnpak(vsid2, HDF_VSPACK,
     +             ' ', dbuf, 320*4, 5,'float32',buff)
c     +             ' ', dbuf, 320*4, 5,'float32',fdata)
      call VERIFY(ret,'vsfnpak',number_failed)
      ret = vsfnpak(vsid2, HDF_VSPACK,
     +             ' ', dbuf, 320*4,5,'int16',bufd2)
c     +             ' ', dbuf, 320*4,5,'int16',ddata2)
      call VERIFY(ret,'vsfnpak',number_failed)
      ret = vsfcpak(vsid2, HDF_VSPACK,
C    +             'int32,double,float32,int16,char', dbuf,
     +             ' ', dbuf, 320*4, 5,'char',cdata)
      call VERIFY(ret,'vsfcpak',number_failed)
c      ret = vsfwrt(vsid2, dbuf, 5, FULL_INTERLACE)
      ret = vsfwrt(vsid2, buf, 5, FULL_INTERLACE)
      call VERIFY(ret,'vsfwrt',number_failed)
      call vsfdtch(vsid2)

C     insert vdata1 into vgroup1
      call MESSAGE(5,'Inserting vdata1 into top vgroup')
      vsref1 = vsfgid(fid1, -1)
      call VERIFY(vsref1, 'vsfgid', number_failed)
      vsref2  = vsffnd(fid1, 'mixed type')
      call VERIFY(vsref2, 'vsffnd', number_failed)
      ret = vfadtr(vgid1, DFTAG_VH, vsref1)
      call VERIFY(ret, 'vfadtr', number_failed)
      call vfdtch(vgid1)
      call vfend(fid1)
      ret = hclose(fid1)
      call VERIFY(ret,'hclose',number_failed)

C     read data back
      call MESSAGE(5,'Readng data back')
      fid1 = hopen(fn1, DFACC_RDWR, 0)
      call VERIFY(fid1,'hopen',number_failed)
      call vfstart(fid1)
      vgref1 = vfgid(fid1, -1)
      call VERIFY(vgref1, 'vfgid', number_failed)
      vgid1 = vfatch(fid1, vgref1,'w')
      call VERIFY(vgid1, 'vfatch', number_failed)
      ntrs = vfgttrs(vgid1, tags, refs, 10)
      call VERIFY(ntrs, 'vfgttrs', number_failed)
      if (ntrs .ne. 2) then
         number_failed = number_failed + 1
         call MESSAGE(5,'Wrong number of tag/refs. ')
      endif
C     look for the first vdata
      found = FALSE
      vref = -1
      do 20 i = 1, ntrs
         if (found .eq. FALSE) then
             vref = vfgnxt(vgid1,vref)
             call VERIFY(vsref1, 'vref', number_failed)
             found = vfisvs(vgid1, vref)
         endif
20    continue
      if (found .eq. TRUE) then
         vsid1 = vsfatch(fid1, vref, 'w')
         call VERIFY(vsid1,'vsfatch',number_failed)
         ret = vsfinq(vsid1, nelts,il,fields1,vsize,vname)
         call VERIFY(vsid1,'vsfinq',number_failed)
         if (nelts .ne. 3) then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong number of records. ') 
         endif
         if (il .ne. FULL_INTERLACE) then
             number_failed = number_failed + 1
              call MESSAGE(5,'Wrong interlace ')
         endif
         if (fields1 .ne. 'char type') then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong field names. ')
         endif
         if (vsize .ne. 5) then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong vsize. ')
         endif
         ret = vsfsfld(vsid1, 'char type')
         call VERIFY(ret,'vsfsfld',number_failed)
         ret = vsfrdc(vsid1, icdata, 3, FULL_INTERLACE)
         call VERIFY(ret,'vsfrdc',number_failed)
         if (icdata .ne. 'abcdebcdefcdefg') then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong icdata. Correct: abcdebcdefcdefg')
         endif
         call vsfdtch(vsid1)
C     not found, print error message
      else
         call MESSAGE(5,'Not found char type vdata. ')
      endif

C     read the second vdata
      call MESSAGE(5,'Reading the int16 vdata')
      vsref1 = vsfgid(fid1, vref)
      call VERIFY(vsref1, 'vsfgid', number_failed)
      vsid1 = vsfatch(fid1, vsref1, 'w')
      call VERIFY(vsid1,'vsfatch',number_failed)
      ret = vsfinq(vsid1, nelts,il, fields2,vsize,vname)
      call VERIFY(ret,'vsfinq',number_failed)
      if (nelts .ne. 5) then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong number of records. ')
      endif
      if (il .ne. FULL_INTERLACE) then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong interlace. ')
      endif
      if (fields2 .ne. 'int16 type') then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong fields. ')
      endif
      ret = vsfsfld(vsid1, 'int16 type')
      call VERIFY(ret,'vsfsfld',number_failed)
      ret = vsfrd(vsid1, iddata1, 5, FULL_INTERLACE)
      call VERIFY(ret,'vsfrd',number_failed)
      do 40 i=1,10 
          if (iddata1(i) .ne. (9+i)) then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong data. ')  
          endif
40    continue
      call vsfdtch(vsid1)
C     read the 'c1' vdata
      vsref1 = vsffnd(fid1, 'c1')
      call VERIFY(vsref1, 'vsffnd', number_failed)
      vsid1 = vsfatch(fid1, vsref1, 'w')
      call VERIFY(vsid1, 'vsfatch', number_failed)
      ret = vsfsfld(vsid1, 'char1')
      call VERIFY(ret, 'vsfsfld', number_failed)
c  Use equivalence for idbuf
c      ret = vsfrd(vsid1, idbuf, 5,FULL_INTERLACE)
      ret = vsfrd(vsid1, ibuf, 5,FULL_INTERLACE)
      call VERIFY(ret, 'vsfrd', number_failed)
      icdata = '               '
      ret = vsfcpak(vsid1,HDF_VSUNPACK,' ',idbuf,320*4,
     +        5, ' ', icdata)
      call VERIFY(ret, 'vsfcpak', number_failed)
      if (icdata .ne. 'abcde          ')  then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong icdata. ')
      endif
      call vsfdtch(vsid1)
C     read the 'c3' vdata
      vsref1 = vsffnd(fid1, 'c3')
      call VERIFY(vsref1, 'vsffnd', number_failed)
      vsid1 = vsfatch(fid1, vsref1, 'w')
      call VERIFY(vsid1, 'vsfatch', number_failed)
      ret = vsfsfld(vsid1, 'char3') 
      call VERIFY(ret, 'vsfsfld', number_failed)
c      ret = vsfrd(vsid1, idbuf, 5,FULL_INTERLACE)
      ret = vsfrd(vsid1, ibuf, 5,FULL_INTERLACE)
      call VERIFY(ret, 'vsfrd', number_failed)
      icdata = '               '
      ret = vsfcpak(vsid1,HDF_VSUNPACK,' ',idbuf,320*4,
     +        5, ' ', icdata)
      call VERIFY(ret, 'vsfcpak', number_failed)
      if (icdata .ne. 'abcdebcdefcdefg')  then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong data. ')
      endif 
      call vsfdtch(vsid1)
 
C     read the 'mixed type' vdata
      vsref2 = vsffnd(fid1, 'mixed type')
      call VERIFY(vsref2, 'vsffnd', number_failed)
      vsid2 = vsfatch(fid1, vsref2, 'w')
      call VERIFY(vsid2, 'vsfatch', number_failed)
      ret = vsfinq(vsid2, nelts,il, fields3,vsize,vname)
      call VERIFY(ret, 'vsfinq', number_failed)
      if (nelts .ne. 5) then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong number of records. ')
      endif
      if (il .ne. FULL_INTERLACE) then
          number_failed = number_failed + 1
          call MESSAGE(5, 'Wrong interlace. ')
      endif
      if (fields3 .ne. 'int32,double,float32,int16,char') then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong fields. ')
      endif
      ret = vsfsfld(vsid2, 'int32,double,float32,int16,char')
      call VERIFY(ret, 'vsfsfld', number_failed)
c      ret = vsfrd(vsid2, idbuf, 3, FULL_INTERLACE)
      ret = vsfrd(vsid2, ibuf, 3, FULL_INTERLACE)
      call VERIFY(ret, 'vsfrd', number_failed)
      ret = vsfnpak(vsid2, HDF_VSUNPACK,
     +             ' ', idbuf, 320*4,3,'int32',iddata4)
      call VERIFY(ret, 'vsfnpak', number_failed)
      ret = vsfnpak(vsid2, HDF_VSUNPACK,
     +             ' ', idbuf, 320*4,3,'double',ibufg1)
c     +             ' ', idbuf, 320*4,3,'double',igdata1)
      call VERIFY(ret, 'vsfnpak', number_failed)
      ret = vsfnpak(vsid2, HDF_VSUNPACK,
     +             ' ', idbuf, 320*4,3,'float32',ibuff)
c     +             ' ', idbuf, 320*4,3,'float32',ifdata)
      call VERIFY(ret, 'vsfnpak', number_failed)
      ret = vsfnpak(vsid2, HDF_VSUNPACK,
     +             ' ', idbuf, 320*4,3,'int16',ibufd2)
c     +             ' ', idbuf, 320*4,3,'int16',iddata2)
      call VERIFY(ret, 'vsfnpak', number_failed)
      icdata = '               '
      ret = vsfcpak(vsid2, HDF_VSUNPACK,
     +             'int32,double,float32,int16,char', idbuf,
     +             320*4,3,'char',icdata)
      call VERIFY(ret, 'vsfcpak', number_failed)

      do 45 i=1,3
         if (iddata4(i) .ne. (39+i)) then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong data. ')
         endif
         if (iddata2(i) .ne. (19+i)) then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong data. ')
         endif
45    continue
      do 50 i = 1, 3
         if (abs(gdata1(i) - igdata1(i)) .GE.
     +        gdata1(i)*geps)  then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong data. ')
         endif
         if (abs(fdata(i) - ifdata(i)) .GE.
     +        fdata(i)*feps)  then
             number_failed = number_failed + 1
             call MESSAGE(5,'Wrong data. ')
         endif
50    continue
      if (icdata .ne. 'abcdebcde      ') then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong data. ')
      endif
C     read field 'char' only, test pckfld ' '
      ret = vsfsfld(vsid2, 'char')
      call VERIFY(ret, 'vsfsfld', number_failed)
c      ret = vsfrd(vsid2, idbuf, 2, FULL_INTERLACE)
      ret = vsfrd(vsid2, ibuf, 2, FULL_INTERLACE)
      call VERIFY(ret, 'vsfrd', number_failed)
      icdata = '               '
      ret = vsfcpak(vsid2, HDF_VSUNPACK,
     +             'char', idbuf, 320*4,2,' ',icdata)
      call VERIFY(ret, 'vsfcpak', number_failed)
      if (icdata .ne. 'fcdefg         ') then
          number_failed = number_failed + 1
          call MESSAGE(5,'Wrong data. ')
      endif

      call vsfdtch(vsid2)
      call vfdtch(vgid1)
      call vfend(fid1)
      ret = hclose(fid1)
      call VERIFY(ret, 'hclose', number_failed)
      return
      end
