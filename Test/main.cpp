#include <QApplication>
#include <QTest>
#include <QDebug>

#include "testinformation.h"
#include "testentry.h"
#include "testfrequency.h"
#include "testdebt.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    int status = 0;

    TestInformation ti;

    status |= QTest::qExec(&ti, argc, argv);

    TestEntry te;
    status |= QTest::qExec(&te, argc, argv);


    TestFrequency tf;
    status |= QTest::qExec(&tf, argc, argv);

    TestDebt td;
    status |= QTest::qExec(&td, argc, argv);

    return status;
}
