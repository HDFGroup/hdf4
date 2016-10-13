#!/usr/bin/perl
# makeOuterhdf4README.pl
#
# Copyright by The HDF Group.
# # All rights reserved.
# #
# # This file is part of HDF4.  The full HDF4 copyright notice, including
# # terms governing use, modification, and redistribution, is contained in
# # the files COPYING and Copyright.html.  COPYING can be found at the root
# # of the source code distribution tree; Copyright.html can be found at the
# # root level of an installed copy of the electronic HDF4 document set and
# # is linked from the top-level documents page.  It can also be found at
# # http://hdfgroup.org/HDF4/doc/Copyright.html.  If you do not have
# # access to either file, you may request a copy from help@hdfgroup.org.
# #

use warnings;
use strict;


my $indirectory = ".";
$indirectory = shift;

my $outdirectory = ".";
$outdirectory = shift;

my $outsubdir = shift;

my $version;

my $outfile = "$outdirectory/$outsubdir/README";
print "Attempting to open $outfile.\n";
open OUTFILE, ">$outfile" or die "Couldn\'t $!\n";
my $hostname;
my $cmd = "grep \"HDF4 Version\" $indirectory-static/lib/libhdf4.settings";
$_ = `$cmd`;
#print OUTFILE $_, "\n";
s/HDF4 Version://;
#my $cmd = "grep \"Configured by:\" $indirectory/lib/libhdf4.settings";
#$_ = `$cmd`;
#s/Configured by:\shdftest@//;
s/^\s+//;
chomp;
$version = $_;
#print OUTFILE $_, "\n";
my $versionstring=  "This directory contains the precompiled HDF $version binary distribution\n(include files, libraries, utilities) for ";

$cmd = "grep \"Uname information:\" $indirectory-static/lib/libhdf4.settings";
$_ = `$cmd`;
s/Uname information://;
s/^\s+//;
chomp;

print "String to work with is $_\n";
my $platformstring = "";
my $hostnamestring = $_;
my @hostnamestring = split / /, $hostnamestring;
$platformstring = "$hostnamestring[0] ";
if ($indirectory =~ /jam/ || $indirectory =~ /koala/ || $indirectory =~ /ostrich/) {
   $hostnamestring = $hostnamestring[2];
   my $pos = index $hostnamestring, "-";
   if ($pos > 0) {
      $platformstring .=  substr $hostnamestring, 0, $pos;
   } else {
      $platformstring .= $hostnamestring[2];
   }
   $platformstring .= " ".$hostnamestring[-3];
} elsif ($indirectory =~ /emu/) {
   $platformstring .= "$hostnamestring[2] $hostnamestring[-2]";
} else {
   $platformstring .= "$hostnamestring[2] $hostnamestring[-1]";
}

print OUTFILE $versionstring." ".$platformstring.":\n\n";
my $line1;
my $line3;
my $line5 = "";
my $compilerstring="";

print $indirectory, "\n";
$line3 = "  hdf-$version-$outsubdir.tar.gz  - Includes C and Fortran APIs (using\n";

if ($indirectory =~ /ostrich/) {
   $compilerstring = "                                   gcc and gfortran 4.4.7)\n";
}
elsif ($indirectory =~ /moohan/) {
   $compilerstring = "                                   gcc and gfortran 4.8.5)\n";
}
elsif ($indirectory =~ /platypus/) {
   $compilerstring = "                                   gcc and gfortran 4.4.7)\n";
}
elsif ($indirectory =~ /emu/) {
   $compilerstring = "                              Sun C 5.12, Sun Fortran 95 8.6)\n";
}
elsif ($indirectory =~ /kite/) {
   $compilerstring = "                              Apple clang 5.1 from Xcode 5.1 and GNU Fortran 4.8.2)\n";
}
elsif ($indirectory =~ /quail/) {
   $compilerstring = "                              Apple clang 6.0 from Xcode 6.2.0 and GNU Fortran 4.9.2)\n";
}
elsif ($indirectory =~ /osx1010test/) {
   $compilerstring = "                              Apple clang 6.1 from Xcode 7.0.0 and GNU Fortran 4.9.2)\n";
}
elsif ($indirectory =~ /osx1010test/) {
   $compilerstring = "                              Apple clang 7.0 from Xcode 7.0.3 and GNU Fortran 4.9.2)\n";
}

    print OUTFILE $line3;
    print OUTFILE $compilerstring."\n";

print OUTFILE "  utilities/ - Directory containing the compiled HDF utilities.\n";
print OUTFILE "               These utilities are STATICALLY linked and will run as is.\n\n";
#print OUTFILE "             The tools can be downloaded separately, or ALL in one\n";
#print OUTFILE "             compressed file (5-$version-$outsubdir-static-util.tar.gz).\n\n";
print OUTFILE "\n";
print OUTFILE "To obtain the HDF distribution, 'unzip' and 'untar' the distribution\n";
print OUTFILE "file:\n\n";
print OUTFILE "  gzip -cd <gz file from above> | tar xvf -\n";


