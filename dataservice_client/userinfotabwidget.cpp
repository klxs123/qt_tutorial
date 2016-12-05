#include "userinfotabwidget.h"



UserInfoTabWidget::UserInfoTabWidget(QWidget *parent):QTabWidget(parent)
{

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
