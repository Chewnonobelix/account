#include "../include/Model/Business/category.h"
#include "../include/Model/Models/categorylistmodel.h"
#include "../include/Model/Models/pagefilterproxymodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestPageFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static CategoryPtr makeCategory(const QString &name) {
    CategoryPtr category = CategoryPtr::create();
    category->setId(QUuid::createUuid());
    category->setName(name);
    return category;
  }

  static CategoryListModel *makeSourceWithRows(QObject *parent, int rowCount) {
    auto *model = new CategoryListModel(parent);
    for (int i = 0; i < rowCount; ++i)
      model->addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));
    return model;
  }

private slots:
  void defaultState_singlePageNoSource() {
    PageFilterProxyModel proxy;

    QCOMPARE(proxy.pageSize(), 100);
    QCOMPARE(proxy.currentPage(), 1);
    QCOMPARE(proxy.pageCount(), 1);
    QCOMPARE(proxy.rowCount(), 0);
  }

  void fewerRowsThanPageSize_showsEverythingOnOnePage() {
    CategoryListModel source;
    for (int i = 0; i < 5; ++i)
      source.addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));

    PageFilterProxyModel proxy;
    proxy.setSourceModel(&source);

    QCOMPARE(proxy.pageCount(), 1);
    QCOMPARE(proxy.rowCount(), 5);
  }

  void moreRowsThanPageSize_splitsIntoPages() {
    CategoryListModel source;
    for (int i = 0; i < 250; ++i)
      source.addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));

    PageFilterProxyModel proxy;
    proxy.setPageSize(100);
    proxy.setSourceModel(&source);

    QCOMPARE(proxy.pageCount(), 3);
    QCOMPARE(proxy.rowCount(), 100);

    proxy.setCurrentPage(2);
    QCOMPARE(proxy.rowCount(), 100);
    QCOMPARE(proxy.data(proxy.index(0, 0), CategoryListModel::NameRole).toString(),
             QStringLiteral("Item 100"));

    proxy.setCurrentPage(3);
    QCOMPARE(proxy.rowCount(), 50);
    QCOMPARE(proxy.data(proxy.index(0, 0), CategoryListModel::NameRole).toString(),
             QStringLiteral("Item 200"));
  }

  void setCurrentPage_clampsToValidRange() {
    CategoryListModel source;
    for (int i = 0; i < 150; ++i)
      source.addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));

    PageFilterProxyModel proxy;
    proxy.setPageSize(100);
    proxy.setSourceModel(&source);

    proxy.setCurrentPage(50);
    QCOMPARE(proxy.currentPage(), 2);

    proxy.setCurrentPage(-5);
    QCOMPARE(proxy.currentPage(), 1);
  }

  void sourceShrinking_clampsCurrentPageAndReapplies() {
    CategoryListModel source;
    for (int i = 0; i < 250; ++i)
      source.addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));

    PageFilterProxyModel proxy;
    proxy.setPageSize(100);
    proxy.setSourceModel(&source);
    proxy.setCurrentPage(3);
    QCOMPARE(proxy.currentPage(), 3);

    QSignalSpy currentPageSpy(&proxy, &PageFilterProxyModel::currentPageChanged);
    QSignalSpy pageCountSpy(&proxy, &PageFilterProxyModel::pageCountChanged);

    source.setCategories({});
    QTRY_COMPARE(proxy.pageCount(), 1);

    QCOMPARE(proxy.currentPage(), 1);
    QVERIFY(currentPageSpy.count() >= 1);
    QVERIFY(pageCountSpy.count() >= 1);
    QCOMPARE(proxy.rowCount(), 0);
  }

  void insertionBeforeCurrentPage_shiftsWindowCorrectly() {
    CategoryListModel source;
    for (int i = 0; i < 150; ++i)
      source.addCategory(makeCategory(QStringLiteral("Item %1").arg(i)));

    PageFilterProxyModel proxy;
    proxy.setPageSize(100);
    proxy.setSourceModel(&source);
    proxy.setCurrentPage(2);
    QCOMPARE(proxy.data(proxy.index(0, 0), CategoryListModel::NameRole).toString(),
             QStringLiteral("Item 100"));

    // Inserting a new first row shifts every existing row's absolute index
    // by one: what used to be "Item 99" (the last row of page 1) is now the
    // first row of page 2's window.
    CategoryPtr inserted = makeCategory(QStringLiteral("Inserted"));
    QList<CategoryPtr> next;
    next.append(inserted);
    next.append(source.categories());
    source.setCategories(next);

    QTRY_COMPARE(proxy.data(proxy.index(0, 0), CategoryListModel::NameRole).toString(),
                 QStringLiteral("Item 99"));
  }
};

QTEST_MAIN(TestPageFilterProxyModel)
#include "tst_pagefilterproxymodel.moc"
