# REFLECTION = true
ifdef REFLECTION
CXX = /usr/local/bloomberg/bin/clang++
CXXFLAGS = -g -O2 -std=c++26 -stdlib=libc++ -freflection-latest -I/usr/local/bloomberg/include/c++/v1 -I../../classes/include
LDFLAGS  = -g -std=c++26 -stdlib=libc++ -freflection-latest
else
CXXFLAGS = -g -O2 -std=c++20 -I../../classes/include
LDFLAGS  = -g
endif

dwmwhat: dwmwhat.o
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cc
	${CXX} ${CXXFLAGS} -c $<

clean::
	rm -f *.o dwmwhat
