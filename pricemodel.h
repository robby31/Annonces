#ifndef PRICEMODEL_H
#define PRICEMODEL_H

#include <QDateTime>
#include "Models/sqllistmodel.h"

class PriceModel : public SqlListModel
{
    Q_OBJECT

public:
    explicit PriceModel(QObject *parent = 0);

    Q_INVOKABLE qint64 getDate(int i);
    Q_INVOKABLE QDateTime minDate();
    Q_INVOKABLE QDateTime maxDate();

    Q_INVOKABLE qint64 getCurrentDate();
    Q_INVOKABLE QVariant getCurrentPrice();

signals:

public slots:
};

#endif // PRICEMODEL_H
