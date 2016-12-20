#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "userinfoshower.h"

#include <QModelIndex>

#include <QPoint>

class UserInfoTabWidget;
class QTabWidget;
class QListView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onConnectClicked();
    void onUserClicked(const QModelIndex&);
    void onUsersListContextMenu(const QPoint&point);
    void onUserShow();
    void OnAddUser();
    void OnAddUser(const QString& name);
    void OnDelUser();
    void onOpenDataManager();

private:
    Ui::MainWindow *ui;
    void createUsersView();
    void showUser(const QString& name, UserInfoShower::UserInfoShowerMode= UserInfoShower::Showing);
    QListView* usersList;
    UserInfoTabWidget* usersView;
};

#endif // MAINWINDOW_H
