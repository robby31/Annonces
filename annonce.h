#ifndef ANNONCE_H
#define ANNONCE_H

#include <QObject>
#include <QUrl>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>

class Annonce : public QObject
{
    Q_OBJECT

public:
    explicit Annonce(QUrl url, QByteArray data, QObject *parent = 0);

    void importData();

    QByteArray data() const { return m_data; }

    virtual QDateTime date() = 0;
    virtual QString titre() = 0;
    virtual QString person() = 0;

signals:

public slots:

private:
    QUrl m_url;
    QByteArray m_data;
};

#endif // ANNONCE_H
