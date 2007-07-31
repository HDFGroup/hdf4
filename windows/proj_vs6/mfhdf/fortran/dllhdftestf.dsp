# Microsoft Developer Studio Project File - Name="dllhdftestf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=dllhdftestf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dllhdftestf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dllhdftestf.mak" CFG="dllhdftestf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dllhdftestf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dllhdftestf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "dllhdftestf - Win32 AlphaRel" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "dllhdftestf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "dllhdftestf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\dllhdftestf\Release"
# PROP BASE Intermediate_Dir ".\dllhdftestf\Release"
# PROP BASE Target_Dir ".\dllhdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\fortran\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\fortran\Release\dllhdftestf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\dllhdftestf"
F90=df.exe
# ADD BASE F90 /compile_only /include:".\dllhdftestf\Release/" /nologo /I "..\..\..\..\dllhdftestf\Release/"
# ADD F90 /compile_only /define:"_HDFDLL_" /define:"BUILD_HDF_DLL" /include:".\dllhdftestf\Release/" /libs:dll /nologo /threads /I "..\..\..\..\dllhdftestf\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /Gi /GX /O2 /I "..\..\..\..\hdf\src" /D "_CONSOLE" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /libpath:".\windows\lib\release\multithreaded"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dllhdftestf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\dllhdftestf\Debug"
# PROP BASE Intermediate_Dir ".\dllhdftestf\Debug"
# PROP BASE Target_Dir ".\dllhdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\fortran\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\fortran\Debug\dllhdftestf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\dllhdftestf"
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\dllhdftestf\Debug/" /nologo /I "..\..\..\..\dllhdftestf\Debug/"
# ADD F90 /compile_only /debug:full /define:"BUILD_HDF_DLL" /define:"_HDFDLL_" /include:".\dllhdftestf\Debug/" /libs:dll /nologo /threads /pdbfile:"dllhdftestf\Debug/dllhdftestf.pdb" /I "..\..\..\..\dllhdftestf\Debug/"
# SUBTRACT F90 /dbglibs /libdir:noauto
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\..\..\hdf\src" /D "_CONSOLE" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /stack:0x1f4000 /subsystem:console /debug /machine:I386 /libpath:".\windows\lib\debug\multithreaded"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dllhdftestf - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dllhdftestf\AlphaRel"
# PROP BASE Intermediate_Dir "dllhdftestf\AlphaRel"
# PROP BASE Target_Dir "dllhdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dllhdftestf\AlphaRel"
# PROP Intermediate_Dir "dllhdftestf\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "dllhdftestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf/src" /I "..\..\..\..\hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\..\hdf/src" /I "..\..\..\..\hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"dllhdftestf\AlphaRel/" /nologo /warn:nofileopt /I "..\..\..\..\dllhdftestf\Release/"
# ADD F90 /compile_only /include:"dllhdftestf\AlphaRel/" /libs:dll /nologo /threads /warn:nofileopt /I "..\..\..\..\dllhdftestf\AlphaRel/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libsrc.lib xdr.lib src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /nodefaultlib:"libcd" /libpath:"libsrc/release" /libpath:"xdr/release" /libpath:"src/release" /libpath:"jpeg/release" /libpath:"zlib/release"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"libsrc/AlphaRel" /libpath:"xdr/AlphaRel" /libpath:"src/AlphaRel" /libpath:"jpeg/AlphaRel" /libpath:"zlib/AlphaRel"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "dllhdftestf - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dllhdftestf\AlphaDbg"
# PROP BASE Intermediate_Dir "dllhdftestf\AlphaDbg"
# PROP BASE Target_Dir "dllhdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dllhdftestf\AlphaDbg"
# PROP Intermediate_Dir "dllhdftestf\AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "dllhdftestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\..\..\..\hdf\src" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"dllhdftestf\AlphaDbg/" /nologo /warn:nofileopt /pdbfile:"dllhdftestf\Debug/dllhdftestf.pdb" /I "..\..\..\..\dllhdftestf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"dllhdftestf\AlphaDbg/" /libs:dll /nologo /threads /warn:nofileopt /pdbfile:"dllhdftestf\AlphaDbg/dllhdftestf.pdb" /I "..\..\..\..\dllhdftestf\AlphaDbg/"
# SUBTRACT F90 /libdir:noauto
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libsrc.lib xdr.lib src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA /nodefaultlib:"libcd" /libpath:"libsrc/debug" /libpath:"xdr/debug" /libpath:"src/debug" /libpath:"zlib/debug"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA /libpath:"libsrc/AlphaDbg" /libpath:"xdr/AlphaDbg" /libpath:"src/AlphaDbg" /libpath:"jpeg/AlphaDbg" /libpath:"zlib/AlphaDbg"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "dllhdftestf - Win32 Release"
# Name "dllhdftestf - Win32 Debug"
# Name "dllhdftestf - Win32 AlphaRel"
# Name "dllhdftestf - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\..\mfhdf\fortran\fort_ps\hdftestp.f
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
