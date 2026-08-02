# Generated from View.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(View
    src/view.cpp
    include/View/view.h
    OpenAccount.qrc
)

target_include_directories(View PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/DesignLibrary/DesignPattern
)

target_link_libraries(View PRIVATE
    Qt6::Quick
    Qt6::Qml
)
