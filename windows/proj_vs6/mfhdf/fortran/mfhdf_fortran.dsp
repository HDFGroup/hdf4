# Microsoft Developer Studio Project File - Name="mfhdf_fortran" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mfhdf_fortran - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf_fortran.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf_fortran.mak" CFG="mfhdf_fortran - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mfhdf_fortran - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mfhdf_fortran - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mfhdf_fortran - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\fortran\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\fortran\Release\mfhdf_fortran"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "mfhdf_fortran - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\fortran\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\fortran\Debug\mfhdf_fortran"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /compile_only /debug:full /include:"Release/" /nologo /warn:argument_checking /warn:nofileopt
# SUBTRACT F90 /check:bounds /dbglibs /traceback
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\mfhdf\fortran\Debug\mfhdf_fortrand.lib"

!ENDIF 

# Begin Target

# Name "mfhdf_fortran - Win32 Release"
# Name "mfhdf_fortran - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=..\..\..\..\mfhdf\fortran\fort_ps\mfsdpff.f
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# End Target
# End Project
