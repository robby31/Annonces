#include "leboncoinlist.h"

LeBonCoinList::LeBonCoinList(int parserId, QNetworkReply *reply, QObject *parent):
    AnnoncesList(parserId, reply, parent)
{
}

void LeBonCoinList::readAnnonces(const QByteArray &data)
{
    // parse all annonces

    // links of annonces
    QRegularExpression ref("<li itemscope.+?<a href=\"([^\"]+)\"(.+?)</a>", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator it = ref.globalMatch(data);
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        if (match.captured(0).contains("class=\"list_item clearfix trackable\"") && !match.captured(1).contains("gallery"))
            addAnnonce(match.captured(1));
    }

    // get number of pages
    QRegularExpression refPage("<a class=\"element page\" href=\"([^\"]+)\"(.+?)</a>", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator itPage = refPage.globalMatch(data);
    while (itPage.hasNext())
    {
        QRegularExpressionMatch match = itPage.next();

        // read the number of pages to parse to get all annonces
        QRegularExpression pages("o=(\\d+)");
        QRegularExpressionMatch pages_matched = pages.match(match.captured(0));
        if (pages_matched.hasMatch())
        {
            int nb = pages_matched.captured(1).toInt();
            if (nb>0 && nbPages()<nb)
                setPages(nb);
        }
    }

    if (nbPages() < 0)
    {
        qCritical() << "invalid number of pages" << nbPages();
        qWarning() << data;
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
    LeBonCoin annonce(parserId(), url, data);
    connect(&annonce, SIGNAL(linkUpdated(int,QString)), this, SLOT(linkUpdated(int,QString)));
    annonce.importData();
}
