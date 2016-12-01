#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T09:14:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += "C:\Program Files\MySQL\MySQL Connector C 6.1\include"
LIBS+= -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib\vs12"   -llibmysql

TARGET = um_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connect.cpp \
    userinfoshower.cpp \
    userinfomanager.cpp

HEADERS  += mainwindow.h \
    connect.h \
    userinfoshower.h \
    userinfomanager.h

FORMS    += mainwindow.ui \
    connect.ui \
    userinfoshower.ui
