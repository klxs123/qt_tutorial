#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include <QModelIndex>

class QTableView;
class QTabWidget;
class QListView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void onConnectClicked();
    void onUserClicked(const QModelIndex&);

private:
    Ui::MainWindow *ui;
    void createUsersView();
    QListView* usersList;
    QTabWidget* usersView;
};

#endif // MAINWINDOW_H
