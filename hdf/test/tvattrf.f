C***********************************************************
C * NCSA HDF
C   *
C * Software Development Group
C   *
C * National Center for Supercomputing Applications
C   *
C * University of Illinois at Urbana-Champaign
C   *
C * 605 E. Springfield, Champaign IL 61820
C   *
C *
C   *
C * For conditions of distribution and use, see the accompanying
C   *
C * hdf/COPYING file.
C   *
C *
C   *
C***************************************************
C
C $Id$
C
C **************************************************
C *
C * tvattrf.f
C * test Vset attribute Fortran API 
C *
C *************************************************
       subroutine tvattrf (number_failed)
C       program tvattrf 
C Test Program:
C   Tests the vdata and vgroup attribute interface
C Input file: tvattr.dat
C Output file: tvattrf.hdf

      implicit none
      include 'fortest.inc'

      integer number_failed
      character*(*) testname, ifn, ofn
      parameter (testname = 'vattrf',
     +           ifn = 'tvattr.dat' ,
     +           ofn = 'tvattrf.hdf'
     +          )  
      integer vsffidx, vsfsat, vsfscat, vsfnat
      integer vsffnat, vsffdat, vsfainf, vsfgat
      integer vsfgcat, vsfisat, vfnflds
      integer vfsatt, vfscatt, vfnatts
      integer vffdatt, vfainfo, vfgatt, vfgcatt
      integer vfgver
      integer hopen, hclose, vfatch, vfdtch, vfstart
      integer vfend, vsfatch, vsfdtch, vfgid, vsgver
      integer vsffnd, vhfsd

      integer nattrs, itype, icount, imsize, vsver
      integer fid1, vsid, vgid, vsref, vgref
      integer ret, findex, iattri(5), vsbuf, aindex
      integer*2 iattrs(6)
      real    iattrr(5), feps, RATTR1, abs
      double precision iattrg(5), geps, dabs, GATTR1
      character*10  iattrc
      character*20 iattrnm
     
      integer DFACC_CREATE, DFACC_RDWR, DFNT_CHAR
      integer DFNT_INT32, DFNT_FLOAT32 
      integer VSET_VERSION, VSET_NEW_VERSION
      integer ENTIRE_VDATA

      parameter (DFACC_CREATE = 4,
     +           DFACC_RDWR = 3,
     +           DFNT_CHAR = 4,
     +           DFNT_INT32 = 24,
     +           DFNT_FLOAT32 = 5,
     +           VSET_VERSION = 3,
     +           VSET_NEW_VERSION = 4,
     +           ENTIRE_VDATA = -1,
     +           GATTR1 = -64.123450D0,
     +           RATTR1 = 32.0099,
     +           feps = 1.0E-5,
     +           geps = 1.0D-9
     +          )

      DATA vsbuf/200/
      DATA iattrc/' '/

      call ptestban('Testing', testname)
C      number_failed = 0
C Open the file 
      fid1 = hopen(ifn, DFACC_RDWR, 0)
      call VERIFY(fid1, 'hopen', number_failed)
      ret = vfstart(fid1)
      call VERIFY(ret, 'vfstart', number_failed)
      vsref = vsffnd(fid1, 'vsname1')
      call VERIFY(vsref, 'vsffnd', number_failed)
      vsid = vsfatch(fid1, vsref, 'r')
      call VERIFY(vsid, 'vsfatch', number_failed)
      vsver = vsgver(vsid)
      if (vsver .ne. VSET_NEW_VERSION) then
         call MESSAGE(1, 'Wrong version number of vsname1')
         number_failed = number_failed + 1
      endif
      ret = vsfisat(vsid)
      if (ret .ne. FALSE) then
         call MESSAGE(1, 'Vsname1 is not an attr.')
         number_failed = number_failed + 1
      endif
      ret = vfnflds(vsid)
      if (ret .ne. 2) then
         call MESSAGE(1, 'Wrong number of fields of vsname1')
         number_failed = number_failed + 1
      endif
      nattrs = vsfnat(vsid)
      if (nattrs .ne. 6) then
         call MESSAGE(1, 'Wrong number of total attrs for vsname1')
         number_failed = number_failed + 1
      endif
      nattrs = vsffnat(vsid, ENTIRE_VDATA)
      if (nattrs .ne. 2) then
         call MESSAGE(1, 'Wrong number of attrs for vsname1')
         number_failed = number_failed + 1
      endif
