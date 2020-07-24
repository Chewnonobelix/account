TEMPLATE = subdirs

SUBDIRS = \
Account \
    AccountApp \
    Test

TRANSLATIONS += Account\Francais.ts \
                Account\English.ts

AccountApp.depends = Account
Test.depends = Account
