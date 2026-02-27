# Generated from ModelTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ModelTests
    main.cpp
    tst_Profile.cpp
    tst_account.cpp
    tst_category.cpp
    tst_model.cpp
    tst_total.cpp
    tst_transaction.cpp
)

target_include_directories(ModelTests PRIVATE
    $$PWD/../include
    $$PWD/../../lib/DesignLibrary/DesignPattern
)

target_link_libraries(ModelTests PRIVATE
    Qt6::Test
)

add_test(NAME ModelTests COMMAND ModelTests)
