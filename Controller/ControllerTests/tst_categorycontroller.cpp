#include <QtTest>

#include "Controller/categorycontroller.h"
#include "Controller/inmemorystoragelayer.h"

using namespace Controller;

class CategoryControllerTests : public QObject {
	Q_OBJECT
private slots:
	void isAnAbstractController();
	void saveThenLoad_roundTrips();
	void loadAll_returnsEveryCategory();
	void remove_deletesTheCategory();
};

void CategoryControllerTests::isAnAbstractController() {
	InMemoryStorageLayer layer;
	CategoryController controller(layer);
	QVERIFY(qobject_cast<AbstractController *>(&controller) != nullptr);
}

void CategoryControllerTests::saveThenLoad_roundTrips() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	CategoryController controller(layer);

	const CategoryPtr category = CategoryPtr::create();
	category->setId(QUuid::createUuid());
	category->setName(QStringLiteral("Groceries"));
	category->setDirection(OpenAccountEnums::Movement::Debit);
	QVERIFY(controller.save(category));
	QVERIFY(controller.exists(category->id()));

	const CategoryPtr reloaded = controller.load(category->id());
	QVERIFY(reloaded);
	QCOMPARE(reloaded->name(), QStringLiteral("Groceries"));
	QCOMPARE(reloaded->direction(), OpenAccountEnums::Movement::Debit);
}

void CategoryControllerTests::loadAll_returnsEveryCategory() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	CategoryController controller(layer);

	const CategoryPtr first = CategoryPtr::create();
	first->setId(QUuid::createUuid());
	const CategoryPtr second = CategoryPtr::create();
	second->setId(QUuid::createUuid());
	QVERIFY(controller.save(first));
	QVERIFY(controller.save(second));

	QCOMPARE(controller.loadAll().size(), 2);
}

void CategoryControllerTests::remove_deletesTheCategory() {
	InMemoryStorageLayer layer;
	QVERIFY(layer.open());
	CategoryController controller(layer);

	const CategoryPtr category = CategoryPtr::create();
	category->setId(QUuid::createUuid());
	QVERIFY(controller.save(category));
	QVERIFY(controller.remove(category->id()));
	QVERIFY(!controller.exists(category->id()));
}

QTEST_APPLESS_MAIN(CategoryControllerTests)

#include "tst_categorycontroller.moc"
