#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QWidget>

class DataManagerModel;

class DataManager : public QWidget
{
    Q_OBJECT
public:
    explicit DataManager(QWidget *parent = 0);    

signals:

public slots:
private:
    DataManagerModel* m_model;
};

#endif // DATAMANAGER_H
