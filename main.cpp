#include "annoncesapplication.h"

int main(int argc, char **argv)
{
    AnnoncesApplication app(argc, argv);

    app.loadMainQml(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
