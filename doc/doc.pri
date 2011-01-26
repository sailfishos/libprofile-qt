DOXYGEN_BIN=doxygen

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH"
} else {
    doc.commands = $${DOXYGEN_BIN} ./doc/doxyfile.cfg ;
    doc.commands += ./doc/xmlize.pl ;

    # Install rules
    htmldocs.files = ./doc/html/*

    htmldocs.path = /usr/share/doc/libprofile-qt
    htmldocs.CONFIG += no_check_exist
    INSTALLS += htmldocs
}
doc.depends = FORCE
