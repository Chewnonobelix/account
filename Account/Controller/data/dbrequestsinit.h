#ifndef DBREQUESTSINIT_H
#define DBREQUESTSINIT_H

#include <QLatin1String>

const auto account_table = QStringLiteral(
    "CREATE TABLE `account` ("
    "`id` TEXT NOT NULL PRIMARY KEY,"
    "`account` TEXT NOT NULL,"
    "`value` double NOT NULL,"
    "`date_eff` date DEFAULT NULL,"
    "`type` TEXT NOT NULL  DEFAULT `outcome` ,"
    "`profile` TEXT NOT NULL DEFAULT `Default`,"
    "`frequencyReference` INTEGER DEFAULT NULL REFERENCES frequency(id))");

const auto account_trigger_delete = QStringLiteral(
    "CREATE TRIGGER delete_entry BEFORE DELETE ON account "
    "BEGIN "
    "DELETE FROM entrymetadata "
    "WHERE entry=OLD.id; "
    "END;");

const auto entrymetadata_table = QStringLiteral("CREATE TABLE `entrymetadata` ("
                                                "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                                                "`entry` TEXT NOT NULL REFERENCES account(id), "
                                                "`name` TEXT NOT NULL, "
                                                "`value` TEXT NOT NULL, "
                                                "CONSTRAINT meta_unique UNIQUE(entry, name)"
                                                ")");

const auto categories_table = QStringLiteral(
    "CREATE TABLE `categories` ("
    "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
    "`name` TEXT NOT NULL,"
    "`type` TEXT NOT NULL DEFAULT `outcome`,"
    "`account` TEXT NOT NULL,"
    "`profile` TEXT NOT NULL,"
    "CONSTRAINT category_unique UNIQUE(name, account, profile))");

const auto trigger_delete_category = QStringLiteral(
    "CREATE TRIGGER delete_category BEFORE DELETE ON categories "
    "BEGIN "
    "DELETE FROM budget "
    "WHERE category=OLD.id; "
    "UPDATE information "
    "SET category=NULL "
    "WHERE category=OLD.id;"
    "END;");


const auto budget_table = QStringLiteral("CREATE TABLE `budget` ("
                                         "`id` TEXT NOT NULL PRIMARY KEY,"
                                         "`category` INTEGER NOT NULL REFERENCES categories(id),"
                                         "`reference` date NOT NULL,"
                                         "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                         "`account` TEXT NOT NULL, "
                                         "`removed`INTEGER NOT NULL DEFAULT '0')");

const auto budget_trigger = QStringLiteral("CREATE TRIGGER update_budget AFTER UPDATE ON budget "
                                           "BEGIN "
                                           "DELETE FROM subbudget "
                                           "WHERE idBudget=OLD.id;"
                                           "END;");

const auto budget_delete_trigger = QStringLiteral(
    "CREATE TRIGGER delete_budget BEFORE DELETE ON budget "
    "BEGIN "
    "DELETE FROM subbudget "
    "WHERE idBudget=OLD.id;"
    "END;");

const auto subbudget_table = QStringLiteral(
    "CREATE TABLE `subbudget` ("
    "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
    "`idBudget` TEXT NOT NULL REFERENCES budget(id),"
    "`frequency` INTEGER NOT NULL,"
    "`target` REAL NOT NULL,"
    "`fromDate` date NOT NULL, "
    "CONSTRAINT subbudget_unique UNIQUE (idBudget, fromDate) "
    ")");

const auto frequency_table = QStringLiteral("CREATE TABLE `frequency` ("
                                            "`id` TEXT NOT NULL PRIMARY KEY,"
                                            "`freq` INTEGER NOT NULL,"
                                            "`nbGroup` INTEGER NOT NULL,"
                                            "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                            "`account` TEXT NOT NULL, "
                                            "`endless` tinyint(1) NOT NULL DEFAULT '0', "
                                            "`removed` INTEGER NOT NULL DEFAULT '0')");

