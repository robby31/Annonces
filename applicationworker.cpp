#include "applicationworker.h"

ApplicationWorker::ApplicationWorker(QObject *parent) :
    Worker(parent),
    nam(this)
{

}

void ApplicationWorker::importAllResults(const QUrl &url)
{
    emit processStarted();
    emit progress(-1);

    // set all annonces inactive
    QSqlQuery query;
    if (!query.exec("UPDATE annonces SET is_active=0"))
        qWarning() << "ERROR, unable to set annonces inactive";

    QNetworkReply *reply = nam.get(QNetworkRequest(url));
    LeBonCoinList *list = new LeBonCoinList(reply);
    connect(list, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
    connect(list, SIGNAL(finished()), this, SLOT(allResultsRead()));
    connect(this, SIGNAL(processAborted()), list, SLOT(abort()));
}

void ApplicationWorker::allResultsRead()
{
    LeBonCoinList *list = qobject_cast<LeBonCoinList*>(sender());
    QString msg = QString("%1 annonces loaded.").arg(list->href().size());
    emit processOver(msg);
    list->deleteLater();
}
