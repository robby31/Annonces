import QtQuick 2.3
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import QtGraphicalEffects 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 100

    onItemDoubleClicked: selectAnnonce()

    function selectAnnonce() {
        delegate.ListView.view.selectAnnonce(model["id"], model["ref"], model["date"], model["created_date"])
    }

    contentItem: Item {
        id: annonces

        property color color: model["is_active"]===1 ? "black" : "grey"

        Row {
            id: annonceText
            anchors { left: parent.left; right: arrow.left; top: parent.top; bottom: parent.bottom }
            spacing: 5
            clip: true

            Image {
                id: picture
                height: delegate.height
                source: model["img"]
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }

            Column {
                spacing: 10
                width: parent.width - picture.width - parent.spacing
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    width: parent.width

                    Text {
                        id: annonceId
                        width: 50
                        font.pixelSize: 18
                        text: model["id"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annonceDate
                        width: 200
                        font.pixelSize: 18
                        text: Qt.formatDateTime(model["created_date"], "dd MMM yyyy")
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annonceLieu
                        width: 400
                        font.pixelSize: 18
                        text: model["code_postal"] + " - " + model["ville"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annonceModele
                        width: 300
                        font.pixelSize: 18
                        text: model["marque"] + " " + model["modele"] + " " + model["carburant"] + " " + model["bv"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annonceAnnee
                        width: 50
                        font.pixelSize: 18
                        text: model["annee_modele"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annonceKm
                        width: 200
                        font.pixelSize: 18
                        text: model["km"] + " km"
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }
                }

                Row {
                    width: parent.width
                    spacing: 10

                    Text {
                        id: annonceTitre
                        font.pixelSize: 18
                        width: parent.width - annoncePrix.width - parent.spacing*2
                        text: model["titre"]
                        elide: Text.ElideRight
                        color: annonces.color
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }

                    Text {
                        id: annoncePrix
                        width: contentWidth
                        font.pixelSize: 18
                        font.bold: true
                        text: model["price"] + " euros"
                        color: annonces.color
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }
                }
            }
        }

        Image {
            id: arrow
            source: "qrc:/images/arrow.png"
            height: 25
            width: 25
            anchors {verticalCenter: parent.verticalCenter; right: parent.right}
            visible: delegate.ListView.isCurrentItem

            MouseArea {
                id: arrowMouseArea
                anchors.fill: parent
                onClicked: selectAnnonce()
            }
        }

        Colorize {
            anchors.fill: arrow
            source: arrow
            hue: 0.0
            saturation: 0.5
            lightness: 1.0
            visible: arrowMouseArea.pressed
        }
    }
}

