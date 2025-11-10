include ../Makefile.vars

.PHONY : clean clean-install clean-tarprep install tarprep uninstall

html/index.html: ../classes/include/*.hh
	-doxygen Doxyfile
	cp mcplex_net_blue_shadowed.svg html

www: html/index.html
	tar --exclude=\*.md5 -zcvf html.tgz html

tarprep: html/index.html
	mkdir -p ${TARDIR}/share/doc/libDwmPkg
	find html -name \*.md5 -exec rm {} \;
	cp html/* ${TARDIR}/share/doc/libDwmPkg

install: ${DOCINSTALLDIR}/index.html

${DOCINSTALLDIR}/index.html: html/index.html
	mkdir -p ${DOCINSTALLDIR}
	cp html/* ${DOCINSTALLDIR}

uninstall: clean-install

clean:: clean-tarprep
	rm -Rf html latex man

clean-install::
	rm -Rf ${DOCINSTALLDIR}

clean-tarprep::
	rm -Rf ${TARDIR}/share/doc/libDwmPkg

distclean::
	rm -f Doxyfile
