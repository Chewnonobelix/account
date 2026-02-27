#include "../include/Model/model.h"
#include <QtTest>

class ModelTests : public QObject {
  Q_OBJECT
private slots:
  void name_returns_Model();
};

void ModelTests::name_returns_Model() {
  Model::ModelApi api;
  QCOMPARE(api.name(), QStringLiteral("OpenAccount Model"));
}

#include "tst_model.moc"
