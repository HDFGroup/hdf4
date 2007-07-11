# Microsoft Developer Studio Project File - Name="hdf_ap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=hdf_ap - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hdf_ap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdf_ap.mak" CFG="hdf_ap - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdf_ap - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "hdf_ap - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "hdf_ap - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "hdf_ap - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "hdf_ap - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\hdf_ap\Release"
# PROP Intermediate_Dir ".\hdf_ap\Release"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /include:".\Release/" /nologo /I "hdf_ap\Release/"
# ADD F90 /compile_only /include:".\Release/" /nologo /I "hdf_ap\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /O2 /I "hdf_ap\src" /D "_WINDOWS" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "hdf_ap - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\hdf_ap___Wi"
# PROP BASE Intermediate_Dir ".\hdf_ap___Wi"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\hdf_ap\Debug"
# PROP Intermediate_Dir ".\hdf_ap\Debug"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\hdf_ap___Wi/" /nologo /I "hdf_ap\hdf_ap___Wi/"
# ADD F90 /compile_only /dbglibs /debug:full /include:".\hdf_ap___Wi/" /nologo /I "hdf_ap\hdf_ap___Wi/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /Z7 /Od /I "hdf_ap\src" /D "_WINDOWS" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\debug\hdf_ap.lib"

!ELSEIF  "$(CFG)" == "hdf_ap - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AlphaRel"
# PROP BASE Intermediate_Dir "AlphaRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaRel"
# PROP Intermediate_Dir "AlphaRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf_ap/src" /I "hdf_ap\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf_ap/src" /I "hdf_ap\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"AlphaRel/" /nologo /warn:nofileopt /I "hdf_ap\Release/"
# ADD F90 /compile_only /include:"AlphaRel/" /nologo /warn:nofileopt /I "hdf_ap\Release/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "hdf_ap - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AlphaDbg"
# PROP BASE Intermediate_Dir "AlphaDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AlphaDbg"
# PROP Intermediate_Dir "AlphaDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "hdf_ap\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "hdf_ap\src" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"AlphaDbg/" /nologo /warn:nofileopt /I "hdf_ap\hdf_ap___Wi/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"AlphaDbg/" /nologo /warn:nofileopt /I "hdf_ap\hdf_ap___Wi/"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\debug\hdf_ap.lib"
# ADD LIB32 /nologo /out:".\debug\hdf_ap.lib"

!ENDIF 

# Begin Target

# Name "hdf_ap - Win32 Release"
# Name "hdf_ap - Win32 Debug"
# Name "hdf_ap - Win32 AlphaRel"
# Name "hdf_ap - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
