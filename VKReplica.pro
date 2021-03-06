#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T14:46:37
#
#-------------------------------------------------

#DEFINES += QT_NO_DEBUG_OUTPUT

QT       += core gui network webkitwidgets multimedia
CONFIG   += c++11

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
    apimethodexecutor.cpp \
    contactmodel.cpp \
    longpollexecutor.cpp \
    dialogmanager.cpp \
    dialog.cpp \
    contactdelegate.cpp \
    focustextedit.cpp \
    audioplayer.cpp \
    audiomodel.cpp \
    audiodelegate.cpp \
    draggableprogressbar.cpp \
    systemtrayicon.cpp

HEADERS  += mainwindow.h \
    connection/customnetworkmanager.h \
    auth.h \
    application.h \
    connection/autosaver.h \
    connection/cookiejar.h \
    apimethodexecutor.h \
    contactmodel.h \
    contactdelegate.h \
    longpollexecutor.h \
    dialogmanager.h \
    dialog.h \
    focustextedit.h \
    audioplayer.h \
    audiomodel.h \
    audiodelegate.h \
    draggableprogressbar.h \
    systemtrayicon.h

FORMS    += mainwindow.ui \
    dialogmanager.ui \
    dialog.ui \
    audioplayer.ui

RESOURCES += \
    resources.qrc
