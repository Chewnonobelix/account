TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = ModelTests
INCLUDEPATH += $$PWD/../include
INCLUDEPATH += $$PWD/../../lib/DesignLibrary/DesignPattern
DESTDIR = $$OUT_PWD

SOURCES += \
    main.cpp \
    tst_debt.cpp \
    tst_frequency.cpp \
    tst_profile.cpp \
    tst_account.cpp \
    tst_category.cpp \
    tst_model.cpp \
    tst_total.cpp \
    tst_transaction.cpp

win32:LIBS += -L$$OUT_PWD/.. -lModel
unix:LIBS += -L$$OUT_PWD/.. -lModel

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern -lDesignPattern

