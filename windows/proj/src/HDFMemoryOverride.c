#include <windows.h>
#include <assert.h>
#include "hdf.h"

_declspec(dllexport) VOIDP HDmalloc(uint32 qty) 
{
	return ((VOIDP)GlobalAlloc(GMEM_FIXED, (DWORD)qty));
}

_declspec(dllexport) VOIDP HDcalloc(uint32 n, uint32 size)
{
	return ((VOIDP)GlobalAlloc(GPTR, (DWORD)n * (DWORD)size));
}

_declspec(dllexport) void HDfree(VOIDP ptr)
{
	if (ptr) GlobalFree((HGLOBAL)ptr);
}

_declspec(dllexport) VOIDP HDrealloc(VOIDP p, uint32 s)
{
	void* temp = NULL;

	if ( s ) {
		temp = (void *)GlobalReAlloc((HGLOBAL)p, s, 0);

		if ( !temp ) {
			temp = GlobalAlloc(GMEM_FIXED, s);
			if ( temp ) {
				DWORD	copyAmount = GlobalSize( (HGLOBAL)p );
				if ( copyAmount > s ) copyAmount = s;
				CopyMemory ( temp, p, copyAmount );
				GlobalFree( (HGLOBAL) p );
			}
		}

		assert(temp);
	} else {
		// realloc down to 0 size is just a free.
		GlobalFree( (HGLOBAL)p );
	}
	return temp;
}

_declspec(dllexport) char* HDstrdup(const char *s)
{
	char * retVal = NULL;

	if ( s ) {
		long	size = lstrlen( s );
		retVal = (char *)GlobalAlloc(GMEM_FIXED, (DWORD)size+1);

		if ( NULL != retVal ) {
			lstrcpy( retVal, s );
		}
	}
	return retVal;
}

