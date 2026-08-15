#pragma once

#include "Model/enums.h"

#include <QDate>
#include <QList>
#include <QMetaType>
#include <QString>
#include <QUuid>

// Plain-data mirrors of the Account/Category/Profile/Transaction/Budget
// business entities, used only to carry DemoDataWorker's output back to the
// GUI thread. None of these are QObjects: the real business objects have
// QObject identity and thread affinity, so building them off the GUI thread
// and handing them to a QAbstractItemModel that lives on it would be unsafe.
// DemoDataGenerator turns each entry into the real type once it's back on
// the GUI thread (see onResultReady()).

struct GeneratedAccount {
    QUuid id;
    QString number;
    QString bank;
    QString description;
    double interest = 0.0;
    QDate opening;
};

struct GeneratedCategory {
    QUuid id;
    QString name;
    OpenAccountEnums::Movement direction = OpenAccountEnums::Movement::Debit;
};

struct GeneratedProfile {
    QUuid id;
    QString firstName;
    QString lastName;
    QList<QUuid> accounts;
};

struct GeneratedTransaction {
    QUuid id;
    QString name;
    QString description;
    double value = 0.0;
    OpenAccountEnums::Movement movement = OpenAccountEnums::Movement::Debit;
    OpenAccountEnums::Support support = OpenAccountEnums::Support::Other;
    QDate date;
    QUuid accountId;
    QUuid category;
};

struct GeneratedBudget {
    QUuid id;
    QString name;
    QUuid categoryId;
    double threshold = 0.0;
    QDate startDate;
    QDate endDate;
    OpenAccountEnums::Frequency recurrence = OpenAccountEnums::Frequency::Monthly;
};

struct GeneratedDataset {
    QList<GeneratedAccount> accounts;
    QList<GeneratedCategory> categories;
    QList<GeneratedProfile> profiles;
    QList<GeneratedTransaction> transactions;
    QList<GeneratedBudget> budgets;
};

Q_DECLARE_METATYPE(GeneratedDataset)
