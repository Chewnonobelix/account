#include "../include/Model/Business/account.h"
#include "../include/Model/Business/debt.h"
#include "../include/Model/Models/debtaccountfilterproxymodel.h"
#include "../include/Model/Models/debtlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestDebtAccountFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static DebtPtr makeDebt(const QString &name, const QUuid &accountId) {
    DebtPtr debt = DebtPtr::create();
    debt->setId(QUuid::createUuid());
    debt->setName(name);
    debt->setAccountId(accountId);
    return debt;
  }

private slots:
  void withoutAccount_showsEverything() {
    DebtListModel sourceModel;
    QVERIFY(sourceModel.addDebt(makeDebt(QStringLiteral("A"), QUuid::createUuid())));
    QVERIFY(sourceModel.addDebt(makeDebt(QStringLiteral("B"), QUuid::createUuid())));

    DebtAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 2);
  }

  void setAccount_filtersDebtsByAccountId() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const DebtPtr selected = makeDebt(QStringLiteral("Selected"), selectedAccountId);
    const DebtPtr other = makeDebt(QStringLiteral("Other"), QUuid::createUuid());

    DebtListModel sourceModel;
    QVERIFY(sourceModel.addDebt(selected));
    QVERIFY(sourceModel.addDebt(other));

    DebtAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(selected.data()));
    QCOMPARE(proxy.get(0).value(QStringLiteral("accountId")).toUuid(), selectedAccountId);
  }

  void debtAccountIdChanges_reapplyTheFilter() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const DebtPtr first = makeDebt(QStringLiteral("Included"), selectedAccountId);
    const DebtPtr second = makeDebt(QStringLiteral("Hidden"), QUuid::createUuid());

    DebtListModel sourceModel;
    QVERIFY(sourceModel.addDebt(first));
    QVERIFY(sourceModel.addDebt(second));

    DebtAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QSignalSpy rowsRemovedSpy(&proxy, &QAbstractItemModel::rowsRemoved);
    QSignalSpy rowsInsertedSpy(&proxy, &QAbstractItemModel::rowsInserted);

    first->setAccountId(QUuid::createUuid());
    second->setAccountId(selectedAccountId);

    QVERIFY(rowsRemovedSpy.count() >= 1);
    QVERIFY(rowsInsertedSpy.count() >= 1);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(second.data()));
  }
};

QTEST_MAIN(TestDebtAccountFilterProxyModel)
#include "tst_debtaccountfilterproxymodel.moc"
