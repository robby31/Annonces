import QtQuick 2.3
import QtQuick.Controls 1.2
import QtWebView 1.0
import MyComponents 1.0
import QtQuick.Dialogs 1.2

Page {
    width: 300
    height: 200

    Row {
        id: cmdBar

        height: 30
        width: parent.width

        Button {
            text: "Home"
            onClicked: webview.url = "http://www.leboncoin.fr/"
        }

        Button {
            text: "Prev"
            onClicked: webview.goBack()
        }

        Button {
            text: "Next"
            onClicked: webview.goForward()
        }


        Button {
            text: "Save"
            onClicked: saveDialog.visible = true
        }

//        TextField {
//            height: parent.height
//            onAccepted: importResults(text)
//        }
    }

    ScrollView {
        id: scrollwebview
        width: parent.width
        anchors {
            top: cmdBar.bottom
            bottom: parent.bottom
        }

        WebView {
            id: webview
            width: scrollwebview.width
            height: scrollwebview.height
            url: "http://www.leboncoin.fr/voitures/offres/midi_pyrenees/occasions/?f=a&th=1&pe=22&brd=Bmw&q=330"
            Component.onCompleted: url = "http://www.leboncoin.fr/voitures/offres/midi_pyrenees/occasions/?f=a&th=1&pe=22&brd=Bmw&q=330"
        }
    }

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

    Dialog {
        id: saveDialog
        title: "Save annonces"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        width: 500
        height: 300
        visible: false

        contentItem: SaveDialog {
            id: saveItem
            implicitWidth: 400
            implicitHeight: 120

            onSave: saveDialog.click(StandardButton.Ok)
            onCancel: saveDialog.click(StandardButton.Cancel)
        }

        onAccepted: {
            if (saveItem.title != "")
                saveLink(webview.url, saveItem.parser, saveItem.title)
            else
                rejected()
        }
    }
}

