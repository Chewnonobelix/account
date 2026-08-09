#include "../include/Model/Business/profile.h"
#include "../include/Model/Models/profilelistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestProfileListModel : public QObject {
  Q_OBJECT

private:
  static ProfilePtr makeProfile(const QString &firstName, const QString &lastName) {
    ProfilePtr profile = ProfilePtr::create();
    profile->setId(QUuid::createUuid());
    profile->setFirstName(firstName);
    profile->setLastName(lastName);
    return profile;
  }

private slots:
  void defaultState_isEmpty() {
    ProfileListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.profiles().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addProfile_exposesFirstAndLastName() {
    ProfileListModel model;

    const ProfilePtr profile = makeProfile(QStringLiteral("Ada"), QStringLiteral("Lovelace"));

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &ProfileListModel::countChanged);

    QVERIFY(model.addProfile(profile));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), ProfileListModel::FirstNameRole).toString(),
             QStringLiteral("Ada"));
    QCOMPARE(model.data(model.index(0, 0), ProfileListModel::LastNameRole).toString(),
             QStringLiteral("Lovelace"));
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), ProfileListModel::ProfileRole)),
             static_cast<QObject *>(profile.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("lastName")).toString(), QStringLiteral("Lovelace"));
  }

  void duplicateProfiles_areRejected() {
    ProfileListModel model;
    const ProfilePtr profile = makeProfile(QStringLiteral("Grace"), QStringLiteral("Hopper"));

    QVERIFY(model.addProfile(profile));
    QVERIFY(!model.addProfile(profile));

    const ProfilePtr duplicateId = makeProfile(QStringLiteral("Grace"), QStringLiteral("Hopper copy"));
    duplicateId->setId(profile->id());
    QVERIFY(!model.addProfile(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void profileChanges_emitDataChanged() {
    ProfileListModel model;
    const ProfilePtr profile = makeProfile(QStringLiteral("Initial"), QStringLiteral("Name"));
    QVERIFY(model.addProfile(profile));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    profile->setLastName(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(ProfileListModel::LastNameRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsProfileById() {
    ProfileListModel model;
    const ProfilePtr first = makeProfile(QStringLiteral("A"), QStringLiteral("A"));
    const ProfilePtr second = makeProfile(QStringLiteral("B"), QStringLiteral("B"));
    model.setProfiles({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeProfile_shrinksModel() {
    ProfileListModel model;
    const ProfilePtr profile = makeProfile(QStringLiteral("A"), QStringLiteral("A"));
    QVERIFY(model.addProfile(profile));

    QSignalSpy countSpy(&model, &ProfileListModel::countChanged);

    QVERIFY(model.removeProfile(profile->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeProfile(profile->id()));
  }

  void setProfiles_replacesContentAndSkipsInvalidEntries() {
    ProfileListModel model;

    const ProfilePtr first = makeProfile(QStringLiteral("A"), QStringLiteral("A"));
    const ProfilePtr second = makeProfile(QStringLiteral("B"), QStringLiteral("B"));
    const ProfilePtr duplicate = makeProfile(QStringLiteral("C"), QStringLiteral("C"));
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &ProfileListModel::countChanged);

    model.setProfiles({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryProfile() {
    ProfileListModel model;
    model.setProfiles({makeProfile(QStringLiteral("A"), QStringLiteral("A")),
                        makeProfile(QStringLiteral("B"), QStringLiteral("B"))});

    QSignalSpy countSpy(&model, &ProfileListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestProfileListModel)
#include "tst_profilelistmodel.moc"
