# Microsoft Developer Studio Project File - Name="hdf_fcstub" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=hdf_fcstub - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hdf_fcstub.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdf_fcstub.mak" CFG="hdf_fcstub - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdf_fcstub - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "hdf_fcstub - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\hdf\src\Release"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Release\hdf_fcstub"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gi /GX /O2 /I "..\..\..\..\hdf\src\fort_ps" /I "..\..\..\..\hdf\src" /D "NDEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\hdf\src\Debug"
# PROP Intermediate_Dir "..\..\..\..\hdf\src\Debug\hdf_fcstub"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# SUBTRACT F90 /dbglibs
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gi /GX /ZI /Od /I "..\..\..\..\hdf\src" /D "_DEBUG" /D "_WINDOWS" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_HDFLIB_" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\hdf\src\Debug\hdf_fcstubd.lib"

!ENDIF 

# Begin Target

# Name "hdf_fcstub - Win32 Release"
# Name "hdf_fcstub - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\herrpf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\hfilepf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\mfanpf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\mfgrpf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\vattrpf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\hdf\src\fort_ps\vgpf.c

!IF  "$(CFG)" == "hdf_fcstub - Win32 Release"

# SUBTRACT CPP /I "..\..\..\..\hdf\src\fort_ps"

!ELSEIF  "$(CFG)" == "hdf_fcstub - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# End Target
# End Project
