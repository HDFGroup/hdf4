# Microsoft Developer Studio Project File - Name="hdf_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=hdf_dll - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hdf_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdf_dll.mak" CFG="hdf_dll - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdf_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hdf_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hdf_dll - Win32 AlphaDbg" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "hdf_dll - Win32 AlphaRel" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\release\multithreaded"
# PROP Intermediate_Dir ".\hdf_dll\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Release/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Release/" /libdir:noauto /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /D "HDFLIBDLL" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /out:"..\dll\release\hd421m.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\debug\multithreaded"
# PROP Intermediate_Dir ".\hdf_dll\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Debug/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Debug/" /libs:dll /threads
# SUBTRACT F90 /dbglibs
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /D "HDFLIBDLL" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /out:"..\dll\debug\hd421md.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hdf_dll_"
# PROP BASE Intermediate_Dir "hdf_dll_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"hdf_dll_/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaDbg/" /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\hdf\jpeg" /I "..\..\hdf\zlib" /I "..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\hdf\jpeg" /I "..\..\hdf\zlib" /I "..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /MDd /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"hd412md.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:"..\lib/hd412md.pdb" /debug /machine:ALPHA /out:"..\dll\debug\hd413md.dll" /implib:"..\lib\debug\multithreaded/hd413md.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "hdf_dll0"
# PROP BASE Intermediate_Dir "hdf_dll0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"hdf_dll0/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaRel/" /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\hdf\jpeg" /I "..\..\hdf\zlib" /I "..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\hdf\jpeg" /I "..\..\hdf\zlib" /I "..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"hd412m.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:"..\lib/hd412m.pdb" /machine:ALPHA /out:"..\dll\release\hd413m.dll" /implib:"..\lib\release\multithreaded/hd413m.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "hdf_dll - Win32 Release"
# Name "hdf_dll - Win32 Debug"
# Name "hdf_dll - Win32 AlphaDbg"
# Name "hdf_dll - Win32 AlphaRel"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\hdf\src\cdeflate.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cnbit.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cnone.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\crle.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cskphuff.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfan.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfconvrt.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfgr.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfi.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfsd.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfstubs.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfufp2i.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\glist.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hchunks.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hfile.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hkit.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\mstdio.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\patchlevel.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vattr.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vg.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vgint.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\hdf\src\atom.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_ATOM_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_ATOM_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_ATOM_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_ATOM_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\bitvect.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_BITVE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_BITVE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_BITVE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_BITVE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cdeflate.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_CDEFL=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CDEFL=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_CDEFL=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CDEFL=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cnbit.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_CNBIT=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CNBIT=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_CNBIT=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CNBIT=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cnone.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_CNONE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CNONE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_CNONE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CNONE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\crle.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_CRLE_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CRLE_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_CRLE_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CRLE_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cskphuff.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_CSKPH=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CSKPH=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_CSKPH=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_CSKPH=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\df24.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DF24_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfgr.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DF24_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DF24_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfgr.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DF24_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfan.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFAN_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfan.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFAN_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFAN_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfan.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFAN_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFCOM=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFCOM=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFCOM=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFCOM=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFCON=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFCON=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFCON=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFCON=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfgr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFGR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfgr.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFGR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFGR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfgr.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFGR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfgroup.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFGRO=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFGRO=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFGRO=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFGRO=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfimcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFIMC=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFIMC=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFIMC=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFIMC=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfjpeg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFJPE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFJPE=\
	"..\..\hdf\jpeg\jconfig.h"\
	"..\..\hdf\jpeg\jerror.h"\
	"..\..\hdf\jpeg\jmorecfg.h"\
	"..\..\hdf\jpeg\jpegint.h"\
	"..\..\hdf\jpeg\jpeglib.h"\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFJPE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFJPE=\
	"..\..\hdf\jpeg\jconfig.h"\
	"..\..\hdf\jpeg\jerror.h"\
	"..\..\hdf\jpeg\jmorecfg.h"\
	"..\..\hdf\jpeg\jpegint.h"\
	"..\..\hdf\jpeg\jpeglib.h"\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfkconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKCO=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKCO=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKCO=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKCO=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfkcray.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKCR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKCR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKCR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKCR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfkfuji.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKFU=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKFU=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKFU=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKFU=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfknat.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKNA=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKNA=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKNA=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKNA=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfkswap.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKSW=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKSW=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKSW=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKSW=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfkvms.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFKVM=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKVM=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFKVM=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hconv.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFKVM=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFP_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFP_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFP_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFP_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfr8.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFR8_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfrig.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFR8_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFR8_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfrig.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFR8_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfrle.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFRLE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFRLE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFRLE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFRLE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfsd.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFSD_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfsd.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFSD_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFSD_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfsd.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFSD_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfstubs.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFSTU=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\df.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfstubs.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFSTU=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFSTU=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\df.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dfstubs.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFSTU=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfunjpeg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFUNJ=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFUNJ=\
	"..\..\hdf\jpeg\jconfig.h"\
	"..\..\hdf\jpeg\jerror.h"\
	"..\..\hdf\jpeg\jmorecfg.h"\
	"..\..\hdf\jpeg\jpegint.h"\
	"..\..\hdf\jpeg\jpeglib.h"\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFUNJ=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFUNJ=\
	"..\..\hdf\jpeg\jconfig.h"\
	"..\..\hdf\jpeg\jerror.h"\
	"..\..\hdf\jpeg\jmorecfg.h"\
	"..\..\hdf\jpeg\jpegint.h"\
	"..\..\hdf\jpeg\jpeglib.h"\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dfutil.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DFUTI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFUTI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DFUTI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DFUTI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\dynarray.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_DYNAR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DYNAR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_DYNAR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_DYNAR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\glist.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_GLIST=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\glist.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_GLIST=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_GLIST=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\glist.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_GLIST=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hbitio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HBITI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HBITI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HBITI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HBITI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hblocks.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HBLOC=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HBLOC=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HBLOC=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HBLOC=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hbuffer.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hchunks.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HCHUN=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HCHUN=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HCHUN=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HCHUN=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HCOMP=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HCOMP=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HCOMP=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HCOMP=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hcompri.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hdfalloc.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HDFAL=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HDFAL=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HDFAL=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HDFAL=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\herr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HERR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HERR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HERR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HERR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hextelt.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HEXTE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HEXTE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HEXTE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HEXTE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hfile.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HFILE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dir_mac.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\glist.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\sys_dir_mac.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HFILE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HFILE=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dir_mac.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\glist.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\sys_dir_mac.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HFILE=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hfiledd.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HFILED=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HFILED=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HFILED=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HFILED=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\hkit.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_HKIT_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hkit.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HKIT_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_HKIT_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hkit.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_HKIT_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\mcache.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_MCACH=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MCACH=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_MCACH=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MCACH=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\mfan.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_MFAN_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfan.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MFAN_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_MFAN_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfan.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MFAN_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\mfgr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_MFGR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MFGR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_MFGR_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MFGR_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\mstdio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_MSTDI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MSTDI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_MSTDI=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\cdeflate.h"\
	"..\..\hdf\src\cnbit.h"\
	"..\..\hdf\src\cnone.h"\
	"..\..\hdf\src\crle.h"\
	"..\..\hdf\src\cskphuff.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hcompi.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\mstdio.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_MSTDI=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	"..\..\hdf\zlib\zconf.h"\
	"..\..\hdf\zlib\zlib.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\tbbt.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_TBBT_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_TBBT_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_TBBT_=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_TBBT_=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vattr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VATTR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VATTR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VATTR=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VATTR=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VCONV=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VCONV=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VCONV=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VCONV=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VG_C5e=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VG_C5e=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VG_C5e=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VG_C5e=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vgp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VGP_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VGP_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VGP_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VGP_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vhi.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VHI_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VHI_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VHI_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VHI_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VIO_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VIO_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VIO_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VIO_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vparse.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VPARS=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VPARS=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VPARS=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VPARS=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vrw.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VRW_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VRW_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VRW_C=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VRW_C=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\vsfld.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

