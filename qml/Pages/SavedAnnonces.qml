import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import Models 1.0
import MyComponents 1.0

Page {
    width: 300
    height: 200

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            Qt.quit()
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }


    function selectSavedAnnonce(id) {
        loader.setSource("Annonces.qml", {parserId: id})
    }

    function backToSavedAnnonces() {
        loader.source = "AnnoncesListView.qml"
    }

    function parserUpdated() {
        if (loader.source == "qrc:/qml/Pages/AnnoncesListView.qml")
            loader.item.parserUpdated()
    }

    Loader {
        id: loader
        anchors.fill: parent
        source: "AnnoncesListView.qml"
    }
}

