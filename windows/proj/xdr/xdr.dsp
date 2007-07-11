# Microsoft Developer Studio Project File - Name="xdr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xdr - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xdr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xdr.mak" CFG="xdr - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xdr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xdr - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xdr - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "xdr - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "xdr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\xdr\Release"
# PROP BASE Intermediate_Dir ".\xdr\Release"
# PROP BASE Target_Dir ".\xdr"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\xdr\Release"
# PROP Intermediate_Dir ".\xdr\Release"
# PROP Target_Dir ".\xdr"
F90=df.exe
# ADD BASE F90 /include:".\xdr\Release/" /I "xdr\Release/"
# ADD F90 /include:"Release/" /I "xdr\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\xdr\Debug"
# PROP BASE Intermediate_Dir ".\xdr\Debug"
# PROP BASE Target_Dir ".\xdr"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\xdr\Debug"
# PROP Intermediate_Dir ".\xdr\Debug"
# PROP Target_Dir ".\xdr"
F90=df.exe
# ADD BASE F90 /include:".\xdr\Debug/" /I "xdr\Debug/"
# ADD F90 /include:"Debug/" /I "xdr\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "xdr\AlphaDbg"
# PROP BASE Intermediate_Dir "xdr\AlphaDbg"
# PROP BASE Target_Dir "xdr"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir "xdr"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\hdf\jpeg" /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\hdf\jpeg" /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
F90=df.exe
# ADD BASE F90 /include:"xdr\AlphaDbg/" /I "xdr\Debug/"
# ADD F90 /include:"AlphaDbg/" /I "xdr\Debug/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "xdr\AlphaRel"
# PROP BASE Intermediate_Dir "xdr\AlphaRel"
# PROP BASE Target_Dir "xdr"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir "xdr"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /I ".\hdf\src" /I "hdf/src" /I "..\..\hdf\jpeg" /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /I ".\hdf\src" /I "hdf/src" /I "..\..\hdf\jpeg" /I "..\..\hdf\src" /I "..\..\mfhdf\xdr" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
F90=df.exe
# ADD BASE F90 /include:"xdr\AlphaRel/" /I "xdr\Release/"
# ADD F90 /include:"AlphaRel/" /I "xdr\Release/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "xdr - Win32 Release"
# Name "xdr - Win32 Debug"
# Name "xdr - Win32 AlphaDbg"
# Name "xdr - Win32 AlphaRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\mfhdf\xdr\types.h
# End Source File
# Begin Source File

SOURCE=..\..\mfhdf\xdr\xdr.c

!IF  "$(CFG)" == "xdr - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\mfhdf\xdr\xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\mfhdf\xdr\xdrarray.c

!IF  "$(CFG)" == "xdr - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\mfhdf\xdr\xdrfloat.c

!IF  "$(CFG)" == "xdr - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\mfhdf\xdr\xdrstdio.c

!IF  "$(CFG)" == "xdr - Win32 Release"

!ELSEIF  "$(CFG)" == "xdr - Win32 Debug"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "xdr - Win32 AlphaRel"

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
