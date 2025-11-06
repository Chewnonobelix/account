TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    Controller \
    Model \
    View \
    app/OpenAccount \
    Controller/ControllerTests \
    Model/ModelTests \
    View/ViewTests

# Dépendances pour garantir l'ordre de build
app/OpenAccount.depends = Controller Model View
Controller/tests.depends = Controller
Model/tests.depends = Model
View/tests.depends = View


