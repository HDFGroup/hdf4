# Microsoft Developer Studio Project File - Name="mfhdf_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=mfhdf_dll - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf_dll.mak" CFG="mfhdf_dll - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mfhdf_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mfhdf_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mfhdf_dll - Win32 AlphaDbg" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "mfhdf_dll - Win32 AlphaRel" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\libsrc\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\libsrc\Release\mfhdf_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Release/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Release/" /libdir:noauto /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /D "SWAP" /D "HDFAPDLL" /YX /FD /c
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
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\mfhdf\libsrc\Release\hm423m.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\libsrc\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\libsrc\Debug\mfhdf_dll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Debug/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"Debug/" /libs:dll /optimize:0 /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /D "SWAP" /D "HDFAPDLL" /D "_HDFLIB_" /YX /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"..\..\..\..\mfhdf\libsrc\Debug\hm423md.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mfhdf_dl"
# PROP BASE Intermediate_Dir "mfhdf_dl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"mfhdf_dl/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaDbg/" /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /YX /FD /MDd /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /YX /FD /MDd /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"hm412md.dll" /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\dll\debug\hm413md.dll" /implib:"..\lib\debug\multithreaded/hm413md.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mfhdf_d0"
# PROP BASE Intermediate_Dir "mfhdf_d0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"mfhdf_d0/"
# ADD F90 /compile_only /define:"BUILD_HDF_DLL" /include:"AlphaRel/" /libs:dll /threads
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /D "_MFHDFLIB_" /D "_HDFDLL_" /YX /FD /c
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"hm412m.dll"
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\dll\release\hm413m.dll" /implib:"..\lib\release\multithreaded/hm413m.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "mfhdf_dll - Win32 Release"
# Name "mfhdf_dll - Win32 Debug"
# Name "mfhdf_dll - Win32 AlphaDbg"
# Name "mfhdf_dll - Win32 AlphaRel"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\local_nc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\mfhdf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\mfhdfi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\netcdf.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\array.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\attr.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\cdf.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\dim.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\error.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\file.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\globdef.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\hdfsds.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\iarray.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\mfsd.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\nssdc.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\putget.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\putgetg.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\sharray.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\string.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\var.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\xdrposix.c

!IF  "$(CFG)" == "mfhdf_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
