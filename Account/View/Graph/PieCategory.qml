import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

import "../Style"

GridLayout {
    
    ChartView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        backgroundColor: "transparent"
        objectName: "incomingPie"
        
        function clear() {
            incoming.clear()
        }
        
        function append(name, value) {
            incoming.append(name, value)
        }

        PieSeries {
            name: "incoming"
            id: incoming

            onHovered: {
                slice.exploded = state
                slice.labelVisible = state
                tool.visible = state
                tool.text = (slice.percentage*100).toFixed(2) + "%"
            }
        }
    }
    
    ChartView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        backgroundColor: "transparent"
        objectName: "outcomePie"
        
        function clear() {
            outcome.clear()
        }
        
        function append(name, value) {
            outcome.append(name, value)
        }
        
        ToolTip {
            id: tool
        }
        
        PieSeries {
            id: outcome
            name: "outcome"            
            
            onHovered: {
                slice.exploded = state
                slice.labelVisible = state
                tool.visible = state
                tool.text = (slice.percentage*100).toFixed(2) + "%"
            }
        }
    }
}
