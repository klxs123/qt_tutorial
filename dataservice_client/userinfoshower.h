#ifndef USERINFOSHOWER_H
#define USERINFOSHOWER_H

#include <QWidget>

namespace Ui {
class UserInfoShower;
}
class UserInfo;
class UserInfoShower : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoShower(QWidget *parent = 0);
    ~UserInfoShower();

    void setUserInfo(UserInfo* userInfo);

private:
    Ui::UserInfoShower *ui;
    UserInfo* userInfo;
private   slots:
    void onBrowseClick();
    void onUploadClick();
};

#endif // USERINFOSHOWER_H
