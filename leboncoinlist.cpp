#include "leboncoinlist.h"

LeBonCoinList::LeBonCoinList(QNetworkReply *reply, QObject *parent):
    AnnoncesList(reply, parent)
{
}

void LeBonCoinList::readAnnonces(const QByteArray &data)
{
    // parse all annonces
    QRegularExpression lbc("<div class=\"list-lbc\">(.*)", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch res = lbc.match(data);
    if (res.hasMatch())
    {
        QRegularExpression ref("<a href=\"([^\"]+)\"(.+?)</a>", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatchIterator it = ref.globalMatch(res.captured(1));
        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            if (match.captured(2).contains("<div class=\"lbc\">"))
                addAnnonce(match.captured(1));
            else
            {
                // read the number of pages to parse to get all annonces
                QRegularExpression pages("o=(\\d+)");
                QRegularExpressionMatch pages_matched = pages.match(match.captured(1));
                if (pages_matched.hasMatch())
                {
                    int nb = pages_matched.captured(1).toInt();
                    if (nb>0 && nbPages()<nb)
                        setPages(nb);
                }
            }
        }
    }
}

int LeBonCoinList::currentPage()
{
    QRegularExpression pages("o=(\\d+)");
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

    QRegularExpression pages("o=(\\d+)");
    QString str_url = url().toString();
    QRegularExpressionMatch match = pages.match(str_url);
    if (match.hasMatch())
    {
        str_url.replace(pages, QString("o=%1").arg(currentPage()+1));
        return QUrl(str_url);
    }
    else
    {
        return QUrl(str_url + "&o=2");
    }
}

void LeBonCoinList::parseAnnonce(const QUrl &url, const QByteArray &data)
{
    qWarning() << "parsse annonce" << url;

    LeBonCoin annonce(url, data);
    annonce.importData();

//    QFile file("annonce.txt");
//    if (file.open(QIODevice::ReadWrite))
//    {
//        file.write(data);
//        file.close();
//    }
}
