#!/usr/bin/perl
# makeInternalhdf4README.pl
use warnings;
use strict;

my $section2="For more information on the options that were used to compile
these binaries, see the following file:

    lib/libhdf4.settings

Contents of this directory:

    COPYING     - Copyright notice
    README      - This file
    RELEASE.txt - Release notes
    bin/        - HDF utilities
    include/    - Include files
    lib/        - HDF libraries
    share/      - Man pages (for ncdump, ncgen...)

These binaries were built with the following external libraries 
which are included in the lib directory for convenience.

    ZLIB library (version 1.2.5)";

my $section3 = "    SZIP library (version 2.1) with the Encoder enabled

We also provide the ZLIB, SZIP, and JPEG 6b source code on our ftp server at:

    ftp://ftp.hdfgroup.org/lib-external/

NOTE: The software can be accessed by specifying either
\"http://www.hdfgroup.org/ftp/\" or \"ftp://ftp.hdfgroup.org/\".
   
For information regarding each of these libraries, see:

    ZLIB:  http://www.zlib.net/
    JPEG:  (See the JPEG source code) 
    SZIP:  http://hdfgroup.org/doc_resource/SZIP/
 
";

my $section5 = "Scripts for compiling applications are included with the binaries:

    bin/h4cc  - for C
    bin/h4fc  - for Fortran 

After you have installed the binaries to their final directory, then:

    cd to the bin/ directory 
    run ./h4redeploy and enter yes to the question  

Each compile script has site specific paths in it, and in many cases,
./h4redeploy will change the paths to run on your system.  However, 
you should still check the compile script, and if need be, modify the 
variables (or set them as environment variables) to work on your system.  
Here are some of the variables to check:

  prefix      - Path to the HDF top level installation directory
  CCBASE      - Name of the alternative C compiler
  CLINKERBASE - Name of the alternative linker
  LDFLAGS     - Path to different libraries your application will link 
                with (this path should include the path to the ZLIB 
                library)
  LIBS        - Libraries your application will link with

You can use the -show option with the compile scripts to see if the paths
being used are correct. For example:

    h4cc -show

If these binaries do not work for you, you can compile and install
HDF yourself.  The HDF source code can be found in:

    http://www.hdfgroup.org/ftp/HDF/HDF_Current/src/

NOTE: The software can be accessed by specifying either
\"http://www.hdfgroup.org/ftp/\" or \"ftp://ftp.hdfgroup.org/\".

To build from source, refer to the INSTALL file in the top 
directory of the HDF source code.

Please send questions, comments, and suggestions to The HDF Group 
Helpdesk.  Contact information can be found at:

   http://www.hdfgroup.org/about/contact.html

";


my $indirectory = ".";
$indirectory = shift;
my $linktype = "shared";
if ($indirectory =~ /static/) {
   $linktype = "static";
}

 my $version;

my $outfile = "$indirectory/README";
open OUTFILE, ">$outfile" or die "$!Couldn't open $outfile - check permissions for $indirectory\n";
my $hostname;
my $cmd = "grep \"HDF4 Version\" $indirectory/lib/libhdf4.settings";
$_ = `$cmd`;
#print OUTFILE $_, "\n";
s/HDF4 Version://;
s/^\s+//;
chomp;
$version = $_;
#print OUTFILE $_, "\n";
my $versionstring=  "This directory contains the binary distribution of HDF-".$version;
$cmd = "grep \"Uname information:\" $indirectory/lib/libhdf4.settings";
$_ = `$cmd`;
s/Uname information://;
s/^\s+//;
#print OUTFILE $_;
chomp;
my $platformstring = "";
my $hostnamestring = $_;
#s/(^\w+)(\s)(\S+)/$1/;
#s/(^.*)(-)(.*)(200[7-8])(.*)(\s)(\S+)/$1 $5/;
#my $platformstring = "\nthat was compiled on " . $_ . " ";
print "Working with $hostnamestring.\n";
my @hostnamestring = split / /, $hostnamestring;
#print OUTFILE "Size of hostnamestring is ", scalar @hostnamestring, "\n";
#print OUTFILE $hostnamestring[0] . "\t" . $hostnamestring[2]."\t".$hostnamestring[19]."\n";
$hostname = $hostnamestring[1];
#my $size = scalar @hostnamestring;
if ($hostname =~ /loyalty/) {
   $platformstring = "\nthat was compiled on " . $hostnamestring[0]." " . $hostnamestring[2]." " . $hostnamestring[-1] . " ";
} elsif ($hostname =~ /freedom/) {
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0]." " . $hostnamestring[2]." " . $hostnamestring[-1] . " ";
} elsif ($hostname =~ /emu/) {
   $platformstring = "\nthat was compiled on " . $hostnamestring[0]." " . $hostnamestring[2]." " . $hostnamestring[-2] . " ";
} elsif ($hostname =~ /osx1011test/) {
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0]." " . $hostnamestring[2] . " " . $hostnamestring[-1] . " ";
} elsif ($hostname =~ /osx1010test/) {
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0]." " . $hostnamestring[2] . " " . $hostnamestring[-1] . " ";
} elsif ($hostname =~ /quail/) {
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0]." " . $hostnamestring[2] . " " . $hostnamestring[-1] . " ";
} elsif ($hostname =~ /kite/) {
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0]." " . $hostnamestring[2] . " " . $hostnamestring[-1] . " ";
} else {
   $_ = $hostnamestring[2];
   my $pos = index $_, '-';
   my $os = substr $_, 0, $pos;
   $platformstring = "\nthat was compiled on: " . $hostnamestring[0] . " " . $os . " " . $hostnamestring[-2] . " ";
}


