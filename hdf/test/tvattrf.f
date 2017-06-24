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
C Input file: test_files/tvattr.dat
C Output file: tvattrf.hdf

      implicit none
      include 'fortest.inc'

      integer number_failed

      integer len_in
      character*1024 ifn_out
      character*25 myname, ifn, ofn
      parameter (myname = 'vattrf',
     +           ifn = 'test_files/tvattr.dat' ,
     +           ofn = 'tvattrf.hdf'
     +          )

      integer nattrs, itype, icount, imsize, vsver
      integer fid1, vsid, vgid, vsref, vgref
      integer ret, findex, vsbuf, aindex
      integer fixname
C
C iattri is for int32 values
C iattrs is for int16 values
C iattrr, RATTR1 are for float32 values
C iattrg, GATTR1 are for float64 values
      integer*4 iattri(5)
      integer*2 iattrs(6)
      real*4    iattrr(5), feps, RATTR1, abs
      real*8    iattrg(5)
      double precision geps, GATTR1, dabs
      character*10  iattrc
      character*20 iattrnm
     
      integer VSET_VERSION, VSET_NEW_VERSION

      parameter (VSET_VERSION = 3,
     +           VSET_NEW_VERSION = 4,
     +           GATTR1 = -64.123450D0,
     +           RATTR1 = 32.0099,
     +           feps = 1.0E-5,
     +           geps = 1.0D-9
     +          )

      DATA vsbuf/200/
      DATA iattrc/' '/

      call ptestban('Testing', myname)
C      number_failed = 0
C Open the file 
      len_in = len(ifn_out)
      ret = fixname(ifn, ifn_out, len_in)
      fid1 = hopen(ifn_out(1:len_in), DFACC_READ, 0)
      call VRFY(fid1, 'hopen', number_failed)
      ret = vfstart(fid1)
      call VRFY(ret, 'vfstart', number_failed)
      vsref = vsffnd(fid1, 'vsname1')
      call VRFY(vsref, 'vsffnd', number_failed)
      vsid = vsfatch(fid1, vsref, 'r')
      call VRFY(vsid, 'vsfatch', number_failed)
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
      nattrs = vsfnats(vsid)
      if (nattrs .ne. 6) then
         call MESSAGE(1, 'Wrong number of total attrs for vsname1')
         number_failed = number_failed + 1
      endif
      nattrs = vsffnas(vsid, HDF_VDATA)
      if (nattrs .ne. 2) then
         call MESSAGE(1, 'Wrong number of attrs for vsname1')
         number_failed = number_failed + 1
      endif
C get the 0th attr 
      aindex = 0
      findex = HDF_VDATA
      ret = vsffdat(vsid, findex, 'attname3') 
      call VRFY(ret, 'vsffdat', number_failed)
      ret = vsfainf(vsid, findex, aindex, iattrnm,
     + itype, icount, imsize)
      call VRFY(ret, 'vsfainf', number_failed)
      if ((itype .ne. DFNT_CHAR) .or. (icount .ne. 3)) then
         call MESSAGE(1, 'Wrong info of char attr for vsname1')
         number_failed = number_failed + 1
      endif
      iattrc = '          '
      ret = vsfgcat(vsid, findex, aindex, iattrc)
      call VRFY(ret, 'vsfgcat', number_failed)
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
      call VRFY(ret, 'vsffidx', number_failed)
      if (findex .ne. 0) then
         call MESSAGE(1, 'Wrong findex of fldname1 of  vsname1')
         number_failed = number_failed + 1
      endif
C
      ret = vsfgnat(vsid, findex, aindex, iattrg)
      call VRFY(ret, 'vsfgnatt1', number_failed)
      if (dabs(iattrg(1)-GATTR1) .gt. dabs(geps * GATTR1)) 
     +          then
         call MESSAGE(1, 'Wrong double attr for vsname1')
         number_failed = number_failed + 1
      print *, 'should be: ',GATTR1,' get: ',iattrg(1)
      endif
      ret = vsfdtch(vsid)
      call VRFY(ret, 'vsfdtch', number_failed)
C get next vdata and test vsfisat 
      vsref = vsffnd(fid1, 'attname1')
      vsid = vsfatch(fid1, vsref, 'r')
      call VRFY(ret, 'vsfatch', number_failed)
      ret = vsfisat(vsid)
      if (ret .ne. TRUE) then
         call MESSAGE(1, 'vsfisat failed.   ')
         number_failed = number_failed + 1
      endif
      ret = vsfdtch(vsid)
      call VRFY(ret, 'vsfdtch', number_failed)
C test vgroup routines
      vgref = vfgid(fid1, -1)
      call VRFY(vgref, 'vfgid',number_failed)
      vgid = vfatch(fid1, vgref, 'r')
      call VRFY(vgid, 'vfatch',number_failed)
      ret = vfgver(vgid)
      call VRFY(ret, 'vfgver',number_failed)
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
      call VRFY(ret, 'vfainfo',number_failed)
      ret = vfgnatt(vgid, 0, iattri)
      call VRFY(ret, 'vfgnatt',number_failed)
      if (iattri(1) .ne. 10032 .or. iattri(2) .ne. 10064)
     +      then
         call MESSAGE(1, 'vfgnatt failed.   ')
         number_failed = number_failed + 1
      endif
