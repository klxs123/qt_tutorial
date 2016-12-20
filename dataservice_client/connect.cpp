#include "connect.h"
#include "ui_connect.h"
#include "appconfig.h"
#include "databaseconnectionpool.h"

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

void Connect::setDatabaseInfo(const DatabaseInfo &info)
{
    m_databaseInfo = info;
    ui->le_ip->setText(QString::fromStdString(info.server));
    ui->le_port->setText(QString::number(info.port));
    ui->le_user->setText(QString::fromStdString(info.user));
    ui->le_pwd->setText(QString::fromStdString(info.pwd));
}

const DatabaseInfo &Connect::getDatabaseInfo()
{
    m_databaseInfo.server = ui->le_ip->text().toStdString();
    m_databaseInfo.port = ui->le_port->text().toInt();
    m_databaseInfo.user = ui->le_user->text().toStdString();
    m_databaseInfo.pwd = ui->le_pwd->text().toStdString();

    return m_databaseInfo;
}

int Connect::exec()
{
    if(QDialog::exec() == QDialog::Rejected)
    {
        return QDialog::Rejected;
    }

    if(DatabaseConnectionPool::Instance().init(getDatabaseInfo()) !=0)
    {
        return QDialog::Rejected;//数据库连接初始化失败
    }
    return QDialog::Accepted;
}

void Connect::onTest()
{
//todo...
}
