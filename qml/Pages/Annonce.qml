import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtWebView 1.0
import MyComponents 1.0
import Models 1.0
import QtCharts 2.0
import QtGraphicalEffects 1.0

Item {
    width: 600
    height: 200

    property int id: -1
    property date date
    property date created_date
    property alias url: webview.url

    property string dateFormat: "ddd d MMMM yyyy"

    onIdChanged: {
        priceModel.query = "SELECT * from prix WHERE annonceid=%1".arg(id)

        priceSerie.clear()

        for (var i=0;i<priceModel.rowCount;++i) {
            priceSerie.append(priceModel.getDate(i), priceModel.get(i, "prix"))
        }

        priceSerie.append(priceModel.getCurrentDate(), priceModel.getCurrentPrice())
    }

    PriceModel {
        id: priceModel
        connectionName: "Annonces"
        tablename: "prix"
        query: "SELECT * from prix WHERE annonceid=%1".arg(id)
    }

    Column {
        anchors { fill: parent }
        spacing: 0

        Row {
            id: row
            spacing: 0
            height: 200
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter

            MyButton {
                id: backButton
                anchors { top: parent.top; topMargin: 10 }
                sourceComponent: Text { text: "< Back"; font.pixelSize: 12 }
                onButtonClicked: back()
            }

            Column
            {
                id: columnDate
                spacing: 20
                width: textDate.width
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: textDate
                    width: contentWidth
                    height: contentHeight
                    clip: true

                    text: "updated: " + date.toLocaleDateString(Qt.locale(), dateFormat)
                }

                Text {
                    width: contentWidth
                    height: contentHeight
                    clip: true

                    text: "created: " + created_date.toLocaleDateString(Qt.locale(), dateFormat)
                }
            }

            ChartView {
              id: chartview
              width: parent.width - backButton.width - columnDate.width
              height: parent.height
              antialiasing: true

              legend { visible: false }

              AreaSeries {
                  id: serie
                  axisX: DateTimeAxis { format: "dd MMM yyyy" }
                  upperSeries: LineSeries { id: priceSerie }
                  color: "lightsteelblue"
              }
            }
        }


        ScrollView {
            id: scrollwebview

            width: parent.width
            height: parent.height - row.height

            WebView {
                id: webview
                width: scrollwebview.width
                height: scrollwebview.height
            }
        }
    }
}
