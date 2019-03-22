import QtQuick 2.12
import QtQuick.Controls 2.5

MenuItem {
    id: budgetItem
    AccountStyle {
        id: pageStyle
    }

    property double realValue
    property double to
    property string title
    contentItem: ProgressBar {
        id: budget
        //        anchors.top: title.bottom
        //    enabled: false
        from: 0
        to: budgetItem.to

        value: budgetItem.realValue
        //    height: 50
        background: Rectangle {
            implicitWidth: 100
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
                property int val: budgetItem.realValue / budget.to * 10000
                text: budgetItem.title + ": " + (val / 100.0) + "%"
                color: "black"
                anchors.left: parent.left
                anchors.leftMargin: 10
                id: title
                font.family: pageStyle.title.name
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
