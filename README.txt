HDF version 4.2.4-snap8 currently under development
===================================================

Obtaining the latest version of HDF4
------------------------------------
 The most recent version of the distribution can be obtained from
 the THG FTP site:

     ftp://ftp.hdfgroup.org/HDF/HDF_Current/


Source Distribution Layout
--------------------------
 The top level of the source code distribution contains the following 
 subdirectories:

   bin    -- Scripts for maintenance.

   config -- Configuration files to be used by conifgure script.

   hdf    -- The source code for the HDF 'base library', the multi-file 
             annotation interface, the multi-file raster image interface, 
             HDF command line utilities (hdp, the hdf dumper is in mfhdf/), 
             and a test suite. 
             Please see the README in each directory for further 
             information on each package. 

   mfhdf  -- The netCDF(mfhdf) part of the HDF/mfhdf distribution and
             the HDF dumper utility (hdp).

   man    -- An incomplete set of man page(s) for HDF.

   release_notes -- 
             Installation instructions for UNIX and Windows.
             Descriptions of new features and bug fixes in this release.
             Files in this sub-directory can be used as supplemental 
             documentation for HDF. 
             These files are also available on the THG FTP server: 
             ftp://ftp.hdfgroup.org/HDF/HDF_Current/src/unpacked/release_notes

   windows-- Files related to Windows support.


Third Party Software Requirements
---------------------------------
 1. JPEG distribution release 6b(libjpeg.a). The "official" site 
    for this is ftp://ftp.uu.net/graphics/jpeg/jpegsrc.v6b.tar.gz

 2. ZLIB 1.1.4(libz.a) or later  distribution.  


System Requirements
-------------------
 To build the HDF library from source, you need:

   * C and Fortran compilers. For list of the supported compilers
     check release_notes/RELEASE.txt file.


Configuring/Testing/Installing 
------------------------------
 See the INSTALL file for instructions on configuring, testing, 
 and installing this software on  Unix and non-UNIX systems.


DOCUMENTATION/FAQ/HELP
----------------------
 The HDF4 documentation can be found on the THG FTP server and on 
 the THG website:

      ftp://ftp.hdfgroup.org/HDF/Documentation  
      http://hdfgroup.org/doc.html

 The HDF home page is at:

      http://hdfgroup.org

 An FAQ is available on the FTP server and as under "Information about 
 HDF" on the website.

      http://hdfgroup.org/products/hdf4/index.html

 If you have any questions or comments, or would like to be added to 
 or removed from our hdfnews email list, contact information and other 
 resource information can be found on the HDF Support page:

     http://hdfgroup.org/services/support.html


