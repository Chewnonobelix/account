# Generated from ControllerTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ControllerTests
    tst_controller.cpp
)

target_include_directories(ControllerTests PRIVATE
    $$PWD/../include
    $$PWD/../../lib/DesignLibrary/DesignPattern
)

target_link_libraries(ControllerTests PRIVATE
    Qt6::Test
)

add_test(NAME ControllerTests COMMAND ControllerTests)
