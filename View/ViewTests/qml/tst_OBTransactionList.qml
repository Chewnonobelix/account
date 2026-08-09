import QtQuick
import QtTest

import OpenAccount.Model

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBTransactionList"
    when: windowShown
    width: 400
    height: 300

    TransactionListModel {
        id: sourceModel
    }

    AccountTransactionFilterProxyModel {
        id: proxyModel
        sourceModel: sourceModel
    }

    Component {
        id: transactionPrototype
        Transaction {}
    }

    Composed.OBTransactionList {
        id: list
        model: proxyModel
        anchors.fill: parent
    }

    function test_modelPropertyExposesAssignedModel() {
        compare(list.model, proxyModel)
    }

    function test_currentIndexDefaultsToMinusOne() {
        compare(list.currentIndex, -1)
    }

    // QtQuickTest runs test functions in alphabetical order rather than
    // declaration order, so the empty-model and add-transaction assertions
    // are combined here into a single function: sourceModel is shared across
    // the whole test file (TransactionListModel.clear() isn't Q_INVOKABLE,
    // so it can't be reset from QML) and splitting these across two
    // independently-ordered functions would make the "empty" check depend on
    // no other test having already populated the model.
    function test_countTracksTransactionAdditions() {
        compare(list.count, 0)

        sourceModel.addTransaction(transactionPrototype.createObject(sourceModel, {
            name: "Salary", value: 2500, movement: OpenAccountEnums.Movement.Credit
        }))
        compare(list.count, 1)

        sourceModel.addTransaction(transactionPrototype.createObject(sourceModel, {
            name: "Rent", value: 950, movement: OpenAccountEnums.Movement.Debit
        }))
        compare(list.count, 2)
    }

    function test_defaultPageSizeIsOneHundred() {
        compare(list.pageSize, 100)
    }

    // Uses its own dedicated model/proxy/list (rather than the file-shared
    // sourceModel/list above) so pushing past pageSize doesn't leak rows
    // into the other, independently-ordered test functions.
    function test_pageSizeCapsVisibleRowsPerPage() {
        const localSource = pagingModelComponent.createObject(testCase)
        const localProxy = pagingProxyComponent.createObject(testCase, { sourceModel: localSource })
        const localList = pagingListComponent.createObject(testCase, { model: localProxy })

        for (let i = 0; i < 120; ++i) {
            localSource.addTransaction(transactionPrototype.createObject(localSource, {
                name: "Tx " + i, value: 10, movement: OpenAccountEnums.Movement.Debit
            }))
        }

        compare(localList.totalCount, 120)
        // PageFilterProxyModel recomputes pageCount off a queued connection
        // (see pagefilterproxymodel.h) so it lags one event-loop turn behind
        // the synchronous addTransaction() calls above.
        tryCompare(localList, "pageCount", 2)
        tryCompare(localList, "count", 100)

        localList.currentPage = 2
        compare(localList.count, 20)

        localList.destroy()
        localProxy.destroy()
        localSource.destroy()
    }

    Component {
        id: pagingModelComponent
        TransactionListModel {}
    }
    Component {
        id: pagingProxyComponent
        AccountTransactionFilterProxyModel {}
    }
    Component {
        id: pagingListComponent
        Composed.OBTransactionList {}
    }
}
