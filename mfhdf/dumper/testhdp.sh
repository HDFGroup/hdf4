#! /bin/sh -x
# The following commands can be used for testing dumper hdp

CMD='./hdp'
RM='rm -f'

# Test command list
$CMD list tdata.hdf
$CMD list -l tdata.hdf
$CMD list -d tdata.hdf
$CMD list -e tdata.hdf
$CMD list -t 720 tdata.hdf
$CMD list -d -t "Numeric Data Group" tdata.hdf
$CMD list -g tdata.hdf
$CMD list -a tdata.hdf
$CMD list -a Example6.hdf
$CMD list -n Example6.hdf

# Test command dumpsds
$CMD dumpsds swf32.hdf
$CMD dumpsds -i 2 swf32.hdf
$CMD dumpsds -i 1,3 swf32.hdf
echo 'Next command should fail with error message: "SD with name Time: not found"'
$CMD dumpsds -n Time swf32.hdf
$CMD dumpsds -n fakeDim0,Data-Set-2 swf32.hdf
$CMD dumpsds -r 3,2 swf32.hdf
$CMD dumpsds -r 3,2 -d swf32.hdf

# Test command dumprig
$CMD dumprig tdf24.hdf
$CMD dumprig -i 1,2 tdf24.hdf
$CMD dumprig -i 1,3 tdf24.hdf		# '-i 3' is invalid
$CMD dumprig -m 24 tdf24.hdf
$CMD dumprig -r 3,4 tdf24.hdf
$CMD dumprig -r 3,4 -d tdf24.hdf

# Test command dumpvd
$CMD dumpvd tvset.hdf
$CMD dumpvd -i 1,3,5 tvset.hdf
$CMD dumpvd -r 1238,1239,1251,1252 tvset.hdf
$CMD dumpvd -n "Multi-Order Vdata" tvset.hdf
$CMD dumpvd -n "Mixed Vdata","Integer Vdata" tvset.hdf
$CMD dumpvd -c "Test object","No class specified" tvset.hdf
$CMD dumpvd -f B tvset.hdf
$CMD dumpvd -f "STATION_NAME","FLOATS" tvset.hdf
$CMD dumpvd -f "STATION_NAME","FLOATS" -d tvset.hdf
$CMD dumpvd tvattr.hdf

# Test command dumpvg
$CMD dumpvg tvset.hdf
$CMD dumpvg -i 0,1 tvset.hdf
$CMD dumpvg -r 3 tvset.hdf
$CMD dumpvg -n "Simple Vgroup" tvset.hdf
$CMD dumpvg -c "Test object" tvset.hdf
$CMD dumpvg -i 1,3,5 tdata.hdf
$CMD dumpvg -r 32,39 tdata.hdf
$CMD dumpvg -n nsamp,tdata.hdf tdata.hdf
$CMD dumpvg -c CDF0.0 tdata.hdf
$CMD dumpvg -c Dim0.0,Var0.0 tdata.hdf
$CMD dumpvg -c Dim0.0,Var0.0 -d tdata.hdf
$CMD dumpvg tvattr.hdf

#Test command dumpgr
$CMD dumpgr grtdfui82.hdf
$CMD dumpgr -i 0,1,3 grtdfui82.hdf
$CMD dumpgr -i 0 grtdfui82.hdf
$CMD dumpgr -n Image_array_5 grtdfui82.hdf
$CMD dumpgr -r 6,2,3 grtdfui82.hdf
$CMD dumpgr -r 6 -d  grtdfui82.hdf
$CMD dumpgr -o my.dat grtdfui82.hdf
$RM my.dat
$CMD dumpgr -o mybin.dat  -b grtdfui82.hdf
$RM mybin.dat
$CMD dumpgr grtdfui83.hdf
$CMD dumpgr grtdfui84.hdf
$CMD dumpgr grtdfui162.hdf
$CMD dumpgr grtdfi322.hdf
