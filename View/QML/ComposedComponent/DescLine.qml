import QtQuick

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
Item {
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

    readonly property Item value: root.contentType !== DescLine.ContentType.None ? contentLoader.item :
                                   (contentItem.children.length > 0 ? contentItem.children[0] : null)

    implicitWidth: titleLabel.width + Style.OBConstants.horizontalSpacing +
                   Math.max(contentItem.childrenRect.width, contentLoader.width)
    implicitHeight: Math.max(titleLabel.implicitHeight, contentItem.childrenRect.height, contentLoader.height)

    Comp.OBTitle {
        id: titleLabel
        text: root.title
        width: Style.OBConstants.widthMedium
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    // Explicit-content mode: holds whatever the caller declares as a child.
    Item {
        id: contentItem
        anchors.left: titleLabel.right
        anchors.leftMargin: Style.OBConstants.horizontalSpacing
        anchors.verticalCenter: parent.verticalCenter
        width: childrenRect.width
        height: childrenRect.height
        visible: root.contentType === DescLine.ContentType.None
    }

    // Preset mode: instantiates a default-configured OB component for contentType.
    Loader {
        id: contentLoader
        anchors.left: titleLabel.right
        anchors.leftMargin: Style.OBConstants.horizontalSpacing
        anchors.verticalCenter: parent.verticalCenter
        active: root.contentType !== DescLine.ContentType.None
        sourceComponent: {
            switch (root.contentType) {
            case DescLine.ContentType.CheckBox: return checkBoxPreset
            case DescLine.ContentType.Label: return labelPreset
            case DescLine.ContentType.SpinBox: return spinBoxPreset
            case DescLine.ContentType.TextInput: return textInputPreset
            case DescLine.ContentType.ComboBox: return comboBoxPreset
            default: return null
            }
        }
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
