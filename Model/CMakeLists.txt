# Generated from Model.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(OAModel
    src/accounttransactionfilterproxymodel.cpp
    src/account.cpp
    src/category.cpp
    src/debt.cpp
    src/profile.cpp
    src/model.cpp
    src/total.cpp
    src/transaction.cpp
    src/transactionlistmodel.cpp
    include/Model/accounttransactionfilterproxymodel.h
    include/Model/account.h
    include/Model/category.h
    include/Model/debt.h
    include/Model/enums.h
    include/Model/model.h
    include/Model/model_global.h
    include/Model/profile.h
    include/Model/total.h
    include/Model/transaction.h
    include/Model/transactionlistmodel.h
    include/Model/frequency.h
    src/frequency.cpp
    src/enums.cpp
)

target_include_directories(OAModel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(OAModel PRIVATE
    "MODEL_LIBRARY"
)

target_link_libraries(OAModel PUBLIC
    Qt6::Core
    ChewnoDesignPattern
)
