! $Id$
!
!          MMS file for netcdf Fortran API
!
!==============================================================================
! ASSUMPTIONS:

!==============================================================================
! CUSTOMIZATIONS (change as necessary):

!Root of the installation:
INSTROOT	= LDMROOT:

!Operating system version:
OS		= vms_5_4

!Compilation options:
COPTS		= /opt/nodebug

!==============================================================================
INCDIR		= $(INSTROOT)[include]
LIBDIR		= $(INSTROOT)[lib]
LIBRARY		= [-]netcdf.olb
LINKFLAGS	= /nodebug/exec=$(mms$target_name).exe
!LINKFLAGS	= /debug/exec=$(mms$target_name).exe
SRCINCDIR	= [-.src]
INCLUDES	= /include=([-.xdr],$(SRCINCDIR))
CFLAGS		= $(COPTS)$(INCLUDES)/obj=$(mms$target_name).obj
LIBOBJS 	= array=array.obj, attr=attr.obj, cdf=cdf.obj, dim=dim.obj,-
		    file=file.obj, iarray=iarray.obj, error=error.obj,-
		    putget=putget.obj, sharray=sharray.obj, string=string.obj,-
		    var=var.obj,-
		    htons.obj, ntohs.obj
CLIB 		=  sys$library:vaxcrtl/lib
LINKLIBS 	=  $(LIBRARY)/lib, $(CLIB)

all :		netcdf.inc, library, test
	@ continue

netcdf.inc :	[.vms]netcdf.inc
	copy [.vms]$@ $@

library :	$(LIBRARY)(jackets.obj)
	@ continue

jackets.obj :	[.vms]jackets.c

test :		ftest.exe
	run ftest
	@ if $severity .eq. 0 then exit 1

ftest.exe :	ftest.obj, jackets.obj
	$(LINK)$(LINKFLAGS) ftest.obj,jackets.obj,$(LINKLIBS)

install :	netcdf.inc
	copy netcdf.inc $(INCDIR)

clean :
	purge
	- delete netcdf.inc.*,*.obj.*,*.lis.*,*.map.*,*.exe.*

ftest.obj :	netcdf.inc, [.vms]ftest.for
jackets.obj :	$(SRCINCDIR)netcdf.h
