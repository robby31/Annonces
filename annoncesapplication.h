#ifndef ANNONCESAPPLICATION_H
#define ANNONCESAPPLICATION_H

#include "application.h"
#include "homepagecontroller.h"
#include "applicationworker.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class AnnoncesApplication : public Application
{
    Q_OBJECT

public:
    explicit AnnoncesApplication(int argc, char **argv);

signals:

public slots:
    void InterfaceLoaded(QObject *obj);

private:
    Homepagecontroller controller;
    ApplicationWorker *worker;
    QSqlDatabase m_db;
};

#endif // ANNONCESAPPLICATION_H
