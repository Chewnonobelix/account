TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    lib/DesignLibrary \
    Controller \
    Model \
    View \
    app/OpenAccount \
    Controller/ControllerTests \
    Model/ModelTests \
    View/ViewTests

# Dépendances pour garantir l'ordre de build
app/OpenAccount.depends = Controller Model View
Controller.depends = lib/DesignLibrary
Model.depends = lib/DesignLibrary
View.depends = lib/DesignLibrary
Controller/tests.depends = Controller
Model/tests.depends = Model
View/tests.depends = View


