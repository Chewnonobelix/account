import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "../Style"

Item {
    id: root
    property int pageIndex: 1
    property int maxPage: 5
    
    signal s_pageChange()
    
    onPageIndexChanged: {
        if(pageIndex < 1) pageIndex = 1
        if(pageIndex > maxPage) pageIndex = maxPage
        
        pageSkip.pagesIndex = pageIndex
        s_pageChange()
    }
    
    RowLayout
    {
        anchors.fill: parent
        spacing: 0
        AccountButton {
            id: previousPages
            
            text: "<<"
            Layout.fillHeight: true
            Layout.preferredWidth: root.width * 0.2           
            onClicked: root.pageIndex -= 10
            
            enabled: root.maxPage > 1 && (root.pageIndex > 1)
            
            ToolTip.text: qsTr("Previous 10 pages")
            ToolTip.delay: 500
            ToolTip.visible: hovered
        }
        
        AccountButton {
            id: previousPage
            text: "<"
            Layout.fillHeight: true
            Layout.preferredWidth: root.width * 0.2            
            
            onClicked: root.pageIndex--
            
            enabled: root.maxPage > 1 && (root.pageIndex > 1)
            
            ToolTip.text: qsTr("Previous page")
            ToolTip.delay: 500
            ToolTip.visible: hovered
        }
        
        TextField {
            id: pageSkip
            objectName: "pageSkip"
            text: pagesIndex
            horizontalAlignment: Qt.AlignHCenter
            property int pagesIndex: root.pageIndex
            
            Layout.fillHeight: true
            Layout.fillWidth: true 
            
            enabled: root.maxPage > 1
            
            ToolTip.text: qsTr("Current page")
            ToolTip.delay: 500
            ToolTip.visible: hovered
        }
        
        AccountButton {
            id: nextPage
            text: ">"
            Layout.fillHeight: true
            Layout.preferredWidth: root.width * 0.2            
            
            onClicked: root.pageIndex++
            
            enabled: root.maxPage > 1 || (root.pageIndex < root.maxPage)
            
            ToolTip.text: qsTr("Next page")
            ToolTip.delay: 500
            ToolTip.visible: hovered
        }
        
        AccountButton {
            id: nextPages
            text: ">>"
            Layout.fillHeight: true
            Layout.preferredWidth: root.width * 0.2           
            
            onClicked: root.pageIndex += 10
                        
            enabled: root.maxPage > 1 || (root.pageIndex < root.maxPage)
            
            ToolTip.text: qsTr("Next 10 pages")
            ToolTip.delay: 500
            ToolTip.visible: hovered
        }
    }
}
