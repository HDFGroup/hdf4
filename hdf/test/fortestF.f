C $Id$
C Interface to invoke tests for HDF Fortran interfaces.

	Program main
	implicit none

	common CleanUp
	logical CleanUp

	integer nerror, retcode
	character cmd*15, test*30

C
	CleanUp = .TRUE.

	nerror = 0
	call getcmd(cmd, test, retcode)
	do while (retcode .eq. 0)
	    call runcmd(cmd, test, retcode)
	    if (retcode .ne. 0) nerror = nerror + 1
	    call getcmd(cmd, test, retcode)
	end do

	if (nerror .ne. 0) then
	    print*, nerror, " tests have errors"
	else
	    print*, "All tests passed"
	endif

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

	retcode = 0

	read(*,*,END=100,err=100) cmd, test
	retcode = 0
	return

100	retcode =1
	return
	end
	    

C Run the Fortran test command.
C
	subroutine runcmd(cmd, test, retcode)
	character*(*) cmd, test
	integer retcode

	common CleanUp
	logical CleanUp

C
	retcode = 0

C Parse command types
	print *, "=========================================="
	print *, "* Command/test are: ", cmd, test
	print *, "=========================================="
	if (cmd .EQ. "Skip") then
	    return
	endif

	if (cmd .EQ. "Clean") then
	    CleanUp = .FALSE.
	    return
	endif
	    

	if (cmd .NE. "Test") then
	    print *, "Unknown Command: ", cmd, test
	    print *, 'Try one of "Skip", "Test", or "Clean"'
	    retcode = -1
	    return
	endif

C run the command
	if (test .EQ. "slab1") then
	    call slab1wf(retcode)
	    return
	endif

	if (test .EQ. "slab2") then
	    call slab2wf(retcode)
	    return
	endif

	if (test .EQ. "slab3") then
	    call slab3wf(retcode)
	    return
	endif

	if (test .EQ. "slab4") then
	    call slab4wf(retcode)
	    return
	endif

	if (test .EQ. "slab") then
	    call slabwf(retcode)
	    return
	endif

	if (test .EQ. "r24") then
	    call t24f(retcode)
	    return
	endif

	if (test .EQ. "an") then
	    call tanf(retcode)
	    return
	endif

	if (test .EQ. "anfile") then
	    call tanfilef(retcode)
	    return
	endif

	if (test .EQ. "manf") then
	    call manf(retcode)
	    return
	endif

	if (test .EQ. "mgrf") then
	    call mgrf(retcode)
	    return
	endif

	if (test .EQ. "p") then
	    call tpf(retcode)
	    return
	endif

	if (test .EQ. "r8") then
	    call tr8f(retcode)
	    return
	endif

	if (test .EQ. "sdmms") then
	    call tsdmmsf(retcode)
	    return
	endif

	if (test .EQ. "sdnmms") then
	    call tsdnmmsf(retcode)
	    return
	endif

	if (test .EQ. "sdnnt") then
	    call tsdnntf(retcode)
	    return
	endif

	if (test .EQ. "sdnt") then
	    call tsdntf(retcode)
	    return
	endif

	if (test .EQ. "sdstr") then
	    call tsdstrf(retcode)
	    return
	endif

	if (test .EQ. "stubs") then
	    call tstubsf(retcode)
	    return
	endif

C
	print *, "Unknown Command: ", cmd, test
	retcode = -1
	return
	end

