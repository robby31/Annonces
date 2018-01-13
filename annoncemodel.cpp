#include "annoncemodel.h"

AnnonceModel::AnnonceModel(QObject *parent):
    SqlListModel(parent)
{

}

QVariantMap AnnonceModel::getAnnonce(const double &km, const double &euros)
{
    QVariantMap res;

    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare(QString("SELECT * FROM (%1) WHERE km=:km and price=:euros").arg(this->query()));
        query.bindValue(":km", km);
        query.bindValue(":euros", euros);
        if (query.exec())
        {
            if (query.next())
            {
                QSqlRecord record = query.record();
                for (int i=0;i<record.count();++i)
                {
                    QSqlField param = record.field(i);
                    res[param.name()] = param.value();
                }
            }

            query.last();
            if (query.at() > 0)
                qWarning() << query.at()+1 << "annonces returned.";
        }
        else
        {
            qCritical() << query.lastError().text();
        }

    }

    return res;
}
void AnnonceModel::setVersionModele(const int &annonceId, const QString &version)
{
    setOptionalParam(annonceId, "version_modele", version);
}

void AnnonceModel::setOptionalParam(const int &annonceId, const QString &name, const QString &value)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery dbQuery(db);
        dbQuery.prepare("SELECT * FROM optional_param JOIN param_name ON optional_param.param=param_name.id WHERE annonceId=:id AND param_name.name=:name");
        dbQuery.bindValue(":id", annonceId);
        dbQuery.bindValue(":name", name);
        if (dbQuery.exec())
        {
            int valueId = getOptionalValueId(value, true);
            if (valueId == -2)
            {
                qCritical() << "unable to get id for value" << value;
                return;
            }

            if (dbQuery.next())
            {
                QSqlQuery updateQuery(db);
                updateQuery.prepare("UPDATE optional_param SET value=:valueId WHERE id=:id");
                updateQuery.bindValue(":id", dbQuery.value("id"));
                updateQuery.bindValue(":valueId", valueId);
                if (!updateQuery.exec())
                    qCritical() << "unable to update" << updateQuery.lastError().text();
            }
            else
            {
                int paramId = getOptionalParamId(name, true);
                if (paramId == -2)
                {
                    qCritical() << "unable to get id for" << name;
                    return;
                }

                QSqlQuery insertQuery(db);
                insertQuery.prepare("INSERT INTO optional_param(annonceId, param, value) VALUES (:annonceId, :param, :value)");
                insertQuery.bindValue(":annonceId", annonceId);
                insertQuery.bindValue(":param", paramId);
                insertQuery.bindValue(":value", valueId);
                if (!insertQuery.exec())
                    qCritical() << "unable to insert" << insertQuery.lastError().text();
            }
        }
        else
        {
            qCritical() << dbQuery.lastError().text();
        }
    }
    else
    {
        qCritical() << "database not open.";
    }
}

int AnnonceModel::getOptionalParamId(const QString &name, bool createParam)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery dbQuery(db);
        dbQuery.prepare("SELECT * FROM param_name WHERE name=:name");
        dbQuery.bindValue(":name", name);
        if (dbQuery.exec() && dbQuery.next())
        {
            return dbQuery.value("id").toInt();
        }
        else
        {
            if (createParam)
                return addOptionalParam(name);
            else
                return -1;
        }
    }
    else
    {
        return -2;
    }
}

int AnnonceModel::getOptionalValueId(const QString &value, bool createParam)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery dbQuery(db);
        dbQuery.prepare("SELECT * FROM param_value WHERE value=:value");
        dbQuery.bindValue(":value", value);
        if (dbQuery.exec() && dbQuery.next())
        {
            return dbQuery.value("id").toInt();
        }
        else
        {
            if (createParam)
                return addOptionalValue(value);
            else
                return -1;
        }
    }
    else
    {
        return -2;
    }
}

int AnnonceModel::addOptionalParam(const QString &name)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery dbQuery(db);
        dbQuery.prepare("INSERT INTO param_name(name) VALUES (:name)");
        dbQuery.bindValue(":name", name);
        if (dbQuery.exec())
            return dbQuery.lastInsertId().toInt();
        else
            return -1;
    }
    else
    {
        return -2;
    }
}

int AnnonceModel::addOptionalValue(const QString &value)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (db.isOpen())
    {
        QSqlQuery dbQuery(db);
        dbQuery.prepare("INSERT INTO param_value(value) VALUES (:value)");
        dbQuery.bindValue(":value", value);
        if (dbQuery.exec())
            return dbQuery.lastInsertId().toInt();
        else
            return -1;
    }
    else
    {
        return -2;
    }
}
