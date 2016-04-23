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

    if (!date().isValid())
    {
        qWarning() << "ERROR, date invalid" << date();
    }
    else
    {
        query.prepare("INSERT INTO annonces (ref, created_date, date, titre, ville, code_postal, marque, modele, annee_modele, km, carburant, bv, description, img) "
                      "VALUES (:ref, :created_date, :date, :titre, :ville, :code_postal, :marque, :modele, :annee_modele, :km, :carburant, :bv, :description, :picture)");
        query.bindValue(":ref", m_url.toString());
        query.bindValue(":created_date", date());
        query.bindValue(":date", QDateTime::currentDateTime());
        query.bindValue(":titre", titre());
        query.bindValue(":ville", ville());
        query.bindValue(":code_postal", code_postal());
        query.bindValue(":marque", marque());
        query.bindValue(":modele", modele());
        query.bindValue(":annee_modele", annee());
        query.bindValue(":km", kilometrage());
        query.bindValue(":carburant", carburant());
        query.bindValue(":bv", boite_vitesse());
        query.bindValue(":description", description());
        query.bindValue(":picture", picture());

        if (!query.exec())
        {
            if (query.lastError().number() == 19 && query.lastError().text().contains("annonces.ref"))
            {
                // update annonce

                // set annonce active
                QSqlQuery querySelect;

                querySelect.prepare("SELECT * from annonces WHERE ref=:ref");
                querySelect.bindValue(":ref", m_url.toString());
                if (querySelect.exec() && querySelect.next())
                {
                    QSqlQuery queryPrix;
                    queryPrix.prepare("SELECT * from prix WHERE annonceid=:id ORDER BY date DESC");
                    queryPrix.bindValue(":id", querySelect.value("id"));
                    if (queryPrix.exec() && queryPrix.next())
                    {
                        // update prix
                        if (queryPrix.value("prix") != prix())
                        {
                            qWarning() << "UPDATE prix" << queryPrix.value("date") << queryPrix.value("prix") << "-->" << prix();

                            queryPrix.prepare("INSERT INTO prix (annonceid, date, prix) VALUES (:id, :date, :prix)");
                            queryPrix.bindValue(":id", querySelect.value("id"));
                            queryPrix.bindValue(":date", QDateTime::currentDateTime());
                            queryPrix.bindValue(":prix", prix());
                            if (!queryPrix.exec())
                                qWarning() << "unable to set prix for annonce" << querySelect.value("id").toLongLong() << queryPrix.lastError().text();
                        }
                    }
                    else
                    {
                        // prix not found
                        queryPrix.prepare("INSERT INTO prix (annonceid, date, prix) VALUES (:id, :date, :prix)");
                        queryPrix.bindValue(":id", querySelect.value("id"));
                        queryPrix.bindValue(":date", QDateTime::currentDateTime());
                        queryPrix.bindValue(":prix", prix());
                        if (!queryPrix.exec())
                            qWarning() << "unable to set prix for annonce" << querySelect.value("id").toLongLong() << queryPrix.lastError().text();
                    }

                    if (!updateParam(querySelect.value("id").toLongLong(), "date", querySelect.value("date").toDateTime(), QDateTime::currentDateTime()))
                        qWarning() << "ERROR, unable to update date for annonce" << querySelect.value("id").toLongLong();

                    if (date() < querySelect.value("created_date").toDateTime())
                    {
                        if (!updateParam(querySelect.value("id").toLongLong(), "created_date", querySelect.value("created_date").toDateTime(), date()))
                            qWarning() << "ERROR, unable to update created_date for annonce" << querySelect.value("id").toLongLong();
                    }

                    if (!updateParam(querySelect.value("id").toLongLong(), "is_active", QVariant(0), QVariant(1)))
                        qWarning() << "ERROR, unable to set annonce active";

                    if (!updateParam(querySelect.value("id").toLongLong(), "titre", querySelect.value("titre"), titre()))
                        qWarning() << "ERROR, unable to update titre for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "ville", querySelect.value("ville"), ville()))
                        qWarning() << "ERROR, unable to update ville for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "code_postal", querySelect.value("code_postal"), code_postal()))
                        qWarning() << "ERROR, unable to update code postal for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "marque", querySelect.value("marque"), marque()))
                        qWarning() << "ERROR, unable to update marque for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "modele", querySelect.value("modele"), modele()))
                        qWarning() << "ERROR, unable to update modele for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "annee_modele", querySelect.value("annee_modele"), annee()))
                        qWarning() << "ERROR, unable to update annee modele for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "km", querySelect.value("km"), kilometrage()))
                        qWarning() << "ERROR, unable to update km for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "carburant", querySelect.value("carburant"), carburant()))
                        qWarning() << "ERROR, unable to update carburant for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "bv", querySelect.value("bv"), boite_vitesse()))
                        qWarning() << "ERROR, unable to update boite de vitesse for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "description", querySelect.value("description"), description()))
                        qWarning() << "ERROR, unable to update description for annonce" << querySelect.value("id").toLongLong();

                    if (!updateParam(querySelect.value("id").toLongLong(), "img", querySelect.value("img"), picture()))
                        qWarning() << "ERROR, unable to update picture for annonce" << querySelect.value("id").toLongLong();
                }
                else
                {
                    qWarning() << query.lastError().text();
                    qWarning() << querySelect.lastError().text();
                    qWarning() << "ERROR, unable to update annonce" << m_url;
                }
            }
            else if (query.lastError().number() == 19 && query.lastError().text().contains("annonces.description"))
            {
                // update annonce

                // set annonce active
                QSqlQuery querySelect;

                querySelect.prepare("SELECT * from annonces WHERE description=:descr");
                querySelect.bindValue(":descr", description());
                if (querySelect.exec() && querySelect.next())
                {
                    emit linkUpdated(querySelect.value("id").toInt(), querySelect.value("ref").toString());
                }
                else
                {
                    qWarning() << query.lastError().text();
                    qWarning() << querySelect.lastError().text();
                    qWarning() << "ERROR, unable to update annonce" << m_url;
                }
            }
            else
            {
                qWarning() << query.lastError().text();
            }
        }
        else
        {
            QSqlQuery queryPrix;

            // set prix for the new annonce
            queryPrix.prepare("INSERT INTO prix (annonceid, date, prix) VALUES (:id, :date, :prix)");
            queryPrix.bindValue(":id", query.lastInsertId());
            queryPrix.bindValue(":date", QDateTime::currentDateTime());
            queryPrix.bindValue(":prix", prix());
            if (!queryPrix.exec())
                qWarning() << "unable to set prix for annonce" << query.lastInsertId() << queryPrix.lastError().text();
        }
    }
}

bool Annonce::updateData(const QString &tablename, const qlonglong &id, QString param, QVariant value)
{
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET %2=:value WHERE id=:id").arg(tablename).arg(param));
    query.bindValue(":id", id);
    query.bindValue(":value", value);
    return query.exec();

}

bool Annonce::updateParam(const qlonglong &id, const QString &param, const QVariant &old, const QVariant &value)
{
    if (old != value)
    {
        if (param != "is_active" && param != "date")
            qWarning() << "UPDATE" << param << old << "-->" << value;
        return updateData("annonces", id, param, value);
    }
    else
    {
        return true;
    }
}
