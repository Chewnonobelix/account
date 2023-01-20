import QtQuick 2.15
import QtQuick.Layouts 1.13

import QtQuick.Controls 2.15
import QtQuick.Window 2.13

import "../Style"
import "../Functionnal"
import "../Budget"

import Account 1.0

Page {


	implicitWidth: parent.width
	implicitHeight: parent.height

	id: pageTable

	background: AccountBackground {
		invisible: true
	}

	MouseArea {
		acceptedButtons: Qt.NoButton
		hoverEnabled: true

		onMouseXChanged: addingid.pX = (mouseX - pageTable.x) / pageTable.width
		onMouseYChanged: addingid.pY = (mouseY - pageTable.y) / pageTable.height
	}

	GridLayout {
		id: grid
		anchors.fill: parent
		rowSpacing: pageTable.height * 0.02
		columnSpacing: pageTable.width * 0.02
		anchors.topMargin: pageTable.height * 0.02
		anchors.bottomMargin: pageTable.height * 0.02
		anchors.rightMargin: pageTable.width * 0.02
		anchors.leftMargin: pageTable.width * 0.02


		ZoomCalendar {
			id: cal
			objectName: "cal"

			Layout.column: 0
			Layout.row: 0
			Layout.rowSpan: 1
			Layout.columnSpan: 2
			Layout.preferredHeight: pageTable.height * 0.30
			Layout.preferredWidth: pageTable.width * 0.20

            onUpdateSelected:  {
                _main.dateList = selectedDates
                _mainModel.dateList = selectedDates
                _mainModel.currentIndex = -1
                _main.updateQuickView()
            }

			onDatesChanged:  {
				_mainModel.currentIndex = -1
			}

			onMonthChanged: {
                _main.previewCalendar(currentMonth+1, currentYear)
			}

			Component.onCompleted: set(new Date())
			Connections {
				target: _main

				function onClearCalendar() {
					cal.clear()
				}

				function onAppendMonthPreview(val) {
					cal.addMonth(val)
				}

				function onAppendCalendarPreview(val) {
					cal.addCalendar(val)
				}
			}
		}

		AccountButton {
			id: add
			text: qsTr("Add")


			enabled: accountSelect.model.length > 0
			Layout.column: 0
			Layout.row: 1
			Layout.rowSpan: 1
			Layout.columnSpan: 1
			Layout.preferredHeight: pageTable.height * 0.05
			Layout.preferredWidth: pageTable.width * 0.09

			ToolTip.text: qsTr("Add new transaction")

			onClicked: {
				_main.add(false)
			}
		}

		AccountButton {
			id: remove
			text: qsTr("Remove")

			property int index: _mainModel ? _mainModel.currentIndex : -1
			enabled: currentEntry && !currentEntry.isBlocked
			Layout.column: 1
			Layout.row: 1
			Layout.rowSpan: 1
			Layout.columnSpan: 1
			Layout.preferredHeight: pageTable.height * 0.05
			Layout.preferredWidth: pageTable.width * 0.09

			ToolTip.text: qsTr("Remove select transaction")

			onClicked: {
				if (enabled) {
					_main.remove(pageTable.currentEntry.id)
				}
			}
		}

		Column {
			Layout.column: 0
			Layout.row: 2
			Layout.rowSpan: 1
			Layout.columnSpan: 2
			Layout.preferredHeight: pageTable.height * 0.57
			Layout.preferredWidth: pageTable.width * 0.20
			spacing: pageTable.height * .02

			AccountHeader {
				id: quickViewDate
				objectName: "quickViewDate"
				property date currentDate: new Date()

				text: qsTr("Budget quick view") + ": " + Qt.formatDate(currentDate, "dd-MM-yyyy")
				height: parent.height * .10
				width: parent.width
				clip: true
			}

			Loader {
				id: loadQuick
				Connections {
					target: _main

					function onEnableQuickView(enable) {
						loadQuick.active = true
					}
				}

				active: false
				width: parent.width
				height: parent.height * .86

				onActiveChanged: {
					item.width = width
					item.height = height
				}
				source: "../Budget/BudgetView.qml"
			}
		}

		Column {
			id: colTable
			Layout.column: 2
			Layout.row: 0
			Layout.rowSpan: 3
			Layout.columnSpan: 1

			Layout.preferredHeight: pageTable.height * 0.96
			Layout.preferredWidth: pageTable.width * 0.20
			spacing: 0

			HorizontalHeaderView {
				id: headerView
				syncView: view
				z:5
				clip: true
                implicitHeight: colTable.height * 0.05
                delegate: Rectangle {
                    border.color: "goldenrod"
					AccountHeader {
						anchors {
							fill: parent
							margins: 1
						}

						text: display
						clip: true

						MouseArea {
							anchors.fill: parent
							onClicked: {
								if(_mainModel.columnSort !== index)
									_mainModel.columnSort = index
								else
									_mainModel.sort = _mainModel.sort === Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder
							}
						}
					}
				}
			}

			TableView {

				height: parent.height * 0.85
				width: parent.width
				id: view
				model: _mainModel
				clip: true
				reuseItems: true
				Connections {
					target: _main

					function onCurrentRowChanged(index) {
						_mainModel.currentIndex = index
						view.viewEntry()
					}
				}

				function viewEntry() {
					currentEntry = _mainModel.currentIndex > -1 ? _mainModel.at(_mainModel.currentIndex) : null

					if(currentEntry) {
						_main.edit(currentEntry.id)
						infoView.visible = !currentEntry.isBlocked
					}
					else {
						infoView.visible = false
					}
				}

				property var columns: [width*0.10,
					width*0.23,
					width*0.23,
					width*0.22,
					width*0.22]

				onHeightChanged: forceLayout()
				rowHeightProvider: function(row) {
					return height * 0.05
				}

				columnWidthProvider: function(column) {
					return columns[column]
				}

				rowSpacing: height* 0.02
				delegate: AccountLabel {
					required property var type

					clip: true
					Component.onCompleted: {
					}

					function isSelect() {
						return _mainModel ? _mainModel.currentIndex === row : false
					}

					MouseArea {
						anchors.fill: parent
						onClicked: {
							_mainModel.currentIndex = isSelect() ? -1 : row
							view.viewEntry()
						}
					}


					text: _mainModel ? _mainModel.at(row,column) : ""


					background: Rectangle {
						anchors.fill: parent
						gradient: isSelect() ? type == Account.TypeEnum.Income ? AccountStyle.selectViewIn : AccountStyle.selectViewOut : AccountStyle.unselectView
					}
				}
			}

			PageChanger {
				id: changer
				objectName: "pageSkip"
				height: parent.height * 0.05
				width: parent.width

				Connections {
					target: _main

					function onMaxPageChanged(max) {
						changer.maxPage = max
					}
				}

				onPageChange:  {
					view.unselectAll()
					_main.currentPage = pageIndex
					_main.pageChange()
				}
			}
		}

		ScrollView {
			Layout.column: 3
			Layout.row: 0
			Layout.rowSpan: 3
			Layout.columnSpan: 1

			Layout.preferredWidth: pageTable.width * 0.52
			Layout.preferredHeight: pageTable.height * .96
			ScrollBar.horizontal.policy: ScrollBar.AsNeeded

			clip: true
			InformationView {
				id: infoView
				objectName: "infoView"
				clip: true
				enabled: visible
				visible: false
				implicitWidth: Screen.width * 0.52
				implicitHeight: grid.height
			}
		}

	}


	/* */
	Adding {
		id: addingid
		objectName: "addingid"
		y: pY > 0 ? (mainWindow.height * pY) - Math.max(0, mY) : (parent.height / 2 + add.height / 2 + 10)
		x: pY > 0 ? (mainWindow.width * pX) - Math.max(0, mX) : (add.width / 2)
		property double pX: -1
		property double pY: -1
		property double mX: ((mainWindow.width * pX) + width) - mainWindow.width
		property double mY: ((mainWindow.height * pY) + height) - (mainWindow.height)

		implicitWidth: parent.width * .35
		implicitHeight: parent.height * .12

		Component.onCompleted: {
			reset()
		}

		onOpened: {
			openDate = cal.selectedDates.length === 0 ? new Date() : Date.fromLocaleString(Qt.locale(), cal.selectedDates[0], "dd-MM-yyyy")
		}
	}


	Component.onCompleted: {
		_mainModel.currentIndex = -1
		_main.exec()
	}

	property var currentEntry: null
}
