# Microsoft Developer Studio Project File - Name="hrepacktst" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=hrepacktst - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hrepacktst.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hrepacktst.mak" CFG="hrepacktst - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hrepacktst - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "hrepacktst - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hrepacktst - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\mfhdf\hrepack\Release"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\hrepack\Release\hrepacktst"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\hdiff" /D "NDEBUG" /D "_CONSOLE" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /machine:I386 /libpath:".\windows\lib\release\singlethreaded"

!ELSEIF  "$(CFG)" == "hrepacktst - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hrepacktst___Win32_Debug"
# PROP BASE Intermediate_Dir "hrepacktst___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\mfhdf\hrepack\Debug"
# PROP Intermediate_Dir "..\..\..\..\mfhdf\hrepack\Debug\hrepacktst"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"hrepacktst___Win32_Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD F90 /browser /check:bounds /compile_only /debug:full /include:".\hrepacktst\Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\..\mfhdf\xdr" /I "..\..\..\..\mfhdf\libsrc" /I "..\..\..\..\hdf\src" /I "..\..\..\..\mfhdf\hdiff" /D "_DEBUG" /D "_CONSOLE" /D "HDF" /D "NO_SYS_XDR_INC" /D "INTEL86" /D "WIN32" /D "DOS_FS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(HDF4_EXT_ZLIB) $(HDF4_EXT_SZIP) $(HDF4_EXT_JPEG) /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:".\windows\lib\debug\singlethreaded"

!ENDIF 

# Begin Target

# Name "hrepacktst - Win32 Release"
# Name "hrepacktst - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_array.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_dim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_gattr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_gr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_list.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_mattbl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_misc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_sds.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_vs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_an.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_dim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_gr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_list.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_lsttable.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_opttable.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_parse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_sds.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_sdutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_utils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_vg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_vs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepacktst.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepacktst_add.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepacktst_verify.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_array.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_mattbl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hdiff\hdiff_table.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_an.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_gr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_lsttable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_opttable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_sds.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_sdutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_vg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepack_vs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\hrepacktst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\mfhdf\hrepack\pal_rgb.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