C get the 0th attr 
      aindex = 0
      findex = ENTIRE_VDATA
      ret = vsffdat(vsid, findex, 'attname3') 
      call VERIFY(ret, 'vsffdat', number_failed)
      ret = vsfainf(vsid, findex, aindex, iattrnm,
     + itype, icount, imsize)
      call VERIFY(ret, 'vsfainf', number_failed)
      if ((itype .ne. DFNT_CHAR) .or. (icount .ne. 3)) then
         call MESSAGE(1, 'Wrong info of char attr for vsname1')
         number_failed = number_failed + 1
      endif
      ret = vsfgcat(vsid, findex, aindex, iattrc)
      call VERIFY(ret, 'vsfgcat', number_failed)
C      if ((iattrc(1) .ne. 'm') .or. (iattrc(2) .ne. 'N') 
C    +    .or. (iattrc(3) .ne. 'p'))  then
      if (iattrc .ne. 'mNp') then
         call MESSAGE(1,'Wrong values of char attr for vsname1')
         number_failed = number_failed + 1
      endif
C get the 1st attr of fld 0
      aindex = 1
      findex = 0
      ret = vsffidx(vsid, 'fldname1', findex)
      call VERIFY(ret, 'vsffidx', number_failed)
      if (findex .ne. 0) then
         call MESSAGE(1, 'Wrong findex of fldname1 of  vsname1')
         number_failed = number_failed + 1
      endif
      ret = vsfgat(vsid, findex, aindex, iattrg)
      call VERIFY(ret, 'vsfgat1', number_failed)
      if (dabs(iattrg(1)-GATTR1) .gt. dabs(geps * GATTR1)) 
     +          then
       call MESSAGE(1,'Wrong double attr for vsname1')
       number_failed = number_failed + 1
      print *, 'should be: ',GATTR1,' get: ',iattrg(1)
      endif
      ret = vsfdtch(vsid)
      call VERIFY(ret, 'vsfdtch', number_failed)
C get next vdata and test vsfisat 
      vsref = vsffnd(fid1, 'attname1')
      vsid = vsfatch(fid1, vsref, 'r')
      call VERIFY(ret, 'vsfatch', number_failed)
      ret = vsfisat(vsid)
      if (ret .ne. TRUE) then
         call MESSAGE(1, 'vsfisat failed.   ')
         number_failed = number_failed + 1
      endif
      ret = vsfdtch(vsid)
      call VERIFY(ret, 'vsfdtch', number_failed)
C test vgroup routines
      vgref = vfgid(fid1, -1)
      call VERIFY(vgref, 'vfgid',number_failed)
      vgid = vfatch(fid1, vgref, 'r')
      call VERIFY(vgid, 'vfatch',number_failed)
      ret = vfgver(vgid)
      call VERIFY(ret, 'vfgver',number_failed)
      if (ret .ne. VSET_NEW_VERSION) then
         call MESSAGE(1, 'vfgver failed.   ')
         number_failed = number_failed + 1
      endif
      nattrs = vfnatts(vgid)
      if (nattrs .ne. 2) then
         call MESSAGE(1, 'vfnatts failed.   ')
         number_failed = number_failed + 1
      endif
      ret = vfainfo(vgid,0,iattrnm,itype,icount,imsize)
      call VERIFY(ret, 'vfainfo',number_failed)
      ret = vfgatt(vgid, 0, iattri)
      call VERIFY(ret, 'vfgatt',number_failed)
      if (iattri(1) .ne. 10032 .or. iattri(2) .ne. 10064)
     +      then
         call MESSAGE(1, 'vfgatt failed.   ')
         number_failed = number_failed + 1
      endif
C get attname2
      ret = vffdatt(vgid, 'attname2')
      call VERIFY(ret, 'vffdatt',number_failed)
      ret = vfgatt(vgid, ret, iattrs)
      call VERIFY(ret, 'vfgatt',number_failed)
      if (iattrs(1) .ne. 16 .or. iattrs(2) .ne. 32)
     +       then
         call MESSAGE(1, 'vfgatt failed.   ')
         number_failed = number_failed + 1
      endif
C finish reading existing attrs
      ret = vfdtch(vgid)
      call VERIFY(ret, 'vfdtch',number_failed)
      ret = vfend(fid1)
      call VERIFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VERIFY(ret, 'hclose1',number_failed)
