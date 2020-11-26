QT += quick sql core xml widgets qml gui network
CONFIG += c++17 windeployqt

TEMPLATE = lib

TARGET = LibAccount
DLLDESTDIR = $$OUT_PWD/../AccountApp $$OUT_PWD/../Test

##Command de deploiement: windeployeqt --qmldir %{sourceDir}\Account\View
##--qmldir %{sourceDir}\Account\View --dir %{buildDir}\deploy %{buildDir}\Account\release\LibAccount.dll

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS ACCOUNT_LIBRARY
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../DesignLibrary/DesignPattern
DEPENDPATH += $$OUT_PWD/../DesignLibrary/DesignPattern/debug
LIBS += -L$$OUT_PWD/../DesignLibrary/DesignPattern/debug -lDesignPattern

SOURCES += \
	Controller/ItemModel/budgetmodel.cpp \
    Controller/ItemModel/categorylistmodel.cpp \
    Controller/ItemModel/mainmodel.cpp \
	Controller/ItemModel/targetlistmodel.cpp \
    Controller/core/abstractcontroller.cpp \
    Controller/core/controllerinformation.cpp \
    Controller/core/controllersettings.cpp \
    Controller/core/controllersynchronization.cpp \
    Controller/core/controllertransfert.cpp \
    Controller/core/languagecontroller.cpp \
    Controller/core/maincontroller.cpp \
    Controller/data/controllerdb.cpp \
 Controller/data/controllerjson.cpp \
    Controller/data/controllerxmlmulti.cpp \
    Controller/data/interfacedatasave.cpp \
    Controller/data/xmltosql.cpp \
    Controller/features/controllerbudget.cpp \
    Controller/features/controllercommon.cpp \
    Controller/features/controllerdebt.cpp \
    Controller/features/controllerfrequency.cpp \
    Controller/graph/abstractgraphcontroller.cpp \
    Controller/graph/controllerpiegraph.cpp \
    Controller/graph/graphcontroller.cpp \
    Controller/network/AccountSocket.cpp \
    Controller/network/TcpServer.cpp \
    Model/accountglobal.cpp \
    Model/category.cpp \
    Model/commonexpanse.cpp \
    Model/debt.cpp \
    Model/frequency.cpp \
    Model/subbudget.cpp \
    Model/entry.cpp \
    Model/synchronizationprofile.cpp \
    Model/total.cpp \
    Model/budget.cpp \

RESOURCES += \
    View/Account.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $${PWD}View

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = "$${PWD}\View"

DEFINES += QML_SOURCE=\\\"$${PWD}\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#TRANSLATIONS += account_fr.ts \
#                account_en.ts \

HEADERS += \
    Controller/ItemModel/budgetmodel.h \
    Controller/ItemModel/categorylistmodel.h \
    Controller/ItemModel/mainmodel.h \
    Controller/ItemModel/targetlistmodel.h \
    Controller/core/abstractcontroller.h \
    Controller/core/controllerinformation.h \
    Controller/core/controllersettings.h \
    Controller/core/controllersynchronization.h \
    Controller/core/controllertransfert.h \
    Controller/core/languagecontroller.h \
    Controller/core/maincontroller.h \
    Controller/data/controllerdb.h \
    Controller/data/controllerjson.h \
    Controller/data/controllerxmlmulti.h \
    Controller/data/dbrequestsinit.h \
    Controller/data/interfacedatasave.h \
    Controller/data/xmltosql.h \
    Controller/features/controllerbudget.h \
    Controller/features/controllercommon.h \
    Controller/features/controllerdebt.h \
    Controller/features/controllerfrequency.h \
    Controller/features/featurebuilder.h \
    Controller/features/filler.h \
    Controller/graph/InterfaceGraph.h \
    Controller/graph/abstractgraphcontroller.h \
    Controller/graph/controllerpiegraph.h \
    Controller/graph/graphcontroller.h \
    Controller/network/AccountSocket.h \
    Controller/network/TcpServer.h \
    Model/category.h \
    Model/commonexpanse.h \
    Model/debt.h \
    Model/entry.h \
    Model/frequency.h \
    Model/subbudget.h \
    Model/synchronizationprofile.h \
    Model/total.h \
    Model/budget.h \
    Model/accountglobal.h \
    account_global.h
    Model/frequency.h

RC_ICONS = $$PWD/View/Style/tray.ico

DISTFILES += \
    ../README_en.md \
    ../README_fr.md
