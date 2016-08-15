#include "homepagecontroller.h"

Homepagecontroller::Homepagecontroller(QObject *parent) :
    Controller(parent)
{

}

void Homepagecontroller::importAllResults(const int &parserId)
{
    if (setActivity("Loading"))
        emit importAllResultsSignal(parserId);
}

void Homepagecontroller::saveLink(const QUrl &url, const QString &parserType, const QString &title)
{
    if (setActivity("Save link"))
        emit saveLinkSignal(url, parserType, title);
}

void Homepagecontroller::removeParser(const int &parserId)
{
    if (setActivity("Remove"))
        emit removeParserSignal(parserId);
}
