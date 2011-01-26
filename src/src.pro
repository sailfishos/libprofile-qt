TEMPLATE = lib
TARGET = profile-qt
QT += dbus
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

PROFILE_PREFIX = .

DEFINES += PROFILE_QT
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = profile.h \
    profile_p.h \
    keys_nokia.h \
    profile_dbus.h
SOURCES = profile.cpp
DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]
target.files += lib/*
headers.path = $$(DEBIAN_DESTDIR)/usr/include/profile-qt
headers.files = Profile \
    profile.h
message("The library will be installed to: " $$target.path)
message("The headers will be installed to: " $$headers.path)
INSTALLS += target \
    headers
