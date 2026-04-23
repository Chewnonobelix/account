TEMPLATE = lib
CONFIG += shared c++17 warn_on plugin
QT -= gui

TARGET = Model
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD
DEFINES = MODEL_LIBRARY
HEADERS += \
    include/Model/accounttransactionfilterproxymodel.h \
    include/Model/account.h \
    include/Model/category.h \
    include/Model/debt.h \
    include/Model/enums.h \
    include/Model/frequency.h \
    include/Model/model.h \
    include/Model/model_global.h \
    include/Model/profile.h \
    include/Model/total.h \
    include/Model/transaction.h \
    include/Model/transactionlistmodel.h

SOURCES += \
    src/accounttransactionfilterproxymodel.cpp \
    src/account.cpp \
    src/category.cpp \
    src/debt.cpp \
    src/enums.cpp \
    src/frequency.cpp \
    src/profile.cpp \
    src/model.cpp \
    src/total.cpp \
    src/transaction.cpp \
    src/transactionlistmodel.cpp


INCLUDEPATH += $$PWD/../lib/DesignLibrary/DesignPattern

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern -lDesignPattern
