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
        titleLabel.enabled = enabled
        categoryLabel.enabled = enabled
        valueLabel.enabled = enabled
    }
    
    onEntryChanged: {
        category.setting(entry.info.category)
        title.text = entry.info.title
        spinbox.value =  entry.value*100
    }
    
    function reloadCat() {
        if(entry) category.setting(entry.info.category)
    }
    
    function changeDirection() {
        grid.flow = grid.flow === GridLayout.LeftToRight ? GridLayout.TopToBottom : GridLayout.LeftToRight
        
    }
    
    
    GridLayout {
        id: grid
        rows: 3
        columns: 3
        anchors.fill: parent
        rowSpacing: 0
        
        property var tWidth: flow === GridLayout.LeftToRight ? 0.33 : 1
        property var tHeight: flow === GridLayout.LeftToRight ? 1 : 0.33
        
        Column {
            Layout.preferredWidth: parent.width * parent.tWidth
            Layout.preferredHeight: parent.height *parent.tHeight
            spacing: height * 0.05
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
                height: parent.height * 0.40
            }
            
            TextField {
                id: title
                
                width: parent.width
                height: parent.height * 0.60
                text: ""
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                onEditingFinished: {
                    s_titleChanged(text)
                    info.opening = false
                }
                
                ToolTip.text: qsTr("Change transaction's title")
                ToolTip.visible: hovered
                ToolTip.delay: 500
                
                onTextEdited: {
                    info.opening = true
                }
            }
        }
        
        Column {
            spacing: height * 0.05
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
                
                height: parent.height * .40
                width: parent.width
            }
            
            DoubleSpinBox {
                id: spinbox
                
                height: parent.height * 0.60
                width: parent.width
                
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
                        if(!info.opening && !spinbox.isEditing)
                            info.s_valueChanged(spinbox.realValue)
                        
                    }
                }
                
                onRealValueChanged: {
                    timer.restart()
                }
            }
            
        }
        
        Column {
            spacing: height * 0.05
            Layout.preferredWidth: parent.width * parent.tWidth
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
                
                height: parent.height * 0.40
                width: parent.width
            }
            
            
            
            
            CategoryItem {
                id: category
                objectName: "category"
                
                height: parent.height * 0.60
                width: parent.width
                
                editable: currentText === ""
                model: catModel

                onS_currentTextChanged: {
                    if(currentText !== "")
                        info.s_catChanged(currentText)
                }
            }
        }
        
    }
}
