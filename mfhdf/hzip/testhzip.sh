#! /bin/sh
# Tests for the hdiff tool

HZIP=hzip                 # The tool name
HZIP_BIN=`pwd`/$HZIP      # The path of the tool binary

CMP='cmp -s'
DIFF='diff -c'

nerrors=0
verbose=yes

# The build (current) directory might be different than the source directory.
if test -z "$srcdir"; then
   srcdir=.
fi

test -d testfiles || mkdir testfiles

# Print a line-line message left justified in a field of 70 characters
# beginning with the word "Testing".
#
TESTING() {
   SPACES="                                                               "
   echo "Testing $* $SPACES" | cut -c1-70 | tr -d '\012'
}

# Run a test and print PASS or *FAIL*.  If a test fails then increment
# the `nerrors' global variable and (if $verbose is set) display the
# difference between the actual output and the expected output. The
# expected output is given as the first argument to this function and
# the actual output file is calculated by replacing the `.ddl' with
# `.out'.  The actual output is not removed if $HDF5_NOCLEANUP has a
# non-zero value.
#
TOOLTEST() {
   expect="$srcdir/testfiles/$1"
   actual="testfiles/`basename $1 .txt`.out"
   actual_err="testfiles/`basename $1 .txt`.err"
   shift

   # Run test.
   # Tflops interprets "$@" as "" when no parameter is given (e.g., the
   # case of missing file name).  Changed it to use $@ till Tflops fixes it.
   TESTING $HDIFF $@
   (
      echo "#############################"
      echo "Expected output for '$HDIFF $@'" 
      echo "#############################"
      cd $srcdir/testfiles
      if [ "`uname -s`" = "TFLOPS O/S" ]; then
        $RUNSERIAL $HDIFF_BIN $@
      else
        $RUNSERIAL $HDIFF_BIN "$@"
      fi
   ) >$actual 2>$actual_err
   cat $actual_err >> $actual

   if [ ! -f $expect ]; then
   # Create the expect file if it doesn't yet exist.
      echo " CREATED"
      cp $actual $expect
   elif $CMP $expect $actual; then
      echo " PASSED"
   else
      echo "*FAILED*"
      echo "    Expected result (*.txt) differs from actual result (*.out)"
      nerrors="`expr $nerrors + 1`"
      test yes = "$verbose" && $DIFF $expect $actual |sed 's/^/    /'
   fi

   # Clean up output file
     if test -z "$HDF5_NOCLEANUP"; then
     rm -f $actual $actual_err
     fi
}

##############################################################################
##############################################################################
###			  T H E   T E S T S                                            ###
##############################################################################
##############################################################################


# just run the program hziptst; this has several runs with 
# different compression and chunking options
./hziptst 



# ##############################################################################
# # END
# ##############################################################################



if test $nerrors -eq 0 ; then
   echo "All $HDIFF tests passed."
fi

exit $nerrors
