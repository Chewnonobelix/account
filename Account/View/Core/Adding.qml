import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.13

import "../Style" as S

Popup {
    id: root
    
    function reset() {
        valueLabel.text = ""
        spinbox.value = 0
    }
    
    S.AccountStyle {
        id: pageStyle
    }
    
    CoreModel {
        id: models
    }
    
    signal accept()
    
    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: accountSelect.model.get(accountSelect.currentIndex).type
    property string v_date: dateLabel.text
    property string v_member: member.text
    property bool newAccount: false
    property bool common: false
    property var completionList: []
    property var openDate

    onClosed: { 
        valueLabel.background.border.color = "#bdbdbd"
    }
    
    onOpened: {        
        l_new.text = newAccount ? qsTr("Add new account"): qsTr("Add new entry")
    }
    
    onOpenDateChanged: dateLabel.extern(openDate)

    leftPadding: width * 0.02
    rightPadding: width * 0.02
    topPadding: height * 0.05
    bottomPadding: height * 0.05
    
    background: Rectangle {
        gradient: pageStyle.backgroundGradient
        border.color: "darkgoldenrod"
        id: backRect
    }
    
    
    contentItem: Rectangle {
        color: "transparent"
        
        GridLayout {
            id: grid
            
            rowSpacing: root.height * 0.02
            columnSpacing: root.width * 0.02
            
            anchors.fill: parent
            
            property double firstRow: root.height * 0.25
            property double secondRow: root.height * 0.38
            property double thirdRow: root.height * 0.23
            
            Label {
                id: labelAdd
                text: qsTr("Adding")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                
                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.15
                Layout.row: 0
                Layout.column: 0 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            CalendarButton {
                id: dateLabel
                
                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 0
                Layout.column: 1 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            Label{
                visible: common
                text: qsTr("Member")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                
                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.09
                Layout.row: 0
                Layout.column: 2
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            TextField {
                id: member
                objectName: "member"
                visible: common
                property bool isValid: (common && length > 0) || !common
                
                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.41
                Layout.row: 0
                Layout.column: 3
                Layout.rowSpan: 1 
                Layout.columnSpan: 3 
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                onTextEdited: {
                    var s = -1
                    for(var i in root.completionList) {
                        if(root.completionList[i].indexOf(text) === 0)
                            s = i
                    }
                    
                    completionLabel.text = s !== -1 ? root.completionList[s] : ""
                }
                
                background: Rectangle {
                    anchors.fill: parent
                    color: "white"
                    border.color: member.isValid ? "blue" : "red"
                    Label {
                        id: completionLabel
                        anchors.fill: parent
                        
                        color: "grey"
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        verticalAlignment: Qt.AlignVCenter
                        leftPadding: member.leftPadding                        
                    }
                }
            }       
            
            Label {
                id: l_new
                text: qsTr("Add new \n entry")
                wrapMode: Text.WordWrap
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size2
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                
                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.15
                Layout.row: 1
                Layout.column: 0 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            TextField {
                id: valueLabel
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                ToolTip.text: root.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: !valid
                ToolTip.delay: 100
                
                property bool valid: text.length !== 0
                
                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 1 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            DoubleSpinBox {
                id: spinbox
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                ToolTip.text: root.newAccount ? qsTr("Specify account opening value") : qsTr("Specify transaction's value")
                ToolTip.visible: hovered
                ToolTip.delay: 500
                
                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 2 
                Layout.rowSpan: 1 
                Layout.columnSpan: 2 
            }
            
            ComboBox {
                id: accountSelect
                objectName: "type"
                textRole: "name"
                model: models.typeModel
                
                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 4 
                Layout.rowSpan: 1 
                Layout.columnSpan: 2 
                
                enabled: !root.newAccount
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                ToolTip.text: qsTr("Specify income or outcome")
                ToolTip.visible: hovered
                ToolTip.delay: 500
                
                background:  Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                }
                
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }
                delegate: ItemDelegate {
                    width: accountSelect.width
                    
                    contentItem: Rectangle {
                        gradient: pageStyle.goldButton
                        anchors.fill: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                        Label {
                            color: "black"
                            text: name
                            anchors.centerIn: parent
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                        }
                    }
                    
                    highlighted: accountSelect.highlightedIndex === index
                }
            }
            
            Button {
                id: b_save
                text: qsTr("Save")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                ToolTip.text:  root.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: ttVisible
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    id: saveRect
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
                
                property bool ttVisible: false
                onClicked: {
                    
                    if(valueLabel.valid && member.isValid) {
                        root.accept()
                        reset()
                        close()
                    }
                    else {
                        ttVisible = true
                        valueLabel.background.border.color = "red"
                    }
                }
                
                Layout.preferredHeight: parent.thirdRow
                Layout.preferredWidth: root.width * 0.11
                Layout.row: 2
                Layout.column: 4 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
            
            Button {
                id:b_cancel
                text: qsTr("Cancel")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    id: cancelRect
                }
                
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }
                
                onClicked: {
                    reset()
                    close()
                }
                
                Layout.preferredHeight: parent.thirdRow
                Layout.preferredWidth: root.width * 0.10
                Layout.row: 2
                Layout.column: 5 
                Layout.rowSpan: 1 
                Layout.columnSpan: 1 
            }
        }
    }
}

