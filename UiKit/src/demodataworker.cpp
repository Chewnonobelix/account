#include "demodataworker.h"

#include "demodatalog.h"

#include <QElapsedTimer>
#include <QRandomGenerator>

#include <algorithm>
#include <cmath>

using OpenAccountEnums::Frequency;
using OpenAccountEnums::Movement;
using OpenAccountEnums::Support;

Q_LOGGING_CATEGORY(demoDataLog, "uikit.demodata")

namespace {

const QStringList kBanks = {"BNP Paribas", "Société Générale", "Crédit Agricole",
                             "La Banque Postale", "LCL",           "Boursorama",
                             "Revolut",     "N26",               "ING", "HSBC"};

const QStringList kAccountKinds = {"Compte Courant", "Livret A", "LDDS", "Compte Joint",
                                    "Compte Pro", "PEL", "Compte Épargne"};

const QStringList kFirstNames = {"Alice", "Bruno",  "Chloé",  "David", "Emma",
                                  "Farid", "Gaëlle", "Hugo",  "Inès",  "Julien",
                                  "Karim", "Laura",  "Marc",  "Nadia", "Oscar"};

const QStringList kLastNames = {"Martin",  "Bernard", "Durand",  "Petit",  "Robert",
                                 "Richard", "Moreau",  "Simon",   "Laurent", "Lefebvre",
                                 "Leroy",   "Roux",    "Fournier", "Girard", "Bonnet"};

const QStringList kCreditCategories = {"Salaire", "Freelance", "Remboursement", "Intérêts",
                                        "Vente occasion", "Prime", "Cadeau reçu"};

const QStringList kDebitCategories = {"Loyer",     "Courses",     "Transport",  "Restaurant",
                                       "Loisirs",   "Santé",       "Assurance",  "Abonnements",
                                       "Voyages",   "Cadeaux",     "Impôts",     "Énergie",
                                       "Téléphonie", "Équipement", "Divers"};

const QStringList kMerchants = {"Carrefour", "Amazon", "SNCF", "EDF", "Free Mobile",
                                 "Fnac",      "Decathlon", "Uber", "Deliveroo", "Airbnb",
                                 "Leroy Merlin", "Pharmacie", "Boulangerie", "Netflix", "Spotify"};

// QRandomGenerator::bounded() overloads on int/quint32/double; QList/
// QStringList::size() returns qsizetype, which is ambiguous against those
// (equally "narrowing" to more than one of them), so every lookup funnels
// through here instead of calling bounded(container.size()) directly.
int randomIndex(QRandomGenerator& rng, qsizetype size) {
    return rng.bounded(static_cast<int>(size));
}

QString pooledName(const QStringList& pool, int index) {
    const int poolSize = static_cast<int>(pool.size());
    const QString base = pool.at(index % poolSize);
    const int cycle = index / poolSize;
    return cycle == 0 ? base : QStringLiteral("%1 %2").arg(base).arg(cycle + 1);
}

QDate randomDate(QRandomGenerator& rng, QDate from, QDate to) {
    const int spanDays = static_cast<int>(from.daysTo(to));
    if (spanDays <= 0)
        return from;
    return from.addDays(rng.bounded(spanDays + 1));
}

double randomAmount(QRandomGenerator& rng, double min, double max) {
    return std::round((min + rng.generateDouble() * (max - min)) * 100.0) / 100.0;
}

// Caps how often a stage reports back: one message per row would both
// flood the log and flood the queued connection back to the GUI thread for
// a 25000-transaction run.
int progressStep(int total) { return std::max(1, total / 50); }

} // namespace

DemoDataWorker::DemoDataWorker(QObject* parent) : QObject(parent) {}

