TEMPLATE = lib
TARGET = profile-qt$${QT_MAJOR_VERSION}
QT += dbus
QT -= gui
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

unix:QMAKE_CXXFLAGS *= -fvisibility=hidden -fvisibility-inlines-hidden

PROFILE_PREFIX = .

DEFINES += PROFILE_QT
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = profile.h \
    profile_p.h \
    keys_nokia.h \
    profile_dbus.h
SOURCES = profile.cpp
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]
headers.path = $$(DEBIAN_DESTDIR)/usr/include/$$TARGET
headers.files = Profile \
    profile.h
message("The library will be installed to: " $$target.path)
message("The headers will be installed to: " $$headers.path)
INSTALLS += target \
    headers
