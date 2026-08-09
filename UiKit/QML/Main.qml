import QtQuick
import QtQuick.Controls

import OpenAccount.Model

import "../../View/QML/Style" as Style
import "../../View/QML/Components" as Comp
import "../../View/QML/ComposedComponent" as Composed

ApplicationWindow {
    id: window

    component OBTabButton: TabButton {
        id: tabButton

        implicitWidth: Style.OBConstants.widthMedium
        implicitHeight: Style.OBConstants.heightMedium

        background: Rectangle {
            radius: Style.OBConstants.borderRadius
            border.width: Style.OBConstants.borderWidth
            border.color: !tabButton.enabled ? Style.OBTheme.palette.disabled :
                          (tabButton.checked || tabButton.down) ? Style.OBTheme.palette.outlineStrong : Style.OBTheme.palette.outline
            gradient: !tabButton.enabled ? Style.OBStyle.silver :
                      (tabButton.checked || tabButton.down) ? Style.OBStyle.goldIn : Style.OBStyle.goldOut
        }

        contentItem: Comp.OBLabel {
            text: tabButton.text
            horizontalAlignment: Text.AlignHCenter
            textState: tabButton.enabled ? Comp.OBLabel.TextState.Neutral : Comp.OBLabel.TextState.Disabled
        }
    }

    width: 560
    height: 820
    visible: true
    title: "OpenAccount — UiKit"
    color: Style.OBTheme.palette.backgroundStart

    Row {
        id: headerRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.OBConstants.leftMargins
        height: Style.OBConstants.heightBig
        spacing: Style.OBConstants.horizontalSpacing

        Comp.OBLabel {
            text: "OpenAccount UiKit"
            font.bold: true
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
            width: headerRow.width - themeRow.width - headerRow.spacing
        }

        Row {
            id: themeRow
            anchors.verticalCenter: parent.verticalCenter
            spacing: Style.OBConstants.horizontalSpacing / 2

            Comp.OBLabel {
                text: "Night mode"
                anchors.verticalCenter: parent.verticalCenter
            }

            Comp.OBSwitch {
                id: themeSwitch
                anchors.verticalCenter: parent.verticalCenter
                checked: Style.OBTheme.isNight
                onCheckedChanged: Style.OBTheme.mode = checked ? Style.OBTheme.Mode.Night : Style.OBTheme.Mode.Day
            }
        }
    }

    TabBar {
        id: tabBar
        anchors.top: headerRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.OBConstants.leftMargins

        OBTabButton {
            text: "Components"
        }
        OBTabButton {
            text: "Complex components"
        }
        OBTabButton {
            text: "Calendar"
        }
    }

    ScrollView {
        id: componentsTab
        visible: tabBar.currentIndex === 0
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        // ScrollView's own style only anchors vertical-to-right/full-height
        // and horizontal-to-bottom/full-width for the *default* scrollbars
        // it declares internally; supplying our own via the attached
        // properties opts out of that layout, so it's replicated here.
        ScrollBar.vertical: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: componentsTab
            x: componentsTab.mirrored ? 0 : componentsTab.width - width
            y: componentsTab.topPadding
            height: componentsTab.availableHeight
            active: componentsTab.ScrollBar.horizontal.active
        }

        ScrollBar.horizontal: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: componentsTab
            x: componentsTab.leftPadding
            y: componentsTab.height - height
            width: componentsTab.availableWidth
            active: componentsTab.ScrollBar.vertical.active
        }

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins - Style.OBConstants.scrollBarThickness
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                title: "OBButton"
                target: demoButton
                availableWidth: parent.width

                Comp.OBButton {
                    id: demoButton
                    text: buttonTextInput.text
                }
                Comp.OBTextInput {
                    id: buttonTextInput
                    text: "Click me"
                }
                Comp.OBButton {
                    text: "With icon"
                    icon.source: "data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24'><circle cx='12' cy='12' r='10' fill='white'/></svg>"
                }
            }

            UiKitSection {
                title: "OBLabel"
                target: demoLabel
                availableWidth: parent.width

                Comp.OBLabel {
                    id: demoLabel
                    text: labelTextInput.text
                    textState: labelStateBox.currentIndex
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBTextInput {
                    id: labelTextInput
                    text: "Sample label"
                }
                Comp.OBComboBox {
                    id: labelStateBox
                    model: ["Neutral", "Positive", "Negative", "Disabled"]
                }
            }

            UiKitSection {
                title: "OBLabel states"
                availableWidth: parent.width

                Comp.OBLabel {
                    text: "Neutral"
                    textState: Comp.OBLabel.TextState.Neutral
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: "Positive"
                    textState: Comp.OBLabel.TextState.Positive
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: "Negative"
                    textState: Comp.OBLabel.TextState.Negative
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: "Disabled"
                    textState: Comp.OBLabel.TextState.Disabled
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBHeader"
                target: demoHeader
                availableWidth: parent.width

                Comp.OBHeader {
                    id: demoHeader
                    text: headerTextInput.text
                    width: Style.OBConstants.widthBig
                    orientation: headerOrientationBox.currentIndex === 0 ? Gradient.Vertical : Gradient.Horizontal
                    reversed: headerReversedToggle.checked
                }
                Comp.OBTextInput {
                    id: headerTextInput
                    text: "Section title"
                }
                Comp.OBComboBox {
                    id: headerOrientationBox
                    model: ["Vertical", "Horizontal"]
                }
                Comp.OBCheckBox {
                    id: headerReversedToggle
                    text: "reversed"
                    checked: false
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBTitle"
                target: demoTitle
                availableWidth: parent.width

                Comp.OBTitle {
                    id: demoTitle
                    text: titleTextInput.text
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBTextInput {
                    id: titleTextInput
                    text: "Section title"
                }
            }

            UiKitSection {
                title: "OBTextInput"
                target: demoInput
                availableWidth: parent.width

                Comp.OBTextInput {
                    id: demoInput
                    placeholderText: "Type here"
                }
                Comp.OBLabel {
                    text: "-> " + demoInput.text
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBSpinBox (real numbers)"
                target: demoSpin
                availableWidth: parent.width

                Comp.OBSpinBox {
                    id: demoSpin
                    decimals: 2
                    realFrom: 0
                    realTo: 100
                    realStepSize: 0.5
                    realValue: 10
                }
                Comp.OBLabel {
                    text: "realValue = " + demoSpin.realValue.toFixed(2)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBSwitch (tristate)"
                target: demoSwitch
                availableWidth: parent.width

                Comp.OBSwitch {
                    id: demoSwitch
                    tristate: switchTristateToggle.checked
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBCheckBox {
                    id: switchTristateToggle
                    text: "tristate"
                    checked: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: demoSwitch.checkState === Qt.Unchecked ? "Unchecked" :
                          demoSwitch.checkState === Qt.PartiallyChecked ? "Partial" : "Checked"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBCheckBox"
                target: demoCheck
                availableWidth: parent.width

                Comp.OBCheckBox {
                    id: demoCheck
                    text: "Accept terms"
                    tristate: checkTristateToggle.checked
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBCheckBox {
                    id: checkTristateToggle
                    text: "tristate"
                    checked: false
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: demoCheck.checkState === Qt.Unchecked ? "Unchecked" :
                          demoCheck.checkState === Qt.PartiallyChecked ? "Partial" : "Checked"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBRadioButton"
                target: radioA
                availableWidth: parent.width

                ButtonGroup {
                    id: radioGroup
                    buttons: [radioA, radioB, radioC]
                }

                Comp.OBRadioButton {
                    id: radioA
                    text: "Option A"
                    checked: true
                }
                Comp.OBRadioButton {
                    id: radioB
                    text: "Option B"
                }
                Comp.OBRadioButton {
                    id: radioC
                    text: "Option C"
                }
            }

            UiKitSection {
                title: "OBComboBox"
                target: demoCombo
                availableWidth: parent.width

                Comp.OBComboBox {
                    id: demoCombo
                    model: ["Checking", "Savings", "Credit card", "Cash"]
                }
                Comp.OBLabel {
                    text: "-> " + demoCombo.displayText
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBComboBox orientation"
                target: orientationCombo
                availableWidth: parent.width

                Comp.OBComboBox {
                    id: orientationCombo
                    model: ["Checking", "Savings", "Credit card", "Cash"]
                    popupOrientation: orientationBox.currentIndex === 0 ? Comp.OBComboBox.PopupOrientation.Vertical : Comp.OBComboBox.PopupOrientation.Horizontal
                }
                Comp.OBComboBox {
                    id: orientationBox
                    model: ["Vertical", "Horizontal"]
                }
            }

            UiKitSection {
                title: "OBComboBox + MovementModel"
                target: movementCombo
                availableWidth: parent.width

                Comp.OBComboBox {
                    id: movementCombo
                    model: MovementModel {}
                    currentIndex: 0
                }
                Comp.OBLabel {
                    text: "-> " + movementCombo.model.valueAt(movementCombo.currentIndex)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBComboBox + SupportModel"
                target: supportCombo
                availableWidth: parent.width

                Comp.OBComboBox {
                    id: supportCombo
                    model: SupportModel {}
                    currentIndex: 0
                }
                Comp.OBLabel {
                    text: "-> " + supportCombo.model.valueAt(supportCombo.currentIndex)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBComboBox + FrequencyModel"
                target: frequencyCombo
                availableWidth: parent.width

                Comp.OBComboBox {
                    id: frequencyCombo
                    model: FrequencyModel {}
                    currentIndex: 0
                }
                Comp.OBLabel {
                    text: "-> " + frequencyCombo.model.valueAt(frequencyCombo.currentIndex)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBScrollBar"
                target: demoScrollBar
                availableWidth: parent.width

                Flickable {
                    id: demoFlickable
                    width: Style.OBConstants.widthBig
                    height: Style.OBConstants.heightBig * 2
                    contentWidth: width
                    contentHeight: scrollContent.height
                    clip: true

                    ScrollBar.vertical: Comp.OBScrollBar {
                        id: demoScrollBar
                        policy: ScrollBar.AlwaysOn
                    }

                    Column {
                        id: scrollContent
                        width: demoFlickable.width

                        Repeater {
                            model: 10
                            Comp.OBLabel {
                                text: "Row " + (index + 1)
                                width: scrollContent.width
                                height: Style.OBConstants.heightLittle
                            }
                        }
                    }
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }

    ScrollView {
        id: complexComponentsTab
        visible: tabBar.currentIndex === 1
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        // ScrollView's own style only anchors vertical-to-right/full-height
        // and horizontal-to-bottom/full-width for the *default* scrollbars
        // it declares internally; supplying our own via the attached
        // properties opts out of that layout, so it's replicated here.
        ScrollBar.vertical: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: complexComponentsTab
            x: complexComponentsTab.mirrored ? 0 : complexComponentsTab.width - width
            y: complexComponentsTab.topPadding
            height: complexComponentsTab.availableHeight
            active: complexComponentsTab.ScrollBar.horizontal.active
        }

        ScrollBar.horizontal: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: complexComponentsTab
            x: complexComponentsTab.leftPadding
            y: complexComponentsTab.height - height
            width: complexComponentsTab.availableWidth
            active: complexComponentsTab.ScrollBar.vertical.active
        }

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins - Style.OBConstants.scrollBarThickness
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                title: "DescLine"
                availableWidth: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.DescLine {
                        title: "Account name"
                        contentType: Composed.DescLine.ContentType.TextInput
                        contentConfig: ({ "text": "Checking" })
                    }

                    Composed.DescLine {
                        title: "Active"
                        contentType: Composed.DescLine.ContentType.CheckBox
                        contentConfig: ({ "checked": true })
                    }

                    Composed.DescLine {
                        title: "Balance"
                        contentType: Composed.DescLine.ContentType.SpinBox
                        contentConfig: ({ "realTo": 1000, "realValue": 250 })
                    }

                    Composed.DescLine {
                        title: "Status"
                        contentType: Composed.DescLine.ContentType.Label
                        contentConfig: ({ "text": "Ok", "textState": Comp.OBLabel.TextState.Positive })
                    }

                    Composed.DescLine {
                        title: "Type"
                        contentType: Composed.DescLine.ContentType.ComboBox
                        contentConfig: ({ "model": ["Checking", "Savings", "Credit card", "Cash"] })
                    }

                    // Explicit-content mode: any custom Item works as the value slot too.
                    Composed.DescLine {
                        title: "Custom content"

                        Row {
                            spacing: Style.OBConstants.horizontalSpacing / 2

                            Comp.OBButton {
                                text: "Edit"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }

            UiKitSection {
                title: "TableRow"
                availableWidth: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Groceries — Neutral"
                        selected: true
                        rowState: Composed.TableRow.RowState.Neutral
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Salary — Positive"
                        selected: true
                        rowState: Composed.TableRow.RowState.Positive
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Overdraft fee — Negative"
                        selected: true
                        rowState: Composed.TableRow.RowState.Negative
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Tap me to select"
                    }
                }
            }

            UiKitSection {
                title: "TransactionRow"
                availableWidth: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.TransactionRow {
                        transaction: Transaction {
                            name: "Salary"
                            description: "Monthly salary"
                            date: "2026-01-05"
                            value: 2500
                            movement: OpenAccountEnums.Movement.Credit
                        }
                    }

                    Composed.TransactionRow {
                        transaction: Transaction {
                            name: "Rent"
                            description: "Monthly rent"
                            date: "2026-01-06"
                            value: 950
                            movement: OpenAccountEnums.Movement.Debit
                        }
                    }
                }
            }

            UiKitSection {
                title: "OBTransactionInfo"
                availableWidth: parent.width

                CategoryListModel {
                    id: transactionInfoCategoryModel

                    Component.onCompleted: {
                        const salary = categoryPrototype.createObject(transactionInfoCategoryModel)
                        salary.setId("{11111111-1111-1111-1111-111111111111}")
                        salary.name = "Salary"
                        salary.direction = OpenAccountEnums.Movement.Credit
                        addCategory(salary)

                        const groceries = categoryPrototype.createObject(transactionInfoCategoryModel)
                        groceries.setId("{22222222-2222-2222-2222-222222222222}")
                        groceries.name = "Groceries"
                        groceries.direction = OpenAccountEnums.Movement.Debit
                        addCategory(groceries)
                    }
                }
                Component {
                    id: categoryPrototype
                    Category {}
                }

                Row {
                    spacing: Style.OBConstants.horizontalSpacing

                    Composed.OBTransactionInfo {
                        readOnly: !transactionInfoEditToggle.checked
                        categoryModel: transactionInfoCategoryModel
                        transaction: Transaction {
                            name: "Salary"
                            description: "Monthly salary"
                            date: "2026-01-05"
                            value: 2500
                            movement: OpenAccountEnums.Movement.Credit
                            support: OpenAccountEnums.Support.Transfer
                            // QUuid isn't one of QML's built-in value types, so
                            // it can't be set via a static property assignment
                            // (unlike QDate above) — only via a method call.
                            Component.onCompleted: setCategory("{11111111-1111-1111-1111-111111111111}")
                        }
                    }

                    Comp.OBCheckBox {
                        id: transactionInfoEditToggle
                        text: "editable"
                        checked: false
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            UiKitSection {
                title: "OBTransactionList"
                availableWidth: parent.width

                TransactionListModel {
                    id: transactionListDemoModel

                    Component.onCompleted: {
                        addTransaction(transactionPrototype.createObject(transactionListDemoModel, {
                            name: "Salary", description: "Monthly salary", date: new Date("2026-01-05"),
                            value: 2500, movement: OpenAccountEnums.Movement.Credit
                        }))
                        addTransaction(transactionPrototype.createObject(transactionListDemoModel, {
                            name: "Rent", description: "Monthly rent", date: new Date("2026-01-06"),
                            value: 950, movement: OpenAccountEnums.Movement.Debit
                        }))
                        addTransaction(transactionPrototype.createObject(transactionListDemoModel, {
                            name: "Groceries", description: "Weekly groceries", date: new Date("2026-01-08"),
                            value: 120, movement: OpenAccountEnums.Movement.Debit
                        }))
                        addTransaction(transactionPrototype.createObject(transactionListDemoModel, {
                            name: "Freelance", description: "Side project payout", date: new Date("2026-01-10"),
                            value: 400, movement: OpenAccountEnums.Movement.Credit
                        }))
                    }
                }
                Component {
                    id: transactionPrototype
                    Transaction {}
                }

                AccountTransactionFilterProxyModel {
                    id: transactionListDemoProxy
                    sourceModel: transactionListDemoModel
                }

                Composed.OBTransactionList {
                    model: transactionListDemoProxy
                    // Real usage defaults to 100/page; shrunk here so the
                    // gallery's 4 demo transactions actually span 2 pages
                    // and the Pager is visibly exercised.
                    pageSize: 2
                }
            }

            UiKitSection {
                title: "Pager"
                target: demoPager
                availableWidth: parent.width

                Composed.Pager {
                    id: demoPager
                    maxPage: pagerMaxSpin.realValue
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBSpinBox {
                    id: pagerMaxSpin
                    decimals: 0
                    realFrom: 1
                    realTo: 20
                    realValue: 5
                }
            }

            UiKitSection {
                id: counterSection
                title: "OBButtonPair (+/-)"
                target: counterPair
                availableWidth: parent.width

                property int counterValue: 0

                Composed.OBButtonPair {
                    id: counterPair
                    leftText: "-"
                    rightText: "+"
                    anchors.verticalCenter: parent.verticalCenter
                    onLeftClicked: counterSection.counterValue -= 1
                    onRightClicked: counterSection.counterValue += 1
                }
                Comp.OBLabel {
                    text: "value = " + counterSection.counterValue
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                id: confirmSection
                title: "OBButtonPair (accept/cancel)"
                target: confirmPair
                availableWidth: parent.width

                property string lastAction: "none"

                Composed.OBButtonPair {
                    id: confirmPair
                    leftText: "Cancel"
                    rightText: "Accept"
                    anchors.verticalCenter: parent.verticalCenter
                    onLeftClicked: confirmSection.lastAction = "cancelled"
                    onRightClicked: confirmSection.lastAction = "accepted"
                }
                Comp.OBLabel {
                    text: "last action: " + confirmSection.lastAction
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }

    ScrollView {
        id: calendarTab
        visible: tabBar.currentIndex === 2
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        // ScrollView's own style only anchors vertical-to-right/full-height
        // and horizontal-to-bottom/full-width for the *default* scrollbars
        // it declares internally; supplying our own via the attached
        // properties opts out of that layout, so it's replicated here.
        ScrollBar.vertical: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: calendarTab
            x: calendarTab.mirrored ? 0 : calendarTab.width - width
            y: calendarTab.topPadding
            height: calendarTab.availableHeight
            active: calendarTab.ScrollBar.horizontal.active
        }

        ScrollBar.horizontal: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: calendarTab
            x: calendarTab.leftPadding
            y: calendarTab.height - height
            width: calendarTab.availableWidth
            active: calendarTab.ScrollBar.vertical.active
        }

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins - Style.OBConstants.scrollBarThickness
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                title: "OBCalendar"
                availableWidth: parent.width

                Composed.OBCalendar {
                    id: demoCalendar
                    anchors.verticalCenter: parent.verticalCenter
                    // Demo data: alternating daily gain/loss, and a running
                    // total that trends up in the first half of the month
                    // and down in the second half.
                    dailyTotal: function (date) { return date.getDate() % 3 === 0 ? -40 : 60 }
                    cumulativeTotal: function (date) { return 15 - date.getDate() }
                }
                Comp.OBLabel {
                    text: "selected: " + (demoCalendar.selectedDates.length > 0 ?
                              demoCalendar.selectedDates.map(function (d) { return Qt.formatDate(d, "yyyy-MM-dd") }).join(", ") :
                              "none")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBDateButton"
                target: demoDateButton
                availableWidth: parent.width

                Composed.OBDateButton {
                    id: demoDateButton
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: "selected: " + (demoDateButton.hasDate ? Qt.formatDate(demoDateButton.selectedDate, "yyyy-MM-dd") : "none")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }
}
