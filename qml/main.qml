import QtQuick 2.3
import QtQuick.Controls 1.2
import MyComponents 1.0
import "Pages"

MyApplication {
    id: mainWindow

    signal importResults(url url)

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

        ApplicationPages {

        }
    }
}
