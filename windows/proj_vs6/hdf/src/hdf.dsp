# Microsoft Developer Studio Project File - Name="hdf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=hdf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hdf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdf.mak" CFG="hdf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdf - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "hdf - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "hdf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "hdf - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "hdf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\hdf\src\Release"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Release\hdf"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /include:".\Release/" /nologo /I "..\..\..\..\hdf\Release/"
# ADD F90 /compile_only /include:".\Release/" /nologo /I "..\..\..\..\hdf\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /O2 /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\hdf\src\Release\hd424.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\hdf___Wi"
# PROP BASE Intermediate_Dir ".\hdf___Wi"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\hdf\src\Debug"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Debug\hdf"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\hdf___Wi/" /nologo /I "..\..\..\..\hdf\hdf___Wi/"
# ADD F90 /compile_only /debug:full /include:".\Debug/" /nologo /I "..\..\..\..\hdf\hdf___Wi/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /ZI /Od /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\hdf\src\Debug\hd424d.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AlphaDbg"
# PROP BASE Intermediate_Dir "AlphaDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:"AlphaDbg/" /nologo /I "..\..\..\..\hdf\hdf___Wi/"
# ADD F90 /compile_only /debug:full /include:"AlphaDbg/" /nologo /I "..\..\..\..\hdf\hdf___Wi/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\hd412d.lib"
# ADD LIB32 /nologo /out:"..\lib\debug\singlethreaded\hd413d.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AlphaRel"
# PROP BASE Intermediate_Dir "AlphaRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\AlphaLib"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\zlib" /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"AlphaRel/" /nologo /I "..\..\..\..\hdf\Release/"
# ADD F90 /compile_only /include:"AlphaRel/" /nologo /I "..\..\..\..\hdf\Release/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\hd412.lib"
# ADD LIB32 /nologo /out:"..\lib\release\singlethreaded\hd413.lib" /NODEFAULTLIB

!ENDIF 

# Begin Target

# Name "hdf - Win32 Release"
# Name "hdf - Win32 Debug"
# Name "hdf - Win32 AlphaDbg"
# Name "hdf - Win32 AlphaRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\..\hdf\src\atom.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\bitvect.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cdeflate.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnbit.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cnone.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\crle.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cskphuff.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\cszip.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\df24.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfan.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfcomp.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfconv.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfgr.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfgroup.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfimcomp.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfjpeg.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkconv.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkcray.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkfuji.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfknat.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkswap.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfkvms.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfp.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfr8.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfrle.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfsd.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfstubs.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfunjpeg.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfutil.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dynarray.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\glist.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hbitio.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hblocks.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hbuffer.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hchunks.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcomp.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcompri.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hdfalloc.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\herr.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hextelt.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfile.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfiledd.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hkit.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mcache.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfan.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfgr.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mstdio.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\tbbt.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vattr.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vconv.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vg.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vgp.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vhi.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vio.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vparse.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vrw.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\vsfld.c

!IF  "$(CFG)" == "hdf - Win32 Release"

!ELSEIF  "$(CFG)" == "hdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "hdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\..\..\hdf\src\atom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\bitvect.h
# End Source File
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

SOURCE=..\..\..\..\hdf\src\df.h
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

SOURCE=..\..\..\..\hdf\src\dfivms.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dfrig.h
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

SOURCE=..\..\..\..\hdf\src\dir_mac.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\dynarray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\glist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\h4config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hbitio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hchunks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcomp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hcompi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hconv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hdf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hdfi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\herr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hkit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hlimits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hntdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hproto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\hqueue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\htags.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\linklist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\maldebug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mcache.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfan.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mfgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\mstdio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\patchlevel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\sys_dir_mac.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\tbbt.h
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
