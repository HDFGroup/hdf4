# Release locations for lib and include
CMHDF_DIR	= /u/ncsa/acheng/CMHDF
HDF_INCLUDE	= ${CMHDF_DIR}/include
HDFLIBS		= ${CMHDF_DIR}/lib/libmfhdfcm5.a \
		    ${CMHDF_DIR}/lib/libdfcm5.a \
		    ${CMHDF_DIR}/lib/libjpeg.a \
		    ${CMHDF_DIR}/lib/libz.a

CSLIBS		= -lcmfs_cs -lcmfs_cm5
CMFLIBS		= -L/usr/cstar-7.2/lib -lcmfs_cs_cm5_vu_sp -lcs_cm5_vu_sp -lcmfs_cs_cm5_vu_pn -lcs_cm5_vu_pn

.SUFFIXES: .cs .fcm

DEBUG = 

CSFLAGS		= ${DEBUG} -I${HDF_INCLUDE} -DHDF
CMFFLAGS	= ${DEBUG}
CMFLDFLAGS	= ${DEBUG} 

all: Fexample example

example: example.o ${HDFLIBS}
	cs ${CSFLAGS} example.o -o $@ ${HDFLIBS} ${CSLIBS}

Fexample: Fexample.o ${HDFLIBS}
	cmf ${CMFFLAGS} Fexample.o -o $@ ${HDFLIBS} ${CMFLIBS}

clean:
	rm -f *.o example Fexample Cosmo.hdf sda/Cosmo.Quarant001.data

.fcm.o:
	cmf -c ${CMFFLAGS} $<

.cs.o:
	cs -c ${CSFLAGS} $<
