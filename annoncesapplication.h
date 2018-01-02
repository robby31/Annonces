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
    void parserUpdatedSignal();
    void annoncesUpdatedSignal();

public slots:
    void initializeDatabase();
    void InterfaceLoaded(QObject *obj);

private:
    QSettings m_settings;
    Homepagecontroller controller;
    ApplicationWorker *worker = Q_NULLPTR;
};

#endif // ANNONCESAPPLICATION_H
