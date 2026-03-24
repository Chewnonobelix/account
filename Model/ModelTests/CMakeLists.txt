# Generated from ModelTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(TestProfile tst_profile.cpp)
add_executable(TestAccount tst_account.cpp)
add_executable(TestCategory tst_category.cpp)
add_executable(TestModel tst_model.cpp)
add_executable(TestTotal tst_total.cpp)
add_executable(TestTransaction tst_transaction.cpp)
add_executable(TestFrequency tst_frequency.cpp)


target_link_libraries(TestProfile PRIVATE
    Qt6::Test
    OAModel
)
target_link_libraries(TestTransaction PRIVATE
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

add_test(NAME TestProfile COMMAND TestProfile)
add_test(NAME TestTransaction COMMAND TestTransaction)
add_test(NAME TestTotal COMMAND TestTotal)
add_test(NAME TestModel COMMAND TestModel)
add_test(NAME TestCategory COMMAND TestCategory)
add_test(NAME TestAccount COMMAND TestAccount)
add_test(NAME TestFrequency COMMAND TestFrequency)

add_custom_target(run_model_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS
        TestProfile
        TestAccount
        TestCategory
        TestModel
        TestTotal
        TestTransaction
        TestFrequency
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
