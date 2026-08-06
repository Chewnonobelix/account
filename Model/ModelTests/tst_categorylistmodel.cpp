#include "../include/Model/Business/category.h"
#include "../include/Model/Models/categorylistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestCategoryListModel : public QObject {
  Q_OBJECT

private:
  static CategoryPtr makeCategory(const QString &name, OpenAccountEnums::Movement direction) {
    // Unlike Transaction, Category doesn't self-assign an id in its
    // constructor (ids are expected to come from whoever persists it), so
    // tests must set one explicitly to exercise id-based lookups.
    CategoryPtr category = CategoryPtr::create();
    category->setId(QUuid::createUuid());
    category->setName(name);
    category->setDirection(direction);
    return category;
  }

private slots:
  void defaultState_isEmpty() {
    CategoryListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.categories().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addCategory_exposesNameAndDirection() {
    CategoryListModel model;

    const CategoryPtr category =
        makeCategory(QStringLiteral("Groceries"), OpenAccountEnums::Movement::Debit);

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &CategoryListModel::countChanged);

    QVERIFY(model.addCategory(category));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), CategoryListModel::NameRole).toString(),
             QStringLiteral("Groceries"));
    QCOMPARE(model.data(model.index(0, 0), CategoryListModel::DirectionRole)
                 .value<OpenAccountEnums::Movement>(),
             OpenAccountEnums::Movement::Debit);
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), CategoryListModel::CategoryRole)),
             static_cast<QObject *>(category.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("name")).toString(), QStringLiteral("Groceries"));
  }

  void duplicateCategories_areRejected() {
    CategoryListModel model;
    const CategoryPtr category =
        makeCategory(QStringLiteral("Salary"), OpenAccountEnums::Movement::Credit);

    QVERIFY(model.addCategory(category));
    QVERIFY(!model.addCategory(category));

    const CategoryPtr duplicateId =
        makeCategory(QStringLiteral("Salary copy"), OpenAccountEnums::Movement::Credit);
    duplicateId->setId(category->id());
    QVERIFY(!model.addCategory(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void categoryChanges_emitDataChanged() {
    CategoryListModel model;
    const CategoryPtr category =
        makeCategory(QStringLiteral("Initial"), OpenAccountEnums::Movement::Both);
    QVERIFY(model.addCategory(category));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    category->setName(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(CategoryListModel::NameRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsCategoryById() {
    CategoryListModel model;
    const CategoryPtr first = makeCategory(QStringLiteral("A"), OpenAccountEnums::Movement::Both);
    const CategoryPtr second = makeCategory(QStringLiteral("B"), OpenAccountEnums::Movement::Both);
    model.setCategories({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeCategory_shrinksModel() {
    CategoryListModel model;
    const CategoryPtr category = makeCategory(QStringLiteral("Rent"), OpenAccountEnums::Movement::Debit);
    QVERIFY(model.addCategory(category));

    QSignalSpy countSpy(&model, &CategoryListModel::countChanged);

    QVERIFY(model.removeCategory(category->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeCategory(category->id()));
  }

  void setCategories_replacesContentAndSkipsInvalidEntries() {
    CategoryListModel model;

    const CategoryPtr first = makeCategory(QStringLiteral("A"), OpenAccountEnums::Movement::Both);
    const CategoryPtr second = makeCategory(QStringLiteral("B"), OpenAccountEnums::Movement::Both);
    const CategoryPtr duplicate = makeCategory(QStringLiteral("C"), OpenAccountEnums::Movement::Both);
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &CategoryListModel::countChanged);

    model.setCategories({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryCategory() {
    CategoryListModel model;
    model.setCategories({makeCategory(QStringLiteral("A"), OpenAccountEnums::Movement::Both),
                          makeCategory(QStringLiteral("B"), OpenAccountEnums::Movement::Both)});

    QSignalSpy countSpy(&model, &CategoryListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestCategoryListModel)
#include "tst_categorylistmodel.moc"
