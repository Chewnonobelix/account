cmake_minimum_required(VERSION 3.16)
project(OpenAccount LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Test Qml Quick QuickControls2 Network Sql Xml)
# Note: components above are a superset; you can trim later.
# include(CTest)

add_subdirectory(lib/DesignLibrary)
# add_subdirectory(Controller)
add_subdirectory(Model)
# add_subdirectory(View)
add_subdirectory(app/OpenAccount)
# add_subdirectory(Controller/ControllerTests)
add_subdirectory(Model/ModelTests)
# add_subdirectory(View/ViewTests)
