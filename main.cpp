#include "annoncesapplication.h"
#include "annoncemodel.h"

int main(int argc, char **argv)
{
    qmlRegisterType<AnnonceModel>("myTypes", 1, 0, "AnnonceModel");

    AnnoncesApplication app(argc, argv);

    app.loadMainQml(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
