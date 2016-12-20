#include <QListView>
#include <QHBoxLayout>
#include <QStandardItem>

#include "datamanager.h"
#include "datamanagermodel.h"

DataManager::DataManager(QWidget *parent) : QWidget(parent), m_model(0)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    QListView * view = new QListView(this);
    view->setViewMode(QListView::IconMode);
    view->setResizeMode(QListView::Adjust);

    //view->setModelColumn(0);
    view->setIconSize(QSize(48,48));


    m_model = new  DataManagerModel(this);

    view->setModel(m_model);
    view->setModelColumn(5);

    layout->addWidget(view);
}
