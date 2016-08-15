import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import Models 1.0
import MyComponents 1.0

ColumnLayout {
    id: columnAnnonces
    anchors.fill: parent

    function parserUpdated() {
        annoncesModel.reload()
    }

    SqlListModel {
        id: annoncesModel
        connectionName: "Annonces"
        tablename: "parser"
        query: "SELECT * from parser ORDER BY title"
    }

    Rectangle {
        id: header
        height: 40
        Layout.fillWidth: true

        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.gradientStartColor }
            GradientStop { position: 1.0; color: theme.gradientEndColor }
        }

        Text {
            id: text
            anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
            width: contentWidth
            height: contentHeight
            text: annoncesModel ? annoncesModel.rowCount + " annonces." : ""
            color: "blue"
        }
    }

    ScrollView {
        id: listAnnonces
        Layout.fillHeight: true
        Layout.fillWidth: true

        ListView {
            id: listview
            anchors.fill: parent
            model: annoncesModel

            delegate: SavedAnnoncesDelegate { }
            focus: true
            highlightFollowsCurrentItem: false
            clip: true
        }
    }
}
