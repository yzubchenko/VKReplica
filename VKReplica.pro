#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T14:46:37
#
#-------------------------------------------------

#DEFINES += QT_NO_DEBUG_OUTPUT

QT       += core gui network webkitwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VKReplica
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connection/customnetworkmanager.cpp \
    auth.cpp \
    application.cpp \
    connection/cookiejar.cpp \
    connection/autosaver.cpp \
    errordialog.cpp \
    apimethodexecutor.cpp \
    contacts.cpp

HEADERS  += mainwindow.h \
    connection/customnetworkmanager.h \
    auth.h \
    application.h \
    connection/autosaver.h \
    connection/cookiejar.h \
    errordialog.h \
    apimethodexecutor.h \
    contacts.h

FORMS    += mainwindow.ui \
    errordialog.ui
