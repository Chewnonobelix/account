#include "../include/Model/Business/account.h"
#include "../include/Model/Business/frequency.h"
#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/frequencyaccountfilterproxymodel.h"
#include "../include/Model/Models/frequencylistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestFrequencyAccountFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static FrequencyPtr makeFrequency(const QUuid &accountId) {
    FrequencyPtr frequency = FrequencyPtr::create();
    frequency->setId(QUuid::createUuid());

    TransactionPtr prototype = TransactionPtr::create();
    prototype->setAccountId(accountId);
    frequency->setPrototype(prototype);

    return frequency;
  }

private slots:
  void withoutAccount_showsEverything() {
    FrequencyListModel sourceModel;
    QVERIFY(sourceModel.addFrequency(makeFrequency(QUuid::createUuid())));
    QVERIFY(sourceModel.addFrequency(makeFrequency(QUuid::createUuid())));

    FrequencyAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 2);
  }

  void setAccount_filtersFrequenciesByPrototypeAccountId() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const FrequencyPtr selected = makeFrequency(selectedAccountId);
    const FrequencyPtr other = makeFrequency(QUuid::createUuid());

    FrequencyListModel sourceModel;
    QVERIFY(sourceModel.addFrequency(selected));
    QVERIFY(sourceModel.addFrequency(other));

    FrequencyAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(selected.data()));
  }

  void frequencyWithoutPrototype_isExcludedWhenFilteringByAccount() {
    Account account;
    account.setId(QUuid::createUuid());

    FrequencyListModel sourceModel;
    QVERIFY(sourceModel.addFrequency(FrequencyPtr::create()));

    FrequencyAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QCOMPARE(proxy.rowCount(), 0);
  }

  void replacingPrototype_reappliesTheFilter() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const FrequencyPtr frequency = makeFrequency(QUuid::createUuid());

    FrequencyListModel sourceModel;
    QVERIFY(sourceModel.addFrequency(frequency));

    FrequencyAccountFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);
    QCOMPARE(proxy.rowCount(), 0);

    TransactionPtr matchingPrototype = TransactionPtr::create();
    matchingPrototype->setAccountId(selectedAccountId);
    frequency->setPrototype(matchingPrototype);

    QCOMPARE(proxy.rowCount(), 1);
  }
};

QTEST_MAIN(TestFrequencyAccountFilterProxyModel)
#include "tst_frequencyaccountfilterproxymodel.moc"