C get attname2
      ret = vffdatt(vgid, 'attname2')
      call VRFY(ret, 'vffdatt',number_failed)
      ret = vfgnatt(vgid, ret, iattrs)
      call VRFY(ret, 'vfgnatt',number_failed)
      if (iattrs(1) .ne. 16 .or. iattrs(2) .ne. 32)
     +       then
         call MESSAGE(1, 'vfgnatt failed.   ')
         number_failed = number_failed + 1
      endif
C finish reading existing attrs
      ret = vfdtch(vgid)
      call VRFY(ret, 'vfdtch',number_failed)
      ret = vfend(fid1)
      call VRFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VRFY(ret, 'hclose1',number_failed)
C write attributes
      fid1 = hopen(ofn, DFACC_CREATE, 0)
      call VRFY(fid1, 'hopen',number_failed)
      ret =  vfstart(fid1)
      call VRFY(ret, 'vfstart', number_failed)
C vgroup attrs
      vgid = vfatch(fid1, -1, 'w')
      call VRFY(ret, 'vfatch', number_failed)
      ret = vfscatt(vgid, 'vgcattr0', DFNT_CHAR, 4, 'cat0')
      call VRFY(ret, 'vfscatt', number_failed)
      ret = vfsnatt(vgid,'vgattr1',DFNT_FLOAT32,1,RATTR1)
      call VRFY(ret, 'vfsnatt', number_failed)
      ret = vfdtch(vgid)
      call VRFY(ret, 'vfdtch', number_failed)
C vdata attrs
      vsref = vhfsd(fid1, 'vsfld0', vsbuf, 1, DFNT_INT32,
     +              'vs0', 'test_vdata')
      call VRFY(ret, 'vhfsd', number_failed)
      vsid = vsfatch(fid1, vsref, 'w')
      call VRFY(ret, 'vsfatch', number_failed)
      ret = vsfscat(vsid, HDF_VDATA, 'vscattr0', DFNT_CHAR, 3,
     +              'at2')
      call VRFY(ret, 'vfscatt', number_failed)
      ret = vsfsnat(vsid, 0, 'vsattr1', DFNT_FLOAT32, 1, 
     +             RATTR1)
      call VRFY(ret, 'vsfsnat', number_failed)
      ret = vsfdtch(vsid) 
      call VRFY(ret, 'vsfdtch', number_failed)
      ret = vfend(fid1)
      call VRFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VRFY(ret, 'hclose2',number_failed)
C read the new attrs back
      fid1 = hopen(ofn, DFACC_RDWR, 0)
      call VRFY(fid1, 'hopen',number_failed)
      ret =  vfstart(fid1)
      call VRFY(ret, 'vfstart', number_failed)
C vgroup attrs
      vgref = vfgid(fid1, -1)
      call VRFY(ret, 'vfgid', number_failed)
      vgid = vfatch(fid1, vgref, 'w')
      call VRFY(ret, 'vfatch', number_failed)
      iattrc = '          '
      ret = vfgcatt(vgid, 0, iattrc)
      call VRFY(ret, 'vfgcatt', number_failed)
C      if (iattrc(1) .ne. 'a' .or. iattrc(2) .ne. 't'
C     +     .or. iattrc(3) .ne. '0') then
      if (iattrc .ne. 'cat0') then
         call MESSAGE(1, 'Wrong values of char attr for vg')
         number_failed = number_failed + 1
      endif
      ret = vfgnatt(vgid, 1, iattrr)
      call VRFY(ret, 'vfgnatt', number_failed)
      if (abs(iattrr(1)-RATTR1) .gt. abs(RATTR1*feps)) 
     +          then
         call MESSAGE(1, 'Wrong values of real attr for vg')
         number_failed = number_failed + 1
      endif
      ret = vfdtch(vgid)
      call VRFY(ret, 'vfdtch', number_failed)
C vdata attrs
      vsref = vsffnd(fid1, 'vs0')
      call VRFY(ret, 'vsffnd', number_failed)
      vsid = vsfatch(fid1, vsref, 'w')
      call VRFY(ret, 'vsfatch', number_failed)
      iattrc = '          '
      ret = vsfgcat(vsid, HDF_VDATA, 0, iattrc)
      call VRFY(ret, 'vsfgcat', number_failed)
C      if (iattrc(1) .ne. 'a' .or. iattrc(2) .ne. 't'
C     +     .or. iattrc(3) .ne. '2') then
      if (iattrc .ne. 'at2') then
         call MESSAGE(1, 'Wrong values of char attr for vs')
         number_failed = number_failed + 1
         print *, iattrc, 'at2'
      endif
      ret = vsfgnat(vsid, 0, 0, iattrr)
      call VRFY(ret, 'vsfgnatt2', number_failed)
      if (abs(iattrr(1)-RATTR1) .gt. abs(RATTR1*feps)) 
     +          then
         call MESSAGE(1, 'Wrong values of real attr for vs')
         number_failed = number_failed + 1
      endif
      ret = vsfdtch(vsid)
      call VRFY(ret, 'vsfdtch', number_failed)
      ret = vfend(fid1)
      call VRFY(ret, 'vfend',number_failed)
      ret = hclose(fid1)
      call VRFY(ret, 'hclose3',number_failed)
      return
      end









       

     
        
     
