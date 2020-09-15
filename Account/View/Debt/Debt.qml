import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"

Item {
    id: root

    GridLayout {
        anchors.fill: parent
        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02
    }
}
