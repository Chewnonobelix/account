#ifndef DBREQUESTSINIT_H
#define DBREQUESTSINIT_H

#include <QLatin1String>

const auto account_table = QString("CREATE TABLE `account` ("
                                       "`id` int(11) NOT NULL,"
                                       "`account` varchar(50) NOT NULL,"
                                       "`value` double NOT NULL,"
                                       "`date_eff` date DEFAULT NULL,"
                                       "`type` varchar(200) NOT NULL  DEFAULT `outcome` ,"
                                       "`profile` varchar(200) NOT NULL DEFAULT `Default`,"
                                       "`commonExpanse` int(11) DEFAULT NULL,"
                                       "`frequencyReference` int(11) DEFAULT NULL)");

const auto budget_table = QString("CREATE TABLE `budget` ("
                                      "`id` int(11) NOT NULL,"
                                      "`category` int(11) NOT NULL,"
                                      "`reference` date NOT NULL,"
                                      "`profile` varchar(200) NOT NULL DEFAULT 'Default',"
                                      "`account` varchar(200) NOT NULL)");

const auto categories_table = QString("CREATE TABLE `categories` ("
                                          "`ID` int(11) NOT NULL,"
                                          "`name` varchar(50) NOT NULL,"
                                          "`type` varchar(200) DEFAULT NOT NULL DEFAULT `outcome`,"
                                          "`account` varchar(200) NOT NULL,"
                                          "`profile` varchar(200) NOT NULL)");

const auto commonEntry_table = QString("CREATE TABLE `commonEntry` ("
                                           "`idCommon` int(11) NOT NULL,"
                                           "`name` varchar(200) NOT NULL,"
                                           "`entry` int(11) NOT NULL"
                                         ")");

const auto commonExpanse_table = QString("CREATE TABLE `commonExpanse` ("
                                             "`id` int(11) NOT NULL,"
                                             "`begin` date NOT NULL,"
                                             "`isClose` tinyint(1) NOT NULL DEFAULT '0',"
                                             "`title` varchar(200) NOT NULL,"
                                             "`profile` varchar(200) NOT NULL DEFAULT 'Default',"
                                             "`account` varchar(200) NOT NULL"
                                           ")");

const auto entrymetadata_table = QString("CREATE TABLE `entrymetada` ("
                                             "`id` int(11) NOT NULL,"
                                             "`entry` int(11) NOT NULL,"
                                             "`name` varchar(200) NOT NULL,"
                                             "`value` varchar(200) NOT NULL"
                                           ")");

const auto frequency_table = QString("CREATE TABLE `frequency` ("
                                         "`id` int(11) NOT NULL,"
                                         "`freq` int(11) NOT NULL,"
                                         "`nbGroup` int(11) NOT NULL,"
                                         "`end` date DEFAULT NULL,"
                                         "`profile` varchar(200) NOT NULL DEFAULT 'Default',"
                                         "`account` varchar(200) NOT NULL"
                                       ")");

const auto information_table = QString("CREATE TABLE `information` ("
                                           "`id` int(11) NOT NULL,"
                                           "`id_entry` int(11) NOT NULL,"
                                           "`info` varchar(200) NOT NULL,"
                                           "`prev` tinyint(1) NOT NULL DEFAULT '0',"
                                           "`category` int(11) DEFAULT NULL"
                                         ")");

const auto subbudget_table = QString("CREATE TABLE `subbudget` ("
                                         "`ID` int(11) NOT NULL,"
                                         "`idBudget` int(11) NOT NULL,"
                                         "`frequency` int(11) NOT NULL,"
                                         "`target` double NOT NULL,"
                                         "`fromDate` date NOT NULL"
                                       ")");

const auto account_key = QString("ALTER TABLE `account`"
                                 "ADD PRIMARY KEY (`id`),"
                                 "ADD KEY `type` (`type`)"
                                 "MODIFY `id` int(11) NOT NULL AUTO_INCREMENT");

const auto information_key = QString("ALTER TABLE `information`"
                                     "ADD PRIMARY KEY (`id`)"
                                     "MODIFY `id` int(11) NOT NULL AUTO_INCREMENT");

#endif // DBREQUESTSINIT_H
