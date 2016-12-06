#ifndef USERINFOSHOWER_H
#define USERINFOSHOWER_H

#include <QWidget>

namespace Ui {
class UserInfoShower;
}



struct UserInfo;
struct UserInfoEx;
class UserInfoShower : public QWidget
{
    Q_OBJECT

public:
    enum UserInfoShowerMode
    {
        Showing,
        Adding,
        Editing,
    };
    explicit UserInfoShower(QWidget *parent = 0, UserInfoShowerMode mode = UserInfoShower::Showing );
    ~UserInfoShower();

    void setUserInfo(UserInfo* m_userInfo);
    const UserInfoEx* getUserInfoEx() const;
    const UserInfo& getUserInfo() const;
signals:
    void nameChanged(const QString& name);
    void userAdded(const QString& name);
private:
    Ui::UserInfoShower *ui;
    UserInfoEx* m_userInfo;
    UserInfoShowerMode m_mode;
private   slots:
    void onBrowseClick();
    void onUploadClick();
    void onUserNameChanged(const QString&);
};

#endif // USERINFOSHOWER_H
