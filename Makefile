###
#
# @file Makefile
#
#  PLASMA is a software package provided by Univ. of Tennessee,
#  Univ. of California Berkeley and Univ. of Colorado Denver
#
# @version 2.8.0
# @author Jakub Kurzak
# @author Mathieu Faverge
# @date 2010-11-15
#
###

# Overwritten in make.inc
PLASMA_DIR = .
include ./Makefile.internal

LIBS=   libquark        \
        libplasma       \
        libcoreblas     \
        libcoreblasqw   \

all: lib test example timings

lib: ${LIBS} pkgconfig

libquark:
	(cd quark && $(MAKE) lib)

libcoreblas:
	(cd core_blas && $(MAKE))

libcoreblasqw:
	(cd core_blas-qwrapper && $(MAKE))

libplasma:
	(cd control && $(MAKE))
	(cd compute && $(MAKE))

test: testplasma testlapack

testplasma: lib
	(cd testing && $(MAKE))

testlapack: lib
	(cd testing/lin && $(MAKE))

example: lib
	(cd examples && $(MAKE))

timings: lib
	(cd timing && $(MAKE))

clean:
	(cd quark       && $(MAKE) clean )
	(cd core_blas   && $(MAKE) clean )
	(cd core_blas-qwrapper && $(MAKE) clean )
	(cd compute     && $(MAKE) clean )
	(cd control     && $(MAKE) clean )
	(cd testing     && $(MAKE) clean )
	(cd testing/lin && $(MAKE) clean )
	(cd examples    && $(MAKE) clean )
	(cd timing      && $(MAKE) clean )

cleanall:
	(cd quark       && $(MAKE) cleanall )
	(cd core_blas   && $(MAKE) cleanall )
	(cd core_blas-qwrapper && $(MAKE) cleanall )
	(cd compute     && $(MAKE) cleanall )
	(cd control     && $(MAKE) cleanall )
	(cd testing     && $(MAKE) cleanall )
	(cd testing/lin && $(MAKE) cleanall )
	(cd examples    && $(MAKE) cleanall )
	(cd timing      && $(MAKE) cleanall )
	(cd docs        && $(MAKE) cleanall )
	(cd lib         && rm -f *.a )

cleanmake:
	find . -name "Makefile.src" -exec rm -rf {} \;

##############################################################
#            Trace: available only if $PLASMA_TRACE = 1

ifeq (${PLASMA_TRACE}, 1)
libeztrace-coreblas:
	(cd core_blas-eztrace && $(MAKE))

clean-libeztrace-coreblas:
	(cd core_blas-eztrace && $(MAKE) clean)

cleanall-libeztrace-coreblas:
	(cd core_blas-eztrace && $(MAKE) cleanall)

install-libeztrace-coreblas: libeztrace-coreblas dir
	cp $(LIBEZT_COREBLAS) $(prefix)/lib
	cp $(LIBEZT_CONVERT)  $(prefix)/lib

lib     : libeztrace-coreblas
clean   : clean-libeztrace-coreblas
cleanall: cleanall-libeztrace-coreblas
install : install-libeztrace-coreblas
endif

##############################################################
#            Installation

pkgconfig-coreblas:
	cat $(PLASMA_DIR)/lib/pkgconfig/coreblas.pc.in | \
	    sed -e s:\__PREFIX:"$(prefix)":          | \
	    sed -e s:\__LIBEXT:"$(LIBEXT)":          | \
	    sed -e s:\__INCLUDE_EXT:"$(INCEXT)":     | \
	    sed -e s:\__REQUIRE:"$(require)":          \
	    > $(PLASMA_DIR)/lib/pkgconfig/coreblas.pc

pkgconfig-plasma:
	cat $(PLASMA_DIR)/lib/pkgconfig/plasma.pc.in | \
	    sed -e s:\__PREFIX:"$(prefix)":          | \
	    sed -e s:\__LIBEXT:"$(LIBEXT)":          | \
	    sed -e s:\__INCLUDE_EXT:"$(INCEXT)":     | \
	    sed -e s:\__REQUIRE:"$(require)":          \
	    > $(PLASMA_DIR)/lib/pkgconfig/plasma.pc

pkgconfig: pkgconfig-coreblas pkgconfig-plasma

dir:
	mkdir -p $(prefix)
	mkdir -p $(prefix)/include
	mkdir -p $(prefix)/lib
	mkdir -p $(prefix)/lib/pkgconfig

install-coreblas: libcoreblas pkgconfig-coreblas dir
	cp $(PLASMA_DIR)/include/cblas.h       $(prefix)/include
	cp $(PLASMA_DIR)/include/plasmatypes.h $(prefix)/include
	cp $(PLASMA_DIR)/include/core_*.h      $(prefix)/include
	cp $(LIBCOREBLAS)              $(prefix)/lib
#       pkgconfig coreblas
	cp $(PLASMA_DIR)/lib/pkgconfig/coreblas.pc $(prefix)/lib/pkgconfig/coreblas.pc

install-plasma: ${LIBS} pkgconfig-plasma dir
	cp $(PLASMA_DIR)/include/*.h   $(prefix)/include
ifeq (${PLASMA_F90}, 1)
	cp $(PLASMA_DIR)/include/*.mod $(prefix)/include
endif
	cp $(LIBCOREBLAS)              $(prefix)/lib
	cp $(LIBCOREBLASQW)            $(prefix)/lib
	cp $(LIBPLASMA)                $(prefix)/lib
#       pkgconfig plasma
	cp $(PLASMA_DIR)/lib/pkgconfig/plasma.pc $(prefix)/lib/pkgconfig/plasma.pc

install-quark:
	(cd $(QUARKDIR) && make install)

install: install-coreblas install-plasma install-quark

include Makefile.tau
