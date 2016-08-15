#include "pricemodel.h"

PriceModel::PriceModel(QObject *parent):
    SqlListModel(parent)
{

}

qint64 PriceModel::getDate(int i)
{
    return get(i, "date").toDateTime().toMSecsSinceEpoch();
}

QDateTime PriceModel::maxDate()
{
    return QDateTime::currentDateTime();
}

QDateTime PriceModel::minDate()
{
    QDateTime min;
    for (int i=0;i<rowCount();++i)
    {
        QDateTime value = get(i, "date").toDateTime();
        if (!min.isValid() || value<min)
            min = value;
    }
    return min;
}

qint64 PriceModel::getCurrentDate()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

QVariant PriceModel::getCurrentPrice()
{
    return get(rowCount()-1, "prix");
}
