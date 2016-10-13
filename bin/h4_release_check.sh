#!/bin/bash
red=$(tput setaf 1)
green=$(tput setaf 2)
yellow=$(tput setaf 3)
magenta=$(tput setaf 5)
reset=$(tput sgr0)
pass=0
fail=1

##################################################
# TESTING CONTENTS OF lib/libhdf4.settings file.
##################################################

if [ ! -f ./lib/libhdf4.settings ]; then
   echo " $red FAILED$reset: ./lib/libhdf4.settings  DOES NOT EXIST"
   exit 1
fi

# CHECKING FOR SHARED LIBRARIES
if grep -q "Shared Libraries: no" ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- Shared Libraries  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- Shared Libraries  ... $red FAILED$reset"
   echo "  ... Should be 'Shared Libraries: no'"
fi

# CHECKING FOR STATIC LIBRARIES
if grep -q "Static Libraries: yes" ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- Static Libraries  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- Shared Libraries  ... $red FAILED$reset"
   echo "  ... Should be 'Static Libraries: yes'"
fi

# CHECKING FOR FORTRAN ENABLED
if grep -q "Fortran: yes" ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- Fortran Support   ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- Fortran Support   ... $red FAILED$reset"
   echo "  ... Should be 'Fortran: yes'"
fi

# CHECKING COMPILATION MODE
if grep -q "Compilation Mode: production" ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- Compilation Mode  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- Compilation Mode  ... $red FAILED$reset"
   echo "  ... Should be 'Compilation Mode: production'"
fi

# CHECKING OPTIMIZATION
if grep -q " -g " ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- Optimization Mode ... $red FAILED$reset"
   echo "  ... Should be no there is no '-g' flag among compiler flags"
else
   echo "Checking ./lib/libhdf4.settings -- Optimization Mode ... $green PASSED$reset"
fi

#CHECKING COMPRESSION
if grep -q " -ljpeg " ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- JPEG COMPRESSION  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- JPEG COMPRESSION  ... $yellow WARNING$reset"
   echo "  ... JPEG compression was not found"
fi
if grep -q " -lz " ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- ZLIB COMPRESSION  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- ZLIB COMPRESSION  ... $red FAILED$reset"
   echo "  ... ZLIB compression is required"
fi

if grep -q "SZIP compression: enabled" ./lib/libhdf4.settings; then
   echo "Checking ./lib/libhdf4.settings -- SZIP COMPRESSION  ... $green PASSED$reset"
else
   echo "Checking ./lib/libhdf4.settings -- SZIP COMPRESSION  ... $red FAILED$reset"
   echo "  ... SZIP compression should be enabled"
fi



#########################
# TESTING IF FILES EXISTS
#########################

include_file=(atom.h bitvect.h cdeflate.h cnbit.h cnone.h crle.h cskphuff.h cszip.h dfan.h
dffunc.f90 dffunc.inc dfgr.h df.h dfi.h dfrig.h dfsd.h dfstubs.h dfufp2i.h dynarray.h H4api_adpt.h
h4config.h hbitio.h hchunks.h hcomp.h hcompi.h hconv.h hdatainfo.h hdf2netcdf.h hdf.f90
hdf.h hdfi.h hdf.inc herr.h hfile.h hkit.h hlimits.h hntdefs.h hproto_fortran.h hproto.h
htags.h linklist.h local_nc.h mfan.h mfdatainfo.h mffunc.f90 mffunc.inc mfgr.h mfhdf.h
mfhdfi.h mstdio.h netcdf.f90 netcdf.h netcdf.inc tbbt.h vattr.h vg.h)

lib_file=(libdf.a libdf.la libhdf4.settings libjpeg.a libmfhdf.a libmfhdf.la libsz.a libz.a)

bin_file=(gif2hdf h4cc h4fc h4redeploy hdf24to8 hdf2gif hdf2jpeg hdf8to24 hdfcomp
hdfed hdfimport hdfls hdfpack hdftopal hdftor8 hdfunpac hdiff hdp hrepack jpeg2hdf
ncdump ncgen paltohdf r8tohdf ristosds vmake vshow)

# TEST TO MAKE SURE THE DIRECTORIES EXISTS

if [ ! -d ./include ]; then
   echo " FAILED: ./include DIRECTORY DOES NOT EXIST"
   exit
fi
if [ ! -d ./bin ]; then
   echo " FAILED: ./bin DIRECTORY DOES NOT EXIST"
   exit
fi
if [ ! -d ./lib ]; then
   echo " FAILED: ./lib DIRECTORY DOES NOT EXIST"
   exit
