# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32jpg.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
OUTDIR=.
INTDIR=.

ALL : $(OUTDIR)/win32jpg.lib $(OUTDIR)/win32jpg.bsc

# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR$(INTDIR)/\
 /Fo$(INTDIR)/ /c 
CPP_OBJS=
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"win32jpg.bsc" 
BSC32_SBRS= \
	$(INTDIR)/jdcolor.sbr \
	$(INTDIR)/jfdctint.sbr \
	$(INTDIR)/jcmainct.sbr \
	$(INTDIR)/jcsample.sbr \
	$(INTDIR)/jdsample.sbr \
	$(INTDIR)/jdcoefct.sbr \
	$(INTDIR)/jcmaster.sbr \
	$(INTDIR)/jfdctflt.sbr \
	$(INTDIR)/jccolor.sbr \
	$(INTDIR)/jutils.sbr \
	$(INTDIR)/jdmerge.sbr \
	$(INTDIR)/jdmarker.sbr \
	$(INTDIR)/jcomapi.sbr \
	$(INTDIR)/jquant2.sbr \
	$(INTDIR)/jidctfst.sbr \
	$(INTDIR)/jdapi.sbr \
	$(INTDIR)/jdpostct.sbr \
	$(INTDIR)/jdatasrc.sbr \
	$(INTDIR)/jerror.sbr \
	$(INTDIR)/jccoefct.sbr \
	$(INTDIR)/jcprepct.sbr \
	$(INTDIR)/jcdctmgr.sbr \
	$(INTDIR)/jddctmgr.sbr \
	$(INTDIR)/jfdctfst.sbr \
	$(INTDIR)/jcparam.sbr \
	$(INTDIR)/jmemmgr.sbr \
	$(INTDIR)/jcmarker.sbr \
	$(INTDIR)/jchuff.sbr \
	$(INTDIR)/jcapi.sbr \
	$(INTDIR)/jidctred.sbr \
	$(INTDIR)/jdmainct.sbr \
	$(INTDIR)/jidctint.sbr \
	$(INTDIR)/jdmaster.sbr \
	$(INTDIR)/jidctflt.sbr \
	$(INTDIR)/jdatadst.sbr \
	$(INTDIR)/jdhuff.sbr \
	$(INTDIR)/jquant1.sbr \
	$(INTDIR)/jmemansi.sbr \
	$(INTDIR)/ckconfig.sbr

$(OUTDIR)/win32jpg.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=lib.exe
DEF_FLAGS=
DEF_FILE=
LIB32_OBJS= \
	$(INTDIR)/jdcolor.obj \
	$(INTDIR)/jfdctint.obj \
	$(INTDIR)/jcmainct.obj \
	$(INTDIR)/jcsample.obj \
	$(INTDIR)/jdsample.obj \
	$(INTDIR)/jdcoefct.obj \
	$(INTDIR)/jcmaster.obj \
	$(INTDIR)/jfdctflt.obj \
	$(INTDIR)/jccolor.obj \
	$(INTDIR)/jutils.obj \
	$(INTDIR)/jdmerge.obj \
	$(INTDIR)/jdmarker.obj \
	$(INTDIR)/jcomapi.obj \
	$(INTDIR)/jquant2.obj \
	$(INTDIR)/jidctfst.obj \
	$(INTDIR)/jdapi.obj \
	$(INTDIR)/jdpostct.obj \
	$(INTDIR)/jdatasrc.obj \
	$(INTDIR)/jerror.obj \
	$(INTDIR)/jccoefct.obj \
	$(INTDIR)/jcprepct.obj \
	$(INTDIR)/jcdctmgr.obj \
	$(INTDIR)/jddctmgr.obj \
	$(INTDIR)/jfdctfst.obj \
	$(INTDIR)/jcparam.obj \
	$(INTDIR)/jmemmgr.obj \
	$(INTDIR)/jcmarker.obj \
	$(INTDIR)/jchuff.obj \
	$(INTDIR)/jcapi.obj \
	$(INTDIR)/jidctred.obj \
	$(INTDIR)/jdmainct.obj \
	$(INTDIR)/jidctint.obj \
	$(INTDIR)/jdmaster.obj \
	$(INTDIR)/jidctflt.obj \
	$(INTDIR)/jdatadst.obj \
	$(INTDIR)/jdhuff.obj \
	$(INTDIR)/jquant1.obj \
	$(INTDIR)/jmemansi.obj \
	$(INTDIR)/ckconfig.obj
# ADD BASE LIB32 /NOLOGO
# ADD LIB32 /NOLOGO
LIB32_FLAGS=/NOLOGO /OUT:$(OUTDIR)\"win32jpg.lib" 

$(OUTDIR)/win32jpg.lib : $(OUTDIR)  $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
OUTDIR=.
INTDIR=.

ALL : $(OUTDIR)/win32jpg.lib $(OUTDIR)/win32jpg.bsc

