#pragma once

#include <QObject>
#include <QPointer>
#include <QString>
#include <QThread>

class AccountListModel;
class CategoryListModel;
class ProfileListModel;
class TransactionListModel;
class BudgetListModel;
class DemoDataWorker;
struct GeneratedDataset;

// Bulk-generates a large, randomized dataset for the shared UiKit demo
// models. Components that only reveal their real weight under load
// (OBTransactionList's Pager, OBCalendar, the Budget page's Details table,
// OBComboBox filters, ...) can't be meaningfully perf-tested against the
// handful of hand-written rows Main.qml seeds by default, and hand-writing
// thousands of QML object literals to get there isn't practical.
//
// generate() returns immediately: the actual randomization work runs on a
// dedicated background QThread (DemoDataWorker), reporting progress via the
// running/progressText/progressPercent properties (bindable from QML) and
// through the "uikit.demodata" logging category. Once the worker hands back
// its plain-data result, DemoDataGenerator builds the real QObject-derived
// business entities and applies them to the models with a single setXxx()
// call each — all on the GUI thread, where those models and entities live.
class DemoDataGenerator : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(QString progressText READ progressText NOTIFY progressChanged)
    Q_PROPERTY(int progressPercent READ progressPercent NOTIFY progressChanged)

public:
    explicit DemoDataGenerator(QObject* parent = nullptr);
    ~DemoDataGenerator() override;

    bool isRunning() const { return m_running; }
    QString progressText() const { return m_progressText; }
    int progressPercent() const { return m_progressPercent; }

    // No-op (with a logged warning) if a run is already in progress.
    Q_INVOKABLE void generate(AccountListModel* accountModel, CategoryListModel* categoryModel,
                               ProfileListModel* profileModel, TransactionListModel* transactionModel,
                               BudgetListModel* budgetModel, int accountCount = 50, int categoryCount = 40,
                               int profileCount = 25, int transactionCount = 25000, int budgetCount = 200);

signals:
    void runningChanged();
    void progressChanged();
    void finished();

    // Internal: crosses onto the worker thread to kick off DemoDataWorker::run.
    void startRequested(int accountCount, int categoryCount, int profileCount, int transactionCount,
                         int budgetCount);

private slots:
    void onProgress(const QString& stage, int current, int total);
    void onResultReady(const GeneratedDataset& dataset);

private:
    QThread m_thread;
    DemoDataWorker* m_worker;

    bool m_running = false;
    QString m_progressText;
    int m_progressPercent = 0;

    QPointer<AccountListModel> m_accountModel;
    QPointer<CategoryListModel> m_categoryModel;
    QPointer<ProfileListModel> m_profileModel;
    QPointer<TransactionListModel> m_transactionModel;
    QPointer<BudgetListModel> m_budgetModel;
};
