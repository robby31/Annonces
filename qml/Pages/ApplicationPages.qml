import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import MyComponents 1.0

Item {
    id: pages

    function toggleMenu() {
        if (mainWindow.state==="ANNONCES")
            annonces.toggleMenu()
    }

    SavedAnnonces {
        id: annonces
        anchors.fill: parent
        visible: mainWindow.state === "ANNONCES"
    }
}

