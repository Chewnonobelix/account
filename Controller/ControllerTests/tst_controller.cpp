#include <QtTest>
#include "Controller/controller.h"

class ControllerTests : public QObject {
	Q_OBJECT
private slots:
	void name_returns_Controller();
};

void ControllerTests::name_returns_Controller() {
	Controller::ControllerApi api;
	QCOMPARE(api.name(), QStringLiteral("OpenAccount Controller"));
}

QTEST_APPLESS_MAIN(ControllerTests)

#include "tst_controller.moc"


