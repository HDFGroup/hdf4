#!/bin/sh
#
# Testing HDF utilities

# hdf24to8
if [ -f hdf24to8 -a -f hdftor8 ]; then
echo "** Testing hdf24to8  ***"
/bin/rm -f head.r24 head.r8 head8.hdf img001-263.328 pal001
cp testfiles/head.r24.Z testfiles/head.r8.Z .
uncompress head.r24.Z head.r8.Z
./hdf24to8 head.r24 head8.hdf
./hdftor8 head8.hdf
cmp img001-263.328 head.r8
/bin/rm -f head.r24 head.r8 head8.hdf img001-263.328 pal001
else
echo "** hdf24to8 or hdftor8 is not available ***"
fi

# hdfed
if [ -f hdfed ]; then
echo "** Testing hdfed  ***"
/bin/rm -f storm110.hdf ntcheck.hdf hdfed.tmp
cp testfiles/storm110.hdf testfiles/ntcheck.hdf .
./hdfed < hdfed.input1 > hdfed.tmp 2>&1
diff hdfed.tmp hdfed.out1
/bin/rm -f storm110.hdf ntcheck.hdf hdfed.tmp
else
echo "** hdfed not available ***"
fi

# ristosds
if [ -f ristosds -a -f hdfed ]; then
echo "** Testing ristosds  ***"
/bin/rm -f storm*.hdf hdfed.tmp1
cp testfiles/storm110.hdf testfiles/storm120.hdf testfiles/storm130.hdf .
./ristosds storm*.hdf -o storm.hdf
./hdfed < ristosds.input1 > hdfed.tmp1 2>&1
diff hdfed.tmp1 ristosds.out1
/bin/rm -f storm*.hdf hdfed.tmp1
else
echo "** ristosds or hdfed not available ***"
fi

#hdfpack
if [ -f hdfpack -a -f hdfls ]; then
echo "** Testing hdfpack  ***"
/bin/rm -f test.hdf test.blk test.pck hdfls.tmp1
cp testfiles/test.hdf .
./hdfpack test.hdf test.pck
./hdfpack -b test.hdf test.blk
./hdfls test.hdf > hdfls.tmp1 2>&1
./hdfls test.pck >> hdfls.tmp1 2>&1
diff hdfls.tmp1 hdfpack.out1
/bin/rm -f test.hdf test.blk test.pck hdfls.tmp1
else
echo "** hdfpack or hdfls not available ***"
fi

#hdftopal/paltohdf
if [ -f  hdftopal -a -f paltohdf ]; then
echo "** Testing hdftopal/paltohdf  ***"
/bin/rm -f palette.* 
cp testfiles/palette.raw .
./paltohdf palette.raw palette.hdf
./hdftopal palette.hdf palette.raw.new
cmp palette.raw palette.raw.new
/bin/rm -f palette.* 
else
echo "** hdftopal or paltohdf not available ***"
fi

# r8tohdf/hdftor8
if [ -f r8tohdf -a -f hdftor8 -a -f hdfls ]; then
echo "** Testing r8tohdf/hdftor8  ***"
/bin/rm -f storm* img* palette.raw hdfls.tmp2 pal005
cp testfiles/storm*.raw testfiles/palette.raw .
./r8tohdf 57 57 storm.hdf storm*.raw
./r8tohdf 57 57 storm.hdf -p palette.raw -i storm110.raw
./hdftor8 storm.hdf
./hdfls -l storm.hdf > hdfls.tmp2 2>&1
diff hdfls.tmp2 hdftor8.out1
cmp img001-057.057  storm110.raw
cmp img002-057.057  storm120.raw
cmp img003-057.057  storm130.raw
cmp img004-057.057  storm140.raw
/bin/rm -f storm* img* palette.raw hdfls.tmp2 pal005
else
echo "** r8tohdf, hdftor8 or hdfls not available ***"
fi

# hdfcomp
if [ -f  hdfcomp -a -f hdfls ]; then
echo "** Testing hdfcomp  ***"
/bin/rm -f storm*.hdf all*.hdf hdfls.tmp3
cp testfiles/storm*.hdf .
./hdfcomp allstorms.hdf storm*.hdf
./hdfcomp allcomp.hdf -c storm*.hdf
./hdfls -l allstorms.hdf > hdfls.tmp3 2>&1
./hdfls -l allcomp.hdf >> hdfls.tmp3 2>&1
diff hdfls.tmp3 hdfcomp.out1
/bin/rm -f storm*.hdf all*.hdf hdfls.tmp3
else
echo "** hdfcomp or hdfls not available ***"
fi

#jpeg2hdf/hdf2jpeg
if [ -f jpeg2hdf -a -f hdf2jpeg -a -f hdfls ]; then
echo "** Testing jpeg2hdf/hdf2jpeg  ***"
/bin/rm -f jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
cp testfiles/jpeg_img.jpg .
./jpeg2hdf jpeg_img.jpg jpeg.hdf
./hdf2jpeg jpeg.hdf jpeg2.jpg
./hdfls -l jpeg.hdf > hdfls.tmp4 2>&1
diff hdfls.tmp4 jpeg2hdf.out1
cmp jpeg_img.jpg jpeg2.jpg
/bin/rm -f jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
else
echo "** jpeg2hdf, hdf2jpeg or hdfls  not available ***"
fi

# fp2hdf
if [ -f fp2hdf -a -f hdfls ]; then
echo "** Testing fp2hdf  ***"
/bin/rm -f ctxtr* cb* *.hdf hdfls.tmp5
./fptest
./fp2hdf ctxtr2 -o ctxtr2.hdf
./fp2hdf ctxtr3 -o ctxtr3.hdf
./fp2hdf cb32r2 -o cb32r2.hdf
./fp2hdf cb32r3 -o cb32r3.hdf
./fp2hdf cb64r2 -o cb64r2.hdf
./fp2hdf cb64r3 -o cb64r3.hdf
./fp2hdf ctxtr2 -o ctxtr2_ris.hdf -raster -e 50 50
./fp2hdf cb64r2 -o cb64r2_ris.hdf -raster -i 50 50 -f
./hdfls -l ctxtr2.hdf > hdfls.tmp5 2>&1
./hdfls -l ctxtr3.hdf >> hdfls.tmp5 2>&1
./hdfls -l cb32r2.hdf >> hdfls.tmp5 2>&1
./hdfls -l cb32r3.hdf >> hdfls.tmp5 2>&1
./hdfls -l cb64r2.hdf >> hdfls.tmp5 2>&1
./hdfls -l cb64r3.hdf >> hdfls.tmp5 2>&1
./hdfls -l ctxtr2_ris.hdf >> hdfls.tmp5 2>&1
./hdfls -l cb64r2_ris.hdf >> hdfls.tmp5 2>&1
diff hdfls.tmp5 fp2hdf.out1
/bin/rm -f ctxtr* cb* *.hdf hdfls.tmp5
else
echo "** fp2hdf or hdfls not available ***"
fi
