#!/usr/bin/perl
# makeTarFiles.pl
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
use Cwd;

# usage:
# run in the pkgscripts directory because this script uses the other scripts in the directory
# run with absolute path
# ./makeHDF4BinaryTarfiles.pl <input directory> <output directory>
# where <input directory> contains HDF4 installs with keynames in %destsubdir
# below and tar files with names based on matching values in  %destsubdir
# will be created in <output directory>

my $key;

my %destsubdir = ('duck' => 'mac-lion-x86_64',
                  'emu' => 'solaris',
                  'emu64' => 'solaris64',
                  'freedom' => 'freebsd-amd64',
                  'jam' => 'linux',
                  'jamgnu484' => 'linux-gnu484',
                  'koala' => 'linux-x86_64',
                  'koalagnu484' => 'linux-x86_64-gnu484',
                  'loyalty' => 'freebsd',
                  'ostrich' => 'linux-ppc64',
                  'ostrichxl' => 'linux-ppc64xl',
                  'platypus' => 'linux-centos6-x86_64',
                  'platypus32' => 'linux-centos6-x86_64-32',
                  'moohan' => 'linux-centos7-x86_64',
                  'moohan32' => 'linux-centos7-x86_64-32',
                  'hdf-duck' => 'mac-lion-x86_64',
                  'kite' => 'mac-mountainlion-x86_64',
                  'quail' => 'mac-mavericks-x86_64',
                  'osx1010test' => 'mac-yosemite-x86_64',
                  'osx1011test' => 'mac-elcapitan-x86_64');

my %jpegdir = ('jam' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-gcc',
               'jam-static' => '/mnt/hdf/packages/static/jpeg/Linux2.6-gcc',
               'koala' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-x86_64-gcc',
               'koala-static' => '/mnt/hdf/packages/jpeg/Linux2.6-x86_64',
               'moohan' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-x86_64-gcc',
               'moohan32' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-x86_64-gcc-32',
               'moohan-static' => '/mnt/hdf/packages/jpeg/Linux2.6-x86_64',
               'moohan32-static' => '/mnt/hdf/packages/jpeg/Linux2.6-x86_64-32',
               'platypus' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-x86_64-gcc',
               'platypus32' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-x86_64-gcc',
               'platypus-static' => '/mnt/hdf/packages/jpeg/Linux2.6-x86_64',
               'platypus32-static' => '/mnt/hdf/packages/jpeg/Linux2.6-x86_64-32',
               'emu' =>  '/mnt/hdf/packages/jpeg-PIC/SunOS-5.10',
               'emu-static' => '/mnt/hdf/packages/static/jpeg/SunOS-5.10',
               'emu64' =>  '/mnt/hdf/packages/jpeg-PIC/SunOS-5.11-64',
               'emu64-static' => '/mnt/hdf/packages/jpeg-PIC/SunOS-5.11-64',
               'ostrich' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-ppc64-gcc-64',
               'ostrich-static' => '/mnt/hdf/packages/jpeg-PIC/Linux2.6-ppc64-gcc-64');