void DemoDataWorker::run(int accountCount, int categoryCount, int profileCount, int transactionCount,
                          int budgetCount) {
    QElapsedTimer timer;
    timer.start();

    accountCount = std::max(accountCount, 1);
    categoryCount = std::max(categoryCount, 1);
    profileCount = std::max(profileCount, 1);
    transactionCount = std::max(transactionCount, 0);
    budgetCount = std::max(budgetCount, 0);

    qCInfo(demoDataLog) << "Generating" << accountCount << "accounts," << categoryCount << "categories,"
                         << profileCount << "profiles," << transactionCount << "transactions," << budgetCount
                         << "budgets...";

    // Fixed seed: reruns produce the same dataset, so a perf regression
    // spotted against this data stays comparable across sessions.
    QRandomGenerator rng(42);

    const QDate today = QDate::currentDate();
    const QDate accountsFrom = today.addYears(-10);
    const QDate transactionsFrom = today.addYears(-3);

    GeneratedDataset dataset;

    dataset.accounts.reserve(accountCount);
    const int accountStep = progressStep(accountCount);
    for (int i = 0; i < accountCount; ++i) {
        GeneratedAccount account;
        account.id = QUuid::createUuid();
        account.number = pooledName(kAccountKinds, i) + QStringLiteral(" %1").arg(i + 1);
        account.bank = kBanks.at(randomIndex(rng, kBanks.size()));
        account.interest = randomAmount(rng, 0.0, 3.5);
        account.description = QStringLiteral("Demo account #%1").arg(i + 1);
        account.opening = randomDate(rng, accountsFrom, today);
        dataset.accounts.append(account);

        if ((i + 1) % accountStep == 0 || i + 1 == accountCount) {
            qCInfo(demoDataLog) << "Accounts:" << (i + 1) << "/" << accountCount;
            emit progress(QStringLiteral("Accounts"), i + 1, accountCount);
        }
    }

    dataset.categories.reserve(categoryCount);
    const int categoryStep = progressStep(categoryCount);
    for (int i = 0; i < categoryCount; ++i) {
        // Roughly one credit category for every four debit ones, in line
        // with a typical household's mostly-outgoing category list.
        const bool credit = rng.bounded(4) == 0;
        GeneratedCategory category;
        category.id = QUuid::createUuid();
        category.name = pooledName(credit ? kCreditCategories : kDebitCategories, i);
        category.direction = credit ? Movement::Credit : Movement::Debit;
        dataset.categories.append(category);

        if ((i + 1) % categoryStep == 0 || i + 1 == categoryCount) {
            qCInfo(demoDataLog) << "Categories:" << (i + 1) << "/" << categoryCount;
            emit progress(QStringLiteral("Categories"), i + 1, categoryCount);
        }
    }

    dataset.profiles.reserve(profileCount);
    const int profileStep = progressStep(profileCount);
    for (int i = 0; i < profileCount; ++i) {
        GeneratedProfile profile;
        profile.id = QUuid::createUuid();
        profile.firstName = kFirstNames.at(randomIndex(rng, kFirstNames.size()));
        profile.lastName = kLastNames.at(randomIndex(rng, kLastNames.size()));

        const int ownedAccounts = 1 + rng.bounded(3);
        for (int a = 0; a < ownedAccounts; ++a)
            profile.accounts.append(dataset.accounts.at(randomIndex(rng, dataset.accounts.size())).id);

        dataset.profiles.append(profile);

        if ((i + 1) % profileStep == 0 || i + 1 == profileCount) {
            qCInfo(demoDataLog) << "Profiles:" << (i + 1) << "/" << profileCount;
            emit progress(QStringLiteral("Profiles"), i + 1, profileCount);
        }
    }

    const QList<Support> supports = {Support::Cash, Support::Cheque, Support::Card,
                                      Support::Transfer, Support::Other};

    dataset.transactions.reserve(transactionCount);
    const int transactionStep = progressStep(transactionCount);
    for (int i = 0; i < transactionCount; ++i) {
        const GeneratedAccount& account = dataset.accounts.at(randomIndex(rng, dataset.accounts.size()));
        const GeneratedCategory& category = dataset.categories.at(randomIndex(rng, dataset.categories.size()));
        const bool credit = category.direction == Movement::Credit;

        GeneratedTransaction transaction;
        transaction.id = QUuid::createUuid();
        transaction.name = category.name;
        transaction.description =
            QStringLiteral("%1 — %2").arg(category.name, kMerchants.at(randomIndex(rng, kMerchants.size())));
        transaction.value = credit ? randomAmount(rng, 20.0, 3000.0) : randomAmount(rng, 2.0, 1200.0);
        transaction.movement = credit ? Movement::Credit : Movement::Debit;
        transaction.support = supports.at(randomIndex(rng, supports.size()));
        transaction.date = randomDate(rng, transactionsFrom, today);
        transaction.accountId = account.id;
        transaction.category = category.id;
        dataset.transactions.append(transaction);

        if ((i + 1) % transactionStep == 0 || i + 1 == transactionCount) {
            qCInfo(demoDataLog) << "Transactions:" << (i + 1) << "/" << transactionCount;
            emit progress(QStringLiteral("Transactions"), i + 1, transactionCount);
        }
    }

    const QList<Frequency> recurrences = {Frequency::Weekly, Frequency::Monthly, Frequency::Quarterly,
                                           Frequency::Yearly};

    QList<GeneratedCategory> debitCategories;
    for (const GeneratedCategory& category : std::as_const(dataset.categories))
        if (category.direction == Movement::Debit)
            debitCategories.append(category);
    if (debitCategories.isEmpty())
        debitCategories = dataset.categories;

    dataset.budgets.reserve(budgetCount);
    const int budgetStep = progressStep(budgetCount);
    for (int i = 0; i < budgetCount; ++i) {
        const QDate start = randomDate(rng, transactionsFrom, today);

        GeneratedBudget budget;
        budget.id = QUuid::createUuid();
        budget.name = QStringLiteral("Budget #%1").arg(i + 1);
        budget.categoryId = debitCategories.at(randomIndex(rng, debitCategories.size())).id;
        budget.threshold = randomAmount(rng, 50.0, 1500.0);
        budget.startDate = start;
        budget.recurrence = recurrences.at(randomIndex(rng, recurrences.size()));
        budget.endDate = start.addDays(static_cast<int>(budget.recurrence));
        dataset.budgets.append(budget);

        if (budgetCount > 0 && ((i + 1) % budgetStep == 0 || i + 1 == budgetCount)) {
            qCInfo(demoDataLog) << "Budgets:" << (i + 1) << "/" << budgetCount;
            emit progress(QStringLiteral("Budgets"), i + 1, budgetCount);
        }
    }

    qCInfo(demoDataLog) << "Generation finished in" << timer.elapsed() << "ms";
    emit resultReady(dataset);
}
