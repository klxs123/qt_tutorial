#include "datamanagermodel.h"
#include <QIcon>

#include <fileservice_def.h>

using namespace std;

DataManagerModel::~DataManagerModel()
{
    for(map<string, list<FileInfo*> >::iterator it = m_infos.begin(); it!= m_infos.end(); it++)
    {
        list<FileInfo*>& infos = it->second;
        for(list<FileInfo*>::iterator fit = infos.begin(); fit!= infos.end(); fit++)
        {
            delete *fit;
        }
    }
}

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
    else if(role == Qt::DisplayRole)
    {
        return "name.....";
    }
    return QVariant();
}

QVariant DataManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant variant;


    return variant;

}

