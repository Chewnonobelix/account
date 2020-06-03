import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Functionnal"
import "../Style"
import Account 1.0

GridLayout {
    id: root
    
    rowSpacing: height * 0.02
    columnSpacing: width * 0.02
    
    property date currentDate: new Date()
    property var currentGran: Account.Month
    signal s_increment(int i)
    signal s_zoom(int dir)
    property bool okNext: true
    property bool okPrev: true
    
    AccountButton {
        id: prev
        text: qsTr("<<")
        
        Layout.preferredHeight: root.height * 0.05
        Layout.preferredWidth: root.width * 0.2
        Layout.columnSpan: 1
        Layout.column: 0            
        Layout.row: 1 
        
        enabled: root.okPrev
        
        onClicked: {
            root.s_increment(-1)
        }
    }
    
    AccountLabel {
        text: Qt.formatDate(root.currentDate, root.currentGran === Account.Month ? "MMMM-yyyy" : root.currentGran === Account.Year ? "yyyy" : "")
        Layout.preferredHeight: root.height * 0.05
        Layout.preferredWidth: root.width * 0.4
        Layout.columnSpan: 1
        Layout.column: 1            
        Layout.row: 1 
        font.family: AccountStyle.title.name
        font.pixelSize: AccountStyle.title.size
        
    }
    
    AccountButton {
        id: next
        enabled: root.okNext
        
        text: qsTr(">>")
        
        Layout.preferredHeight: root.height * 0.05
        Layout.preferredWidth: root.width * 0.1
        Layout.alignment: Qt.AlignLeft
        Layout.columnSpan: 1
        Layout.column: 2            
        Layout.row: 1 
        
        
        onClicked: {
            root.s_increment(1)
        }
    }
    
    PageView {
        id: graphSwipe
        Layout.preferredHeight: root.height * 0.93
        Layout.fillWidth: true
        Layout.columnSpan: 3
        Layout.column: 0
        Layout.row: 0
        Page2Form {
            id: graph
            objectName: "timeGraph"
//                anchors.fill: parent
            
        }

        PieCategory {
            id: pie
            objectName: "pieCategory"
//                anchors.fill: parent
            
        }

    }
    
}
