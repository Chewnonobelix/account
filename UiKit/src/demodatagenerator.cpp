#include "demodatagenerator.h"

#include "demodatalog.h"
#include "demodataset.h"
#include "demodataworker.h"

#include "Model/Business/account.h"
#include "Model/Business/budget.h"
#include "Model/Business/category.h"
#include "Model/Business/profile.h"
#include "Model/Business/transaction.h"
#include "Model/Models/accountlistmodel.h"
#include "Model/Models/budgetlistmodel.h"
#include "Model/Models/categorylistmodel.h"
#include "Model/Models/profilelistmodel.h"
#include "Model/Models/transactionlistmodel.h"

#include <QQmlEngine>

DemoDataGenerator::DemoDataGenerator(QObject* parent) : QObject(parent), m_worker(new DemoDataWorker) {
    qRegisterMetaType<GeneratedDataset>("GeneratedDataset");

    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &DemoDataGenerator::startRequested, m_worker, &DemoDataWorker::run);
    connect(m_worker, &DemoDataWorker::progress, this, &DemoDataGenerator::onProgress);
    connect(m_worker, &DemoDataWorker::resultReady, this, &DemoDataGenerator::onResultReady);
    m_thread.start();
}

DemoDataGenerator::~DemoDataGenerator() {
    m_thread.quit();
    m_thread.wait();
}

void DemoDataGenerator::generate(AccountListModel* accountModel, CategoryListModel* categoryModel,
                                  ProfileListModel* profileModel, TransactionListModel* transactionModel,
                                  BudgetListModel* budgetModel, int accountCount, int categoryCount,
                                  int profileCount, int transactionCount, int budgetCount) {
    if (!accountModel || !categoryModel || !profileModel || !transactionModel || !budgetModel)
        return;

    if (m_running) {
        qCWarning(demoDataLog) << "generate() called while a run is already in progress; ignoring";
        return;
    }

    m_accountModel = accountModel;
    m_categoryModel = categoryModel;
    m_profileModel = profileModel;
    m_transactionModel = transactionModel;
    m_budgetModel = budgetModel;

    m_running = true;
    emit runningChanged();
    m_progressText = QStringLiteral("Starting…");
    m_progressPercent = 0;
    emit progressChanged();

    emit startRequested(accountCount, categoryCount, profileCount, transactionCount, budgetCount);
}

void DemoDataGenerator::onProgress(const QString& stage, int current, int total) {
    m_progressText = QStringLiteral("%1: %2 / %3").arg(stage).arg(current).arg(total);
    m_progressPercent = total > 0 ? static_cast<int>(100.0 * current / total) : 100;
    emit progressChanged();
}

void DemoDataGenerator::onResultReady(const GeneratedDataset& dataset) {
    qCInfo(demoDataLog) << "Applying generated dataset to models...";

    if (m_accountModel) {
        QList<AccountPtr> accounts;
        accounts.reserve(dataset.accounts.size());
        for (const GeneratedAccount& a : dataset.accounts) {
            auto account = QSharedPointer<Account>::create();
            // These are parent-less QObjects owned by the QSharedPointer,
            // not by QML: without this, the moment QML/JS touches one
            // (e.g. through the account filter combo below), the engine
            // defaults an unparented QObject returned from C++ to
            // JavaScriptOwnership and its GC eventually deleteLater()s it
            // out from under the shared pointer — a double free once both
            // "owners" try to destroy the same object.
            QQmlEngine::setObjectOwnership(account.data(), QQmlEngine::CppOwnership);
            account->setId(a.id);
            account->setNumber(a.number);
            account->setBank(a.bank);
            account->setInterest(a.interest);
            account->setDescription(a.description);
            account->setOpening(a.opening);
            accounts.append(account);
        }
        m_accountModel->setAccounts(accounts);
    }

    if (m_categoryModel) {
        QList<CategoryPtr> categories;
        categories.reserve(dataset.categories.size());
        for (const GeneratedCategory& c : dataset.categories) {
            auto category = QSharedPointer<Category>::create();
            QQmlEngine::setObjectOwnership(category.data(), QQmlEngine::CppOwnership);
            category->setId(c.id);
            category->setName(c.name);
            category->setDirection(c.direction);
            categories.append(category);
        }
        m_categoryModel->setCategories(categories);
    }

    if (m_profileModel) {
        QList<ProfilePtr> profiles;
        profiles.reserve(dataset.profiles.size());
        for (const GeneratedProfile& p : dataset.profiles) {
            auto profile = QSharedPointer<Profile>::create();
            QQmlEngine::setObjectOwnership(profile.data(), QQmlEngine::CppOwnership);
            profile->setId(p.id);
            profile->setFirstName(p.firstName);
            profile->setLastName(p.lastName);
            profile->setAccounts(p.accounts);
            profiles.append(profile);
        }
        m_profileModel->setProfiles(profiles);
    }

    if (m_transactionModel) {
        QList<TransactionPtr> transactions;
        transactions.reserve(dataset.transactions.size());
        for (const GeneratedTransaction& t : dataset.transactions) {
            auto transaction = QSharedPointer<Transaction>::create();
            QQmlEngine::setObjectOwnership(transaction.data(), QQmlEngine::CppOwnership);
            transaction->setId(t.id);
            transaction->setName(t.name);
            transaction->setDescription(t.description);
            transaction->setValue(t.value);
            transaction->setMovement(t.movement);
            transaction->setSupport(t.support);
            transaction->setDate(t.date);
            transaction->setAccountId(t.accountId);
            transaction->setCategory(t.category);
            transactions.append(transaction);
        }
        m_transactionModel->setTransactions(transactions);
    }

    if (m_budgetModel) {
        QList<BudgetPtr> budgets;
        budgets.reserve(dataset.budgets.size());
        for (const GeneratedBudget& b : dataset.budgets) {
            auto budget = QSharedPointer<Budget>::create();
            QQmlEngine::setObjectOwnership(budget.data(), QQmlEngine::CppOwnership);
            budget->setId(b.id);
            budget->setName(b.name);
            budget->setCategoryId(b.categoryId);
            budget->setThreshold(b.threshold);
            budget->setStartDate(b.startDate);
            budget->setEndDate(b.endDate);
            budget->setRecurrence(b.recurrence);
            budgets.append(budget);
        }
        m_budgetModel->setBudgets(budgets);
    }

    qCInfo(demoDataLog) << "Applied generated dataset to models:" << dataset.accounts.size() << "accounts,"
                         << dataset.categories.size() << "categories," << dataset.profiles.size() << "profiles,"
                         << dataset.transactions.size() << "transactions," << dataset.budgets.size() << "budgets";

    m_running = false;
    emit runningChanged();
    m_progressText = QStringLiteral("Done");
    m_progressPercent = 100;
    emit progressChanged();

    emit finished();
}
