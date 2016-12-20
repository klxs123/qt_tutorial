#include "datamanagermodel.h"
#include <QIcon>


int DataManagerModel::rowCount(const QModelIndex &parent) const
{
    return 100;
}

QVariant DataManagerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DecorationRole)
    {
        QPixmap image;
        if(image.load(":imgs/res/imgs/folder/Folder-icon-48.png"))
        {
            return image;
        }

       return QIcon(":imgs/res/imgs/data_manager.png");
    }
    return QVariant();
}

QVariant DataManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant variant;


    return variant;

}

