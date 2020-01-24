import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Dialog {
   id: root
   title: qsTr("Settings")
   standardButtons: Dialog.Save | Dialog.Cancel
   
   ColumnLayout {
       anchors.fill: parent
       
       GroupBox {
           title: qsTr("General")
           
           GridLayout {
               columns: 2
               
               Label {
                   text: qsTr("Language")
               }
               
               ComboBox {
                   model: ["English", "Francais"]
               }
           }
       }
       
      
       GroupBox {
           title: qsTr("Features")
       }

       Component {
           id: db
           ComboBox {
               model: ["Sql", "Xml"]
           }
       }

       GroupBox {
           title: qsTr("Database")
           
           GridLayout {
               columns: 3
               
               Label {
                   text: qsTr("Main database")
               }
               
               Loader {
                   active: true
                   sourceComponent: db
               }
               
               Rectangle {
                   
               }
               
               Label {
                   text: qsTr("Secondary database")
               }
               
               Loader {
                   source: db
                   active: secondadyEnable.checked
               }
               
               CheckBox {
                   id: secondadyEnable
                   text: qsTr("Enable")
                   checked: true
               }
           }
       }
       
      
   }
}
