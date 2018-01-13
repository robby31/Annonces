import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtWebView 1.0
import MyComponents 1.0
import QtQuick.Dialogs 1.2

Item {
    id: item

    property int idAnnonce: -1
    property string currentUrl: ""
    property string currentTitle: ""

    ListModel {
        id: parserModel
        ListElement { name: "LeBonCoin"; url: "https://www.leboncoin.fr/annonces/offres/midi_pyrenees/"}
    }

    function checkUrl(url, indexParser) {
        return titleField.text != ""
    }

    RowLayout {
        id: cmdBar

        width: parent.width
        height: 30

        ComboBox {
            id: parserCombo
            anchors.verticalCenter: parent.verticalCenter
            width: 200
            Layout.preferredHeight: parent.height
            visible: idAnnonce == -1

            model: parserModel
            textRole: "name"
            onCurrentIndexChanged: webview.url = parserModel.get(currentIndex).url
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: "Reload"
            onClicked: webview.reload()
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: "Home"
            onClicked: {
                var index = parserCombo.currentIndex
                if (index >= 0)
                    webview.url = parserModel.get(index).url
            }
        }


        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: "Prev"
            onClicked: webview.goBack()
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: "Next"
            onClicked: webview.goForward()
        }


        TextField {
            id: titleField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            placeholderText: "choose title"
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: idAnnonce == -1 ? "Save" : "Update"
            enabled: checkUrl(webview.url, parserCombo.currentIndex)

            onClicked: {
                if (text == "Save") {
                    if (titleField.text != "")
                        homepagecontroller.saveLink(webview.url, parserCombo.currentText, titleField.text)
                }
                else if (text == "Update") {
                    if (titleField.text != "")
                        homepagecontroller.updateLink(idAnnonce, webview.url, titleField.text)
                }
            }
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            Layout.preferredHeight: parent.height
            text: "Cancel"
            onClicked: stack.pop()
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
        }
    }

    Component.onCompleted: {
        if (currentUrl && idAnnonce > 0)
            webview.url = currentUrl
        if (currentTitle && idAnnonce > 0)
            titleField.text = currentTitle
    }
}

