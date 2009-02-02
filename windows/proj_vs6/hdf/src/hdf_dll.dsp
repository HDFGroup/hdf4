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
# PROP Output_Dir "..\..\..\..\hdf\src\Release"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Release\hdf_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Release/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Release/" /libdir:noauto /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /D "HDFLIBDLL" /YX /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /out:"..\..\..\..\hdf\src\release\hd424m.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\hdf\src\Debug"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Debug\hdf_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Debug/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Debug/" /libs:dll /threads
# SUBTRACT F90 /dbglibs
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /D "HDFLIBDLL" /YX /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /out:"..\..\..\..\hdf\src\debug\hd424md.dll" /pdbtype:sept
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
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /MDd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /MDd /c
F90=df.exe
# ADD BASE F90 /include:"hdf_dll_/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaDbg/" /libs:dll /threads
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
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /D "_HDFDLL_" /D "_HDFLIB_" /YX /FD /c
F90=df.exe
# ADD BASE F90 /include:"hdf_dll0/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaRel/" /libs:dll /threads
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

SOURCE=..\..\..\..\hdf\src\cdeflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnbit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\crle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cskphuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cszip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfan.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfconvrt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfsd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfstubs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfufp2i.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\glist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\h4config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hchunks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hkit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mstdio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\patchlevel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vattr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vgint.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\hdf\src\atom.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_ATOM_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_ATOM_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\bitvect.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_BITVE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_BITVE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cdeflate.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_CDEFL=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_CDEFL=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnbit.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_CNBIT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_CNBIT=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnone.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_CNONE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_CNONE=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\crle.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_CRLE_=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_CRLE_=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cskphuff.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_CSKPH=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_CSKPH=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cszip.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\df24.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DF24_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfgr.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DF24_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfgr.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfan.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFAN_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfan.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFAN_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfan.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFCOM=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFCOM=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFCON=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFCON=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfgr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFGR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfgr.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFGR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfgr.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfgroup.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFGRO=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFGRO=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfimcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFIMC=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFIMC=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfjpeg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFJPE=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFJPE=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKCO=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKCO=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkcray.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKCR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKCR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkfuji.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKFU=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKFU=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfknat.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKNA=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKNA=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkswap.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKSW=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKSW=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkvms.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFKVM=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFKVM=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hconv.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFP_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFP_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfr8.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFR8_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dfrig.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFR8_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dfrig.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfrle.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFRLE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFRLE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfsd.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFSD_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dfsd.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFSD_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dfsd.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfstubs.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFSTU=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\df.h"\
	".\dfivms.h"\
	".\dfstubs.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFSTU=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\df.h"\
	".\dfivms.h"\
	".\dfstubs.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfunjpeg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFUNJ=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFUNJ=\
	"..\jpeg\jconfig.h"\
	"..\jpeg\jerror.h"\
	"..\jpeg\jmorecfg.h"\
	"..\jpeg\jpegint.h"\
	"..\jpeg\jpeglib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfutil.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DFUTI=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DFUTI=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dynarray.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_DYNAR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_DYNAR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\glist.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_GLIST=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\glist.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_GLIST=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\glist.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hbitio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HBITI=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HBITI=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hblocks.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HBLOC=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HBLOC=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hbuffer.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hchunks.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HCHUN=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HCHUN=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcomp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HCOMP=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HCOMP=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcompri.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hdfalloc.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HDFAL=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HDFAL=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\herr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HERR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HERR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hextelt.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HEXTE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HEXTE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfile.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HFILE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dir_mac.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\glist.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\sys_dir_mac.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HFILE=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dir_mac.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\glist.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\sys_dir_mac.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfiledd.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HFILED=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HFILED=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hkit.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_HKIT_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hkit.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_HKIT_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hkit.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mcache.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_MCACH=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_MCACH=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfan.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_MFAN_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfan.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_MFAN_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfan.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfgr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_MFGR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_MFGR_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mstdio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_MSTDI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_MSTDI=\
	"..\zlib\zconf.h"\
	"..\zlib\zlib.h"\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\cdeflate.h"\
	".\cnbit.h"\
	".\cnone.h"\
	".\crle.h"\
	".\cskphuff.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hcompi.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\mstdio.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\tbbt.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_TBBT_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_TBBT_=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vattr.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VATTR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VATTR=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vconv.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VCONV=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VCONV=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vg.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VG_C5e=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VG_C5e=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vgp.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VGP_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VGP_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vhi.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VHI_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VHI_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vio.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VIO_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VIO_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vparse.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VPARS=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VPARS=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vrw.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VRW_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VRW_C=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vsfld.c

!IF  "$(CFG)" == "hdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaDbg"

NODEP_CPP_VSFLD=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "hdf_dll - Win32 AlphaRel"

NODEP_CPP_VSFLD=\
	".\api_adpt.h"\
	".\atom.h"\
	".\bitvect.h"\
	".\dfivms.h"\
	".\dynarray.h"\
	".\fmpio.h"\
	".\hbitio.h"\
	".\hchunks.h"\
	".\hcomp.h"\
	".\hdf.h"\
	".\hdfi.h"\
	".\HDFIOTrace.h"\
	".\herr.h"\
	".\hfile.h"\
	".\hlimits.h"\
	".\hntdefs.h"\
	".\hproto.h"\
	".\hqueue.h"\
	".\htags.h"\
	".\linklist.h"\
	".\maldebug.h"\
	".\mcache.h"\
	".\mfgr.h"\
	".\ProcIDs.h"\
	".\tbbt.h"\
	".\trace.h"\
	".\vg.h"\
	".\vgint.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
