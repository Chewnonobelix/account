# Generated from ControllerTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ControllerTests
    tst_controller.cpp
)

target_link_libraries(ControllerTests PRIVATE
    Qt6::Test
    Controller
)

add_test(NAME ControllerTests COMMAND ControllerTests)

add_executable(StorageLayerTests
    tst_storagelayer.cpp
)

target_link_libraries(StorageLayerTests PRIVATE
    Qt6::Test
    Controller
)

add_test(NAME StorageLayerTests COMMAND StorageLayerTests)
