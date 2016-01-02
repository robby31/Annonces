#include "leboncoin.h"

LeBonCoin::LeBonCoin(QUrl url, QByteArray data, QObject *parent) :
    Annonce(url, data, parent),
    m_date(),
    m_titre(),
    m_person()
{
    QRegularExpression reDate("(.*) - Mise en ligne le (.*?)\\.", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);

    QTextDocument doc;

    QTextDecoder decode(QTextCodec::codecForName("iso 8859-15"));
    doc.setHtml(decode.toUnicode(data));

    QTextBlock block = doc.begin();
    while (block.length() != 0)
    {
        QRegularExpressionMatch match = reDate.match(block.text());
        if (match.hasMatch())
        {
            m_person = match.captured(1);
            QDateTime date = QDateTime::fromString(match.captured(2), "dd MMMM à hh:mm");
            date = date.addYears(QDateTime::currentDateTime().toString("yyyy").toInt()-date.toString("yyyy").toInt());
            if (date > QDateTime::currentDateTime())
                date = date.addYears(-1);
            m_date = date;
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
