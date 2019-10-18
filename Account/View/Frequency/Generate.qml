import QtQuick 2.13
import QtQuick.Controls 2.5

import "../Style" as S
import "../Core" as C

Popup {
    property int freqId: -1
    property int freqGroup: -1
    
    signal s_generate(date fBegin, date fEnd)
    
    Frequency {
        id: freq
        enabled: false
    }

    Column {
        anchors.top: freq.bottom
        anchors.left: freq.left
        width: freq.width /2 - 5
        Label {
            text: qsTr("From")
        }

        C.CalendarButton {
            id: from
        }
    }
    
    Column {   
        anchors.top: freq.bottom
        anchors.right: freq.right
        anchors.leftMargin: 10
        width: freq.width /2 - 5
        
        Label {
            text:qsTr("To")
        }

        C.CalendarButton {
            id: to
        }
    }
}
