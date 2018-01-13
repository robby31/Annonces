#ifndef HOMEPAGECONTROLLER_H
#define HOMEPAGECONTROLLER_H

#include "UIController/controller.h"
#include <QDebug>
#include "mysqldatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

class Homepagecontroller : public Controller
{
    Q_OBJECT

public:
    explicit Homepagecontroller(QObject *parent = 0);

signals:
    void importAllResultsSignal(const int &parserId);
    void saveLinkSignal(const QUrl &url, const QString &parserType, const QString &title);
    void removeParserSignal(const int &parserId);
    void parserUpdated();
    void annoncesUpdated();

public slots:
    void importAllResults(const int &parserId);
    void saveLink(const QUrl &url, const QString &parserType, const QString &title);
    void updateLink(const int &id, const QUrl &url, const QString &title);
    void removeParser(const int &parserId);
};

#endif // HOMEPAGECONTROLLER_H
