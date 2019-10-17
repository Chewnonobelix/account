import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S

Item {
    id: info
    property bool opening: false
    S.AccountStyle {
        id: pageStyle
    }
    property int maximum: Screen.width * .55 - 10
    
    //    width: maximum
    property var entry
    //property var infoModel: entry.info
    
    property var catModel: []
    
    onCatModelChanged: category.model = catModel
    signal s_titleChanged(string title)
    signal s_estimatedChanged(bool title)
    signal s_valueChanged(real value)
    signal s_catChanged(string cat)
    
    onEnabledChanged: {
        titleLabel.enabled = true
        categoryLabel.enabled = true
        valueLabel.enabled = true
    }
    
    onEntryChanged: {
        category.setting(entry.info.category)
    }
    
    function reloadCat() {
        category.setting(entry.info.category)
    }

    GridLayout {
        rows: 3
        columns: 3
        anchors.fill: parent
        rowSpacing: 0
        Label {
            id: titleLabel
            text: qsTr("Title")
            font.pixelSize: pageStyle.title.size
            font.family: pageStyle.title.name
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            background: Rectangle {
                gradient: pageStyle.goldHeader
                border.color: "darkgoldenrod"
            }
            
            Layout.preferredWidth: parent.width * 0.33
            Layout.preferredHeight: parent.height * 0.2
            Layout.row: 0
            Layout.column: 0
        }
        
        Label {
            id: valueLabel
            text: qsTr("Value")
            font.pixelSize: pageStyle.title.size
            font.family: pageStyle.title.name
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            background: Rectangle {
                gradient: pageStyle.goldHeader
                border.color: "darkgoldenrod"
            }
            Layout.preferredWidth:  parent.width * 0.34
            Layout.preferredHeight: parent.height * 0.2
            Layout.row: 0
            Layout.column: 1
        }
        Label {
            id: categoryLabel
            text: qsTr("Category")
            font.pixelSize: pageStyle.title.size
            font.family: pageStyle.title.name
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            background: Rectangle {
                gradient: pageStyle.goldHeader
                border.color: "darkgoldenrod"
            }
            Layout.preferredWidth: parent.width * 0.33
            Layout.preferredHeight: parent.height * 0.2
            Layout.row: 0
            Layout.column: 2
        }
        TextField {
            id: title
            Layout.maximumWidth: parent.width * 0.33
            Layout.maximumHeight: parent.height * 0.3
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.33
            text: entry.info.title
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            onEditingFinished: {
                s_titleChanged(text)
            }
            
            ToolTip.text: qsTr("Change transaction's title")
            ToolTip.visible: hovered
            ToolTip.delay: 500
            Layout.row: 1
            Layout.column: 0
        }
        
        DoubleSpinBox {
            id: spinbox
            Layout.maximumWidth: parent.width * 0.34
            Layout.fillHeight: true
            Layout.maximumHeight: parent.height * 0.3
            Layout.preferredWidth: parent.width * 0.34
            
            value: entry.value*100
            font.family:  pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            
            ToolTip.text: qsTr("Change transaction's value")
            ToolTip.visible: hovered
            ToolTip.delay: 500
            Layout.row: 1
            Layout.column: 1
            property date s_date
            Timer {
                id: timer
                repeat: false
                
                onTriggered: {
                    if(!info.opening)
                        info.s_valueChanged(spinbox.realValue)
                    
                }
            }
            
            onRealValueChanged: {
                timer.restart()
            }
        }
        
        
        CategoryItem {
            id: category
            objectName: "category"
            Layout.maximumWidth: parent.width * 0.33
            Layout.fillHeight: true
            Layout.maximumHeight: parent.height * 0.3
            Layout.preferredWidth: parent.width * 0.33
            editable: currentText === ""
            model: catModel
            Layout.row: 1
            Layout.column: 2
            onS_currentTextChanged: {
                if(currentText !== "")
                   info.s_catChanged(currentText)
            }
        }
        
    }
}
