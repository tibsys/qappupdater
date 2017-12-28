#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T18:38:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppUpdater
TEMPLATE = app

QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

SOURCES += main.cpp \
    Updater.cpp

HEADERS  += \
    Updater.h

FORMS    += MainWindow.ui

TRANSLATIONS += fr.ts

RESOURCES += \
    AppUpdater.qrc
