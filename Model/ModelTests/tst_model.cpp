#include "../include/Model/model.h"
#include <QtTest>

class TestModel : public QObject {
  Q_OBJECT
private slots:
  void name_returns_Model();
};

void TestModel::name_returns_Model() {
  Model::ModelApi api;
  QCOMPARE(api.name(), QStringLiteral("OpenAccount Model"));
}

QTEST_MAIN(TestModel)

#include "tst_model.moc"
