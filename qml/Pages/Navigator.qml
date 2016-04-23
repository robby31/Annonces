import QtQuick 2.3
import QtQuick.Controls 1.2
import QtWebView 1.0
import MyComponents 1.0

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
            text: "Analyse"
            onClicked: importResults(webview.url)
        }

        TextField {
            height: parent.height
            onAccepted: importResults(text)
        }
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
}

