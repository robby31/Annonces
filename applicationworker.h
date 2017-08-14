#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

#include "mysqldatabase.h"
#include "Worker/worker.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "leboncoinlist.h"
#include <QUrl>

class ApplicationWorker : public Worker
{
    Q_OBJECT

public:
    explicit ApplicationWorker(QObject *parent = 0);

private:
    bool removePrix(QSqlDatabase db, const int &parserId);

signals:
    void parserUpdated();
    void annoncesUpdated();

public slots:
    void importAllResults(const int &parserId);
    void allResultsRead();
    void errorRaised(const QString &message);
    void saveLink(const QUrl &url, const QString &parserType, const QString &title);
    void removeParser(const int &parserId);

private:
    QNetworkAccessManager nam;
};

#endif // APPLICATIONWORKER_H
