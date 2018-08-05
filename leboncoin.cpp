#include "leboncoin.h"

LeBonCoin::LeBonCoin(int parserId, QUrl url, QByteArray data, QObject *parent) :
    Annonce(parserId, url, data, parent)
{
    m_data["Prix"] = "";
    m_data["Ville"] = "";
    m_data["Code postal"] = "";
    m_data["Marque"] = "";
    m_data["Modèle"] = "";
    m_data["Année-modèle"] = "";
    m_data["Carburant"] = "";
    m_data["Boîte de vitesse"] = "";
    m_data["Description"] = "";

    QRegularExpression script_flux("<script>window.FLUX_STATE\\s*=\\s*(.*)</script>");
    QRegularExpressionMatch match_flux = script_flux.match(data);
    if (match_flux.hasMatch())
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(match_flux.captured(1).toUtf8());
        if (jsonDocument.isObject())
        {
            QJsonObject jsonObject = jsonDocument.object();
            if (jsonObject.contains("adview"))
                m_adView = jsonObject["adview"].toObject();
        }
    }

    if (m_adView.contains("price"))
    {
        QJsonArray price = m_adView["price"].toArray();

        if (price.size() == 1)
            m_data["Prix"] = QVariant::fromValue(price.at(0).toDouble()).toString();
        else
            qWarning() << "several price in annonce" << url << m_adView["price"];
    }

    if (m_adView.contains("body"))
        m_data["Description"] = m_adView["body"].toString();

    if (m_adView.contains("location"))
    {
        QJsonObject location = m_adView["location"].toObject();
        if (location.contains("city"))
            m_data["Ville"] = location["city"].toString();
        if (location.contains("zipcode"))
            m_data["Code postal"] = location["zipcode"].toString();
    }

    if (m_adView.contains("attributes"))
    {
        foreach (const QJsonValue &data, m_adView["attributes"].toArray())
        {
            QJsonObject attribute = data.toObject();
            if (attribute.contains("key") << attribute.contains("value_label"))
            {
                QString key = attribute["key"].toString();

                if (key == "brand")
                    m_data["Marque"] = attribute["value_label"].toString();
                else if (key == "model")
                    m_data["Modèle"] = attribute["value_label"].toString();
                else if (key == "regdate")
                    m_data["Année-modèle"] = attribute["value_label"].toString();
                else if (key == "mileage")
                {
                    QString km = attribute["value_label"].toString();
                    if (km.contains(' '))
                        m_data["Kilométrage"] = km.split(' ').at(0);
                    else
                        m_data["Kilométrage"] = km;
                }
                else if (key == "fuel")
                    m_data["Carburant"] = attribute["value_label"].toString();
                else if (key == "gearbox")
                    m_data["Boîte de vitesse"] = attribute["value_label"].toString();
            }
        }
    }
}

QDateTime LeBonCoin::date()
{
    if (m_adView.contains("first_publication_date"))
        return QDateTime::fromString(m_adView["first_publication_date"].toString(), "yyyy-MM-dd hh:mm:ss");

    return QDateTime();
}

QString LeBonCoin::titre()
{
    if (m_adView.contains("subject"))
        return m_adView["subject"].toString();
    else
        return QString();
}

QString LeBonCoin::person()
{
    if (m_adView.contains("owner"))
    {
        QJsonObject owner = m_adView["owner"].toObject();
        if (owner.contains("name"))
            return owner["name"].toString();
    }

    return QString();
}

QUrl LeBonCoin::picture()
{
    if (m_adView.contains("images"))
    {
        QJsonObject images = m_adView["images"].toObject();
        if (images.contains("thumb_url"))
            return QUrl(images["thumb_url"].toString());
    }

    return QUrl();
}
