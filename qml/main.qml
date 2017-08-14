import QtQuick 2.3
import QtQuick.Controls 1.2
import MyComponents 1.0
import "Pages"
import QtQuick.Dialogs 1.2

MyApplication {
    id: mainWindow

    property var pageLoaded
    onItemLoaded: pageLoaded = item

    signal importResults(int url)
    signal saveLink(url url, string parserType, string title)

    function chooseDatabase() {
        chooseDatabaseDialog.open()
    }

    function parserUpdated() {
        if (pageLoaded)
            pageLoaded.parserUpdated()
    }

    function annoncesUpdated() {
        if (pageLoaded)
            pageLoaded.annoncesUpdated()
    }

    controller: homepagecontroller
    modelButtons : mybuttons
    srcPages: _app.databasePathName.toString() === "" ? "SelectDatabase.qml" : "Pages/ApplicationPages.qml"

    ListModel {
        id: mybuttons

        ListElement {
            title: "Search"
            state: "NAVIGATOR"
        }

        ListElement {
            title: "Annonces"
            state: "ANNONCES"
        }
    }

    FileDialog {
        id: chooseDatabaseDialog
        selectExisting: true
        title: "Choose database file"
        nameFilters: [ "Database (*.sql)" ]
        onAccepted: _app.databasePathName = fileUrl
    }
}
