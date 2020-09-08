import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "../Style"
import "../Core"
import "../Functionnal"

Popup {        
    id: root
    
    padding: 0
    property string budgetName

    onOpened: {
        cButton.extern(new Date())
    }


    contentItem: Rectangle {
        id: rectWindow
        anchors.fill: parent
        gradient: AccountStyle.backgroundGradient
        
        border.color: "gold"
        
        Connections {
            target: _budget

            function onCatChanged(cat: string) {
                budgetName = cat
                root.open()
            }

            function onClose() {
                root.close()
            }
        }

        GridLayout {
            id: gridId
            anchors.fill: parent
            anchors.leftMargin: root.width * 0.02
            anchors.rightMargin: root.width * 0.02
            anchors.topMargin: root.height * 0.02
            anchors.bottomMargin: root.height * 0.02
            
            columnSpacing: root.width * 0.02
            rowSpacing: root.height * 0.02
            
            AccountLabel {
                id: calendarLabel
                text: qsTr("From")

                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 0
                Layout.alignment: Qt.AlignLeft
            }
            
            CalendarButton {
                id: cButton
                objectName: "cButton"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 0
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight       
                
                ToolTip.text: qsTr("Select begin date")
            }
            
            AccountLabel {
                text: qsTr("Target")

                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 1
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft                
            }
            
            AccountSpinbox {
                id: targetValue
                objectName: "targetValue"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight

                ToolTip.text: qsTr("Specify target value")                
            }
            
            AccountLabel {
                text: qsTr("Every")
                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 2
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft                
            }
            
            AccountComboBox {
                id: freqCombo
                objectName: "freqCombo"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 2
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight                
                
                model: CoreModel.freqModel
                textRole: "name"
                
                property int currentRole: 0
                
                onCurrentIndexChanged: currentRole = CoreModel.freqModel.get(currentIndex).role
                
                ToolTip.text: qsTr("Select frequency")
            }
            
            AccountButton {
                text: "ok"
                id: okButton
                objectName: "okButton"
                
                Layout.preferredHeight: root.height * 0.15
                Layout.preferredWidth:  root.width * 0.23
                Layout.column: 1
                Layout.row: 3
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft

                onClicked: _budget.editReference()
            }
            
            AccountButton {
                text: "Cancel"
                onClicked: root.close()
                
                Layout.preferredHeight: root.height * 0.15
                Layout.preferredWidth:  root.width * 0.22
                Layout.column: 2
                Layout.row: 3
                Layout.columnSpan:  1
                Layout.alignment: Qt.AlignRight                
            }
            
        }
    }
}

