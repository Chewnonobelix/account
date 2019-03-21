import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    AccountStyle {
        id: pageStyle
    }

    ProgressBar {
        id: budget
        //        anchors.top: title.bottom
        //    enabled: false
        from: 0
        to: 100

        value: realValue
        property double realValue: 95
        //    height: 50
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 10
            color: "#e6e6e6"
            radius: 3
        }

        contentItem: Item {

            Rectangle {
                width: budget.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: budget.value < budget.to ? "green" : "red"
            }
            Label {
                fontSizeMode: Text.Fit
                text: (budget.realValue / budget.to * 100) + "%"
                color: "black"
                anchors.centerIn: parent
                anchors.leftMargin: 10
                id: title
                font.family: pageStyle.title.name
                font.pixelSize: 14
            }
        }
    }
}
