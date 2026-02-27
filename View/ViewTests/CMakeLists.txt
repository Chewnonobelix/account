# Generated from ViewTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ViewTests
    tst_view.cpp
)

target_include_directories(ViewTests PRIVATE
    $$PWD/../include
    $$PWD/../../lib/DesignLibrary/DesignPattern
)

target_link_libraries(ViewTests PRIVATE
    Qt6::Test
)

add_test(NAME ViewTests COMMAND ViewTests)
