#include "mainwindow.h"
#include <QApplication>
#include <cstdlib>
#include "appconfig.h"
#include "databaseconnectionpool.h"


void cleanup()
{
    DatabaseConnectionPool::Instance().clear();
}

int main(int argc, char *argv[])
{
    atexit(cleanup);
    QApplication a(argc, argv);

    if(AppConfig::Instance().init()!=0)
    {
        return -1;//程序初始化失败
    }

    MainWindow w;
    w.show();

    return a.exec();
}
