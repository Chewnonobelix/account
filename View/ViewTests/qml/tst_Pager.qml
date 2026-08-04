import QtQuick
import QtTest

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "Pager"
    when: windowShown
    width: 300
    height: 100

    Composed.Pager {
        id: pager
        currentPage: 1
        maxPage: 5
    }

    function init() {
        pager.maxPage = 5
        pager.currentPage = 1
    }

    function test_defaultIsFirstPage() {
        compare(pager.currentPage, 1)
        compare(pager.isFirstPage, true)
        compare(pager.isLastPage, false)
    }

    function test_goToNextPage() {
        pager.goToNextPage()
        compare(pager.currentPage, 2)
    }

    function test_goToPreviousPage() {
        pager.currentPage = 3
        pager.goToPreviousPage()
        compare(pager.currentPage, 2)
    }

    function test_goToLastPage() {
        pager.goToLastPage()
        compare(pager.currentPage, 5)
        compare(pager.isLastPage, true)
    }

    function test_goToFirstPage() {
        pager.currentPage = 4
        pager.goToFirstPage()
        compare(pager.currentPage, 1)
        compare(pager.isFirstPage, true)
    }

    function test_setPageClampsToBounds() {
        pager.setPage(-3)
        compare(pager.currentPage, 1)
        pager.setPage(42)
        compare(pager.currentPage, 5)
    }

    function test_nextPageStopsAtLastPage() {
        pager.currentPage = 5
        pager.goToNextPage()
        compare(pager.currentPage, 5)
    }

    function test_previousPageStopsAtFirstPage() {
        pager.currentPage = 1
        pager.goToPreviousPage()
        compare(pager.currentPage, 1)
    }
}
