#include "annonceslist.h"

AnnoncesList::AnnoncesList(QNetworkReply *reply, QObject *parent) :
    QObject(parent),
    m_reply(reply),
    m_href(),
    m_nbpages(-1),
    allPagesLoaded(false)
{
    if (reply)
    {
        connect(reply, SIGNAL(finished()), this, SLOT(pageLoaded()));
        connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
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

//    save(data, "results.txt");

    if  (!atEnd())
    {
        if (m_reply != reply)
        {
            m_reply->deleteLater();
            m_reply = reply;
        }
        else if (href().size() >= 1)
        {
            // first page loaded, start loading of first annonce
            requestAnnonce(m_href.at(0));
        }

        QNetworkReply *reply = m_reply->manager()->get(QNetworkRequest(nextPageUrl()));
        connect(reply, SIGNAL(finished()), this, SLOT(pageLoaded()));
        connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
    }
    else
    {
        allPagesLoaded = true;
    }
}

void AnnoncesList::requestAnnonce(const QUrl &url)
{
    QNetworkReply *reply = m_reply->manager()->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(annonceLoaded()));
    connect(this, SIGNAL(destroyed(QObject*)), reply, SLOT(deleteLater()));
}

void AnnoncesList::annonceLoaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int index = href().indexOf(reply->request().url().toString());
    parseAnnonce(reply->request().url(), reply->readAll());
    reply->deleteLater();

    if (allPagesLoaded)
        emit progress(100*index/m_href.size());

    if ((index+1) < m_href.size())
    {
        // get next annonce
        requestAnnonce(m_href.at(index+1));
    }
    else
    {
        emit finished();
    }
}
