# Generated from Model.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(OAModel
    src/account.cpp
    src/category.cpp
    src/profile.cpp
    src/model.cpp
    src/total.cpp
    src/transaction.cpp
    include/Model/account.h
    include/Model/category.h
    include/Model/enums.h
    include/Model/model.h
    include/Model/model_global.h
    include/Model/profile.h
    include/Model/total.h
    include/Model/transaction.h
)

target_include_directories(OAModel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(OAModel PRIVATE
    "MODEL_LIBRARY"
)

target_link_libraries(OAModel PUBLIC
    Qt6::Core
    DesignPattern
)
