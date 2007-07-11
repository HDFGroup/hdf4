# Microsoft Developer Studio Project File - Name="testhdf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=testhdf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "testhdf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testhdf.mak" CFG="testhdf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testhdf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "testhdf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "testhdf - Win32 AlphaRel" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "testhdf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "testhdf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\testhdf\Release"
# PROP BASE Intermediate_Dir ".\testhdf\Release"
# PROP BASE Target_Dir ".\testhdf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\testhdf\Release"
# PROP Intermediate_Dir ".\testhdf\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\testhdf"
F90=df.exe
# ADD BASE F90 /include:".\testhdf\Release/" /I "testhdf\Release/"
# ADD F90 /include:".\testhdf\Release/" /I "testhdf\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /I "hdf\src" /D "_CONSOLE" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /libpath:".\windows\lib\release\singlethreaded"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\testhdf\Debug"
# PROP BASE Intermediate_Dir ".\testhdf\Debug"
# PROP BASE Target_Dir ".\testhdf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\testhdf\Debug"
# PROP Intermediate_Dir ".\testhdf\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\testhdf"
F90=df.exe
# ADD BASE F90 /include:".\testhdf\Debug/" /I "testhdf\Debug/"
# ADD F90 /dbglibs /include:".\testhdf\Debug/" /I "testhdf\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "hdf\src" /D "_CONSOLE" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:".\windows\lib\debug\singlethreaded"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "testhdf\AlphaRel"
# PROP BASE Intermediate_Dir "testhdf\AlphaRel"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "testhdf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "testhdf\AlphaRel"
# PROP Intermediate_Dir "testhdf\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "testhdf"
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"testhdf\AlphaRel/" /nologo /warn:nofileopt /I "testhdf\Debug/"
# ADD F90 /compile_only /debug:full /include:"testhdf\AlphaRel/" /nologo /warn:nofileopt /I "testhdf\Debug/"
# SUBTRACT F90 /dbglibs /libdir:noauto /winapp
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /ML /Gt0 /W3 /GX /I ".\hdf\zlib" /I ".\hdf\jpeg" /I "hdf\src" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT CPP /Z<none> /O<none> /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 src\debug\src.lib jpeg\debug\jpeg.lib zlib\debug\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "testhdf\AlphaDbg"
# PROP BASE Intermediate_Dir "testhdf\AlphaDbg"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "testhdf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "testhdf\AlphaDbg"
# PROP Intermediate_Dir "testhdf\AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "testhdf"
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"testhdf\AlphaDbg/" /nologo /warn:nofileopt /I "testhdf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"testhdf\AlphaDbg/" /nologo /warn:nofileopt /I "testhdf\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I "hdf\src" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 src\debug\src.lib jpeg\debug\jpeg.lib zlib\debug\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA

!ENDIF 

# Begin Target

# Name "testhdf - Win32 Release"
# Name "testhdf - Win32 Debug"
# Name "testhdf - Win32 AlphaRel"
# Name "testhdf - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\hdf\test\an.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_AN_C0=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_AN_C0=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_AN_C0=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_AN_C0=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\anfile.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_ANFIL=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_ANFIL=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_ANFIL=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_ANFIL=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\bitio.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_BITIO=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_BITIO=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_BITIO=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_BITIO=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\blocks.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_BLOCK=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_BLOCK=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_BLOCK=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_BLOCK=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\chunks.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_CHUNK=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_CHUNK=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_CHUNK=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_CHUNK=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\comp.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_COMP_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_COMP_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_COMP_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_COMP_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\conv.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_CONV_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_CONV_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_CONV_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_CONV_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\extelt.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_EXTEL=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_EXTEL=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_EXTEL=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_EXTEL=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\file.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_FILE_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_FILE_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_FILE_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_FILE_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\file1.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_FILE1=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_FILE1=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_FILE1=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_FILE1=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\litend.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_LITEN=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_LITEN=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_LITEN=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_LITEN=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\macros.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_MACRO=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MACRO=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_MACRO=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MACRO=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\man.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_MAN_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MAN_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_MAN_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MAN_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\mgr.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_MGR_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MGR_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_MGR_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_MGR_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\nbit.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_NBIT_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_NBIT_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_NBIT_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_NBIT_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\rig.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_RIG_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_RIG_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_RIG_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_RIG_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\sdmms.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_SDMMS=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDMMS=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_SDMMS=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDMMS=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\sdnmms.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_SDNMM=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDNMM=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_SDNMM=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDNMM=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\sdstr.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_SDSTR=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDSTR=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_SDSTR=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SDSTR=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\slab.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_SLAB_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SLAB_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_SLAB_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_SLAB_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tbv.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TBV_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TBV_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TBV_C=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TBV_C=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tdupimgs.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\testhdf.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TESTH=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TESTH=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TESTH=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TESTH=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tproto.h
# End Source File
# Begin Source File

SOURCE=.\hdf\test\tree.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TREE_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TREE_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TREE_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TREE_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tszip.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tutils.h
# End Source File
# Begin Source File

SOURCE=.\hdf\test\tvattr.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TVATT=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVATT=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TVATT=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVATT=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tvset.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TVSET=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVSET=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TVSET=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVSET=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\tvsfpack.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_TVSFP=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVSFP=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_TVSFP=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_TVSFP=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hdf\test\vers.c

!IF  "$(CFG)" == "testhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "testhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaRel"

DEP_CPP_VERS_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_VERS_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "testhdf - Win32 AlphaDbg"

DEP_CPP_VERS_=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\trace.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\hdf\test\tproto.h"\
	".\hdf\test\tutils.h"\
	
NODEP_CPP_VERS_=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\HDFIOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
