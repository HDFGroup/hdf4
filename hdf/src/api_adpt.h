// api_adpt.h
//
// Project:  mfhdf_dll
// Copyright (C) Fortner Software LLC, 1998
// All rights reserved
// Author:	Russell Richards
// Created:	1998/7/14
// Revision History:
//	1998/7/14	Logic taken from modifications made by CDB 1997/7/30
//
/////////////////////////////////////////////////////////////////////////////
/**********
This header file provides the macros to adapt the HDF public functions to
dll entry points.

In addition it provides error lines if the configuration is incorrect.
**********/


#if defined( WIN32 )
/**********
Under WIN32 we have single threaded static libraries, or
mutli-threaded DLLs using the multithreaded runtime DLLs.
**********/
#	if defined(_MT) &&	defined(_DLL) &&!defined(_HDFDLL_)
/*		If the user really ment to use _HDFDLL_, but he forgot, just define it. */
#		define _HDFDLL_
#	endif

#	if defined(_MT) && !defined(_HDFDLL_)
#		error To use the HDF libraries from a multithreaded project, you must use the HDF DLLs
#		error Use the Mutlithreaded DLL runtime libraries (prefered), or define the macro "_HDFDLL_"
#	endif

#	if !defined(_MT) && defined(_HDFDLL_)
#		error To use the HDF libraries from a single-threaded project, you must use static HDF libraries
#		error Undefine the macro "_HDFDLL_"
#	endif

#	if defined(_HDFDLL_)
#		pragma warning( disable: 4273 )	/* Disable the stupid dll linkage warnings */

#		if !defined(_HDFLIB_)
#			define HDFPUBLIC __declspec(dllimport)
#		else
#			define HDFPUBLIC __declspec(dllexport)
#		endif

#		if !defined(_MFHDFLIB_) && !defined(_HDFLIB_)
#			define HDFLIBAPI __declspec(dllimport) extern
#		else
#			define HDFLIBAPI __declspec(dllexport) extern
#		endif 
#	else
#		define HDFPUBLIC
#		define HDFLIBAPI extern
#	endif
#else	/* !defined( WIN32 ) */
#	define HDFPUBLIC
#	define HDFLIBAPI extern
#endif