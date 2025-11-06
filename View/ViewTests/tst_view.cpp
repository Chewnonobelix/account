#include <QtTest>
#include "View/view.h"

class ViewTests : public QObject {
	Q_OBJECT
private slots:
	void name_returns_View();
};

void ViewTests::name_returns_View() {
	View::ViewApi api;
	QCOMPARE(api.name(), QStringLiteral("OpenAccount View"));
}

QTEST_APPLESS_MAIN(ViewTests)

#include "tst_view.moc"


