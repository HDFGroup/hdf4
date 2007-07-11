# Microsoft Developer Studio Project File - Name="ncdump" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=ncdump - Win32 AlphaDbg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ncdump.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ncdump.mak" CFG="ncdump - Win32 AlphaDbg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ncdump - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ncdump - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ncdump - Win32 AlphaRel" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "ncdump - Win32 AlphaDbg" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ncdump - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ncdump\Release"
# PROP BASE Intermediate_Dir ".\ncdump\Release"
# PROP BASE Target_Dir ".\ncdump"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ncdump\Release"
# PROP Intermediate_Dir ".\ncdump\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\ncdump"
F90=df.exe
# ADD BASE F90 /include:".\ncdump\Release/" /I "ncdump\Release/"
# ADD F90 /include:".\ncdump\Release/" /I "ncdump\Release/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /Zi /I "hdf\src" /I "mfhdf\xdr" /I "mfhdf\libsrc" /I "mfhdf\ncgen" /D "_CONSOLE" /D "HDF" /D "NO_SYS_XDR_INC" /D "NDEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /stack:0x1f4000 /subsystem:console /incremental:yes /machine:I386 /libpath:".\windows\lib\release\singlethreaded"

!ELSEIF  "$(CFG)" == "ncdump - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\ncdump\Debug"
# PROP BASE Intermediate_Dir ".\ncdump\Debug"
# PROP BASE Target_Dir ".\ncdump"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\ncdump\Debug"
# PROP Intermediate_Dir ".\ncdump\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\ncdump"
F90=df.exe
# ADD BASE F90 /include:".\ncdump\Debug/" /I "ncdump\Debug/"
# ADD F90 /dbglibs /include:".\ncdump\Debug/" /I "ncdump\Debug/"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /I "mfhdf\xdr" /I "mfhdf\libsrc" /I "mfhdf\ncgen" /I "hdf\src" /D "_CONSOLE" /D "HDF" /D "NO_SYS_XDR_INC" /D "_DEBUG" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /stack:0x1f4000 /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /libpath:".\windows\lib\debug\singlethreaded"

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ncdump\AlphaRel"
# PROP BASE Intermediate_Dir "ncdump\AlphaRel"
# PROP BASE Target_Dir "ncdump"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ncdump\AlphaRel"
# PROP Intermediate_Dir "ncdump\AlphaRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "ncdump"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /O2 /I ".\hdf\src" /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I ".\mfhdf\ncgen" /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /O2 /I ".\hdf\src" /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I ".\mfhdf\ncgen" /I "hdf/src" /I "hdf\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
F90=df.exe
# ADD BASE F90 /compile_only /include:"ncdump\AlphaRel/" /nologo /warn:nofileopt /I "ncdump\Release/"
# ADD F90 /compile_only /include:"ncdump\AlphaRel/" /nologo /warn:nofileopt /I "ncdump\Release/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib src\release\src.lib jpeg\release\jpeg.lib zlib\release\zlib.lib xdr\release\xdr.lib libsrc\release\libsrc.lib getopt\release\getopt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /machine:ALPHA
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /machine:ALPHA /out:"windows\bin\release\ncdump.exe"

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ncdump\AlphaDbg"
# PROP BASE Intermediate_Dir "ncdump\AlphaDbg"
# PROP BASE Target_Dir "ncdump"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ncdump\AlphaDbg"
# PROP Intermediate_Dir "ncdump\AlphaDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "ncdump"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I ".\mfhdf\ncgen" /I "hdf\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "DOS_FS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".\hdf\zlib" /I ".\hdf\jpeg" /I ".\mfhdf\xdr" /I ".\mfhdf\libsrc" /I ".\mfhdf\ncgen" /I "hdf\src" /D "_CONSOLE" /D "_MBCS" /D "HDF" /D "NO_SYS_XDR_INC" /D "WIN32" /D "_DEBUG" /D "DOS_FS" /FD /c
# SUBTRACT CPP /YX
F90=df.exe
# ADD BASE F90 /compile_only /dbglibs /debug:full /include:"ncdump\AlphaDbg/" /nologo /warn:nofileopt /I "ncdump\Debug/"
# ADD F90 /compile_only /dbglibs /debug:full /include:"ncdump\AlphaDbg/" /nologo /warn:nofileopt /I "ncdump\Debug/"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib src\debug\src.lib jpeg\debug\jpeg.lib zlib\debug\zlib.lib xdr\debug\xdr.lib libsrc\debug\libsrc.lib getopt\debug\getopt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:ALPHA /out:"windows\bin\debug\ncdump.exe"

