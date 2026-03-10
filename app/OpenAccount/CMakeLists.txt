# Generated from OpenAccount.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(OpenAccount
    src/main.cpp
)

target_link_libraries(OpenAccount PRIVATE
    Qt6::Core
    OAModel
)
