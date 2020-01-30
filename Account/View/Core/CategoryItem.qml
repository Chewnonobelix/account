import QtQuick 2.13
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style"

ComboBox {
    
    id: root
    
    onAccepted: {
        s_addCategory(editText)
    }

    signal s_addCategory(string cat)
    signal s_currentTextChanged(string cat)
    property bool blocked: false
        
    function close() {
        s_currentTextChanged(currentText)
    }

    Connections {
        target: popup
        
        function onClosed() {
            s_currentTextChanged(currentText)
        }
    }

    MouseArea {
        anchors.fill: parent
        z: -1
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.NoButton
    }
    
    function setting(type) {
        var index = find(type)
        
        return index >= 0 ? index : model.lenth - 1
    }
    
    background: Rectangle {
        anchors.fill: parent
        gradient: AccountStyle.goldButton
    }
    
    contentItem: TextInput {
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font.family: AccountStyle.core.name
        font.pixelSize: AccountStyle.core.size
        
        text: currentText
        readOnly: !editable
        onAccepted: {
            s_addCategory(text)
        }
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            acceptedButtons: editable ? Qt.NoButton : Qt.LeftButton
            onClicked: popup.open()
            cursorShape: editable ? Qt.IBeamCursor : Qt.PointingHandCursor
        }
    }        
    
    
    editable: currentText === ""
    delegate: ItemDelegate {
        width: root.width
        contentItem: Rectangle  {
            gradient: AccountStyle.goldButton
            anchors.fill: parent
            
            AccountLabel {
                color: "black"
                text: modelData
                anchors.centerIn: parent
            }
            
            MouseArea {
                property string cTxt: modelData
                anchors.fill: parent
                cursorShape: index === (root.count - 1)  ? Qt.WhatsThisCursor : Qt.PointingHandCursor
                
                ToolTip.visible: (index === (root.count - 1)) && hovered
                ToolTip.text: qsTr("Add a new category")
                ToolTip.delay: 500
                
                onClicked: {
                    if(mouse.button === Qt.LeftButton) {
                        var index = root.find(cTxt)
                        root.currentIndex = index
                        root.popup.close()
                    }
                }
            }
        }
    }
}
