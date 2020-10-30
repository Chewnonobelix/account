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

        Connections {
            target: _info

            function onPageModel(list) {
                frequency.model = list
            }

            function onMaxPageChanged(max) {
                frequency.maxPage = max
            }

            function onCurrentPageChanged(index) {
                frequency.currentPage = index
            }

            function onCatList(list) {
                ee.catModel = list
            }

            function onView(e) {
                ee.entry = e
                root.visible = !e.isBlocked
            }
        }

        EntryEdit {
            id: ee

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

            onCatChanged: _info.categoryChange(cat)
            onTitleChanged: _info.titleChange(title)
            onValueChanged: _info.valueChange(value)
            onSupportChanged: _info.supportChange(supp)
        }
        rowSpacing: root.height * 0.02
        columnSpacing: root.width * 0.02
        
        Frequency {
            visible:ee.entry ?  ee.entry.isFrequency : null
            enabled: visible
            id: frequency
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
