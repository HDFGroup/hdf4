C $Id$
C Common block definiton to be included by fortestF.f modules.  If your
C fortran compile cannot do include, you need to replace each include
C statement with this file.

	common CleanUp, Verbosity
	logical CleanUp
	integer Verbosity
C
C Predefined verbosity levels.
	integer VERBO_NONE, VERBO_DEF, VERBO_LO, VERBO_MED, VERBO_HI
	parameter (
     +		VERBO_NONE = 0,
     +		VERBO_DEF  = 3,
     +		VERBO_LO   = 5,
     +		VERBO_MED  = 7,
     +		VERBO_HI   = 9
     +		)
