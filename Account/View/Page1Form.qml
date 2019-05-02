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
        implicitHeight: parent.height / 2
        implicitWidth: parent.width * 0.2
    }

    /* */
    Adding {
        id: addingid
        objectName: "addingid"
        y: pY > 0 ? (mainWindow.height * pY) - Math.max(
                        0, mY) : (parent.height / 2 + add.height / 2 + 10)
        x: pY > 0 ? (mainWindow.width * pX) - Math.max(0, mX) : (add.width / 2)
        property double pX: -1
        property double pY: -1
        property double mX: -1
        property double mY: -1
        Component.onCompleted: {
            reset()
        }

        function setSize(px,py) {
            mX = ((mainWindow.width * px) + width) - mainWindow.width
            mY = ((mainWindow.height * py) + height) - (mainWindow.height)

            pX = px
            pY = py
        }
    }

    Component.onCompleted: {
        addingid.close()
    }

    function openAdding(pX, pY) {

        addingid.setSize(pX, pY)

        if (cal.selectedDates.length > 0) {
            for (index in cal.selectedDates) {
                addingid.addDate(cal.selectedDates[index])
            }
        } else {
            addingid.addDate(Qt.formatDate(new Date(), "dd-MM-yyyy"))
        }
        addingid.open()
    }

    RowLayout {
        anchors.top: cal.bottom
        anchors.topMargin: 10
        id: group
        spacing: 10
        //        height: 50
        width: cal.width
        enabled: accountSelect.model.length > 0
        Component.onCompleted: console.log("XXX", height)
        Button {
            id: add
            text: qsTr("Add")
            width: parent.width * .45
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            ToolTip.text: qsTr("Add new transaction")
            ToolTip.visible: hovered
            ToolTip.delay: 500

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

            onClicked: {
                mainWindow.adding(false)
            }
        }

        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            property int index: view.currentIndex
            enabled: view.currentIndex !== -1

            ToolTip.text: qsTr("Remove select transaction")
            ToolTip.visible: hovered
            ToolTip.delay: 500

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
                if (index > -1) {
                    mainWindow.remove(defaultModel.get(index).id)
                }
            }
        }
    }

    property int currentId: view.currentIndex > -1 && defaultModel.get(
                                view.currentIndex).label
                            !== "Initial" ? defaultModel.get(
                                                view.currentIndex).id : -1

    Rectangle {
        color: "transparent"
        anchors.top: group.bottom
        //        anchors.topMargin: 210
        anchors.right: view.left
        anchors.left: parent.left

        BudgetView {
            anchors.fill: parent
        }
    }

    ListModel {
        id: defaultModel
        objectName: "defaultModel"

        function swap(ij) {
            move(j, i, 1)
            move(i + 1, j, 1)
        }

        function sort(roleorder) {
            for (; i < count; i++) {
                for (; j < count; j++) {
                    if (order === Qt.AscendingOrder) {
                        if (get(j)[role] < get(i)[role]) {
                            swap(i, j)
                        }
                    } else {
                        if (get(j)[role] > get(i)[role]) {
                            swap(i, j)
                        }
                    }
                }
            }
        }
    }

    onWidthChanged: {
        view.width = ((width * 0.25) - 5)
                < view.maximumWidth ? (width * 0.25) + 5 : view.maximumWidth
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

        enabled: pageSkip.maxPage > 1 || (pageSkip.pageIndex < pageSkip.maxPage)

        ToolTip.text: qsTr("Next 10 pages")
        ToolTip.delay: 500
        ToolTip.visible: hovered

        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            anchors.fill: parent
            border.color: "darkgoldenrod"
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

        onClicked: pageSkip.pageIndex++

        enabled: pageSkip.maxPage > 1 || (pageSkip.pageIndex < pageSkip.maxPage)

        ToolTip.text: qsTr("Next page")
        ToolTip.delay: 500
        ToolTip.visible: hovered

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
            border.color: "darkgoldenrod"
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

        onClicked: pageSkip.pageIndex--

        enabled: pageSkip.maxPage > 1 && (pageSkip.pageIndex > 1)

        ToolTip.text: qsTr("Previous page")
        ToolTip.delay: 500
        ToolTip.visible: hovered

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
            border.color: "darkgoldenrod"
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

        enabled: pageSkip.maxPage > 1 && (pageSkip.pageIndex > 1)

        ToolTip.text: qsTr("Previous 10 pages")
        ToolTip.delay: 500
        ToolTip.visible: hovered

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
            border.color: "darkgoldenrod"
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

        enabled: maxPage > 1

        ToolTip.text: qsTr("Current page")
        ToolTip.delay: 500
        ToolTip.visible: hovered

        signal s_pageChange

        onPageIndexChanged: {
            if (pageIndex < 1)
                pageIndex = 1
            if (pageIndex > maxPage)
                pageIndex = maxPage

            s_pageChange()
        }
    }

    TableView {
        anchors.left: cal.right
        anchors.leftMargin: 5
        height: parent.height * 0.95
        width: (parent.width * 0.30) - 5
        id: view
        objectName: "entryView"
        model: defaultModel

        property int maximumWidth: 4 * 100 + 60

        horizontalScrollBarPolicy: Qt.ScrollBarAsNeeded

        sortIndicatorVisible: true
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
            if (selection.contains(index)) {
                selection.clear()
                currentIndex = -1
            } else {
                selection.clear()
                currentIndex = index
                selection.select(index)
            }
        }

        function selectFromId(id) {
            for (var i = 0; i < defaultModel.count; i++) {
                if (defaultModel.get(i).id === id) {
                    setNewIndex(i)
                }
            }
        }

        TableViewColumn {
            role: "type"
            title: qsTr("[+/-]")
            width: 45
            movable: false
            resizable: false
            id: typeColumn
            property string tipText: "*:" + qsTr("estimated entry")
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
                    property string est: defaultModel.get(
                                             styleData.row).estimated ? "*" : ""
                    text: styleData.value === "income" ? "+" + est : "-" + est
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
                    text: styleData.value
                    clip: true
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                }
            }
        }

        TableViewColumn {
            role: "label"
            title: qsTr("Label")
            width: 110
            movable: false
            resizable: false
            id: labelHeader

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
                    text: styleData.value
                    clip: true
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                }
            }
        }

        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
            movable: false
            resizable: false

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
                    text: styleData.value
                    clip: true
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                }
            }
        }

        headerDelegate: Rectangle {
            gradient: isClicked ? pageStyle.darkGoldButton : pageStyle.goldHeader

            height: view.height * 0.03
            anchors.centerIn: parent
            anchors.leftMargin: 10
            property bool isHovered: styleData.containsMouse
            property bool isClicked: styleData.pressed

            border.color: "darkgoldenrod"
            Label {
                id: headerText
                height: parent.height * .8
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
            defaultModel.sort(getColumn(sortIndicatorColumn).role,
                              sortIndicatorOrder)
            if (currentIndex !== -1) {
                s_view(defaultModel.get(currentIndex).id)
            }
        }

        onSortIndicatorOrderChanged: {
            defaultModel.sort(getColumn(sortIndicatorColumn).role,
                              sortIndicatorOrder)
            if (currentIndex !== -1) {
                s_view(defaultModel.get(currentIndex).id)
            }
        }

        function getToolTip(index) {
            if (index === 2) {
                return typeColumn.tipText
            }

            return ""
        }

        itemDelegate: Rectangle {
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor

                ToolTip.text: styleData.value
                ToolTip.visible: styleData.hovered
                ToolTip.timeout: 500

                onClicked: {
                    if (view.selection.contains(styleData.row)) {
                        view.selection.clear()
                    } else {
                        view.selection.clear()
                        view.selection.select(styleData.row)
                    }

                    if (view.currentIndex !== styleData.row) {
                        view.currentIndex = styleData.row
                    } else {
                        view.unselectAll()
                    }
                }
            }
        }

        rowDelegate: Rectangle {
            id: rectRow

            width: parent.width
            height: 20

            gradient: styleData.selected ? defaultModel.get(
                                               styleData.row).type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
        }

        onCurrentIndexChanged: {
            infoView.visible = (currentIndex !== -1)
                    && (defaultModel.get(currentIndex).label !== "Initial")
            remove.enabled = (view.currentIndex !== -1)
                    && (defaultModel.get(view.currentIndex).label !== "Initial")

            if (currentIndex !== -1)
                s_view(defaultModel.get(currentIndex).id)
        }
    }

    ScrollView {

        contentWidth: infoView.width
        contentHeight: infoView.height
        anchors.left: view.right
        anchors.right: parent.right
        height: parent.height

        anchors.leftMargin: 10

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded

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
