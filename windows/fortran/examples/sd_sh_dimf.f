      PROGRAM SD_SH_DIMF
C     This program collects 9 3D data sets of 8 cities
C     All SDS's share dimensions.
 
      integer istat, i,j,k,l, city, dataset, city_num, dset_num
      integer sfstart,sfend,sfcreate,sfsdmname, sfendacc
      integer sfwdata,sfdimid
      integer dimsizes(3),start(3),end(3),stride(3),fid, sdsid
      integer dimid0, dimid1, dimid2
      integer DFACC_CREATE, DFNT_INT32, HEIGHT, LAT,LON
      parameter (DFACC_CREATE = 4, DFNT_INT32 = 24)
      integer data(1000)
      character suf1, suf2, char
      character*3 basenm
      character*8 sdsname
      HEIGHT = 10
      LAT = 10
      LON = 10
      city_num = 8
      dset_num = 9
      dimsizes(1) = LON
      dimsizes(2) = LAT
      dimsizes(3) = HEIGHT
      l = 1
      do 300 k=1,HEIGHT
         do 250 j=1,LAT
            do 200 i=1,LON
               data(l) = l
               l = l+1
200         continue
250      continue
300   continue
      basenm = 'sds'
      start(1) = 0
      start(2) = 0
      start(3) = 0
      stride(1) = 1
      stride(2) = 1
      stride(3) = 1
      end(1) = LON
      end(2) = LAT
      end(3) = HEIGHT
      fid = sfstart('sd_sh_dimf.hdf',DFACC_CREATE)
      do 30 city=1, city_num
         do 20 dataset=1, dset_num
            suf1 = char(48+city)
            suf2 = char(48+dataset)
            sdsname = basenm//'_'//suf1//'_'//suf2
            sdsid = sfcreate(fid, sdsname,DFNT_INT32,3,dimsizes)
            dimid2 = sfdimid(sdsid, 0)
            istat = sfsdmname(dimid2,'LONG')
            dimid1 = sfdimid(sdsid, 1)
            istat = sfsdmname(dimid1, 'LAT')
            dimid0 = sfdimid(sdsid, 2)
            istat = sfsdmname(dimid0, 'HEIGHT')
            istat = sfwdata(sdsid,start,stride,end,data)
            istat = sfendacc(sdsid)
20       continue
30    continue
      istat = sfend(fid)
      end

          
