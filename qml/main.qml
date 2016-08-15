import QtQuick 2.3
import QtQuick.Controls 1.2
import MyComponents 1.0
import "Pages"

MyApplication {
    id: mainWindow

    property var pageLoaded
    onItemLoaded: pageLoaded = item

    signal importResults(int url)
    signal saveLink(url url, string parserType, string title)

    function parserUpdated() {
        if (pageLoaded)
            pageLoaded.parserUpdated()
    }

    controller: homepagecontroller
    modelButtons : mybuttons
    pages: myPages

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

    Component {
        id: myPages

        ApplicationPages { id: pages }
    }
}
