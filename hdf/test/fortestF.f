C $Id$
C Interface to invoke tests for HDF Fortran interfaces.

	Program main
	implicit none
	include "fortest.inc"

	integer nerror, retcode
	character cmd*15, test*30

C Default to cleanup *.hdf files and set verbosity at 3
	CleanUp = .TRUE.
	Verbosity = 3

	nerror = 0
	call getcmd(cmd, test, retcode)
	do while (retcode .eq. 0)
	    call runcmd(cmd, test, retcode)
	    if (retcode .ne. 0) nerror = nerror + 1
	    call getcmd(cmd, test, retcode)
	end do

	print *, "====================================="
	if (nerror .ne. 0) then
	    print*, "Fortran tests have ", nerror, "errors"
	else
	    print*, "All fortran tests passed"
	endif
	print *, "====================================="

	if (CleanUp) call system("rm -f *.hdf")

	stop
	end


C Get a test command.
C Currently taking it from standard input.
C If EOF encounters, set retcode = 1.  Else retcode = 0.
C
	subroutine getcmd(cmd, test, retcode)
	character*(*) cmd, test
	integer retcode

	character*120 inline
	integer linelen, i

	retcode = 0

	read(*,11,END=100,err=100) inline
C	print *, "inline=", inline
	linelen = len(inline)
	i = index(inline, ' ')
	if (i .le. 0) i = linelen+1
	cmd = inline(1 : i - 1)

	do while (i .le. linelen)
	    if (inline(i:i) .eq. ' ') then
		i = i + 1
	    else
		goto 50
	    endif
	end do

50	test = inline(i:linelen)

C	print * , "cmd=", cmd, ", test=", test

	retcode = 0
	return

100	retcode =1
	return
C
11	format(A120)
	end
	    

C Run the Fortran test command.
C
	subroutine runcmd(cmd, param, retcode)
	implicit none
	character*(*) cmd, param

	include "fortest.inc"
	
	integer retcode
C
	retcode = 0

C Parse command types
	print *, "====================================="
	print *, cmd, param
	print *, "====================================="
	if (cmd .EQ. "Skip") then
	    return
	endif

	if (cmd .EQ. "Verbosity") then
	    Verbosity = index('0123456789', param(1:1)) - 1
	    return
	endif
	    
	if (cmd .EQ. "Cleanup") then
	    CleanUp = .FALSE.
	    return
	endif
	    

	if (cmd .NE. "Test") then
	    print *, "Unknown Command: ", cmd, param
	    print *, 'Try one of "Skip", "Test", "Verbosity" or "Cleanup"'
	    retcode = -1
	    return
	endif

C run the command
	if (param .EQ. "slab") then
	    call slabwf(retcode)
	    return
	endif

	if (param .EQ. "r24") then
	    call t24f(retcode)
	    return
	endif

	if (param .EQ. "an") then
	    call tanf(retcode)
	    return
	endif

	if (param .EQ. "anfile") then
	    call tanfilef(retcode)
	    return
	endif

	if (param .EQ. "manf") then
	    call manf(retcode)
	    return
	endif

	if (param .EQ. "mgrf") then
	    call mgrf(retcode)
	    return
	endif

	if (param .EQ. "p") then
	    call tpf(retcode)
	    return
	endif

	if (param .EQ. "r8") then
	    call tr8f(retcode)
	    return
	endif

	if (param .EQ. "sdmms") then
	    call tsdmmsf(retcode)
	    return
	endif

	if (param .EQ. "sdnmms") then
	    call tsdnmmsf(retcode)
	    return
	endif

	if (param .EQ. "sdnnt") then
	    call tsdnntf(retcode)
	    return
	endif

	if (param .EQ. "sdnt") then
	    call tsdntf(retcode)
	    return
	endif

	if (param .EQ. "sdstr") then
	    call tsdstrf(retcode)
	    return
	endif

	if (param .EQ. "vsetf") then
	    call tvsetf(retcode)
	    return
	endif

	if (param .EQ. "stubs") then
	    call tstubsf(retcode)
	    return
	endif

C
	print *, "Unknown Command: ", cmd, param
	retcode = -1
	return
	end

