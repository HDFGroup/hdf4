bin_PROGRAMS = ncgen
man1_MANS = ncgen.1

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

ncgen_SOURCES = close.c escapes.c generate.c genlib.c getfill.c init.c      \
                load.c main.c ncgentab.c
ncgen_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
ncgen_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
ncgen_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

ncgentab.c ncgentab.h: ncgen.h ncgen.y ncgenyy.c
	$(YACC) -d ${srcdir}/ncgen.y
	mv y.tab.c ncgentab.c
	mv y.tab.h ncgentab.h

ncgenyy.c: ncgen.l
	$(LEX) ${srcdir}/ncgen.l
	mv lex.yy.c ncgenyy.c

vms-stuff:	ncgentab.h ncgentab.c ncgenyy.c
	cp ${srcdir}/ncgentab.h vmstab.h
	cp ${srcdir}/ncgentab.c vmstab.c
	cp ${srcdir}/ncgenyy.c vms_yy.c

##
## Testing -- Here there be dragons.
##

NCDUMP=../ncdump/ncdump

check: ncgen ${srcdir}/test0.cdl b-check c-check f-check

#
# Test the "-b" option of ncgen
#
b-check:	ncgen ${srcdir}/test0.cdl test1.cdl
	./ncgen -b test1.cdl
	${NCDUMP} test1.nc > test2.cdl
	@${DIFF} test1.cdl test2.cdl && \
	    echo "*** ncgen -b test successful ***"

#
# Test the "-c" option of ncgen
#
c-check:	test2.cdl ctest0
	./ctest0        # tests `-c' option, creates ctest0.nc
	${NCDUMP} -n test1 ctest0.nc > ctest1.cdl
	@${DIFF} $(DIFF_FLAGS) test1.cdl ctest1.cdl && \
	    echo "*** $(PROGRAM) -c test successful ***"

#
# Test the "-f" option of ncgen
#
f-check:	test1.cdl ftest0
	./ftest0 ; \
	$(NCDUMP) -n test1 ftest0.nc > ftest1.cdl; \
	if $(DIFF) $(DIFF_FLAGS) test1.cdl ftest1.cdl; then \
	  echo "*** ncgen -f test successful ***"; \
	else \
	  echo "*** ncgen -f test failed (but roundoff differences are OK) ***"; \
	fi

ftest0:		ncgen test0.cdl netcdf.inc
	./ncgen -f -o ftest0.nc ${srcdir}/test0.cdl > test0.f; \
	  ${F77} ${FFLAGS} -o $@ test0.f ${ncgen_LDADD}

netcdf.inc:
	ln -s ../fortran/$@ .

test1.cdl:	test0.nc
	${NCDUMP} -n test1 test0.nc > $@

test0.nc:	ncgen ${srcdir}/test0.cdl
	./ncgen -b -o test0.nc ${srcdir}/test0.cdl

ctest0:		ncgen ${srcdir}/test0.cdl
	./ncgen -c -o ctest0.nc ${srcdir}/test0.cdl > test0.c
	  ${CC} ${INCLUDES} ${DEFINES} ${CPPFLAGS} ${CFLAGS} -o $@ test0.c ${ncgen_LDADD}
