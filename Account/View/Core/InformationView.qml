import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S
import "../Frequency" as F

Item {
    id: info
    objectName: "info"
    property int maximum: Screen.width * .55 - 10
    width: maximum
    

    S.AccountStyle {
        id: pageStyle
    }
    
    ColumnLayout {
        anchors.fill: parent
        EntryEdit {
            id: ee
            objectName: "entryEdit"
            visible: parent.visible
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * .15
        }
        spacing: 10
        Label {
            text: qsTr("Coming Soon")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
    //Frequency
    //    GroupBox {
    //        anchors.top:title.bottom
    //        anchors.topMargin: 10
    //        anchors.left: parent.left
    //        anchors.right: parent.right
    //        anchors.bottom: parent.bottom
    //        anchors.bottomMargin: 10
    
    
    //        label:  CheckBox {
    //            id: freqCheck
    //            objectName: "freqCheck"
    //            text:  qsTr("Frequency")
    
    //            signal s_check(bool check)
    
    //            onCheckedChanged: s_check(checked) 
    //        }
    
    //        contentItem: F.Frequency {
    //            id: freq
    //            enabled: freqCheck.checked
    //            objectName: "freq"
    //        }
    //    }
}
