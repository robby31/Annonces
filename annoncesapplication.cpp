#include "annoncesapplication.h"

AnnoncesApplication::AnnoncesApplication(int argc, char **argv) :
    Application(argc, argv),
    controller(this),
    worker(0),
    m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    qmlRegisterType<SqlListModel>("Models", 1, 0, "SqlListModel");
    qmlRegisterType<ListModel>("Models", 1, 0, "CListModel");
    qmlRegisterType<CheckedSqlListModel>("Models", 1, 0, "CheckedSqlListModel");

    connect(this, SIGNAL(mainQmlLoaded(QObject*)), this, SLOT(InterfaceLoaded(QObject*)));
    addController("homepagecontroller", &controller);

    m_db.setDatabaseName("/Users/doudou/workspaceQT/Annonces/data.sql");
    if (!m_db.open())
    {
        qWarning() << "unable to open database";
    }
    else
    {
        QSqlQuery query;
        if (!query.exec("create table IF NOT EXISTS prix "
                        "(annonceid INTEGER NOT NULL, "
                        " date TIMESTAMP NOT NULL, "
                        " prix INTEGER NOT NULL)"))
            qWarning() << query.lastError().text();

        if (!query.exec("create table IF NOT EXISTS annonces "
                        "(id INTEGER primary key, "
                        "is_active INTEGER DEFAULT 1, "
                        "titre TEXT NOT NULL, "
                        "rating INTEGER DEFAULT 0, "
                        "description TEXT UNIQUE, "
                        "commentaire TEXT, "
                        "img TEXT, "
                        "ref TEXT UNIQUE NOT NULL, "
                        "urgent INTEGER, "
                        "pro INTEGER,"
                        "person TEXT,"
                        "date DATETIME NOT NULL, "
                        "created_date DATETIME DEFAULT CURRENT_DATETIME, "
                        "lieu TEXT, "
                        "ville TEXT, "
                        "code_postal TEXT, "
                        "type TEXT, "
                        "surface INTEGER, "
                        "pieces INTEGER, "
                        "GES TEXT, "
                        "energie TEXT, "
                        "prix_m2 REAL, "
                        "prix INTEGER, "
                        "marque TEXT, "
                        "modele TEXT, "
                        "annee_modele INTEGER, "
                        "carburant TEXT, "
                        "bv TEXT, "
                        "km INTEGER)"))
            qWarning() << query.lastError().text();

        // table to store definition of parser used in database
        if (!query.exec("create table IF NOT EXISTS parser "
                        "(id INTEGER primary key, "
                        " parser_type TEXT, "
                        " url TEXT UNIQUE)"))
            qWarning() << query.lastError().text();

         // table to store properties
        if (!query.exec("create table IF NOT EXISTS properties "
                        "(id INTEGER primary key, "
                        "param TEXT UNIQUE, "
                        "value TEXT)"))
            qWarning() << query.lastError().text();
    }
}

void AnnoncesApplication::InterfaceLoaded(QObject *obj)
{
    Q_UNUSED(obj)

    // start worker
    worker = new ApplicationWorker();
    addWorker(&controller, worker);

    connect(obj, SIGNAL(importResults(QUrl)), &controller, SLOT(importAllResults(QUrl)));
    connect(&controller, SIGNAL(importAllResultsSignal(QUrl)), worker, SLOT(importAllResults(QUrl)));
}
