# Microsoft Visual C++ generated build script - Do not modify

PROJ = NCGEN
DEBUG = 1
PROGTYPE = 2
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = E:\KOZIOL\MFHDF\NCGEN\ 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MAIN.C
FIRSTCPP = 
RC = rc
CFLAGS_D_DEXE32 = /nologo /W3 /Zi /YX /D "_DEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fd"NCGEN.PDB"  /Fp"NCGEN.PCH"
CFLAGS_R_DEXE32 = /nologo /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_CONSOLE" /D "HDF" /D "WINNT" /D "STDC_INCLUDES" /D "NO_SYS_XDR_INC" /D "i386" /I "e:\koziol\mfhdf\libsrc" /I "e:\koziol\mfhdf\xdr" /FR /ML /Fp"NCGEN.PCH"
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
SBRS = MAIN.SBR \
		GENERATE.SBR \
		LOAD.SBR \
		ESCAPES.SBR \
		GETFILL.SBR \
		INIT.SBR \
		CLOSE.SBR \
		GENLIB.SBR \
		NCGENTAB.SBR


MFHDF_DEP = 

HDF33SRC_DEP = 

MAIN_DEP =  \
	e:\koziol\mfhdf\ncgen\genlib.h


GENERATE_DEP =  \
	e:\koziol\mfhdf\ncgen\ncgen.h \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h \
	e:\koziol\mfhdf\ncgen\genlib.h


LOAD_DEP =  \
	e:\koziol\mfhdf\ncgen\ncgen.h \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h \
	e:\koziol\mfhdf\ncgen\genlib.h


ESCAPES_DEP = 

GETFILL_DEP =  \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h \
	e:\koziol\mfhdf\ncgen\genlib.h


INIT_DEP =  \
	e:\koziol\mfhdf\ncgen\ncgen.h \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h


CLOSE_DEP =  \
	e:\koziol\mfhdf\ncgen\ncgen.h \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h \
	e:\koziol\mfhdf\ncgen\genlib.h


GENLIB_DEP =  \
	e:\koziol\mfhdf\ncgen\genlib.h


NCGENTAB_DEP =  \
	e:\koziol\mfhdf\ncgen\ncgen.h \
	e:\koziol\mfhdf\libsrc\netcdf.h \
	e:\koziol\mfhdf\ncgen\generic.h \
	e:\koziol\mfhdf\ncgen\ncgenyy.c \
	e:\koziol\mfhdf\ncgen\ncgentab.h


all:	$(PROJ).EXE $(PROJ).BSC

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MAIN.C

GENERATE.OBJ:	GENERATE.C $(GENERATE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GENERATE.C

LOAD.OBJ:	LOAD.C $(LOAD_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LOAD.C

ESCAPES.OBJ:	ESCAPES.C $(ESCAPES_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ESCAPES.C

GETFILL.OBJ:	GETFILL.C $(GETFILL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GETFILL.C

INIT.OBJ:	INIT.C $(INIT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c INIT.C

CLOSE.OBJ:	CLOSE.C $(CLOSE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CLOSE.C

GENLIB.OBJ:	GENLIB.C $(GENLIB_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GENLIB.C

NCGENTAB.OBJ:	NCGENTAB.C $(NCGENTAB_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c NCGENTAB.C

$(PROJ).EXE:	MAIN.OBJ GENERATE.OBJ LOAD.OBJ ESCAPES.OBJ GETFILL.OBJ INIT.OBJ CLOSE.OBJ \
	GENLIB.OBJ NCGENTAB.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
MAIN.OBJ 
GENERATE.OBJ 
LOAD.OBJ 
ESCAPES.OBJ 
GETFILL.OBJ 
INIT.OBJ 
CLOSE.OBJ 
GENLIB.OBJ 
NCGENTAB.OBJ 
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
