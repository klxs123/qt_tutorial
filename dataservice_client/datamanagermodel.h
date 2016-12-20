#ifndef DATAMANAGERMODEL_H
#define DATAMANAGERMODEL_H

#include <QAbstractItemModel>


class DataManagerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DataManagerModel(QObject *parent = 0)
        : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;


};

#endif // DATAMANAGERMODEL_H