my %szipdir = ('duck' => '/mnt/hdf/packages/szip/shared/encoder/macOS-10.8',
               'duck-static' => '/mnt/hdf/packages/szip/static/encoder/mac-intel-x86_64',
               'emu' =>  '/mnt/hdf/packages/szip/shared/encoder/SunOS-5.10',
               'emu-static' => '/mnt/hdf/packages/szip/static/encoder/SunOS-5.10',
               'emu64' => '/mnt/hdf/packages/szip-PIC/shared/encoder/SunOS-5.11-64',
               'emu64-static' => '/mnt/hdf/packages/szip-PIC/shared/encoder/SunOS-5.11-64',
               'freedom' => '/mnt/hdf/packages/szip/shared/encoder/FreeBSD-64',
               'freedom-static' => '/mnt/hdf/packages/szip/static/encoder/FreeBSD-64',
               'hdf-duck' => '/mnt/hdf/packages/szip/shared/encoder/MacOS-10.8',
               'hdf-duck-static' => '/mnt/hdf/packages/szip/static/encoder/mac-intel-x86_64',
               'jam' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-gcc',
               'jamgnu484' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-gcc',
               'jam-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-gcc',
               'jamgnu484-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-gcc',
               'kite' => '/mnt/hdf/packages/szip/shared/encoder/MacOS-10.8',
               'kite-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/MacOS-10.8',
               'jam-new' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-gcc',
               'jam-new-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-gcc',
               'loyalty' => '/mnt/hdf/packages/szip/shared/encoder/FreeBSD',
               'loyalty-static' => '/mnt/hdf/packages/szip/static/encoder/FreeBSD',
               'ostrich32' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-ppc64-gcc',
               'ostrich32-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-ibmppc64-gcc-32',
               'ostrich' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-ppc64-gcc-64',
               'ostrich-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-ibmppc64-gcc',
               'ostrichxl' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-ppc64-gcc-64',
               'ostrichxl-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-ibmppc64-gcc',
               'osx1010test' => '/mnt/hdf/packages/szip/shared/encoder/MacOS-10.8',
               'osx1010test-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/MacOS-10.8',
               'osx1011test' => '/mnt/hdf/packages/szip/shared/encoder/MacOS-10.8',
               'osx1011test-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/MacOS-10.8',
               'moohan' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-x86_64-gcc',
               'moohan-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-x86_64-gcc',
               'moohan32' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-x86_64-gcc-m32',
               'moohan32-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/Linux2.6-x86_64-gcc-m32',
               'platypus32' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-x86_64-gcc-m32',
               'platypus32-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/Linux2.6-x86_64-gcc-m32',
               'platypus' => '/mnt/hdf/packages/szip/shared/encoder/Linux2.6-x86_64-gcc',
               'platypus-static' => '/mnt/hdf/packages/szip/static/encoder/Linux2.6-x86_64-gcc',
               'quail' => '/mnt/hdf/packages/szip/shared/encoder/MacOS-10.8',
               'quail-static' => '/mnt/hdf/packages/szip-PIC/static/encoder/MacOS-10.8');


my %zlibdir = ('duck' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'duck-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'emu' => '/mnt/hdf/packages/zlib-125/shared/SunOS-5.10',
               'emu-static' => '/mnt/hdf/packages/zlib-125/static/SunOS-5.10',
               'emu64' => '/mnt/hdf/packages/zlib-123-PIC/SunOS-5.11-64',
               'emu64-static' => '/mnt/hdf/packages/zlib-123-PIC/SunOS-5.11-64',
               'fred' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'fred-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'freedom' => '/mnt/hdf/packages/zlib-125/shared/FreeBSD-64',
               'freedom-static' => '/mnt/hdf/packages/zlib-125/static/FreeBSD-64',
               'hdf-duck' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'hdf-duck-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'jam' => '/mnt/hdf/packages/zlib-125/shared/Linux2.6-gcc',
               'jamgnu482' => '/mnt/hdf/packages/zlib-125/shared/Linux2.6-gcc',
               'jam-static' => '/mnt/hdf/packages/zlib-125/static/Linux2.6-gcc',
               'jamgnu482-static' => '/mnt/hdf/packages/zlib-125/static/Linux2.6-gcc',
               'kite' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'kite-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'jam-new' => '/mnt/hdf/packages/zlib-125/shared/Linux2.6-gcc',
               'jam-new-static' => '/mnt/hdf/packages/zlib-125/static/Linux2.6-gcc',
               'loyalty' => '/mnt/hdf/packages/zlib-125/shared/FreeBSD',
               'loyalty-static' => '/mnt/hdf/packages/zlib-125/static/FreeBSD',
               'ostrich32' => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc',
               'ostrich32-static' => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc',
               'ostrich' => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc-64',
               'ostrich-static'  => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc-64',
               'ostrichxl' => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc-64',
               'ostrichxl-static'  => '/mnt/hdf/packages/zlib-125/PIC/Linux2.6-ppc64-gcc-64',
               'osx1010test' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'osx1010test-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'osx1011test' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'osx1011test-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64',
               'moohan' => '/mnt/hdf/packages/zlib-125/shared/Linux2.6-x86_64-gcc',
               'moohan-static' => '/mnt/hdf/packages/zlib-125/static/Linux2.6-x86_64-gcc',
               'moohan32' => '/mnt/hdf/packages/zlib-128/Linux2.6-x86_64-gcc-m32',
               'moohan32-static' => '/mnt/hdf/packages/zlib-128/Linux2.6-x86_64-gcc-m32',
               'platypus32' => '/mnt/hdf/packages/zlib-128/Linux2.6-x86_64-gcc-m32',
               'platypus32-static' => '/mnt/hdf/packages/zlib-128/Linux2.6-x86_64-gcc-m32',
               'platypus' => '/mnt/hdf/packages/zlib-125/shared/Linux2.6-x86_64-gcc',
               'platypus-static' => '/mnt/hdf/packages/zlib-125/static/Linux2.6-x86_64-gcc',
               'quail' => ' /mnt/hdf/packages/zlib-125/shared/mac-intel-x86_64',
               'quail-static' => ' /mnt/hdf/packages/zlib-125/static/mac-intel-x86_64');

