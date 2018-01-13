#ifndef ANNONCEMODEL_H
#define ANNONCEMODEL_H

#include "Models/sqllistmodel.h"

class AnnonceModel : public SqlListModel
{
    Q_OBJECT

public:
    explicit AnnonceModel(QObject *parent = 0);

public slots:
    QVariantMap getAnnonce(const double &km, const double &euros);
    void setVersionModele(const int &annonceId, const QString &version);

private:
    void setOptionalParam(const int &annonceId, const QString &name, const QString &value);

    int getOptionalParamId(const QString &name, bool createParam=false);
    int getOptionalValueId(const QString &value, bool createParam=false);

    int addOptionalParam(const QString &name);
    int addOptionalValue(const QString &value);

};

#endif // ANNONCEMODEL_H
