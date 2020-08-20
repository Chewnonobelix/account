TEMPLATE = subdirs

SUBDIRS = \
Account \
    AccountApp \
    DesignLibrary \
    Test

TRANSLATIONS += Account\Francais.ts \
                Account\English.ts

AccountApp.depends = Account
Test.depends = Account
Account.depends = DesignLibrary
