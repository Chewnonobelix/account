import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "../"
Rectangle {
    id: root


    GridLayout {
        anchors.fill: parent

        TableView {
            id: table
        }

        Button {
            id: add
        }

        Button{
            id: remove
        }

        ListView{
            id: recap
        }

        Button {
            id: close
        }

        Label {
            id: title
        }
    }
}
