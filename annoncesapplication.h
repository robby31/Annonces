#ifndef ANNONCESAPPLICATION_H
#define ANNONCESAPPLICATION_H

#include "application.h"
#include "homepagecontroller.h"
#include "applicationworker.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "pricemodel.h"
#include <QDir>

class AnnoncesApplication : public Application
{
    Q_OBJECT

public:
    explicit AnnoncesApplication(int &argc, char **argv);
    virtual ~AnnoncesApplication();

signals:

public slots:
    void initializeDatabase();
    void InterfaceLoaded(QObject *obj);
    void removeParser(const int &parserId);

private:
    QSettings m_settings;
    Homepagecontroller controller;
    ApplicationWorker *worker;
};

#endif // ANNONCESAPPLICATION_H
