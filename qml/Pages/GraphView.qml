import QtQuick 2.0
import QtQuick.Controls 2.2
import QtCharts 2.0

Item {
    id: item

    property alias model: mapper.model
    property int idAnnonce: -1
    property string xTitle: "km"

    ChartView {
      id: chartview
      anchors.fill: parent
      antialiasing: true

      legend { visible: false }

      ScatterSeries {
          id: scatter

          axisX: ValueAxis { min:0; max: 1; labelFormat: "%d"; titleText: xTitle }
          axisY: ValueAxis { min:0; max: 1; labelFormat: "%d"; titleText: "euros" }

          VXYModelMapper {
              id: mapper
              rowCount: model.rowCount
              onRowCountChanged: { setXYColumn(); setMaxAxis(); }

              function setXYColumn() {
                  for (var i=0;i<model.columnCount;++i)
                  {
                      var titleColumn = model.headerData(i, Qt.Horizontal)
                      if (titleColumn == xTitle)
                          mapper.xColumn = i
                      if (titleColumn == "price")
                          mapper.yColumn = i
                  }
              }
          }

          onHovered: {
              if (state)
              {
                  popup.open()

                  var chartPoint = chartview.mapToPosition(point, scatter)
                  popup.text = "%1 km, %2 euros".arg(point.x).arg(point.y)

                  var obj = model.getAnnonce(point.x, point.y)
                  popup.title = obj.titre
                  popup.pictureSource = obj.img

                  popup.setPosition(chartPoint)
              }
              else
              {
                  popup.close()
              }
          }

          onClicked: {
              var obj = model.getAnnonce(point.x, point.y)
              if (obj && obj.ref)
                  stack.push("Annonce.qml", {id: obj.id, url: obj.ref, date: obj.date, created_date: obj.created_date})
          }

      }
    }

    Item {
        id: popup
        width: column.width+10
        height: column.height+10
        visible: false
        state: "close"

        property alias text: popupText.text
        property alias title: popupTitle.text
        property alias pictureSource: picture.source

        function setPosition(position) {
            if (position.x+column.width >= chartview.plotArea.x+chartview.plotArea.width)
                popup.x = chartview.plotArea.x+chartview.plotArea.width-column.width
            else
                popup.x = position.x

            popup.y = position.y
        }

        function open() {
            popup.state = "open"
        }

        function close() {
            popup.state = "close"
        }

        states: [
            State {
                name: "open"
                PropertyChanges { target: popup; visible: true; opacity: 1.0 }
            },
            State {
                name: "close"
                PropertyChanges { target: popup; visible: false; opacity: 0.0 }
            }
        ]

        transitions: [ Transition { PropertyAnimation { property: "opacity"; duration: 500 } } ]

        Rectangle {
            anchors.fill: parent
            border.color: "black"
            color: "#e6e6e6"
        }

        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Text {
                id: popupText
                width: contentWidth
                height: contentHeight
                clip: true
            }

            Text {
                id: popupTitle
                width: contentWidth
                height: contentHeight
                clip: true
            }

            Image {
                id: picture
                width: 100
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }
        }
    }

    function setMaxAxis() {

        var maxXvalue = -1
        var maxPrice = -1
        for (var i=0;i<mapper.model.rowCount;++i)
        {
            var km = mapper.model.get(i, xTitle)
            if (km > maxXvalue)
                maxXvalue = km

            var price = mapper.model.get(i, "price")
            if (price > maxPrice)
                maxPrice = price
        }

        if (maxXvalue > 0) {
            scatter.axisX.max = maxXvalue
            scatter.axisX.applyNiceNumbers()
        }
        if (maxPrice > 0) {
            scatter.axisY.max = maxPrice
            scatter.axisY.applyNiceNumbers()
        }
    }
}
