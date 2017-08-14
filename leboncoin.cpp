#include "leboncoin.h"

LeBonCoin::LeBonCoin(int parserId, QUrl url, QByteArray data, QObject *parent) :
    Annonce(parserId, url, data, parent),
    m_date(),
    m_titre(),
    m_person(),
    m_data()
{
    m_data["Prix"] = "";
    m_data["Ville"] = "";
    m_data["Code postal"] = "";
    m_data["Marque"] = "";
    m_data["Modèle"] = "";
    m_data["Année-modèle"] = "";
    m_data["Kilométrage"] = "";
    m_data["Carburant"] = "";
    m_data["Boîte de vitesse"] = "";
    m_data["Description :"] = "";

    QRegularExpression reDate("Mise en ligne le (.+)", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);

    QTextDocument doc;

    QTextDecoder decode(QTextCodec::codecForName("iso 8859-15"));
    doc.setHtml(decode.toUnicode(data));

    QString prevTextBlock;
    QTextBlock block = doc.begin();
    while (block.length() != 0)
    {
        QRegularExpressionMatch match = reDate.match(block.text());
        if (match.hasMatch())
        {
//            m_person = match.captured(1);
            QDateTime date = QDateTime::fromString(match.captured(1).trimmed(), "d MMMM à hh:mm");
            date = date.addYears(QDateTime::currentDateTime().toString("yyyy").toInt()-date.toString("yyyy").toInt());
            if (date > QDateTime::currentDateTime())
                date = date.addYears(-1);
            m_date = date;
        }
        else
        {
            if (m_data.contains(prevTextBlock))
            {
                m_data[prevTextBlock] = block.text().trimmed();
            }
            else
            {
                QRegularExpression pattern("^(Prix|Ville|Marque|Modèle|Kilométrage|Carburant|Boîte de vitesse|Année-modèle) (.+)");
                QRegularExpressionMatch match = pattern.match(prevTextBlock);
                if (match.hasMatch() && m_data.keys().contains(match.captured(1)))
                {
                    if (match.captured(1) == "Ville")
                    {
                        if (m_data["Ville"].isEmpty())
                        {
                            QRegularExpression ville_code("^(?:(.+) )?(\\d+)$");
                            QRegularExpressionMatch matchVille = ville_code.match(match.captured(2));
                            if (matchVille.hasMatch())
                            {
                                m_data["Ville"] = matchVille.captured(1);
                                if (m_data["Code postal"].isEmpty())
                                    m_data["Code postal"] = matchVille.captured(2);
                            }
                            else
                            {
                                qWarning() << "ERROR" << match.captured(2);
                            }
                        }
                    }
                    else if (match.captured(1) == "Kilométrage")
                    {
                        QString km = match.captured(2);
                        if (m_data[match.captured(1)].isEmpty())
                            m_data[match.captured(1)] = km.left(km.size()-3).replace(" ", "");
                    }
                    else if (match.captured(1) == "Prix" && match.captured(2).endsWith("€"))
                    {
                        QString prix = match.captured(2);
                        if (m_data[match.captured(1)].isEmpty())
                            m_data[match.captured(1)] = prix.left(prix.size()-2).replace(" ", "");
                    }
                    else
                    {
                        if (m_data[match.captured(1)].isEmpty())
                            m_data[match.captured(1)] = match.captured(2);
                    }
                }
            }

            prevTextBlock = block.text().trimmed();
        }
        block = block.next();
    }
}

QString LeBonCoin::titre()
{
    QRegularExpression pattern("<title>(.*?)</title>", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = pattern.match(data());
    if (match.hasMatch())
        return match.captured(1).trimmed();

    return QString();
}

QUrl LeBonCoin::picture()
{
    QRegularExpression pattern("data-imgSrc=\"(.+?)\"", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator iter = pattern.globalMatch(data());
    while (iter.hasNext())
    {
        QRegularExpressionMatch match = iter.next();
        QString img_url = match.captured(1).trimmed();
        return QUrl(url().resolved(img_url));
    }

    return QUrl();
}
