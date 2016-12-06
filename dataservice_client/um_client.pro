#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T09:14:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32 {
    INCLUDEPATH += "C:\Program Files\MySQL\MySQL Connector C 6.1\include"
    LIBS+= -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib\vs12"   -llibmysql
}

unix {
    INCLUDEPATH += /usr/local/mysql/include/
    LIBS+= -L"/usr/local/mysql/lib/"   -lmysqlclient
}

TARGET = um_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connect.cpp \
    userinfoshower.cpp \
    userinfomanager.cpp \
    appconfig.cpp \
    databaseconnectionpool.cpp \
    userinfotabwidget.cpp

HEADERS  += mainwindow.h \
    connect.h \
    userinfoshower.h \
    userinfomanager.h \
    appconfig.h \
    databaseconnectionpool.h \
    singleton.hpp \
    databaseinfo.hpp \
    userinfotabwidget.h

FORMS    += mainwindow.ui \
    connect.ui \
    userinfoshower.ui
