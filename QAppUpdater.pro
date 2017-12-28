#-------------------------------------------------
#
# Project created by QtCreator 2012-02-12T16:28:01
#
#-------------------------------------------------

QT       += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): cache()

build_pass:CONFIG(release, debug|release) {
    message(Building Release)
    CONFIG += _release
    CONFIG -= _debug
    CONFIG -= debug
}
else:build_pass {
    message(Building Debug)
    CONFIG += _debug
    CONFIG -= release
    CONFIG -= _release
}
CONFIG(release, debug|release) {
    message(Building Release)
    CONFIG += _release
    CONFIG -= _debug
    CONFIG -= debug
}
else {
    message(Building Debug)
    CONFIG += _debug
    CONFIG -= release
    CONFIG -= _release
}

TEMPLATE = lib
debug:TARGET = QAppUpdaterd
release:TARGET = QAppUpdater
macx:CONFIG += staticlib
win32:CONFIG += staticlib
linux-g++:CONFIG += staticlib
linux-g++-64:CONFIG += staticlib
linux-clang:CONFIG += staticlib

unix:QMAKE_CXXFLAGS += -Wno-write-strings -pipe
linux-g++:QMAKE_CXXFLAGS_RELEASE += -ggdb
linux-g++:QMAKE_CFLAGS_RELEASE += -ggdb
linux-g++:QMAKE_LFLAGS_RELEASE = -ggdb
linux-clang:QMAKE_CXXFLAGS_RELEASE += -ggdb
linux-clang:QMAKE_CFLAGS_RELEASE += -ggdb
linux-clang:QMAKE_LFLAGS_RELEASE = -ggdb
macx:QMAKE_CXXFLAGS_RELEASE += -g -gdwarf-2
macx:QMAKE_CFLAGS_RELEASE += -g -gdwarf-2
macx:QMAKE_LFLAGS_RELEASE =
win32-msvc2010:QMAKE_CXXFLAGS_RELEASE += -MD -Zi
win32-msvc2010:QMAKE_LFLAGS_RELEASE += /DEBUG

DESTDIR = ../lib/

dll {
    DEFINES += _QAPPUPDATER_SHARED
}

staticlib {
    DEFINES += _QAPPUPDATER_STATIC
}

SOURCES += QAppUpdater.cpp \
    UpdaterDialogController.cpp

HEADERS += QAppUpdater.h \
    QAppUpdateInfo.hpp \
    QAppHttpClient.hpp \
    QAppUpdateFile.hpp \
    UpdaterDialogController.h \
    QAppPlatform.h \
    QAppUpdaterHelper.hpp

FORMS += \
    UpdaterDialog.ui

TRANSLATIONS += \
    i18n/QAppUpdater_fr.ts

RESOURCES += \
    QAppUpdater.qrc
