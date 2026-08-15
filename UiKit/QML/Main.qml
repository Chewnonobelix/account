import QtQuick
import QtQuick.Controls

import OpenAccount.Model
import OpenAccount.UiKit

import "../../View/QML/Style" as Style
import "../../View/QML/Components" as Comp
import "../../View/QML/ComposedComponent" as Composed
import "../../View/QML/Popup" as Popup
import "../../View/QML/Page" as Page

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

    // ------------------------------------------------------------------
    // Shared demo dataset: one set of C++ list models + a filter proxy,
    // instantiated once here and referenced from every tab below instead
    // of each section building its own disposable data. Editing it in one
    // place — e.g. adding a category via OBAddCategoryPopup in the Popups
    // tab, or picking an account in the OBTransactionList filter — is
    // immediately visible everywhere else that reads it (the Category
    // combo in OBTransactionInfo/Budget page, the Details table on both
    // OBTransactionList and the Budget page, ...).
    // ------------------------------------------------------------------

    // Business objects created from the popups need an id but nothing in
    // this app can mint a real QUuid from QML (see OBAddCategoryPopup);
    // this stands in for that until the Controller layer exposes one.
    property int nextDemoSuffix: 100
    function nextDemoId() {
        window.nextDemoSuffix += 1
        return "{00000000-0000-0000-0000-" + String(window.nextDemoSuffix).padStart(12, "0") + "}"
    }

    CategoryListModel { id: categoryModel }
    Component { id: categoryPrototype; Category {} }

    AccountListModel { id: accountModel }
    Component { id: accountPrototype; Account {} }

    ProfileListModel { id: profileModel }
    Component { id: profilePrototype; Profile {} }

    TransactionListModel { id: transactionModel }
    Component { id: transactionPrototype; Transaction {} }

    BudgetListModel { id: budgetModel }
    Component { id: budgetPrototype; Budget {} }

    // Bulk-fills the same shared models with a large randomized dataset —
    // see UiKit/src/demodatagenerator.{h,cpp}. Toggled from the "Massive
    // dataset" switch below, so every section of the gallery can be
    // exercised against thousands of transactions instead of the handful
    // curated ones seeded by loadCuratedDataset().
    DemoDataGenerator { id: demoDataGenerator }

    // Narrows transactionModel down to one account; the "Account filter"
    // combo in the OBTransactionList section drives it, and the Budget
    // page's Details table reuses this same proxy instance, so picking an
    // account there is visible in both tabs at once.
    AccountTransactionFilterProxyModel {
        id: accountTransactionProxy
        sourceModel: transactionModel
    }

    // The gallery's original hand-curated dataset: a handful of readable,
    // fixed-id rows well suited to demoing individual components and the
    // add-category/profile/account popups. Re-runnable: clears every
    // model first so it can also be switched back to after
    // loadMassiveDataset() below.
    function loadCuratedDataset() {
        accountModel.clear()
        categoryModel.clear()
        profileModel.clear()
        transactionModel.clear()
        budgetModel.clear()

        const checking = accountPrototype.createObject(accountModel)
        checking.setId("{a0000000-0000-0000-0000-000000000001}")
        checking.number = "Checking"
        checking.bank = "BNP Paribas"
        checking.interest = 0
        checking.description = "Everyday account"
        checking.opening = new Date(2024, 0, 1)
        accountModel.addAccount(checking)

        const savings = accountPrototype.createObject(accountModel)
        savings.setId("{a0000000-0000-0000-0000-000000000002}")
        savings.number = "Savings"
        savings.bank = "BNP Paribas"
        savings.interest = 2.5
        savings.description = "Rainy day fund"
        savings.opening = new Date(2023, 5, 15)
        accountModel.addAccount(savings)

        const salary = categoryPrototype.createObject(categoryModel)
        salary.setId("{c0000000-0000-0000-0000-000000000001}")
        salary.name = "Salary"
        salary.direction = OpenAccountEnums.Movement.Credit
        categoryModel.addCategory(salary)

        const freelance = categoryPrototype.createObject(categoryModel)
        freelance.setId("{c0000000-0000-0000-0000-000000000002}")
        freelance.name = "Freelance"
        freelance.direction = OpenAccountEnums.Movement.Credit
        categoryModel.addCategory(freelance)

        const groceries = categoryPrototype.createObject(categoryModel)
        groceries.setId("{c0000000-0000-0000-0000-000000000003}")
        groceries.name = "Groceries"
        groceries.direction = OpenAccountEnums.Movement.Debit
        categoryModel.addCategory(groceries)

        const rent = categoryPrototype.createObject(categoryModel)
        rent.setId("{c0000000-0000-0000-0000-000000000004}")
        rent.name = "Rent"
        rent.direction = OpenAccountEnums.Movement.Debit
        categoryModel.addCategory(rent)

        const alice = profilePrototype.createObject(profileModel)
        alice.setId("{p0000000-0000-0000-0000-000000000001}")
        alice.firstName = "Alice"
        alice.lastName = "Martin"
        alice.accounts = [checking.id, savings.id]
        profileModel.addProfile(alice)

        function addTx(name, description, date, value, movement, accountId, categoryId, estimated) {
            const tx = transactionPrototype.createObject(transactionModel, {
                name: name, description: description, date: date,
                value: value, movement: movement
            })
            tx.accountId = accountId
            tx.category = categoryId
            tx.estimated = estimated === true
            transactionModel.addTransaction(tx)
            return tx
        }

        addTx("Salary", "Monthly salary", new Date("2026-01-05"), 2500, OpenAccountEnums.Movement.Credit, checking.id, salary.id)
        addTx("Rent", "Monthly rent", new Date("2026-01-06"), 950, OpenAccountEnums.Movement.Debit, checking.id, rent.id)
        addTx("Groceries", "Weekly groceries", new Date("2026-01-08"), 120, OpenAccountEnums.Movement.Debit, checking.id, groceries.id)
        addTx("Freelance", "Side project payout", new Date("2026-01-10"), 400, OpenAccountEnums.Movement.Credit, checking.id, freelance.id)
        addTx("Interest", "Savings interest", new Date("2026-01-15"), 12.5, OpenAccountEnums.Movement.Credit, savings.id, salary.id)
        addTx("Transfer to savings", "Monthly top-up", new Date("2026-01-16"), 300, OpenAccountEnums.Movement.Debit, checking.id, rent.id)
        // Falls inside Budget 1's [10/01, 17/01] window (see below), unlike
        // the "Groceries" transaction above (08/01) — demonstrates the
        // Budget page's Details table actually scoping to the selected
        // budget's category + period instead of showing every transaction.
        addTx("Groceries", "Week 2 groceries", new Date("2026-01-12"), 45, OpenAccountEnums.Movement.Debit, checking.id, groceries.id)

        // Predicted recurring bills, generated ahead of time and not yet
        // confirmed — demos OBValidateTransactionsPopup, which only lists
        // estimated transactions whose date has already passed. The last
        // one is dated in the future on purpose: it must NOT show up in
        // the popup, demonstrating that date cutoff.
        addTx("Internet bill", "ISP monthly estimate", new Date("2026-01-20"), 45, OpenAccountEnums.Movement.Debit, checking.id, rent.id, true)
        addTx("Phone bill", "Mobile plan estimate", new Date("2026-01-22"), 25, OpenAccountEnums.Movement.Debit, checking.id, rent.id, true)
        addTx("Insurance", "Home insurance estimate", new Date("2026-01-25"), 60, OpenAccountEnums.Movement.Debit, checking.id, rent.id, true)
        addTx("Insurance", "Next year's estimate", new Date("2027-01-25"), 65, OpenAccountEnums.Movement.Debit, checking.id, rent.id, true)

        for (let i = 1; i <= 4; ++i) {
            const budget = budgetPrototype.createObject(budgetModel)
            budget.setId("{b0000000-0000-0000-0000-00000000000" + i + "}")
            budget.name = "Budget " + i
            budget.categoryId = groceries.id
            // Budget 1's threshold (30) sits below the 45 spent on
            // "Week 2 groceries" above, so it demos the Summary panel's
            // "Over budget" state out of the box; Budgets 2-4 (100/200/300)
            // stay under it, demoing "Within budget".
            budget.threshold = i === 1 ? 30 : 100 * i
            budget.startDate = new Date(2026, 0, 10)
            budget.endDate = new Date(2026, 0, 17)
            budget.recurrence = OpenAccountEnums.Frequency.Weekly
            budgetModel.addBudget(budget)
        }

        accountTransactionProxy.account = checking
    }

    // Replaces every shared model's contents with a large randomized
    // dataset (see DemoDataGenerator), so components that only reveal
    // their real weight under load — the Pager depth on
    // OBTransactionList, the Budget page's Details table, the account/
    // category OBComboBox filters, ... — can be exercised and profiled.
    // Re-runnable, and safe to switch back from via loadCuratedDataset().
    // Runs on a background thread: generate() returns immediately, and
    // accountTransactionProxy.account is only reassigned once the
    // "finished" handler below fires with the models actually populated.
    function loadMassiveDataset() {
        demoDataGenerator.generate(accountModel, categoryModel, profileModel, transactionModel, budgetModel)
    }

    Connections {
        target: demoDataGenerator
        function onFinished() {
            accountTransactionProxy.account = accountModel.count > 0 ? accountModel.at(0) : null
        }
    }

    Component.onCompleted: loadCuratedDataset()

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

    Row {
        id: datasetRow
        anchors.top: headerRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.OBConstants.leftMargins
        height: Style.OBConstants.heightMedium
        spacing: Style.OBConstants.horizontalSpacing / 2

        Comp.OBLabel {
            text: "Massive dataset"
            anchors.verticalCenter: parent.verticalCenter
        }

        // Swaps every shared model between the small hand-curated dataset
        // (readable, fixed ids — good for the popups and single-component
        // sections) and DemoDataGenerator's large randomized one (good for
        // perf-testing the Pager, the Budget page's Details table, ...).
        Comp.OBSwitch {
            id: massiveDatasetSwitch
            anchors.verticalCenter: parent.verticalCenter
            checked: false
            // Disabled mid-run: flipping back to the curated dataset while
            // the worker thread is still filling the models out from under
            // it would race loadCuratedDataset()'s clear() calls.
            enabled: !demoDataGenerator.running
            onCheckedChanged: checked ? loadMassiveDataset() : loadCuratedDataset()
        }

        Comp.OBLabel {
            // While generating, mirrors DemoDataGenerator's progress
            // (also logged through the "uikit.demodata" category on the
            // console); otherwise shows what's actually loaded right now.
            text: demoDataGenerator.running ? demoDataGenerator.progressText + " (" + demoDataGenerator.progressPercent + "%)" :
                  accountModel.count + " accounts, " + categoryModel.count + " categories, " +
                  profileModel.count + " profiles, " + transactionModel.count + " transactions, " +
                  budgetModel.count + " budgets"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    TabBar {
        id: tabBar
        anchors.top: datasetRow.bottom
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
        OBTabButton {
            text: "Popups"
        }
        OBTabButton {
            text: "Page"
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

                Row {
                    spacing: Style.OBConstants.horizontalSpacing

                    Composed.OBTransactionInfo {
                        readOnly: !transactionInfoEditToggle.checked
                        categoryModel: categoryModel
                        // First transaction of the shared dataset — editing
                        // it here (in editable mode) changes the same
                        // Transaction object shown in OBTransactionList/the
                        // Budget page below, since list models hand out
                        // live references, not copies.
                        transaction: transactionModel.count > 0 ? transactionModel.at(0) : null
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

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Row {
                        spacing: Style.OBConstants.horizontalSpacing

                        Comp.OBLabel {
                            text: "Account filter"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Comp.OBComboBox {
                            id: accountFilterCombo
                            model: accountModel
                            textRole: "number"
                            currentIndex: accountModel.indexOf(accountTransactionProxy.account ? accountTransactionProxy.account.id : "")
                            onActivated: function (index) { accountTransactionProxy.account = accountModel.at(index) }
                        }
                    }

                    Composed.OBTransactionList {
                        model: accountTransactionProxy
                        // Real usage defaults to 100/page; shrunk here so the
                        // gallery's demo transactions actually span several
                        // pages and the Pager is visibly exercised.
                        pageSize: 50
                    }
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

            UiKitSection {
                title: "OBBudgetToday"
                availableWidth: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Row {
                        spacing: Style.OBConstants.horizontalSpacing

                        Comp.OBLabel {
                            text: "Date checked"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Composed.OBDateButton {
                            id: budgetTodayDateButton
                            anchors.verticalCenter: parent.verticalCenter
                            // Falls inside Budget 1's window (see
                            // loadCuratedDataset()) so the section below
                            // shows real entries out of the box; leaving
                            // OBBudgetToday.dates unset instead defaults to
                            // today, which the curated budgets don't cover.
                            selectedDate: new Date(2026, 0, 15)
                        }
                    }

                    Composed.OBBudgetToday {
                        width: implicitWidth
                        budgetModel: budgetModel
                        transactionModel: accountTransactionProxy
                        dates: budgetTodayDateButton.hasDate ? [budgetTodayDateButton.selectedDate] : [new Date()]
                    }
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

    ScrollView {
        id: popupsTab
        visible: tabBar.currentIndex === 3
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
            parent: popupsTab
            x: popupsTab.mirrored ? 0 : popupsTab.width - width
            y: popupsTab.topPadding
            height: popupsTab.availableHeight
            active: popupsTab.ScrollBar.horizontal.active
        }

        ScrollBar.horizontal: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: popupsTab
            x: popupsTab.leftPadding
            y: popupsTab.height - height
            width: popupsTab.availableWidth
            active: popupsTab.ScrollBar.vertical.active
        }

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins - Style.OBConstants.scrollBarThickness
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                id: addCategorySection
                title: "OBAddCategoryPopup"
                availableWidth: parent.width

                property string lastResult: "none"

                Comp.OBButton {
                    text: "Add category"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: addCategoryPopup.open()
                }
                Comp.OBLabel {
                    // Reads the shared model directly, so it also reflects
                    // categories added anywhere else that touches it.
                    text: "last result: " + addCategorySection.lastResult + " — " + categoryModel.count + " total"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Popup.OBAddCategoryPopup {
                    id: addCategoryPopup
                    parent: popupsTab
                    x: (popupsTab.width - width) / 2
                    y: (popupsTab.height - height) / 2
                    onAccepted: function (name, direction) {
                        const category = categoryPrototype.createObject(categoryModel)
                        category.setId(window.nextDemoId())
                        category.name = name
                        category.direction = direction
                        categoryModel.addCategory(category)
                        addCategorySection.lastResult = name + " (" + direction + ")"
                    }
                    onRejected: addCategorySection.lastResult = "cancelled"
                }
            }

            UiKitSection {
                id: addProfileSection
                title: "OBAddProfilePopup"
                availableWidth: parent.width

                property string lastResult: "none"

                Comp.OBButton {
                    text: "Add profile"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: addProfilePopup.open()
                }
                Comp.OBLabel {
                    text: "last result: " + addProfileSection.lastResult + " — " + profileModel.count + " total"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Popup.OBAddProfilePopup {
                    id: addProfilePopup
                    parent: popupsTab
                    x: (popupsTab.width - width) / 2
                    y: (popupsTab.height - height) / 2
                    onAccepted: function (firstName, lastName) {
                        const profile = profilePrototype.createObject(profileModel)
                        profile.setId(window.nextDemoId())
                        profile.firstName = firstName
                        profile.lastName = lastName
                        profileModel.addProfile(profile)
                        addProfileSection.lastResult = firstName + " " + lastName
                    }
                    onRejected: addProfileSection.lastResult = "cancelled"
                }
            }

            UiKitSection {
                id: addAccountSection
                title: "OBAddAccountPopup"
                availableWidth: parent.width

                property string lastResult: "none"

                Comp.OBButton {
                    text: "Add account"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: addAccountPopup.open()
                }
                Comp.OBLabel {
                    // New accounts show up immediately in the "Account
                    // filter" combo of the OBTransactionList section.
                    text: "last result: " + addAccountSection.lastResult + " — " + accountModel.count + " total"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Popup.OBAddAccountPopup {
                    id: addAccountPopup
                    parent: popupsTab
                    x: (popupsTab.width - width) / 2
                    y: (popupsTab.height - height) / 2
                    onAccepted: function (name, bank, opening, interest) {
                        const account = accountPrototype.createObject(accountModel)
                        account.setId(window.nextDemoId())
                        account.number = name
                        account.bank = bank
                        account.opening = opening
                        account.interest = interest
                        accountModel.addAccount(account)
                        addAccountSection.lastResult = name + " / " + bank + " (" + interest + "%)"
                    }
                    onRejected: addAccountSection.lastResult = "cancelled"
                }
            }

            UiKitSection {
                id: validateTransactionsSection
                title: "OBValidateTransactionsPopup"
                availableWidth: parent.width

                property string lastResult: "none"
                property int lastConfirmedCount: 0

                Comp.OBButton {
                    text: "Validate estimated"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: validateTransactionsPopup.open()
                }
                Comp.OBLabel {
                    text: "last result: " + validateTransactionsSection.lastResult
                    anchors.verticalCenter: parent.verticalCenter
                }

                Popup.OBValidateTransactionsPopup {
                    id: validateTransactionsPopup
                    parent: popupsTab
                    x: (popupsTab.width - width) / 2
                    y: (popupsTab.height - height) / 2
                    transactionModel: transactionModel
                    // validated() and discarded() both fire synchronously
                    // from the same applyValidation() call (confirmed
                    // first), so lastConfirmedCount set here is already
                    // current by the time onDiscarded combines it below.
                    onValidated: function (transactions) {
                        validateTransactionsSection.lastConfirmedCount = transactions.length
                    }
                    onDiscarded: function (transactions) {
                        validateTransactionsSection.lastResult =
                            validateTransactionsSection.lastConfirmedCount + " confirmed, " +
                            transactions.length + " discarded"
                    }
                    onRejected: validateTransactionsSection.lastResult = "postponed"
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }

    ScrollView {
        id: pageTab
        visible: tabBar.currentIndex === 4
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        // OBBudgetPage is wider than this gallery window, unlike every
        // other tab's content — both scrollbars are needed here, not just
        // the vertical one.
        ScrollBar.vertical: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: pageTab
            x: pageTab.mirrored ? 0 : pageTab.width - width
            y: pageTab.topPadding
            height: pageTab.availableHeight
            active: pageTab.ScrollBar.horizontal.active
        }

        ScrollBar.horizontal: Comp.OBScrollBar {
            policy: ScrollBar.AlwaysOn
            parent: pageTab
            x: pageTab.leftPadding
            y: pageTab.height - height
            width: pageTab.availableWidth
            active: pageTab.ScrollBar.vertical.active
        }

        // OBBudgetPage derives the selected instance's used amount (and
        // therefore the Figma "state=good"/"state=bad" wash) from the
        // Details table's filtered transactions rather than from
        // Budget.usedAmount()/overBudget() — those stay at 0/false here
        // since Budget.addTransaction() isn't Q_INVOKABLE and nothing in
        // this QML-built demo ever populates a Budget's own transaction
        // list. Budget 1 above is seeded to demo "state=bad"; 2-4 demo
        // "state=good".
        Page.OBBudgetPage {
            width: implicitWidth
            height: implicitHeight
            budgetModel: budgetModel
            categoryModel: categoryModel
            // Same proxy as the OBTransactionList section above: switching
            // its account filter there is reflected here too.
            transactionModel: accountTransactionProxy
            iterationNumber: 4
            overedCount: 1
            respectedCount: 3
        }
    }
}
