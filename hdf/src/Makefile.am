lib_LIBRARIES = libdf.a
libdf_a_SOURCES = atom.c bitvect.c cdeflate.c cnbit.c cnone.c crle.c    \
                  cskphuff.c df24.c df24f.c df24ff.f dfan.c dfanf.c     \
                  dfanff.f dfcomp.c dfconv.c dff.c dfff.f dfgr.c        \
                  dfgroup.c dfimcomp.c dfjpeg.c dfkconv.c dfkcray.c     \
                  dfkfuji.c dfknat.c dfkswap.c dfkvms.c dfp.c dfpf.c    \
                  dfpff.f dfr8.c dfr8f.c dfr8ff.f dfrle.c dfsd.c dfsdf.c\
                  dfsdff.f dfstubs.c dfufp2i.c dfufp2if.f dfunjpeg.c    \
                  dfutil.c dfutilf.c dynarray.c glist.c hbitio.c        \
                  hblocks.c hbuffer.c hchunks.c hcomp.c hcompri.c       \
                  hdfalloc.c herr.c herrf.c hextelt.c hfile.c hfiledd.c \
                  hfilef.c hfileff.f hkit.c linklist.c mcache.c mfan.c  \
                  mfanf.c mfgr.c mfgrf.c mfgrff.f mstdio.c tbbt.c       \
                  vattr.c vattrf.c vattrff.f vconv.c vg.c vgf.c vgff.f  \
                  vgp.c vhi.c vio.c vparse.c vrw.c vsfld.c
include_HEADERS = hfile.h hdf.h hdfi.h herr.h hproto.h hcomp.h hconv.h  \
                  hbitio.h tbbt.h crle.h cnbit.h mstdio.h cnone.h       \
                  cskphuff.h cdeflate.h hlimits.h hntdefs.h htags.h     \
                  hkit.h hcompi.h bitvect.h atom.h dynarray.h linklist.h\
                  hchunks.h vattr.h df.h dfi.h dfstubs.h dfufp2i.h      \
                  dfan.h dfrig.h dfgr.h dfsd.h vg.h mfan.h mfgr.h       \
                  dffunc.f90 hdf.f90 dffunc.inc hdf.inc

SUFFIXES = .inc .f90
.inc.f90:
	sed -e 's/^[cC]/!/' -e 's/^     [^ ]/     \&/' < $< > $*.f90
