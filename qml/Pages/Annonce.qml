import QtQuick 2.3
import QtQuick.Controls 1.2
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

        xaxis.min = priceModel.minDate()
        xaxis.max = priceModel.maxDate()

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

    ColumnLayout {
        anchors { fill: parent }
        spacing: 0

        Row {
            spacing: 10
            height: 100
            width: parent.width

            MyButton {
                id: backButton
                sourceComponent: Text { text: "< Back"; font.pointSize: 10 }
                onButtonClicked: back()
            }

            Column
            {
                spacing: 5
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
              width: 300
              height: parent.height
              antialiasing: true

              legend.visible: false

              AreaSeries {
                  id: serie
                  axisX: DateTimeAxis { id: xaxis; tickCount: 3; format: "dd MMM yyyy" }
                  upperSeries: LineSeries { id: priceSerie }
                  color: "lightsteelblue"
              }
            }
        }


        ScrollView {
            id: scrollwebview
            Layout.fillWidth: true
            Layout.fillHeight: true

            WebView {
                id: webview
                width: scrollwebview.width
                height: scrollwebview.height
            }
        }
    }
}