DEP_CPP_VSFLD=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VSFLD=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

DEP_CPP_VSFLD=\
	"..\..\hdf\src\atom.h"\
	"..\..\hdf\src\bitvect.h"\
	"..\..\hdf\src\dfivms.h"\
	"..\..\hdf\src\dynarray.h"\
	"..\..\hdf\src\hbitio.h"\
	"..\..\hdf\src\hchunks.h"\
	"..\..\hdf\src\hcomp.h"\
	"..\..\hdf\src\hdf.h"\
	"..\..\hdf\src\hdfi.h"\
	"..\..\hdf\src\herr.h"\
	"..\..\hdf\src\hfile.h"\
	"..\..\hdf\src\hlimits.h"\
	"..\..\hdf\src\hntdefs.h"\
	"..\..\hdf\src\hproto.h"\
	"..\..\hdf\src\hqueue.h"\
	"..\..\hdf\src\htags.h"\
	"..\..\hdf\src\linklist.h"\
	"..\..\hdf\src\maldebug.h"\
	"..\..\hdf\src\mcache.h"\
	"..\..\hdf\src\mfgr.h"\
	"..\..\hdf\src\tbbt.h"\
	"..\..\hdf\src\trace.h"\
	"..\..\hdf\src\vg.h"\
	"..\..\hdf\src\vgint.h"\
	
NODEP_CPP_VSFLD=\
	"..\..\hdf\src\api_adpt.h"\
	"..\..\hdf\src\fmpio.h"\
	"..\..\hdf\src\HDFIOTrace.h"\
	"..\..\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\hdf\src\cszip.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\cszip.h
# End Source File
# Begin Source File

SOURCE=..\..\hdf\src\h4config.h
# End Source File
# End Target
# End Project
