import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: root

    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size
    clip: true
    property bool valid: true

    ToolTip.delay: 500
    ToolTip.timeout: 1000
    ToolTip.visible: root.hovered

    hoverEnabled: true

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
