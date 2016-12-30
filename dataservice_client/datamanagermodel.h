#ifndef DATAMANAGERMODEL_H
#define DATAMANAGERMODEL_H

#include <QAbstractItemModel>
#include <map>
#include <string>
#include <list>

struct FileInfo;

class DataManagerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DataManagerModel(QObject *parent = 0)
        : QAbstractListModel(parent) {}
    ~DataManagerModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

protected:

    std::map<std::string, std::list<FileInfo*> > m_infos;

};

#endif // DATAMANAGERMODEL_H
