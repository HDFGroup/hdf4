# Microsoft Visual C++ generated build script - Do not modify

PROJ = NCTEST
DEBUG = 1
PROGTYPE = 2
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = E:\KOZIOL\MFHDF\NCTEST\ 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = ADD.C
FIRSTCPP = 
RC = rc
CFLAGS_D_DEXE32 = /nologo /W3 /Zi /YX /D "_DEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fd"NCTEST.PDB"  /Fp"NCTEST.PCH"
CFLAGS_R_DEXE32 = /nologo /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fp"NCTEST.PCH"
LFLAGS_D_DEXE32 = /NOLOGO /DEBUG /DEBUGTYPE:cv /SUBSYSTEM:console netapi32.lib wsock32.lib
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
SBRS = ADD.SBR \
		VARGET.SBR \
		VARGETG.SBR \
		VARPUT.SBR \
		VARPUTG.SBR \
		VARDEF.SBR \
		VARTESTS.SBR \
		VPUTGET.SBR \
		VPUTGETG.SBR \
		DRIVER.SBR \
		CDFTESTS.SBR \
		DIMTESTS.SBR \
		REC.SBR \
		ATTTESTS.SBR \
		MISCTEST.SBR \
		ERROR.SBR \
		EMALLOC.SBR \
		VAL.SBR \
		SLABS.SBR


ADD_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARGET_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARGETG_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARPUT_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARPUTG_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARDEF_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VARTESTS_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VPUTGET_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VPUTGETG_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


DRIVER_DEP =  \
	e:\koziol\mfhdf\nctest\tests.h


CDFTESTS_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


DIMTESTS_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


REC_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


ATTTESTS_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\emalloc.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\val.h


MISCTEST_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h


ERROR_DEP =  \
	e:\koziol\mfhdf\nctest\error.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h


EMALLOC_DEP =  \
	e:\koziol\mfhdf\nctest\error.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\emalloc.h


VAL_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\val.h \
	e:\koziol\mfhdf\nctest\error.h


SLABS_DEP =  \
	e:\koziol\mfhdf\nctest\testcdf.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	e:\koziol\mfhdf\nctest\add.h \
	e:\koziol\mfhdf\nctest\error.h \
	e:\koziol\mfhdf\nctest\tests.h \
	e:\koziol\mfhdf\nctest\emalloc.h


MFHDF_DEP = 

HDF33SRC_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

ADD.OBJ:	ADD.C $(ADD_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ADD.C

VARGET.OBJ:	VARGET.C $(VARGET_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARGET.C

VARGETG.OBJ:	VARGETG.C $(VARGETG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARGETG.C

VARPUT.OBJ:	VARPUT.C $(VARPUT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARPUT.C

VARPUTG.OBJ:	VARPUTG.C $(VARPUTG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARPUTG.C

VARDEF.OBJ:	VARDEF.C $(VARDEF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARDEF.C

VARTESTS.OBJ:	VARTESTS.C $(VARTESTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VARTESTS.C

VPUTGET.OBJ:	VPUTGET.C $(VPUTGET_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VPUTGET.C

VPUTGETG.OBJ:	VPUTGETG.C $(VPUTGETG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VPUTGETG.C

DRIVER.OBJ:	DRIVER.C $(DRIVER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DRIVER.C

CDFTESTS.OBJ:	CDFTESTS.C $(CDFTESTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CDFTESTS.C

DIMTESTS.OBJ:	DIMTESTS.C $(DIMTESTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DIMTESTS.C

REC.OBJ:	REC.C $(REC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c REC.C

ATTTESTS.OBJ:	ATTTESTS.C $(ATTTESTS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ATTTESTS.C

MISCTEST.OBJ:	MISCTEST.C $(MISCTEST_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MISCTEST.C

ERROR.OBJ:	ERROR.C $(ERROR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ERROR.C

EMALLOC.OBJ:	EMALLOC.C $(EMALLOC_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c EMALLOC.C

VAL.OBJ:	VAL.C $(VAL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VAL.C

SLABS.OBJ:	SLABS.C $(SLABS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SLABS.C

$(PROJ).EXE:	ADD.OBJ VARGET.OBJ VARGETG.OBJ VARPUT.OBJ VARPUTG.OBJ VARDEF.OBJ VARTESTS.OBJ \
	VPUTGET.OBJ VPUTGETG.OBJ DRIVER.OBJ CDFTESTS.OBJ DIMTESTS.OBJ REC.OBJ ATTTESTS.OBJ \
	MISCTEST.OBJ ERROR.OBJ EMALLOC.OBJ VAL.OBJ SLABS.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
ADD.OBJ 
VARGET.OBJ 
VARGETG.OBJ 
VARPUT.OBJ 
VARPUTG.OBJ 
VARDEF.OBJ 
VARTESTS.OBJ 
VPUTGET.OBJ 
VPUTGETG.OBJ 
DRIVER.OBJ 
CDFTESTS.OBJ 
DIMTESTS.OBJ 
REC.OBJ 
ATTTESTS.OBJ 
MISCTEST.OBJ 
ERROR.OBJ 
EMALLOC.OBJ 
VAL.OBJ 
SLABS.OBJ 
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
