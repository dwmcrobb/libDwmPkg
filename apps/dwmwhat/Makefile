load $(shell pkg-config --variable=libdir dwmgmk)/dwm_gmk.so(dwm_gmk_setup)
myfile := $(abspath $(lastword $(MAKEFILE_LIST)))
$(dwm_myns dwmwhat)
$(dwm_aliasfn my,dwm_my)
$(my mydir := $(abspath $(dir $(myfile))))

$(dwm_include_once $(abspath $(my mydir)/../../Makefile.vars))

$(my CxxFlags    := ${CXXFLAGS} ${PTHREADCXXFLAGS})
$(my Incs        := -I$(abspath $(my mydir)/../../classes/include))
$(my Link        := ${LIBTOOL} --quiet --mode=link --tag=CXX ${CXX})
$(my ObjNames    := dwmwhat.o)
$(my Objs        := $(patsubst %.o,$(my mydir)/%.o,$(my ObjNames)))
$(my ObjDeps     := $(patsubst %.o,$(my mydir)/deps/%_deps,$(my ObjNames)))
$(my Targets     := $(my mydir)/dwmwhat)
$(my Clean       += $(my dwmwhat.Objs))
$(my LtClean     := $(my dwmwhat.Targets))
$(my TarTargets  := ${TARDIR}/bin/dwmwhat ${TARDIR}/man/man1/dwmwhat.1)

$(eval TARGETS          $(dwm_ifcwd :=,+=) $(my dwmwhat.Targets))
$(eval CLEANTARGETS     $(dwm_ifcwd :=,+=) $(my dwmwhat.Clean))
$(eval LTCLEANTARGETS   $(dwm_ifcwd :=,+=) $(my dwmwhat.LtClean))
$(eval DEPSTARGETS      $(dwm_ifcwd :=,+=) $(my dwmwhat.ObjDeps))
$(eval DISTCLEANTARGETS $(dwm_ifcwd :=,+=) $(my dwmwhat.ObjDeps))
$(eval TARTARGETS       $(dwm_ifcwd :=,+=) $(my dwmwhat.TarTargets))

$(dwm_include $(abspath $(dir $(myfile))/../../Makefile.rules))

ifeq ("${MANDOC}","mandoc")
MANHTML = ${MANDOC} -Thtml -Ostyle=../mcplexman.css -Oman=../html%S/%N.%S.html
TARTARGETS += ${TARDIR}/${HTMLMAN}/html1/dwmwhat.1.html
endif

$(my mydir)/dwmwhat: $(my Objs)
	@dwmgmk_quiet "linking $(dwm_relpwd $@)" \
	 $(my dwmwhat.Link) ${LDFLAGS} -rpath ${INSTALLPREFIX}/lib -o $@ $^ ${EXTLIBS} ${PTHREADLDFLAGS}

${TARDIR}/bin/dwmwhat: $(my mydir)/dwmwhat
	${INSTALL} -s -c -m 555 $< $@

${TARDIR}/man/man1/dwmwhat.1: $(my mydir)/dwmwhat.1
	${INSTALL} -c -m 644 $< $@

${TARDIR}/${HTMLMAN}/html1/dwmwhat.1.html: $(my mydir)/dwmwhat.1
	${MANHTML} $< > $<.html
	${INSTALL} -c -m 644 $<.html $@
	rm $<.html

#  generate dependency rule
$(eval $(dwm_cppdeps $(my dwmwhat.mydir)/deps,\
$(my dwmwhat.mydir),$(my dwmwhat.mydir)/%.cc,\
${CXX} -MM $(my dwmwhat.CxxFlags) $(my dwmwhat.Incs) -c $<))

#  only include dependency makefiles if target is not 'clean' or 'distclean'
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
$(dwm_include $(my ObjDeps))
endif
endif

$(my mydir)/%.o: $(my mydir)/%.cc $(my mydir)/deps/%_deps
	@dwmgmk_quiet "compiling $(dwm_relpwd $<)" \
	 ${CXX} $(my dwmwhat.CxxFlags) $(my dwmwhat.Incs) -c $< -o $@


