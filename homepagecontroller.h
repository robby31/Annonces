#ifndef HOMEPAGECONTROLLER_H
#define HOMEPAGECONTROLLER_H

#include "UIController/controller.h"

class Homepagecontroller : public Controller
{
    Q_OBJECT

public:
    explicit Homepagecontroller(QObject *parent = 0);

signals:
    void importAllResultsSignal(const QUrl &url);

public slots:
    void importAllResults(const QUrl &url);

};

#endif // HOMEPAGECONTROLLER_H
