#pragma once

#include <QObject>

#include "demodataset.h"

// Runs on the background QThread owned by DemoDataGenerator. All the random
// generation work happens here, off the GUI thread; it only ever reports
// back through queued signals; it never touches a QAbstractItemModel or any
// QObject-derived business entity, since those belong to the GUI thread.
class DemoDataWorker : public QObject {
    Q_OBJECT

public:
    explicit DemoDataWorker(QObject* parent = nullptr);

public slots:
    void run(int accountCount, int categoryCount, int profileCount, int transactionCount, int budgetCount);

signals:
    // stage is a human-readable label ("Accounts", "Transactions", ...);
    // current/total is progress within that stage, not across the whole run.
    void progress(const QString& stage, int current, int total);
    void resultReady(const GeneratedDataset& dataset);
};
