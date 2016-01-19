#!/bin/bash
red=$(tput setaf 1)
green=$(tput setaf 2)
yellow=$(tput setaf 3)
reset=$(tput sgr0)
pass=0
fail=1

##################################################
# TESTING CONTENTS OF lib/libhdf4.settings file.
##################################################

if [ ! -f ./lib/libhdf4.settings ]; then
   echo " $red FAILED$reset: ./lib/libhdf4.settings  DOES NOT EXIST"
   exit
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
