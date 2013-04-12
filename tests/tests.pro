equals(QT_MAJOR_VERSION, 4): system(cp tests.xml.template tests.xml)
equals(QT_MAJOR_VERSION, 5): system(sed -e s/qt-tests/qt5-tests/g tests.xml.template > tests.xml)

TEMPLATE = app
TARGET = ut_profile
equals(QT_MAJOR_VERSION, 4): target.path = /usr/lib/libprofile-qt-tests
equals(QT_MAJOR_VERSION, 5): target.path = /usr/lib/libprofile-qt5-tests
equals(QT_MAJOR_VERSION, 4): xml.path = /usr/share/libprofile-qt-tests
equals(QT_MAJOR_VERSION, 5): xml.path = /usr/share/libprofile-qt5-tests
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
