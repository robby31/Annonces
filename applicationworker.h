#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include "mysqldatabase.h"
#include "Worker/worker.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "leboncoinlist.h"

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QObject *parent = 0);

    void initialize() { emit initializeSignal(); }

private:
    bool removePrix(QSqlDatabase db, const int &parserId);

signals:
    void initializeSignal();
    void parserUpdated();
    void annoncesUpdated();

public slots:
    void initializeDatabase();
    void importAllResults(const int &parserId);
    void allResultsRead();
    void saveLink(const QUrl &url, const QString &parserType, const QString &title);
    void removeParser(const int &parserId);

private:
    QNetworkAccessManager nam;
};

#endif // APPLICATIONWORKER_H