# ADD BASE CPP /nologo /W3 /GX /Z7 /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /c
# SUBTRACT CPP /YX /Fr
CPP_PROJ=/nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fo$(INTDIR)/ /c 
CPP_OBJS=
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"win32jpg.bsc" 
BSC32_SBRS= \
	

$(OUTDIR)/win32jpg.bsc : $(OUTDIR)  $(BSC32_SBRS)
LIB32=lib.exe
DEF_FLAGS=
DEF_FILE=
LIB32_OBJS= \
	$(INTDIR)/jdcolor.obj \
	$(INTDIR)/jfdctint.obj \
	$(INTDIR)/jcmainct.obj \
	$(INTDIR)/jcsample.obj \
	$(INTDIR)/jdsample.obj \
	$(INTDIR)/jdcoefct.obj \
	$(INTDIR)/jcmaster.obj \
	$(INTDIR)/jfdctflt.obj \
	$(INTDIR)/jccolor.obj \
	$(INTDIR)/jutils.obj \
	$(INTDIR)/jdmerge.obj \
	$(INTDIR)/jdmarker.obj \
	$(INTDIR)/jcomapi.obj \
	$(INTDIR)/jquant2.obj \
	$(INTDIR)/jidctfst.obj \
	$(INTDIR)/jdapi.obj \
	$(INTDIR)/jdpostct.obj \
	$(INTDIR)/jdatasrc.obj \
	$(INTDIR)/jerror.obj \
	$(INTDIR)/jccoefct.obj \
	$(INTDIR)/jcprepct.obj \
	$(INTDIR)/jcdctmgr.obj \
	$(INTDIR)/jddctmgr.obj \
	$(INTDIR)/jfdctfst.obj \
	$(INTDIR)/jcparam.obj \
	$(INTDIR)/jmemmgr.obj \
	$(INTDIR)/jcmarker.obj \
	$(INTDIR)/jchuff.obj \
	$(INTDIR)/jcapi.obj \
	$(INTDIR)/jidctred.obj \
	$(INTDIR)/jdmainct.obj \
	$(INTDIR)/jidctint.obj \
	$(INTDIR)/jdmaster.obj \
	$(INTDIR)/jidctflt.obj \
	$(INTDIR)/jdatadst.obj \
	$(INTDIR)/jdhuff.obj \
	$(INTDIR)/jquant1.obj \
	$(INTDIR)/jmemansi.obj \
	$(INTDIR)/ckconfig.obj
# ADD BASE LIB32 /NOLOGO
# ADD LIB32 /NOLOGO
LIB32_FLAGS=/NOLOGO /OUT:$(OUTDIR)\"win32jpg.lib" 

$(OUTDIR)/win32jpg.lib : $(OUTDIR)  $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\jdcolor.c

$(INTDIR)/jdcolor.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jfdctint.c

$(INTDIR)/jfdctint.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcmainct.c

$(INTDIR)/jcmainct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jversion.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcsample.c

$(INTDIR)/jcsample.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdsample.c

$(INTDIR)/jdsample.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdcoefct.c

$(INTDIR)/jdcoefct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcmaster.c

$(INTDIR)/jcmaster.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jfdctflt.c

$(INTDIR)/jfdctflt.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cderror.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jccolor.c

$(INTDIR)/jccolor.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jutils.c

$(INTDIR)/jutils.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdmerge.c

$(INTDIR)/jdmerge.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdmarker.c

$(INTDIR)/jdmarker.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcomapi.c

$(INTDIR)/jcomapi.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jquant2.c

$(INTDIR)/jquant2.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jidctfst.c

$(INTDIR)/jidctfst.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jinclude.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jpegint.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdapi.c

$(INTDIR)/jdapi.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdpostct.c

$(INTDIR)/jdpostct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdatasrc.c

$(INTDIR)/jdatasrc.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jerror.c

$(INTDIR)/jerror.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jccoefct.c

$(INTDIR)/jccoefct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcprepct.c

$(INTDIR)/jcprepct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcdctmgr.c

$(INTDIR)/jcdctmgr.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmemsys.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jddctmgr.c

$(INTDIR)/jddctmgr.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jerror.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jfdctfst.c

$(INTDIR)/jfdctfst.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcparam.c

$(INTDIR)/jcparam.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmemmgr.c

$(INTDIR)/jmemmgr.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcmarker.c

$(INTDIR)/jcmarker.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jchuff.c

$(INTDIR)/jchuff.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdct.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jcapi.c

$(INTDIR)/jcapi.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmorecfg.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jidctred.c

$(INTDIR)/jidctred.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jpeglib.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdmainct.c

$(INTDIR)/jdmainct.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jidctint.c

$(INTDIR)/jidctint.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdmaster.c

$(INTDIR)/jdmaster.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cdjpeg.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jidctflt.c

$(INTDIR)/jidctflt.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdatadst.c

$(INTDIR)/jdatadst.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jdhuff.c

$(INTDIR)/jdhuff.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jquant1.c

$(INTDIR)/jquant1.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmemansi.c

$(INTDIR)/jmemansi.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ckconfig.c

$(INTDIR)/ckconfig.obj :  $(SOURCE)  $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################
