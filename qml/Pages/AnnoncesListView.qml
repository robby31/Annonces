import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import Models 1.0
import MyComponents 1.0

Item {
    id: item

    function parserUpdated() {
        annoncesModel.reload()
    }

    SqlListModel {
        id: annoncesModel
        connectionName: "Annonces"
        tablename: "parser"
        query: "SELECT * from parser ORDER BY title"
    }

    Component {
        id: listAnnonces

        Item {
            Rectangle {
                id: header

                anchors { top: parent.top; left: parent.left }
                width: parent.width
                height: 40

                gradient: Gradient {
                    GradientStop { position: 0.0; color: theme.gradientStartColor }
                    GradientStop { position: 1.0; color: theme.gradientEndColor }
                }

                MyButton {
                    anchors { left: parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter }
                    sourceComponent: Text { text: "+ New" }
                    onButtonClicked: stack.push(navigator)
                }

                Text {
                    id: text
                    anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
                    width: contentWidth
                    height: contentHeight
                    text: annoncesModel ? annoncesModel.rowCount + " annonces" : ""
                    color: "blue"
                }
            }

            ScrollView {
                anchors { top: header.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right }

                ListView {
                    id: listview

                    model: annoncesModel
                    delegate: SavedAnnoncesDelegate { }
                    focus: true
                    highlightFollowsCurrentItem: false
                    clip: true
                }
            }
        }
    }

    Component {
        id: navigator

        Navigator {
            anchors.fill: parent
            onClose: stack.pop()
        }
    }


    StackView {
        id: stack

        anchors.fill: parent
        initialItem: listAnnonces
    }
}
