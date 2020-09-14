TEMPLATE = subdirs

SUBDIRS = \
Account \
    AccountApp \
    DesignLibrary \
    Test

TRANSLATIONS += Account\Francais.ts \
                Account\English.ts

CONFIG += ENABLE_HOTRELOADING QML_SOURCE=Account/View

AccountApp.depends = Account
Test.depends = Account
Account.depends = DesignLibrary
