# Generated from Controller.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Controller
    src/controller.cpp
    include/Controller/controller.h
)

target_include_directories(Controller PRIVATE
    $$PWD/include
    $$PWD/../lib/DesignLibrary/DesignPattern
)

target_link_libraries(Controller PRIVATE
    Qt6::Core
)
