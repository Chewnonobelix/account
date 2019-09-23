import QtQuick 2.12
import QtQuick.Controls 2.5 as Control2
import QtQuick.Window 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13

import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }
    title: qsTr("Frequency manager")
    
    visible: false
    Rectangle {
        anchors.fill: parent        
        gradient: pageStyle.backgroundGradient
        ColumnLayout {
//            column: 1    
            id: colFreqList
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            width: parent.width * .3

            Component.onCompleted: console.log("layout", children.length)
            Label {
                text: qsTr("Frequency list")
                Layout.maximumHeight: parent.height * .10
                onHeightChanged: console.log(height)
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onHoveredChanged: console.log("Vsas chier")
                }
                
            }
            
        
            ListView {                
//                anchors.fill: parent
                id: frequencyList
                objectName: "frequencyList"
                model: []
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onHoveredChanged: console.log("Vsas chier2")
                }

                onCountChanged: console.log("Count", count)
                Layout.fillHeight: true
//                height: parent.height * .8
                width: parent.width
                section.property:  "name"
//                onHeightChanged: console.log( model, model.length, model[1].name )
                delegate: Label {
//                    color: "red"
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    
                    height: 40
                    width: frequencyList.width
//                    anchors.centerIn: parent    
                    text: modelData.name + "//" + modelData.id
//                    width: frequencyList.width
//                    onWidthChanged: console.log(width)
//                    onHeightChanged: console.log(height)
                    Component.onCompleted: console.log("Del ", text, index, modelData.name)
                }
            }
            
        }
        
//        Frequency {
//            id: reference
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.left: colFreqList.right
//            anchors.leftMargin: 10
//            anchors.right: parent.right        
//        }
    }    
}/**/
