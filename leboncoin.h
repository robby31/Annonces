#ifndef LEBONCOIN_H
#define LEBONCOIN_H

#include "annonce.h"
#include <QTextDecoder>
#include <QTextDocument>
#include <QTextBlock>

class LeBonCoin : public Annonce
{
    Q_OBJECT

public:
    explicit LeBonCoin(int parserId, QUrl url, QByteArray data, QObject *parent = 0);

    virtual QDateTime date() { return m_date; }
    virtual QString titre();
    virtual QString person()        { return m_person; }
    virtual QString ville()         { return m_data["Ville"]; }
    virtual QString code_postal()   { return m_data["Code postal"]; }
    virtual QString marque()        { return m_data["Marque"]; }
    virtual QString modele()        { return m_data["Modèle"]; }
    virtual int annee()             { return m_data["Année-modèle"].toInt(); }
    virtual int kilometrage()       { return m_data["Kilométrage"].toInt(); }
    virtual QString carburant()     { return m_data["Carburant"]; }
    virtual QString boite_vitesse() { return m_data["Boîte de vitesse"]; }
    virtual QString description()   { return m_data["Description :"]; }
    virtual int prix()              { return m_data["Prix"].toInt(); }
    virtual QUrl picture();

signals:

public slots:

private:
    QDateTime m_date;
    QString m_titre;
    QString m_person;
    QHash<QString, QString> m_data;
};

#endif // LEBONCOIN_H
