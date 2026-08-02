import QtQuick
import QtQuick.Controls

import "../Style" as Style

Control {
    id: root

    implicitWidth: Style.OBConstants.widthBig
    implicitHeight: Style.OBConstants.heightBig

    leftPadding: Style.OBConstants.leftMargins
    rightPadding: Style.OBConstants.rightMargins
    topPadding: Style.OBConstants.topMargins
    bottomPadding: Style.OBConstants.bottomMargins

    property alias text: labelID.text
    property alias pixelSize: labelID.font.pixelSize
    property Gradient gradient: Style.OBStyle.header
    property color borderColor: Style.OBTheme.palette.outline
    property real borderWidth: Style.OBConstants.borderWidth

    background: Rectangle {
        gradient: root.gradient
        border.width: root.borderWidth
        border.color: root.borderColor
    }

    contentItem: OBLabel {
        id: labelID
        text: "Header"
        font.bold: true
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
    }
}
