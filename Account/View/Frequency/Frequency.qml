import QtQuick 2.12
import QtQuick.Controls 2.5

import "../Core" as C
import "../Style" as S

Item {
    
    S.AccountStyle {
        id: pageStyle
    }
    
    FrequencyReference {
        id: freqReference
        objectName: "reference"
        width: parent.width
        height: parent.height * .20
    }
    
    ListModel {
        id: pastModel
        objectName: "pastModel"
    }
    
    function add(element) {
        pastModel.append(element)
    }
    
    ListView {
        id: pastView
        anchors.top: freqReference.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        
        model: pastModel
        
        delegate: Text {
            text: dateRef
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            fontSizeMode: Text.Fit
        }
    }
}
