# Microsoft Visual C++ generated build script - Do not modify

PROJ = PALTOHDF
DEBUG = 1
PROGTYPE = 2
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = D:\USERS\KOZIOL\HDF\UTIL\ 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = PALTOHDF.C
FIRSTCPP = 
RC = rc
CFLAGS_D_DEXE32 = /nologo /W3 /Zi /YX /D "_DEBUG" /D "_X86_" /D "_CONSOLE" /D "WINNT" /FR /ML /Fd"PALTOHDF.PDB"  /Fp"PALTOHDF.PCH"
CFLAGS_R_DEXE32 = /nologo /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_CONSOLE" /D "WINNT" /FR /ML /Fp"PALTOHDF.PCH"
LFLAGS_D_DEXE32 = /NOLOGO /DEBUG /DEBUGTYPE:cv /SUBSYSTEM:console netapi32.lib
LFLAGS_R_DEXE32 = /NOLOGO /SUBSYSTEM:console netapi32.lib
LFLAGS_D_LIB32 = /NOLOGO
LFLAGS_R_LIB32 = /NOLOGO
LIBS_D_DEXE32 = 
LIBS_R_DEXE32 = 
RCFLAGS32 = 
D_RCDEFINES32 = -d_DEBUG
R_RCDEFINES32 = -dNDEBUG
OBJS_EXT = 
LIBS_EXT = ..\SRC\HDF33SRC.LIB 
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
SBRS = PALTOHDF.SBR


PALTOHDF_DEP =  \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h


HDF33SRC_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

PALTOHDF.OBJ:	PALTOHDF.C $(PALTOHDF_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c PALTOHDF.C

$(PROJ).EXE:	PALTOHDF.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
PALTOHDF.OBJ 
$(OBJS_EXT)
-OUT:$(PROJ).EXE
$(MAPFILE_OPTION)
..\SRC\HDF33SRC.LIB
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
