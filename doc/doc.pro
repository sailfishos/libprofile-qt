# Stupid qmake wants to link everything.
QMAKE_LINK = @: IGNORE THIS LINE

QMAKE_EXTRA_TARGETS += doc
QMAKE_EXTRA_COMPILER += doc
QT -= gui

doc.target        = html/index.html
doc.config	 += no_check_exist no_link explicit_dependencies
doc.commands	  = doxygen doxyfile.cfg
doc.input         = $${doc.depends}
doc.output        = $${doc.target}

## Make are doc target build by default If 'make' is executed in doc folder
PRE_TARGETDEPS += $${doc.output}

# Install rules
htmldocs.files = html/*
equals(QT_MAJOR_VERSION, 4): htmldocs.path = /usr/share/doc/profile-qt
equals(QT_MAJOR_VERSION, 5): htmldocs.path = /usr/share/doc/profile-qt5
htmldocs.CONFIG += no_check_exist
INSTALLS += htmldocs
