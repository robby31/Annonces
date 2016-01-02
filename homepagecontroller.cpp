#include "homepagecontroller.h"

Homepagecontroller::Homepagecontroller(QObject *parent) :
    Controller(parent)
{

}

void Homepagecontroller::importAllResults(const QUrl &url)
{
    if (setActivity("Loading"))
        emit importAllResultsSignal(url);
}
