import QtQuick 2.15
import QtQuick.Controls 2.15


MouseArea {
    StackView {
        id: root
        anchors.fill: parent

        Component {
            id: year

            Label {
                text: "year"
            }
        }

        Component {
            id: decade

            Label {
                text: "decade"
            }
        }

        initialItem: MultiCalendar {
            weekNumbersVisible: true
        }
    }

    onWheel: {
        if(wheel.angleDelta.y < 1) {
            if(root.depth === 2) {
                root.push(decade)
            }
            if(root.depth === 1) {
                root.push(year)
            }
        }

        if(wheel.angleDelta.y > 1) {
            if(root.depth > 1) {
                root.pop()
            }
        }
    }


    acceptedButtons: Qt.NoButton
}