my $indirectory = ".";
$indirectory = shift;

my $outdirectory = ".";
$outdirectory = shift;

unless (-d $outdirectory) {
   print "$outdirectory not found.  Create it or choose another one and try again.\n";
   exit 1;
}
 
print "Subdirectories of $indirectory will be tarred and stored in $outdirectory.\n";


my $tarfilename;
my $tardirname;
my $output;
my $cmd;
sub addzandszlibs {
   my $dir = shift;
   my $indirectory = shift;
   my $currentdir = getcwd();

   if (-d "$indirectory/$dir" ) {
      my $jpdir = $jpegdir{$dir};
      my $szdir = $szipdir{$dir};
      my $zldir = $zlibdir{$dir};
      if ($dir =~ /static/) {
         $cmd = "cp $jpdir/lib/libjpeg.a $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $szdir/lib/libsz.a $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $zldir/lib/libz.a $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
      } elsif ($dir eq "freedom" || $dir eq "loyalty") {
         $cmd = "cp $szdir/lib/libsz.so.2 $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $zldir/lib/libz.so.5 $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;

         chdir "$indirectory/$dir/lib" or die "Couldn't change directory to $indirectory/$dir/lib, $!";
         $cmd = "ln -s libsz.so.2 libsz.so";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libz.so.5 libz.so";
         $output = `$cmd`;
         print $output;

         chdir $currentdir or die "Couldn't change directory back to $currentdir, $!";
      } elsif ($dir eq "fred" || $dir eq "tejeda") {
         #we've been using the static binaries for the macs - built with -fPIC
         $cmd = "cp $szdir/lib/libsz.a $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $zldir/lib/libz.1.2.5.dylib $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libz.1.2.5.dylib libz.1.dylib";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libz.1.2.5.dylib libz.dylib";
         $output = `$cmd`;
         print $output;

      } else {
         $cmd = "cp $jpdir/lib/libjpeg.a $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $szdir/lib/libsz.so.2.0.0 $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;
         $cmd = "cp $zldir/lib/libz.so.1.2.5 $indirectory/$dir/lib";
         $output = `$cmd`;
         print $output;

         chdir "$indirectory/$dir/lib" or die "Couldn't change directory to $indirectory/$dir/lib, $!";
         $cmd = "ln -s libsz.so.2.0.0 libsz.so.2";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libsz.so.2.0.0 libsz.so";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libz.so.1.2.5 libz.so.1";
         $output = `$cmd`;
         print $output;

         $cmd = "ln -s libz.so.1.2.5 libz.so";
         $output = `$cmd`;
         print $output;

         chdir $currentdir or die "Couldn't change directory back to $currentdir, $!";
      }
   }
}

