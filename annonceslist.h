#ifndef ANNONCESLIST_H
#define ANNONCESLIST_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "mysqldatabase.h"

class AnnoncesList : public QObject
{
    Q_OBJECT

public:
    explicit AnnoncesList(int parserId, QNetworkReply *reply = 0, QObject *parent = 0);

    int parserId() const { return m_parserId; }

    bool save(const QByteArray &data, QString pathname);

    int nbPages() const { return m_nbpages; }
    void setPages(const int &nb);

    QUrl url() const;
    QStringList href() const { return m_href; }

    virtual void readAnnonces(const QByteArray &data) = 0;
    void addAnnonce(const QString &ref);

    virtual void parseAnnonce(const QUrl &url, const QByteArray &data) = 0;

    virtual int currentPage() = 0;
    bool atEnd();
    virtual QUrl nextPageUrl() = 0;

private:
    void requestAnnonce(const QUrl &url);
    void updateLinks();

signals:
    void progress(const int &value);
    void finished();
    void error(const QString &message);

private slots:
    void pageLoaded();
    void annonceLoaded();
    void replyError(const QNetworkReply::NetworkError &error);
    void abort();
    void linkUpdated(const int &id, const QString &newUrl);

private:
    int m_parserId;
    QNetworkReply *m_reply;
    QStringList m_href;
    int m_nbpages;
    bool allPagesLoaded;
    bool m_abort;
    QHash<int, QString> m_linksToUpdate;
};

#endif // ANNONCESLIST_H
