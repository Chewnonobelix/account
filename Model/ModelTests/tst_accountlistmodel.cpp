#include "../include/Model/Business/account.h"
#include "../include/Model/Models/accountlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestAccountListModel : public QObject {
  Q_OBJECT

private:
  static AccountPtr makeAccount(const QString &number, const QString &bank) {
    AccountPtr account = AccountPtr::create();
    account->setId(QUuid::createUuid());
    account->setNumber(number);
    account->setBank(bank);
    return account;
  }

private slots:
  void defaultState_isEmpty() {
    AccountListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.accounts().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addAccount_exposesNumberAndBank() {
    AccountListModel model;

    const AccountPtr account = makeAccount(QStringLiteral("FR1234"), QStringLiteral("Bank A"));

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &AccountListModel::countChanged);

    QVERIFY(model.addAccount(account));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), AccountListModel::NumberRole).toString(),
             QStringLiteral("FR1234"));
    QCOMPARE(model.data(model.index(0, 0), AccountListModel::BankRole).toString(),
             QStringLiteral("Bank A"));
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), AccountListModel::AccountRole)),
             static_cast<QObject *>(account.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("number")).toString(), QStringLiteral("FR1234"));
  }

  void duplicateAccounts_areRejected() {
    AccountListModel model;
    const AccountPtr account = makeAccount(QStringLiteral("FR0001"), QStringLiteral("Bank A"));

    QVERIFY(model.addAccount(account));
    QVERIFY(!model.addAccount(account));

    const AccountPtr duplicateId = makeAccount(QStringLiteral("FR0002"), QStringLiteral("Bank B"));
    duplicateId->setId(account->id());
    QVERIFY(!model.addAccount(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void accountChanges_emitDataChanged() {
    AccountListModel model;
    const AccountPtr account = makeAccount(QStringLiteral("FR0001"), QStringLiteral("Bank A"));
    QVERIFY(model.addAccount(account));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    account->setNumber(QStringLiteral("FR9999"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(AccountListModel::NumberRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsAccountById() {
    AccountListModel model;
    const AccountPtr first = makeAccount(QStringLiteral("A"), QStringLiteral("Bank"));
    const AccountPtr second = makeAccount(QStringLiteral("B"), QStringLiteral("Bank"));
    model.setAccounts({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeAccount_shrinksModel() {
    AccountListModel model;
    const AccountPtr account = makeAccount(QStringLiteral("FR0001"), QStringLiteral("Bank A"));
    QVERIFY(model.addAccount(account));

    QSignalSpy countSpy(&model, &AccountListModel::countChanged);

    QVERIFY(model.removeAccount(account->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeAccount(account->id()));
  }

  void setAccounts_replacesContentAndSkipsInvalidEntries() {
    AccountListModel model;

    const AccountPtr first = makeAccount(QStringLiteral("A"), QStringLiteral("Bank"));
    const AccountPtr second = makeAccount(QStringLiteral("B"), QStringLiteral("Bank"));
    const AccountPtr duplicate = makeAccount(QStringLiteral("C"), QStringLiteral("Bank"));
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &AccountListModel::countChanged);

    model.setAccounts({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryAccount() {
    AccountListModel model;
    model.setAccounts({makeAccount(QStringLiteral("A"), QStringLiteral("Bank")),
                        makeAccount(QStringLiteral("B"), QStringLiteral("Bank"))});

    QSignalSpy countSpy(&model, &AccountListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestAccountListModel)
#include "tst_accountlistmodel.moc"
