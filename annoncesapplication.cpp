#include "annoncesapplication.h"

AnnoncesApplication::AnnoncesApplication(int &argc, char **argv) :
    Application(argc, argv),
    m_settings("G. HIMBERT", "Annonces"),
    controller(this)
{    
    qmlRegisterType<PriceModel>("Models", 1, 0, "PriceModel");

    connect(this, SIGNAL(databaseOpened(QString)), this, SLOT(initializeDatabase()));

    connect(this, SIGNAL(mainQmlLoaded(QObject*)), this, SLOT(InterfaceLoaded(QObject*)));

    addController("homepagecontroller", &controller);

    setdatabaseDiverName("QSQLITE");
    setdatabaseConnectionName("Annonces");

    QString path = m_settings.value("databasePathName").toString();
    if (!path.isEmpty())
        setdatabaseName(QUrl::fromLocalFile(path).toLocalFile());
}

AnnoncesApplication::~AnnoncesApplication()
{
    QSqlDatabase db = database();
    if (db.isOpen())
    {
        QSqlQuery query(db);
        if (!query.exec("VACUUM"))
            qDebug() << "unable to clean database" << query.lastError().text();
    }
}

void AnnoncesApplication::InterfaceLoaded(QObject *obj)
{
    Q_UNUSED(obj)

    // start worker
    worker = new ApplicationWorker();
    addWorker(&controller, worker);

    connect(&controller, SIGNAL(importAllResultsSignal(int)), worker, SLOT(importAllResults(int)));
    connect(&controller, SIGNAL(saveLinkSignal(QUrl,QString,QString)), worker, SLOT(saveLink(QUrl,QString,QString)));
    connect(&controller, SIGNAL(removeParserSignal(int)), worker, SLOT(removeParser(int)));

    connect(worker, SIGNAL(parserUpdated()), &controller, SIGNAL(parserUpdated()));
    connect(worker, SIGNAL(annoncesUpdated()), &controller, SIGNAL(annoncesUpdated()));
}

void AnnoncesApplication::initializeDatabase()
{
    QSqlDatabase db = database();

    if (db.isOpen())
    {
        m_settings.setValue("databasePathName", db.databaseName());

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

        // table to store optional parameters
        if (!query.exec("create table IF NOT EXISTS param_name "
                        "(id INTEGER PRIMARY KEY, "
                        "name TEXT UNIQUE NOT NULL)"))
            qCritical() << query.lastError().text();

        if (!query.exec("create table IF NOT EXISTS param_value "
                        "(id INTEGER PRIMARY KEY, "
                        "value TEXT UNIQUE NOT NULL)"))
            qCritical() << query.lastError().text();

        if (!query.exec("create table IF NOT EXISTS optional_param "
                        "(id INTEGER PRIMARY KEY, "
                        "annonceId NOT NULL REFERENCES annonces, "
                        "param NOT NULL REFERENCES param_name, "
                        "value NOT NULL REFERENCES param_value,"
                        "UNIQUE(annonceId, param))"))
            qCritical() << query.lastError().text();
    }
    else
    {
        qCritical() << "database not open, cannot initialize database.";
    }
}