fi

# TESTING FOR INCLUDE FILES

status=$pass
for var in "${include_file[@]}"
do
  if [ ! -f ./include/$var ]; then
      echo "FILE$red ./include/${var}$reset DOES NOT EXIST"
      status=$fail
  fi
done

if [ "$status" -eq "$fail" ]; then
  echo "Testing for include files ... $red FAILED $reset"
else
  echo "Testing for include files ... $green PASSED $reset"
fi

# TESTING FOR LIB FILES
status=$pass
for var in "${lib_file[@]}"
do
  if [ ! -f ./lib/$var ]; then
      echo "FILE$red ./lib/${var}$reset DOES NOT EXIST"
      status=$fail
  fi
done

if [ "$status" -eq "$fail" ]; then
  echo "Testing for lib files     ... $red FAILED $reset"
else
  echo "Testing for lib files     ... $green PASSED $reset"
fi

# TESTING FOR BIN FILES
status=$pass
for var in "${bin_file[@]}"
do
  if [ ! -f ./bin/$var ]; then
      echo "FILE$red ./bin/${var}$reset DOES NOT EXIST"
      status=$fail
  fi
done

if [ "$status" -eq "$fail" ]; then
  echo "Testing for bin files     ... $red FAILED $reset"
else
  echo "Testing for bin files     ... $green PASSED $reset"
fi

# Check settings in libhdf4.settings match what is in RELEASE.txt

# Check the OS type

UnameInfo=`grep -i 'Uname information:' lib/libhdf4.settings`
OStype=`echo $UnameInfo | cut -d ":" -f 2 | cut -d " " -f 4`

# This code will change the OStype to remove all the code between 
# the "- first number" and ".el". For example 2.6.32-573.22.1.el6.x86_64 
# will become 2.6.32-573.el6.x86_64
#
#OSsize=`echo ${OStype} | cut -d "e" -f 2`
#OSsize=`echo .e${OSsize}`
#
#kernel=`echo ${OStype} | cut -d "-" -f 1`
#vendor_kernel=`echo ${OStype} | cut -d "-" -f 2 | cut -d "." -f 1`
#OStype=`echo ${kernel}-${vendor_kernel}${OSsize}`

libCC=`grep -i 'C compiler' lib/libhdf4.settings`
libFC=`grep -i 'Fortran Compiler' lib/libhdf4.settings`

IFS='%'

if grep -i -q "$OStype" RELEASE.txt; then
  echo "Testing for OS type       ... $green PASSED $reset"
# Check the processor  
    line_start=`grep -in "$OStype" RELEASE.txt | cut -d : -f 1`

# print to screen to have the user check compiler versions
 echo "$yellow      __      ________ _____  _____ ________     __ $reset"
 echo "$yellow      \ \    / /  ____|  __ \|_   _|  ____\ \   / / $reset"
 echo "$yellow       \ \  / /| |__  | |__) | | | | |__   \ \_/ /  $reset"
 echo "$yellow        \ \/ / |  __| |  _  /  | | |  __|   \   /   $reset"
 echo "$yellow         \  /  | |____| | \ \ _| |_| |       | |    $reset"
 echo "$yellow          \/   |______|_|  \_\_____|_|       |_|    $reset"
 echo ""
                                              

    echo "$yellow  (1) VERIFY: C Compiler in libhdf4.settings: $reset"
    echo "  $libCC"
    echo "$yellow              should match a compiler listed in RELEASE.txt $reset"
    line_end=$line_start-2
    sed -n "$line_start,/^$/p" RELEASE.txt |
    {
    while read line; do
      ((line_end++))
      ## counter=$[$line_endcounter +1]
    done
    sed -n $line_start,${line_end}p RELEASE.txt
    }
else
  echo "Testing for OS type       ... $red FAILED $reset"
  echo "  Operating system $OStype not found in RELEASE.txt"
fi

echo ""
echo "$yellow  (2) VERIFY: Binary executables do not requre any external $reset"
echo "$yellow      libraries (except system libraries on some machines): $reset"

echo `echo "      Result from ${magenta}ldd bin/hdp${reset}:  "; ldd bin/hdp`
echo ""


echo ""
echo "$yellow  (3) VERIFY: Binary executables match libhdf4.settings $reset"

echo `echo "    Result from ${magenta}file bin/hdp${reset}:  "; file bin/hdp`
echo `echo "    Result from lib/libhdf4.settings:  "; echo ${UnameInfo}`
echo ""
