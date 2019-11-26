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
    
    property var cModel
    
    onCModelChanged: {
        console.log(cModel, cModel.title, cModel.begin, Qt.formatDate(cModel.begin, "dd-MM-yyyy"))
//        title.text = Qt.binding(function() {return model ? Qt.formatDate(model.begin, "dd-MM-yyyy") + ": " + model.title : ""})
        table.membersList = Qt.binding(function() {return cModel ? cModel.members : []})
    }
    
    AccountStyle {
        id: pageStyle
    }    
    MouseArea {
        anchors.fill: parent
        onClicked: console.log("top", cModel)
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
            
            property var membersList
            
            Repeater {
                model: table.membersList
                ListView {
                    id: listComponent
                    height: table.height
                    width: table.width * .20
                    
                    model: root.model.entries(modelData)
                    header: Rectangle {
                        height: listComponent.height * .05
                        width: listComponent.width
                    }
                    
                    footer: Rectangle {
                        height: listComponent.height * .05
                        width: listComponent.width
                    }
                    
                    delegate: Rectangle {
                        height: listComponent.height * .10
                        width: listComponent.width
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
            
//            onModelChanged: console.log(model)
            text: root.model ? /*Qt.formatDate(model.begin, "dd-MM-yyyy") + ": " + */root.model.title : ""
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log(parent.text, root.model)
                }
            }

            background: Rectangle{
                color: "transparent"
                border.color: "red"
            }
        }
    }
}
