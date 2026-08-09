#include "../include/Model/Business/account.h"
#include "../include/Model/Business/profile.h"
#include "../include/Model/Models/accountlistmodel.h"
#include "../include/Model/Models/profileaccountfilterproxymodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestProfileAccountFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static AccountPtr makeAccount(const QString &number) {
    AccountPtr account = AccountPtr::create();
    account->setId(QUuid::createUuid());
    account->setNumber(number);
    return account;
  }

private slots:
  void withoutProfile_showsNothing() {
    AccountListModel sourceModel;
    QVERIFY(sourceModel.addAccount(makeAccount(QStringLiteral("A"))));

    ProfileAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 0);
  }

  void setProfile_filtersAccountsByProfileAccountIds() {
    const AccountPtr owned = makeAccount(QStringLiteral("Owned"));
    const AccountPtr other = makeAccount(QStringLiteral("Other"));

    AccountListModel sourceModel;
    QVERIFY(sourceModel.addAccount(owned));
    QVERIFY(sourceModel.addAccount(other));

    Profile profile;
    profile.setAccounts({owned->id()});

    ProfileAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setProfile(&profile);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(owned.data()));
    QCOMPARE(proxy.get(0).value(QStringLiteral("number")).toString(),
             QStringLiteral("Owned"));
  }

  void jointAccount_isVisibleThroughBothProfiles() {
    const AccountPtr joint = makeAccount(QStringLiteral("Joint"));

    AccountListModel sourceModel;
    QVERIFY(sourceModel.addAccount(joint));

    Profile first;
    first.setAccounts({joint->id()});
    Profile second;
    second.setAccounts({joint->id()});

    ProfileAccountFilterProxyModel firstProxy;
    firstProxy.setSourceModel(&sourceModel);
    firstProxy.setProfile(&first);

    ProfileAccountFilterProxyModel secondProxy;
    secondProxy.setSourceModel(&sourceModel);
    secondProxy.setProfile(&second);

    QCOMPARE(firstProxy.rowCount(), 1);
    QCOMPARE(secondProxy.rowCount(), 1);
    QCOMPARE(firstProxy.at(0), secondProxy.at(0));
  }

  void profileAccountsChange_reappliesTheFilter() {
    const AccountPtr first = makeAccount(QStringLiteral("First"));
    const AccountPtr second = makeAccount(QStringLiteral("Second"));

    AccountListModel sourceModel;
    QVERIFY(sourceModel.addAccount(first));
    QVERIFY(sourceModel.addAccount(second));

    Profile profile;
    profile.setAccounts({first->id()});

    ProfileAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setProfile(&profile);

    QSignalSpy rowsInsertedSpy(&proxy, &QAbstractItemModel::rowsInserted);
    profile.setAccounts({first->id(), second->id()});

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(proxy.rowCount(), 2);
  }
};

QTEST_MAIN(TestProfileAccountFilterProxyModel)
#include "tst_profileaccountfilterproxymodel.moc"
