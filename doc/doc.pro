system(doxygen doxyfile.cfg)

htmldocs.files = html/*
equals(QT_MAJOR_VERSION, 4): htmldocs.path = /usr/share/doc/profile-qt
equals(QT_MAJOR_VERSION, 5): htmldocs.path = /usr/share/doc/profile-qt5
INSTALLS += htmldocs
