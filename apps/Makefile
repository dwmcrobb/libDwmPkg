load $(shell pkg-config --variable=libdir dwmgmk)/dwm_gmk.so(dwm_gmk_setup)

appsDir := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

$(dwm_my apps.subdirs := $(dwm_subdirs $(appsDir)))

$(dwm_include $(abspath $(appsDir)/dwmwhat/Makefile))

$(dwm_myns apps)
$(dwm_my apps := $(dwm_rgxreplace \ ,|,$(my apps.subdirs)))

#  Alias some functions just to make for less typing.
$(dwm_aliasfn my,dwm_my)
$(dwm_aliasfn myvrgx,dwm_myvrgx)
$(dwm_aliasfn ifcwd,dwm_ifcwd)

#  If make is being run from this directory, limit our targets to
#  things under this directory.
$(eval TARGETS          $(ifcwd :=,?=) $(myvrgx ($(my apps))\.Targets))
$(eval CLEANTARGETS     $(ifcwd :=,?=) $(myvrgx ($(my apps))\.Clean))
$(eval LTCLEANTARGETS   $(ifcwd :=,?=) $(myvrgx ($(my apps))\.LtClean))
$(eval DEPSTARGETS      $(ifcwd :=,?=) $(myvrgx ($(my apps))\.ObjDeps))
$(eval DISTCLEANTARGETS $(ifcwd :=,?=) $(myvrgx ($(my apps))\.ObjDeps))
$(eval TARTARGETS       $(ifcwd :=,?=) $(myvrgx ($(my apps))\.TarTargets))


