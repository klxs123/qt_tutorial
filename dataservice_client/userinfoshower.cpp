#include "userinfoshower.h"
#include "ui_userinfoshower.h"

#include "userinfomanager.h"
#include <QFileDialog>
#include <QImageReader>

UserInfoShower::UserInfoShower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoShower)
{
    ui->setupUi(this);
    ui->le_file->setReadOnly(true);
    ui->bt_upload->setDisabled(true);

    connect(ui->bt_browse, SIGNAL(clicked(bool)), this, SLOT(onBrowseClick()));
    connect(ui->bt_upload, SIGNAL(clicked(bool)), this, SLOT(onUploadClick()));
}

UserInfoShower::~UserInfoShower()
{
    delete ui;
}

void UserInfoShower::setUserInfo(UserInfo *userInfo)
{
    this->userInfo = userInfo;
    if(!this->userInfo->pic.empty())
    {
        QPixmap pix;
        pix.loadFromData((const uchar*)userInfo->pic.c_str(),userInfo->pic.length());
        ui->lb_img->setPixmap(pix);
    }
}

void UserInfoShower::onBrowseClick()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
            tr("image files (*.png)"));

    if(fileName.isEmpty())
    {
        return;
    }

    ui->le_file->setText(fileName);

    QImageReader ir(fileName);
    ui->lb_img->setPixmap(QPixmap::fromImageReader(&ir));
    ui->bt_upload->setDisabled(false);

}

void UserInfoShower::onUploadClick()
{
    QString file = ui->le_file->text();
    if(file.isEmpty())
        return;
    //以二进制格式打开图像文件
    FILE *fp = fopen(file.toStdString().c_str(), "rb");

    //获取文件大小
    fseek(fp, 0, SEEK_END);
    int flen = ftell(fp);

    //读取数据
    fseek(fp, 0, SEEK_SET);
    char *data = (char*)malloc(flen + 1);
    int size = fread(data, 1, flen, fp);

    UserInfoManager um;

    this->userInfo->pic.clear();
    userInfo->pic.append(data, size);

    um.updateUserInfo(this->userInfo);

    free(data);
    ui->bt_upload->setDisabled(true);
}
