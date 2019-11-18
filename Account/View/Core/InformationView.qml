import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S
import "../Frequency" as F

Item {
    id: info
    objectName: "info"
    S.AccountStyle {
        id: pageStyle
    }
    
    GridLayout {
        anchors.fill: parent
        EntryEdit {
            id: ee
            objectName: "entryEdit"
            visible: parent.visible
            Layout.maximumWidth: info.width
            Layout.maximumHeight: info.height * .15
            Layout.preferredWidth: info.width
            Layout.preferredHeight: info.height * .15
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignTop
        }
        rowSpacing: info.height * 0.02
        columnSpacing: info.width * 0.02
        
        F.Frequency {
            enabled: visible
            objectName: "frequency"
            Layout.maximumWidth: info.width *.49
            Layout.maximumHeight: info.height * .83
            Layout.preferredWidth: info.width * 49
            Layout.preferredHeight: info.height * .83
            Layout.row: 1
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.alignment: Qt.AlignLeft            
        }
    }
}
