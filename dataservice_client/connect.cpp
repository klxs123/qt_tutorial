#include "connect.h"
#include "ui_connect.h"

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

const DatabaseInfo &Connect::getDatabaseInfo() const
{
    return m_databaseInfo;
}
