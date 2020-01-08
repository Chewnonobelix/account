#ifndef DBREQUESTSINIT_H
#define DBREQUESTSINIT_H

#include <QLatin1String>

const auto account_table = QString("CREATE TABLE `account` ("
                                       "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                       "`account` TEXT NOT NULL,"
                                       "`value` double NOT NULL,"
                                       "`date_eff` date DEFAULT NULL,"
                                       "`type` TEXT NOT NULL  DEFAULT `outcome` ,"
                                       "`profile` TEXT NOT NULL DEFAULT `Default`,"
                                       "`commonExpanse` INTEGER DEFAULT NULL,"
                                       "`frequencyReference` INTEGER DEFAULT NULL)");

const auto entrymetadata_table = QString("CREATE TABLE `entrymetadata` ("
                                             "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                                             "`entry` INTEGER NOT NULL REFERENCES account(id), "
                                             "`name` TEXT NOT NULL, "
                                             "`value` TEXT NOT NULL, "
                                             "CONSTRAINT meta_unique UNIQUE(entry, name)"
                                           ")");

const auto categories_table = QString("CREATE TABLE `categories` ("
                                          "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                          "`name` TEXT NOT NULL,"
                                          "`type` TEXT NOT NULL DEFAULT `outcome`,"
                                          "`account` TEXT NOT NULL,"
                                          "`profile` TEXT NOT NULL,"
                                      "CONSTRAINT category_unique UNIQUE(name, account, profile))");

const auto information_table = QString("CREATE TABLE `information` ("
                                           "`id` INTEGER NOT NULL  PRIMARY KEY AUTOINCREMENT,"
                                           "`idEntry` INTEGER NOT NULL REFERENCES account(id),"
                                           "`info` TEXT NOT NULL,"
                                           "`prev` INTEGER NOT NULL DEFAULT '0',"
                                           "`category` INTEGER DEFAULT NULL REFERENCES categories(id)"
                                         ")");

const auto budget_table = QString("CREATE TABLE `budget` ("
                                      "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                      "`category` INTEGER NOT NULL REFERENCES categories(id),"
                                      "`reference` date NOT NULL,"
                                      "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                      "`account` TEXT NOT NULL)");

const auto subbudget_table = QString("CREATE TABLE `subbudget` ("
                                         "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                         "`idBudget` INTEGER NOT NULL REFERENCES budget(id),"
                                         "`frequency` INTEGER NOT NULL,"
                                         "`target` REAL NOT NULL,"
                                         "`fromDate` date NOT NULL"
                                       ")");

const auto frequency_table = QString("CREATE TABLE `frequency` ("
                                         "`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                         "`freq` INTEGER NOT NULL,"
                                         "`nbGroup` INTEGER NOT NULL,"
                                         "`end` date DEFAULT NULL,"
                                         "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                         "`account` TEXT NOT NULL"
                                       ")");

const auto commonExpanse_table = QString("CREATE TABLE `commonExpanse` ("
                                             "`id` INTEGER NOT NULL  PRIMARY KEY AUTOINCREMENT,"
                                             "`begin` date NOT NULL,"
                                             "`isClose` tinyint(1) NOT NULL DEFAULT '0',"
                                             "`title` TEXT NOT NULL,"
                                             "`profile` TEXT NOT NULL DEFAULT 'Default',"
                                             "`account` TEXT NOT NULL"
                                           ")");

const auto commonEntry_table = QString("CREATE TABLE `commonEntry` ("
                                           "`idCommon` INTEGER NOT NULL REFERENCES commonExpanse (id),"
                                           "`name` TEXT NOT NULL,"
                                           "`entry` INTEGER NOT NULL REFERENCES account(id)"
                                         ")");




#endif // DBREQUESTSINIT_H
