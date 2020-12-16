import QtQuick 2.12
import QtQuick.Controls 2.5
import "../Style"

MenuItem {
		id: root

    property double realValue
    property double to
    property string title

    contentItem: ProgressBar {
        id: budget

				anchors.fill: root
				from: 0
				to: root.to

				value: root.realValue

				background: Rectangle {
            color: "#e6e6e6"
            radius: 3
        }

        contentItem: Item {
						anchors.fill: budget
            Rectangle {
                width: budget.visualPosition * parent.width
								height: budget.height
                radius: 2
                color: budget.value < budget.to ? "green" : "red"
            }

            AccountLabel {
								property int val: root.realValue / budget.to * 10000
								text: root.title + ": " + (val / 100.0) + "%"
                color: "black"
                anchors.left: parent.left
								anchors.leftMargin: root.width * 0.01
                id: titleLab
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
