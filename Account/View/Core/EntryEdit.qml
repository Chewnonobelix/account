import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S

GridLayout {
    id: grid
    rows: 3
    columns: 3
    //        anchors.fill: parent
    rowSpacing: height * 0.03
    columnSpacing: 0
    
    onColumnSpacingChanged: console.log("connard", columnSpacing)
    
    
    property bool opening: false
    S.AccountStyle {
        id: pageStyle
    }
    
    property var entry
    
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
        if(entry) {
            category.setting(entry.info.category)
            title.text = entry.info.title
            spinbox.value =  entry.value*100
        }
    }
    
    function reloadCat() {
        if(entry) category.setting(entry.info.category)
    }
    
    function changeDirection() {
        grid.flow = grid.flow === GridLayout.LeftToRight ? GridLayout.TopToBottom : GridLayout.LeftToRight
        
    }
    
    
    
    property var tWidth: flow === GridLayout.LeftToRight ? 0.33 : 1
    property var tWidth2: flow === GridLayout.LeftToRight ? 0.33 : 1
    property var tHeight: flow === GridLayout.LeftToRight ? 1 : 0.32
    
    Column {
        Layout.preferredWidth: parent.width * parent.tWidth2
        Layout.preferredHeight: parent.height *parent.tHeight
        spacing: parent.height * 0.02
        
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
            
            width: parent.width
            height: parent.height * 0.39
        }
        
        TextField {
            id: title
            enabled: !opening
            width: parent.width
            height: parent.height * 0.59
            text: ""
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            onEditingFinished: {
                s_titleChanged(text)
                opening = false
                enabled = true
            }
            
            ToolTip.text: qsTr("Change transaction's title")
            ToolTip.visible: hovered
            ToolTip.delay: 500
            
            onTextEdited: {
                enabled = false
                opening = true
            }
        }
    }
    
    Column {
        spacing: parent.height * 0.02
        Layout.preferredWidth: parent.width * parent.tWidth
        Layout.preferredHeight: parent.height *parent.tHeight
        
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
            
            height: parent.height * .39
            width: parent.width
        }
        
        DoubleSpinBox {
            id: spinbox
            
            height: parent.height * 0.59
            width: parent.width
            enabled: !opening
            
            value: 0
            font.family:  pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            
            ToolTip.text: qsTr("Change transaction's value")
            ToolTip.visible: hovered
            ToolTip.delay: 500
            
            property date s_date
            
            Timer {
                id: timer
                repeat: false
                
                onTriggered: {
                    if(!opening && !spinbox.isEditing)
                        s_valueChanged(spinbox.realValue)
                    
                }
            }
            
            onRealValueChanged: {
                timer.restart()
            }
        }
        
    }
    
    Column {
        spacing: parent.height * 0.02
        Layout.preferredWidth: parent.width * parent.tWidth2
        Layout.preferredHeight: parent.height *parent.tHeight
        
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
            
            height: parent.height * 0.39
            width: parent.width
        }
        
        CategoryItem {
            id: category
            objectName: "category"
            enabled: !info.opening
            
            height: parent.height * 0.59
            width: parent.width
            
            editable: currentText === ""
            model: catModel
            
            onS_currentTextChanged: {
                if(currentText !== "")
                    s_catChanged(currentText)
            }
        }
    }        
}
