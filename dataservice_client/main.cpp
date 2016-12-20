#include "mainwindow.h"
#include <QApplication>
#include <cstdlib>
#include "appconfig.h"
#include "databaseconnectionpool.h"

#include "FtpClient/FTPClient.h"


void cleanup()
{
    DatabaseConnectionPool::Instance().clear();
}

void TestFTP()
{
#define TRACE printf
   nsFTP::CFTPClient ftpClient;
   nsFTP::CLogonInfo logonInfo("localhost", 4131);

   // connect to server
   ftpClient.Login(logonInfo);

   // get directory listing
   nsFTP::TFTPFileStatusShPtrVec list;
   ftpClient.NameList(_T("/tmp"), list);


   // iterate listing
   for( nsFTP::TFTPFileStatusShPtrVec::iterator it=list.begin();
                                            it!=list.end(); ++it )
   {
       TRACE(_T("\n%s"), (*it)->Name().c_str());
   }

//   // do file operations
   ftpClient.DownloadFile(_T("/pub/test.txt"), _T("c:\\temp\\test.txt"));

//   ftpClient.UploadFile(_T("c:\\temp\\test.txt"), _T("/upload/test.txt"));

//   ftpClient.Rename(_T("/upload/test.txt"), _T("/upload/NewName.txt"));

//   ftpClient.Delete(_T("/upload/NewName.txt"));

   // disconnect
   ftpClient.Logout();
}

int main(int argc, char *argv[])
{
    //TestFTP();
    atexit(cleanup);
    QApplication a(argc, argv);

    if(AppConfig::Instance().init()!=0)
    {
        return -1;//程序初始化失败
    }

    a.setWindowIcon(QIcon(":imgs/res/imgs/app.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
