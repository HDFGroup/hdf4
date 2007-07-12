# Microsoft Developer Studio Project File - Name="hdftestf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=hdftestf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hdftestf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdftestf.mak" CFG="hdftestf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdftestf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "hdftestf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "hdftestf - Win32 AlphaRel" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "hdftestf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "hdftestf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\hdftestf\Release"
# PROP BASE Intermediate_Dir ".\hdftestf\Release"
# PROP BASE Target_Dir ".\hdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\hdftestf\Release"
# PROP Intermediate_Dir ".\hdftestf\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\hdftestf"
F90=df.exe
# ADD BASE F90 /compile_only /include:".\hdftestf\Release/" /nologo /I "hdftestf\Release/"
# ADD F90 /compile_only /include:".\hdftestf\Release/" /nologo /I "hdftestf\Release/"
# SUBTRACT F90 /threads
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gi /GX /O2 /I "hdf\src" /D "_CONSOLE" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /libpath:".\proj\lib\release\singlethreaded"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "hdftestf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\hdftestf\Debug"
# PROP BASE Intermediate_Dir ".\hdftestf\Debug"
# PROP BASE Target_Dir ".\hdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\hdftestf\Debug"
# PROP Intermediate_Dir ".\hdftestf\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\hdftestf"
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\hdftestf\Debug/" /nologo /I "hdftestf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:".\hdftestf\Debug/" /nologo /pdbfile:"hdftestf\Debug/hdftestf.pdb" /I "hdftestf\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "hdf\src" /D "_CONSOLE" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /stack:0x1f4000 /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:".\proj\lib\debug\singlethreaded"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "hdftestf - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "hdftestf\AlphaRel"
# PROP BASE Intermediate_Dir "hdftestf\AlphaRel"
# PROP BASE Target_Dir "hdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "hdftestf\AlphaRel"
# PROP Intermediate_Dir "hdftestf\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "hdftestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"hdftestf\AlphaRel/" /nologo /warn:nofileopt /I "hdftestf\Release/"
# ADD F90 /compile_only /include:"hdftestf\AlphaRel/" /nologo /warn:nofileopt /I "hdftestf\AlphaRel/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libsrc.lib xdr.lib src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /nodefaultlib:"libcd" /libpath:"libsrc/release" /libpath:"xdr/release" /libpath:"src/release" /libpath:"jpeg/release" /libpath:"zlib/release"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"libsrc/AlphaRel" /libpath:"xdr/AlphaRel" /libpath:"src/AlphaRel" /libpath:"jpeg/AlphaRel" /libpath:"zlib/AlphaRel"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "hdftestf - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hdftestf\AlphaDbg"
# PROP BASE Intermediate_Dir "hdftestf\AlphaDbg"
# PROP BASE Target_Dir "hdftestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "hdftestf\AlphaDbg"
# PROP Intermediate_Dir "hdftestf\AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "hdftestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "hdf\src" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"hdftestf\AlphaDbg/" /nologo /warn:nofileopt /pdbfile:"hdftestf\Debug/hdftestf.pdb" /I "hdftestf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"hdftestf\AlphaDbg/" /nologo /warn:nofileopt /pdbfile:"hdftestf\AlphaDbg/hdftestf.pdb" /I "hdftestf\AlphaDbg/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libsrc.lib xdr.lib src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA /nodefaultlib:"libcd" /libpath:"libsrc/debug" /libpath:"xdr/debug" /libpath:"src/debug" /libpath:"zlib/debug"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA /libpath:"libsrc/AlphaDbg" /libpath:"xdr/AlphaDbg" /libpath:"src/AlphaDbg" /libpath:"jpeg/AlphaDbg" /libpath:"zlib/AlphaDbg"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "hdftestf - Win32 Release"
# Name "hdftestf - Win32 Debug"
# Name "hdftestf - Win32 AlphaRel"
# Name "hdftestf - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\mfhdf\fortran\fort_ps\hdftestp.f
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
