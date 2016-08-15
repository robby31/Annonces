import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MyComponents 1.0
import QtGraphicalEffects 1.0

Item {
    id: delegate
    width: parent.width
    height: annonces.height

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

    RowLayout {
        id: annonces
        height: 40
        anchors { left: parent.left; right: arrow.left; verticalCenter: parent.verticalCenter }

        MyButton {
            id: remove
            sourceComponent: Text { text: "Remove" }
            onButtonClicked: _app.removeParser(model["id"])
            visible: delegate.ListView.isCurrentItem
        }

        Text {
            id: title
            Layout.fillWidth: true
            height: parent.height
            font.pixelSize: 18
            text: model["title"]
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: parserType
            width: contentWidth
            height: parent.height
            font.pixelSize: 18
            text: model["parser_type"]
            verticalAlignment: Text.AlignVCenter
        }
    }

    Image {
        id: arrow
        source: "qrc:/images/arrow.png"
        height: parent.height/2
        width: height
        anchors {verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 10}
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
        visible: mouseArea.pressed || arrowMouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        visible: !delegate.ListView.isCurrentItem
        onClicked: delegate.ListView.view.currentIndex = index
    }
}

