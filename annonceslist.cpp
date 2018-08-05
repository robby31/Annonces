#include "annonceslist.h"

AnnoncesList::AnnoncesList(int parserId, QNetworkReply *reply, QObject *parent) :
    QObject(parent),
    m_parserId(parserId),
    m_reply(reply),
    m_href(),
    m_nbpages(-1),
    allPagesLoaded(false),
    m_abort(false),
    m_linksToUpdate()
{
    if (reply)
    {
        connect(reply, SIGNAL(finished()), this, SLOT(pageLoaded()));
        connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
    }
    else
    {
        qCritical() << this << "invalid reply" << reply;
    }
}

QUrl AnnoncesList::url() const
{
    if (m_reply)
        return m_reply->request().url();
    else
        return QUrl();
}

bool AnnoncesList::save(const QByteArray &data, QString pathname)
{
    QFile file(pathname);
    if (file.open(QIODevice::ReadWrite))
    {
        file.write(data);
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void AnnoncesList::setPages(const int &nb)
{
    if (nb>0)
        m_nbpages = nb;
}

bool AnnoncesList::atEnd()
{
    if (currentPage()>0 && nbPages()>0)
        return currentPage() >= nbPages();
    else
        return true;
}

void AnnoncesList::pageLoaded()
{        
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    QByteArray data = reply->readAll();
    readAnnonces(data);
    qDebug() << "page loaded" << url() << data.size() << "bytes," << nbPages()-currentPage() << "pages to read" << href().size() << "annonces to read.";

//    save(data, "results.txt");

    if (nbPages() > 0)
    {
        if (m_reply != reply)
        {
            m_reply->deleteLater();
            m_reply = reply;
        }
        else
        {
            // start to read annonces
            if (href().size() >= 1)
            {
                // first page loaded, start loading of first annonce
                requestAnnonce(m_href.at(0));
            }
            else
            {
                emit error("no annonce to read.");
            }
        }

        if  (!atEnd())
        {
            // read next page
            QNetworkReply *reply = m_reply->manager()->get(QNetworkRequest(nextPageUrl()));
            connect(reply, SIGNAL(finished()), this, SLOT(pageLoaded()));
            connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
        }
        else
        {
            allPagesLoaded = true;
        }
    }
    else
    {
        emit error("invalid number of pages.");
    }
}

void AnnoncesList::requestAnnonce(const QUrl &url)
{
    qDebug() << "request Annonce" << url;
    QNetworkReply *reply = m_reply->manager()->get(QNetworkRequest(url));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(annonceLoaded()));
    connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
}

void AnnoncesList::replyError(const QNetworkReply::NetworkError &error)
{
    qWarning() << "ERROR" << error;
}

void AnnoncesList::annonceLoaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    qDebug() << "annonce loaded" << reply->request().url().toString();
    int index = href().indexOf(reply->request().url().toString());
    QByteArray data = reply->readAll();
    if (data.size()>0)
        parseAnnonce(reply->request().url(), data);
    else
        qWarning() << "ERROR, no data received for" << reply->request().url();
    reply->deleteLater();

    if (m_abort)
        return;

    if (allPagesLoaded)
        emit progress(100*index/m_href.size());

    if ((index+1) < m_href.size())
    {
        // get next annonce
        requestAnnonce(m_href.at(index+1));
    }
    else
    {
        updateLinks();

        emit finished();
    }
}

void AnnoncesList::abort()
{
    m_abort = true;

    QSqlDatabase db = GET_DATABASE("Annonces");
    db.rollback();
}

void AnnoncesList::addAnnonce(const QString &ref)
{
    m_href << url().resolved(ref).toString();
}

void AnnoncesList::linkUpdated(const int &id, const QString &newUrl)
{
    m_linksToUpdate[id] = newUrl;
}

void AnnoncesList::updateLinks()
{
    QSqlDatabase db = GET_DATABASE("Annonces");

    if (db.isOpen())
    {
        foreach (const int &id, m_linksToUpdate.keys())
        {
            qWarning() << "links to update" << id << m_linksToUpdate[id];
            QSqlQuery query(db);

            query.prepare("SELECT is_active, ref from annonces WHERE id=:id");
            query.bindValue(":id", id);
            if (query.exec() && query.next())
            {
                if (query.record().value("is_active").toBool())
                {
                    qCritical() << "annonce" << id << "is active, cannot update the link.";
                }
                else
                {
                    // update the link of exsting annonce
                    query.prepare("UPDATE annonces SET ref=:value WHERE id=:id");
                    query.bindValue(":id", id);
                    query.bindValue(":value", m_linksToUpdate[id]);
                    if (!query.exec())
                        qCritical() << "unable to update annonce" << id << query.lastError().text();
                }
            }
            else
            {
               qCritical() << "unable to find annonce" << id << query.lastError().text();
            }
        }
    }
    else
    {
        qCritical() << "cannot open database to update links.";
    }
}
