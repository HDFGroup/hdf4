C-----------------------------------------------------------------------------
C Name: hestring
C Purpose: retrieves the error message associated with the specified error code 
C Inputs:  error_code 
C Outputs: error_message - string associated with the error code 
C Retruns: SUCCEED (0) if successful and FAIL(-1) otherwise
C-----------------------------------------------------------------------------*/

      integer function hestringf(error_code, error_message)
	!MS$if defined(BUILD_HDF_DLL)
	!MS$attributes dllexport :: hestringf
	!MS$endif
      integer error_code 
      character*(*) error_message 
C      integer hestringc 

      INTERFACE
        INTEGER FUNCTION hestringc(error_code, error_message, length)
          !MS$ATTRIBUTES C,reference,alias:'_HESTRINGC' :: hestringc
          integer error_code, length
	    character*(*) error_message
        END FUNCTION hestringc
      END INTERFACE
      hestringf = hestringc(error_code, error_message,
     +                      len(error_message))
      return
      end
C-----------------------------------------------------------------------------
C Name: heprnt
C Purpose: prints values from the error stack 
C Inputs:  print_levels - number of levels to print 
C Retruns: SUCCEED (0) if successful and FAIL(-1) otherwise
C
C Remarks: This routine always prints to the standard output.
C-----------------------------------------------------------------------------*/

      integer function heprnt(print_levels)
	!MS$if defined(BUILD_HDF_DLL)
	!MS$attributes dllexport :: heprnt
	!MS$endif
      integer print_levels 

      INTERFACE
        INTEGER FUNCTION heprntc(print_levels)
          !MS$ATTRIBUTES C,reference,alias:'_HEPRNTC' :: heprntc
          integer print_levels 
        END FUNCTION heprntc
      END INTERFACE
      heprnt = heprntc(print_levels)
      return
      end
