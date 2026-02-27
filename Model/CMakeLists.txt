# Generated from Model.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Model
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

target_include_directories(Model PRIVATE
    $$PWD/include
    $$PWD/../lib/DesignLibrary/DesignPattern
)

target_compile_definitions(Model PRIVATE
    "MODEL_LIBRARY"
)

target_link_libraries(Model PRIVATE
    Qt6::Core
)
