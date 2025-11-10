load $(shell pkg-config --variable=libdir dwmgmk)/dwm_gmk.so(dwm_gmk_setup)
classesDir := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

$(dwm_include $(classesDir)/tests/Makefile)
$(dwm_include $(classesDir)/include/Makefile)
