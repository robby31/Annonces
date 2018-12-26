#include "leboncoinlist.h"

LeBonCoinList::LeBonCoinList(int parserId, QNetworkReply *reply, QObject *parent):
    AnnoncesList(parserId, reply, parent)
{
}

void LeBonCoinList::readAnnonces(const QByteArray &data)
{
    // parse all annonces

    int count_per_page = -1;

    if (nbPages() <= 0)
    {
        QRegularExpression script_config("<script>window.APP_CONFIG\\s*=\\s*(.*)</script>");
        QRegularExpressionMatch match_config = script_config.match(data);
        if (match_config.hasMatch())
        {
            QJsonDocument jsonDocument = QJsonDocument::fromJson(match_config.captured(1).toUtf8());
            if (jsonDocument.isObject())
            {
                QJsonObject jsonObject = jsonDocument.object();
                if (jsonObject.contains("AD_LIST"))
                {
                    QJsonObject ad_list = jsonObject["AD_LIST"].toObject();
                    if (ad_list.contains("COUNT_PER_PAGE"))
                        count_per_page = ad_list["COUNT_PER_PAGE"].toInt();
                }
            }
        }
    }

    if (nbPages() > 0 || count_per_page > 0)
    {
        QRegularExpression script_flux("<script>window.FLUX_STATE\\s*=\\s*(.*)</script>");
        QRegularExpressionMatch match_flux = script_flux.match(data);
        if (match_flux.hasMatch())
        {
            QJsonDocument jsonDocument = QJsonDocument::fromJson(match_flux.captured(1).toUtf8());
            if (jsonDocument.isObject())
            {
                QJsonObject jsonObject = jsonDocument.object();
                if (jsonObject.contains("adSearch"))
                {
                    QJsonObject adSearch = jsonObject["adSearch"].toObject();
                    if (adSearch.contains("data"))
                    {
                        QJsonObject data = adSearch["data"].toObject();

                        if (nbPages() <= 0)
                        {
                            int totalPages = (int)(data["total"].toInt() / count_per_page) + 1;
                            setPages(totalPages);
                        }

                        if (data.contains("ads") && data["ads"].isArray())
                        {
                            foreach (const QJsonValue &value, data["ads"].toArray())
                            {
                                QJsonObject annonce = value.toObject();
                                if (annonce.contains("url"))
                                {
                                    QString url = annonce["url"].toString();
                                    if (!url.endsWith('/'))
                                        url += '/';
                                    addAnnonce(url);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        qCritical() << "invalid count per page" << count_per_page;
    }

    if (nbPages() < 0)
        qCritical() << "invalid number of pages" << nbPages();
}

int LeBonCoinList::currentPage()
{
    QRegularExpression pages("page=(\\d+)");
    QRegularExpressionMatch match = pages.match(url().toString());
    if (match.hasMatch())
        return match.captured(1).toInt();
    else
        return 1;
}

QUrl LeBonCoinList::nextPageUrl()
{
    if (atEnd())
        return QUrl();

    QRegularExpression pages("page=(\\d+)");
    QString str_url = url().toString();
    QRegularExpressionMatch match = pages.match(str_url);
    if (match.hasMatch())
    {
        str_url.replace(pages, QString("page=%1").arg(currentPage()+1));
        return QUrl(str_url);
    }
    else
    {
        return QUrl(str_url + "&page=2");
    }
}

void LeBonCoinList::parseAnnonce(const QUrl &url, const QByteArray &data)
{
    LeBonCoin annonce(parserId(), url, data);
    connect(&annonce, SIGNAL(linkUpdated(int,QString)), this, SLOT(linkUpdated(int,QString)));
    annonce.importData();
}
