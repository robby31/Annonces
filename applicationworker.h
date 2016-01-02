#ifndef APPLICATIONWORKER_H
#define APPLICATIONWORKER_H

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

signals:

public slots:
    void importAllResults(const QUrl &url);
    void allResultsRead();

private:
    QNetworkAccessManager nam;
};

#endif // APPLICATIONWORKER_H
