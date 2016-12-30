#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>

namespace Ui {
class Connect;
}

#include "appconfig.h"

class Connect : public QDialog
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = 0);
    ~Connect();
    void setConnectInfo(const ConnectInfo&);
    const ConnectInfo& getConnectInfo() ;
    int exec();
private slots:
    void onTest();
private:
    Ui::Connect *ui;
    ConnectInfo m_serverInfo;

};

#endif // CONNECT_H
