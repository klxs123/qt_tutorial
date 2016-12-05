#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>

namespace Ui {
class Connect;
}


#include "databaseinfo.hpp"

class Connect : public QDialog
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = 0);
    ~Connect();
    void setDatabaseInfo(const DatabaseInfo&);
    const DatabaseInfo& getDatabaseInfo() const;

private:
    Ui::Connect *ui;
    DatabaseInfo m_databaseInfo;
};

#endif // CONNECT_H
