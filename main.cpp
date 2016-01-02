#include "annoncesapplication.h"

int main(int argc, char *argv[])
{
    AnnoncesApplication app(argc, argv);

    app.loadMainQml(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
