#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

# These are the Fortran test programs to build.
TEST_PROG = sd_create

# These are the Fortran example files to be installed
INSTALL_FILES= sd_create.f

# Where to install example files
EXAMPLEDIR=$(prefix)/examples/fortran

# How to build Fortran programs using h4fc
$(EXTRA_PROG): $(H4FC)
	$(H4FC) $(H4FCFLAGS) $(FCFLAGS) -o $@ $(srcdir)/$@.f;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal Fortran compiler, not h4fc.
sd_create: $(srcdir)/sd_create.f

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = *.chkexe *.chklog
