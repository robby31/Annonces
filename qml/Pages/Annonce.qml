import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtWebView 1.0
import MyComponents 1.0
import Models 1.0

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

        var ChartBarData = {
            labels: [],
            datasets: [{
                    fillColor: "rgba(151,187,205,0.5)",
                    strokeColor: "rgba(151,187,205,1)",
                    data: []
                }]
        }

        ChartBarData.labels.push("")
        ChartBarData.datasets[0].data.push(0)

        for (var i=0;i<priceModel.rowCount;++i) {
            var str_date = Date.fromLocaleString(Qt.locale(), priceModel.get(i, "date"), "yyyy-MM-ddThh:mm:ss.zzz").toLocaleDateString(Qt.locale(), "d MMM")
            ChartBarData.labels.push(str_date)

            ChartBarData.datasets[0].data.push(priceModel.get(i, "prix"))
        }

        chart_bar.chartData = ChartBarData
    }

    SqlListModel {
        id: priceModel
        query: "SELECT * from prix WHERE annonceid=%1".arg(id)
    }

    ColumnLayout {
        anchors { fill: parent; topMargin: 10 }
        spacing: 10

        Row{
            spacing: 10
            height: 100

            Rectangle {
                id: backButton
                width: 60
                height: 30
                color: backButtonMouseArea.pressed? theme.highlightSelectColor : theme.highlightColor
                radius: 10
                border.color: theme.separatorColor
                clip: true
                opacity: 1

                Text {
                    anchors.centerIn: parent
                    font.pixelSize:14
                    text: "< Back"
                }

                MouseArea {
                    id: backButtonMouseArea
                    anchors.fill: parent
                    onClicked: back()
                }
            }

            Column
            {
                spacing: 5
                width: textDate.width
                height: parent.height

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

            Chart {
              id: chart_bar
              width: 300
              height: parent.height
              chartAnimated: true
              chartAnimationEasing: Easing.OutBounce
              chartAnimationDuration: 2000
              chartType: Charts.ChartType.BAR
              chartOptions: ({ barShowStroke: true, scaleLabel: "<%=value%> €", barValueSpacing: 30 })
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
