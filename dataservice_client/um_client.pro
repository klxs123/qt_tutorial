#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T09:14:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#注意!!路径修改为本机环境下的路径
win32 {
    INCLUDEPATH += "C:\Program Files\MySQL\MySQL Connector C 6.1\include"
    LIBS+= -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib\vs12"   -llibmysql
}

unix {
    INCLUDEPATH += /usr/local/mysql/include/ ../dataservice_pub
    LIBS+= -L"/usr/local/mysql/lib/"  -L"/mnt/linux_lab/examples/qt_tutorial/bin"  -lmysqlclient -ldataservice_pub
    DEFINES+=unix
}

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connect.cpp \
    userinfoshower.cpp \
    appconfig.cpp \
    userinfotabwidget.cpp \
    FtpClient/BlockingSocket.cpp \
    FtpClient/FTPClient.cpp \
    FtpClient/FTPDataTypes.cpp \
    FtpClient/FTPFileStatus.cpp \
    FtpClient/FTPListParse.cpp \
    datamanager.cpp \
    datamanagermodel.cpp \
    userinfomanager.cpp

HEADERS  += mainwindow.h \
    connect.h \
    userinfoshower.h \
    userinfomanager.h \
    appconfig.h \
    databaseconnectionpool.h \
    singleton.hpp \
    databaseinfo.hpp \
    userinfotabwidget.h \
    FtpClient/BlockingSocket.h \
    FtpClient/Definements.h \
    FtpClient/FTPClient.h \
    FtpClient/FTPDataTypes.h \
    FtpClient/FTPFileStatus.h \
    FtpClient/FTPListParse.h \
    FtpClient/smart_ptr.h \
    datamanager.h \
    datamanagermodel.h \
    userinfomanager.h

FORMS    += mainwindow.ui \
    connect.ui \
    userinfoshower.ui

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS += -Wno-deprecated
