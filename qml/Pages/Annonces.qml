import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import Models 1.0
import MyComponents 1.0
import myTypes 1.0

Page {
    id: page

    width: parent ? parent.width : 300
    height: parent ? parent.height : 100

    property int parserId: -1
    property string title: ""

    Connections {
        target: homepagecontroller
        onAnnoncesUpdated: annoncesModel.reload()
    }

    AnnonceModel {
        id: annoncesModel
        connectionName: "Annonces"
        tablename: "annonces"
        query: "SELECT *, (SELECT prix.prix from prix WHERE prix.annonceid=annonces.id ORDER BY prix.date DESC LIMIT 1) AS price, (SELECT param_value.value FROM optional_param JOIN param_name ON optional_param.param=param_name.id JOIN param_value ON optional_param.value=param_value.id WHERE param_name.name='version_modele' AND optional_param.annonceId=annonces.id) AS version_modele from annonces WHERE parserId=%1 ORDER BY price ASC".arg(parserId)

        Component.onCompleted: {
            annoncesModel.addColumnToFilter("is_active")
            annoncesModel.addColumnToFilter("id")
            annoncesModel.addColumnToFilter("urgent")
            annoncesModel.addColumnToFilter("pro")
            annoncesModel.addColumnToFilter("annee_modele")
            annoncesModel.addColumnToFilter("km")
            annoncesModel.addColumnToFilter("marque")
            annoncesModel.addColumnToFilter("modele")
            annoncesModel.addColumnToFilter("version_modele")
            annoncesModel.addColumnToFilter("carburant")
            annoncesModel.addColumnToFilter("bv")
            annoncesModel.addColumnToFilter("code_postal")
        }
    }

    SqlListModel {
        id: versionModel
        connectionName: "Annonces"
        tablename: "optional_param"
        query: "SELECT DISTINCT param_value.value AS version_modele FROM optional_param JOIN annonces ON optional_param.id=annonces.id JOIN param_name ON optional_param.param=param_name.id JOIN param_value ON optional_param.value=param_value.id WHERE param_name.name='version_modele' ORDER BY version_modele"
    }

    SqlListModel {
        id: optionalParamName
        connectionName: "Annonces"
        tablename: "param_name"
        query: "SELECT * FROM param_name"
    }

    SqlListModel {
        id: optionalParamValue
        connectionName: "Annonces"
        tablename: "param_value"
        query: "SELECT * FROM param_value"
    }

    SqlListModel {
        id: optionalParam
        connectionName: "Annonces"
        tablename: "optional_param"
        query: "SELECT * FROM optional_param"
    }

    SwipeView {
        id: view

        currentIndex: 0
        width: parent.width
        anchors { top: parent.top; bottom: indicator.top }

        Column {
            id: columnAnnonces
            spacing: 0

            Rectangle {
                id: header
                width: parent.width
                height: 40

                gradient: Gradient {
                    GradientStop { position: 0.0; color: theme.gradientStartColor }
                    GradientStop { position: 1.0; color: theme.gradientEndColor }
                }

                MyButton {
                    id: backButton
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    sourceComponent: Text { text: "< Annonces"; font.pixelSize: 12 }
                    onButtonClicked: stack.pop()
                }

                MyButton {
                    id: refreshButton
                    anchors.left: backButton.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    sourceComponent: Text { text: "Refresh"; font.pixelSize: 12 }
                    onButtonClicked: homepagecontroller.importAllResults(parserId)
                }

                Text {
                    id: textTitle
                    text: title
                    anchors.left: refreshButton.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }

                MyButton {
                    anchors.right: text.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    sourceComponent: Text { text: "Filter"; font.pixelSize: 12 }
                    onButtonClicked: filterDialog.visible = true
                }


                Text {
                    id: text
                    anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
                    width: contentWidth
                    height: contentHeight
                    font.pixelSize: 12
                    text: annoncesModel ? annoncesModel.rowCount + " annonces" : ""
                    color: "blue"
                }
            }

            ScrollView {
                id: listAnnonces
                width: parent.width
                height: parent.height-header.height

                ListView {
                    anchors.fill: parent
                    model: annoncesModel

                    delegate: AnnoncesDelegate { }
                    focus: true
                    highlightFollowsCurrentItem: false
                    clip: true

                    function selectAnnonce(id, url, date, created_date) {
                        stack.push("Annonce.qml", {id: id, url: url, date: date, created_date: created_date})
                    }
                }
            }
        }

        GraphView {
            idAnnonce: parserId
            model: annoncesModel
        }
    }

    PageIndicator {
        id: indicator

        count: view.count
        currentIndex: view.currentIndex

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Dialog {
        id: filterDialog
        title: "Filter annonces"

        contentItem: FilteringDialog {
            implicitWidth: 500
            implicitHeight: 300
            columnModel: annoncesModel.columnsToFilter
            columnDataModel: annoncesModel.columnDataModel

            onColumnSelected: {
                annoncesModel.setColumnDataModel(name)
            }

            onOk: {
                filterDialog.visible = false
                annoncesModel.updateFilter()
            }
        }
    }
}

