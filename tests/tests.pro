TEMPLATE = app
TARGET = ut_profile
target.path = /usr/lib/libprofile-qt-tests
xml.path = /usr/share/libprofile-qt-tests
xml.files = tests.xml

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

CONFIG += link_prl
QT += testlib dbus
DEFINES += UNIT_TEST
QMAKE_EXTRA_TARGETS = check
check.depends = $$TARGET
check.commands = LD_LIBRARY_PATH=../../lib ./$$TARGET

INCLUDEPATH += ../src/

SOURCES += \
    ut_profile.cpp \
    ../src/profile.cpp

HEADERS += \
    ut_profile.h \
    ../src/profile.h \
    ../src/profile_p.h \
    ../src/Profile

INSTALLS += target xml
