equals(QT_MAJOR_VERSION, 4): PACKAGENAME = libprofile-qt
equals(QT_MAJOR_VERSION, 5): PACKAGENAME = libprofile-qt5

system(sed -e s/@PACKAGENAME@/$${PACKAGENAME}/g tests.xml.template > tests.xml)

TEMPLATE = app
TARGET = ut_profile
target.path = /usr/lib/$${PACKAGENAME}-tests
xml.path = /usr/share/$${PACKAGENAME}-tests
xml.files = tests.xml

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

CONFIG += link_prl
QT += testlib dbus
QT -= gui
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
