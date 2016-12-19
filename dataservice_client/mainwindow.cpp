#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connect.h"
#include "userinfoshower.h"

#include <QSplitter>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include "userinfotabwidget.h"
#include <list>
using namespace std;

#include "userinfomanager.h"

#include "appconfig.h"

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
    usersList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //usersList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    usersList->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑


    connect(usersList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onUserClicked(QModelIndex)));
    //必须设置,否则无法发出上下文菜单信号
    usersList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(usersList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onUsersListContextMenu(QPoint)));

    usersView = new UserInfoTabWidget(qs);
}

void MainWindow::showUser(const QString &name, UserInfoShower::UserInfoShowerMode mode)
{
    if(!name.isEmpty() && mode !=UserInfoShower::Adding )
    {
        //检查是否已有同一用户的卡片
        int tabs = usersView->count();
        for(int i=0; i< tabs; i++)
        {
            if(usersView->tabText(i) == name)
            {
				//将已有卡片设置为当前页
                usersView->setCurrentIndex(i);
                return;
            }
        }
    }
    UserInfo* puser = 0;
    if(mode != UserInfoShower::Adding)
    {
        UserInfoManager fetch;
        puser = fetch.fullFetchUserInfo(name.toStdString());
        if(!puser)
        {
            return;
        }
    }
    else
    {
        puser = new UserInfo;
    }


    UserInfoShower* pshower = new UserInfoShower(usersView, mode);
    pshower->setUserInfo(puser);
    usersView->addTab(pshower, name.isEmpty()?"new user":name);

    usersView->setCurrentWidget(pshower);
    connect(pshower, SIGNAL(nameChanged(QString)), usersView, SLOT(onUserNameChanged(QString)));
    connect(pshower, SIGNAL(userAdded(QString)), this, SLOT(OnAddUser(QString)));

    delete puser;

}

void MainWindow::onConnectClicked()
{
    Connect con;
    con.setDatabaseInfo(AppConfig::Instance().getDatabaseInfo());

    if(con.exec() != QDialog::Accepted)
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

    showUser(variant.toString());
}

void MainWindow::onUsersListContextMenu(const QPoint &point)
{
    QModelIndex index = usersList->indexAt(point);
    QMenu myMenu;

    if(index.isValid())
    {
       QVariant variant =  usersList->model()->data(index, Qt::DisplayRole);

       string name = variant.toString().toStdString();

       myMenu.addAction("Open",  this, SLOT(onUserShow()))->setData(QString::fromStdString(name));
       myMenu.addAction("Erase",  this, SLOT(OnDelUser()))->setData(QString::fromStdString(name));
    }

    myMenu.addAction("Add",  this, SLOT(OnAddUser()));



    // Show context menu at handling position
    myMenu.exec(usersList->mapToGlobal(point));

}

void MainWindow::onUserShow()
{
    QVariant data = dynamic_cast<QAction*>(QObject::sender())->data();

    showUser(data.toString());
}

void MainWindow::OnAddUser()
{
    showUser("", UserInfoShower::Adding);
}

void MainWindow::OnAddUser(const QString &name)
{
    if(name.isNull() || name.isEmpty())
    {
        return;
    }
    QStringListModel* model =   dynamic_cast<QStringListModel*>(usersList->model());

    QStringList userNames = model->stringList();
    userNames << name;
    model->setStringList(userNames);
}

void MainWindow::OnDelUser()
{
    QVariant name = dynamic_cast<QAction*>(QObject::sender())->data();
    UserInfoManager::Instance().delUser(name.toString().toStdString());

    //检查是否已有用户的卡片
    int tabs = usersView->count();
    for(int i=0; i< tabs; i++)
    {
        if(usersView->tabText(i) == name)
        {
            usersView->removeTab(i);
            break;
        }
    }

   QStringListModel* model =   dynamic_cast<QStringListModel*>(usersList->model());

   for(int i=0; i< model->rowCount(); i++)
   {
       const QModelIndex& index = model->index(i);
       QVariant variant = model->data(index, Qt::DisplayRole);
       if(variant == name)
       {
           model->removeRow(i);
           break;
       }
   }

}
