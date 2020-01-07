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
#endif // DBREQUESTSINIT_H
