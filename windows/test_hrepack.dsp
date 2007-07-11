# Microsoft Developer Studio Project File - Name="test_hrepack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test_hrepack - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "test_hrepack.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "test_hrepack.mak" CFG="test_hrepack - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test_hrepack - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test_hrepack - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "test_hrepack - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\test_hrepack\Release"
# PROP Intermediate_Dir ".\test_hrepack\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "hdf\src" /I "mfhdf\libsrc" /I "mfhdf\hrepack" /I "mfhdf\hdiff" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /out:"windows\bin\release\test_hrepack.exe"

!ELSEIF  "$(CFG)" == "test_hrepack - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\test_hrepack\Debug"
# PROP Intermediate_Dir ".\test_hrepack\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "hdf\src" /I "mfhdf\libsrc" /I "mfhdf\hrepack" /I "mfhdf\hdiff" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"windows\bin\debug\test_hrepack.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "test_hrepack - Win32 Release"
# Name "test_hrepack - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_array.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_gattr.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_gr.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_list.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_mattbl.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_misc.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_sds.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_table.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hdiff\hdiff_vs.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_an.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_cmp.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_gr.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_list.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_lsttable.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_opttable.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_pal.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_parse.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_sds.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_sdutil.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_utils.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_vg.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\hrepack_vs.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\test_hrepack_add.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\test_hrepack_main.c
# End Source File
# Begin Source File

SOURCE=.\mfhdf\hrepack\test_hrepack_verify.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
