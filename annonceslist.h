#ifndef ANNONCESLIST_H
#define ANNONCESLIST_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include <QRegularExpression>


class AnnoncesList : public QObject
{
    Q_OBJECT

public:
    explicit AnnoncesList(QNetworkReply *reply = 0, QObject *parent = 0);

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

private slots:
    void pageLoaded();
    void annonceLoaded();
    void replyError(const QNetworkReply::NetworkError &error);
    void abort();
    void linkUpdated(const int &id, const QString &newUrl);

private:
    QNetworkReply *m_reply;
    QStringList m_href;
    int m_nbpages;
    bool allPagesLoaded;
    bool m_abort;
    QHash<int, QString> m_linksToUpdate;
};

#endif // ANNONCESLIST_H
