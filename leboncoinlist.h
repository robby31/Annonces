#ifndef LEBONCOINLIST_H
#define LEBONCOINLIST_H

#include "annonceslist.h"
#include "leboncoin.h"

class LeBonCoinList : public AnnoncesList
{
    Q_OBJECT

public:
    explicit LeBonCoinList(int parserId, QNetworkReply *reply = 0, QObject *parent = 0);

    virtual void readAnnonces(const QByteArray &data);
    virtual void parseAnnonce(const QUrl &url, const QByteArray &data);

    virtual int currentPage();
    virtual QUrl nextPageUrl();
};

#endif // LEBONCOINLIST_H
