import QtQuick 2.3
import QtQuick.Controls 2.2
import MyComponents 1.0
import "Pages"

MyApplication {
    id: mainWindow

    property var pageLoaded
    onItemLoaded: pageLoaded = item

    Connections {
        target: _app

        onParserUpdatedSignal: {
            if (pageLoaded)
                pageLoaded.parserUpdated()
        }

        onAnnoncesUpdatedSignal: {
            if (pageLoaded)
                pageLoaded.annoncesUpdated()
        }
    }

    controller: homepagecontroller
    modelButtons : mybuttons
    srcPages: _app.databasePathName.toString() === "" ? "SelectDatabase.qml" : "Pages/ApplicationPages.qml"

    ListModel {
        id: mybuttons

        ListElement {
            title: "Annonces"
            state: "ANNONCES"
        }
    }
}
