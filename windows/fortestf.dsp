# Microsoft Developer Studio Project File - Name="fortestf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=fortestf - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fortestf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fortestf.mak" CFG="fortestf - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fortestf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "fortestf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "fortestf - Win32 AlphaDbg" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "fortestf - Win32 AlphaRel" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "fortestf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\fortestf\Release"
# PROP BASE Intermediate_Dir ".\fortestf\Release"
# PROP BASE Target_Dir ".\fortestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\fortestf\Release"
# PROP Intermediate_Dir ".\fortestf\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\fortestf"
F90=df.exe
# ADD BASE F90 /compile_only /include:".\fortestf\Release/" /nologo /I "fortestf\Release/"
# ADD F90 /compile_only /include:".\fortestf\Release/" /include:".\hdf\test" /nologo /I "fortestf\Release/"
# SUBTRACT F90 /libdir:noauto /threads
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /libpath:".\windows\lib\release\singlethreaded" /libpath:".\libtest_fortran\libtest_fortran.lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "fortestf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\fortestf\Debug"
# PROP BASE Intermediate_Dir ".\fortestf\Debug"
# PROP BASE Target_Dir ".\fortestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\fortestf\Debug"
# PROP Intermediate_Dir ".\fortestf\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\fortestf"
F90=df.exe
# ADD BASE F90 /compile_only /debug:full /include:".\fortestf\Debug/" /nologo /I "fortestf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:".\fortestf\Debug/" /include:".\hdf\test" /nologo /I "fortestf\Debug/"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:".\windows\lib\debug\singlethreaded" /libpath:".\libtest_fortran\libtest_fortrand.lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "fortestf - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fortestf\AlphaDbg"
# PROP BASE Intermediate_Dir "fortestf\AlphaDbg"
# PROP BASE Target_Dir "fortestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fortestf\AlphaDbg"
# PROP Intermediate_Dir "fortestf\AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "fortestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "hdf\src" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"fortestf\AlphaDbg/" /nologo /warn:nofileopt /I "fortestf\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"fortestf\AlphaDbg/" /include:".\hdf\test" /nologo /warn:nofileopt /I "fortestf\Debug/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"src/debug" /libpath:"jpeg/debug" /libpath:"zlib/debug"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"src/AlphaDbg" /libpath:"jpeg/AlphaDbg" /libpath:"zlib/AlphaDbg"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "fortestf - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fortestf\AlphaRel"
# PROP BASE Intermediate_Dir "fortestf\AlphaRel"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "fortestf"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fortestf\AlphaRel"
# PROP Intermediate_Dir "fortestf\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "fortestf"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INTEL86" /D "DOS_FS" /YX /FD /c
F90=df.exe
# ADD BASE F90 /compile_only /include:"fortestf\AlphaRel/" /libdir:noauto /nologo /warn:nofileopt /I "fortestf\Release/"
# ADD F90 /compile_only /include:"fortestf\AlphaRel/" /include:".\hdf\test" /nologo /warn:nofileopt /I "fortestf\Release/"
# SUBTRACT F90 /dbglibs /libdir:noauto /winapp
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 src.lib jpeg.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /nodefaultlib:"libcd" /libpath:"src/release" /libpath:"jpeg/release" /libpath:"zlib/release"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /nodefaultlib:"libcd" /libpath:"src/AlphaRel" /libpath:"jpeg/AlphaRel" /libpath:"zlib/AlphaRel"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "fortestf - Win32 Release"
# Name "fortestf - Win32 Debug"
# Name "fortestf - Win32 AlphaDbg"
# Name "fortestf - Win32 AlphaRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\hdf\test\fort_ps\forsupffp.f
DEP_F90_FORSU=\
	".\hdf\src\hdf.inc"\
	".\hdf\test\fortest.inc"\
	
# End Source File
# Begin Source File

SOURCE=.\hdf\test\fort_ps\fortestFp.f
DEP_F90_FORTE=\
	".\hdf\test\fortest.inc"\
	
# End Source File
# Begin Source File

SOURCE=.\hdf\test\fort_ps\manpf.f
DEP_F90_MANPF=\
	".\hdf\test\fortest.inc"\
	
# End Source File
# Begin Source File

SOURCE=.\hdf\test\mgrf.f
DEP_F90_MGRF_=\
	".\hdf\test\fortest.inc"\
	
# End Source File
# Begin Source File

SOURCE=.\hdf\test\tvattrf.f
DEP_F90_TVATT=\
	".\hdf\test\fortest.inc"\
	
# End Source File
# Begin Source File

SOURCE=.\hdf\test\tvsetf.f
DEP_F90_TVSET=\
	".\hdf\src\dffunc.inc"\
	".\hdf\src\hdf.inc"\
	".\hdf\test\fortest.inc"\
	
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
