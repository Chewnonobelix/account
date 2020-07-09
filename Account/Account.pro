QT += quick sql core xml widgets charts qml gui xmlpatterns
CONFIG += c++17 windeployqt

##Command de deploiement: windeployeqt --qmldir %{sourceDir}\Account\View
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/abstractgraphcontroller.cpp \
    Controller/controllerbudget.cpp \
    Controller/controllercommon.cpp \
    Controller/controllerfrequency.cpp \
    Controller/controllerpiegraph.cpp \
    Controller/controllersettings.cpp \
    Controller/interfacedatasave.cpp \
    Model/accountglobal.cpp \
    Model/commonexpanse.cpp \
    Model/frequency.cpp \
    Model/metadata.cpp \
    Model/subbudget.cpp \
        main.cpp \
    Model/entry.cpp \
    Model/information.cpp \
    Model/total.cpp \
    Controller/abstractcontroller.cpp \
    Controller/controllerdb.cpp \
    Controller/maincontroller.cpp \
    Controller/controllerinformation.cpp \
    Controller/graphcontroller.cpp \
    Controller/xmltosql.cpp \
    Model/budget.cpp \
    Controller/controllerxmlmulti.cpp \
    Controller/languagecontroller.cpp \
    Controller/controllertransfert.cpp

RESOURCES += \
    View/Account.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $${PWD}\View

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $${PWD}\View


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#TRANSLATIONS += account_fr.ts \
#                account_en.ts \

HEADERS += \
    Controller/InterfaceGraph.h \
    Controller/abstractgraphcontroller.h \
    Controller/controllerbudget.h \
    Controller/controllercommon.h \
    Controller/controllerfrequency.h \
    Controller/controllerpiegraph.h \
    Controller/controllersettings.h \
    Controller/dbrequestsinit.h \
    Controller/featurebuilder.h \
    Controller/filler.h \
    Model/commonexpanse.h \
    Model/entry.h \
    Model/frequency.h \
    Model/information.h \
    Model/metadata.h \
    Model/subbudget.h \
    Model/total.h \
    Controller/abstractcontroller.h \
    Controller/controllerdb.h \
    Controller/maincontroller.h \
    Controller/controllerinformation.h \
    Controller/interfacedatasave.h \
    Controller/graphcontroller.h \
    Controller/xmltosql.h \
    Model/budget.h \
    Controller/controllerxmlmulti.h \
    Model/accountglobal.h \
    Controller/controllertransfert.h \
    Controller/languagecontroller.h
    Model/frequency.h

RC_ICONS = $$PWD/View/Style/tray.ico

DISTFILES += \
    ../README_en.md \
    ../README_fr.md
