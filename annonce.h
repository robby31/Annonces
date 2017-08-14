#ifndef ANNONCE_H
#define ANNONCE_H

#include <QObject>
#include <QUrl>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include "mysqldatabase.h"

class Annonce : public QObject
{
    Q_OBJECT

public:
    explicit Annonce(int parserId, QUrl url, QByteArray data, QObject *parent = 0);

    int parserId() const { return m_parserId; }
    void importData();

    QByteArray data() const { return m_data; }

    QUrl url() const;

    virtual QDateTime date() = 0;
    virtual QString titre() = 0;
    virtual QString person() = 0;

    virtual QString ville() = 0;
    virtual QString code_postal() = 0;

    virtual QString marque() = 0;
    virtual QString modele() = 0;
    virtual int annee() = 0;
    virtual int kilometrage() = 0;
    virtual QString carburant() = 0;
    virtual QString boite_vitesse() = 0;

    virtual QUrl picture() = 0;

    virtual QString description() = 0;

    virtual int prix() = 0;

private:
    bool updateData(const QString &tablename, const qlonglong &id, QString param, QVariant value);
    bool updateParam(const qlonglong &id, const QString &param, const QVariant &old, const QVariant &value);

signals:
    void linkUpdated(const int &id, const QString &newUrl);

private:
    int m_parserId;
    QUrl m_url;
    QByteArray m_data;
};

#endif // ANNONCE_H
