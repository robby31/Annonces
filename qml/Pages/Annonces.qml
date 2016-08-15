import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import Models 1.0
import MyComponents 1.0

Page {
    width: 300
    height: 200

    property int parserId: -1

    SqlListModel {
        id: annoncesModel
        connectionName: "Annonces"
        tablename: "annonces"
        query: "SELECT *, (SELECT prix.prix from prix WHERE prix.annonceid=annonces.id ORDER BY prix.date DESC LIMIT 1) AS price from annonces WHERE parserId=%1 ORDER BY annonces.created_date DESC".arg(parserId)

        Component.onCompleted: {
            annoncesModel.addColumnToFilter("is_active")
            annoncesModel.addColumnToFilter("id")
            annoncesModel.addColumnToFilter("urgent")
            annoncesModel.addColumnToFilter("pro")
            annoncesModel.addColumnToFilter("annee_modele")
            annoncesModel.addColumnToFilter("km")
            annoncesModel.addColumnToFilter("marque")
            annoncesModel.addColumnToFilter("modele")
            annoncesModel.addColumnToFilter("carburant")
            annoncesModel.addColumnToFilter("bv")
        }
    }

    Loader {
        id: annonce
        anchors.fill: parent
        visible: item != null
    }

    ColumnLayout {
        id: columnAnnonces
        anchors.fill: parent
        visible: !annonce.visible
        spacing: 0

        Rectangle {
            id: header
            height: 40
            Layout.fillWidth: true

            gradient: Gradient {
                GradientStop { position: 0.0; color: theme.gradientStartColor }
                GradientStop { position: 1.0; color: theme.gradientEndColor }
            }

            MyButton {
                id: backButton
                anchors.left: parent.left
                anchors.leftMargin: 10
                sourceComponent: Text { text: "< Back" }
                onButtonClicked: backToSavedAnnonces()
            }

            MyButton {
                id: refreshButton
                anchors.left: backButton.right
                anchors.leftMargin: 10
                sourceComponent: Text { text: "Refresh" }
                onButtonClicked: importResults(parserId)
            }

            MyButton {
                anchors.right: text.left
                anchors.rightMargin: 10
                sourceComponent: Text { text: "Filter" }
                onButtonClicked: filterDialog.visible = true
            }


            Text {
                id: text
                anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
                width: contentWidth
                height: contentHeight
                text: annoncesModel ? annoncesModel.rowCount + " annonces." : ""
                color: "blue"
            }
        }

        ScrollView {
            id: listAnnonces
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                anchors.fill: parent
                model: annoncesModel

                delegate: AnnoncesDelegate { }
                focus: true
                highlightFollowsCurrentItem: false
                clip: true

                function selectAnnonce(id, url, date, created_date) {
                    annonce.setSource("Annonce.qml", {id: id, url: url, date: date, created_date: created_date})
                }
            }
        }
    }


    function back() {
        annonce.source = ""
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

