import QtQuick

import "../../View/QML/Style" as Style
import "../../View/QML/Components" as Comp

// One row in the gallery: a title, an "enabled" toggle wired to `target`,
// and a horizontal strip of live component instances declared as children.
Column {
    id: root

    property string title: ""
    property Item target: null
    default property alias content: contentRow.data

    spacing: Style.OBConstants.verticalSpacing / 2

    Row {
        width: parent.width
        height: Style.OBConstants.heightLittle
        spacing: Style.OBConstants.horizontalSpacing

        Comp.OBLabel {
            text: root.title
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBCheckBox {
            id: enabledToggle
            text: "enabled"
            checked: true
            visible: root.target !== null
            anchors.verticalCenter: parent.verticalCenter
            onCheckedChanged: if (root.target) root.target.enabled = checked
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        color: Style.OBTheme.palette.outline
        opacity: 0.3
    }

    Row {
        id: contentRow
        spacing: Style.OBConstants.horizontalSpacing
    }
}
