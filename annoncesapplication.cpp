#include "annoncesapplication.h"

AnnoncesApplication::AnnoncesApplication(int argc, char **argv) :
    Application(argc, argv),
    controller(this),
    worker(0)
{
    qmlRegisterType<SqlListModel>("Models", 1, 0, "SqlListModel");
    qmlRegisterType<ListModel>("Models", 1, 0, "CListModel");
    qmlRegisterType<CheckedSqlListModel>("Models", 1, 0, "CheckedSqlListModel");
    qmlRegisterType<PriceModel>("Models", 1, 0, "PriceModel");

    connect(this, SIGNAL(mainQmlLoaded(QObject*)), this, SLOT(InterfaceLoaded(QObject*)));
    addController("homepagecontroller", &controller);

    QSqlDatabase db = CREATE_DATABASE("QSQLITE", "Annonces");
    db.setDatabaseName("C:/Users/NJUT/Documents/workspaceQT/Annonces/data.sql");

    if (!db.open())
    {
        qCritical() << "unable to open database";
    }
    else
    {
        QSqlQuery query(db);
        if (!query.exec("PRAGMA foreign_keys = ON;"))
            qCritical() << "unable to active foreign key";

        if (!query.exec("create table IF NOT EXISTS prix "
                        "(annonceid INTEGER NOT NULL REFERENCES annonces, "
                        " date TIMESTAMP NOT NULL, "
                        " prix REAL NOT NULL)"))
            qCritical() << query.lastError().text();

        if (!query.exec("create table IF NOT EXISTS annonces "
                        "(id INTEGER PRIMARY KEY, "
                        "parserId INTEGER REFERENCES parser, "
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
                        "km INTEGER )"))
            qCritical() << query.lastError().text();

        // table to store definition of parser used in database
        if (!query.exec("create table IF NOT EXISTS parser "
                        "(id INTEGER PRIMARY KEY, "
                        " parser_type TEXT, "
                        " title TEXT, "
                        " url TEXT UNIQUE)"))
            qCritical() << query.lastError().text();

         // table to store properties
        if (!query.exec("create table IF NOT EXISTS properties "
                        "(id INTEGER PRIMARY KEY, "
                        "annonceId NOT NULL REFERENCES annonces, "
                        "param TEXT UNIQUE, "
                        "value TEXT)"))
            qCritical() << query.lastError().text();
    }
}

AnnoncesApplication::~AnnoncesApplication()
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    QSqlQuery query(db);
    if (!query.exec("VACUUM"))
        qDebug() << "unable to clean database" << query.lastError().text();
}

void AnnoncesApplication::InterfaceLoaded(QObject *obj)
{
    // start worker
    worker = new ApplicationWorker();
    addWorker(&controller, worker);

    connect(obj, SIGNAL(importResults(int)), &controller, SLOT(importAllResults(int)));
    connect(&controller, SIGNAL(importAllResultsSignal(int)), worker, SLOT(importAllResults(int)));

    connect(obj, SIGNAL(saveLink(QUrl,QString,QString)), &controller, SLOT(saveLink(QUrl,QString,QString)));
    connect(&controller, SIGNAL(saveLinkSignal(QUrl,QString,QString)), worker, SLOT(saveLink(QUrl,QString,QString)));

    connect(worker, SIGNAL(parserUpdated()), obj, SLOT(parserUpdated()));
    connect(worker, SIGNAL(annoncesUpdated()), obj, SLOT(annoncesUpdated()));

    connect(&controller, SIGNAL(removeParserSignal(int)), worker, SLOT(removeParser(int)));

    worker->initialize();
}

void AnnoncesApplication::removeParser(const int &parserId)
{
    controller.removeParser(parserId);
}
