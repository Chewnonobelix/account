import QtQuick 2.12
import QtQuick.Window 2.12

import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }

    ListModel {
        id: frequencyModel
        objectName: "frequencyModel"
    }

    ListView {
        id: frequencyList
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        
        model: frequencyModel
        
        delegate: Text {
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            fontSizeMode: Text.Fit
            
            text: name
        }
    }
    
    Frequency {
        id: reference
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: frequencyList.right
        anchors.leftMargin: 10
        anchors.right: parent.right        
    }
    
}
