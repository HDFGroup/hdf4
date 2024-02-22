# HDF 4.2 to 4.3 Migration Guide

For the past few decades, HDF4 has deployed almost all of the library's header
files, both public and private. This deployment scheme makes even trivial
refactoring and bug fixing difficult to accomplish without breaking binary
compatibility when "private" headers change.

HDF 4.3.0 will finally separate the public and private APIs in both the single-
and multi-file libraries, allowing us to begin the process of cleaning up
several decades of accumulated technical debt.

## Changes in HDF 4.3.0

* Private header files are no longer distributed. A complete list can be found below.
* The error-handling macros have been removed from `herr.h`.
* Most of `hbitio.h` has been moved to a private header file.
* Most of `hfile.h` has been moved to a private header file.
* `hdf.h` no longer pulls in the `hdfi.h` internal header file. See below for the implications of this.

## Migration Steps

Most users should not have to make any changes to their applications, as long
as they only use the public API as documented in the HDF4 Reference Manual.
The single- and multi-file API calls are all unchanged and we have preserved
the legacy HDF 3 API (found in `df.h`).

If you HAVE used any of the undocumented functionality packeded with the library
(e.g., `linklist.h`'s linked lists), you will either have to pull those
source files out of an older version of the library for incorporation into
your own code, or stick with HDF 4.2.16. Note that we don't plan to have
additional HDF 4.2.x maintenance releases.

An additional problem you may encounter stems from the removal of the `hdfi.h`
internal header file from `hdf.h`. That internal header brought in a lot of OS,
POSIX, and C headers which will now be missing from your application. If you
relied on the implicit inclusion of `hdfi.h` to bring in things like `sys/types`,
you will have to add these to your source code, as `hdf.h` only includes what
it needs.

## Appendix: List of Removed Header Files

The following headers are no longer installed:

* atom.h
* bitvect.h
* cdeflate.h
* cnbit.h
* cnone.h
* crle.h
* cskphuff.h
* cszip.h
* df.h
* dfan.h
* dfgr.h
* dfrig.h
* dfsd.h
* dfufp2i.h
* dynarray.h
* hchunks.h
* hcompi.h
* hconv.h
* hdfi.h
* hfile.h
* hkit.h
* linklist.h
* local\_nc.h
* mfani.h
* mfgri.h
* mstdio.h
* tbbt.h

## Appendix: List of Removed Functions


## Appendix: List of Removed Symbols, Types, etc.
