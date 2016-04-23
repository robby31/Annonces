import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MyComponents 1.0

Rectangle {
    id: delegate
    width: parent.width
    height: annonces.height
    clip: true

    Rectangle {
        id: hover
        anchors.fill: parent
        color: theme.hoverColor
        visible: mouseArea.containsMouse
    }

    Rectangle {
        id: highlight
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.highlightGradientStart }
            GradientStop { position: 1.0; color: theme.highlightGradientEnd }
        }
        visible: mouseArea.pressed
    }

    Item {
        id: annonces
        height: 100
        width: parent.width
        anchors { right: parent.right; verticalCenter: parent.verticalCenter }

        property color color: model["is_active"]===1 ? "black" : "grey"

        Row {
            id: annonceText
            anchors.fill: parent
            spacing: 10

            Image {
                id: picture
                width: 200
                height: parent.height
                source: model["img"]
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }

            Column {
                width: parent.width-picture.width
                height: parent.height

                Row {
                    height: parent.height/2
                    width: parent.width

                    Text {
                        id: annonceId
                        width: 50
                        height: parent.height
                        font.pointSize: 18
                        text: model["id"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annonceDate
                        width: 200
                        height: parent.height
                        font.pointSize: 18
                        text: Qt.formatDateTime(model["created_date"], "dd MMM yyyy")
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annonceLieu
                        width: 400
                        height: parent.height
                        font.pointSize: 18
                        text: model["code_postal"] + " - " + model["ville"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annonceModele
                        width: 300
                        height: parent.height
                        font.pointSize: 18
                        text: model["marque"] + " " + model["modele"] + " " + model["carburant"] + " " + model["bv"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annonceAnnee
                        width: 100
                        height: parent.height
                        font.pointSize: 18
                        text: model["annee_modele"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annonceKm
                        width: 200
                        height: parent.height
                        font.pointSize: 18
                        text: model["km"] + " km"
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Row {
                    height: parent.height/2
                    width: parent.width

                    Text {
                        id: annonceTitre
                        width: parent.width-annoncePrix.width
                        height: parent.height
                        font.pointSize: 18
                        text: model["titre"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        id: annoncePrix
                        width: 200
                        height: parent.height
                        font.pointSize: 18
                        font.bold: true
                        text: model["price"] + " euros"
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: delegate.ListView.view.selectAnnonce(model["id"], model["ref"], model["date"], model["created_date"])
    }
}

