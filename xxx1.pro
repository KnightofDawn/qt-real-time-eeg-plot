#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T16:23:15
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = xxx1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datathread.cpp \
    serialthread.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    datathread.h \
    serialthread.h \
    qcustomplot.h

FORMS    += mainwindow.ui

RESOURCES += \
    xxx1.qrc
RC_FILE += 14.rc

DEFINES += QCUSTOMPLOT_USE_OPENGL
LIBS += -lOpenGL32



