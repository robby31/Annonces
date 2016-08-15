import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    id: item
    width: 300
    height: 200

    property alias title: titleField.text
    property alias parser: parserCombo.currentText

    signal save()
    signal cancel()

    ListModel {
        id: parserModel
        ListElement { name: "LeBonCoin"}
    }

    Column
    {
        id: column
        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: buttonRow.top; margins: 10 }
        spacing: 10

        Row {
            Label {
                text: "Title"
                width: 100
            }

            TextField {
                id: titleField
                placeholderText: "choose title name"
            }
        }

        Row {
            Label {
                text: "Parser"
                width: 100
            }

            ComboBox {
                id: parserCombo
                model: parserModel
                textRole: "name"
            }
        }
    }

    Row {
        id: buttonRow
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        height: 40

        Button {
            text: "Cancel"
            onClicked: cancel()
        }

        Button {
            text: "Save"
            enabled: title != ""
            onClicked: save()
        }
    }
}
