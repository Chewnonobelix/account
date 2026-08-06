import QtQuick
import QtQuick.Layouts

import "../Style" as Style
import "../Components" as Comp

// A single "title: value" row used across forms and detail screens.
//
// Either declare the value widget explicitly as the DescLine's child:
//   DescLine { title: "Name"; OBTextInput { text: "Alice" } }
// or pick one of the built-in presets via contentType, optionally
// overriding a few of its properties through contentConfig:
//   DescLine { title: "Balance"; contentType: DescLine.ContentType.SpinBox
//              contentConfig: { "realTo": 1000, "realValue": 250 } }
//
// Built on RowLayout rather than manual anchoring: the title and value
// cells size and position themselves from their own preferred/implicit
// widths, so a long title can never overlap the value next to it, and the
// row's own implicitWidth/implicitHeight fall out of that automatically.
RowLayout {
    id: root

    enum ContentType {
        None,
        CheckBox,
        Label,
        SpinBox,
        TextInput,
        ComboBox
        // ButtonDate will be added once OBButtonDate exists.
    }

    property string title: ""
    property int contentType: DescLine.ContentType.None
    property var contentConfig: ({})
    default property alias content: contentItem.data

    // Floor for the title column; it still grows past this when the title
    // text itself is wider, so a long title is never clipped. Callers that
    // want several DescLines to line up (e.g. a form) can measure
    // titleNaturalWidth across their rows and feed the max back in here.
    property real titleWidth: Style.OBConstants.widthMedium
    readonly property alias titleNaturalWidth: titleLabel.implicitWidth

    readonly property Item value: root.contentType !== DescLine.ContentType.None ? contentLoader.item :
                                   (contentItem.children.length > 0 ? contentItem.children[0] : null)

    spacing: Style.OBConstants.horizontalSpacing

    state: {
        switch (root.contentType) {
        case DescLine.ContentType.CheckBox: return "checkBox"
        case DescLine.ContentType.Label: return "label"
        case DescLine.ContentType.SpinBox: return "spinBox"
        case DescLine.ContentType.TextInput: return "textInput"
        case DescLine.ContentType.ComboBox: return "comboBox"
        default: return "custom"
        }
    }

    states: [
        State {
            name: "custom"
            PropertyChanges { target: contentItem; visible: true }
            PropertyChanges { target: contentLoader; active: false; visible: false; sourceComponent: null }
        },
        State {
            name: "checkBox"
            PropertyChanges { target: contentItem; visible: false }
            PropertyChanges { target: contentLoader; active: true; visible: true; sourceComponent: checkBoxPreset }
        },
        State {
            name: "label"
            PropertyChanges { target: contentItem; visible: false }
            PropertyChanges { target: contentLoader; active: true; visible: true; sourceComponent: labelPreset }
        },
        State {
            name: "spinBox"
            PropertyChanges { target: contentItem; visible: false }
            PropertyChanges { target: contentLoader; active: true; visible: true; sourceComponent: spinBoxPreset }
        },
        State {
            name: "textInput"
            PropertyChanges { target: contentItem; visible: false }
            PropertyChanges { target: contentLoader; active: true; visible: true; sourceComponent: textInputPreset }
        },
        State {
            name: "comboBox"
            PropertyChanges { target: contentItem; visible: false }
            PropertyChanges { target: contentLoader; active: true; visible: true; sourceComponent: comboBoxPreset }
        }
    ]

    Comp.OBTitle {
        id: titleLabel
        text: root.title
        Layout.preferredWidth: Math.max(root.titleWidth, implicitWidth)
        Layout.alignment: Qt.AlignVCenter
    }

    // Explicit-content mode: holds whatever the caller declares as a child.
    Item {
        id: contentItem
        Layout.alignment: Qt.AlignVCenter
        implicitWidth: childrenRect.width
        implicitHeight: childrenRect.height
    }

    // Preset mode: instantiates a default-configured OB component for contentType.
    Loader {
        id: contentLoader
        Layout.alignment: Qt.AlignVCenter
        onLoaded: {
            for (var key in root.contentConfig) {
                item[key] = root.contentConfig[key]
            }
        }
    }

    Component { id: checkBoxPreset; Comp.OBCheckBox {} }
    Component { id: labelPreset; Comp.OBLabel {} }
    Component { id: spinBoxPreset; Comp.OBSpinBox {} }
    Component { id: textInputPreset; Comp.OBTextInput {} }
    Component { id: comboBoxPreset; Comp.OBComboBox {} }
}
