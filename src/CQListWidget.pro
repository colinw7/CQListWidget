TEMPLATE = lib

TARGET = CQListWidget

QT += widgets

DEPENDPATH += .

CONFIG += staticlib

# Input
HEADERS += \
../include/CQListWidget.h \
CQListWidgetColumnsDialog.h \

SOURCES += \
CQListWidget.cpp \
CQListWidgetColumnsDialog.cpp \

OBJECTS_DIR = ../obj

DESTDIR = ../lib

INCLUDEPATH += \
. \
../include \
