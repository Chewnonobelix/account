import QtQuick 2.15
import QtQuick.Controls 2.15


MouseArea {
    id: area
    signal s_datesChanged()
    signal s_monthChanged()
    property int currentMonth: month.currentMonth
    property int currentYear: month.currentYear
    property var selectedDates: month.selectedDates
    property var selectedDate: month.selectedDate
    
    property bool multiple: true
    
    function set(date) {
        selectedDate = date
    }
    
    StackView {
        id: root
        anchors.fill: parent
        clip: true
        Component {
            id: year
            
            CalendarPage {
                columns: 3
                rows: 4
                label: month.currentYear
                model: [Date.fromLocaleString(locale, "01-01-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-02-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-03-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-04-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-05-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-06-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-07-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-08-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-09-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-10-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-11-"+month.currentYear, "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-12-"+month.currentYear, "dd-MM-yyyy")]
                
                onS_select: {
                    month.currentMonth = Qt.formatDate(d, "MM") - 1  
                    zoom(2)
                }
            }
        }
        
        
        Component {
            id: decade
            
            CalendarPage {
                columns: 5
                rows: 2
                label: Math.floor(month.currentYear/10)+"X"
                decade: true
                model: [Date.fromLocaleString(locale, "01-01-"+Math.floor(month.currentYear/10)+"0", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-02-"+Math.floor(month.currentYear/10)+"1", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-03-"+Math.floor(month.currentYear/10)+"2", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-04-"+Math.floor(month.currentYear/10)+"3", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-05-"+Math.floor(month.currentYear/10)+"4", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-06-"+Math.floor(month.currentYear/10)+"5", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-07-"+Math.floor(month.currentYear/10)+"6", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-08-"+Math.floor(month.currentYear/10)+"7", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-09-"+Math.floor(month.currentYear/10)+"8", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-10-"+Math.floor(month.currentYear/10)+"9", "dd-MM-yyyy")]
                
                onS_select: {
                    month.currentYear = Qt.formatDate(d, "yyyy")
                    zoom(2)
                }
            }
        }
        
        initialItem: MultiCalendar {
            id: month
            weekNumbersVisible: true
            multiple: area.multiple
            onS_datesChanged: area.s_datesChanged()
            onS_monthChanged: area.s_monthChanged()
        }
        
        pushEnter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    from: -root.width/2
                    to: 0
                }
                NumberAnimation {
                    property: "y"
                    from: -root.height/2
                    to: 0
                }
                NumberAnimation {
                    property: "width"
                    from: root.width*2
                    to: root.width
                }
                NumberAnimation {
                    property: "height"
                    from: root.height*2
                    to: root.height
                }
            }
        }
        
        popEnter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    from: 0
                    to: 0
                }
                NumberAnimation {
                    property: "y"
                    from: 0
                    to: 0
                }
                NumberAnimation {
                    property: "width"
                    from: root.width
                    to: root.width
                }
                NumberAnimation {
                    property: "height"
                    from: root.height
                    to: root.height
                }
            }
        }

        popExit: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "x"
                    to: -root.width/2
                    from: 0
                }
                NumberAnimation {
                    property: "y"
                    to: -root.height/2
                    from: 0
                }
                NumberAnimation {
                    property: "width"
                    to: root.width*2
                    from: root.width
                }
                NumberAnimation {
                    property: "height"
                    to: root.height*2
                    from: root.height
                }
            }
        }
    }
    
    function zoom(way) {
        if(way < 1) {
            if(root.depth === 2) {
                root.push(decade)
            }
            if(root.depth === 1) {
                root.push(year)
            }
        }
        
        if(way > 1) {
            if(root.depth > 1) {
                root.pop()
            }
        }
    }
    
    onWheel: {
        zoom(wheel.angleDelta.y > 1 ? 2 : 0)
    }
    
    
    acceptedButtons: Qt.NoButton
    
}
