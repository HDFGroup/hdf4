
                          README for HDF 4.2r4
                          ====================
                            January 25, 2009

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

   config -- Machine dependent makefile fragments(mh-<os>). 

   hdf    -- The source code for the HDF 'base library', the multi-file 
             annotation interface, the multi-file raster image interface, 
             HDF command line utilities (hdp, the hdf dumper is in mfhdf/), 
             and a test suite. 
             This directory also contains the software distributions for 
             JPEG(hdf/jpeg) and ZLIB(hdf/zlib) which are required by the 
             HDF library. 
             Please see the README in each directory for further 
             information on each package. 

   mfhdf  -- The netCDF(mfhdf) part of the HDF/mfhdf distribution and
             the HDF dumper utility (hdp).

   man    -- An incomplete set of man page(s) for HDF.

   release_notes -- 
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

   * An ANSI C compiler. The native ANSI compilers on the above 
     platforms are supported. On platforms where no ANSI compiler
     was available, the free GNU ANSI compiler GCC was used.


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


