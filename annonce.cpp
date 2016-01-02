#include "annonce.h"

Annonce::Annonce(QUrl url, QByteArray data, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_data(data)
{

}

void Annonce::importData()
{
    QSqlQuery query;

    query.prepare("INSERT INTO annonces (ref, date, titre) VALUES (:ref, :date, :titre)");
    query.bindValue(":ref", m_url.toString());
    query.bindValue(":date", date());
    query.bindValue(":titre", titre());
    if (!query.exec())
        qWarning() << query.lastError().text();

    qWarning() << date();
    qWarning() << titre();
}
