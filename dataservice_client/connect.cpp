#include "connect.h"
#include "ui_connect.h"
#include "appconfig.h"

#include <dataservice_pub.h>


Connect::Connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    ui->lb_ip->setAlignment(Qt::AlignRight);

    ui->lb_port->setAlignment(Qt::AlignRight);
    ui->lb_pwd->setAlignment(Qt::AlignRight);
    ui->lb_user->setAlignment(Qt::AlignRight);
    ui->le_pwd->setEchoMode(QLineEdit::Password);

    connect(ui->bt_test, SIGNAL(clicked(bool)), this, SLOT(onTest()));
}

Connect::~Connect()
{
    delete ui;
}

void Connect::setConnectInfo(const ConnectInfo &info)
{
    m_serverInfo = info;
    ui->le_ip->setText(QString::fromStdString(info.ip));
    ui->le_port->setText(QString::number(info.port));
    ui->le_user->setText(QString::fromStdString(info.user));
    ui->le_pwd->setText(QString::fromStdString(info.pwd));
}

const ConnectInfo &Connect::getConnectInfo()
{
    m_serverInfo.ip = ui->le_ip->text().toStdString();
    m_serverInfo.port = ui->le_port->text().toInt();
    m_serverInfo.user = ui->le_user->text().toStdString();
    m_serverInfo.pwd = ui->le_pwd->text().toStdString();

    return m_serverInfo;
}

int Connect::exec()
{
    if(QDialog::exec() == QDialog::Rejected)
    {
        return QDialog::Rejected;
    }

    tcp_client tc;
    if( tc.connect("localhost", 3333) != 0)
    {
        return QDialog::Rejected;
    }
    //login

    Message msg;
    msg.command = CT_LoginRequest;
    LoginRequest request;
    request.user.first = "zf";
    request.user.second = "123456";
    make_request(request, msg.data);
    string data;
    msg_to_package(msg, data);

    tc.write(data);
    data.clear();

    tc.read(data, 1024);


    msg.data.clear();
    package_to_msg(msg, data);

    LoginResponse response;

    get_response(response, msg.data);

    if(!response.success)
    {
        printf("recv login response:failed\n");
        return QDialog::Rejected;
    }

    printf("recv login response:success\n");


    return QDialog::Accepted;
}

void Connect::onTest()
{
//todo...
}
