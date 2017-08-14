import QtQuick 2.3
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MyComponents 1.0
import QtGraphicalEffects 1.0

ListViewDelegate {
    id: delegate
    width: parent.width
    height: 40

    swipe.left: Label {
        id: deleteLabel
        text: qsTr("Remove")
        color: "white"
        verticalAlignment: Label.AlignVCenter
        padding: 12
        height: parent.height
        anchors.left: parent.left

        SwipeDelegate.onClicked: {
            swipe.close()
            _app.removeParser(model["id"])
        }

        background: Rectangle {
            color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
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
                font.pointSize: 10
                text: model["title"]
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id: parserType
                width: contentWidth
                height: parent.height
                font.pointSize: 10
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
                onClicked: selectSavedAnnonce(model["id"])
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
