# Microsoft Visual C++ generated build script - Do not modify

PROJ = HDFED
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
FIRSTC = HE_CNTRL.C
FIRSTCPP = 
RC = rc
CFLAGS_D_DEXE32 = /nologo /W3 /Zi /YX /D "_DEBUG" /D "_X86_" /D "_CONSOLE" /D "WINNT" /FR /ML /Fd"HDFED.PDB"  /Fp"HDFED.PCH"
CFLAGS_R_DEXE32 = /nologo /W3 /YX /O2 /D "NDEBUG" /D "_X86_" /D "_CONSOLE" /D "WINNT" /FR /ML /Fp"HDFED.PCH"
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
SBRS = HE_CNTRL.SBR \
		HE_DISP.SBR \
		HE_FILE.SBR \
		HE_MAIN.SBR


HE_CNTRL_DEP =  \
	d:\users\koziol\hdf\util\he.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	d:\users\koziol\hdf\util\he_proto.h


HE_DISP_DEP =  \
	d:\users\koziol\hdf\util\he.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	d:\users\koziol\hdf\util\he_proto.h


HE_FILE_DEP =  \
	d:\users\koziol\hdf\util\he.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	d:\users\koziol\hdf\util\he_proto.h


HE_MAIN_DEP =  \
	d:\users\koziol\hdf\util\he.h \
	d:\users\koziol\hdf\src\hdf.h \
	d:\users\koziol\hdf\src\hdfi.h \
	d:\msvcnt\include\sys\file.h \
	d:\users\koziol\hdf\src\dfivms.h \
	d:\users\koziol\hdf\src\maldebug.h \
	d:\users\koziol\hdf\src\hcomp.h \
	d:\users\koziol\hdf\src\herr.h \
	d:\users\koziol\hdf\src\hproto.h \
	d:\users\koziol\hdf\src\vproto.h \
	d:\users\koziol\hdf\util\he_proto.h


HDF33SRC_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

HE_CNTRL.OBJ:	HE_CNTRL.C $(HE_CNTRL_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c HE_CNTRL.C

HE_DISP.OBJ:	HE_DISP.C $(HE_DISP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c HE_DISP.C

HE_FILE.OBJ:	HE_FILE.C $(HE_FILE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c HE_FILE.C

HE_MAIN.OBJ:	HE_MAIN.C $(HE_MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c HE_MAIN.C

$(PROJ).EXE:	HE_CNTRL.OBJ HE_DISP.OBJ HE_FILE.OBJ HE_MAIN.OBJ $(OBJS_EXT) $(LIBS_EXT)
	echo >NUL @<<$(PROJ).CRF
HE_CNTRL.OBJ 
HE_DISP.OBJ 
HE_FILE.OBJ 
HE_MAIN.OBJ 
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