sub makeTarFile { 
   my $directoryname = shift;
   my $origdirname = shift;
   my $tarfilename = shift;
   my $tardirname = shift;
   $cmd = "mv $indirectory/$origdirname $indirectory/$tardirname";
   $output = `$cmd`;
   #print $output;
   #if (-d $indirectory."/".$tardirname."/doc/hdf4/examples") {
   #   $cmd = "mv $indirectory/$tardirname/doc/hdf4/examples $indirectory/$tardirname/examples";
   #   $output = `$cmd`;
   #   print $output;
   #   $cmd = "rm -rf $indirectory/$tardirname/doc";
   #   $output = `$cmd`;
   #   print $output;
   #}
   $cmd = "tar czvf $outdirectory/$directoryname/$tarfilename -C $indirectory $tardirname";
   print "Need to run $cmd.\n";
   $output = `$cmd`;
   print "Create $tarfilename:  $output\n";
   $cmd = "mv $indirectory/$tardirname $indirectory/$origdirname";
   $output = `$cmd`;
   print $output;
}

foreach $key (keys %destsubdir) {
   #skip unless there's a directory by the name of $key or $key-shared
   next unless -d $indirectory.'/'.$key."-static" || -d $indirectory.'/'.$key."-shared";
   my $version;
   # This assumes a static directory.  Probably the others should be checked if this
   # doesn't exist.
   $cmd = "grep \"HDF4 Version\" $indirectory/$key-static/lib/libhdf4.settings";
   $_ = `$cmd`;
   #print OUTFILE $_, "\n";
   print $_, "\n";
   s/HDF4 Version://;
   s/^\s+//;
   chomp;
   $version = $_;
   #my $directoryname = substr $destsubdir{$key}, 0, rindex($destsubdir{$key}, '-');
   my $directoryname = $destsubdir{$key};
   mkdir $outdirectory."/".$directoryname, 0755 unless -d $outdirectory."/".$directoryname;
   my $staticdir = $key."-static";
   print $indirectory."/$key tarfile will be put in " . $outdirectory."/".$directoryname."\n";
   if (-e $outdirectory."/".$destsubdir{$key}."/README") {
      print $outdirectory."/".$destsubdir{$key}."/README" . " has already been created.\n";
   }
   else {
      print "Make the Outer README file:  ";
      $cmd = "./makeOuterhdf4README.pl $indirectory/$key $outdirectory $directoryname";
      print $cmd, "\n";
      $output = `$cmd`;
      print $output;
   }
   my $file = "";
   my @dirnames = "";
   opendir(DIR, $indirectory) or die "can't open .: $!";
   while (defined($file = readdir(DIR))) {
      next unless $file =~ /$key/ && -d $indirectory."/".$file;
      push @dirnames, $file;
   }
   print "Make the Inner README files.\n";
   foreach my $dir (@dirnames) {
      next if $dir eq "";
      $cmd = "./makeInternalhdf4README.pl $indirectory/$dir";
      print $cmd, "\n";
      $output = `$cmd`;
      print $output;
      print "Add the zlib and szip files for $dir.\n";
      &addzandszlibs($dir, $indirectory);
      my $currentdir = getcwd();

      chdir "$indirectory/$dir/bin" or die "Couldn't change directory to $indirectory/$dir/bin, $!";
      $cmd = "/home/lrknox/hdf/release_scripts/latesthdf4/h4rmflags -force";
      $output = `$cmd`;
      print $output;
      chdir $currentdir or die "Couldn't change directory back to $currentdir, $!";

      print "Tar up the files into the output directory.\n";
      if (-d $indirectory."/".$dir) {
         if ($dir =~ /static/) {
            $tarfilename = "hdf-$version-$destsubdir{$key}.tar.gz";
            # for HDF4 the static binary won't have a -static suffix, as we are
            # not building shared binaries
            $tardirname = "hdf-$version-$destsubdir{$key}";
         } elsif ($dir =~ /shared/)  {
            $tarfilename = "hdf-$version-$destsubdir{$key}-shared.tar.gz";
            $tardirname = "hdf-$version-$destsubdir{$key}-shared";
         } else {
            next
         }
         &makeTarFile($directoryname, $dir, $tarfilename, $tardirname);
      } 
   }
    # If this copy is done after h4rmflags is run on all the directories the compile scripts
    # in the utilities directory will already have the paths removed.
   if (-d $indirectory."/".$staticdir) {
      $cmd = "cp -prv $indirectory/$staticdir/bin $outdirectory/$directoryname/utilities";
      $output = `$cmd`;
      print $output;
   }
}

