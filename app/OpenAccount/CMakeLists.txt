# Generated from OpenAccount.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(OpenAccount
    src/main.cpp
)

target_include_directories(OpenAccount PRIVATE
    $$PWD/../../Controller/include
    $$PWD/../../Model/include
    $$PWD/../../View/include
    $$PWD/../../lib/DesignLibrary/DesignPattern
)

target_link_libraries(OpenAccount PRIVATE
    Qt6::Core
)
