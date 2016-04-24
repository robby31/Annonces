import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import Models 1.0
import MyComponents 1.0

Page {
    width: 300
    height: 200

    actions: pageActions

    onActionClicked: {
        if (name == "Quit")
            Qt.quit()
        else if (name == "Refresh")
            annoncesModel.reload()
        else if (name == "Filter")
            filterDialog.visible = true
    }

    ListModel {
        id: pageActions

        ListElement {
            name: "Refresh"
            description: "reload annonces"
            icon: "qrc:///images/view-refresh.png"
        }

        ListElement {
            name: "Filter"
            description: "filter annonces"
            icon: "qrc:///images/view-refresh.png"
        }

        ListElement {
            name: "Quit"
            description: "exit application"
            icon: "qrc:///images/exit.png"
        }
    }

    SqlListModel {
        id: annoncesModel
        tablename: "annonces"
        query: "SELECT *, (SELECT prix.prix from prix WHERE prix.annonceid=annonces.id ORDER BY prix.date DESC LIMIT 1) AS price from annonces ORDER BY annonces.created_date DESC"

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

    ScrollView {
        id: listAnnonces
        anchors.fill: parent
        visible: !annonce.visible

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

    function back() {
        annonce.source = ""
    }

    Loader {
        id: annonce
        anchors.fill: parent
        visible: item != null
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

