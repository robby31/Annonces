import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import MyComponents 1.0

Item {
    id: pages
    anchors { fill:parent; topMargin: mainWindow.headerHeight }

    function toggleMenu() {
        if (mainWindow.state==="NAVIGATOR")
            navigator.toggleMenu()
        else if (mainWindow.state==="ANNONCES")
            annonces.toggleMenu()
    }

    Navigator {
        id: navigator
        anchors.fill: parent
        visible: mainWindow.state === "NAVIGATOR"
    }

    Annonces {
        id: annonces
        anchors.fill: parent
        visible: mainWindow.state === "ANNONCES"
    }
}

