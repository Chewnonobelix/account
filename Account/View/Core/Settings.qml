import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Dialog {
   id: root
   title: qsTr("Settings")
   standardButtons: Dialog.Save | Dialog.Cancel
   
   width: 600
   height: 800

   anchors.centerIn: parent

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

           GridLayout {
               columns: 2
               Label {
                  text: qsTr("Budget")
               }

               CheckBox {
                   objectName: "budget"
               }

               Label {
                  text: qsTr("Frequency")
               }

               CheckBox {
                   objectName: "frequency"
               }

               Label {
                  text: qsTr("Common Expanse")
               }

               CheckBox {
                   objectName: "common"
               }

           }
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
               
               Frame {
                   visible: false
               }
               
               CheckBox {
                   id: secondadyEnable
                   text: qsTr("Enable")
                   checked: true
               }

               Label {
                   text: qsTr("Secondary database")
               }
               
               Loader {
                   sourceComponent: db
                   active: secondadyEnable.checked
               }
               
           }
       }
       
      
   }
}
