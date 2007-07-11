# Microsoft Developer Studio Project File - Name="getopt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=getopt - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "getopt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "getopt.mak" CFG="getopt - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "getopt - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "getopt - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "getopt - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "getopt - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "getopt - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\getopt\Release"
# PROP BASE Intermediate_Dir ".\getopt\Release"
# PROP BASE Target_Dir ".\getopt"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\getopt\Release"
# PROP Intermediate_Dir ".\getopt\Release"
# PROP Target_Dir ".\getopt"
F90=df.exe
# ADD BASE F90 /include:".\getopt\Release/" /I "getopt\Release/"
# ADD F90 /include:".\getopt\Release/" /libdir:noauto /I "getopt\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /ZI /Od /I "hdf\src" /I "mfhdf\xdr" /I "mfhdf\libsrc" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /Zl /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "getopt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\getopt\Debug"
# PROP BASE Intermediate_Dir ".\getopt\Debug"
# PROP BASE Target_Dir ".\getopt"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\getopt\Debug"
# PROP Intermediate_Dir ".\getopt\Debug"
# PROP Target_Dir ".\getopt"
F90=df.exe
# ADD BASE F90 /include:".\getopt\Debug/" /I "getopt\Debug/"
# ADD F90 /dbglibs /include:".\getopt\Debug/" /libdir:noauto /I "getopt\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "mfhdf\xdr" /I "mfhdf\libsrc" /I "hdf\src" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /Zl /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "getopt - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "getopt\AlphaRel"
# PROP BASE Intermediate_Dir "getopt\AlphaRel"
# PROP BASE Target_Dir "getopt"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "getopt\AlphaRel"
# PROP Intermediate_Dir "getopt\AlphaRel"
# PROP Target_Dir "getopt"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\src" /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\src" /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /Zl /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"getopt\AlphaRel/" /nologo /warn:nofileopt /I "getopt\Release/"
# ADD F90 /compile_only /include:"getopt\AlphaRel/" /libdir:noauto /nologo /warn:nofileopt /I "getopt\Release/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "getopt - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "getopt\AlphaDbg"
# PROP BASE Intermediate_Dir "getopt\AlphaDbg"
# PROP BASE Target_Dir "getopt"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "getopt\AlphaDbg"
# PROP Intermediate_Dir "getopt\AlphaDbg"
# PROP Target_Dir "getopt"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I "hdf\src" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /Zl /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"getopt\AlphaDbg/" /nologo /warn:nofileopt /I "getopt\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"getopt\AlphaDbg/" /libdir:noauto /nologo /warn:nofileopt /I "getopt\Debug/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "getopt - Win32 Release"
# Name "getopt - Win32 Debug"
# Name "getopt - Win32 AlphaRel"
# Name "getopt - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\mfhdf\util\getopt.c

!IF  "$(CFG)" == "getopt - Win32 Release"

!ELSEIF  "$(CFG)" == "getopt - Win32 Debug"

!ELSEIF  "$(CFG)" == "getopt - Win32 AlphaRel"

!ELSEIF  "$(CFG)" == "getopt - Win32 AlphaDbg"

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
