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

    // Space offered by the containing ScrollView's column. root.width can't
    // just be bound to this directly: Column (like Row) reports its own
    // implicitWidth from children's actual *width*, not their implicitWidth,
    // so if root.width were pinned to availableWidth, an overflowing
    // contentRow underneath would overflow invisibly — the outer ScrollView
    // would never see it and couldn't scroll to it. Growing root.width to
    // match contentRow when it's wider keeps that overflow visible one level
    // up the chain, all the way to the ScrollView's contentWidth.
    property real availableWidth: 0
    width: Math.max(availableWidth, contentRow.implicitWidth)

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

    // Row, not Flow: most demo children anchor.verticalCenter to this
    // item, and Flow (unlike Row) claims full control of both axes to
    // support wrapping, so it rejects anchored children outright and
    // silently stops laying out anything. Overflow is instead handled by
    // the horizontal scrollbar on the containing ScrollView.
    Row {
        id: contentRow
        spacing: Style.OBConstants.horizontalSpacing
    }
}
