system(doxygen doxyfile.cfg)

TEMPLATE = subdirs
QT -= gui

htmldocs.files = html/*
htmldocs.path = /usr/share/doc/libprofile-qt5
INSTALLS += htmldocs
