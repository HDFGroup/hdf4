#!/bin/sh
#
# Testing HDF utilities

# initialize errors variable
errors=0

echo ""
echo "======================="
echo "Utilities tests started"
echo "======================="

# hdf24to8
if [ -f hdf24to8 -a -f hdftor8 ]; then
echo "** Testing hdf24to8  ***"
/bin/rm -f head.r24 head.r8 head8.hdf img001-263.328 pal001
cp testfiles/head.r24.Z testfiles/head.r8.Z .
uncompress head.r24.Z head.r8.Z
./hdf24to8 head.r24 head8.hdf
./hdftor8 head8.hdf
cmp img001-263.328 head.r8 || errors=1
/bin/rm -f head.r24 head.r8 head8.hdf img001-263.328 pal001
else
echo "** hdf24to8 or hdftor8 is not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " hdf24to8 failed ***"
   errors=0
fi

# hdfed
if [ -f hdfed ]; then
echo "** Testing hdfed  ***"
/bin/rm -f storm110.hdf ntcheck.hdf hdfed.tmp
cp testfiles/storm110.hdf testfiles/ntcheck.hdf .
./hdfed < hdfed.input1 > hdfed.tmp 2>&1
diff -w hdfed.tmp hdfed.out1 || errors=1
/bin/rm -f storm110.hdf ntcheck.hdf hdfed.tmp
else
echo "** hdfed not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " hdfed failed ***"
   errors=0
fi

# ristosds
if [ -f ristosds -a -f hdfed ]; then
echo "** Testing ristosds  ***"
/bin/rm -f storm*.hdf hdfed.tmp1
cp testfiles/storm110.hdf testfiles/storm120.hdf testfiles/storm130.hdf .
./ristosds storm*.hdf -o storm.hdf > /dev/null 2>&1
./hdfed < ristosds.input1 > hdfed.tmp1 2>&1
diff -w hdfed.tmp1 ristosds.out1 || errors=1
/bin/rm -f storm*.hdf hdfed.tmp1
else
echo "** ristosds or hdfed not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " ristosds failed ***"
   errors=0
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
diff -w hdfls.tmp1 hdfpack.out1 || errors=1
/bin/rm -f test.hdf test.blk test.pck hdfls.tmp1
else
echo "** hdfpack or hdfls not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " hdfpack failed ***"
   errors=0
fi

#hdftopal/paltohdf
if [ -f  hdftopal -a -f paltohdf ]; then
echo "** Testing hdftopal/paltohdf  ***"
/bin/rm -f palette.* 
cp testfiles/palette.raw .
./paltohdf palette.raw palette.hdf
./hdftopal palette.hdf palette.raw.new
cmp palette.raw palette.raw.new || errors=1
/bin/rm -f palette.* 
else
echo "** hdftopal or paltohdf not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " hdftopal/paltohdf failed ***"
   errors=0
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
diff -w hdfls.tmp2 hdftor8.out1 || errors=1
cmp img001-057.057  storm110.raw || errors=1
cmp img002-057.057  storm120.raw || errors=1
cmp img003-057.057  storm130.raw || errors=1
cmp img004-057.057  storm140.raw || errors=1
/bin/rm -f storm* img* palette.raw hdfls.tmp2 pal005
else
echo "** r8tohdf, hdftor8 or hdfls not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " r8tohdf/hdftor8 failed ***"
   errors=0
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
diff -w hdfls.tmp3 hdfcomp.out1 || errors=1
/bin/rm -f storm*.hdf all*.hdf hdfls.tmp3
else
echo "** hdfcomp or hdfls not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " hdfcomp failed ***"
   errors=0
fi

#jpeg2hdf/hdf2jpeg
if [ -f jpeg2hdf -a -f hdf2jpeg -a -f hdfls ]; then
echo "** Testing jpeg2hdf/hdf2jpeg  ***"
/bin/rm -f jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
cp testfiles/jpeg_img.jpg .
./jpeg2hdf jpeg_img.jpg jpeg.hdf
./hdf2jpeg jpeg.hdf jpeg2.jpg
./hdfls -l jpeg.hdf > hdfls.tmp4 2>&1
diff -w hdfls.tmp4 jpeg2hdf.out1 || errors=1
cmp jpeg_img.jpg jpeg2.jpg || errors=1
/bin/rm -f jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
else
echo "** jpeg2hdf, hdf2jpeg or hdfls  not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " jpeg2hdf/hdf2jpeg failed ***"
   errors=0
fi

# fp2hdf
if [ -f fp2hdf -a -f hdfls -a -f hdfed ]; then
echo "** Testing fp2hdf  ***"
/bin/rm -f ctxtr* cb* *.hdf hdfls.tmp5 hdfed.tmp5
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
diff -w hdfls.tmp5 fp2hdf.out1 || errors=1
./hdfed < fp2hdf.input1 > hdfed.tmp6 2>&1
diff -w hdfed.tmp6 fp2hdf.out2 || errors=1
/bin/rm -f ctxtr* cb* *.hdf hdfls.tmp5 hdfed.tmp5
else
echo "** fp2hdf, hdfed or hdfls not available ***"
fi

if [ $errors -eq 1 ]; then
   echo " fp2hdf failed ***"
fi

#
# Check errors result
if [ $errors -eq 0 ]; then
    echo "======================="
    echo "Utilities tests passed."
    echo "======================="
else
    echo "**********************************"
    echo "Utilities tests encountered errors"
    echo "**********************************"
fi
echo ""
exit $errors