my $mappingstring = ""; 
#$cmd = "grep \"Default API Mapping:\" $indirectory/lib/libhdf4.settings";
#$_ = `$cmd`;
#s/Default API Mapping://;
#s/^\s+//;
#chomp;
#if (/v\d+/) {
#   s/v//;
#   s/(\d)(\d)/$1\.$2/g;
#   $mappingstring = "using the default\nAPI mapping for VERSION ".$_.".";
#   
#} 
print OUTFILE $versionstring;
print OUTFILE $platformstring."\n\n";
#print OUTFILE $mappingstring;

#The default fortran is broken on duty and liberty.  When it's fixed, just use the lines from the else section.
print OUTFILE "These binaries were built with the following C and Fortran\n";
print OUTFILE "compilers:\n\n";


# Only the gcc compiler version is in libhdf4.settings, so for now I looked 
# up the versions and hardcoded them here.
if ($hostname =~ /jam/ || $hostname =~ /koala/) {
   print OUTFILE "\tgcc and gfortran 4.1.2\n\n";
} elsif ($hostname =~ /moohan/) {
   print OUTFILE "\tgcc and gfortran 4.8.5\n\n";
} elsif ($hostname =~ /platypus/) {
   print OUTFILE "\tgcc and gfortran 4.4.7\n\n";
} elsif ($hostname =~ /emu/) {
   print OUTFILE "\tSun C 5.12, Sun Fortran 95 8.6\n\n";
} elsif ($hostname =~ /ostrich/) {
   print OUTFILE "\tgcc 4.4.7 and GNU Fortran 4.4.7\n\n";
} elsif ($hostname =~ /loyalty/ || $hostname =~ /freedom/) {
   print OUTFILE "\tgcc, g++, and gfortran 4.6.1\n\n";
} elsif ($hostname =~ /kite/) {
   print OUTFILE "\tApple clang v5.1 from Xcode 5.1 and GNU Fortran 4.8.2\n\n";
} elsif ($hostname =~ /quail/) {
   print OUTFILE "\tApple clang v6.0 from Xcode 6.2.0 and GNU Fortran 4.9.2\n\n";
} elsif ($hostname =~ /osx1010test/) {
   print OUTFILE "\tApple clang v6.1 from Xcode 7.0.0 and GNU Fortran 4.9.2\n\n";
} elsif ($hostname =~ /osx1011test/) {
   print OUTFILE "\tApple clang v7.0 from Xcode 7.0.3 and GNU Fortran 4.9.2\n\n";
}

print OUTFILE $section2;

if ($hostname =~ /osx1011test/ || $hostname =~ /osx1010test/ || $hostname =~ /kite/ || $hostname =~ /quail/) {
print OUTFILE "    JPEG library (version 8d)\n";
} else {
print OUTFILE "    JPEG library (version 6b)\n";
}

print OUTFILE $section3;

print OUTFILE $section5;

