HDF version 4.2.10 released on 2014-02-09
===================================================

Obtaining the latest version of HDF4
------------------------------------
 The most recent version of the distribution can be obtained from
 the The HDF Group's FTP site:

     ftp://ftp.hdfgroup.org/HDF/HDF_Current/


Source Distribution Layout
--------------------------
 The top level of the source code distribution contains the following 
 subdirectories:

   bin    -- Scripts for maintenance.

   config -- Configuration files to be used by configure script.

   hdf    -- The source code for the HDF 'base library', the multi-file 
             annotation interface, the multi-file raster image interface, 
             HDF command line utilities, and a test suite. 
             Please see the README in each directory for further 
             information on each package. 

   mfhdf  -- The netCDF(mfhdf) part of the HDF/mfhdf distribution and
             additional HDF utilities, such as hdp, hrepack, hdfimport, etc.

   man    -- An incomplete set of man page(s) for HDF.

   release_notes -- 
             Installation instructions for UNIX and Windows.
             Descriptions of new features and bug fixes in this release.
             Files in this sub-directory can be used as supplemental 
             documentation for HDF. 
             These files are also available on the THG FTP server: 
             ftp://ftp.hdfgroup.org/HDF/HDF_Current/src/unpacked/release_notes

   windows-- Removed 4/2013.


Third Party Software Requirements
---------------------------------
 1. JPEG distribution release 6b(libjpeg.a).  You may download the software
    from http://www.hdfgroup.org/release4/obtain.html.

 2. ZLIB 1.1.4(libz.a) or later  distribution.  You may download the software
    from the http://www.gzip.org/ site.



System Requirements
-------------------
 To build the HDF library from source, you need:

   * C and Fortran compilers. For a list of the supported compilers,
     see release_notes/RELEASE.txt file.


Configuring/Testing/Installing 
------------------------------
 See the INSTALL file for instructions on configuring, testing, 
 and installing this software on Unix and non-UNIX systems.


DOCUMENTATION/FAQ/HELP
----------------------
 The HDF documentation can be found on the THG FTP server and on 
 the THG website:

      http://hdfgroup.org/doc.html
      ftp://ftp.hdfgroup.org/HDF/Documentation  

 The HDF home page is at:

      http://hdfgroup.org

 An FAQ is available on the FTP server and as under "Information about 
 HDF" on the website.

      http://hdfgroup.org/products/hdf4/index.html

 If you have any questions or comments, or would like to be added to 
 or removed from our hdfnews email list, contact information and other 
 resource information can be found on the HDF Support page:

     http://hdfgroup.org/services/support.html