!ENDIF 

# Begin Target

# Name "ncdump - Win32 Release"
# Name "ncdump - Win32 Debug"
# Name "ncdump - Win32 AlphaRel"
# Name "ncdump - Win32 AlphaDbg"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\mfhdf\ncdump\dumplib.c

!IF  "$(CFG)" == "ncdump - Win32 Release"

!ELSEIF  "$(CFG)" == "ncdump - Win32 Debug"

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaRel"

DEP_CPP_DUMPL=\
	".\hdf\src\hlimits.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaDbg"

DEP_CPP_DUMPL=\
	".\hdf\src\hlimits.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mfhdf\ncdump\dumplib.h
# End Source File
# Begin Source File

SOURCE=.\mfhdf\ncdump\ncdump.c

!IF  "$(CFG)" == "ncdump - Win32 Release"

!ELSEIF  "$(CFG)" == "ncdump - Win32 Debug"

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaRel"

DEP_CPP_NCDUM=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\local_nc.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	".\mfhdf\ncdump\ncdump.h"\
	".\mfhdf\ncdump\vardata.h"\
	".\mfhdf\xdr\xdr.h"\
	
NODEP_CPP_NCDUM=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\IOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaDbg"

DEP_CPP_NCDUM=\
	".\hdf\src\atom.h"\
	".\hdf\src\bitvect.h"\
	".\hdf\src\dfivms.h"\
	".\hdf\src\dynarray.h"\
	".\hdf\src\hbitio.h"\
	".\hdf\src\hchunks.h"\
	".\hdf\src\hcomp.h"\
	".\hdf\src\hdf.h"\
	".\hdf\src\hdfi.h"\
	".\hdf\src\herr.h"\
	".\hdf\src\hfile.h"\
	".\hdf\src\hlimits.h"\
	".\hdf\src\hntdefs.h"\
	".\hdf\src\hproto.h"\
	".\hdf\src\hqueue.h"\
	".\hdf\src\htags.h"\
	".\hdf\src\linklist.h"\
	".\hdf\src\maldebug.h"\
	".\hdf\src\mcache.h"\
	".\hdf\src\mfgr.h"\
	".\hdf\src\tbbt.h"\
	".\hdf\src\vg.h"\
	".\hdf\src\vgint.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\local_nc.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	".\mfhdf\ncdump\ncdump.h"\
	".\mfhdf\ncdump\vardata.h"\
	".\mfhdf\xdr\xdr.h"\
	
NODEP_CPP_NCDUM=\
	".\hdf\src\fmpio.h"\
	".\hdf\src\IOTrace.h"\
	".\hdf\src\ProcIDs.h"\
	".\ys\stat.h"\
	".\ys\types.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mfhdf\ncdump\ncdump.h
# End Source File
# Begin Source File

SOURCE=.\mfhdf\ncdump\vardata.c

!IF  "$(CFG)" == "ncdump - Win32 Release"

!ELSEIF  "$(CFG)" == "ncdump - Win32 Debug"

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaRel"

DEP_CPP_VARDA=\
	".\hdf\src\hlimits.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	".\mfhdf\ncdump\ncdump.h"\
	".\mfhdf\ncdump\vardata.h"\
	

!ELSEIF  "$(CFG)" == "ncdump - Win32 AlphaDbg"

DEP_CPP_VARDA=\
	".\hdf\src\hlimits.h"\
	".\mfhdf\libsrc\hdf2netcdf.h"\
	".\mfhdf\libsrc\netcdf.h"\
	".\mfhdf\ncdump\dumplib.h"\
	".\mfhdf\ncdump\ncdump.h"\
	".\mfhdf\ncdump\vardata.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mfhdf\ncdump\vardata.h
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
