import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: root

    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size
    property bool valid: true

    background: Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: root.valid ? "gold" : "red"
        AccountLabel {
            id: completionLabel
            anchors.fill: parent

            color: "grey"
            leftPadding: root.leftPadding
        }
    }

}
