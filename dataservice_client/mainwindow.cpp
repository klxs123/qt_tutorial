#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connect.h"
#include "userinfoshower.h"

#include <QSplitter>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QTabWidget>
#include <list>
using namespace std;

#include "userinfomanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_connect, SIGNAL(triggered(bool)), this, SLOT(onConnectClicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

static void fillUsersListView(QListView* view)
{
    UserInfoManager fetch;
    list<UserInfo*> users;
    fetch.fetchAll(users);
    QStringList userNames;
    for(list<UserInfo*>::iterator it = users.begin(); it != users.end(); it++)
    {
        userNames << QString::fromStdString((*it)->name);
    }

    QAbstractItemModel *model = new QStringListModel(userNames);
    view->setModel(model);

}

void MainWindow::createUsersView()
{
    QSplitter* qs = new QSplitter(this);
    this->setCentralWidget(qs);

    usersList = new QListView(qs);
    usersList->setMaximumWidth(150);

    connect(usersList, SIGNAL(clicked(QModelIndex)), this, SLOT(onUserClicked(QModelIndex)));

    usersView = new QTabWidget(qs);
}

void MainWindow::onConnectClicked()
{
    Connect con;
    con.exec();

    if(con.result() != QDialog::Accepted)
    {
        return;
    }
    createUsersView();
    fillUsersListView(usersList);
    ui->action_connect->setDisabled(true);
}

void MainWindow::onUserClicked(const QModelIndex &index)
{
    QVariant variant =  usersList->model()->data(index, Qt::DisplayRole);

    string name = variant.toString().toStdString();
    if(name.empty())
    {
        return;
    }
    //检查是否已有同一用户的卡片
    int tabs = usersView->count();
    for(int i=0; i< tabs; i++)
    {
        if(usersView->tabText(i).toStdString() == name)
        {
            return;
        }
    }

    UserInfoManager fetch;
    UserInfo* puser = fetch.fullFetchUserInfo(name);
    if(!puser)
    {
        return;
    }
    UserInfoShower* pshower = new UserInfoShower(usersView);
    pshower->setUserInfo(puser);
    usersView->addTab(pshower, QString::fromStdString(name));
    usersView->setCurrentWidget(pshower);

}