const auto trigger_delete_frequency = QStringLiteral(
    "CREATE TRIGGER delete_frequency BEFORE DELETE ON frequency "
    "BEGIN "
    "DELETE FROM account WHERE frequencyReference=OLD.id; "
    "END;");

const auto commonExpanse_table = QStringLiteral("CREATE TABLE `commonExpanse` ("
                                                "`id` TEXT NOT NULL  PRIMARY KEY,"
                                                "`begin` date NOT NULL,"
                                                "`isClose` tinyint(1) NOT NULL DEFAULT '0',"
                                                "`title` TEXT NOT NULL,"
                                                "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                                "`account` TEXT NOT NULL, "
                                                "`removed`INTEGER NOT NULL DEFAULT '0')");

const auto expanse_trigger = QStringLiteral(
    "CREATE TRIGGER update_expanse AFTER UPDATE ON commonExpanse "
    "BEGIN "
    "DELETE FROM commonEntry "
    "WHERE idCommon=OLD.id;"
    "END;");

const auto trigger_delete_commonExpanse = QStringLiteral(
    "CREATE TRIGGER delete_expanse BEFORE DELETE ON commonExpanse "
    "BEGIN "
    "DELETE FROM commonEntry WHERE idCommon=OLD.id;"
    "END;");

const auto commonEntry_table = QStringLiteral(
    "CREATE TABLE `commonEntry` ("
    "`id` TEXT PRIMARY KEY, "
    "`idCommon` TEXT NOT NULL REFERENCES commonExpanse (id),"
    "`name` TEXT NOT NULL,"
    "`entry` INTEGER NOT NULL REFERENCES account(id)"
    ")");

const auto trigger_delete_commonEntry = QStringLiteral(
    "CREATE TRIGGER delete_commonEntry BEFORE DELETE ON commonEntry "
    "BEGIN "
    "DELETE FROM account WHERE id=OLD.entry; "
    "END;");

const auto account_remove_table = QStringLiteral("CREATE TABLE temp_account ("
                                                 "name TEXT NOT NULL)");

const auto remove_account_trigger = QStringLiteral(
    "CREATE TRIGGER delete_account AFTER INSERT ON temp_account "
    "BEGIN "
    "DELETE FROM commonExpanse WHERE account=NEW.name; "
    "DELETE FROM account WHERE account=NEW.name; "
    "DELETE FROM categories WHERE account=NEW.name; "
    "DELETE FROM frequency WHERE account=NEW.name; "
    "DELETE FROM debt WHERE account=NEW.name; "
    "DELETE FROM temp_account WHERE name=NEW.name; "
    "END;");

const auto profile_remove_table = QStringLiteral("CREATE TABLE temp_profile ("
                                                 "name TEXT NOT NULL)");

const auto remove_profile_trigger = QStringLiteral(
    "CREATE TRIGGER delete_profile AFTER INSERT ON temp_profile "
    "BEGIN "
    "DELETE FROM commonExpanse WHERE profile=NEW.name; "
    "DELETE FROM account WHERE profile=NEW.name; "
    "DELETE FROM categories WHERE profile=NEW.name; "
    "DELETE FROM frequency WHERE profile=NEW.name; "
    "DELETE FROM debt WHERE profile=NEW.name; "
    "DELETE FROM temp_profile WHERE name=NEW.name; "
    "END;");

const auto debt_table = QStringLiteral("CREATE TABLE `debt` ("
                                       "`id` TEXT PRIMARY KEY, "
                                       "`freq` INTEGER NOT NULL, "
                                       "`nb` INTEGER NOT NULL, "
                                       "`rate` double NOT NULL, "
                                       "`name` TEXT, "
                                       "`removed` integer DEFAULT 0, "
                                       "`account` TEXT NOT NULL, "
                                       "`profile` TEXT NOT NULL"
                                       ")");
#endif // DBREQUESTSINIT_H
