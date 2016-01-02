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
    explicit LeBonCoin(QUrl url, QByteArray data, QObject *parent = 0);

    virtual QDateTime date() { return m_date; }
    virtual QString titre();
    virtual QString person() { return m_person; }

signals:

public slots:

private:
    QDateTime m_date;
    QString m_titre;
    QString m_person;
};

#endif // LEBONCOIN_H
