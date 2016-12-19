#include "userinfotabwidget.h"



UserInfoTabWidget::UserInfoTabWidget(QWidget *parent):QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabRequest(int)));

}

void UserInfoTabWidget::onUserNameChanged(const QString &name)
{
    QWidget *tab = dynamic_cast<QWidget*>(QObject::sender());
    if(tab == 0)
    {
        return;
    }

    int index = this->indexOf(tab);

    this->setTabText(index, name);
}

void UserInfoTabWidget::onCloseTabRequest(int index)
{
    this->removeTab(index);
}
