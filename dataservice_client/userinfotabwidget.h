#ifndef USERINFOTABWIDGET_H
#define USERINFOTABWIDGET_H

#include <QTabWidget>

class UserInfoTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit  UserInfoTabWidget(QWidget* parent = 0);

public slots:
    void onUserNameChanged(const QString& name);
};

#endif // USERINFOTABWIDGET_H
