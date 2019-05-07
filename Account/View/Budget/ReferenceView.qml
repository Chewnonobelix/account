import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import "../Style"
import "../Core"

Window {
    flags: Qt.Popup
    
    Column {
        id: col1
        Label {
            id: l_reference
            text: "Reference Date"
        }
        
        CalendarButton {
        }
    }
    
    Column {
        id: col2
        anchors.left: col1.right
        anchors.verticalCenter: col1.horizontalCenter
        Label {
            id: l_frequency
            text: "Frequency"
        }
        
        ComboBox {
            id: frequency
            model: ["day", "week"]
            
        }
    }
}
