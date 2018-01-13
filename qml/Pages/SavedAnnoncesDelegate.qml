import QtQuick 2.3
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import QtGraphicalEffects 1.0

ListViewDelegate {
    id: delegate
    width: parent ? parent.width : 0
    height: 40

    onDoubleClicked: selectAnnonce()

    function selectAnnonce() {
        selectSavedAnnonce(model["id"], model["title"])
    }

    swipe.left: Row {
        anchors.left: parent.left
        height: parent.height

        Label {
            id: editLabel
            text: qsTr("Edit")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height

            SwipeDelegate.onClicked: {
                swipe.close()
                stack.push("Navigator.qml", {idAnnonce: model["id"], currentUrl: model["url"], currentTitle: model["title"] })
            }

            background: Rectangle {
                color: editLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
            }
        }

        Label {
            id: deleteLabel
            text: qsTr("Remove")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height

            SwipeDelegate.onClicked: {
                swipe.close()
                homepagecontroller.removeParser(model["id"])
            }

            background: Rectangle {
                color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
            }
        }
    }

    contentItem: Item {
        RowLayout {
            id: annonces
            anchors { left: parent.left; right: arrow.left; verticalCenter: parent.verticalCenter }

            Text {
                id: title
                Layout.fillWidth: true
                height: parent.height
                font.pixelSize: 14
                text: model["title"]
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id: parserType
                width: contentWidth
                height: parent.height
                font.pixelSize: 14
                text: model["parser_type"]
                verticalAlignment: Text.AlignVCenter
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
