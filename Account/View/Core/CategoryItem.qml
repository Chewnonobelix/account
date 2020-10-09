import QtQuick 2.13
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style"

import Account.Model 1.0
import Account 1.0
/*
Account*/ComboBox {
    
    id: root
    
    onAccepted: {
        addCategory(editText)
    }

    property int type: Account.Outcome


    model: _categoryModel

    onTypeChanged: _categoryModel.currentType = type

    textRole: "display"
    valueRole: "type"

    editable: currentText === ""

    delegate: ItemDelegate {
        id: categoryDelegate

        width: root.width
        height: root.height

        text: model[root.textRole]

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: Qt.PointingHandCursor
        }
    }


//        property Rectangle back2: Rectangle{

//        }

//        contentItem: AccountTextInput {
//                background: root.editable ? back2 : back
//                readOnly: !root.editable
//                text: categoryDelegate.text
//                z:10
//                onAccepted:  {
//                    if(!readOnly) {
//                        root.addCategory(text)
//                    }
//                }
//        }
//    }

    signal addCategory(string cat)
    signal currentTextChanged(string cat)
    property bool blocked: false

//    function close() {
//        currentTextChanged(currentText)
//    }

//    Connections {
//        target: popup

//        function onClosed() {
//            currentTextChanged(currentText)
//        }
//    }
    
//    function setting(type) {
//        var index = find(type)

//        return index >= 0 ? index : model.lenth - 1
//    }

//    contentItem: AccountTextInput {
//        id: input
//        horizontalAlignment: Qt.AlignHCenter
//        verticalAlignment: Qt.AlignVCenter

//        ToolTip.text: readOnly ? qsTr("Select a transaction category") : qsTr("Add a new category")

//        property Item back2: AccountLabel{
//            text: input.text
//        }

//        background: readOnly ? back2 : back
//        text: currentText
//        readOnly: !editable
//        onAccepted: {
//            addCategory(text)
//        }
//        MouseArea {
//            anchors.fill: parent
//            propagateComposedEvents: true
//            acceptedButtons: editable ? Qt.NoButton : Qt.LeftButton
//            onClicked: popup.open()
//            cursorShape: editable ? Qt.IBeamCursor : Qt.PointingHandCursor
//        }
//    }
    
    
//    editable: currentText === ""
//    delegate: ItemDelegate {
//        width: root.width
//        contentItem: Rectangle  {
//            gradient: AccountStyle.goldButton
//            anchors.fill: parent

//            AccountLabel {
//                color: "black"
//                text: modelData
//                anchors.centerIn: parent
//            }

//            MouseArea {
//                property string cTxt: modelData
//                anchors.fill: parent
//                cursorShape: index === (root.count - 1)  ? Qt.WhatsThisCursor : Qt.PointingHandCursor

//                ToolTip.visible: (index === (root.count - 1)) && hovered
//                ToolTip.text: qsTr("Add a new category")
//                ToolTip.delay: 500

//                onClicked: {
//                    if(mouse.button === Qt.LeftButton) {
//                        var index = root.find(cTxt)
//                        root.currentIndex = index
//                        root.popup.close()
//                    }
//                }
//            }
//        }
//    }
}
