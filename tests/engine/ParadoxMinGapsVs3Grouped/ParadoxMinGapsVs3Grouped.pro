QT       += testlib

QT       -= gui

TARGET = tst_paradoxmingapsvs3groupedtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include( ../GeneratePre/GeneratePre.pri )

SOURCES += \
        tst_paradoxmingapsvs3groupedtest.cpp 

DEFINES += SRCDIR=\\\"$$PWD/\\\" FET_COMMAND_LINE

INCLUDEPATH += $$PWD/../../../src/engine $$PWD/../../../src/interface
