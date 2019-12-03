import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "../Style"
import "../Core"

Rectangle {
    id: root
    
    signal s_add(int id)
    signal s_remove(int id)
    signal s_close(int id)
    
    property var model
    
    onModelChanged: close.checked = model ? model.isClose : false
    
    Component.onCompleted: {
        title.text = Qt.binding(function() {return model ? Qt.formatDate(model.begin, "dd-MM-yyyy") + ": " + model.title : ""})
        table.membersList = Qt.binding(function() {return model ? model.members : []})
        recap.model = Qt.binding(function() {if(model) {
                var ret = []
                for(var it in model.members) {
                    ret[ret.length] = ({"id": model.members[it], "total": model.totalForMember(model.members[it])})
                }
                return ret
            }
            return []})
        closing.model = Qt.binding(function() {return model ? model.closing() : []})
    }
    
    AccountStyle {
        id: pageStyle
    }    
    
    color: "transparent"
    
    Adding {
        common: true
        id: addingid
        objectName: "commonAdding"
        anchors.centerIn: parent
    }
    
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
                            gradient: pageStyle.goldHeader
                            Label {
                                anchors.fill: parent
                                text: modelData
                                font.pixelSize: pageStyle.title.size
                                font.family: pageStyle.title.name
                                fontSizeMode: Text.fit
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }
                        }
                        
                        footer: Rectangle {
                            height: listComponent.height * .03
                            width: listComponent.width
                            gradient: pageStyle.goldHeader
                            Label {
                                anchors.fill: parent
                                text: root.model.totalForMember(modelData).value
                                font.pixelSize: pageStyle.title.size
                                font.family: pageStyle.title.name
                                fontSizeMode: Text.fit
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }                     
                        }
                        
                        delegate: Rectangle {
                            height: listComponent.height * .07
                            width: listComponent.width
                            gradient: pageStyle.unselectView
                            Label {
                                anchors.fill: parent
                                text: modelData.value
                                font.pixelSize: pageStyle.title.size
                                font.family: pageStyle.title.name
                                fontSizeMode: Text.fit
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }                     
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
            
            onClicked: addingid.open()
            
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size      
            
            background: Rectangle {
                MouseArea{
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                }
                gradient: pageStyle.goldButton
            }            
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
            
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size            
            
            background: Rectangle {
                MouseArea{
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                }
                gradient: pageStyle.goldButton
            }
        }
        
        Column {
            Layout.preferredHeight: root.height * 0.93
            Layout.preferredWidth: root.width * .20
            Layout.row: 0
            Layout.column: 3 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            spacing: height * .02
            
            ListView{
                id: recap
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                width: parent.width
                height: parent.height * 0.49
                
                delegate: Rectangle {
                    gradient: pageStyle.unselectView
                    width: recap.width
                    height: recap.height * .07
                    
                    Label {
                        anchors.fill: parent
                        text: modelData.id + ": " + modelData.total.value + "€"
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }
                    
                }
            }
            ListView{
                id: closing
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                width: parent.width
                height: parent.height * 0.49
                
                delegate: Rectangle {
                    gradient: pageStyle.unselectView
                    width: recap.width
                    height: recap.height * .07
                    
                    Label {
                        anchors.fill: parent
                        text: modelData.from + qsTr(" to ") + modelData.to + ": " + modelData.value + "€"
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }
                    
                }
                
            }
        }
        CheckBox {
            id: close
            objectName: "close"
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .20
            Layout.row: 1
            Layout.column: 3 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            Layout.alignment: Qt.AlignRight
            text: qsTr("Close")
            
            checked: model ? model.isClose : false
            
            signal s_checked(bool check)
            onCheckedChanged: { 
                s_checked(checked)
            }
            
            background: MouseArea {
                acceptedButtons: Qt.NoButton
                cursorShape: Qt.PointingHandCursor
            }
        }
        
        Label {
            id: title
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * .56
            Layout.row: 1
            Layout.column: 2 
            Layout.rowSpan: 1
            Layout.columnSpan: 1 
            
            font.family: pageStyle.title.name
            font.pixelSize: pageStyle.title.size
            fontSizeMode: Text.Fit
            
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            
            background: Rectangle{
                color: "transparent"
            }
        }
    }
}
