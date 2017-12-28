#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T18:03:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppLauncher
TEMPLATE = app

DEFINES += WIN32_LEAN_AND_MEAN
INCLUDEPATH += ../../../QAppUpdater
LIBS += -lShell32

SOURCES += main.cpp \
    Launcher.cpp

HEADERS  += \
    Launcher.h

FORMS    += MainWindow.ui

TRANSLATIONS += fr.ts

RESOURCES += \
    AppLauncher.qrc
