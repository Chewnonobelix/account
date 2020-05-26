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
        PieSeries {
            name: "incoming"
            PieSlice {
                label: "test"
                value: 1.3
            }
            PieSlice {
                label: "test2"
                value: 1
            }
        }
    }
    
    ChartView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        backgroundColor: "transparent"
        
        ToolTip {
            text: "parent.percentage" + "%"
            id: tool
        }
        
        PieSeries {
            name: "outcome"
            PieSlice {
                label: "test3"
                value: 1.5
            }
            PieSlice {
                label: "test4"
                value: 1
            }
            
            PieSlice {
                label: "test5"
                value: 3
                
            }
            
            
            onHovered: {
                slice.exploded = state
                slice.labelVisible = state
                tool.visible = state
                tool.text = (slice.percentage*100).toFixed(2) + "%"
                if(state) console.log(slice.label, slice.value, slice.percentage, state)
            }
        }
        
    }
}
