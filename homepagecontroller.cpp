#include "homepagecontroller.h"

Homepagecontroller::Homepagecontroller(QObject *parent) :
    Controller(parent)
{

}

void Homepagecontroller::importAllResults(const int &parserId)
{
    if (setActivity("Loading"))
        emit importAllResultsSignal(parserId);
}

void Homepagecontroller::saveLink(const QUrl &url, const QString &parserType, const QString &title)
{
    if (setActivity("Save link"))
        emit saveLinkSignal(url, parserType, title);
}

void Homepagecontroller::updateLink(const int &id, const QUrl &url, const QString &title)
{
    QSqlDatabase db = GET_DATABASE("Annonces");
    if (!db.isOpen())
    {
        errorDuringProcess("Unable to open Database.");
    }
    else
    {
        db.transaction();

        QSqlQuery query(db);
        query.prepare("UPDATE parser SET url=:url WHERE id=:id");
        query.bindValue(":id", id);
        query.bindValue(":url", url);
        if (!query.exec())
        {
            qCritical() << query.lastError().text();
            db.rollback();
            errorDuringProcess("Unable to update parser.");
        }
        else
        {
            query.prepare("UPDATE parser SET title=:title WHERE id=:id");
            query.bindValue(":id", id);
            query.bindValue(":title", title);
            if (!query.exec())
            {
                qCritical() << query.lastError().text();
                db.rollback();
                errorDuringProcess("Unable to update parser.");
            }
            else
            {
                db.commit();
                processOver("parser updated.");
                emit parserUpdated();
            }
        }
    }
}

void Homepagecontroller::removeParser(const int &parserId)
{
    if (setActivity("Remove"))
        emit removeParserSignal(parserId);
}
