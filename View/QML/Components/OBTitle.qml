import QtQuick

import "../Style" as Style

Item {
    id: root

    implicitWidth: label.implicitWidth
    implicitHeight: label.implicitHeight

    property string text: "Title"
    readonly property alias displayText: label.text
    property alias textState: label.textState
    property alias horizontalAlignment: label.horizontalAlignment
    property alias verticalAlignment: label.verticalAlignment

    OBLabel {
        id: label
        anchors.fill: parent
        text: root.text.toUpperCase()
        font.bold: true
        font.pixelSize: 18
        // Callers (e.g. DescLine) pin this to a fixed column width; without
        // eliding, text longer than that width paints past it and
        // overlaps whatever sits to the right.
        elide: Text.ElideRight
    }
}
