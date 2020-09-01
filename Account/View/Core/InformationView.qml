import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style"
import "../Frequency"

Item {
    id: root
    objectName: "info"
    
    property var model: ee.entry
    
    GridLayout {
        anchors.fill: parent
        EntryEdit {
            id: ee
            objectName: "entryEdit"

            Layout.maximumWidth: root.width
            Layout.maximumHeight: root.height * .15
            Layout.preferredWidth: root.width
            Layout.preferredHeight: root.height * .15
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignTop

            onAddNewCategory: _info.addNewCategory(cat)

            onS_catChanged: _info.categoryChange(cat)
            onS_titleChanged: _info.titleChange(title)
            onS_valueChanged: _info.valueChange(value)
            onS_supportChanged: _info.supportChange(supp)
        }
        rowSpacing: root.height * 0.02
        columnSpacing: root.width * 0.02
        
        Frequency {
            enabled: visible
            objectName: "frequency"
            Layout.maximumWidth: root.width *.49
            Layout.maximumHeight: root.height * .83
            Layout.preferredWidth: root.width * 49
            Layout.preferredHeight: root.height * .83
            Layout.row: 1
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.alignment: Qt.AlignLeft            
        }
    }
}