C write attributes
      fid1 = hopen(ofn, DFACC_CREATE, 0)
      call VERIFY(fid1, 'hopen',number_failed)
      ret =  vfstart(fid1)
      call VERIFY(ret, 'vfstart', number_failed)
C vgroup attrs
      vgid = vfatch(fid1, -1, 'w')
      call VERIFY(ret, 'vfatch', number_failed)
      ret = vfscatt(vgid, 'vgcattr0', DFNT_CHAR, 4, 'cat0')
      call VERIFY(ret, 'vfsatt', number_failed)
      ret = vfsatt(vgid,'vgattr1',DFNT_FLOAT32,1,RATTR1)
      call VERIFY(ret, 'vfsatt', number_failed)
      ret = vfdtch(vgid)
      call VERIFY(ret, 'vfdtch', number_failed)
C vdata attrs
      vsref = vhfsd(fid1, 'vsfld0', vsbuf, 1, DFNT_INT32,
     +              'vs0', 'test_vdata')
      call VERIFY(ret, 'vhfsd', number_failed)
      vsid = vsfatch(fid1, vsref, 'w')
      call VERIFY(ret, 'vsfatch', number_failed)
      ret = vsfscat(vsid, ENTIRE_VDATA, 'vscattr0', DFNT_CHAR, 3,
     +              'at2')
      call VERIFY(ret, 'vfsatt', number_failed)
      ret = vsfsat(vsid, 0, 'vsattr1', DFNT_FLOAT32, 1, 
     +             RATTR1)
      call VERIFY(ret, 'vsfsat', number_failed)
      ret = vsfdtch(vsid) 
      call VERIFY(ret, 'vsfdtch', number_failed)
      ret = vfend(fid1)
      call VERIFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VERIFY(ret, 'hclose2',number_failed)
C read the new attrs back
      fid1 = hopen(ofn, DFACC_RDWR, 0)
      call VERIFY(fid1, 'hopen',number_failed)
      ret =  vfstart(fid1)
      call VERIFY(ret, 'vfstart', number_failed)
C vgroup attrs
      vgref = vfgid(fid1, -1)
      call VERIFY(ret, 'vfgid', number_failed)
      vgid = vfatch(fid1, vgref, 'w')
      call VERIFY(ret, 'vfatch', number_failed)
      ret = vfgcatt(vgid, 0, iattrc)
      call VERIFY(ret, 'vfgcatt', number_failed)
C      if (iattrc(1) .ne. 'a' .or. iattrc(2) .ne. 't'
C     +     .or. iattrc(3) .ne. '0') then
      if (iattrc .ne. 'cat0') then
         call MESSAGE(1, 'Wrong values of char attr for vg')
         number_failed = number_failed + 1
      endif
      ret = vfgatt(vgid, 1, iattrr)
      call VERIFY(ret, 'vfgatt', number_failed)
      if (abs(iattrr(1)-RATTR1) .gt. abs(RATTR1*feps)) 
     +          then
         call MESSAGE(1, 'Wrong values of real attr for vg')
         number_failed = number_failed + 1
      endif
      ret = vfdtch(vgid)
      call VERIFY(ret, 'vfdtch', number_failed)
C vdata attrs
      vsref = vsffnd(fid1, 'vs0')
      call VERIFY(ret, 'vsffnd', number_failed)
      vsid = vsfatch(fid1, vsref, 'w')
      call VERIFY(ret, 'vsfatch', number_failed)
      ret = vsfgcat(vsid, ENTIRE_VDATA, 0, iattrc)
      call VERIFY(ret, 'vsfgcat', number_failed)
C      if (iattrc(1) .ne. 'a' .or. iattrc(2) .ne. 't'
C     +     .or. iattrc(3) .ne. '2') then
      if (iattrc .ne. 'at2') then
         call MESSAGE(1, 'Wrong values of char attr for vs')
         number_failed = number_failed + 1
         print *, iattrc, 'at2'
      endif
      ret = vsfgat(vsid, 0, 0, iattrr)
      call VERIFY(ret, 'vsfgat2', number_failed)
      if (abs(iattrr(1)-RATTR1) .gt. abs(RATTR1*feps)) 
     +          then
         call MESSAGE(1, 'Wrong values of real attr for vs')
         number_failed = number_failed + 1
      endif
      ret = vsfdtch(vsid)
      call VERIFY(ret, 'vsfdtch', number_failed)
      ret = vfend(fid1)
      call VERIFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VERIFY(ret, 'hclose3',number_failed)
      return
      end









       

     
        
     
