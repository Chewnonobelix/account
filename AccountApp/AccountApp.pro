CONFIG += c++17 
QT += quick sql core xml widgets charts qml gui xmlpatterns

TEMPLATE = app
TARGET = Account
SOURCES += \
    main.cpp

INCLUDEPATH += $$PWD/../Account

LIBS += -L$$OUT_PWD/ -lLibAccount

