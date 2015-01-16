TEMPLATE = app

TARGET = CQListWidgetTest

QT += widgets

DEPENDPATH += .

#CONFIG += debug

# Input
SOURCES += \
CQListWidgetTest.cpp \

HEADERS += \
CQListWidgetTest.h \

DESTDIR     = .
OBJECTS_DIR = .

INCLUDEPATH += \
../include \
.

unix:LIBS += \
-L../lib \
-lCQListWidget
