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

    // Axis the gradient flows along: Gradient.Vertical (default) or Gradient.Horizontal.
    property int orientation: Gradient.Vertical
    // Flips the flow direction on that axis (e.g. top-to-bottom becomes bottom-to-top).
    property bool reversed: false

    background: Rectangle {
        // A 180° rotation mirrors the gradient's start/end without touching its
        // stops: each band is uniform along the perpendicular axis, so rotating
        // the whole rectangle looks identical to reversing the stop order.
        rotation: root.reversed ? 180 : 0

        gradient: Gradient {
            orientation: root.orientation
            stops: root.gradient.stops
        }
    }

    contentItem: OBLabel {
        id: labelID
        text: "Header"
        font.bold: true
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
