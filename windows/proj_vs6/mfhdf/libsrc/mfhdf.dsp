# Microsoft Developer Studio Project File - Name="mfhdf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mfhdf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mfhdf.mak" CFG="mfhdf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mfhdf - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mfhdf - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "mfhdf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "mfhdf - Win32 AlphaRel" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "mfhdf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\libsrc\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\libsrc\Release\mfhdf"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /include:".\Release/" /nologo /I "..\..\..\..\mfhdf\Release/"
# ADD F90 /compile_only /include:"Release/" /nologo /I "..\..\..\..\mfhdf\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\mfhdf\libsrc\Release\hm424.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\libsrc\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\libsrc\Debug\mfhdf"
# PROP Target_Dir "."
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\Debug/" /nologo /I "..\..\..\..\mfhdf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"Debug/" /nologo /I "..\..\..\..\mfhdf\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /ZI /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\mfhdf\libsrc\Debug\hm424d.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

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
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:"AlphaDbg/" /nologo /I "..\..\..\..\mfhdf\Debug/"
# ADD F90 /compile_only /debug:full /include:"AlphaDbg/" /nologo /I "..\..\..\..\mfhdf\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "_DEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\hm412d.lib"
# ADD LIB32 /nologo /out:"..\lib\debug\singlethreaded\hm413d.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

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
F90=df.exe
# ADD BASE F90 /compile_only /include:"AlphaRel/" /nologo /I "..\..\..\..\mfhdf\Release/"
# ADD F90 /compile_only /include:"AlphaRel/" /nologo /I "..\..\..\..\mfhdf\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /D "NDEBUG" /D "_WINDOWS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /D "_MFHDFLIB_" /D "SWAP" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\hm412.lib"
# ADD LIB32 /nologo /out:"..\lib\release\singlethreaded\hm413.lib" /NODEFAULTLIB

!ENDIF 

# Begin Target

# Name "mfhdf - Win32 Release"
# Name "mfhdf - Win32 Debug"
# Name "mfhdf - Win32 AlphaDbg"
# Name "mfhdf - Win32 AlphaRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\array.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\attr.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\cdf.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\dim.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\error.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\file.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\globdef.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\hdfsds.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\iarray.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\mfsd.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\nssdc.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\putget.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\putgetg.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\sharray.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\string.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\var.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\libsrc\xdrposix.c

!IF  "$(CFG)" == "mfhdf - Win32 Release"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 Debug"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaDbg"

!ELSEIF  "$(CFG)" == "mfhdf - Win32 AlphaRel"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
