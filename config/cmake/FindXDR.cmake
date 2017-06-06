# Try to find the XDR library and set some required variables
#
# Once run this will define:
#
# XDR_FOUND                    = system has XDR lib
#
# XDR_LIBRARIES            = full path to the libraries, if required
#

INCLUDE(CheckIncludeFile)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckLibraryExists)

## First try to find the required header files (rpc/types.h, rpc/xdr.h)

#check for the XDR functions: their interface and the libraries they're hidden in.
CHECK_INCLUDE_FILE(rpc/types.h XDR_HAS_RPC_TYPES_H)
IF(NOT XDR_HAS_RPC_TYPES_H)
    MESSAGE(STATUS "Cannot find RPC headers (rpc/types.h).")
ENDIF()
CHECK_INCLUDE_FILE(rpc/xdr.h XDR_HAS_RPC_XDR_H)
IF(NOT XDR_HAS_RPC_XDR_H)
    MESSAGE(STATUS "Cannot find RPC headers (rpc/xdr.h).")
ENDIF()

IF (XDR_HAS_RPC_TYPES_H AND XDR_HAS_RPC_XDR_H)
    ## Now let's see if we need an extra lib to compile it
    SET(XDR_INT_FOUND)
    CHECK_FUNCTION_EXISTS(xdr_int XDR_INT_FOUND)
    IF (NOT XDR_INT_FOUND)
        FOREACH(lib nsl rpc)
            ## Try to find the corresponding lib
            SET(XDR_INT_LIBRARY)
            FIND_LIBRARY(XDR_INT_LIBRARY ${lib})

            IF (XDR_INT_LIBRARY)
                CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY} xdr_int "" XDR_INT_SYMBOL_FOUND)
            ENDIF()
            IF (XDR_INT_SYMBOL_FOUND)
                SET(XDR_LIBRARIES ${XDR_INT_LIBRARY})
                SET(XDR_INT_FOUND TRUE)
                BREAK()
            ENDIF()
        ENDFOREACH()
    ENDIF()

    IF(NOT XDR_INT_FOUND)
        MESSAGE(SEND_ERROR "Could not locate xdr symbols in libc or libnsl.")
    ELSE()
        SET(XDR_FOUND TRUE)
    ENDIF()
ENDIF()