#$cmd = "grep \"Configured by:\" $indirectory/$key-static/lib/libhdf4.settings";
#$_ = `$cmd`;
#s/Configured by://;
#s/^\s+//;
#print OUTFILE $_;
#chomp;
#my $hostnamestring = $_;
#s/(^\w+)(\s)(\S+)/$1/;
#s/(^.*)(-)(.*)(200[7-8])(.*)(\s)(\S+)/$1 $5/;
#my $platformstring = $_ . ":\n\n";
#my @hostnamestring = split /@/, $hostnamestring;
#print "Size of hostnamestring is ", scalar @hostnamestring, "\n";
#print $hostnamestring[0] . "\t" . $hostnamestring[2]."\t".$hostnamestring[19]."\n";
#my $platformstring = $hostnamestring[1].":\n\n";
#$hostnamestring = $hostnamestring[1];
#my $pos = index $hostnamestring, ".";
#if ($pos > 0) {
#   @hostnamestring = split /\./, $hostnamestring;
#   $platformstring = $hostnamestring[0].":\n\n";
#}


#my $hostnamestring = $_;
#print "Hostnamestring: $hostnamestring.\n";
##s/(^\w+)(\s)(\S+)/$1/;
##s/(^.*)(-)(.*)(200[7-8])(.*)(\s)(\S+)/$1 $5/;
#my $platformstring = $_ . "\n\n";
#my @hostnamestring = split / /, $hostnamestring;
##print OUTFILE "Size of hostnamestring is ", scalar @hostnamestring, "\n";
##print OUTFILE $hostnamestring[0] . "\t" . $hostnamestring[2]."\t".$hostnamestring[19]."\n";
#$hostname = $hostnamestring[1];
##my $size = scalar @hostnamestring;
#if ($hostname eq "duty.hdfgroup.uiuc.edu") {
#   $platformstring = $hostnamestring[0]." " . $hostnamestring[2]." " . $hostnamestring[-1] . "\n\n";
#}
#elsif ($hostname eq "linew.hdfgroup.uiuc.edu") {
#   $platformstring = $hostnamestring[0]." " . $hostnamestring[2]." " . $hostnamestring[-3]." " . $hostnamestring[-2] . "\n\n";
#}
#
#print OUTFILE $versionstring, $platformstring;
#my $line1;
#my $line3;
#my $compilerstring;
#
#if ($hostname eq "duty.hdfgroup.uiuc.edu") {
#   $line1 = "  5-$version-$outsubdir-16API.tar.gz - Includes C and C++ APIs (using\n";
#   $line3 = "  5-$version-$outsubdir.tar.gz       - Includes C and C++ APIs (using\n";
#   $compilerstring = "                              gcc and g++ 3.4.6)\n";
#}
#elsif ($hostname eq  "kagiso") {
#   $line1 = "  5-$version-$outsubdir-16API.tar.gz - Includes C, C++, F90 APIs (using\n";
#   $line3 = "  5-$version-$outsubdir.tar.gz       - Includes C, C++, F90 APIs (using\n";
#   $compilerstring = "                              gcc & g++ 3.4.6, and g95 0.91!)\n";
#}
#elsif ($hostname eq  "smirom") {
#   $line1 = "  5-$version-$outsubdir-16API.tar.gz - Includes C, C++, F90 APIs (using\n";
#   $line3 = "  5-$version-$outsubdir.tar.gz       - Includes C, C++, F90 APIs (using\n";
#   $compilerstring = "                              pgcc, pgCC, and pgf90 7.1-6\n";
#}
#elsif ($hostname eq  "linew.hdfgroup.uiuc.edu") {
#   $line1 = "  5-$version-$outsubdir-16API.tar.gz - Includes C, C++, F90 APIs (using\n";
#   $line3 = "  5-$version-$outsubdir.tar.gz       - Includes C, C++, F90 APIs (using\n";
#   $compilerstring = "                              Sun C and C++ 5.8, Sun Fortran 95 8.2)\n";
#}
#print OUTFILE $line1;
#print OUTFILE $compilerstring;
#print OUTFILE "                               USES THE VERSION 1.6 DEFAULT API\n";
#print OUTFILE "                               MAPPING.\n\n";
#
#print OUTFILE $line3;
#print OUTFILE $compilerstring."\n";
#
#print OUTFILE "  utilities/ - Directory containing the compiled HDF4 utilities.\n";
#print OUTFILE "             These utilities are STATICALLY linked and will run as is.\n\n";
#print OUTFILE "             The tools can be downloaded separately, or ALL in one\n";
#print OUTFILE "             compressed file (5-$version-$outsubdir-static-util.tar.gz).\n\n";
#print OUTFILE "To obtain the HDF4 distribution, 'unzip' and 'untar' the distribution\n";
#print OUTFILE "file:\n\n";
#print OUTFILE "  gzip -cd <gz file from above> | tar xvf -";

