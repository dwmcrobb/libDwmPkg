load $(shell pkg-config --variable=libdir dwmgmk)/dwm_gmk.so(dwm_gmk_setup)
testsMkFile := $(abspath $(lastword $(MAKEFILE_LIST)))
$(dwm_aliasfn my,dwm_my)
$(dwm_myns tests)
$(my mydir := $(abspath $(dir $(testsMkFile))))

# $(dwm_include_once $(abspath $(my mydir)/../src/Makefile))
$(dwm_include_once $(abspath $(my mydir)/../../Makefile.vars))

$(my CxxFlags   := ${CXXFLAGS} ${PTHREADCXXFLAGS} ${CLASSINC} ${EXTINCS})
# $(my Lib        := $(abspath $(my mydir)/../lib/libDwmPkg.la))
$(my Link       := ${LIBTOOL} --quiet --mode=link --tag=CXX ${CXX})
$(my Srcs       := $(dwm_files $(my mydir),Test.*\.cc))
$(my ObjNames   := $(subst .cc,.o,$(my Srcs)))
$(my ObjDir     := $(my mydir))
$(my DepsDir    := $(my mydir)/deps)
$(my Objs       := $(patsubst %,$(my ObjDir)/%,$(my ObjNames)))
$(my ObjDeps    := $(patsubst %.o,$(my DepsDir)/%_deps,$(my ObjNames)))
$(my Exes       := $(patsubst %.o,%,$(my Objs)))
$(my Clean      := $(my Exes))
$(my Clean      += $(patsubst %.o,$(my ObjDir)/.libs/%,$(my ObjNames)))

$(eval TARGETS          $(dwm_ifcwd :=,+=) $(my tests.Exes))
$(eval DEPSTARGETS      $(dwm_ifcwd :=,+=) $(my tests.ObjDeps))
$(eval CLEANTARGETS     $(dwm_ifcwd :=,+=) $(my tests.Clean,tests.Objs))
$(eval DISTCLEANTARGETS $(dwm_ifcwd :=,+=) $(my tests.ObjDeps))

$(dwm_include $(abspath $(my mydir)/../../Makefile.rules))

.SECONDARY: $(my tests.Objs) $(my tests.ObjDeps)

runtests: $(my Exes)
	@ for tp in $(my tests.Exes) ; do \
		printf "%-36s " `basename $$tp` ; \
		out=`$$tp 2>&1` ; \
		if [ $$? -eq 0 ]; then \
		  printf "%25s\n" "$$out" ; \
		else \
		  printf '\n%s\n' "$$out" ; \
		fi ; \
	done

#  generate dependency rule
$(eval $(dwm_cppdeps $(my tests.mydir)/deps,(my tests.mydir),\
$(my tests.mydir)/%.cc,${CXX} -MM $(my tests.CxxFlags) $<))

#  only include dependency makefiles if target is not a 'clean' target
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
ifneq ($(MAKECMDGOALS),clean-tests)
$(dwm_include $(my tests.ObjDeps))
endif
endif
endif

$(my mydir)/Test%.o: $(my mydir)/Test%.cc $(my DepsDir)/Test%_deps
	@dwmgmk_quiet "compiling $(dwm_relpwd $<)" \
	 ${CXX} $(my tests.CxxFlags) -c $< -o $@

$(my mydir)/Test%: $(my mydir)/Test%.o
	@dwmgmk_quiet "linking $(dwm_relpwd $@)" \
	 $(my tests.Link) ${LDFLAGS} -o $@ $^ ${EXTLIBS} ${PTHREADLDFLAGS}

