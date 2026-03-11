# Generated from ModelTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(TestProfile tst_Profile.cpp)
add_executable(TestAccount tst_account.cpp)
add_executable(TestCategory tst_category.cpp)
add_executable(TestModel tst_model.cpp)
add_executable(TestTotal tst_total.cpp)
add_executable(TestTransaction tst_transaction.cpp)


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


add_test(NAME ModelTests COMMAND ModelTests)
