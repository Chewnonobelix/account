import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "../Style"

Rectangle {
    id: root
    
    signal s_add()
    signal s_remove(int id)
    signal s_close(int id)
    
    property var model
    
    Component.onCompleted: {
        title.text = Qt.binding(function() {return model ? Qt.formatDate(model.begin, "dd-MM-yyyy") + ": " + model.title : ""})
        table.membersList = Qt.binding(function() {return model ? model.members : []})
    }
    
    AccountStyle {
        id: pageStyle
    }    
    
    color: "transparent"
    GridLayout {
        anchors.fill: parent
        rowSpacing: height * 0.02
        columnSpacing:  width * 0.02
        
        ScrollView {
            id: table
            Layout.preferredHeight: root.height * 0.93
            Layout.preferredWidth: root.width * .78
            Layout.row: 0
            Layout.column: 0 
            Layout.rowSpan: 1
            Layout.columnSpan: 3
            clip: true
            property var membersList
            Row {
                anchors.fill: parent
                clip: true
                
                Repeater {
                    model: table.membersList
                    ListView {
                        id: listComponent
                        height: table.height
                        width: table.width * .20
                        clip: true
                        model: root.model.entries(modelData)
                        
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: "black"
                        }

                        header: Rectangle {
                            height: listComponent.height * .05
                            width: listComponent.width
                            color: "blue"
                        }
                        
                        footer: Rectangle {
                            height: listComponent.height * .05
                            width: listComponent.width
                            color: "red"
                        }
                        
                        delegate: Rectangle {
                            height: listComponent.height * .10
                            width: listComponent.width
                            color: "pink"
                        }
                    }
                }
            }
        }
        
        Button {
            id: add
            objectName: "add"
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .09
            Layout.row: 1
            Layout.column: 0 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            text: qsTr("Add")
        }
        
        Button{
            id: remove
            objectName: "remove"
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .09
            Layout.row: 1
            Layout.column: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            text: qsTr("Remove")
        }
        
        ListView{
            id: recap
            Layout.preferredHeight: root.height * 0.93
            Layout.preferredWidth: root.width * .20
            Layout.row: 0
            Layout.column: 3 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            Rectangle {
                anchors.fill: parent
                border.color: "gold"
                color: "transparent"
            }
        }
        
        Button {
            id: close
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .20
            Layout.row: 1
            Layout.column: 3 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            text: qsTr("Close")
        }
        
        Label {
            id: title
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .56
            Layout.row: 1
            Layout.column: 2 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            fontSizeMode: Text.Fit
            
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
                        
            background: Rectangle{
                color: "transparent"
                border.color: "red"
            }
        }
    }
}
