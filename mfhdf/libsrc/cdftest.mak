# Microsoft Visual C++ generated build script - Do not modify

PROJ = CDFTEST
DEBUG = 1
PROGTYPE = 2
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = E:\KOZIOL\MFHDF\LIBSRC\ 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = CDFTEST.C
FIRSTCPP = 
RC = rc
CFLAGS_D_DEXE32 = /nologo /W3 /Zi /YX /D "_DEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fd"CDFTEST.PDB"  /Fp"CDFTEST.PCH"
CFLAGS_R_DEXE32 = /nologo /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fp"CDFTEST.PCH"
LFLAGS_D_DEXE32 = /NOLOGO /DEBUG /DEBUGTYPE:cv /SUBSYSTEM:console wsock32.lib netapi32.lib wsock32.lib
LFLAGS_R_DEXE32 = /NOLOGO /SUBSYSTEM:console netapi32.lib wsock32.lib
LFLAGS_D_LIB32 = /NOLOGO
LFLAGS_R_LIB32 = /NOLOGO
LIBS_D_DEXE32 = 
LIBS_R_DEXE32 = 
RCFLAGS32 = 
D_RCDEFINES32 = -d_DEBUG
R_RCDEFINES32 = -dNDEBUG
OBJS_EXT = 
LIBS_EXT = ..\MFHDF.LIB ..\..\HDF\SRC\HDF33SRC.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE32)
LFLAGS = $(LFLAGS_D_DEXE32)
LIBS = $(LIBS_D_DEXE32)
LFLAGS_LIB=$(LFLAGS_D_LIB32)
MAPFILE_OPTION = 
RCDEFINES = $(D_RCDEFINES32)
!else
CFLAGS = $(CFLAGS_R_DEXE32)
LFLAGS = $(LFLAGS_R_DEXE32)
LIBS = $(LIBS_R_DEXE32)
MAPFILE_OPTION = 
LFLAGS_LIB=$(LFLAGS_R_LIB32)
RCDEFINES = $(R_RCDEFINES32)
!endif
SBRS = CDFTEST.SBR


CDFTEST_DEP =  \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h


MFHDF_DEP = 

HDF33SRC_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

CDFTEST.OBJ:	CDFTEST.C $(CDFTEST_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c CDFTEST.C

$(PROJ).EXE:	CDFTEST.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
CDFTEST.OBJ 
$(OBJS_EXT)
-OUT:$(PROJ).EXE
$(MAPFILE_OPTION)
..\MFHDF.LIB
..\..\HDF\SRC\HDF33SRC.LIB
$(LIBS)
$(LIBS_EXT)
$(DEFFILE_OPTION) -implib:$(PROJ).lib
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
