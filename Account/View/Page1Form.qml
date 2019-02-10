import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Page {

    implicitWidth: parent.width
    implicitHeight: parent.height

    id: pageTable

    property int v_dateMonth: cal.currentMonth + 1
    property int v_dateYear: cal.currentYear

    AccountStyle {
        id: pageStyle
    }

    background: Rectangle {
        color: "transparent"
    }

    MultiCalendar {
        id: cal
        objectName: "cal"
        weekNumbersVisible: true
        implicitHeight: parent.height /2
        implicitWidth: parent.width * 0.2
    }

    /* */

    Adding {
        id: addingid
        objectName: "addingid"
        y: pY > 0 ? (mainWindow.height * pY) - Math.max(0, mY) : (parent.height / 2 + add.height / 2 + 10)
        x: pY > 0 ? (mainWindow.width * pX) - Math.max(0, mX): (add.width / 2)
        property double pX: -1
        property double pY: -1
        property double mX: -1
        property double mY: -1
        Component.onCompleted:   {
            reset()
        }

        function setSize(px, py) {
            mX = ((mainWindow.width * px)+width)-mainWindow.width
            mY = ((mainWindow.height * py)+height)-(mainWindow.height)

            pX = px
            pY= py
        }
    }


    Component.onCompleted: {
        addingid.close()
    }

    function openAdding(pX, pY) {

        addingid.setSize(pX, pY)

        if(cal.selectedDates.length > 0) {
            for(var index in cal.selectedDates){
                addingid.addDate(cal.selectedDates[index])
            }
        }
        else {
            addingid.addDate(Qt.formatDate(new Date(), "dd-MM-yyyy"))
        }
        addingid.open()
    }

    Item {
        anchors.top: cal.bottom
        anchors.topMargin: 10
        width: cal.width
        height: edit.height + add.height + 10
        id: group
        enabled: accountSelect.model.length > 0


        Button {
            id: add
            text: qsTr("Add")
            width: parent.width * .45
            anchors.top: parent.top
            anchors.left: parent.left
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            Shortcut {
                context: Qt.ApplicationShortcut
                onActivated:  {
                    add.clicked()
                }
                sequence: "A"
            }



            Rectangle {
                id: rectAdd
                anchors.fill: parent
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }


            MouseArea {
                z: -1
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }

            onClicked:  {
                mainWindow.adding(false)
            }

        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            property int index: view.currentIndex
            enabled: view.currentIndex !== -1
            Shortcut {
                context: Qt.ApplicationShortcut
                onActivated:  {
                    remove.clicked()
                }
                sequence: "R"
            }

            MouseArea {
                z: -1
                anchors.fill: parent
                cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                acceptedButtons: Qt.NoButton
            }

            Rectangle {
                id: rectRemove

                anchors.fill: parent
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
            onClicked: {
                if(index > -1) {
                    mainWindow.remove(defaultModel.get(index).id)
                }
            }
        }
    }



    ListModel {
        id: defaultModel
        objectName: "defaultModel"

        function swap(i, j) {
            move(j,i,1)
            move(i+1,j,1)
        }

        function sort(role, order) {
            for(var i = 0; i < count; i++ ) {
                for(var j = i; j < count; j++) {
                    if(order === Qt.AscendingOrder) {
                        if(get(j)[role] < get(i)[role]) {
                            swap(i,j)
                        }
                    } else {
                        if(get(j)[role] > get(i)[role]) {
                            swap(i,j)
                        }
                    }
                }
            }
        }
    }

    onWidthChanged: {
        view.width = ((width * 0.25) - 5) < view.maximumWidth ? (width * 0.25) + 5 : view.maximumWidth
    }



    Button {
        id: nextPages
        anchors.top: view.bottom
        anchors.right: view.right
        text: ">>"
        height: parent.height * 0.05
        width: view.width * 0.20

        onClicked: pageSkip.pageIndex += 10

        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }

    }

    Button {
        id: nextPage
        anchors.top: view.bottom
        anchors.right: nextPages.left
        text: ">"
        height: parent.height * 0.05
        width: view.width * 0.20

        onClicked: pageSkip.pageIndex ++

        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }

    }

    Button {
        id: previousPage
        anchors.top: view.bottom
        anchors.left: previousPages.right
        text: "<"
        height: parent.height * 0.05
        width: view.width * 0.20

        onClicked: pageSkip.pageIndex --

        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }

    }

    Button {
        id: previousPages
        anchors.top: view.bottom
        anchors.left: view.left
        text: "<<"
        height: parent.height * 0.05
        width: view.width * 0.20

        onClicked: pageSkip.pageIndex -= 10

        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }

    }


    TextField {
        id: pageSkip
        objectName: "pageSkip"
        property int pageIndex: 1
        property int maxPage: 5
        anchors.top: view.bottom
        anchors.left: previousPage.right
        anchors.right: nextPage.left
        height: parent.height * 0.05
        text: pageIndex
        horizontalAlignment: Qt.AlignHCenter

        signal s_pageChange()

        onPageIndexChanged: {
            if(pageIndex < 1) pageIndex = 1
            if(pageIndex > maxPage) pageIndex = maxPage

            s_pageChange()
        }
    }

    TableView {
        anchors.left: cal.right
        anchors.leftMargin: 5
        height: parent.height * 0.95
        width: (parent.width * 0.25) - 5
        id: view
        objectName: "entryView"
        model: defaultModel

        property int maximumWidth: 4*100+40

        sortIndicatorVisible:  true
        property string currentType
        property int currentIndex: -1

        function fAdd(i) {
            defaultModel.append(i)
        }


        function unselectAll() {
            selection.clear()
            currentIndex = -1
        }

        function reset() {
            defaultModel.clear()
            currentIndex = -1
            infoView.visible = false
        }

        signal s_view(int index)

        backgroundVisible: false
        Connections {
            target: cal
            onS_datesChanged: {
                view.reset()
            }

        }

        onWidthChanged: {
            flickableItem.contentX = 0
        }

        onHeightChanged: {
            flickableItem.contentY = 0
        }

        TableViewColumn {
            role: "id"
            visible: false
        }

        TableViewColumn {
            role: "estimated"
            visible: false
        }

        function setNewIndex(index) {
            if(selection.contains(index)) {
                selection.clear()
                currentIndex = -1
            } else {
                selection.clear()
                currentIndex = index
                selection.select(index)
            }
        }

        function selectFromId(id) {
            for(var i = 0; i < defaultModel.count; i++) {
                if(defaultModel.get(i).id === id) {
                    setNewIndex(i)
                }
            }
        }

        TableViewColumn {
            role: "type"
            title: qsTr("[+/-]")
            width: 40
            movable: false
            resizable: false
            id: typeColumn
            property string tipText: "*: estimated entry"
            delegate: Rectangle {
                color: "transparent"
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        view.setNewIndex(styleData.row)
                    }
                }
                Label {
                    property string est: defaultModel.get(styleData.row).estimated ? "*" : ""
                    text: styleData.value === "income" ? "+"+est:"-"+est
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                }

            }
        }


        TableViewColumn {
            role: "date"
            title: qsTr("Date")
            width: 100
            movable: false
            resizable: false
            id: columnDate


            delegate: Rectangle {
                color: "transparent"
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        view.setNewIndex(styleData.row)
                    }
                }

                Label {
                    text: Qt.formatDate(styleData.value, "dd-MM-yyyy")
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                }
            }
        }

        TableViewColumn {
            role: "value"
            title: qsTr("Value")
            width: 100
            movable: false
            resizable: false
        }

        TableViewColumn {
            role: "label"
            title: qsTr("Label")
            width: 100
            movable: false
            resizable: false
        }


        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
            movable: false
            resizable: false
        }


        headerDelegate: Rectangle {
            gradient: isClicked ? pageStyle.darkGoldButton :  pageStyle.goldHeader

            height: view.height * 0.03
            anchors.centerIn: parent
            anchors.leftMargin: 10
            property bool isHovered: styleData.containsMouse
            property bool isClicked: styleData.pressed

            Label {
                id: headerText
                height: parent.height*.8
                anchors.centerIn: parent
                text: styleData.value
                font.family: pageStyle.title.name
                font.pixelSize: height * 0.85

                ToolTip.visible: isHovered && (styleData.column === 2)
                ToolTip.text: view.getToolTip(styleData.column)
                ToolTip.delay: 500


            }
        }


        onSortIndicatorColumnChanged: {
            defaultModel.sort( getColumn(sortIndicatorColumn).role , sortIndicatorOrder)
            if(currentIndex !== -1) {
                s_view(defaultModel.get(currentIndex).id)
            }
        }

        onSortIndicatorOrderChanged: {
            defaultModel.sort(getColumn(sortIndicatorColumn).role , sortIndicatorOrder)
            if(currentIndex !== -1) {
                s_view(defaultModel.get(currentIndex).id)
            }
        }


        function getToolTip(index) {
            if(index === 2) {
                return typeColumn.tipText
            }

            return ""
        }

        itemDelegate: Rectangle {
            width: parent.width
            height: 20
            anchors.fill: parent
            color: "transparent"
            Label {
                id: textItem
                text: styleData.value


                anchors.centerIn: parent

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    if(view.selection.contains(styleData.row)) {
                        view.selection.clear()
                    }
                    else {
                        view.selection.clear()
                        view.selection.select(styleData.row)
                    }

                    if(view.currentIndex !== styleData.row) {
                        view.currentIndex = styleData.row
                    }
                    else {
                        view.unselectAll()
                    }

                }
            }
        }

        rowDelegate: Rectangle {
            id:rectRow

            width: parent.width
            height: 20


            gradient: styleData.selected ? defaultModel.get(styleData.row).type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
        }

        onCurrentIndexChanged: {
            infoView.visible = (currentIndex !== -1) && (defaultModel.get(currentIndex).label !== "Initial")
            remove.enabled = (view.currentIndex !== -1) && (defaultModel.get(view.currentIndex).label !== "Initial")

            if(currentIndex !== -1)
                s_view(defaultModel.get(currentIndex).id)
        }
    }

    ScrollView {

        contentWidth: infoView.width
        contentHeight: infoView.height
        anchors.left: view.right
        width: (parent.width*0.55)-10
        height: parent.height


        anchors.leftMargin: 10

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

        clip: true
        InformationView {
            id: infoView
            objectName: "infoView"
            height: pageTable.height
            clip: true
            visible: false
            enabled: true
        }



    }

}

