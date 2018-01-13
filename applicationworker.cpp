#include "applicationworker.h"

ApplicationWorker::ApplicationWorker(QObject *parent) :
    Worker(parent),
    nam(this)
{
}

void ApplicationWorker::importAllResults(const int &parserId)
{
    emit processStarted();
    emit progress(-1);

    QSqlDatabase db = GET_DATABASE("Annonces");
    if (!db.isOpen())
    {
        emit errorDuringProcess("Unable to open Database.");
    }
    else
    {
        QUrl url;

        QSqlQuery query(db);

        // get url
        query.prepare("SELECT url FROM parser WHERE id=:id");
        query.bindValue(":id", parserId);
        if (query.exec() && query.next())
        {
            url = query.value("url").toUrl();

            db.transaction();

            // set all annonces inactive
            query.prepare("UPDATE annonces SET is_active=0 WHERE parserId=:id");
            query.bindValue(":id", parserId);
            if (!query.exec())
                qCritical() << "ERROR, unable to set annonces inactive";

            QNetworkReply *reply = nam.get(QNetworkRequest(url));
            LeBonCoinList *list = new LeBonCoinList(parserId, reply);
            connect(list, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
            connect(list, SIGNAL(error(QString)), this, SLOT(errorRaised(QString)));
            connect(list, SIGNAL(finished()), this, SLOT(allResultsRead()));
            connect(this, SIGNAL(processAborted()), list, SLOT(abort()));
        }
        else
        {
            qCritical() << query.lastError().text();
            emit errorDuringProcess("Unable to define Url.");
            db.rollback();
        }
    }
}

void ApplicationWorker::errorRaised(const QString &message)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    db.rollback();

    emit errorDuringProcess(message);
}

void ApplicationWorker::allResultsRead()
{
    LeBonCoinList *list = qobject_cast<LeBonCoinList*>(sender());
    QString msg = QString("%1 annonces loaded.").arg(list->href().size());
    emit processOver(msg);
    list->deleteLater();

    QSqlDatabase db = GET_DATABASE("Annonces");
    db.commit();

    emit annoncesUpdated();
}

void ApplicationWorker::saveLink(const QUrl &url, const QString &parserType, const QString &title)
{
    emit processStarted();
    emit progress(-1);

    QSqlDatabase db = GET_DATABASE("Annonces");
    if (!db.isOpen())
    {
        emit errorDuringProcess("Unable to open Database.");
    }
    else
    {
        QSqlQuery query(db);
        query.prepare("SELECT id from parser WHERE url=:url");
        query.bindValue(":url", url);
        if (query.exec())
        {
            query.last();
            if (query.at() >= 0)
            {
                emit errorDuringProcess("link already saved.");
            }
            else
            {
                // save the link
                query.prepare("INSERT INTO parser(parser_type, title, url) VALUES (:type, :title, :url)");
                query.bindValue(":type", parserType);
                query.bindValue(":title", title);
                query.bindValue(":url", url);
                if (query.exec())
                {
                    emit parserUpdated();
                    emit processOver();
                }
                else
                {
                    emit errorDuringProcess(QString("unable to save link : %1").arg(query.lastError().text()));
                }
            }
        }
        else
        {
            emit errorDuringProcess(QString("unable to find url: %1").arg(query.lastError().text()));
        }
    }
}

void ApplicationWorker::removeParser(const int &parserId)
{
    emit processStarted();
    emit progress(-1);

    QSqlDatabase db = GET_DATABASE("Annonces");
    if (!db.isOpen())
    {
        emit errorDuringProcess("Unable to open Database.");
    }
    else
    {
        db.transaction();

        QSqlQuery query(db);
        if (removePrix(db, parserId))
        {
            query.prepare("DELETE from annonces WHERE parserId=:id");
            query.bindValue(":id", parserId);
            if (query.exec())
            {
                query.prepare("DELETE from parser WHERE id=:id");
                query.bindValue(":id", parserId);
                if (query.exec())
                {
                    db.commit();
                    emit processOver();
                }
                else
                {
                    db.rollback();
                    qDebug() << "ERROR, unable to remove parser" << query.lastError().text();
                    emit errorDuringProcess("Unable to remove parser.");
                }
            }
            else
            {
                db.rollback();
                qDebug() << "ERROR, unable to remove annonces" << query.lastError().text();
                emit errorDuringProcess("Unable to remove annonces.");
            }
        }
        else
        {
            db.rollback();
            qDebug() << "ERROR, unable to remove prix";
            emit errorDuringProcess("Unable to remove prix.");
        }
    }

    emit parserUpdated();
}

bool ApplicationWorker::removePrix(QSqlDatabase db, const int &parserId)
{
    QSqlQuery select(db);
    select.prepare("SELECT id from annonces WHERE parserId=:id");
    select.bindValue(":id", parserId);
    if (select.exec())
    {
        QSqlQuery query(db);
        while (select.next())
        {
            // remove prix
            query.prepare("DELETE FROM prix WHERE annonceId=:id");
            query.bindValue(":id", select.value("id"));
            if (!query.exec())
            {
                qDebug() << "unable to remove prix" << query.lastError().text();
                return false;
            }

            if (processWasAborted())
                return false;
        }
    }

    return true;
}
