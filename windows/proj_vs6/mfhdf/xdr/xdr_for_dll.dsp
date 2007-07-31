# Microsoft Developer Studio Project File - Name="xdr_for_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xdr_for_dll - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xdr_for_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xdr_for_dll.mak" CFG="xdr_for_dll - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xdr_for_dll - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xdr_for_dll - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xdr_for_dll - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "xdr_for_dll - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "xdr_for_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\xdr\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\xdr\Release\xdr_for_dll"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Release/"
# ADD F90 /include:"Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\xdr\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\xdr\Debug\xdr_for_dll"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Debug/"
# ADD F90 /include:"Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "xdr_for_"
# PROP BASE Intermediate_Dir "xdr_for_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /MDd /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Z7 /Od /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /MDd /c
F90=df.exe
# ADD BASE F90 /include:"xdr_for_/"
# ADD F90 /include:"AlphaDbg/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "xdr_for0"
# PROP BASE Intermediate_Dir "xdr_for0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\jpeg" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /include:"xdr_for0/"
# ADD F90 /include:"AlphaRel/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "xdr_for_dll - Win32 Release"
# Name "xdr_for_dll - Win32 Debug"
# Name "xdr_for_dll - Win32 AlphaDbg"
# Name "xdr_for_dll - Win32 AlphaRel"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\xdr.c

!IF  "$(CFG)" == "xdr_for_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\xdrarray.c

!IF  "$(CFG)" == "xdr_for_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\xdrfloat.c

!IF  "$(CFG)" == "xdr_for_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\xdr\xdrstdio.c

!IF  "$(CFG)" == "xdr_for_dll - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr_for_dll - Win32 AlphaRel"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
