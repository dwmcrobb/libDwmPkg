load $(shell pkg-config --variable=libdir dwmgmk)/dwm_gmk.so(dwm_gmk_setup)
libIncDir := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

$(dwm_include $(abspath $(libIncDir)/../../Makefile.vars))

libIncHdrNames    := $(dwm_files $(libIncDir),.+\.hh)
libIncHdrs        := $(libIncHdrNames:%.hh=$(libIncDir)/%.hh)
libIncTarPrepHdrs := $(libIncHdrNames:%=${TARDIR}/include/libDwmPkg/%)

TARTARGETS        += $(libIncTarPrepHdrs)
DISTCLEANTARGETS  += $(libIncTarPrepHdrs)

ifeq ("$(CURDIR)","$(libIncDir)")
TARGETS           :=
CLEANTARGETS      :=
DEPSTARGETS       :=
TARTARGETS        := $(libIncTarPrepHdrs)
DISTCLEANTARGETS  := $(libIncTarPrepHdrs)
endif

${TARDIR}/include/libDwmPkg/%.hh: $(libIncDir)/%.hh
	$(abspath $(libIncDir)/../../install-sh) -c -m 444 $< $@

$(dwm_include $(abspath $(libIncDir)/../../Makefile.rules))
