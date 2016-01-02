import QtQuick 2.3
import QtQuick.Controls 1.2
import MyComponents 1.0
import QtWebKit 3.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    signal importResults(url url)

    AirbusThemeDefault {
        id: theme
    }

    Header {
        id: header
        width: parent.width
        height: 30
    }

    Row {
        id: cmdBar

        height: 30
        width: parent.width
        anchors {
            top: header.bottom
        }

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
        width: parent.width
        anchors {
            top: cmdBar.bottom
            bottom: parent.bottom
        }

        WebView {
            id: webview
            anchors.fill: parent
            url: "http://www.leboncoin.fr/voitures/offres/midi_pyrenees/occasions/?f=a&th=1&pe=22&brd=Bmw&q=330"
        }
    }

    BusyIndicator {
        visible: homepagecontroller.busy
        activity: homepagecontroller.activity
        activityProgress: homepagecontroller.activityProgress
        anchors.fill: parent
    }
}
