import QtQuick 2.12
import QtQuick.Window 2.13
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13

import Account 1.0

import "../Style"
import "../Core"

import Account.Frequency 1.0

Rectangle {
    id: root

    ListModel {
        id: testModel
    }

    onFocusChanged: _frequency.openManager()

    gradient: AccountStyle.unselectView

    Generate {
        objectName: "generate"
        anchors.centerIn: root
    }

    GridLayout {
        anchors.fill: parent
        id: layout

        anchors.topMargin: root.height * 0.02
        anchors.bottomMargin: root.height * 0.02
        anchors.leftMargin: root.width * 0.02
        anchors.rightMargin: root.width * 0.02

        rowSpacing: root.height * 0.02
        columnSpacing: root.width * 0.02

        AccountHeader {
            text: qsTr("Reference") + " ->"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.20
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.alignment: Qt.AlignTop
        }

        ListView {
            Connections {
                target: _frequency

                function onSetModel(model, index) {
                    frequencyList.model = model
                    frequencyList.currentIndex = index
                }
            }

            id: frequencyList
            objectName: "frequencyList"
            model: []

            Layout.preferredHeight: root.height * 0.77
            Layout.preferredWidth: root.width * 0.20
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 2
            Layout.rowSpan: 3
            currentIndex: -1
            header:  AccountHeader {
                text: qsTr("Frequency list")
                height: frequencyList.height * .10
                width: frequencyList.width
            }

            property var currentModel: currentIndex !== -1 ? model[currentIndex] : null

            onCurrentModelChanged: {
                if(currentModel) {
                    _frequency.setWorker(currentModel.name)
                    pageChanger.pageIndex = 1
                    pageChanger.pageChange()
                }
                else
                {
                    testModel.clear()
                    pageChanger.pageIndex = 0
                }

            }

            Component.onCompleted: {
                ref.entry = Qt.binding(function() {return currentModel ? currentModel.reference : null})
                ref.enabled = Qt.binding(function() {return currentModel !== null})
                groupText.nb = Qt.binding(function() {return currentModel ? currentModel.nbGroup : 0})
                countText.nb = Qt.binding(function() {return currentModel ? currentModel.count : 0})
                pageChanger.maxPage = Qt.binding(function() {return countText.nb / 100 + 1})
                whenCombo.enabled = Qt.binding(function() {return count !== 0})
                whenCombo.currentIndex = Qt.binding(function() {return currentModel  ? whenCombo.model.findIndex(currentModel.freq + 0) : 0})
                endless.checked = Qt.binding(function() {return currentModel ? currentModel.endless : false})
            }

            clip: true
            Rectangle {
                anchors.fill: parent
                border.color: "gold"
                color: "transparent"
            }

            onModelChanged: {
                currentIndex = -1
            }

            delegate: Rectangle {
                height: 40
                width: frequencyList.width

                gradient: ref.entry && ListView.isCurrentItem ? ref.entry.type === "income" ? AccountStyle.selectViewIn : AccountStyle.selectViewOut : AccountStyle.unselectView

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        frequencyList.currentIndex = index
                        _frequency.currentId = modelData.id
                    }

                    cursorShape: Qt.PointingHandCursor
                }

                AccountLabel {
                    anchors.fill: parent
                    text: modelData.name === "" ? modelData.id : modelData.name
                }
            }
        }

        AccountButton {
            objectName: "addFreq"
            text: "+"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.09
            Layout.row: 4
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1

            signal s_addFrequency()

            ToolTip.text: qsTr("Add new frequency")
            
            onClicked: _frequency.addFrequency()
        }

        AccountButton {
            objectName: "removeFreq"
            text: "-"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.09
            Layout.row: 4
            Layout.column: 1
            Layout.columnSpan: 1
            Layout.rowSpan: 1

            signal s_removeFrequency(var freq)

            ToolTip.text: qsTr("Remove frequency")
            
            onClicked: _frequency.removeFrequency(frequencyList.currentModel.id)
        }

        EntryEdit {
            id: ref
            objectName: "ref"

            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.74
            Layout.row: 0
            Layout.column: 2
            Layout.columnSpan: 6
            Layout.rowSpan: 1

            Connections {
                target: _frequency

                function onCatList(inc, out) {
                    ref.incomeList = inc
                    ref.outcomeList = out
                }
            }

            property var incomeList: []
            property var outcomeList: []

            onValueChanged: if(entry && enabled) _frequency.updateFreqValue(frequencyList.currentModel.id, value)
            onTitleChanged: if(entry && enabled) _frequency.updateFreqName(frequencyList.currentModel.id, title)
            onCatChanged: if(entry && enabled) _frequency.updateFreqCat(frequencyList.currentModel.id, cat, "manager")
            onSupportChanged: if(entry && enabled) _frequency.updateFreqSupport(frequencyList.currentModel.id, supp)
            onAddNewCategory: { if(entry && enabled) _frequency.addNewCategory(frequencyList.currentModel.id, cat, entry.type)
            }
            onEntryChanged: {
                typeCombo.currentIndex = CoreModel.typeModel.find(entry ? entry.type: "outcome")
            }

            Component.onCompleted: {
                enabled = Qt.binding(function() {return frequencyList.count !== 0})

                catModel = Qt.binding(function(){return (entry && entry.type === "income") ? incomeList : outcomeList})
                linked.catModel = Qt.binding(function() { return catModel})
            }

        }

        AccountComboBox {
            id: whenCombo
            objectName: "whenCombo"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.23
            Layout.row: 1
            Layout.column: 2
            Layout.columnSpan: 2
            Layout.rowSpan: 1

            enabled: ref.enabled

            ToolTip.text: qsTr("Select recurrence")
            
            model: CoreModel.freqModel
            textRole: "name"

            onCurrentIndexChanged: {
                if(down && ref.entry) _frequency.updateFreqFreq(frequencyList.currentModel.id, model.get(currentIndex).role)
            }
        }

        AccountComboBox {
            id: typeCombo
            objectName: "type"

            model: CoreModel.typeModel

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.23
            Layout.row: 1
            Layout.column: 4
            Layout.columnSpan: 2
            Layout.rowSpan: 1

            enabled: ref.enabled

            ToolTip.text: qsTr("Select frequency type")
            
            textRole: "name"
            valueRole: "type"

            onCurrentIndexChanged: {
                if(ref.entry && down){
                    _frequency.updateFreqType(frequencyList.currentModel.id, model.get(currentIndex).type)
                }
            }
        }

        AccountBackground {
            invisible: true
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.24
            Layout.row: 1
            Layout.column: 6
            Layout.columnSpan: 2
            Layout.rowSpan: 1

            AccountCheckBox {
                id: endless
                objectName: "endless"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: qsTr("Endless")
                signal s_endless(var id, bool e)

                onClicked: if(frequencyList.currentModel) _frequency.updateFreqEndless(frequencyList.currentModel.id, checked)
            }

            AccountButton {
                objectName: "generateOpen"
                text: qsTr("Generate")

                anchors.right: parent.right
                anchors.left: endless.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                enabled: ref.enabled && !endless.checked

                onReleased:  _frequency.openGenerate(frequencyList.currentModel.id)
            }

        }

        Rectangle {
            Layout.preferredHeight: root.height * 0.27
            Layout.preferredWidth: root.width * 0.36
            Layout.row: 2
            Layout.column: 5
            Layout.columnSpan: 3
            Layout.rowSpan: 1

            visible: frequencyList.currentIndex !== -1
            color: "transparent"
            border.color: "gold"

            AccountLabel {
                id: groupText
                property int nb: 0
                text: qsTr("Number of generation") + ": " + nb
                width: parent.width
                height: parent.height / parent.children.length
            }

            AccountLabel {
                id: countText
                anchors.top: groupText.bottom

                property int nb: 0
                text: qsTr("Number of entries") + ": " + nb
                width: parent.width
                height: parent.height / parent.children.length
            }

            AccountLabel {
                id: dateText
                property var from: frequencyList.currentModel ? frequencyList.currentModel.begin: ""
                property var to: frequencyList.currentModel ? frequencyList.currentModel.end: ""
                anchors.top: countText.bottom
                text: qsTr("From") + " " + Qt.formatDate(from, "dd-MM-yyyy") + ", " + qsTr("to") + " " + Qt.formatDate(to, "dd-MM-yyyy")
                width: parent.width
                height: parent.height / parent.children.length
            }

            AccountLabel {
                id: worker
                objectName: "worker"
                anchors.top: dateText.bottom
                width: parent.width
                height: parent.height / parent.children.length

                Connections {
                    target: _frequency

                    function onDisplayWorker(work) {
                        worker.worker = work
                    }
                }

                property Worker worker: null;
                text: qsTr("Progress") + ": "  + (worker ? worker.progress + "%" : "N/A")
            }
        }

        EntryEdit {
            id: linked
            objectName: "linkedDisplayer"

            Connections {
                target: _frequency

                function onDisplayLink(entry) {
                    linked.entry = entry
                }
            }

            Component.onCompleted: changeDirection()
            opening: true
            visible: entryList.currentIndex !== -1 && frequencyList.currentIndex !== -1

            Layout.preferredHeight: root.height * 0.45
            Layout.preferredWidth: root.width * 0.36
            Layout.row: 3
            Layout.column: 5
            Layout.columnSpan: 3
            Layout.rowSpan: 2
        }

        ListView {
            id: entryList
            objectName: "entryList"

            Layout.preferredHeight: root.height * 0.70
            Layout.preferredWidth: root.width * 0.36
            Layout.row: 2
            Layout.column: 2
            Layout.columnSpan: 3
            Layout.rowSpan: 2

            model: testModel
            clip: true

            signal s_display(var entryId)

            ToolTip.text: qsTr("Select generate transaction")
            ToolTip.delay: 500
            ToolTip.timeout: 1000
            ToolTip.visible: linkedArea.containsMouse
            
            MouseArea {
                id: linkedArea
                acceptedButtons: Qt.NoButton
                anchors.fill: parent
                hoverEnabled: true
            }

            property var enabledSection: []
            section.property: "freqGroup"
            section.criteria: ViewSection.FullString
            section.labelPositioning: ViewSection.InlineLabels
            section.delegate: Rectangle {
                gradient: AccountStyle.goldHeader
                width: ListView.view.width
                height: ListView.view.height * 0.10
                Text {
                    anchors.fill: parent
                    text: section
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: AccountStyle.title.name
                    font.pixelSize: AccountStyle.title.size2
                }

                Text {
                    id: arrow
                    anchors.right: parent.right
                    anchors.rightMargin: parent.width * 0.05
                    height: parent.height
                    text: ">"
                    rotation: 90
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: AccountStyle.title.name
                    font.pixelSize: AccountStyle.title.size2
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        entryList.enabledSection[section] = !entryList.enabledSection[section]

                        for(var j = 0; j < testModel.count; j++) {
                            if(testModel.get(j).freqGroup == section ) testModel.setProperty(j, "isVisible", entryList.enabledSection[section]);
                        }

                        arrow.rotation = arrow.rotation + 180
                    }
                }

                Component.onCompleted: entryList.enabledSection[section] = true
            }
            Rectangle {
                anchors.fill: parent
                border.color: "gold"
                color: "transparent"
            }

            highlight: Rectangle {
                gradient: AccountStyle.calSelect
            }

            highlightMoveDuration: 0
            delegate: ItemDelegate {
                objectName: "entryDel"
                background: AccountBackground {
                    property var index: parent.index
                    invisible: true
                }

                highlighted: ListView.isCurrentItem
                width: ListView.view.width
                height: isVisible ? ListView.view.height * 0.07 : 0
                visible: isVisible

                function f() {
                    _frequency.displayEntry(entryList.model.get(index).id)
                    ListView.view.currentIndex = index
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        parent.f()
                    }
                }

                AccountLabel {
                    anchors.fill: parent
                    text: Qt.formatDate(date, "dd-MM-yyyy")
                }
            }
        }

        PageChanger {
            id: pageChanger

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.36
            Layout.row: 4
            Layout.column: 2
            Layout.columnSpan: 3
            Layout.rowSpan: 1

            onPageChange: {
                testModel.clear()

                var i = pageIndex - 1
                if(frequencyList.currentModel) {
                    for(var j = 0 ; j < 100; j++) {
                        if(j+100*i < frequencyList.currentModel.entries().length) {
                            testModel.append(frequencyList.currentModel.entries()[j+100*i])
                        }
                    }
                    if(entryList.model.get(0))
                       entryList.s_display(entryList.model.get(0).id)
                }
            }
        }
    }
}/**/
