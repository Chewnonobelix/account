# Generated from ModelTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(TestProfile tst_profile.cpp)
add_executable(TestAccountTransactionFilterProxyModel tst_accounttransactionfilterproxymodel.cpp)
add_executable(TestAccountBalanceProxyModel tst_accountbalanceproxymodel.cpp)
add_executable(TestAccount tst_account.cpp)
add_executable(TestCategory tst_category.cpp)
add_executable(TestDebt tst_debt.cpp)
add_executable(TestModel tst_model.cpp)
add_executable(TestTotal tst_total.cpp)
add_executable(TestTransaction tst_transaction.cpp)
add_executable(TestTransactionListModel tst_transactionlistmodel.cpp)
add_executable(TestFrequency tst_frequency.cpp)
add_executable(TestTricountMember tst_tricountmember.cpp)
add_executable(TestTricountExpense tst_tricountexpense.cpp)
add_executable(TestTricount tst_tricount.cpp)


target_link_libraries(TestProfile PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestAccountTransactionFilterProxyModel PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestAccountBalanceProxyModel PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTransactionListModel PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTransaction PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestDebt PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTotal PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestModel PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestCategory PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestAccount PRIVATE
    Qt6::Test
    OAModel
)

target_link_libraries(TestFrequency PRIVATE
    Qt6::Test
    OAModel
)

target_link_libraries(TestTricountMember PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTricountExpense PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTricount PRIVATE
    Qt6::Test
    OAModel
)

add_test(NAME TestProfile COMMAND TestProfile)
add_test(NAME TestAccountTransactionFilterProxyModel COMMAND TestAccountTransactionFilterProxyModel)
add_test(NAME TestAccountBalanceProxyModel COMMAND TestAccountBalanceProxyModel)
add_test(NAME TestTransaction COMMAND TestTransaction)
add_test(NAME TestDebt COMMAND TestDebt)
add_test(NAME TestTotal COMMAND TestTotal)
add_test(NAME TestModel COMMAND TestModel)
add_test(NAME TestCategory COMMAND TestCategory)
add_test(NAME TestAccount COMMAND TestAccount)
add_test(NAME TestTransactionListModel COMMAND TestTransactionListModel)
add_test(NAME TestFrequency COMMAND TestFrequency)
add_test(NAME TestTricountMember COMMAND TestTricountMember)
add_test(NAME TestTricountExpense COMMAND TestTricountExpense)
add_test(NAME TestTricount COMMAND TestTricount)

add_custom_target(run_model_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS
        TestProfile
        TestAccountTransactionFilterProxyModel
        TestAccountBalanceProxyModel
        TestAccount
        TestCategory
        TestDebt
        TestModel
        TestTotal
        TestTransaction
        TestTransactionListModel
        TestFrequency
        TestTricountMember
        TestTricountExpense
        TestTricount
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
